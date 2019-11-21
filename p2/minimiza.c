#include "minimiza.h"
#include "int_list.h"
#include "state_list.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

IntList *obtenerEstadosAccesibles(AFND *afd, bool debug);
StateList *obtenerEstadosReducidos(AFND *afd, IntList *estados_accesibles, bool debug);

AFND *AFNDMinimiza(AFND *afd, bool debug) {
    IntList *estados_accesibles;
    StateList *estados_reducidos;
    int i;

    if (afd == NULL) return NULL;

    estados_accesibles = obtenerEstadosAccesibles(afd, debug);
    if (estados_accesibles == NULL) return NULL;

    if (debug) {
        printf("Lista de estados accesibles:\n");
        for (i = 0; i < IntListSize(estados_accesibles); i++) {
            printf(" -> %s\n", AFNDNombreEstadoEn(afd, IntListGet(estados_accesibles, i)));
        }
    }

    estados_reducidos = obtenerEstadosReducidos(afd, estados_accesibles, debug);
    if (estados_reducidos == NULL) {
        IntListFree(estados_accesibles);
        return NULL;
    }
    
    /* Construir nuevo AFD */

    StateListFree(estados_reducidos);
    IntListFree(estados_accesibles);

    return NULL;
}

IntList *obtenerEstadosAccesibles(AFND *afd, bool debug) {
    IntList *estados_accesibles;
    int estado_actual, i, j, k;

    estados_accesibles = IntListCreate();
    if (estados_accesibles == NULL) return NULL;

    IntListAdd(estados_accesibles, AFNDIndiceEstadoInicial(afd));

    for (i = 0; i < IntListSize(estados_accesibles); i++) {
        estados_accesibles = IntListGet(estados_accesibles, i);
        for (j = 0; j < AFNDNumSimbolos(afd); j++) {
            for (k = 0; k < AFNDNumEstados(afd); k++) {
                if (AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado_actual, j, k)) {
                    if (!IntListContains(estados_accesibles, k)) IntListAdd(estados_accesibles, k);
                    break;
                }
            }
        }
    }

    IntListSort(estados_accesibles);

    return estados_accesibles;
}

StateList *obtenerEstadosReducidos(AFND *afd, IntList *estados_accesibles, bool debug) {
    StateList *estados_reducidos;
    IntList *lista_aux;
    int i;

    estados_reducidos = StateListCreate();
    if (estados_reducidos == NULL) return NULL;

    /* Por hacer */

    return estados_reducidos;
}