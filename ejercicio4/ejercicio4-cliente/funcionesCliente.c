#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "estructuras.h"
#include "funcionesCliente.h"

void obtenerRegistros(sem_t * clientePuedeEscribir, sem_t * clienteAServidor, sem_t * servidorACliente, t_msgCliente* msgCliente, t_msgServidor* msgServidor, char* consulta)
{
    t_comando dat;

    cls();

    ///ENVIO DE INFORMACION AL SERVIDOR

    //Si puedo escribir entonces hago todo lo que esta abajo.
    sem_wait(clientePuedeEscribir);

    //Seteo la msgCliente y los datos para llevar a cabo la msgCliente.
    msgCliente->comando = dat;

    //Le aviso al servidor que puede leer.
    sem_post(clienteAServidor);
    //Le aviso a otros clientes que pueden escribir.
    sem_post(clientePuedeEscribir);

    ///RECEPCION DE INFORMACION DEL SERVIDOR

    //Si puedo recibir informacion del servidor, entonces recibo.
    sem_wait(servidorACliente);

    ///Comprobacion si efectivamente el mensaje enviado por el servidor es para mi.
    if(msgServidor->pid == msgCliente->pid)
    {
        printf("\n%s", msgServidor->respuesta);
    }
}

//Funcion de normalizacion de cadenas.
int normalizar(char* cad)
{
    while(*cad != '\0')
    {
        ///LLEVO TODO A MAYUSCULA
        ///TODO: SALTEAR BLANCOS
        if(ES_LETRA(*cad))
        {
            *cad = toupper(*cad);
            cad++;
        }
        else
            return 0;
    }
    return 1;
}
