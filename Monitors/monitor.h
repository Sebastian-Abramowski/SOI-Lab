#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


class Semaphore
{
private:
	sem_t sem;
public:
	Semaphore( int value );
	~Semaphore();

	void p();

	void v();
};

class Condition
{
friend class Monitor;

private:
	Semaphore w;
	int waitingCount;
public:
	Condition();

	void wait();

	bool signal();
};


class Monitor
{
private:
	Semaphore s;
public:
	Monitor();

	void enter();

	void leave();

	void wait( Condition &cond );

	void signal( Condition &cond );
};

#endif // MONITOR_H
