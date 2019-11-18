#include "cliente.h"
#include "struct.h"
#include "funcionesCliente.h"


void *reciveAndSendInClient(void *args) {

    t_client *dataClient = (t_client*)args;
    t_request request;
	t_articulo articulos;
	char *tipo;
	char *valor;
	char delimitador[2]="=";

    printf("Por favor ingrese su consulta: ");
    scanf ("%[^\n]s", dataClient->consulta);

    while(strcmp(dataClient->consulta,"QUIT") != 0){
		
		tipo = strtok(dataClient->consulta, delimitador);
		valor = strtok(NULL, delimitador);
		
		strcpy(request.campo, tipo);           
		strcpy(request.valor, valor); 
		
		send(dataClient->socket, (void*)&request, sizeof(t_request), 0); //Envio consulta al servidor
		
		read(dataClient->socket,(void*)&articulos,sizeof(t_articulo)); //Recivo respuesta del servidor
		
		if(articulos.coincidencias > 0){
			
			printf("Se encontraron %d coincidencias:\n", articulos.coincidencias); 
			for (int i = 0; i < articulos.coincidencias; i++) {   
				printf("Id: %s\tArticulos: %s\tProducto: %s\tMarca: %s\n", articulos.item_id[i], articulos.articulo[i], articulos.producto[i], articulos.marca[i]);
			}
			
		}else{
			puts("Su busqueda no produjo coincidencias o los campos no son validos\n");
		}
		
		
		printf("Por favor ingrese una nueva consulta o ingrese QUIT para terminar: ");
		fflush(stdin);
		scanf ("%[^\n]s", dataClient->consulta);		

    }
	
	printf("Saliendo del sistema\n");
	
    return 0;

}


