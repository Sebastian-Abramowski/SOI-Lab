#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>

int main()
{
    printf( "Process has %d pid\n", getpid());
     
    while(1);
}
