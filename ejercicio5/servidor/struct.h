#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

typedef struct{
    int id;
    int socket;
    char consulta[31];
	FILE *archivo;
    pthread_t threadId;
}t_conexionCliente;


typedef struct {
    char campo[10];                   
    char valor[50];
}t_request;


#endif // STRUCT_H_INCLUDED
