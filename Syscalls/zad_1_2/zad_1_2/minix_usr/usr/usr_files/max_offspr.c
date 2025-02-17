#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <stdlib.h>


int main( int argc, char ** argv )
{
    message m;
    int ret;
    int children;
    int i;
    pid_t pid = 0;

    if( argc != 2) {
    	fprintf( stderr, "You need to give some arguemnt to indicate the number of children to create from the current process e.g. ./binary_name 5" );
    	return 1;
    }


    pid = getpid();
    children = atoi(argv[1]);

    printf( "----------\n" );
    printf( "CurrentProcess[%d] will create %d children and each child will create its own one child\n", pid, children );
    for( i = 0; i < children; ++i ) {
	    if( fork() == 0 ) {
            fork();
		    sleep( 10 );
		    return 0;
	    }
    }

    sleep( 1 );
    m.m1_i1 = 1; /* IGNORING INIT PROCESS */
    ret = _syscall( MM, GETMAXOFSPRINGPID, & m );
    printf( "CurrentProcess[%d]: Parent of max offspring (its pid): %d\n", pid, ret );
    m.m1_i1 = 1; /* IGNORING INIT PROCESS */
    ret = _syscall( MM, MAXOFFSPRING, & m );
    printf( "It has %d offspring \n", ret );
    printf( "----------\n" );
    m.m1_i1 = (int)getpid();
    ret = _syscall( MM, DEBUGCOUNTOFFSPRING, & m );
    printf( "CurrentProcess[%d] has %d offspring\n", getpid(), ret );
    m.m1_i1 = 1;
    ret = _syscall( MM, DEBUGCOUNTOFFSPRING, & m );
    printf( "Process with PID 1 has %d offspring\n", ret );
    m.m1_i1 = 0;
    ret = _syscall( MM, DEBUGCOUNTOFFSPRING, & m );
    printf( "Process with PID 0 has %d offspring\n", ret );

    return 0;
}
