#ifndef AFND_LIST_QUEUE_H
#define AFND_LIST_QUEUE_H

#include "int_list.h"
#include <stdbool.h>

typedef struct _IntListQueue IntListQueue;

IntListQueue *IntListQueueCreate();
void IntListQueueAdd(IntListQueue *int_list_queue, IntList int_list);
void IntListQueueFree(IntListQueue *int_list_queue);
int IntListQueueGet(IntListQueue *int_list_queue, IntList int_list);
int IntListQueueSize(IntListQueue *int_list_queue);
bool IntListQueueContains(IntListQueue *int_list_queue, IntList int_list);
void IntListQueuePrint(IntListQueue *int_list_queue);

#endif