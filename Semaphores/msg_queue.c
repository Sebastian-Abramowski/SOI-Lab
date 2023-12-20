#include <stdio.h>

#include "msg_queue.h"

void initializeQueue(MsgQueue *queue) {
    queue->nextIndex = 0;
}

int push(MsgQueue *queue, int value) {
    if (queue->nextIndex == QUEUE_SIZE) {
        return -1; 
    }

    queue->items[queue->nextIndex] = value;
    queue->nextIndex++;
    return 0;
}

int pop(MsgQueue *queue, int *value) {
    if (queue->nextIndex == 0) {
        return -1;
    }

    *value = queue->items[0];
    for (int i = 0; i < queue->nextIndex - 1; i++) {
        queue->items[i] = queue->items[i + 1];
    }
    queue->nextIndex--;
    return 0;
}

void displayQueue(MsgQueue *queue) {
    printf("Queue contents: ");
    for (int i = 0; i < queue->nextIndex; i++) {
        printf("%d ", queue->items[i]);
    }
    printf("\n");
}

