#include <stdio.h>
#include <stdlib.h>
#include "afnd.h"

void ab();
void acaba_aa();
char *mensaje(bool b);

int main() {
    ab();
    acaba_aa();

    return 0;
}

void ab() {
    bool *final_states;
    char *symbols = "ab";
    int ***transitions;
    int **transitions_count;
    int n_states = 2;
    int n_symbols = 2;
    int start_state = 0;
    Afnd *afnd;
    int i, j;

    final_states = (bool *) malloc(sizeof(bool)*n_states);
    final_states[0] = true;
    final_states[1] = false;

    transitions_count = (int **) malloc(sizeof(int *)*n_states);
    for (i = 0; i < n_states; i++) {
        transitions_count[i] = (int *) malloc(sizeof(int)*n_symbols);
    }

    transitions_count[0][0] = 1;
    transitions_count[0][1] = 0;
    transitions_count[1][0] = 0;
    transitions_count[1][1] = 1;

    transitions = (int ***) malloc(sizeof(int **)*n_states);
    for (i = 0; i < n_states; i++) {
        transitions[i] = (int **) malloc(sizeof(int *)*n_symbols);
        for (j = 0; j < n_symbols; j++) {
            transitions[i][j] = (int *) malloc(sizeof(int)*transitions_count[i][j]);
        }
    }

    transitions[0][0][0] = 1;
    transitions[1][1][0] = 0;

    printf("Regex = (ab)*\n");
    afnd = create_afnd(final_states, symbols, transitions, transitions_count, n_states, n_symbols, start_state);
    printf(" -> vacia (accepted) = %s\n", mensaje(process_entry(afnd, "", 0)));
    printf(" -> a (rejected) = %s\n", mensaje(process_entry(afnd, "a", 1)));
    printf(" -> ab (accepted) = %s\n", mensaje(process_entry(afnd, "ab", 2)));
    printf(" -> aba (rejected) = %s\n", mensaje(process_entry(afnd, "aba", 3)));
    printf(" -> abab (accepted) = %s\n", mensaje(process_entry(afnd, "abab", 4)));
    printf(" -> abac (rejected) = %s\n", mensaje(process_entry(afnd, "abac", 4)));
}

void acaba_aa() {
    bool *final_states;
    char *symbols = "ab";
    int ***transitions;
    int **transitions_count;
    int n_states = 3;
    int n_symbols = 2;
    int start_state = 0;
    Afnd *afnd;
    int i, j;

    final_states = (bool *) malloc(sizeof(bool)*n_states);
    final_states[0] = false;
    final_states[1] = false;
    final_states[2] = true;

    transitions_count = (int **) malloc(sizeof(int *)*n_states);
    for (i = 0; i < n_states; i++) {
        transitions_count[i] = (int *) malloc(sizeof(int)*n_symbols);
    }

    transitions_count[0][0] = 2;
    transitions_count[0][1] = 1;
    transitions_count[1][0] = 1;
    transitions_count[1][1] = 0;
    transitions_count[2][0] = 0;
    transitions_count[2][1] = 0;

    transitions = (int ***) malloc(sizeof(int **)*n_states);
    for (i = 0; i < n_states; i++) {
        transitions[i] = (int **) malloc(sizeof(int *)*n_symbols);
        for (j = 0; j < n_symbols; j++) {
            transitions[i][j] = (int *) malloc(sizeof(int)*transitions_count[i][j]);
        }
    }

    transitions[0][0][0] = 0;
    transitions[0][0][1] = 1;
    transitions[0][1][0] = 0;
    transitions[1][0][0] = 2;

    printf("Regex = (a+b)*aa\n");
    afnd = create_afnd(final_states, symbols, transitions, transitions_count, n_states, n_symbols, start_state);
    printf(" -> vacia (rejected) = %s\n", mensaje(process_entry(afnd, "", 0)));
    printf(" -> aa (accepted) = %s\n", mensaje(process_entry(afnd, "aa", 2)));
    printf(" -> abaa (accepted) = %s\n", mensaje(process_entry(afnd, "abaa", 4)));
    printf(" -> abba (rejected) = %s\n", mensaje(process_entry(afnd, "abba", 4)));
    printf(" -> abab (rejected) = %s\n", mensaje(process_entry(afnd, "abab", 4)));
    printf(" -> bbbb (rejected) = %s\n", mensaje(process_entry(afnd, "bbbb", 4)));
    printf(" -> bbbbaa (accepted) = %s\n", mensaje(process_entry(afnd, "bbbbaa", 6)));
    printf(" -> ccaa (rejected) = %s\n", mensaje(process_entry(afnd, "ccaa", 4)));
}

char *mensaje(bool b) {
    return (b ? "accepted" : "rejected");
}