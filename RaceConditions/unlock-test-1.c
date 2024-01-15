#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/*
 * experimental code to test whether thread that is not holding and unlocked
 * lock can unlock it for another
 */

pthread_t T1;
pthread_t T2;
pthread_t T3;

pthread_mutex_t Lock;

void *Thread_1(void *arg)
{
	printf("Thread_1 about to lock\n");
	pthread_mutex_lock(&Lock);
	printf("Thread_1 calling pthread_exit\n");
	pthread_exit(NULL);
	return(NULL);
}

void *Thread_2(void *arg)
{
	int i;
	long j = 0;
	int err;
	printf("Thread_2 sleeping\n");
	for(i=0; i < 100000000; i++) {
		j++;
	}
	printf("Thread_2 awake and calling lock\n");
	pthread_mutex_lock(&Lock);
	printf("Thread_2 has the lock, calling unlock\n");
	err = pthread_mutex_unlock(&Lock);
	printf("Thread_2 for %d from unlock,  exiting\n",err);
	pthread_exit(NULL);
	return(NULL);
}

void *Thread_3(void *arg)
{
	int i;
	long j = 0;
	int err;

	printf("Thread_3 sleeping\n");
	for(i=0; i < 500000000; i++) {
		j++;
	}
	printf("Thread_3 awake and calling unlock\n");
	err = pthread_mutex_unlock(&Lock);
	printf("Thread_3  got %d from unlock, exiting\n",err);
	pthread_exit(NULL);
	return(NULL);
}

int main(int argc, char **argv)
{
	printf("main launching threads\n");
	(void)pthread_create(&T1,NULL,Thread_1,NULL);
	(void)pthread_create(&T2,NULL,Thread_2,NULL);
	(void)pthread_create(&T3,NULL,Thread_3,NULL);
	printf("main launched threads\n");
	(void)pthread_join(T1,NULL);
	printf("main joined with Thread_1\n");
	(void)pthread_join(T2,NULL);
	printf("main joined with Thread_2\n");
	(void)pthread_join(T3,NULL);
	printf("main joined with Thread_3\n");

	exit(0);
}
	



