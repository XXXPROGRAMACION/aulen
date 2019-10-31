#include "state_list.h"
#include <stdlib.h>

struct _StateList {
    IntList **sub_states;
    IntList **conections;
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

    state_list->sub_states = (StateList **) malloc(sizeof(StateList *)*state_list->max_size);
    if (state_list->sub_states == NULL) {
        free(state_list);
        return NULL;
    }

    state_list->conections = (StateList **) malloc(sizeof(StateList *)*state_list->max_size);
    if (state_list->conections == NULL) {
        free(state_list->sub_states);
        free(state_list);
        return NULL;
    }

    return state_list;
}

void StateListFree(StateList *state_list) {
    int i;
    if (state_list == NULL) return;
    for (i = 0; i < state_list->size; i++) {
        IntListFree(state_list->sub_states[i]);
        IntListFree(state_list->conections[i]);
    }
    free(state_list->conections);
    free(state_list->sub_states);
    free(state_list);
}

void StateListAdd(StateList *state_list, IntList *sub_states, IntList *conections) {
    if (state_list == NULL) return;
    if (state_list->size >= state_list->max_size) StateListResize(state_list);
    state_list->sub_states[state_list->size] = sub_states;
    state_list->conections[state_list->size] = conections;
    state_list->size++;
}

StateList *StateListGetSubStates(StateList *state_list, int index) {
    if (state_list == NULL) return NULL;
    if (index < 0 || index >= state_list->size) return NULL;
    return state_list->sub_states[index];
}

StateList *StateListGetConections(StateList *state_list, int index) {
    if (state_list == NULL) return NULL;
    if (index < 0 || index >= state_list->size) return NULL;
    return state_list->conections[index];
}

int StateListSize(StateList *state_list) {
    if (state_list == NULL) return -1;
    return state_list->size;
}

bool StateListContainsSubstates(StateList *state_list, IntList *sub_states) {
    int i;
    if (state_list == NULL || sub_states == NULL) return false;
    for (i = 0; i < state_list->size; i++) {
        if (IntListCompare(state_list->conections[i], sub_states)) return true;
    }
    return false;
}

void StateListPrint(StateList *state_list);

void StateListResize(StateList *state_list) {
    if (state_list == NULL) return;
    state_list->max_size *= 2;
    state_list->sub_states = (IntList **) realloc(state_list->sub_states, sizeof(StateList *)*state_list->max_size);
    state_list->conections = (IntList **) realloc(state_list->conections, sizeof(StateList *)*state_list->max_size);
}