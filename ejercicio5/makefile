all: servidor.c cliente.c

funcionesServidor.o: ./servidor/funcionesServidor.h ./servidor/funcionesServidor.c
	gcc -Wall -g -c ./servidor/funcionesServidor.o ./servidor/funcionesServidor.c

funcionesCliente.o: ./cliente/funcionesCliente.h ./cliente/funcionesCliente.c
	gcc -Wall -g -c ./cliente/funcionesCliente.o ./cliente/funcionesCliente.c

servidor.c: ./servidor/servidor.c ./servidor/servidor.h ./servidor/funcionesServidor.o 
	gcc -Wall -g -o ./servidor/servidor ./servidor/servidor.c ./servidor/funcionesServidor.o -lpthread

cliente.c: ./cliente/cliente.c ./cliente/cliente.h ./cliente/funcionesCliente.o
	gcc -Wall -g -o ./cliente/cliente ./cliente/cliente.c ./cliente/funcionesCliente.o

clean:
	rm *.o
