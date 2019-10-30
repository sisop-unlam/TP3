#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define CANTPARDATOS 10
double res=0;
int pos=0;
int sumXthreads=0,sumXthreadsRest=0;
int contHilos=0;
int cantThreads=0;
pthread_mutex_t mutex1;

typedef struct entrada{
double valor1;
double valor2;
}t_dato;

int validarParametros(int argc, char *argv[]);
int leerDatos(t_dato datos[]);
void mostrarDatos(t_dato datos[]);



void * suma(void *args){
	
	int t,ini,fin;
	float mysum=0;
	
	t_dato *datos = (t_dato *) args;
	
	pthread_mutex_lock(&mutex1);
	
	ini=pos;
	fin=ini+sumXthreads;
	    printf("Hilo_______________________________________ %d\n",contHilos+1);
	
	if(sumXthreadsRest!=0){
		fin=fin+1;
		sumXthreadsRest--;
		}
	
	
	
	for (t=ini; t<fin ;t++){
		
		mysum+=datos[t].valor1+datos[t].valor2;
		
		printf("RESULTADO PARCIAL DEL HILO %d: %lf\n",contHilos+1,mysum);
		pos++;
		
	}
	    
	    
		res+=mysum;
		printf("CANTIDAD DE SUMAS DEL HILO %d: %d\n",contHilos+1,fin-ini);
		printf("RESULTADO TOTAL PARCIAL DE %d HILOS %lf\n",contHilos+1,res);
		contHilos++;
		printf("_______________________________________________\n\n");
		sleep(1);
		pthread_mutex_unlock(&mutex1);
		
		pthread_exit(NULL);
		
	   

return (void *)1;
}





int main(int argc, char *argv[])
{
	if(validarParametros(argc, argv) == 0){
	printf("Parametros invalidos.Utilize -help para mas ayuda.\n");
	return 0;
	}
	
	if(strcmp(argv[1],"-help")==0){
	printf("Este programa suma 2 vectores almacenados en un archivo datos.txt diviendo el trabajo en n threads pasado por parametro.\n");
	printf("Ejemplo de ejecucion: ./ejercicio2 4(cantidad de threads)\n");
	return 0;
	}
	
	
	t_dato datos[CANTPARDATOS];
	
	int t;
	
	pthread_mutex_init(&mutex1, NULL);
	
	pthread_t *tid;

	
	
	cantThreads = atoi(argv[1]);
	if(cantThreads > CANTPARDATOS){
	
	printf("ERROR LA CANTIDAD MAXIMA DE THREADS ES %d\n",CANTPARDATOS);
	return 0;
	}
	
	leerDatos(datos);
	mostrarDatos(datos);
	
	sumXthreads= CANTPARDATOS/cantThreads;
	sumXthreadsRest= CANTPARDATOS%cantThreads;
    
	
	tid = (pthread_t*) calloc(cantThreads, sizeof(pthread_t));
	
	for (t=0; t<cantThreads ;t++) {
    
    pthread_create(tid+t, NULL, &suma, datos);
    
   
  }
   for (t=0; t<cantThreads;t++){
		
    pthread_join(tid[t], NULL);
    
    
  }
	
	
	printf("Cantidad de threads:%d\n",cantThreads);
	printf("RESULTADO TOTAL:%lf\n",res);
	return 0;
}
	
	
	
int validarParametros(int argc, char *argv[])
{
if(argv[1]==NULL || argc!=2){
     printf("Error en parametros.\n");
     return 0;
     }
    return 1;
}

int leerDatos(t_dato datos[])	{

  FILE *fp;
  int i;
  
  fp=fopen("datos.txt","rt");
  
 if(fp==NULL)
{
    printf("ERROR AL ABRIR EL ARCHIVO");
    return 0; 
}

for(i=0; i<CANTPARDATOS ; i++)
{
	fscanf(fp,"%lf,%lf",&datos[i].valor1,&datos[i].valor2);
}
fclose(fp);
return 1;
}

void mostrarDatos(t_dato datos[]){
	int i;
	printf("DATOS____________________________\n");
	for(i=0; i<CANTPARDATOS ; i++){
	printf("Valor 1:%.2lf   Valor 2: %.2lf\n",datos[i].valor1,datos[i].valor2);
	}
	printf("\n\n\n");
}
