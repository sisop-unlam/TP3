all : Cliente

Cliente : cliente.c funcionesCliente.o
	gcc -o Cliente cliente.c funcionesCliente.o -lpthread -lrt

funcionesCliente.o: estructuras.h funcionesCliente.h funcionesCliente.c
	gcc -c funcionesCliente.c -lpthread -lrt
	
clean :
	rm *.o
