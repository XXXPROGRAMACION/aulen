#include "transforma.h"
#include "int_list.h"
#include "state_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#define DEBUG true

void clausuraLambda(AFND *afnd, IntList *subestados);
IntList *transicionarConSimbolo(AFND *afnd, IntList *subestados, int simbolo);
char *obtenerNombreEstado(AFND *afnd, IntList *subestados);
int obtenerTipoEstado(AFND *afnd, IntList *subestados, int indice);
bool esEstadoFinal(AFND *afnd, IntList *subestados);
void debug(char *str, ...);

AFND *AFNDTransforma(AFND *afnd) {
    AFND *afd;
    IntList *subestados_iniciales, *subestados, *subestados_siguientes;
    IntList *destinos_transiciones, *simbolos_transiciones;
    StateList *estados;
    int n_estados_procesados, indice, tipo_estado, i, j;
    char *nombre_estado, *nombre_estado_origen, *nombre_estado_destino, *simbolo;

    if (afnd == NULL) return NULL;

    estados = StateListCreate();
    if (estados == NULL) return NULL;

    subestados_iniciales = IntListCreate();
    if (subestados_iniciales == NULL) {
        StateListFree(estados);
        return NULL;
    }

    indice = AFNDIndiceEstadoInicial(afnd);
    IntListAdd(subestados_iniciales, indice);
    clausuraLambda(afnd, subestados_iniciales);

    StateListAdd(estados, subestados_iniciales);
    IntListFree(subestados_iniciales);

    debug("Nuevos estados:\n");
    n_estados_procesados = 0;
    while (n_estados_procesados < StateListSize(estados)) {
        subestados = StateListGetSubstates(estados, n_estados_procesados);
        destinos_transiciones = StateListGetTransitionsTargets(estados, n_estados_procesados);
        simbolos_transiciones = StateListGetTransitionsSymbols(estados, n_estados_procesados);

        debug("Procesando %d: ", n_estados_procesados);
        if (DEBUG) IntListPrint(subestados);

        for (i = 0; i < AFNDNumSimbolos(afnd); i++) {
            subestados_siguientes = transicionarConSimbolo(afnd, subestados, i);
            if (IntListSize(subestados_siguientes) == 0) {
                IntListFree(subestados_siguientes);
                continue;
            }

            if (!StateListContainsSubstates(estados, subestados_siguientes)) {
                StateListAdd(estados, subestados_siguientes);

                debug(" -> Estado %d añadido\n", StateListSize(estados)-1);
            }

            indice = StateListGetSubstatesIndex(estados, subestados_siguientes);
            IntListAdd(destinos_transiciones, indice);
            IntListAdd(simbolos_transiciones, i);
            
            debug(" -> Transición a %d con %s añadida\n", indice, AFNDSimboloEn(afnd, i));

            IntListFree(subestados_siguientes);
        }

        n_estados_procesados++;
    }

    afd = AFNDNuevo("determinista", StateListSize(estados), AFNDNumSimbolos(afnd));
    if (afd == NULL) {
        StateListFree(estados);
        return NULL;
    }

    for (i = 0; i < AFNDNumSimbolos(afnd); i++) {
        AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));
    }

    for (i = 0; i < StateListSize(estados); i++) {
        nombre_estado = obtenerNombreEstado(afnd, StateListGetSubstates(estados, i));
        tipo_estado = obtenerTipoEstado(afnd, StateListGetSubstates(estados, i), i);
        AFNDInsertaEstado(afd, nombre_estado, tipo_estado);
        debug("Estado %d añadido con nombre %s\n", i, nombre_estado);
    }

    for (i = 0; i < StateListSize(estados); i++) {
        destinos_transiciones = StateListGetTransitionsTargets(estados, i);
        simbolos_transiciones = StateListGetTransitionsSymbols(estados, i);
        for (j = 0; j < IntListSize(StateListGetTransitionsTargets(estados, i)); j++) {
            nombre_estado_origen = AFNDNombreEstadoEn(afd, i);
            simbolo = AFNDSimboloEn(afd, IntListGet(simbolos_transiciones, j));
            nombre_estado_destino = AFNDNombreEstadoEn(afd, IntListGet(destinos_transiciones, j));
            AFNDInsertaTransicion(afd, nombre_estado_origen, simbolo, nombre_estado_destino);
        }
    }

    StateListFree(estados);

    return afd;
}

void clausuraLambda(AFND *afnd, IntList *subestados) {
    bool hay_lambda;
    int e, i, j;

    for (i = 0; i < AFNDNumEstados(afnd); i++) {
        if (IntListContains(subestados, i)) continue;
        hay_lambda = false;
        for (j = 0; j < IntListSize(subestados); j++) {
            e = IntListGet(subestados, j);
            if (AFNDCierreLTransicionIJ(afnd, e, i)) {
                hay_lambda = true;
                break;
            }
        }
        if (hay_lambda) IntListAdd(subestados, i);
    }

    IntListSort(subestados);
}

IntList *transicionarConSimbolo(AFND *afnd, IntList *subestados, int simbolo) {
    IntList *siguientes_subestados;
    bool hay_transicion;
    int estado, i, j;

    siguientes_subestados = IntListCreate();
    if (siguientes_subestados == NULL) return NULL;

    for (i = 0; i < AFNDNumEstados(afnd); i++) {
        if (IntListContains(siguientes_subestados, i)) continue;
        hay_transicion = false;
        for (j = 0; j < IntListSize(subestados); j++) {
            estado = IntListGet(subestados, j);
            if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, estado, simbolo, i)) {
                hay_transicion = true;
                break;
            }
        }
        if (hay_transicion) IntListAdd(siguientes_subestados, i);
    }

    clausuraLambda(afnd, siguientes_subestados);
    IntListSort(siguientes_subestados);
    return siguientes_subestados;
}

char *obtenerNombreEstado(AFND *afnd, IntList *subestados) {
    char *nombre_estado;
    int tam, i;

    tam = 0;
    for (i = 0; i < IntListSize(subestados); i++) {
        tam += strlen(AFNDNombreEstadoEn(afnd, IntListGet(subestados, i)));
    }

    nombre_estado = (char *) malloc(sizeof(char)*(tam+1));
    if (nombre_estado == NULL) return NULL;

    nombre_estado[0] = '\0';
    for (i = 0; i < IntListSize(subestados); i++) {
        strcat(nombre_estado, AFNDNombreEstadoEn(afnd, IntListGet(subestados, i)));
    }

    return nombre_estado;
}

int obtenerTipoEstado(AFND *afnd, IntList *subestados, int indice) {
    if (esEstadoFinal(afnd, subestados)) {
        if (indice == 0) return 2; /* Inicial y final */
        else return 1; /* Final */
    } else {
        if (indice == 0) return 0; /* Inicial */
        else return 3; /* Normal */
    }
}

bool esEstadoFinal(AFND *afnd, IntList *subestados) {
    int tipo_estado, i;

    for (i = 0; i < IntListSize(subestados); i++) {
        tipo_estado = AFNDTipoEstadoEn(afnd, IntListGet(subestados, i));
        if (tipo_estado == 1 || tipo_estado == 2) return true;
    }

    return false;
}

void debug(char *format, ...) {
    va_list args;
    if (DEBUG) {
        va_start(args, format);
        vprintf(format, args);
        va_end (args);
    }
}