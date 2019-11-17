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
#define BD "./articulos.txt"
#define TAM_CONSULTA 512

typedef struct
{
    int item_id;
    char articulo[512];
    char producto[512];
    char marca[512];
} t_articulo;

void error(char *);
void help();
void leerConsultas(char *);
void procesarConsulta(char *consulta, char *memoriaCompartida, sem_t *semaforoMemoria, sem_t *respuestaEnviada, sem_t *semaforoEnvio, FILE *arch);
FILE *abrirArchivo(char *path);
void recorrerArchivo(char *memoriaCompartida, char *linea, char *buscado, int columna, sem_t *semaforoMemoria, sem_t *respuestaEnviada, sem_t *semaforoEnvio, FILE *arch);
void sendMsg(const char *, char *, sem_t *, sem_t *, sem_t *);
void explode(const char *src, const char *tokens, char ***list, size_t *len);
char *strdup(const char *src);
void liberarRecursos();

#endif
