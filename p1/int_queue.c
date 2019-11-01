#include "int_queue.h"
#include <stdlib.h>

typedef struct _Node Node;

struct _IntQueue {
    Node *head;
    Node *tail;
    int size;
};

struct _Node {
    int value;
    Node *next;
};

IntQueue *IntQueueCreate() {
    IntQueue *int_queue;

    int_queue = (IntQueue *) malloc(sizeof(IntQueue));
    if (int_queue == NULL) return NULL;

    int_queue->head = NULL;
    int_queue->tail = NULL;
    int_queue->size = 0;

    return int_queue;
}

void IntQueueFree(IntQueue *int_queue) {
    Node *current;
    Node *next;

    if (int_queue == NULL) return;

    current = int_queue->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(int_queue);
}

void IntQueueAdd(IntQueue *int_queue, int value) {
    if (int_queue == NULL) return;

    if (int_queue->head == NULL) {
        int_queue->head = (Node *) malloc(sizeof(Node));
        if (int_queue->head == NULL) return;
        int_queue->tail = int_queue->head;
    } else {
        int_queue->tail->next = (Node *) malloc(sizeof(Node));
        if (int_queue->tail->next == NULL) return;
        int_queue->tail = int_queue->tail->next;
    }

    int_queue->tail->value = value;
    int_queue->tail->next = NULL;
    int_queue->size++;
}

int IntQueueRemove(IntQueue *int_queue) {
    Node *head;
    int value;
    if (int_queue == NULL || int_queue->head == NULL) return -1;
    head = int_queue->head;
    int_queue->head = head->next;
    if (int_queue->head == NULL) int_queue->tail = NULL;
    value = head->value;
    free(head);
    int_queue->size--;
    return value;
}

int IntQueueSize(IntQueue *int_queue) {
    if (int_queue == NULL) return -1;
    return int_queue->size;
}

bool IntQueueIsEmpty(IntQueue *int_queue) {
    if (int_queue == NULL) return true;
    return int_queue->head == NULL;
}

void IntQueuePrint(IntQueue *int_queue);