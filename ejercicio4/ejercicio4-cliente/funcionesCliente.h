#ifndef FUNCIONESCLIENTE_H_INCLUDED
#define FUNCIONESCLIENTE_H_INCLUDED

#define cls() printf("\033[H\033[J") // FUNCION PARA LIMPIAR PANTALLA
#define ES_LETRA(x) ((x > 64 && x < 91) || (x > 96 && x < 123)) ? 1 : 0
#define ES_NUMERO(x) ((x > 47 && x < 58)) ? 1 : 0
#define MARCA "MARCA"
#define PRODUCTO "PRODUCTO"
#define ID "ID"

void obtenerRegistros(sem_t * puedeConsultar,sem_t * puedeEnviar,sem_t *, sem_t *, sem_t *, t_msgCliente*, t_msgServidor*, char*);
int normalizar(char*);
void explode(const char *src, const char *tokens, char ***list, size_t *len);

#endif // FUNCIONESCLIENTE_H_INCLUDED
