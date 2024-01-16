#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

int Counter = 0;
pthread_mutex_t Lock;
int MyTurn = 1;

void *Thread_1(void *arg)
{
	int i;
	int j;

	pthread_mutex_lock(&Lock);
	if(MyTurn == 2) {
		pthread_mutex_unlock(&Lock);
		for(j=0; j < 800000; j++);
		printf("Thread_1: not my turn, MyTurn: %d\n",MyTurn);
		return(NULL);
	}
	printf("Thread_1: it IS my turn, MyTurn: %d\n",MyTurn);
	MyTurn = 2;
	pthread_mutex_unlock(&Lock);
	return(NULL);
}

void *Thread_2(void *arg)
{
	int i;
	int j;

	pthread_mutex_lock(&Lock);
	if(MyTurn == 1) {
		pthread_mutex_unlock(&Lock);
		printf("Thread_2: not my turn, MyTurn: %d\n",MyTurn);
		return(NULL);
	}
	printf("Thread_2: it IS my turn: MyTurn: %d\n",MyTurn);
	MyTurn = 1;
	pthread_mutex_unlock(&Lock);
	return(NULL);
}

int main(int argc, char **argv)
{
	pthread_mutex_init(&Lock,NULL);
	pthread_t t1;
	pthread_t t2;

	while(Counter < 500) {
		pthread_create(&t1,NULL,Thread_1,NULL);
		pthread_create(&t2,NULL,Thread_2,NULL);
		pthread_join(t1,NULL);
		pthread_join(t2,NULL);
		Counter++;
	}

	exit(0);
}

	
		
	




