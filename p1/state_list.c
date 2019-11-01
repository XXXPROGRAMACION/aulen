#include "state_list.h"
#include <stdlib.h>

struct _StateList {
    IntList **substates;
    IntList **transitions_targets;
    IntList **transitions_symbols;
    int size;
    int max_size;
};

void StateListResize(StateList *state_list);

StateList *StateListCreate() {
    StateList *state_list;

    state_list = (StateList *) malloc(sizeof(StateList));
    if (state_list == NULL) return NULL;

    state_list->size = 0;
    state_list->max_size = 1;

    state_list->substates = (IntList **) malloc(sizeof(StateList *)*state_list->max_size);
    if (state_list->substates == NULL) {
        free(state_list);
        return NULL;
    }

    state_list->transitions_targets = (IntList **) malloc(sizeof(StateList *)*state_list->max_size);
    if (state_list->transitions_targets == NULL) {
        free(state_list->substates);
        free(state_list);
        return NULL;
    }

    state_list->transitions_symbols = (IntList **) malloc(sizeof(StateList *)*state_list->max_size);
    if (state_list->transitions_symbols == NULL) {
        free(state_list->transitions_targets);
        free(state_list->substates);
        free(state_list);
        return NULL;
    }

    return state_list;
}

void StateListFree(StateList *state_list) {
    int i;
    if (state_list == NULL) return;
    for (i = 0; i < state_list->size; i++) {
        IntListFree(state_list->substates[i]);
        IntListFree(state_list->transitions_targets[i]);
        IntListFree(state_list->transitions_symbols[i]);
    }
    free(state_list->transitions_symbols);
    free(state_list->transitions_targets);
    free(state_list->substates);
    free(state_list);
}

void StateListAdd(StateList *state_list, IntList *substates) {
    IntList *substates_copy, *transitions_targets, *transitions_symbols;
    if (state_list == NULL) return;

    substates_copy = IntListCopy(substates);
    if (substates_copy == NULL) return;
    transitions_targets = IntListCreate();
    if (transitions_targets == NULL) {
        IntListFree(substates_copy);
        return;
    }
    transitions_symbols = IntListCreate();
    if (transitions_symbols == NULL) {
        IntListFree(transitions_targets);
        IntListFree(substates_copy);
        return;
    }

    if (state_list->size >= state_list->max_size) StateListResize(state_list);
    state_list->substates[state_list->size] = substates_copy;
    state_list->transitions_targets[state_list->size] = transitions_targets;
    state_list->transitions_symbols[state_list->size] = transitions_symbols;
    state_list->size++;
}

IntList *StateListGetSubstates(StateList *state_list, int index) {
    if (state_list == NULL) return NULL;
    if (index < 0 || index >= state_list->size) return NULL;
    return state_list->substates[index];
}

IntList *StateListGetTransitionsTargets(StateList *state_list, int index) {
    if (state_list == NULL) return NULL;
    if (index < 0 || index >= state_list->size) return NULL;
    return state_list->transitions_targets[index];
}

IntList *StateListGetTransitionsSymbols(StateList *state_list, int index) {
    if (state_list == NULL) return NULL;
    if (index < 0 || index >= state_list->size) return NULL;
    return state_list->transitions_symbols[index];
}

int StateListSize(StateList *state_list) {
    if (state_list == NULL) return -1;
    return state_list->size;
}

bool StateListContainsSubstates(StateList *state_list, IntList *substates) {
    int i;
    if (state_list == NULL || substates == NULL) return false;
    for (i = 0; i < state_list->size; i++) {
        if (IntListCompare(state_list->substates[i], substates)) return true;
    }
    return false;
}

int StateListGetSubstatesIndex(StateList *state_list, IntList *substates) {
    int i;
    if (state_list == NULL || substates == NULL) return -1;
    for (i = 0; i < state_list->size; i++) {
        if (IntListCompare(state_list->substates[i], substates)) return i;
    }
    return -1;
}

void StateListPrint(StateList *state_list);

void StateListResize(StateList *state_list) {
    if (state_list == NULL) return;
    state_list->max_size *= 2;
    state_list->substates = (IntList **) realloc(state_list->substates, sizeof(StateList *)*state_list->max_size);
    state_list->transitions_targets = (IntList **) realloc(state_list->transitions_targets, sizeof(StateList *)*state_list->max_size);
    state_list->transitions_symbols = (IntList **) realloc(state_list->transitions_symbols, sizeof(StateList *)*state_list->max_size);
}