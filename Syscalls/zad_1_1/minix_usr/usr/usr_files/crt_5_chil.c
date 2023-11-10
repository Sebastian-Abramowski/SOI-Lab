#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <lib.h>
#include <stdlib.h>


int main( void )
{
    int children;
    int i;

    children = 5;

    printf( "CurrentProcess[%d] will create %d children\n", getpid(), children );
    for( i = 0; i < children; ++i ) {
	    if( fork() == 0 ) {
		    sleep( 10 );
		    return 0;
	    }
    }

    sleep( 10 );
    printf( "CurrentProcess[%d] ends\n", getpid() );
    printf( "----------\n" );
    return 0;
}
