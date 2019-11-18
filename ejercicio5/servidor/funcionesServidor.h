#ifndef FUNCTIONS_H_INCLUDED
#include "servidor.h"
#include "struct.h"
#include "stdlib.h"
#include <string.h>

typedef struct {
	int coincidencias;
    char item_id[100][50];
    char articulo[100][60];
    char producto[100][60];
    char marca[100][60];
}t_articulo;

void *reciveAndSendInServer(void *args);
void* resolverConsulta(t_conexionCliente*, t_request*, FILE*);

#endif // FUNCTIONS_H_INCLUDED
