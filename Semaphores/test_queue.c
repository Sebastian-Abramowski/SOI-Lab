#include <stdio.h>

#include "msg_queue.h"


int main() {
    MsgQueue queue;
    initializeQueue(&queue);

    push(&queue, 1);
    push(&queue, 2);
    push(&queue, 3);
    printf("Added 1, 2, 3 into the queue\n");

    int value;
    pop(&queue, &value);
    printf("Removed value %d\n", value);
    pop(&queue, &value);
    printf("Removed value %d\n", value);
    pop(&queue, &value);
    printf("Removed value %d\n", value);

    if (pop(&queue, &value) == -1) {
        printf("Pop operation from empty queue...\n");
    }

    return 0;
}
