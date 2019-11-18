#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

typedef struct{
    char ipServer[17];
    int port;
}t_param;


int main(int argc, char const *argv[]);
void ayuda();

#endif // CLIENTE_H_INCLUDED
