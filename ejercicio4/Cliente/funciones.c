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
#include "funciones.h"

void getHelp()
{
    printf("\nEjercicio 4 (MEMORIA COMPARTIDA)\n================================\n\n");
    printf("El fin del programa es comunicarse con un proceso Servidor\n");
    printf("el cual tiene una base de datos cargada. En base a lo que el usuario\n");
    printf("necesite, ya sea buscar por MARCA, PRODUCTO o ID, el Servidor buscara\n");
    printf("en el archivo de base de datos los registros coincidentes y los enviara al proceso cliente.\n");
    printf("\nPARAMETROS\n==========\n");
    printf("\n\nQUERY: la primer mitad es el parametro buscado (MARCA, PRODUCTO O ID)\n");
    printf("\tla segunda parte ira seguida de un signo igual (=) y tendra el valor a buscar");
    printf("\n\nEJEMPLOS\n========\n");
    printf("\t\t./Cliente PRODUCTO=BOLSA\n");
    printf("\t\t./Cliente ID=12\n");
    printf("\t\t./Cliente MARCA=MIMAGO\n");
    exit(1);
}
