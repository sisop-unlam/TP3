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

void obtenerRegistros(sem_t * puedeEnviar, sem_t * clientePuedeEscribir, sem_t * clienteAServidor, sem_t * servidorACliente, t_msgCliente* msgCliente, t_msgServidor* msgServidor, char* consulta)
{
    t_comando dat;

    cls();

    ///ACA VOY A PARTIR LA QUERY. SI TIENE ALGO INVALIDO, DESAPARECE
    char **list;
    size_t i, len;
    explode(consulta, "=", &list, &len);
    if(len != 2)
    {
        ///NO SE INGRESO LA INFORMACION NECESARIA, SALGO
        printf("\nIngrese una query valida.");
        exit(1);
    }

    if(strcmp(list[0], ID) == 0 || strcmp(list[0], MARCA) == 0 || strcmp(list[0], PRODUCTO) == 0)
    {
        strcpy(dat.parametro, list[0]);
        strcpy(dat.parametroBuscado, list[1]);
    }
    else
    {
        ///NO SE INGRESO UN PARAMETRO CORRECTO
        printf("\nIngrese una query valida.");
        exit(1);
    }

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

    ///Comprobacion si efectivamente el mensaje enviado por el servidor es para mi.
    while (strcmp(msgServidor->respuesta,"FIN") != 0)
    {
        sem_wait(servidorACliente);

        if(msgServidor->pid == msgCliente->pid && strcmp(msgServidor->respuesta,"FIN") != 0)
        {
            printf("%s\n", msgServidor->respuesta);
        }
        sem_post(puedeEnviar);
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

char *strdup(const char *src)
{
    char *tmp = malloc(strlen(src) + 1);
    if(tmp)
        strcpy(tmp, src);
    return tmp;
}

void explode(const char *src, const char *tokens, char ***list, size_t *len)
{
    if(src == NULL || list == NULL || len == NULL)
        return;

    char *str, *copy, **_list = NULL, **tmp;
    *list = NULL;
    *len  = 0;

    copy = strdup(src);
    if(copy == NULL)
        return;

    str = strtok(copy, tokens);
    if(str == NULL)
        goto free_and_exit;

    _list = realloc(NULL, sizeof *_list);
    if(_list == NULL)
        goto free_and_exit;

    _list[*len] = strdup(str);
    if(_list[*len] == NULL)
        goto free_and_exit;
    (*len)++;


    while((str = strtok(NULL, tokens)))
    {
        tmp = realloc(_list, (sizeof *_list) * (*len + 1));
        if(tmp == NULL)
            goto free_and_exit;

        _list = tmp;

        _list[*len] = strdup(str);
        if(_list[*len] == NULL)
            goto free_and_exit;
        (*len)++;
    }


free_and_exit:
    *list = _list;
    free(copy);
}
