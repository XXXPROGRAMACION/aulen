#include "int_list_queue.h"
#include <stdlib.h>

typedef struct _Node Node;

struct _IntListQueue {
    Node *head;
    Node *tail;
};

struct _Node {
    IntList *int_list;
    Node *next;
};

IntListQueue *IntListQueueCreate() {
    IntListQueue *int_list_queue;

    int_list_queue = (IntListQueue *) malloc(sizeof(IntListQueue));
    if (int_list_queue == NULL) return NULL;

    int_list_queue->head = NULL;
    int_list_queue->tail = NULL;

    return int_list_queue;
}

void IntListQueueFree(IntListQueue *int_list_queue) {
    Node *current;
    Node *next;

    if (int_list_queue == NULL) return;

    current = int_list_queue->head;
    while (current != NULL) {
        IntListFree(current->int_list);
        next = current->next;
        free(current);
        current = next;
    }
    free(int_list_queue);
}

void IntListQueueAdd(IntListQueue *int_list_queue, IntList *int_list) {
    if (int_list_queue == NULL || int_list == NULL) return;

    if (int_list_queue->head == NULL) {
        int_list_queue->head = (Node *) malloc(sizeof(Node));
        if (int_list_queue->head == NULL) return;
        int_list_queue->tail = int_list_queue->head;
    } else {
        int_list_queue->tail->next = (Node *) malloc(sizeof(Node));
        if (int_list_queue->tail->next == NULL) return;
        int_list_queue->tail = int_list_queue->tail->next;
    }

    int_list_queue->tail->int_list = int_list;
    int_list_queue->tail->next = NULL;
}

IntList *IntListQueueRemove(IntListQueue *int_list_queue) {
    Node *head;
    IntList *head_int_list;
    if (int_list_queue == NULL || int_list_queue->head) return NULL;
    head = int_list_queue->head;
    int_list_queue->head = head->next;
    if (int_list_queue->head == NULL) int_list_queue->tail = NULL;
    head_int_list = head->int_list;
    free(head);
    return head_int_list;
}

int IntListIsEmpty(IntListQueue *int_list_queue) {
    if (int_list_queue == NULL) return true;
    return int_list_queue->head == NULL;
}

void IntListQueuePrint(IntListQueue *int_list_queue);