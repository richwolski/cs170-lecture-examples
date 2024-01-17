#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

int Counter = 0;
pthread_mutex_t Lock0;
pthread_mutex_t Lock1;
pthread_mutex_t Lock2;
int MyTurn = 1;

/*
 * this version, each thread initially locks the other since the first
 * call to lock by a thread on its own lock might not block
 */

void *Thread_1(void *arg)
{
	int i;
	int j;

	printf("Thread_1: starting\n");
	pthread_mutex_lock(&Lock0);
	pthread_mutex_lock(&Lock2);
	if(MyTurn == 2) {
		printf("Thread_1: it is NOT my turn, MyTurn: %d\n",MyTurn);
		pthread_mutex_unlock(&Lock2); // unlock the other
		pthread_mutex_unlock(&Lock0); // exit critical section
		pthread_mutex_lock(&Lock1);
	// awake! reenter critical section
        	pthread_mutex_lock(&Lock0);
		printf("Thread_1: awake! MyTurn: %d\n",MyTurn);
	} 
	printf("Thread_1: it IS my turn, MyTurn: %d\n",MyTurn);
	MyTurn = 2;
	pthread_mutex_unlock(&Lock2);
	pthread_mutex_unlock(&Lock0);
	printf("Thread_1: exiting\n");
	return(NULL);
}

void *Thread_2(void *arg)
{
	int i;
	int j;

	pthread_mutex_lock(&Lock0);
	pthread_mutex_lock(&Lock1);
	if(MyTurn == 1) {
		printf("Thread_2: it is NOT my turn, MyTurn: %d\n",MyTurn);
		pthread_mutex_unlock(&Lock1); // unlock the other
		pthread_mutex_unlock(&Lock0);
		pthread_mutex_lock(&Lock2); // sleep
		pthread_mutex_lock(&Lock0); // reenter the critical section
		printf("Thread_2: awake!, MyTurn: %d\n",MyTurn);
	}
	printf("Thread_2: it IS my turn, MyTurn: %d\n",MyTurn);
	MyTurn = 1;
	pthread_mutex_unlock(&Lock1);
	pthread_mutex_unlock(&Lock0);
	return(NULL);
}

int main(int argc, char **argv)
{
	pthread_mutex_init(&Lock0,NULL);
	pthread_mutex_init(&Lock1,NULL);
	pthread_mutex_init(&Lock2,NULL);
	pthread_t t1;
	pthread_t t2;
	int iterations;

	if(argc < 2) {
		printf("usage: bad-test iterations\n");
		exit(1);
	}

	iterations = atoi(argv[1]);

	while(Counter < iterations) {
		pthread_create(&t1,NULL,Thread_1,NULL);
		pthread_create(&t2,NULL,Thread_2,NULL);
		pthread_join(t1,NULL);
		pthread_join(t2,NULL);
		Counter++;
	}

	printf("main done\n");
	exit(0);
}

	
		
	




