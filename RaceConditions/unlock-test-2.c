#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/*
 * experimental code to test unlocking a lock that is not locked
 * will block indefinitely of Thread_1 runs before Thread_3
 */

pthread_t T1;
pthread_t T2;
pthread_t T3;

pthread_mutex_t Lock;

void *Thread_1(void *arg)
{
	int err;
	int i;
	int j = 0;

	printf("Thread_1 sleeping\n");
	for(i=0; i < 100000000; i++) {
		j++;
	}
	printf("Thread_1 about to lock\n");
	err = pthread_mutex_lock(&Lock);
	printf("Thread_1 got %d from lock, calling pthread_exit\n",err);
	pthread_exit(NULL);
	return(NULL);
}

void *Thread_2(void *arg)
{
	int err;
	printf("Thread_2 calling unlock\n");
	pthread_mutex_unlock(&Lock);
	err = pthread_mutex_unlock(&Lock);
	printf("Thread_2 got %d from unlock, exiting\n",err);
	pthread_exit(NULL);
	return(NULL);
}

void *Thread_3(void *arg)
{
	int err;
	int i;
	int j =;

	printf("Thread_3 sleeping\n");
	for(i=0; i < 500000000; i++) {
		j++;
	}
	printf("Thread_3 about to lock\n");
	err = pthread_mutex_lock(&Lock);
	printf("Thread_3 got %d from lock,  calling unlock\n",err);
	err = pthread_mutex_unlock(&Lock);
	printf("Thread_3 got %d from unlock, exiting\n",err);
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
	



