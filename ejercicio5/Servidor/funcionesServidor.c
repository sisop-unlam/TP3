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

void *obtenerQuery(void *sockfdVoid)
{
    int sockfd = *((int *)sockfdVoid);
    char query[TAMQUERY];
    FILE *arch = fopen(BD, "rt");
    recv(sockfd, query, TAMQUERY, 0);

    while (strcmp(query, "QUIT") != 0)
    {
        obtenerTuplas(arch, sockfd, query);
        recv(sockfd, query, TAMQUERY, 0);
    }
    printf("[CONEXION TERMINADA] Un usuario se ha desconectado\n");
    fflush(stdout);

    fclose(arch);
    close(sockfd);
}

void obtenerTuplas(FILE *arch, int socketCliente, char *query)
{
    char linea[512], *tipo, *valor;
    char **list;
    size_t len;
    int i = 0;
    t_articulo art;

    ///para pasar el itemID de int a char
    char bufferItemID[10];
    char tamPaquete[10];

    ///Vuelvo al inicio del archivo y me salteo la primer linea (la del encabezado)
    fseek(arch, 0, SEEK_SET);
    fscanf(arch, "%*[^\n]");

    ///Muestro por pantalla la query
    printf("\n%s\n", query);

    ///Hago un explode, similar al de PHP
    ///En un array dejo el valor y el campo buscado
    explode(query, "=", &list, &len);

    ///Primero obtengo el tipo
    tipo = list[0];
    ///Luego el valor buscado
    valor = list[1];

    ///Si tengo alguno de los 3 tipos
    if (strcmp(tipo, "PRODUCTO") == 0 || strcmp(tipo, "ID") == 0 || strcmp(tipo, "MARCA") == 0)
    {

        ///Escaneo linea a linea, matcheando
        while (fscanf(arch, "%d;%[^;];%[^;];%[^;\n] \n", &art.item_id, art.articulo, art.producto, art.marca) != EOF)
        {
            ///Convierto el itemId en un string
            snprintf(bufferItemID, 10, "%d", art.item_id);

            if (strcmp(tipo, "PRODUCTO") == 0 && strcmp(art.producto, valor) == 0)
            {
                snprintf(linea, sizeof(linea), "%s;%s;%s;%s", bufferItemID, art.articulo, art.producto, art.marca);
                enviarMensaje(linea, socketCliente, tamPaquete);
                usleep(1 * 1000); ///Para enviar correctamente via RED
                i++;
            }
            else if (strcmp(tipo, "ID") == 0 && strcmp(bufferItemID, valor) == 0)
            {
                snprintf(linea, sizeof(linea), "%s;%s;%s;%s", bufferItemID, art.articulo, art.producto, art.marca);
                enviarMensaje(linea, socketCliente, tamPaquete);
                usleep(1 * 1000);
                i++;
            }
            else if (strcmp(tipo, "MARCA") == 0 && strcmp(art.marca, valor) == 0)
            {
                snprintf(linea, sizeof(linea), "%s;%s;%s;%s", bufferItemID, art.articulo, art.producto, art.marca);
                enviarMensaje(linea, socketCliente, tamPaquete);
                usleep(1 * 1000);
                i++;
            }
        }
        printf("Enviados %d registros\n", i);
        fflush(stdout);
        if (!i)
        {
            const char noRows[] = "No se encontraron registros que coincidan con la busqueda.";
            enviarMensaje(noRows, socketCliente, tamPaquete);
        }
    }
    else
    {
        const char errorPedido[] = "Por favor, ingrese un campo válido (ID, MARCA o PRODUCTO).";
        enviarMensaje(errorPedido, socketCliente, tamPaquete);
    }

    const char fin[] = "FIN";
    enviarMensaje(fin, socketCliente, tamPaquete);
}

void enviarMensaje(const char *mensaje, int socketCliente, char *tamPaquete)
{
    if (strcmp(mensaje, "FIN") != 0)
    {
        sprintf(tamPaquete, "%lu", strlen(mensaje));
        send(socketCliente, tamPaquete, sizeof(int), 0);
        send(socketCliente, mensaje, strlen(mensaje), 0);
    }
    else
    {
        sprintf(tamPaquete, "%d", (int)sizeof(mensaje));
        send(socketCliente, tamPaquete, sizeof(int), 0);
        send(socketCliente, mensaje, sizeof(mensaje), 0);
    }
}

void set(const char *ip, const char *puerto)
{
    bzero(&(configuracionSocket.sin_zero), 8);
    configuracionSocket.sin_family = AF_INET;
    configuracionSocket.sin_addr.s_addr = inet_addr(ip);
    configuracionSocket.sin_port = htons(atoi(puerto));
}

int comprobacionBD(char *bd, char* path)
{
    ///Primero se comprueba que el archivo de la base de datos exista.
    if (access(path, F_OK) != -1)
        strcpy(bd, path);
    else
    {
        printf("El archivo de la base de datos no existe\n");
        return 1;
    }

    return 0;
}

int creacionSocket(int *serverSocket, int *habilitar)
{
    ///CREO EL SOCKET
    if ((*serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("No se pudo crear el socket!\n");
        return 1;
    }

    ///Configurar opciones del socket
    if (setsockopt(*serverSocket, SOL_SOCKET, SO_REUSEADDR, habilitar, sizeof(int)) == -1)
    {
        printf("No se pudo configurar las opciones del socket!\n");
        return 1;
    }

    return 0;
}

int bindListen(int *serverSocket)
{
    ///Vinculo el socket con la configuracion
    if (bind(*serverSocket, (struct sockaddr *)&configuracionSocket, sizeof(struct sockaddr_in)) < 0)
    {
        printf("No se pudo vincular el socket con la configuración del servidor.");
        return 1;
    }

    /// Empiezo a escuchar en el puerto espeificado anteriormente
    listen(*serverSocket, MAX_QUEUE);
    printf("Esperando conexiones...\n");
    fflush(stdout);
    return 0;
}

void aceptarRequests(pthread_t *tid, int *socketCliente, int *serverSocket, struct sockaddr_in *ca, socklen_t *cl)
{
    *socketCliente = accept(*serverSocket, (struct sockaddr *)ca, cl);

    printf("[NUEVA CONEXION] Un cliente nuevo se conecto al servidor.\n");
    fflush(stdout);

    ///Tengo que crear un hilo por cada usuario
    ///De esta forma, aseguro que no se crucen
    pthread_create(tid, NULL, &obtenerQuery, socketCliente);
}