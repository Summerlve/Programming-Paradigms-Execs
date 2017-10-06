#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void ThreadSleep(int microSecs)
{
    struct timespec sleeper;
    sleeper.tv_sec = 0;
    sleeper.tv_nsec = microSecs * 10e6L; // micro secs 2 nano secs 
    nanosleep(&sleeper, NULL);
}

void *threadFunc(void *arg)
{
	char *str;
	int i = 0;

	str=(char*)arg;

	while(i < 110 )
	{
		ThreadSleep(1);
		printf("threadFunc says: %s\n",str);
		++i;
	}

	return NULL;
}

int main(void)
{
	pthread_t pth;	// this is our thread identifier
	int i = 0;

	pthread_create(&pth,NULL,threadFunc,"foo");
	
	while(i < 100)
	{
		ThreadSleep(1);
		printf("main is running...\n");
		++i;
	}

	printf("main waiting for thread to terminate...\n");
	pthread_join(pth,NULL);

	return 0;
}