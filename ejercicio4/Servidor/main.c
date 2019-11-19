/*
***********************************************************************************
 Nombre Del Script:        Servidor
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

char bd[500];
int shmid;
char *espacio;

sem_t *semaforoMemoria;
sem_t *queryRecibida;
sem_t *respuestaEnviada;
sem_t *semaforoQuery;
sem_t *semaforoEnvio;

FILE *arch;

int main(int argc, char *argv[])
{
    char consulta[TAMQUERY];
    if (argc <= 1)
    {
        printf("Por favor, ingrese un archivo de base de datos\n");
        printf("Ejemplos:\n");
        printf("\t\t./Servidor db.txt\n");
        printf("\t\t./Servidor ../base_de_datos.txt\n");

        exit(255);
    }

    ///Monitoreo si recibo las senales para finalizar el servidor.
    signal(SIGINT, liberarRecursos);
    signal(SIGTERM, liberarRecursos);

    ///Primero se comprueba que el archivo de la base de datos exista.
    if (access(argv[1], F_OK) != -1)
    {
        strcpy(bd, argv[1]);
    }
    else
    {
        printf("El archivo de la base de datos no existe\n");
        return 20;
    }

    ///Inicializo el area de memoria compartida
    shmid = shmget(234, TAMQUERY, IPC_CREAT | 0666);
    espacio = shmat(shmid, NULL, 0);

    ///Inicializo los semaforos
    semaforoMemoria = sem_open("/semaforoMemoria", O_CREAT, 0666, 1);
    semaforoEnvio = sem_open("/semaforoEnvio", O_CREAT, 0666, 1);
    semaforoQuery = sem_open("/semaforoQuery", O_CREAT, 0666, 1);
    respuestaEnviada = sem_open("/respuestaEnviada", O_CREAT, 0666, 0);
    queryRecibida = sem_open("/queryRecibida", O_CREAT, 0666, 0);

    printf("\nMemoria compartida inicializada");
    printf("\nEsperando input...");
    fflush(stdout);

    ///Abro el archivo
    arch = fopen(bd, "rt");
    if (!arch)
    {
        return 1;
    }

    while (1)
    {
        sem_wait(queryRecibida);
        sem_wait(semaforoMemoria);
        strcpy(consulta, espacio);
        sem_post(semaforoMemoria);
        obtenerTuplas(arch, espacio, respuestaEnviada, semaforoMemoria, semaforoEnvio, consulta);
        sem_post(semaforoQuery);
    }

    return 0;
}

///Funcion lanzada por el handler de senales que libera los semaforos y la memoria compartida.
void liberarRecursos()
{
    printf("\nCerrando semaforos, memoria compartida y archivo\n");

    fclose(arch);

    ///Liberacion de memoria compartida.
    shmctl(shmid, IPC_RMID, NULL);

    ///Cierre de semaforos.
    sem_close(semaforoMemoria);
    sem_close(queryRecibida);
    sem_close(respuestaEnviada);
    sem_close(semaforoQuery);
    sem_close(semaforoEnvio);

    ///Liberacion de semaforos.
    sem_unlink("/semaforoMemoria");
    sem_unlink("/queryRecibida");
    sem_unlink("/semaforoQuery");
    sem_unlink("/semaforoEnvio");
    sem_unlink("/respuestaEnviada");

    exit(5);
}
