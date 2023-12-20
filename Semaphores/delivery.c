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

    FILE *file = fopen("delivery_info.txt", "w");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    fclose(file);

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
        usleep(250000);
        int tm, orderId;

        file = fopen("delivery_info.txt", "a");
        if (file == NULL) {
            perror("fopen");
            exit(1);
        }
        
        P(semsid[WAREHOUSE_IN][FULL]); 
        P(semsid[WAREHOUSE_IN][MUTEX]);
        P(semsid[TIME_OF_ORDER_IN2][FULL]);
        P(semsid[TIME_OF_ORDER_IN2][MUTEX]);

        pop(queues[WAREHOUSE_IN], &orderId);
        pop(queues[TIME_OF_ORDER_IN2], &tm);

        V(semsid[TIME_OF_ORDER_IN2][MUTEX]);
        V(semsid[TIME_OF_ORDER_IN2][EMPTY]);
        V(semsid[WAREHOUSE_IN][MUTEX]);
        V(semsid[WAREHOUSE_IN][EMPTY]);

        int seconds = time(NULL) - tm;
        fprintf(file, "Od przyjecia zlecenia (zlecenie o %d id) do wydania go kurierowi minelo %d sekund\n", orderId, seconds);
        fclose(file);

        usleep(250000);
    }
}
