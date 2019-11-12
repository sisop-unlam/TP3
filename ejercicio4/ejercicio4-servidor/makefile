all : Servidor

Servidor : server.c funcionesServidor.o
	gcc -o Servidor server.c funcionesServidor.o -lpthread -lrt

funcionesServidor.o: estructuras.h funcionesServidor.h funcionesServidor.c
	gcc -c funcionesServidor.c -lpthread -lrt
	
clean :
	rm *.o
