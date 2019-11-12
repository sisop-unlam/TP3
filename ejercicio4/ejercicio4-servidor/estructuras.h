#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define BD "./articulos.txt" ///Se define la ruta de la base de datos.
#define cls() printf("\033[H\033[J") /// FUNCION PARA LIMPIAR PANTALLA
#define ES_LETRA(x) ((x > 64 && x < 91) || (x > 96 && x < 123)) ? 1 : 0
#define ES_NUMERO(x) ((x > 47 && x < 58)) ? 1 : 0
#define MARCA "MARCA"
#define PRODUCTO "PRODUCTO"
#define ID "ID"

typedef struct
{
    int item_id;
    char articulo[50];
    char producto[50];
    char marca[50];
} t_articulo;

typedef struct
{
    int item_id;
    char parametro[50];
    char parametroBuscado[50];
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
