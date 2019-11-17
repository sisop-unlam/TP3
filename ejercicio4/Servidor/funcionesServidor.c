#include "funciones.h"
/*
***********************************************************************************
 Nombre Del Script:        Servidor
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           4
 Entrega Nro.:             1
 Integrantes:
    Apellido            Nombre                  DNI
    --------------------------------------------------
   Krasuk               Joaquín               40745090
   Rodriguez            Christian             37947646
   Vivas                Pablo                 38703964
   Ramos		        Maximiliano		      35068917
   Fernández		    Jonathan		      37226233
***********************************************************************************
*/
void procesarConsulta(char *consulta, char *memoriaCompartida, sem_t *semaforoMemoria, sem_t *respuestaEnviada, sem_t *semaforoEnvio, FILE *arch)
{
    char linea[512], *tipo, *valor;
    char **list;
    size_t len;
    int i =0;
    t_articulo art;
    ///para pasar el itemID de int a char
    char bufferItemID[10];

    ///Vuelvo al inicio del archivo y me salteo la primer linea (la del encabezado)
    fseek(arch, 0, SEEK_SET);
    fscanf(arch, "%*[^\n]");

    ///Muestro por pantalla la consulta
    printf("\n%s\n", consulta);

    ///Hago un explode, similar al de PHP
    ///En un array dejo el valor y el campo buscado
    explode(consulta, "=", &list, &len);

    ///Primero obtengo el tipo
    tipo = list[0];
    ///Luego el valor buscado
    valor = list[1];

    ///Si tengo alguno de los 3 tipos
    if (strcmp(tipo, "PRODUCTO") == 0 || strcmp(tipo, "ID") == 0 || strcmp(tipo, "MARCA") == 0)
    {

        ///Escaneo linea a linea, matcheando
        while (fscanf(arch, "%d;%[^;];%[^;];%[^;\n] \n", &art.item_id, art.articulo, art.producto, art.marca) != EOF)
        {
            ///Convierto el itemId en un string
            snprintf(bufferItemID, 10, "%d", art.item_id);

            if (strcmp(tipo, "PRODUCTO") == 0 && strcmp(art.producto, valor) == 0)
            {
                strcpy(linea, art.articulo);

                sendMsg(linea, memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio);
                i++;
            }
            else if (strcmp(tipo, "ID") == 0 && strcmp(bufferItemID, valor) == 0)
            {
                strcpy(linea, art.articulo);

                sendMsg(linea, memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio);
                i++;
            }
            else if (strcmp(tipo, "MARCA") == 0 && strcmp(art.marca, valor) == 0)
            {

                strcpy(linea, art.articulo);

                sendMsg(linea, memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio);
                i++;
            }
        }
        printf("Enviados %d registros\n", i);
        fflush(stdout);
        if(!i)
        {
            const char noRows[] = "No se encontraron registros que coincidan con la busqueda.";
            sendMsg(noRows, memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio);
        }
    }
    else
    {
        const char errorPedido[] = "Por favor, ingrese un campo válido (ID, MARCA o PRODUCTO).";
        sendMsg(errorPedido, memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio);
    }

    /// CON ESTO, LE AVISO AL CLIENTE QUE YA NO HAY MAS DATOS PARA PROCESAR
    sendMsg("FIN", memoriaCompartida, semaforoMemoria, respuestaEnviada, semaforoEnvio);
}

void sendMsg(const char *msg, char *memoriaCompartida, sem_t *semaforoMemoria, sem_t *respuestaEnviada, sem_t *semaforoEnvio)
{
    sem_wait(semaforoEnvio);
    sem_wait(semaforoMemoria);
    strcpy(memoriaCompartida, msg);
    sem_post(semaforoMemoria);
    sem_post(respuestaEnviada);
}
