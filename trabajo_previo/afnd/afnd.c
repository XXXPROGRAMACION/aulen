#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "afnd.h"

struct _Afnd {
    char *symbols;
    int n_symbols;
    char **states_names;
    bool *states_final;
    int n_states;
    int ***transitions;
    int **transitions_count;
    int start_state;
    bool *current_states;
};

bool process_symbol(Afnd *afnd, char symbol);
int get_symbol_index(Afnd *afnd, char symbol);

Afnd *create_afnd(char *symbols, int n_symbols) {
    Afnd *afnd;

    if (symbols == NULL) return NULL;

    afnd = (Afnd *) malloc(sizeof(Afnd));
    if (afnd == NULL) return NULL;

    afnd->symbols = (char *) malloc(sizeof(char)*n_symbols);
    if (afnd->symbols == NULL) {
        free(afnd);
        return NULL;
    }

    memcpy(afnd->symbols, symbol, sizeof(char)*n_symbols)

    afnd->n_symbols = n_symbols;
    afnd->n_states = 0;
    afnd->start_state = -1;

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