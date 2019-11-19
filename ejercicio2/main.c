/*
***********************************************************************************
 Nombre Del Script:        Ejercicio2
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           2
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
#include <unistd.h>
#include <string.h>
#include <pthread.h>

double res = 0;
int pos = 0;
int sumXthreads = 0, sumXthreadsRest = 0;
int contHilos = 0;
int cantThreads = 0;
int cantDatos = 0;
pthread_mutex_t mutex1;

typedef struct entrada
{
	double valor1;
	double valor2;
} t_dato;

int validarParametros(int argc, char *argv[]);
int leerDatos(t_dato datos[], char *path);
void mostrarDatos(t_dato datos[]);
void guardarRes(double res, char *pathRes);

void *suma(void *args)
{

	int t, ini, fin;
	float mysum = 0;

	t_dato *datos = (t_dato *)args;

	pthread_mutex_lock(&mutex1);

	ini = pos;
	fin = ini + sumXthreads;
	printf("Hilo_______________________________________ %d\n", contHilos + 1);

	if (sumXthreadsRest != 0)
	{
		fin = fin + 1;
		sumXthreadsRest--;
	}

	for (t = ini; t < fin; t++)
	{

		mysum += datos[t].valor1 + datos[t].valor2;

		printf("RESULTADO PARCIAL DEL HILO %d: %lf\n", contHilos + 1, mysum);
		pos++;
	}

	res += mysum;
	printf("CANTIDAD DE SUMAS DEL HILO %d: %d\n", contHilos + 1, fin - ini);
	printf("RESULTADO TOTAL PARCIAL DE %d HILOS %lf\n", contHilos + 1, res);
	contHilos++;
	printf("_______________________________________________\n\n");
	//sleep(1);
	pthread_mutex_unlock(&mutex1);

	pthread_exit(NULL);

	return (void *)1;
}

int main(int argc, char *argv[])
{
	if (validarParametros(argc, argv) == 0)
	{
		printf("Parametros invalidos.Utilize -help para mas ayuda.\n");
		return 0;
	}

	if (strcmp(argv[1], "-help") == 0)
	{
		printf("Este programa suma 2 vectores almacenados en un archivo .txt diviendo el trabajo en n threads pasado por parametro junto con el path del archivo de datos y el path de salida.\n");
		printf("Ejemplo de ejecucion: ./ejercicio2 [cantThreads] [pathDatos] [pathResultado] \n");
		return 0;
	}
	int t;
	t_dato datos[500];
	char path[500], pathRes[500];
	pthread_t *tid;

	strcpy(path, argv[2]);
	strcpy(pathRes, argv[3]);

	cantDatos = leerDatos(datos, path);

	pthread_mutex_init(&mutex1, NULL);

	cantThreads = atoi(argv[1]);
	if (cantThreads > cantDatos)
	{

		printf("ERROR LA CANTIDAD MAXIMA DE THREADS ES %d\n", cantDatos);
		return 0;
	}

	mostrarDatos(datos);

	sumXthreads = cantDatos / cantThreads;
	sumXthreadsRest = cantDatos % cantThreads;

	tid = (pthread_t *)calloc(cantThreads, sizeof(pthread_t));

	for (t = 0; t < cantThreads; t++)
	{

		pthread_create(tid + t, NULL, &suma, datos);
	}
	for (t = 0; t < cantThreads; t++)
	{

		pthread_join(tid[t], NULL);
	}

	printf("Cantidad de threads:%d\n", cantThreads);
	printf("RESULTADO TOTAL:%lf\n", res);

	guardarRes(res, pathRes);

	return 0;
}

int validarParametros(int argc, char *argv[])
{
	if ((/*strcmp(argv[1],"-help")!=0 ||*/ argc == 1 || argc == 3 || argc > 4 || (argc == 2 && strcmp(argv[1], "-help") != 0)))
	{

		printf("Error en parametros.\n");
		return 0;
	}

	return 2;
}

int leerDatos(t_dato datos[], char *path)
{

	FILE *fp;
	int i = 0;

	fp = fopen(path, "rt");

	if (fp == NULL)
	{
		printf("ERROR AL ABRIR EL ARCHIVO");
		return 0;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%lf,%lf", &datos[i].valor1, &datos[i].valor2);
		i = i + 1;
	}
	fclose(fp);
	return i - 1;
}

void mostrarDatos(t_dato datos[])
{
	int i;
	printf("DATOS____________________________\n");
	for (i = 0; i < cantDatos; i++)
	{
		printf("Valor 1:%.2lf   Valor 2: %.2lf\n", datos[i].valor1, datos[i].valor2);
	}
	printf("\n\n\n");
}
void guardarRes(double res, char *pathRes)
{

	FILE *fp;

	fp = fopen(pathRes, "wt");

	fprintf(fp, "%lf", res);

	fclose(fp);
}
