#include "funciones.h"
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
char bd[500];
int shmid;
char *memoriaCompartida;

sem_t *semaforoMemoria;
sem_t *queryRecibida;
sem_t *respuestaEnviada;
sem_t *semaforoConsulta;
sem_t *semaforoEnvio;

FILE *arch;

int main(int argc, char *argv[])
{
    char consulta[TAM_CONSULTA];

    ///Monitoreo si recibo las senales para finalizar el servidor.
    signal(SIGINT, liberarRecursos);
    signal(SIGTERM, liberarRecursos);

    ///Primero se comprueba que el archivo de la base de datos exista.
    if (access(BD, F_OK) != -1)
        strcpy(bd, BD);
    else
    {
        printf("El archivo de la base de datos no existe");
        return 20;
    }

    ///Inicializo el area de memoria compartida
    shmid = shmget(234, TAM_CONSULTA, IPC_CREAT | 0666);
    memoriaCompartida = shmat(shmid, NULL, 0);

    ///Inicializo los semaforos
    semaforoMemoria = sem_open("/semaforoMemoria", O_CREAT, 0666, 1);
    semaforoEnvio = sem_open("/semaforoEnvio", O_CREAT, 0666, 1);
    semaforoConsulta = sem_open("/semaforoConsulta", O_CREAT, 0666, 1);
    respuestaEnviada = sem_open("/respuestaEnviada", O_CREAT, 0666, 0);
    queryRecibida = sem_open("/queryRecibida", O_CREAT, 0666, 0);

    printf("\nMemoria compartiada inicializada");
    printf("\nEsperando input...");
    fflush(stdout);

    ///Abro el archivo
    arch = fopen(BD, "rt");
    if (!arch)
    {
        return 1;
    }

    while (1)
    {
        ///Quedo iterando hasta que me llama el cliente
        sem_wait(queryRecibida);
        ///Leo la consulta
        sem_wait(semaforoMemoria);
        strncpy(consulta, memoriaCompartida, sizeof(consulta));
        sem_post(semaforoMemoria);
        ///Proceso
        procesarConsulta(consulta, memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio, arch);
        ///Vuelvo a aceptar consultas por parte de los clientes
        sem_post(semaforoConsulta);
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
    sem_close(semaforoConsulta);
    sem_close(semaforoEnvio);

    ///Liberacion de semaforos.
    sem_unlink("/semaforoMemoria");
    sem_unlink("/queryRecibida");
    sem_unlink("/semaforoConsulta");
    sem_unlink("/semaforoEnvio");
    sem_unlink("/respuestaEnviada");

    exit(5);
}
