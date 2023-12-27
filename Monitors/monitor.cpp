#include "monitor.h"

Semaphore::Semaphore(int value)
{
    if( sem_init( &sem, 0, value ) != 0 )
		throw "sem_init: failed";
}

Semaphore::~Semaphore()
{
	sem_destroy( &sem );
}

void Semaphore::p()
{
    if( sem_wait( &sem ) != 0 )
		throw "sem_wait: failed";
}

void Semaphore::v()
{
    if( sem_post( &sem ) != 0 )
        throw "sem_post: failed";
}




Condition::Condition(): w( 0 )
{
    waitingCount = 0;
}

void Condition::wait()
{
    w.p();
}

bool Condition::signal()
{
    if( waitingCount )
    {
        --waitingCount;
        w.v();
        return true;
    }
    else
        return false;
}




Monitor::Monitor(): s( 1 ) {}

void Monitor::enter()
{
    s.p();
}

void Monitor::leave()
{
    s.v();
}

void Monitor::wait(Condition &cond)
{
    ++cond.waitingCount;
    leave();
    cond.wait();
}

void Monitor::signal(Condition &cond)
{
    if( cond.signal() )
        enter();
}
