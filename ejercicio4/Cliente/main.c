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
int shmid;
char *memoriaCompartida;

sem_t *semaforoMemoria;
sem_t *queryRecibida;
sem_t *respuestaEnviada;
sem_t *semaforoConsulta;
sem_t *semaforoEnvio;

int main(int argc, char *argv[])
{
    char respuesta[512] = "";

    ///Primero hago una comprobacion de la cantidad de argumentos
    if (argc < 2)
    {
        printf("\nIngrese una consulta por favor.");
        return 1;
    }

    if (argc > 2)
    {
        printf("\nIngrese un unico parametro.");
        return 2;
    }

    ///Si estoy pidiendole la help
    if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0)
    {
        help();
    }

    ///Inicializo el area de memoria compartida
    shmid = shmget(234, TAM_CONSULTA, IPC_CREAT | 0666);
    memoriaCompartida = shmat(shmid, NULL, 0);
    semaforoMemoria = sem_open("/semaforoMemoria", 0);
    queryRecibida = sem_open("/queryRecibida", 0);
    respuestaEnviada = sem_open("/respuestaEnviada", 0);
    semaforoConsulta = sem_open("/semaforoConsulta", 0);
    semaforoEnvio = sem_open("/semaforoEnvio", 0);

    hacerConsulta(argv[1]);

    while (strcmp(respuesta, "FIN") != 0)
    {
        printf("%s\n", respuesta);

        sem_wait(respuestaEnviada);
        sem_wait(semaforoMemoria);
        strncpy(respuesta, memoriaCompartida, sizeof(respuesta));
        sem_post(semaforoMemoria);
        sem_post(semaforoEnvio);
    }

    sem_close(semaforoMemoria);
    sem_close(respuestaEnviada);
    sem_close(queryRecibida);
    sem_close(semaforoConsulta);
    sem_close(semaforoEnvio);
}

void hacerConsulta(char *query)
{
    sem_wait(semaforoConsulta);
    sem_wait(semaforoMemoria);
    strcpy(memoriaCompartida, query);
    sem_post(semaforoMemoria);
    sem_post(queryRecibida);
}
