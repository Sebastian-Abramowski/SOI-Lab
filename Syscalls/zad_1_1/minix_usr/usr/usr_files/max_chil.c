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
    printf( "CurrentProcess[%d] will create %d children\n", pid, children );
    for( i = 0; i < children; ++i ) {
	    if( fork() == 0 ) {
	    	printf( "A single child was created\n" );
		    sleep( 10 );
		    return 0;
	    }
    }

    sleep( 1 );
    ret = _syscall( MM, GETMAXCHILDRENPID, & m );
    printf( "CurrentProcess[%d]: Parent of max children (its pid): %d\n", pid, ret );
    ret = _syscall( MM, MAXCHILDREN, & m );
    printf( "It has %d children\n", ret );
    printf( "----------\n" );
    return 0;
}
