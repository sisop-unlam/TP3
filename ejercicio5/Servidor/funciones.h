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
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BD "./articulos.txt"
#define TAMQUERY 200
#define MAX_QUEUE 10
///STRUCT DE ARTICULO
typedef struct
{
    int item_id;
    char articulo[512];
    char producto[512];
    char marca[512];
} t_articulo;

///MAIN
struct sockaddr_in configuracionSocket;
int bindListen(int *serverSocket);
int creacionSocket(int *serverSocket, int *habilitar);
int comprobacionBD(char *bd, char* path);
void set(const char *ip, const char *puerto);

///funcionesServidor
void *obtenerQuery(void *sockfd);
void obtenerTuplas(FILE *arch, int socketCliente, char *query);
void enviarMensaje(const char *mensaje, int socketCliente, char *tamPaquete);
void aceptarRequests(pthread_t *tid, int *socketCliente, int *serverSocket, struct sockaddr_in *ca, socklen_t *cl);
///UTILITARIAS
void sendMsg(const char *, char *, sem_t *, sem_t *, sem_t *);
void explode(const char *src, const char *tokens, char ***list, size_t *len);
char *strdup(const char *src);
#endif
