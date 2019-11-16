#ifndef FUNCIONESSERVIDOR_H_INCLUDED
#define FUNCIONESSERVIDOR_H_INCLUDED

void procesarConsulta(sem_t *clientePuedeEscribir,sem_t* servidorACliente, t_msgCliente*, t_msgServidor*, char*);

#endif // FUNCIONESSERVIDOR_H_INCLUDED
