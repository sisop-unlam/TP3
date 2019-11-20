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
int socketCliente;
int longitud;

void salir()
{
    printf("\nFinalizando el proceso...\n");
    strcpy(query, "QUIT");
    send(socketCliente, query, sizeof(query), 0);
    close(socketCliente);
    exit(1);
}

void set(const char *ip, const char *puerto)
{
    bzero(&(configuracionSocket.sin_zero), 8);
    configuracionSocket.sin_family = AF_INET;
    configuracionSocket.sin_addr.s_addr = inet_addr(ip);
    configuracionSocket.sin_port = htons(atoi(puerto));
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("\nInserte los parametros necesarios");
        getHelp();
        exit(255);
    }

    ///Si estoy pidiendole la help
    if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0)
    {
        getHelp();
    }

    signal(SIGINT, salir);
    signal(SIGTERM, salir);

    if ((socketCliente = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("No se pudo crear el socket!\n");
        return 1;
    }

    ///Le paso IP y PUERTO
    set(argv[1], argv[2]);

    if (connect(socketCliente, (struct sockaddr *)&configuracionSocket, sizeof(configuracionSocket)) == -1)
    {
        printf("Solicitud de conexion rechazada\n");
        return 1;
    }

    printf("Escriba QUIT para cerrar el programa\n");
    printf("Escriba una query: \n");
    fgets(query, sizeof(query), stdin);
    char *e = strrchr(query, '\n');
    *e = '\0';
    char *separator = "=";
    char *elementosDespuesDeSeparador;

    elementosDespuesDeSeparador = strrchr(query, '=');
    while (strcmp(query, "QUIT") != 0)
    {

        if (elementosDespuesDeSeparador && strlen(query) > 1 && strlen(elementosDespuesDeSeparador) > 1)
        {
            ///MANDO EL REQUEST
            send(socketCliente, query, sizeof(query), 0);
            recv(socketCliente, respuesta, sizeof(int), 0);
            longitud = atoi(respuesta);
            recv(socketCliente, respuesta, longitud, 0);

            while (strcmp(respuesta, "FIN") != 0)
            {
                printf("%s\n", respuesta);
                memset(respuesta, 0, strlen(respuesta));
                recv(socketCliente, respuesta, sizeof(int), 0);
                longitud = atoi(respuesta);
                recv(socketCliente, respuesta, longitud, 0);
            }
        }
        else
        {
            printf("Escriba una consulta\n");
        }
        printf("Escriba QUIT para cerrar el programa\n");
        printf("Escriba una query: \n");
        fgets(query, sizeof(query), stdin);
        char *e = strrchr(query, '\n');
        *e = '\0';
        elementosDespuesDeSeparador = strrchr(query, '=');
    }

    send(socketCliente, query, sizeof(query), 0);
    close(socketCliente);
}
