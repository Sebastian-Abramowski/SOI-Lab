#include <sys/sem.h>

#include "utils.h"

void P(int sem_id) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(sem_id, &op, 1);
}

void V(int sem_id) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;
    semop(sem_id, &op, 1);
}
