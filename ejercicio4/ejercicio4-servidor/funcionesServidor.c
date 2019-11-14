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
void procesarConsulta(sem_t* puedeEnviar,sem_t* servidorACliente, t_msgCliente* msgCliente, t_msgServidor* msgServidor, char* bd)
{
    //Log del servidor.
    printf("\nSe recibio la consulta %s=%s", msgCliente->comando.parametro, msgCliente->comando.parametroBuscado);
    FILE* arch = fopen(bd, "rt");
    if(!arch) return;
    t_comando d = msgCliente->comando;
    t_articulo art;

    while(fscanf(arch, "%d;%[^;];%[^;];%[^;\n]\n", &art.item_id, art.articulo, art.producto, art.marca)!=EOF)
    {
//printf("\nID: %d\tArticulo: %s\tProducto: %s\tMarca: %s",art.item_id,art.articulo,art.producto,art.marca);
        ///ACA VERIFICO SI ESTA LO PEDIDO

        if(strcmp(d.parametro, MARCA) ==0)
        {

            if(strcmp(art.marca, d.parametroBuscado) ==0)
            {
                printf("\n%s", art.producto);
                sem_wait(puedeEnviar);

                sprintf(msgServidor->respuesta, "%d|%s|%s|%s\n", art.item_id,art.producto,art.marca,art.articulo);
                msgServidor->pid = msgCliente->pid;
                sem_post(servidorACliente);

            }
        }
        else if(strcmp(d.parametro, PRODUCTO) ==0)
        {
            if(strcmp(art.producto, d.parametroBuscado) ==0)
            {
                sem_wait(puedeEnviar);

                sprintf(msgServidor->respuesta, "%d|%s|%s|%s\n", art.item_id,art.producto,art.marca,art.articulo);
                msgServidor->pid = msgCliente->pid;
                sem_post(servidorACliente);

            }
        }
        else if(strcmp(d.parametro, ID) ==0)
        {
            if(art.item_id, atoi(d.parametroBuscado) ==0)
            {
                sem_wait(puedeEnviar);
                sprintf(msgServidor->respuesta, "%d|%s|%s|%s\n", art.item_id,art.producto,art.marca,art.articulo);
                msgServidor->pid = msgCliente->pid;
                sem_post(servidorACliente);

            }
        }

    }
    sem_wait(puedeEnviar);
    sprintf(msgServidor->respuesta, "%s", "FIN");
    msgServidor->pid = msgCliente->pid;
    sem_post(servidorACliente);
    ///voy a tener que armar un mensaje espeical que le avise al usuario que ya termino.
    ///si no, al terminar va a "cerrar" la sesion
    fclose(arch);
}

