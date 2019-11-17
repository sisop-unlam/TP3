all : Servidor

Servidor : main.c funcionesServidor.o funcionesAuxiliares.o
	gcc -o Servidor main.c funcionesServidor.o funcionesAuxiliares.o -lpthread -lrt

funcionesServidor.o: funciones.h funcionesServidor.c
	gcc -c funcionesServidor.c -lpthread -lrt

funcionesAuxiliares.o: funciones.h funcionesAuxiliares.c
	gcc -c funcionesAuxiliares.c -lpthread -lrt

clean :
	rm *.o
