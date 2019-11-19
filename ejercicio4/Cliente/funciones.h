/*
***********************************************************************************
 Nombre Del Script:        Cliente
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           4
 Entrega Nro.:             1
 Integrantes:
    Apellido            Nombre                  DNI
    --------------------------------------------------
   Krasuk               Joaquín               40745090
   Rodriguez            Christian             37947646
   Vivas                Pablo                 38703964
   Ramos		        Maximiliano		      35068917
   Fernández		    Jonathan		      37226233
***********************************************************************************
*/
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

#define TAMQUERY 200
#define ENVIAR_RECIBIR_QUERY 1
#define CERRAR_SEMAFOROS 2

void getHelp();
void hacerConsulta(char *query);
void operacionesSemaforos(int num);

#endif // FUNCIONES_H_INCLUDED
