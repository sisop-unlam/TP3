#ifndef FUNCIONES_H_
#define FUNCIONES_H_
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <signal.h>
#include <ctype.h>
#define TAMQUERY 200

typedef struct
{
    int item_id;
    char articulo[512];
    char producto[512];
    char marca[512];
} t_articulo;

void getHelp();
void liberarRecursos();

void obtenerTuplas(FILE *arch, char *espacio, sem_t *respuestaEnviada, sem_t *semaforoMemoria, sem_t *semaforoEnvio, char *consulta);
void sendMsg(sem_t *respuestaEnviada, sem_t *semaforoMemoria, sem_t *semaforoEnvio, const char *msg, char *espacio);

///UTILITARIO
void explode(const char *src, const char *tokens, char ***list, size_t *len);
char *strdup(const char *src);

#endif
