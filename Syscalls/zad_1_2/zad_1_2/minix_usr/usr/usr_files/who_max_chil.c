
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <lib.h>

int main( int argc, char ** argv )
{
    message m;
    int ret;
    int ignore;

    if( argc != 2 ) {
        fprintf( stderr, "Example of usage: ./binary_name 5, the number refers to the pid process to ignore" );
    	return 1;
    } 

    ignore = atoi(argv[1]);
    m.m1_i1 = ignore;
    ret = _syscall( MM, GETMAXOFSPRINGPID, & m );
    printf( "CurrentProcess[%d]: Parent of max offspring (its pid): %d\n", getpid(), ret );
    m.m1_i1 = ignore;
    ret = _syscall( MM, MAXOFFSPRING, & m );
    printf( "It has %d offspring\n", ret );
    printf( "----------\n" );
    return 0;
}
