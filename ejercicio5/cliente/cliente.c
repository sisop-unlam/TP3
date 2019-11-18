#include "cliente.h"
#include "funcionesCliente.h"
#include "struct.h"
/************INTEGRANTES **************/
/*TRABAJO PRACTICO Nº 3
    
*/

int main(int argc, char const *argv[]) {
	
    struct sockaddr_in serv_addr;
    t_client client;
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
	
	strcpy(param.ipServer,argv[1]);
	param.port = atoi(argv[2]);
	
    if ((client.socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(param.port);
    //getHostByName
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, param.ipServer, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(client.socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }else{
        printf("Conexión exitosa!\n");
    }

    reciveAndSendInClient((void*)&client);

    return 0;

}

void ayuda() {
	
    puts("Programa Cliente para enviarle consultas al archivo Servidor");
    puts("Los parametros que recibe son la IP donde se encuentra el servidor y el PUERTO");
	puts("Ejemplo de ejecucion: .\\Cliente 192.168.0.1 9000");
    puts("Despues podrá realizar las consulta con el formato CAMPO=VALOR");
	puts("Y para finalizar solo debe escribir QUIT");
    puts("Ejemplos de ejecución:");
    puts("\tID=4444");
    puts("\tMARCA=GEORGALOS");
    puts("\tPRODUCTO=HELADO");
    exit(3);
}