#include "transforma.h"
#include "int_list.h"
#include "int_queue.h"
#include "state_list.h"
#include <stdbool.h>
#include <stdarg.h>

#define DEBUG true

void clausuraLambda(AFND *afnd, IntList *estados);
IntList *transicionarConSimbolo(AFND *afnd, IntList *estados, int symbol);
bool existeTransicion(IntList *destinos_transiciones, IntList *simbolos_transiciones, int target, int symbol);
void debug(char *str, ...);

AFND *AFNDTransforma(AFND *afnd) {
    IntList *subestados_iniciales, *subestados, *subestados_siguientes;
    IntList *destinos_transiciones, *simbolos_transiciones;
    IntQueue *cola_estados;
    StateList *estados;
    int index, i;

    if (afnd == NULL) return NULL;

    estados = StateListCreate();
    if (estados == NULL) return NULL;

    cola_estados = IntQueueCreate();
    if (cola_estados == NULL) {
        StateListFree(estados);
        return NULL;
    }

    subestados_iniciales = IntListCreate();
    if (subestados_iniciales == NULL) {
        IntQueueFree(cola_estados);
        StateListFree(estados);
        return NULL;
    }

    index = AFNDIndiceEstadoInicial(afnd);
    IntListAdd(subestados_iniciales, index);
    clausuraLambda(afnd, subestados_iniciales);

    StateListAdd(estados, subestados_iniciales);
    IntQueueAdd(cola_estados, 0);
    IntListFree(subestados_iniciales);

    debug("Nuevos estados:\n");
    while (!IntQueueIsEmpty(cola_estados)) {
        index = IntQueueRemove(cola_estados);
        subestados = StateListGetSubstates(estados, index);
        destinos_transiciones = StateListGetTransitionsTargets(estados, index);
        simbolos_transiciones = StateListGetTransitionsSymbols(estados, index);

        debug("Procesando %d: ", index);
        if (DEBUG) IntListPrint(subestados);

        for (i = 0; i < AFNDNumSimbolos(afnd); i++) {
            subestados_siguientes = transicionarConSimbolo(afnd, subestados, i);
            if (IntListSize(subestados_siguientes) == 0) continue;

            if (!StateListContainsSubstates(estados, subestados_siguientes)) {
                StateListAdd(estados, subestados_siguientes);
                IntQueueAdd(cola_estados, StateListSize(estados)-1);

                debug(" -> Estado %d añadido\n", StateListSize(estados)-1);
            }

            index = StateListGetSubstatesIndex(estados, subestados_siguientes);
            if (!existeTransicion(destinos_transiciones, simbolos_transiciones, index, i)) {
                IntListAdd(destinos_transiciones, index);
                IntListAdd(simbolos_transiciones, i);
                
                debug(" -> Transición a %d con %s añadida\n", index, AFNDSimboloEn(afnd, i));
            }
            IntListFree(subestados_siguientes);
        }
    }

    IntQueueFree(cola_estados);
    StateListFree(estados);

    return NULL;
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

IntList *transicionarConSimbolo(AFND *afnd, IntList *subestados, int symbol) {
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
            if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, estado, symbol, i)) {
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

bool existeTransicion(IntList *destinos_transiciones, IntList *simbolos_transiciones, int target, int symbol) {
    int i;
    if (destinos_transiciones == NULL || simbolos_transiciones == NULL) return true;
    for (i = 0; i < IntListSize(destinos_transiciones); i++) {
        if (IntListGet(destinos_transiciones, i) == target && IntListGet(simbolos_transiciones, i) == symbol) return true;
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