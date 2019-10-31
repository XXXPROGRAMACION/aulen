#include "transforma.h"
#include "int_list.h"
#include <stdbool.h>

void anadirConLambda(AFND *afnd, IntList *estados);
IntList *estadosConSimbolo(AFND *afnd, IntList *estados, int s);

AFND *AFNDTransforma(AFND *afnd) {
    IntList *estados_iniciales;
    int index;

    if (afnd == NULL) return NULL;

    index = AFNDIndiceEstadoInicial(afnd);
    estados_iniciales = IntListCreate();
    if (estados_iniciales == NULL) return NULL;

    IntListAdd(estados_iniciales, index);
    anadirConLambda(afnd, estados_iniciales);
    IntListSort(estados_iniciales);
    
    

    return NULL;
}

void anadirConLambda(AFND *afnd, IntList *estados) {
    bool hay_lambda;
    int e, i, j;
    for (i = 0; i < AFNDNumEstados(afnd); i++) {
        if (IntListContains(estados, i)) continue;
        hay_lambda = false;
        for (j = 0; j < IntListSize(estados); j++) {
            e = IntListGet(estados, j);
            if (AFNDCierreLTransicionIJ(afnd, e, i)) {
                hay_lambda = true;
                break;
            }
        }
        if (hay_lambda) IntListAdd(estados, i);
    }
}

IntList *estadosConSimbolo(AFND *afnd, IntList *estados, int s) {
    IntList *nuevos_estados;
    bool hay_transicion;
    int e, i, j;

    nuevos_estados = IntListCreate();
    if (nuevos_estados == NULL) return NULL;

    for (i = 0; i < AFNDNumEstados(afnd); i++) {
        if (IntListContains(estados, i)) continue;
        hay_transicion = false;
        for (j = 0; j < IntListSize(estados); j++) {
            e = IntListGet(estados, j);
            if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, e, s, i)) {
                hay_transicion = true;
                break;
            }
        }
        if (hay_transicion) IntListAdd(nuevos_estados, i);
    }

    anadirConLambda(afnd, nuevos_estados);

    return nuevos_estados;
}
