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


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sleep_time>\n", argv[0]);
        exit(1);
    }

    int shmid[4];
    int semsid[4][3];
    MsgQueue *queues[4];

    for(int i=0; i<4; i++) {
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
        srand(time(NULL));
        int value = rand() % 11;

        P(semsid[ORDERS_IN][EMPTY]);
        P(semsid[ORDERS_IN][MUTEX]);
        P(semsid[TIME_OF_ORDER_IN][EMPTY]);
        P(semsid[TIME_OF_ORDER_IN][MUTEX]);

        push(queues[ORDERS_IN], value);
        int tm = (int)(time(NULL));
        push(queues[TIME_OF_ORDER_IN], tm);

        V(semsid[TIME_OF_ORDER_IN][MUTEX]);
        V(semsid[TIME_OF_ORDER_IN][FULL]);
        V(semsid[ORDERS_IN][MUTEX]);
        V(semsid[ORDERS_IN][FULL]);

        float sleepTime = atof(argv[1]);
        float multiplier = 0.5 + ((float)rand() / ((float)RAND_MAX));
        int sleepMicroseconds = (int)(sleepTime * multiplier * 1000000);
        usleep(sleepMicroseconds);
    }
}
