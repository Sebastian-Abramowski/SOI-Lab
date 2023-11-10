#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <stdlib.h>

void create_five_children_in_subprocess( void )
{
    int e = execv( "./crt_5_chil", NULL );
    if( e == - 1) {
        fprintf( stderr, "error while calling ./crt_5_chil" );
    }
}


int main( void )
{
    message m;
    pid_t p;
    int ret;
    int children;
    int i;

    printf( "----------\n" );
    printf( "[MAIN PROCESS] Current process id is: %d\n", getpid() );

    p = fork();
    if( p == 0 ) {
        printf( "Process %d created 5 children", getpid() );
        create_five_children_in_subprocess();
    }
    else {
        sleep( 3 );
        ret = _syscall( MM, GETMAXCHILDRENPID, & m );
        printf( "CurrentProcess[%d]: Parent of max children (its pid): %d\n", getpid(), ret );
        ret = _syscall( MM, MAXCHILDREN, & m );
        printf( "It has %d children\n", ret );
    }

    return 0;
}
