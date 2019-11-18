#include "servidor.h"
#include "funcionesServidor.h"
#include "struct.h"

/************INTEGRANTES **************/
/*
*/

pthread_mutex_t mutex;
int enEjecucion = 1;

int main(int argc, char const *argv[])
{
	int server_fd, client_socket, id = 1;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	t_conexionCliente conexionCliente;
	t_param param;

	//AYUDA
	if(argc == 2 && (strcmp(argv[1],"-h"))==0){
        ayuda();
        exit(0);
	}
	
	if(argc > 3){
		printf("Parámetros de mas!");
		printf("Para mas informacion solicite la ayuda con -h");
		exit(0);
	}

	strcpy(param.pathDatabase,argv[1]);

	param.port = atoi(argv[2]);

	//Inicializo el mutex
	if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("\n Mutex init has failed\n");
        return -1;
    }
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(param.port); //ACA VA EL PUERTO


	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	conexionCliente.archivo = fopen(param.pathDatabase, "r");
	if(!conexionCliente.archivo){
		puts("Error al leer el archivo. Es posible que no exista o no tenga permisos de lectura");
		exit(3);
	}

	while(1) {

        printf("[...] Esperando conexión de algún cliente. [...] \n");

        //Despues del accept hacer un thread
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("[!] Se ha conectado un cliente.\n");

        //Asigno valores a la estructura
        conexionCliente.id=id++;
        conexionCliente.socket = client_socket;
        conexionCliente.archivo =  fopen(param.pathDatabase, "r");
		if(!conexionCliente.archivo){
			puts("Error al leer el archivo. Es posible que no exista o no tenga permisos de lectura");
			exit(3);
		}
        //Creo hilo
        pthread_create(&(conexionCliente.threadId),NULL, reciveAndSendInServer, (void*)&conexionCliente);

	}
	
	fclose(conexionCliente.archivo);
	close(client_socket);
    pthread_mutex_destroy(&mutex);

	return 0;
}

void ayuda(){
	
	puts("Esto es un Servidor que estará a la espera de consultas realizadas por los clientes");
    puts("Recibe como parametros el nombre del archivo y el puerto");
    puts("Ejemplo de ejecucion: ./Servidor articulos.txt 9000 ");
    exit(3);
	
}