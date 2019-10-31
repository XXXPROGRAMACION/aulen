#include <stdio.h>
#include <stdlib.h>
#include "afd.h"

int main() {
    bool *final_states;
    char *symbols = "ab";
    int **transitions;
    int n_states = 2;
    int n_symbols = 2;
    int start_state = 0;
    Afd *afd;

    final_states = (bool *) malloc(sizeof(bool)*2);
    final_states[0] = 1;
    final_states[1] = 0;

    transitions = (int **) malloc(sizeof(int *)*2);
    transitions[0] = (int *) malloc(sizeof(int)*2);
    transitions[1] = (int *) malloc(sizeof(int)*2);
    transitions[0][0] = 1;
    transitions[0][1] = -1;
    transitions[1][0] = -1;
    transitions[1][1] = 0;

    afd = create_afd(final_states, symbols, transitions, n_states, n_symbols, start_state);
    printf("%d\n", processEntry(afd, "abab", 4));

    return 0;
}