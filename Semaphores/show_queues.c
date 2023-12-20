#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#include "msg_queue.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s num_of_iterations\n", argv[0]);
        exit(1);
    }

    int shmid[5];
    MsgQueue *queues[5];

    for(int i=0; i < 5; i++) {
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
    }

    printf("Warehouse queue shmid: %d\n", shmid[WAREHOUSE_IN]);
    printf("Orders queue shmid: %d\n", shmid[ORDERS_IN]);
    printf("Tools queue shmid: %d\n", shmid[TOOLS_IN]);
    printf("Time of order queue shmid: %d\n", shmid[TIME_OF_ORDER_IN]);
    printf("Time of order queue2 shmid: %d\n\n", shmid[TIME_OF_ORDER_IN2]);
    
    int iterations = atoi(argv[1]);

    for(int i=0; i < iterations; i++) {
        usleep(500000);
        printf("---\n");
        printf("Tools: ");
        displayQueue(queues[TOOLS_IN]);
        printf("Orders: ");
        displayQueue(queues[ORDERS_IN]);
        printf("Time of order: ");
        displayQueue(queues[TIME_OF_ORDER_IN]);
        printf("Warehouse: ");
        displayQueue(queues[WAREHOUSE_IN]);
        printf("Time of order 2: ");
        displayQueue(queues[TIME_OF_ORDER_IN2]);
    }

    return 0;
}
