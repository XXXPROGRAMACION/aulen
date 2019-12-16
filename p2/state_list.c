#include "state_list.h"
#include <stdlib.h>

struct _StateList {
    IntList **substates;
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

    return state_list;
}

void StateListFree(StateList *state_list) {
    int i;
    if (state_list == NULL) return;
    for (i = 0; i < state_list->size; i++) {
        IntListFree(state_list->substates[i]);
    }
    free(state_list->substates);
    free(state_list);
}

void StateListAdd(StateList *state_list, IntList *substates) {
    IntList *substates_copy;
    if (state_list == NULL) return;

    substates_copy = IntListCopy(substates);
    if (substates_copy == NULL) return;

    if (state_list->size >= state_list->max_size) StateListResize(state_list);
    state_list->substates[state_list->size] = substates_copy;
    state_list->size++;
}

IntList *StateListGetSubstates(StateList *state_list, int index) {
    if (state_list == NULL) return NULL;
    if (index < 0 || index >= state_list->size) return NULL;
    return state_list->substates[index];
}

int StateListSize(StateList *state_list) {
    if (state_list == NULL) return -1;
    return state_list->size;
}

void StateListPrint(StateList *state_list);

void StateListResize(StateList *state_list) {
    if (state_list == NULL) return;
    state_list->max_size *= 2;
    state_list->substates = (IntList **) realloc(state_list->substates, sizeof(StateList *)*state_list->max_size);
}