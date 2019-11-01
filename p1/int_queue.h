#ifndef INT_QUEUE_H
#define INT_QUEUE_H

#include <stdbool.h>

typedef struct _IntQueue IntQueue;

IntQueue *IntQueueCreate();
void IntQueueFree(IntQueue *int_queue);
void IntQueueAdd(IntQueue *int_queue, int value);
int IntQueueRemove(IntQueue *int_queue);
int IntQueueSize(IntQueue *int_queue);
bool IntQueueIsEmpty(IntQueue *int_queue);
void IntQueuePrint(IntQueue *int_queue);

#endif