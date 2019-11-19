#ifndef FUNCTIONSCLIENT_INCLUDED
#define FUNCTIONSCLIENT_INCLUDED
#include "struct.h"

void *reciveAndSendInClient(void *args);

typedef struct {
	int coincidencias;
    char item_id[100][50];
    char articulo[100][60];
    char producto[100][60];
    char marca[100][60];
}t_articulo;

#endif // FUNCTIONSCLIENT_INCLUDED
