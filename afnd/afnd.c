#include <stdio.h>
#include <stdlib.h>
#include "afnd.h"

struct _Afnd {
    bool *final_states;
    char *symbols;
    int ***transitions;
    int **transitions_count;
    int n_states;
    int n_symbols;
    int start_state;
    bool *current_states;
};

bool process_symbol(Afnd *afnd, char symbol);
int get_symbol_index(Afnd *afnd, char symbol);

Afnd *create_afnd(bool *final_states, char *symbols, int ***transitions, int **transitions_count, int n_states, int n_symbols, int start_state) {
    Afnd *afnd;
    int i, j, k;

    if (final_states == NULL || symbols == NULL || transitions == NULL) return NULL;
    if (n_states <= 0 || n_symbols <= 0) return NULL;
    if (start_state < 0 || start_state >= n_states) return NULL;
    for (i = 0; i < n_states; i++) if (transitions[i] == NULL) return NULL;

    afnd = (Afnd *) malloc(sizeof(Afnd));
    if (afnd == NULL) return NULL;

    afnd->final_states = (bool *) malloc(sizeof(bool)*n_states);
    if (afnd->final_states == NULL) {
        free(afnd);
        return NULL;
    }

    afnd->symbols = (char *) malloc(sizeof(char)*n_symbols);
    if (afnd->symbols == NULL) {
        free(afnd->final_states);
        free(afnd);
        return NULL;
    }

    afnd->current_states = (bool *) malloc(sizeof(bool)*n_states);
    if (afnd->current_states == NULL) {
        free(afnd->symbols);
        free(afnd->final_states);
        free(afnd);
        return NULL;
    }


    afnd->transitions_count = (int **) malloc(sizeof(int *)*n_states);
    if (afnd->transitions_count == NULL) {
        free(afnd->current_states);
        free(afnd->symbols);
        free(afnd->final_states);
        free(afnd);
        return NULL;
    }

    for (i = 0; i < n_states; i++) {
        afnd->transitions_count[i] = (int *) malloc(sizeof(int)*n_symbols);
        if (afnd->transitions_count[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(afnd->transitions_count[j]);
            }            
            free(afnd->transitions_count);
            free(afnd->current_states);
            free(afnd->symbols);
            free(afnd->final_states);
            free(afnd);
            return NULL;
        }
    }

    afnd->transitions = (int ***) malloc(sizeof(int **)*n_states);
    if (afnd->transitions == NULL) {
        for (i = 0; i < n_states; i++) free(afnd->transitions_count[i]);
        free(afnd->transitions_count);
        free(afnd->current_states);
        free(afnd->symbols);
        free(afnd->final_states);
        free(afnd);
        return NULL;
    }

    for (i = 0; i < n_states; i++) {
        afnd->transitions[i] = (int **) malloc(sizeof(int *)*n_symbols);
        if (afnd->transitions[i] == NULL) {
            for (int j = 0; j < i; j++) free(afnd->transitions[j]);
            free(afnd->transitions);
            for (j = 0; j < n_states; j++) free(afnd->transitions_count[j]);
            free(afnd->transitions_count);
            free(afnd->current_states);
            free(afnd->symbols);
            free(afnd->final_states);
            free(afnd);
            return NULL;
        }

        for (j = 0; j < n_symbols; j++) {
            afnd->transitions[i][j] = (int *) malloc(sizeof(int)*transitions_count[i][j]);
            // Comprobacion de errores
        }
    }

    for (i = 0; i < n_states; i++) {
        afnd->final_states[i] = final_states[i];
    }

    for (i = 0; i < n_symbols; i++) {
        afnd->symbols[i] = symbols[i];
    }

    for (i = 0; i < n_states; i++) {
        for (j = 0; j < n_symbols; j++) {
            afnd->transitions_count[i][j] = transitions_count[i][j];
        }
    }

    for (i = 0; i < n_states; i++) {
        for (j = 0; j < n_symbols; j++) {
            for (k = 0; k < transitions_count[i][j]; k++) {
                afnd->transitions[i][j][k] = transitions[i][j][k];
            }
        }
    }

    afnd->n_states = n_states;
    afnd->n_symbols = n_symbols;
    afnd->start_state = start_state;

    return afnd;
}

bool process_entry(Afnd *afnd, char *entry, int entry_length) {
    int i;

    for (i = 0; i < afnd->n_states; i++) {
        afnd->current_states[i] = false;
    }

    afnd->current_states[afnd->start_state] = true;

    for (i = 0; i < entry_length; i++) {
        if (!process_symbol(afnd, entry[i])) return false;
    }

    for (i = 0; i < afnd->n_states; i++) {
        if (afnd->current_states[i] && afnd->final_states[i]) {
            return true;
        }
    }

    return false;
}

bool process_symbol(Afnd *afnd, char symbol) {
    int symbol_index, new_current_state_index, i, j;
    int *new_current_states;
    bool valid;
    
    symbol_index = get_symbol_index(afnd, symbol);
    if (symbol_index == -1) return false;

    new_current_states = (int *) malloc(sizeof(int)*afnd->n_states);
    if (new_current_states == NULL) return false;
    for (i = 0; i < afnd->n_states; i++) new_current_states[i] = false;

    for (i = 0; i < afnd->n_states; i++) {
        if (afnd->current_states[i] && afnd->transitions_count[i][symbol_index] > 0) {
            for (j = 0; j < afnd->transitions_count[i][symbol_index]; j++) {
                new_current_state_index = afnd->transitions[i][symbol_index][j];
                new_current_states[new_current_state_index] = true;
            }
            valid = true;
        }
    }

    if (!valid) return false;

    for (i = 0; i < afnd->n_states; i++) {
        afnd->current_states[i] = new_current_states[i];
    }

    free(new_current_states);
    return true;
}

int get_symbol_index(Afnd *afnd, char symbol) {
    int i;

    for (i = 0; i < afnd->n_symbols; i++) {
        if (afnd->symbols[i] == symbol) return i;
    }

    return -1;
}