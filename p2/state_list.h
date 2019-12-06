#ifndef STATE_LIST_H
#define STATE_LIST_H

#include "int_list.h"
#include <stdbool.h>

typedef struct _StateList StateList;

StateList *StateListCreate();
void StateListFree(StateList *state_list);
void StateListAdd(StateList *state_list, IntList *substates);
IntList *StateListGetSubstates(StateList *state_list, int index);
int StateListSize(StateList *state_list);
void StateListPrint(StateList *state_list);

#endif