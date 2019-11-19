/*
***********************************************************************************
 Nombre Del Script:        Ejercicio1
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           1
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
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>
#define CANT_HIJOS 2
#define MENSAJE_ERROR_FORK "ERROR AL CREAR EL PROCESO"

void mostrarAyuda(){
	printf("\n--------------------------AYUDA---------------------------\n");
	printf("Para ejecutar el script utilizar algunos de los siguientes parametros\n");
	printf("-p:Se generan 2 procesos hijos,3 procesos nietos,5 bisnietos,2 procesos zombies y 3 procesos demonios\n");
	printf("-h o -H:Para obtener ayuda del mismo\n");
	printf("PID: pppp PPID: nnnn Parentesco-Tipo: [hijo/nieto]-[normal/demonio/zombie]\n");
}

int nuevoHijo(int nroHijo,const char*tipo){
	pid_t pid=fork();

	if(pid==0){
		printf("\n");
		printf("PID: %d PPID: %d Parentesco-Tipo: %s\n",getpid(),getppid(),tipo);
		exit(0);
		
	}else if(pid>0){
		return pid;
	}else{
		if(pid==-1){
			return -1;
			printf(MENSAJE_ERROR_FORK);
		}
		
	}

	return 0;
}

int crearZombies(int Nro,char*tipo){
	

	nuevoHijo(Nro,tipo);
	nuevoHijo(Nro,tipo);
	///nuevoHijo(Nro,tipo);
	return 0;
	
}
int crearDemonios(int Nro,char*tipo){
	
}
int main(int argc,char*argv[])
{
	int i,j,k;


	if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"-H")==0)
		mostrarAyuda();
	if(strcmp(argv[1],"-p")==0){

		crearZombies(2,"hijo-zombie");

		for(i=0;i<CANT_HIJOS;i++){
			if(fork()==0){
				
				if(i==0){
					for(j=0;j<CANT_HIJOS;j++){
						pid_t pp=fork();
						if(pp==0){
							for(k=0;k<CANT_HIJOS;k++){
								
								if(fork()==0){
									printf("PID: %d PPID: %d Parentesco-Tipo: %s\n",getpid(),getppid(),"nieto-normal");
									break;
								}
							}
							
							break;
						}
					}
				}
				if(i==1){
					if(fork()==0){
						if(fork()==0){
							printf("PID: %d PPID: %d Parentesco-Tipo: %s\n",getpid(),getppid(),"nieto-normal");
							break;
						} 
						printf("PID: %d PPID: %d Parentesco-Tipo: %s\n",getpid(),getppid(),"hijo-normal");
						break;
					}
				}
					printf("PID: %d PPID: %d Parentesco-Tipo: %s\n",getpid(),getppid(),"hijo-normal");
					break;
				}
			}
				
			}
	while(1);

	return 0;
		
}
