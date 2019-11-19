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

int shmid;
char *espacio;
sem_t *semaforoMemoria;
sem_t *queryRecibida;
sem_t *respuestaEnviada;
sem_t *semaforoQuery;
sem_t *semaforoEnvio;
char respuesta[512] = "";

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        printf("Por favor, ingrese la consulta que quiere realizar\n");
        printf("con el formato PARAMETRO=VALOR\n");
        printf("Ejemplos:\n");
        printf("\t\t./Cliente PRODUCTO=BOLSA\n");
        printf("\t\t./Cliente ID=12\n");
        printf("\t\t./Cliente MARCA=MIMAGO\n");

        exit(255);
    }

    if (argc > 2)
    {
        printf("\nIngrese un unico parametro.");
        return 2;
    }

    ///Si estoy pidiendole la getHelp
    if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0)
    {
        getHelp();
    }

    ///Inicializo el area de memoria compartida
    shmid = shmget(234, TAMQUERY, IPC_EXCL | 0666);

    if (shmid < 0)
    {
        printf("\nEl servidor no esta funcionando\n");
        return 10;
    }

    espacio = shmat(shmid, NULL, 0);

    semaforoMemoria = sem_open("/semaforoMemoria", 0);
    respuestaEnviada = sem_open("/respuestaEnviada", 0);
    queryRecibida = sem_open("/queryRecibida", 0);
    semaforoQuery = sem_open("/semaforoQuery", 0);
    semaforoEnvio = sem_open("/semaforoEnvio", 0);

    hacerConsulta(argv[1]);
    printf("RESULTADOS\n==========");
    ///ENTRO EN UN LOOP INFINITO HASTA TERMINAR DE PROCESAR
    ///EL SERVIDOR ME AVISA CUANDO YA ESTA
    while (strcmp(respuesta, "FIN") != 0)
    {
        operacionesSemaforos(ENVIAR_RECIBIR_QUERY);
    }

    operacionesSemaforos(CERRAR_SEMAFOROS);
}

void operacionesSemaforos(int num)
{
    switch (num)
    {
    case ENVIAR_RECIBIR_QUERY:
        printf("%s\n", respuesta);
        sem_wait(respuestaEnviada);
        sem_wait(semaforoMemoria);
        strcpy(respuesta, espacio);
        sem_post(semaforoMemoria);
        sem_post(semaforoEnvio);
        break;
    case CERRAR_SEMAFOROS:
        sem_close(semaforoMemoria);
        sem_close(respuestaEnviada);
        sem_close(queryRecibida);
        sem_close(semaforoQuery);
        sem_close(semaforoEnvio);
    default:
        break;
    }
}

void hacerConsulta(char *query)
{
    ///PIDO HACER LA CONSULTA
    sem_wait(semaforoQuery);
    ///PIDO MEMORIA
    sem_wait(semaforoMemoria);
    ///COPIO EL VALOR DE LA QUERY
    strcpy(espacio, query);
    ///DEVUELVO LOS SEMAFOROS
    sem_post(semaforoMemoria);
    sem_post(queryRecibida);
}
