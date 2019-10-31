#ifndef AFND_H
#define AFND_H

#include <stdbool.h>

typedef struct _Afnd Afnd;

Afnd *create_afnd(bool *final_states, char *symbols, int ***transitions, int **transitions_count, int n_states, int n_symbols, int start_state);
bool process_entry(Afnd *afnd, char *entry, int entry_length);

#endif