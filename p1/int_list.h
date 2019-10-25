#ifndef AFND_LIST_H
#define AFND_LIST_H

#include "afnd.h"
#include <stdbool.h>

typedef struct _IntList IntList;

IntList *IntListCreate();
void IntListAdd(IntList *int_list, int value);
void IntListFree(IntList *int_list);
int IntListGet(IntList *int_list, int index);
int IntListSize(IntList *int_list);
bool IntListContains(IntList *int_list, int value);
void IntListPrint(IntList *int_list);

#endif