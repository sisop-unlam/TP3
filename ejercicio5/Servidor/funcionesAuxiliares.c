#include "funciones.h"
/*
***********************************************************************************
 Nombre Del Script:        Servidor
 Trabajo Practico Nro.:    3
 Ejercicio Nro.:           5
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

int obtenerIP(char * ip){
        struct ifaddrs *iflist, *iface;

    if (getifaddrs(&iflist) < 0) {
        perror("getifaddrs");
        return 1;
    }

    for (iface = iflist; iface; iface = iface->ifa_next) {
        int af = iface->ifa_addr->sa_family;
        const void *addr;
        char addrp[INET6_ADDRSTRLEN];

        switch (af) {
            case AF_INET:
                addr = &((struct sockaddr_in *)iface->ifa_addr)->sin_addr;
                break;
            case AF_INET6:
                addr = &((struct sockaddr_in6 *)iface->ifa_addr)->sin6_addr;
                break;
            default:
                addr = NULL;
        }

        if (addr) {
            if (inet_ntop(af, addr, addrp, sizeof addrp) == NULL) {
                perror("inet_ntop");
                continue;
            }
            if(!strcmp(iface->ifa_name, "lo") == 0){
            printf("La direccion del servidor es %s\n", addrp);
            strcpy(ip, addrp);
            return 0;
            }
        }
    }

    freeifaddrs(iflist);
    return 1;
}
char *strdup(const char *src)
{
    char *tmp = malloc(strlen(src) + 1);
    if (tmp)
        strcpy(tmp, src);
    return tmp;
}

void explode(const char *src, const char *tokens, char ***list, size_t *len)
{
    if (src == NULL || list == NULL || len == NULL)
        return;

    char *str, *copy, **_list = NULL, **tmp;
    *list = NULL;
    *len = 0;

    copy = strdup(src);
    if (copy == NULL)
        return;

    str = strtok(copy, tokens);
    if (str == NULL)
        goto free_and_exit;

    _list = realloc(NULL, sizeof *_list);
    if (_list == NULL)
        goto free_and_exit;

    _list[*len] = strdup(str);
    if (_list[*len] == NULL)
        goto free_and_exit;
    (*len)++;

    while ((str = strtok(NULL, tokens)))
    {
        tmp = realloc(_list, (sizeof *_list) * (*len + 1));
        if (tmp == NULL)
            goto free_and_exit;

        _list = tmp;

        _list[*len] = strdup(str);
        if (_list[*len] == NULL)
            goto free_and_exit;
        (*len)++;
    }

free_and_exit:
    *list = _list;
    free(copy);
}
