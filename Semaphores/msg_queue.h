#ifndef MSQ_QUEUE_H
#define MSQ_QUEUE_H

#define QUEUE_SIZE 10

#define WAREHOUSE_IN 0
#define ORDERS_IN 1
#define TOOLS_IN 2
#define TIME_OF_ORDER_IN 3
#define TIME_OF_ORDER_IN2 4

#define FULL 0
#define EMPTY 1
#define MUTEX 2

#define A_TOOL 65
#define B_TOOL 66

typedef struct {
    int items[QUEUE_SIZE];
    int nextIndex;
} MsgQueue;

void initializeQueue(MsgQueue *queue);
int push(MsgQueue *queue, int value);
int pop(MsgQueue *queue, int *value);
void displayQueue(MsgQueue *queue);

#endif
