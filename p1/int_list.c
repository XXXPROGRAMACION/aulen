#include "int_list.h"
#include <stdlib.h>
#include <stdio.h>

struct _IntList {
    int *values;
    int size;
    int max_size;
};

void IntListResize();

IntList *IntListCreate() {
    IntList *int_list;

    int_list = (IntList *) malloc(sizeof(IntList));
    if (int_list == NULL) return NULL;

    int_list->size = 0;
    int_list->max_size = 1;

    int_list->values = (int *) malloc(sizeof(int)*int_list->max_size);
    if (int_list->values == NULL) {
        free(int_list);
        return NULL;
    }

    return int_list;
}

void IntListFree(IntList *int_list) {
    if (int_list == NULL) return;
    free(int_list->values);
    free(int_list);
}

void IntListAdd(IntList *int_list, int value) {
    if (int_list == NULL) return;
    if (int_list->size >= int_list->max_size) IntListResize(int_list);
    int_list->values[int_list->size] = value;
    int_list->size++;
}

int IntListGet(IntList *int_list, int index) {
    if (int_list == NULL) return -1;
    if (index < 0 || index >= int_list->size) return -1;
    return int_list->values[index];
}

int IntListSize(IntList *int_list) {
    if (int_list == NULL) return -1;
    return int_list->size;
}

bool IntListContains(IntList *int_list, int value) {
    int i;
    if (int_list == NULL) return false;
    for (i = 0; i < int_list->size; i++) {
        if (int_list->values[i] == value) return true;
    }
    return false;
}

void IntListSort(IntList *int_list) {
    int i, j, aux;
    for (i = int_list->size-1; i >= 1; i--) {
        for (j = 0; j < i; j++) {
            if (int_list->values[j] > int_list->values[j+1]) {
                aux = int_list->values[j];
                int_list->values[j] = int_list->values[j+1];
                int_list->values[j+1] = aux;
            }
        }
    }
}

bool IntListCompare(IntList *int_list_1, IntList *int_list_2) {
    int i;
    if (int_list_1 == NULL || int_list_2 == NULL) return false;
    if (int_list_1->size != int_list_2->size) return false;
    for (i = 0; i < int_list_1->size; i++) {
        if (int_list_1->values[i] != int_list_2->values[i]) return false;
    }
    return true;
}

void IntListPrint(IntList *int_list) {
    int i;
    if (int_list == NULL) {
        printf("La lista que se intenta imprimir es nula\n");
        return;
    }

    printf("[");
    if (int_list->size > 0) {
        for (i = 0; i < int_list->size-1; i++) {
            printf("%d, ", int_list->values[i]);
        }
        printf("%d", int_list->values[int_list->size-1]);
    }
    printf("]\n");
}

void IntListResize(IntList *int_list) {
    if (int_list == NULL) return;
    int_list->max_size *= 2;
    int_list->values = (int *) realloc(int_list->values, sizeof(int)*int_list->max_size);
}