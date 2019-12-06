#include "minimiza.h"
#include "int_list.h"
#include "state_list.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

IntList *obtenerEstadosAccesibles(AFND *afd, IntList *estados_filtrados, bool debug);
IntList *obtenerEstadosUtiles(AFND *afd, IntList *estados_filtrados, bool debug);
StateList *obtenerEstadosReducidos(AFND *afd, IntList *estados_filtrados, bool debug);
bool comprobarMismaClase(AFND *afd, StateList *cociente_actual, int estado_actual, int estado_cabeza);
int obtenerTransicionConSimbolo(AFND *afd, int estado, int s);

AFND *AFNDMinimiza(AFND *afd, bool debug) {
    IntList *estados_originales;
    IntList *estados_accesibles, *estados_accesibles_utiles;
    StateList *estados_reducidos;
    IntList *estado_reducido_actual;
    int estado_actual, i, j;

    if (afd == NULL) return NULL;

    estados_originales = IntListCreate();
    if (estados_originales == NULL) return NULL;

    for (i = 0; i < AFNDNumEstados(afd); i++) {
        IntListAdd(estados_originales, i);
    }

    if (debug) {
        printf("Lista de estados originales:\n");
        for (i = 0; i < IntListSize(estados_originales); i++) {
            printf(" -> %s\n", AFNDNombreEstadoEn(afd, IntListGet(estados_originales, i)));
        }
    }

    estados_accesibles = obtenerEstadosAccesibles(afd, estados_originales, debug);
    if (estados_accesibles == NULL) {
        IntListFree(estados_originales);
        return NULL;
    }

    if (debug) {
        printf("Filtrados inaccesibles:\n");
        for (i = 0; i < IntListSize(estados_originales); i++) {
            estado_actual = IntListGet(estados_originales, i);
            printf(" -> %s", AFNDNombreEstadoEn(afd, estado_actual));
            if (IntListContains(estados_accesibles, estado_actual)) {
                printf("\n");
            } else {
                printf(" - Inaccesible\n");
            }
        }
    }

    estados_accesibles_utiles = obtenerEstadosUtiles(afd, estados_accesibles, debug);
    if (estados_accesibles == NULL) {
        IntListFree(estados_accesibles);
        IntListFree(estados_originales);
        return NULL;
    }

    if (debug) {
        printf("Filtrados inútiles:\n");
        for (i = 0; i < IntListSize(estados_accesibles); i++) {
            estado_actual = IntListGet(estados_accesibles, i);
            printf(" -> %s", AFNDNombreEstadoEn(afd, estado_actual));
            if (IntListContains(estados_accesibles_utiles, estado_actual)) {
                printf("\n");
            } else {
                printf(" - Inútil\n");
            }
        }
    }

    estados_reducidos = obtenerEstadosReducidos(afd, estados_accesibles_utiles, debug);
    if (estados_reducidos == NULL) {
        IntListFree(estados_accesibles);
        return NULL;
    }

    if (debug) {
        printf("Cociente final / Estados equivalentes:\n");
        for (i = 0; i < StateListSize(estados_reducidos); i++) {
            estado_reducido_actual = StateListGetSubstates(estados_reducidos, i);
            printf(" -> {");
            for (j = 0; j < IntListSize(estado_reducido_actual); j++) {
                estado_actual = IntListGet(estado_reducido_actual, j);
                printf("%s", AFNDNombreEstadoEn(afd, estado_actual));
                if (j < IntListSize(estado_reducido_actual)-1) {
                    printf(", ");
                }
            }
            printf("}\n");
        }
    }

    StateListFree(estados_reducidos);
    IntListFree(estados_accesibles_utiles);
    IntListFree(estados_accesibles);
    IntListFree(estados_originales);

    return NULL;
}

IntList *obtenerEstadosAccesibles(AFND *afd, IntList *estados_filtrados, bool debug) {
    IntList *estados_accesibles;
    int estado_actual, estado_siguiente, i, j, s;

    estados_accesibles = IntListCreate();
    if (estados_accesibles == NULL) return NULL;

    if (IntListContains(estados_filtrados, AFNDIndiceEstadoInicial(afd))) {
        IntListAdd(estados_accesibles, AFNDIndiceEstadoInicial(afd));
    }

    for (i = 0; i < IntListSize(estados_accesibles); i++) {
        estado_actual = IntListGet(estados_accesibles, i);
        for (j = 0; j < IntListSize(estados_filtrados); j++) {
            estado_siguiente = IntListGet(estados_filtrados, j);
            if (IntListContains(estados_accesibles, estado_siguiente)) continue;
            for (s = 0; s < AFNDNumSimbolos(afd); s++) {
                if (AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado_actual, s, estado_siguiente)) {
                    IntListAdd(estados_accesibles, estado_siguiente);
                    break;
                }
            }
        }
    }

    IntListSort(estados_accesibles);

    return estados_accesibles;
}

IntList *obtenerEstadosUtiles(AFND *afd, IntList *estados_filtrados, bool debug) {
    IntList *estados_utiles;
    int estado_actual, estado_anterior, tipo_estado, i, j, s;

    estados_utiles = IntListCreate();
    if (estados_utiles == NULL) return NULL;

    for (i = 0; i < IntListSize(estados_filtrados); i++) {
        estado_actual = IntListGet(estados_filtrados, i);
        tipo_estado = AFNDTipoEstadoEn(afd, estado_actual);
        if (tipo_estado == FINAL || tipo_estado == INICIAL_Y_FINAL) {
            IntListAdd(estados_utiles, estado_actual);
        }
    }

    for (i = 0; i < IntListSize(estados_utiles); i++) {
        estado_actual = IntListGet(estados_utiles, i);
        for (j = 0; j < IntListSize(estados_filtrados); j++) {
            estado_anterior = IntListGet(estados_filtrados, j);
            if (IntListContains(estados_utiles, estado_anterior)) continue;
            for (s = 0; s < AFNDNumSimbolos(afd); s++) {
                if (AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado_anterior, s, estado_actual)) {
                    IntListAdd(estados_utiles, estado_anterior);
                    break;
                }
            }
        }
    }

    IntListSort(estados_utiles);

    return estados_utiles;
}

StateList *obtenerEstadosReducidos(AFND *afd, IntList *estados_filtrados, bool debug) {
    StateList *cociente_actual, *cociente_siguiente;
    IntList *clase_actual, *clase_nueva;
    IntList *clase_no_finales, *clase_finales;
    int estado_actual, estado_cabeza, tipo_estado, indice_cociente_actual;
    int n_cocientes, i, j, k;
    bool clase_existente, ha_cambiado;

    cociente_actual = StateListCreate();
    if (cociente_actual == NULL) return NULL;

    clase_no_finales = IntListCreate();
    if (clase_no_finales == NULL) {
        StateListFree(cociente_actual);
        return NULL;
    }

    clase_finales = IntListCreate();
    if (clase_finales == NULL) {
        IntListFree(clase_no_finales);
        StateListFree(cociente_actual);
        return NULL;
    }

    for (i = 0; i < IntListSize(estados_filtrados); i++) {
        estado_actual = IntListGet(estados_filtrados, i);
        tipo_estado = AFNDTipoEstadoEn(afd, estado_actual);
        if (tipo_estado == FINAL || tipo_estado == INICIAL_Y_FINAL) {
            IntListAdd(clase_finales, estado_actual);
        } else {
            IntListAdd(clase_no_finales, estado_actual);
        }
    }

    StateListAdd(cociente_actual, clase_no_finales);
    StateListAdd(cociente_actual, clase_finales);

    IntListFree(clase_finales);
    IntListFree(clase_no_finales);
    
    n_cocientes = 0;
    do {        
        if (debug) {
            printf("Cociente %d:\n", n_cocientes);
            for (i = 0; i < StateListSize(cociente_actual); i++) {
                clase_actual = StateListGetSubstates(cociente_actual, i);
                printf(" -> {");
                for (j = 0; j < IntListSize(clase_actual); j++) {
                    estado_actual = IntListGet(clase_actual, j);
                    printf("%s", AFNDNombreEstadoEn(afd, estado_actual));
                    if (j < IntListSize(clase_actual)-1) {
                        printf(", ");
                    }
                }
                printf("}\n");
            }
        }

        n_cocientes++;
        cociente_siguiente = StateListCreate();
        if (cociente_siguiente == NULL) {
            StateListFree(cociente_actual);
            return NULL;
        }

        for (i = 0; i < StateListSize(cociente_actual); i++) {
            clase_actual = StateListGetSubstates(cociente_actual, i);
            indice_cociente_actual = StateListSize(cociente_siguiente);
            for (j = 0; j < IntListSize(clase_actual); j++) {
                estado_actual = IntListGet(clase_actual, j);
                clase_existente = false;
                for (k = indice_cociente_actual; k < StateListSize(cociente_siguiente); k++) {
                    clase_nueva = StateListGetSubstates(cociente_siguiente, k);
                    estado_cabeza = IntListGet(clase_nueva, 0);
                    if (comprobarMismaClase(afd, cociente_actual, estado_actual, estado_cabeza)) {
                        IntListAdd(clase_nueva, estado_actual);
                        clase_existente = true;
                        break;
                    }
                }
                if (!clase_existente) {
                    clase_nueva = IntListCreate();
                    if (clase_nueva == NULL) {
                        StateListFree(cociente_siguiente);
                        StateListFree(cociente_actual);
                        return NULL;
                    }
                    IntListAdd(clase_nueva, estado_actual);
                    StateListAdd(cociente_siguiente, clase_nueva);
                }
            }
        }

        ha_cambiado = StateListSize(cociente_actual) != StateListSize(cociente_siguiente);
        StateListFree(cociente_actual);
        cociente_actual = cociente_siguiente;
    } while (ha_cambiado);

    return cociente_actual;
}

bool comprobarMismaClase(AFND *afd, StateList *cociente, int estado_actual, int estado_cabeza) {
    IntList *clase_actual;
    int transicion_estado_actual, transicion_estado_cabeza;
    bool contiene_actual, contiene_cabeza;
    int s, i;

    for (s = 0; s < AFNDNumSimbolos(afd); s++) {
        transicion_estado_actual = obtenerTransicionConSimbolo(afd, estado_actual, s);
        transicion_estado_cabeza = obtenerTransicionConSimbolo(afd, estado_cabeza, s);

        if (transicion_estado_actual == -1 && transicion_estado_cabeza == -1) continue;
        if (transicion_estado_actual == -1) return false;
        if (transicion_estado_cabeza == -1) return false;

        for (i = 0; i < StateListSize(cociente); i++) {
            clase_actual = StateListGetSubstates(cociente, i);
            contiene_actual = IntListContains(clase_actual, transicion_estado_actual);
            contiene_cabeza = IntListContains(clase_actual, transicion_estado_cabeza);
            if (contiene_actual && contiene_cabeza) break;
            if (contiene_actual) return false;
            if (contiene_cabeza) return false;
        }
    }

    return true;
}

int obtenerTransicionConSimbolo(AFND *afd, int estado, int simbolo) {
    int i;
    for (i = 0; i < AFNDNumEstados(afd); i++) {
        if (AFNDTransicionIndicesEstadoiSimboloEstadof(afd, estado, simbolo, i)) return i;
    }
    return -1;
}