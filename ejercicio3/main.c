/*
***********************************************************************************
 Nombre Del Script:        Servidor
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           3
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#define SUCCESS 1
#define FAILURE 0
#define PIDE_AYUDA 2
#define CONTINUAR 3
#define ERROR_FIFO -10
#define TAM_CAD 150

#define RUNNING_DIR "/tmp"
#define LOCK_FILE "daemond.lock"
#define LOG_FILE "daemond.log"

//size 100
typedef struct entrada
{
    char key[50];
    char value[50];
} t_entrada;

//size 200
typedef struct salida
{
    char item_id[50];
    char articulo[50];
    char producto[50];
    char marca[50];
} t_salida;

int buscarHits(t_entrada *entrada, t_salida hits[]);
void devolverInfo(t_salida registros[], int cantHits, int fd_salida);
int abrirFIFO_lectura(char *fifo_entrada);
int abrirFIFO_escritura(char *fifo_salida);
int pideAyuda(int argc, char *argv[]);
void mostrar_ayuda();
int splitMovimiento(t_entrada *entrada, char *linea);
int parametrosValidos(int argc, char *argv[]);
char *leer_archivo();

void signal_handler(int sig);
void log_message(char *filename, char *message);
void daemonize();
int fd_entrada = 1;
int fd_salida = 1;

int main(int argc, char *argv[])
{
    if (parametrosValidos(argc, argv) == FAILURE)
    {
        printf("Parametros Invalidos. Saliendo..\n");
        return FAILURE;
    }

    if (pideAyuda(argc, argv) == PIDE_AYUDA)
    {
        mostrar_ayuda();
        return PIDE_AYUDA;
    }

    daemonize();

    //cada uno pesa 150
    char fifo_entrada[TAM_CAD] = "/home/colby/fifo_entrada"; //CAMBIAR ESTO POR LOS PARAMETROS DE ENTRADA
    char fifo_salida[TAM_CAD] = "/home/colby/fifo_salida";

    strcpy(fifo_entrada, argv[1]);
    strcpy(fifo_salida, argv[2]);

    //remove(fifo_entrada);
    //remove(fifo_salida);

    int res = mkfifo(fifo_entrada, 0666);
    if (res == -1)
    {
        if (errno == ENOENT)
        {
            printf("Path fifo entrada incorrecto. Saliendo\n");
            return ERROR_FIFO;
        }
    }
    else
    {
        fd_entrada = open(fifo_entrada, O_RDWR);
        if (fd_entrada < 0)
        {
            printf("No se pudo abrir el Fifo Entrada\n");
            return ERROR_FIFO;
        }
    }

    int res2 = mkfifo(fifo_salida, 0666);
    if (res2 == -1)
    {
        if (errno == ENOENT)
        {
            printf("Path fifo salida incorrecto. Saliendo\n");
            return ERROR_FIFO;
        }
    }
    else
    {
        fd_salida = open(fifo_salida, O_RDWR);
        if (fd_salida < 0)
        {
            printf("No se pudo abrir el Fifo Salida\n");
            return ERROR_FIFO;
        }
    }

    int bytesLeidos = 0;
    t_entrada entrada;
    t_salida search_hits[180]; //800 * 200 pesa 156 KB! supera el maximo del fifo ?

    char linea_entrada[TAM_CAD];

    bytesLeidos = read(fd_entrada, linea_entrada, sizeof(linea_entrada));

    while (bytesLeidos > 0)
    {

        if ((strcmp(linea_entrada, "fin") == 0 || strcmp(linea_entrada, "fin\n") == 0))
        {
            close(fd_entrada);
            close(fd_salida);
            remove(fifo_entrada);
            remove(fifo_salida);

            return SUCCESS;
        }
        entrada.key[0] = '\0';
        entrada.value[0] = '\0';

        splitMovimiento(&entrada, linea_entrada);

        int cantidad_aciertos = buscarHits(&entrada, search_hits);
        devolverInfo(search_hits, cantidad_aciertos, fd_salida);

        close(fd_entrada);
        fd_entrada = open(fifo_entrada, O_RDWR);
        memset(linea_entrada, 0, TAM_CAD);
        linea_entrada[0] = '\0';

        bytesLeidos = read(fd_entrada, linea_entrada, sizeof(linea_entrada));
    }
    close(fd_entrada);
    close(fd_salida);
    remove(fifo_entrada);
    remove(fifo_salida);

    return SUCCESS;
}
//devuelve la cantidad de registros encontrados. Los mismos estan en el vector hits.
int buscarHits(t_entrada *entrada, t_salida hits[])
{
    int cantAciertos = 0;
    const char *path_articulos = "articulos.txt";
    FILE *pf_articulos = fopen(path_articulos, "rt");
    if (!pf_articulos)
    {
        printf("No se pudo abrir el archivo de datos\n");
        return FAILURE;
    }

    int orden = 0;
    char linea[TAM_CAD] = {0};

    if (strcmp(entrada->key, "ITEM_ID") == 0)
    {
        orden = 0;
    }
    if (strcmp(entrada->key, "ARTICULO") == 0)
        orden = 1;
    if (strcmp(entrada->key, "PRODUCTO") == 0)
        orden = 2;
    if (strcmp(entrada->key, "MARCA") == 0)
    {

        orden = 3;
    }

    t_salida reg_salida;
    while ((fgets(linea, sizeof(linea), pf_articulos)))
    {

        memset(&reg_salida, 0, 200);
        reg_salida.articulo[0] = '\0';
        reg_salida.item_id[0] = '\0';
        reg_salida.marca[0] = '\0';
        reg_salida.producto[0] = '\0';

        char *e = strrchr(linea, '\n');
        *e = '\0';
        e = strrchr(linea, '\n');
        if (e)
            *e = '\0';
        e = strrchr(linea, '\r');
        if (e)
            *e = '\0';
        strcpy(reg_salida.item_id, strtok(linea, ";"));
        strcpy(reg_salida.articulo, strtok(NULL, ";"));
        strcpy(reg_salida.producto, strtok(NULL, ";"));
        strcpy(reg_salida.marca, strtok(NULL, ";"));

        if (cantAciertos >= 180)
        {
            fclose(pf_articulos);
            return cantAciertos; //es la maxima cantidad de aciertos permitida. que se yo.
        }
        switch (orden)
        {
        case (0):
            if (strcmp(reg_salida.item_id, entrada->value) == 0)
            {
                strcpy(hits[cantAciertos].articulo, reg_salida.articulo);
                strcpy(hits[cantAciertos].marca, reg_salida.marca);
                strcpy(hits[cantAciertos].producto, reg_salida.producto);
                strcpy(hits[cantAciertos].item_id, reg_salida.item_id);
                cantAciertos++;
            }
            break;
        case (1):
            if (strcmp(reg_salida.articulo, entrada->value) == 0)
            {
                strcpy(hits[cantAciertos].articulo, reg_salida.articulo);
                strcpy(hits[cantAciertos].marca, reg_salida.marca);
                strcpy(hits[cantAciertos].producto, reg_salida.producto);
                strcpy(hits[cantAciertos].item_id, reg_salida.item_id);
                cantAciertos++;
            }
            break;
        case (2):
            if (strcmp(reg_salida.producto, entrada->value) == 0)
            {
                strcpy(hits[cantAciertos].articulo, reg_salida.articulo);
                strcpy(hits[cantAciertos].marca, reg_salida.marca);
                strcpy(hits[cantAciertos].producto, reg_salida.producto);
                strcpy(hits[cantAciertos].item_id, reg_salida.item_id);
                cantAciertos++;
            }
            break;
        case (3):

            if (strcmp(reg_salida.marca, entrada->value) == 0)
            {
                strcpy(hits[cantAciertos].articulo, reg_salida.articulo);
                strcpy(hits[cantAciertos].marca, reg_salida.marca);
                strcpy(hits[cantAciertos].producto, reg_salida.producto);
                strcpy(hits[cantAciertos].item_id, reg_salida.item_id);
                cantAciertos++;
            }
            break;
        default:
            break;
        }
    }
    fclose(pf_articulos);
    return cantAciertos;
}
//envia los hits al cliente.
void devolverInfo(t_salida registros[], int cantHits, int fd_salida)
{
    char linea_salida1[230] = {0}; //230 caracteres es el maximo!
    int i = 0;
    for (i = 0; i < cantHits; i++)
    {
        memset(linea_salida1, 0, 230);
        linea_salida1[0] = '\0';
        sprintf(linea_salida1, "%s - %s - %s - %s\n", registros[i].item_id, registros[i].marca, registros[i].producto, registros[i].articulo);
        write(fd_salida, linea_salida1, sizeof(linea_salida1));
    }
    int end = 16;
    memset(linea_salida1, 0, 230);
    linea_salida1[0] = '\0';
    sprintf(linea_salida1, "%d\n", end);
    write(fd_salida, linea_salida1, sizeof(linea_salida1));
}
void daemonize()
{
    int i, lfp;
    char str[10];

    if (getppid() == 1)
        return;
    i = fork();

    if (i < 0)
        exit(1);
    if (i > 0)
        exit(0);
    setsid();

    for (i = getdtablesize(); i >= 0; --i)
        close(i);

    i = open("/dev/null", O_RDWR);
    dup(i);
    dup(i);
    umask(027);
    lfp = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
    if (lfp < 0)
        exit(1);
    if (lockf(lfp, F_TLOCK, 0) < 0)
        exit(1);

    sprintf(str, "%d\n", getpid());
    write(lfp, str, strlen(str));

    signal(SIGCHLD, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
}
void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGHUP:
        log_message(LOG_FILE, "Hangup Signal Catched");
        break;
    case SIGTERM:
        log_message(LOG_FILE, "Terminate Signal Catched");
        exit(0);
        break;
    }
}
void log_message(char *filename, char *message)
{
    FILE *logfile;
    logfile = fopen(filename, "a");
    if (!logfile)
        return;
    fprintf(logfile, "%s", message);
    fclose(logfile);
}
void mostrar_ayuda()
{
    printf("Debe Ingresar el path de entrada y de salida del fifo (nombres del fifo incluidos)\n");
    printf("Ejemplo: ./Servidor /home/Desktop/entrada    /home/Desktop/salida  \n");
    printf("El path es Case Sensitive! /desktop no es igual a /Desktop\n");
    printf("Utilice Slash hacia delante (/) para el path \n");
}
int pideAyuda(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        return PIDE_AYUDA;
    }
    return CONTINUAR;
}

int parametrosValidos(int argc, char **argv)
{
    if (argv[2] == NULL || argv[1] == NULL || argc != 3)
    {
        printf("ERROR PARAMETROS\n");
        return FAILURE;
    }
    return SUCCESS;
}
void enviarTextoAlCliente(char *cad)
{

    if (cad == NULL)
        return;

    char linea[150] = {0};
    strcpy(linea, cad);

    write(fd_salida, cad, strlen(linea) + 1);
    return;
}
int splitMovimiento(t_entrada *entrada, char *linea)
{
    t_entrada entrada_temp;
    char l[TAM_CAD];
    strcpy(l, linea);
    char *aux;
    aux = strrchr(l, '\n');
    if (aux)
        *aux = '\0';

    aux = strrchr(l, '=');
    if (aux == NULL)
    {
        enviarTextoAlCliente("\nATENCION: Esta ingresando mal los datos. Debe ser KEY=VALUE\n");
        mostrar_ayuda();
        return FAILURE;
    }

    strcpy(entrada_temp.value, aux + 1);
    *aux = '\0';
    strcpy(entrada_temp.key, l);

    if (strlen(entrada_temp.key) == 0 || strlen(entrada_temp.value) == 0)
    {
        enviarTextoAlCliente("Algunos de los campos estaba vacio. Debe ingresar algo para buscar\n");
        return FAILURE;
    }
    strcpy(entrada->key, entrada_temp.key);
    strcpy(entrada->value, entrada_temp.value);

    return SUCCESS;
}
