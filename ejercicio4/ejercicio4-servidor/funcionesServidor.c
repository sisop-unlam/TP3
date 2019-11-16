#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "estructuras.h"
#include "funcionesServidor.h"

///Funcion que busca resultados coincidentes con lo pedido
void procesarConsulta(sem_t *clientePuedeEscribir, sem_t *servidorACliente, t_msgCliente *msgCliente, t_msgServidor *msgServidor, char *bd)
{
    //sem_wait(clientePuedeEscribir);
    //Log del servidor.
    printf("\nSe recibio la consulta %s=%s", msgCliente->comando.parametro, msgCliente->comando.parametroBuscado);
    //sem_post(clientePuedeEscribir);

    FILE *arch = fopen(bd, "rt");
    if (!arch)
        return;
    t_comando d = msgCliente->comando;
    t_articulo art;

    while (fscanf(arch, "%d;%[^;];%[^;];%[^;\n]\n", &art.item_id, art.articulo, art.producto, art.marca) != EOF)
    {

        ///ACA VERIFICO SI ESTA LO PEDIDO

        /* if (strcmp(d.parametro, MARCA) == 0)
        {

            if (strcmp(art.marca, d.parametroBuscado) == 0)
            {
                printf("\n%s", art.producto);
                sem_wait(clientePuedeEscribir);
                sprintf(msgServidor->respuesta, "%d|%s|%s|%s\n", art.item_id, art.producto, art.marca, art.articulo);
                msgServidor->pid = msgCliente->pid;
                sem_post(clientePuedeEscribir);
                sem_post(servidorACliente);
            }
        }
        else */
        if (strcmp(d.parametro, PRODUCTO) == 0)
        {

            if (strcmp(art.producto, d.parametroBuscado) == 0)
            {
                sem_wait(clientePuedeEscribir);
                printf("%d|%s|%s|%s\n", art.item_id, art.producto, art.marca, art.articulo);
                sprintf(msgServidor->respuesta, "%d|%s|%s|%s\n", art.item_id, art.producto, art.marca, art.articulo);
                msgServidor->pid = msgCliente->pid;
                sem_post(clientePuedeEscribir);

                sem_post(servidorACliente);
            }
        } /*
        else if (strcmp(d.parametro, ID) == 0)
        {
            if (art.item_id, atoi(d.parametroBuscado) == 0)
            {
                sem_wait(clientePuedeEscribir);
                sprintf(msgServidor->respuesta, "%d|%s|%s|%s\n", art.item_id, art.producto, art.marca, art.articulo);
                msgServidor->pid = msgCliente->pid;
                sem_post(clientePuedeEscribir);
                sem_post(servidorACliente);
            }
        }*/
    }
    sem_wait(clientePuedeEscribir);

    sprintf(msgServidor->respuesta, "%s", "FIN");
    msgServidor->pid = msgCliente->pid;
    sem_post(clientePuedeEscribir);

    sem_post(servidorACliente);

    printf("\nFinalizada la consulta para el cliente %d", msgCliente->pid);
    ///voy a tener que armar un mensaje espeical que le avise al usuario que ya termino.
    ///si no, al terminar va a "cerrar" la sesion
    fclose(arch);
}
