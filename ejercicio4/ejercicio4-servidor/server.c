#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include "estructuras.h"
#include "funcionesServidor.h"

///VARIABLES GLOBALES DEFINIDAS PARA SER MANEJADAS POR EL MAIN Y POR LOS HANDLERS DE SENALES.
///SEMAFOROS
sem_t* clientePuedeEscribir;
sem_t* clienteAServidor;
sem_t* servidorACliente;
///SHMID
int leerDeCliente;
int escribirACliente;
///ESTRUCTURAS DE BUFFERS
t_msgCliente* msgCliente;
t_msgServidor* msgServidor;
///PATH CON LA BASE DE DATOS
char bd[500];

///Funcion lanzada por el handler de senales que libera los semaforos y la memoria compartida.
void liberarRecursos()
{
    printf("\nCerrando semaforos y memoria compartida");
    ///Desvinculacion de las variables asociadas a memoria compartida.
    shmdt(&msgCliente);
    shmdt(&msgServidor);
    ///Liberacion de memoria compartida.
    shmctl(leerDeCliente, IPC_RMID, NULL);
    shmctl(escribirACliente, IPC_RMID, NULL);
    ///Cierre de semaforos.
    sem_close(clientePuedeEscribir);
    sem_close(clienteAServidor);
    sem_close(servidorACliente);
    ///Liberacion de semaforos.
    sem_unlink("/clientePuedeEscribir");
    sem_unlink("/clienteAServidor");
    sem_unlink("/servidorACliente");
    exit(5);
}

int main(int argc, char *argv[])
{
    ///Primero se comprueba que el archivo de la base de datos exista.
    if(access( BD, F_OK ) != -1)
        strcpy(bd, BD);
    else
    {
        printf("El archivo de la base de datos no existe");
        return 20;
    }

    ///Monitoreo si recibo las senales para finalizar el servidor.
    signal(SIGINT, liberarRecursos);
    signal(SIGTERM, liberarRecursos);

    ///SETEO DE VARIABLES GLOBALES
    //Semaforo a ser chequeado del lado del cliente para ver si puede escribir en la memoria compartida, es inicializado en uno, porque el cliente ya puede empezar a escribir.
    clientePuedeEscribir = sem_open("/clientePuedeEscribir", O_CREAT, 0666, 1);
    //Semaforo a ser chequeado del lado del servidor para ver si un cliente envio algo.
    clienteAServidor = sem_open("/clienteAServidor", O_CREAT, 0666, 0);
    //Semaforo a ser chequeado del lado del cliente para ver si puede leer de la memoria compartido algo que envio el servidor.
    servidorACliente = sem_open("/servidorACliente", O_CREAT, 0666, 0);

    //Creacion de memoria compartida para recibir la orden del cliente.
    leerDeCliente = shmget(234, sizeof(t_msgCliente), IPC_CREAT | 0666);
    //Al cliente le voy a mandar un char de 500 por ahora, ver despues.
    escribirACliente = shmget(235, sizeof(t_msgServidor), IPC_CREAT | 0666);

    //Se enlazan las memorias compartidas serian los buffers de entrada y salida.
    msgCliente = (t_msgCliente*)shmat(leerDeCliente, NULL, 0);
    msgServidor = (t_msgServidor*)shmat(escribirACliente, NULL, 0);

    ///El servidor va a estar andando siempre, excepto que se le envie SIGINT o SIGTERM
    while(1)
    {
        ///Espero hasta que el semaforo recibirOrden sea activado por un cliente.
        sem_wait(clienteAServidor);


        ///Funcion que procesa la query y su mensaje
        procesarConsulta(msgCliente, msgServidor, bd);

        ///Hago un ++ al semaforo del cliente avisando que ya puede leer.
        sem_post(servidorACliente);
    }
}
