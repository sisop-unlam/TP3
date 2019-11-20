#include "funciones.h"
/*
***********************************************************************************
 Nombre Del Script:        Servidor
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           5
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
char ip[16];
int main(int argc, char *argv[])
{
    socklen_t cl = sizeof(struct sockaddr_in);
    struct sockaddr_in ca;
    int socketCliente, serverSocket, sockfd, habilitar = 1;
    pthread_t tid;

    if(obtenerIP(ip)){
        printf("\nOcurrio un error al determina la IP local.\n");
        exit(1);
    }

    if (argc <= 2)
    {
        printf("Por favor, ingrese un puerto y el archivo de base de datos\n");
        printf("Ejemplos:\n");
        printf("\t\t./Servidor 7777 db.txt\n");
        printf("\t\t./Servidor 54222 ../base_de_datos.txt\n");

        exit(255);
    }
    if (comprobacionBD(argv[2]) == 1)
        return 1;

    if (creacionSocket(&serverSocket, &habilitar) == 1)
        return 1;

    ///Le paso IP y PUERTO
    set(ip, argv[1]);

    bindListen(&serverSocket);

    ///Lo dejo corriendo como demonio
    while (1)
        aceptarRequests(&tid, &socketCliente, &serverSocket, &ca, &cl);

    return 0;
}
