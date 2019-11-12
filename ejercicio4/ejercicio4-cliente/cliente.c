#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "estructuras.h"
#include "funcionesCliente.h"

///Flujo MAIN->MENU->ACCION QUE QUIERE LLEVAR A CABO EL CLIENTE.
int main(int argc, char *argv[])
{

    ///Apertura de semaforos y memoria compartida
    sem_t *clientePuedeEscribir = sem_open("/clientePuedeEscribir", 0);
    sem_t *clienteAServidor = sem_open("/clienteAServidor", 0);
    sem_t *servidorACliente = sem_open("/servidorACliente", 0);

    ///Primero compruebo si los segmentos de memoria compartida ya fueron creados.
    ///Si la respuesta es que no fueron creados, entonces el servidor no inicio todavia, por ende error.
    ///http://man7.org/linux/man-pages/man2/shmget.2.html
    int escribirAServidor = shmget(234, sizeof(t_msgCliente), IPC_EXCL | 0666);
    int leerDeServidor = shmget(235, sizeof(t_msgServidor), IPC_EXCL | 0666);

    if(escribirAServidor == -1 || leerDeServidor == -1)
    {
        printf("El servidor no esta prendido.");
        return 10;
    }

    //Variable donde el cliente escribe los datos a mandar al servidor.
    t_msgCliente* msgCliente = shmat(escribirAServidor, NULL, 0);
    t_msgServidor* msgServidor = shmat(leerDeServidor, NULL, 0);

    //Variable donde almaceno la query
    char consulta[500];

    ///Validacion de parametros.
    if(argc < 2 || argc > 3)
    {
        printf("Error, ingrese una query valida\n");
        return 20;
    }
    if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0)))
    {
        printf("La sintaxis es ./Cliente [Consulta]\nFormato: ./Cliente [string]\nEjemplo: ./Cliente PRODUCTO=HELADO\n");
        return 30;
    }

    ///COPIO LA CONSULTA A LA VARIABLE, PARA PROCESARLO
    strcpy(consulta,argv[1]);

    ///Necesario para eliminar espacios en blanco, etc
    normalizar(consulta);

    cls();

    ///IDENTIFICADOR DEL CLIENTE.
    msgCliente->pid = getpid();

    ///Ir al menu
    obtenerRegistros(clientePuedeEscribir, clienteAServidor, servidorACliente, msgCliente, msgServidor, consulta);

    ///En este punto el cliente ya termino de realizar sus funciones.

    //Desvinculo las memorias compartidas de las variables locales.
    shmctl(escribirAServidor, IPC_RMID, NULL);
    shmctl(leerDeServidor, IPC_RMID, NULL);

    //La eliminacion de las memorias compartidas es realizada por el servidor.
    return 0;
}
