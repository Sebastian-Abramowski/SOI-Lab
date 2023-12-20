#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "msg_queue.h"


int main() {
    printf("Zaczecie czyszcenia pamieci wspolnej\n");
    fflush(stdout);

    int shmid[5]; 
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
        else {
            printf("Nie udalo sie usunac nieistniejacego segmentu (kolejki)");
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
            else {
                printf("Nie udalo sie usunac nieistniejacego segmentu (semafory)");
            }
        }
    }
    printf("Wspolna pamiec zostala wyczyszczona\n");

    return 0;
}
