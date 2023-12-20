#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

#include "msg_queue.h"
#include "utils.h"


int main() {
    int shmid[5];
    int semsid[5][3];
    MsgQueue *queues[5];

    for(int i=0; i<5; i++) {
        key_t key = ftok("/home/sebastian/queues", i); 
        shmid[i] = shmget(key, sizeof(MsgQueue), 0666);
        if (shmid[i] == -1) {
            perror("shmget");
            exit(1);
        }

        queues[i] = (MsgQueue *)shmat(shmid[i], 0, 0);
        if (queues[i] == (void *)-1) {
            perror("shmat");
            exit(1);
        }

        for (int j=0; j < 3; j++) {
            key = ftok("/home/sebastian/sems", 10*i + j);
            semsid[i][j] = semget(key, 1, 0666);
            if (semsid[i][j] == -1) {
                perror("semget");
                exit(1);
            }
        }
    }

    while(1) {
        int tool, orderId, timeOrdered;

        P(semsid[ORDERS_IN][FULL]);
        P(semsid[ORDERS_IN][MUTEX]);
        P(semsid[TOOLS_IN][MUTEX]); // Zawsze tutaj sa dwa narzedzia
        P(semsid[TIME_OF_ORDER_IN][FULL]);
        P(semsid[TIME_OF_ORDER_IN][MUTEX]);

        pop(queues[TOOLS_IN], &tool);
        if (tool == A_TOOL)
            push(queues[TOOLS_IN], A_TOOL);
        else
            push(queues[TOOLS_IN], B_TOOL);
        pop(queues[ORDERS_IN], &orderId);
        pop(queues[TIME_OF_ORDER_IN], &timeOrdered);

        V(semsid[TIME_OF_ORDER_IN][MUTEX]);
        V(semsid[TIME_OF_ORDER_IN][EMPTY]);
        V(semsid[TOOLS_IN][MUTEX]);
        V(semsid[ORDERS_IN][MUTEX]);
        V(semsid[ORDERS_IN][EMPTY]);

        if (tool == A_TOOL)
            sleep(1);
        else
            sleep(2);
        
        P(semsid[WAREHOUSE_IN][EMPTY]);
        P(semsid[WAREHOUSE_IN][MUTEX]);
        P(semsid[TIME_OF_ORDER_IN2][EMPTY]);
        P(semsid[TIME_OF_ORDER_IN2][MUTEX]);

        push(queues[WAREHOUSE_IN], orderId);
        push(queues[TIME_OF_ORDER_IN2], timeOrdered);

        V(semsid[TIME_OF_ORDER_IN2][MUTEX]);
        V(semsid[TIME_OF_ORDER_IN2][FULL]);
        V(semsid[WAREHOUSE_IN][MUTEX]);
        V(semsid[WAREHOUSE_IN][FULL]);

        sleep(1);
    }
}
