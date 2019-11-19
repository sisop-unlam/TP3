#include "funciones.h"
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

void getHelp()
{
    printf("\nEjercicio 5 (SOCKET)\n================================\n\n");
    printf("El fin del programa es comunicarse con un proceso Servidor\n");
    printf("el cual tiene una base de datos cargada. En base a lo que el usuario\n");
    printf("necesite, ya sea buscar por MARCA, PRODUCTO o ID, el Servidor buscara\n");
    printf("en el archivo de base de datos los registros coincidentes y los enviara al proceso cliente.\n");
    printf("\nUna vez dentro del programa, se puede utilizar por ejemplo, MARCA=DULCYPAS\n");
    printf("para obtener todos los productos pertenecientes a esa marca. De la misma manera se puede\n");
    printf("\nrealizar lo mismo con ID y PRODUCTO.\n");
    printf("\nPARAMETROS\n==========\n");
    printf("\n\nIP: Direccion IP del servidor\n");
    printf("PUERTO: Puerto del servidor\n");
    printf("\n\nEJEMPLOS\n========\n");
    printf("\t\t./Servidor 192.168.1.21 7777\n");

    exit(1);
}
