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

int main(int argc, char *argv[])
{
    char bd[500];
    socklen_t cl = sizeof(struct sockaddr_in);
    struct sockaddr_in ca;
    int socketCliente, serverSocket, sockfd, habilitar = 1;
    pthread_t tid;

    if (comprobacionBD(bd) == 1)
        return 1;

    if (creacionSocket(&serverSocket, &habilitar) == 1)
        return 1;

    ///Le paso IP y PUERTO
    set(argv[1], argv[2]);

    bindListen(&serverSocket);

    ///Lo dejo corriendo como demonio
    while (1)
        aceptarRequests(&tid, &socketCliente, &serverSocket, &ca, &cl);

    return 0;
}
