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

#define TAM_CONSULTA 512

void help();
void hacerConsulta(char *query);

#endif // FUNCIONES_H_INCLUDED
