#ifndef INT_LIST_H
#define INT_LIST_H

#include <stdbool.h>

typedef struct _IntList IntList;

IntList *IntListCreate();
void IntListAdd(IntList *int_list, int value);
IntList *IntListCopy(IntList *int_list);
void IntListFree(IntList *int_list);
int IntListGet(IntList *int_list, int index);
int IntListSize(IntList *int_list);
bool IntListContains(IntList *int_list, int value);
int IntListGetIndex(IntList *int_list, int value);
void IntListSort(IntList *int_list);
bool IntListCompare(IntList *int_list_1, IntList *int_list_2);
void IntListPrint(IntList *int_list);

#endif