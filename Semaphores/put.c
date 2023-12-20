#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "msg_queue.h"


int main() {
    printf("Przydzielanie pamieci wspolnej dla kolejek i semaforow\n");
    fflush(stdout);

    int shmid[5]; 
    MsgQueue *queues[5]; 
    int semsid[5][3];

    for (int i = 0; i < 5; i++) {
        key_t key = ftok("/home/sebastian/queues", i);
        
        shmid[i] = shmget(key, sizeof(MsgQueue), 0666);
        if (shmid[i] != -1) {
            if (shmctl(shmid[i], IPC_RMID, NULL) == -1) {
                perror("shmctl");
                exit(1);
            }
        }

        shmid[i] = shmget(key, sizeof(MsgQueue), IPC_CREAT | IPC_EXCL);
        if (shmid[i] == -1) {
            perror("shmget");
            exit(1);
        }

        queues[i] = (MsgQueue *)shmat(shmid[i], 0, 0);
        if (queues[i] == (void *)-1) {
            perror("shmat");
            exit(1);
        }
        initializeQueue(queues[i]);

        if (i == TOOLS_IN) {
            push(queues[i], A_TOOL);
            push(queues[i], B_TOOL);
        }
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            key_t key = ftok("/home/sebastian/sems", 10*i + j);
            
            semsid[i][j] = semget(key, 1, 0666);
            if (semsid[i][j] != -1) {
                if (semctl(semsid[i][j], 0, IPC_RMID) == -1) {
                    perror("semctl");
                    exit(1);
                }
            }

            semsid[i][j] = semget(key, 1, IPC_CREAT | IPC_EXCL);
            if (semsid[i][j] == -1) {
                perror("semget");
                exit(1);
            }

            if (j == 0) {
                // Semafor pelnosci
                if (i == TOOLS_IN)
                    semctl(semsid[i][j], 0, SETVAL, 2); 
                else
                    semctl(semsid[i][j], 0, SETVAL, 0); 
            } else if (j == 1) {
                // Semafor maksymalnej pojemnosci
                if (i == TOOLS_IN)
                    semctl(semsid[i][j], 0, SETVAL, 0);   
                else
                    semctl(semsid[i][j], 0, SETVAL, QUEUE_SIZE); 
            } else {
                // Semafor dostepu (mutex)
                semctl(semsid[i][j], 0, SETVAL, 1);
            }
        }
    }

    printf("Wspolna pamiec zostala przydzielona\n");

    for (int i = 0; i < 5; i++) {
        if (shmdt(queues[i]) == -1) {
            perror("shmdt");
            exit(1);
        }
    }

    return 0;
}
