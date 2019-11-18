#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "signal.h"

typedef struct{
    char pathDatabase[200];
    int port;
}t_param;

extern pthread_mutex_t mutex;
void ayuda();
void cerrarServidor(int s);

#endif // SERVIDOR_H_INCLUDED
