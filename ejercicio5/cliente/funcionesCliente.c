#include "cliente.h"
#include "struct.h"
#include "funcionesCliente.h"
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp);
void *reciveAndSendInClient(void *args)
{

	t_client *dataClient = (t_client *)args;
	t_request request;
	t_articulo articulos;
	char *tipo;
	char *valor;
	char delimitador[2] = "=";

	printf("Por favor ingrese su consulta: ");
	fgets_wrapper(dataClient->consulta, sizeof(dataClient->consulta), stdin);

	while (strcmp(dataClient->consulta, "QUIT") != 0)
	{

		tipo = strtok(dataClient->consulta, delimitador);
		valor = strtok(NULL, delimitador);

		strcpy(request.campo, tipo);
		strcpy(request.valor, valor);
		printf("\nTIPO: %s - VALOR: %s\n", request.campo, request.valor);
		send(dataClient->socket, (void *)&request, sizeof(t_request), 0); //Envio consulta al servidor

		read(dataClient->socket, (void *)&articulos, sizeof(t_articulo)); //Recibo respuesta del servidor

		if (articulos.coincidencias > 0)
		{

			printf("Se encontraron %d coincidencias:\n", articulos.coincidencias);
			for (int i = 0; i < articulos.coincidencias - 1; i++)
			{
				printf("Id: %s\tArticulos: %s\tProducto: %s\tMarca: %s\n", articulos.item_id[i], articulos.articulo[i], articulos.producto[i], articulos.marca[i]);
			}
		}
		else
		{
			puts("Su busqueda no produjo coincidencias o los campos no son validos\n");
			fflush(stdout);
		}

		printf("Por favor ingrese una nueva consulta o ingrese QUIT para terminar: ");
		fflush(stdout);

		fflush(stdin);
	fgets_wrapper(dataClient->consulta, sizeof(dataClient->consulta), stdin);
	}

	printf("Saliendo del sistema\n");

	return 0;
}
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp)
{
    if (fgets(buffer, buflen, fp) != 0)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[len-1] = '\0';
        return buffer;
    }
    return 0;
}