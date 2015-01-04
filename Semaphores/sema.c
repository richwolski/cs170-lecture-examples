#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "sema.h"

sema *InitSem(int init_val)
{
	sema *s;

	s = (sema *)malloc(sizeof(sema));
	pthread_mutex_init(&(s->lock),NULL);
	pthread_cond_init(&(s->wait),NULL);
	s->count = init_val;

	return(s);
}

void FreeSem(sema *s)
{
	free(s);
	return;
}

void P(sema *s)
{
	pthread_mutex_lock(&(s->lock));
	s->count--;
	if(s->count < 0) {
		pthread_cond_wait(&(s->wait),&(s->lock));
	}
	pthread_mutex_unlock(&(s->lock));
	return;
}

void V(sema *s)
{
	pthread_mutex_lock(&(s->lock));
	s->count++;
	if(s->count <= 0) {
		pthread_cond_signal(&(s->wait));
	}
	pthread_mutex_unlock(&(s->lock));
	return;
}



