#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED


typedef struct
{
    char item_id;
    char articulo[50];
    char producto[50];
    char marca[50];
} t_articulo;

typedef struct
{
    int item_id;
    char materia[50];
    char producto[50];
    char marca[50];
} t_comando;

typedef struct
{
    pid_t pid; ///El cliente manda un identificador al servidor.
    t_comando comando;
} t_msgCliente;

typedef struct
{
    pid_t pid; ///El servidor devuelve el mensaje junto al identificador recibido por el cliente.
    char respuesta[500];
} t_msgServidor;


#endif // ESTRUCTURAS_H_INCLUDED
