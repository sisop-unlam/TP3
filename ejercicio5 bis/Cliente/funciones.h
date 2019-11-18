#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

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
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define TAMQUERY 512

///MAIN
struct sockaddr_in serverAddress;
void salir();
int conexion(int *clientSocket);
void set(const char *ip, const char *puerto);

///funcionesServidor
void getHelp();
void promptConsulta(char *query, char *respuesta, int *longitud, int *clientSocket);

#endif // FUNCIONES_H_INCLUDED
