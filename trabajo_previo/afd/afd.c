#include <stdio.h>
#include <stdlib.h>
#include "afd.h"

struct _Afd {
    bool *final_states;
    char *symbols;
    int **transitions;
    int n_states;
    int n_symbols;
    int current_state;
    int start_state;
};

bool processSymbol(Afd *afd, char symbol);
int getSymbolIndex(Afd *afd, char symbol);

Afd *create_afd(bool *final_states, char *symbols, int **transitions, int n_states, int n_symbols, int start_state) {
    Afd *afd;
    int i, j;

    if (final_states == NULL || symbols == NULL || transitions == NULL) return NULL;
    if (n_states <= 0 || n_symbols <= 0) return NULL;
    if (start_state < 0 || start_state >= n_states) return NULL;
    for (i = 0; i < n_states; i++) if (transitions[i] == NULL) return NULL;

    afd = (Afd *) malloc(sizeof(Afd));
    if (afd == NULL) return NULL;

    afd->final_states = (bool *) malloc(sizeof(bool)*n_states);
    if (afd->final_states == NULL) {
        free(afd);
        return NULL;
    }

    afd->symbols = (char *) malloc(sizeof(char)*n_symbols);
    if (afd->symbols == NULL) {
        free(afd->final_states);
        free(afd);
        return NULL;
    }

    afd->transitions = (int **) malloc(sizeof(int *)*n_states);
    if (afd->transitions == NULL) {
        free(afd->symbols);
        free(afd->final_states);
        free(afd);
        return NULL;
    }

    for (i = 0; i < n_symbols; i++) {
        afd->transitions[i] = (int *) malloc(sizeof(int *)*n_symbols);
        if (afd->transitions[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(afd->transitions[j]);
            }            
            free(afd->transitions);
            free(afd->symbols);
            free(afd->final_states);
            free(afd);
            return NULL;
        }
    }

    for (i = 0; i < n_states; i++) {
        afd->final_states[i] = final_states[i];
    }

    for (i = 0; i < n_symbols; i++) {
        afd->symbols[i] = symbols[i];
    }

    for (i = 0; i < n_states; i++) {
        for (j = 0; j < n_symbols; j++) {
            afd->transitions[i][j] = transitions[i][j];
        }
    }

    afd->n_states = n_states;
    afd->n_symbols = n_symbols;
    afd->start_state = start_state;

    return afd;
}

bool processEntry(Afd *afd, char *entry, int entry_length) {
    int i;

    afd->current_state = afd->start_state;

    for (i = 0; i < entry_length; i++) {
        if (!processSymbol(afd, entry[i])) return false;
    }

    return afd->final_states[afd->current_state];
}

bool processSymbol(Afd *afd, char symbol) {
    int index, newState;

    printf("Llega\n");
    
    index = getSymbolIndex(afd, symbol);
    if (index == -1) return false;

    newState = afd->transitions[afd->current_state][index];
    if (newState == -1) return false;

    return true;
}

int getSymbolIndex(Afd *afd, char symbol) {
    int i;

    for (i = 0; i < afd->n_symbols; i++) {
        if (afd->symbols[i] == symbol) return i;
    }

    return -1;
}