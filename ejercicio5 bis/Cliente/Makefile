all : Cliente

Cliente : main.c funciones.o
	gcc -o Cliente main.c funciones.o -lpthread -lrt

funciones.o: funciones.h funciones.c
	gcc -c funciones.c -lpthread -lrt
	
clean :
	rm *.o
