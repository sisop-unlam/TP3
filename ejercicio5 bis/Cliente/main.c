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

char respuesta[TAMQUERY];
char query[TAMQUERY];
int clientSocket;
int longitud;

void salir()
{
    printf("\nFinalizando el proceso...\n");
    strcpy(query, "SALIR");
    send(clientSocket, query, sizeof(query), 0);
    close(clientSocket);
    exit(1);
}

void set(const char* ip, const char* puerto)
{
    bzero(&(serverAddress.sin_zero), 8);
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip);
    serverAddress.sin_port=htons(atoi(puerto));
}

int main(int argc, char *argv[])
{

    ///Si estoy pidiendole la getHelp
    if (strcmp(argv[1],"-getHelp") == 0 || strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-?") == 0)
    {
        getHelp();
    }

    signal(SIGINT, salir);
    signal(SIGTERM, salir);

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("No se pudo crear el socket!\n");
        return 1;
    }

    ///Le paso IP y PUERTO
    set(argv[1], argv[2]);

    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Solicitud de conexion rechazada\n");
        return 1;
    }

    printf("Escriba SALIR para cerrar el programa\n");
    printf("Escriba una query: \n");
    scanf("%s", query);

    while (strcmp(query, "SALIR") != 0)
    {
        ///MANDO EL REQUEST
        send(clientSocket, query, sizeof(query), 0);
        recv(clientSocket, respuesta, sizeof(int), 0);
        longitud = atoi(respuesta);
        recv(clientSocket, respuesta, longitud, 0);

        while (strcmp(respuesta,"FIN") != 0)
        {
            printf("%s\n", respuesta);
            memset(respuesta, 0, strlen(respuesta));
            recv(clientSocket, respuesta, sizeof(int), 0);
            longitud = atoi(respuesta);
            recv(clientSocket, respuesta, longitud, 0);
        }

        printf("Escriba SALIR para cerrar el programa\n");
        printf("Escriba una query: \n");
        scanf("%s", query);
    }

    send(clientSocket, query, sizeof(query), 0);
    close(clientSocket);
}
