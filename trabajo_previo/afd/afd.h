#ifndef AFD_H
#define AFD_H

#include <stdbool.h>

typedef struct _Afd Afd;

Afd *create_afd(bool *final_states, char *symbols, int **transitions, int n_states, int n_symbols, int start_state);
bool processEntry(Afd *afd, char *entry, int entry_length);

#endif