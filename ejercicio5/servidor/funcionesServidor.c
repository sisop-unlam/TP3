#include "funcionesServidor.h"
#include "struct.h"

void *reciveAndSendInServer(void *args) {
	
    int bytesRecived;
	
    t_conexionCliente *conexionCliente = (t_conexionCliente*)args;
    t_request request;
	
    bytesRecived = 1;
	
    while(bytesRecived !=0) {
		
        bytesRecived = read(conexionCliente->socket, (void*)&request, sizeof(t_request));

        if(bytesRecived == 0) {
			
            printf("El Cliente salió del sistema\n");
        }
        else
        {
            resolverConsulta(conexionCliente, &request, conexionCliente->archivo);
        }
    }

    return 0;
}


void* resolverConsulta(t_conexionCliente* conexionCliente, t_request* req, FILE *archivo) {
	
	pthread_mutex_lock(&mutex);
	
	fseek(archivo, 0, SEEK_SET);
    fscanf(archivo, "%*[^\n]");
	
	char linea[200];
	char delimitador[]=";\n";
	t_articulo art;
	
	if (strcmp(req->campo, "PRODUCTO") == 0 || strcmp(req->campo, "ID") == 0 || strcmp(req->campo, "MARCA") == 0){
		
		while (feof(archivo) == 0) {
			fgets(linea,200,archivo);					
			char *item_id = strtok(linea,delimitador);		
			char *articulo = strtok(NULL,delimitador);
			char *producto = strtok(NULL,delimitador);
			char *marca = strtok(NULL,delimitador);
			
			if (strcmp(req->campo, "PRODUCTO") == 0 && strcmp(producto, req->valor) == 0)
            {
                strcpy(art.item_id[art.coincidencias], item_id);
				strcpy(art.articulo[art.coincidencias], articulo);
				strcpy(art.producto[art.coincidencias], producto);
				strcpy(art.marca[art.coincidencias], marca);
				art.coincidencias++;
            }
            else if (strcmp(req->campo, "ID") == 0 && strcmp(item_id, req->valor) == 0)
            {
                strcpy(art.item_id[art.coincidencias], item_id);
				strcpy(art.articulo[art.coincidencias], articulo);
				strcpy(art.producto[art.coincidencias], producto);
				strcpy(art.marca[art.coincidencias], marca);
				art.coincidencias++;
            }
            else if (strcmp(req->campo, "MARCA") == 0 && strcmp(marca, req->valor) == 0)
            {

                strcpy(art.item_id[art.coincidencias], item_id);
				strcpy(art.articulo[art.coincidencias], articulo);
				strcpy(art.producto[art.coincidencias], producto);
				strcpy(art.marca[art.coincidencias], marca);
				art.coincidencias++;
            }
		}
		
		send(conexionCliente->socket,(void*)&art,sizeof(t_articulo),0);
		pthread_mutex_unlock(&mutex);	
	}
	
	return 0;
}