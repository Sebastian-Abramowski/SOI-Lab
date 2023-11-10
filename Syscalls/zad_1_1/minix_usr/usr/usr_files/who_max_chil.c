
#include <stdio.h>
#include <unistd.h>
#include <lib.h>

int main( void )
{
    message m;
    int ret = _syscall( MM, GETMAXCHILDRENPID, & m );
    printf( "----------\n" );
    printf( "CurrentProcess[%d]: Parent of max children (its pid): %d\n", getpid(), ret );
    ret = _syscall( MM, MAXCHILDREN, & m );
    printf( "It has %d children\n", ret );
    printf( "----------\n" );
    return 0;
}
