#include <stdio.h>
#include <stdlib.h>
#include <lib.h>

int main(int argc, char** argv)
{
    message m;
    int ret;

    if(argc < 3)
    {
        fprintf(stderr, "usage: %s pid group_id\n", argv[0]);
        return 1;
    }

    m.m1_i1 = atoi(argv[1]);
    m.m1_i2 = atoi(argv[2]);
    ret = _syscall(MM, SETGROUP, &m);

    printf("status: %d\n", ret);
    return ret;
}
