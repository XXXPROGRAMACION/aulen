#ifndef AFND_LIST_QUEUE_H
#define AFND_LIST_QUEUE_H

#include "int_list.h"
#include <stdbool.h>

typedef struct _IntListQueue IntListQueue;

IntListQueue *IntListQueueCreate();
void IntListQueueFree(IntListQueue *int_list_queue);
void IntListQueueAdd(IntListQueue *int_list_queue, IntList *int_list);
IntList *IntListQueueRemove(IntListQueue *int_list_queue);
int IntListQueueIsEmpty(IntListQueue *int_list_queue);
void IntListQueuePrint(IntListQueue *int_list_queue);

#endif