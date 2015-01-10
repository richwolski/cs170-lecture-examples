/*
 * cs170 -- Rich wolski
 * ping-pong code illustrating kt_joinall() functionality
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kt.h"

#define RAND() (drand48())

struct thread_arg
{
	int id;
	int count;
	double *shared_counter;
	kt_sem next;
	int *finished;
	int verbose;
};

/*
 * bumps the shared counter by 1 each time it is enabled
 */
void PingPongThread(void *arg)
{
	struct thread_arg *ta = (struct thread_arg *)arg;
	int i;

	for(i=0; i < ta->count; i++) {
		P_kt_sem(ta->next);
			*(ta->shared_counter) =
				*(ta->shared_counter) + 1.0;
		if(ta->verbose == 1) {
			printf("thread %d: incremented shared counter to %f\n",
				ta->id,
				*(ta->shared_counter));
		}
	}

	*(ta->finished) = *(ta->finished) + 1;

	kt_exit();
	return;
}

#define ARGS "t:VC:"
char *Usage = "ping-pong-joinall -C count -t threads -V <verbose on>\n";


int main(int argc, char **argv)
{
	int c;
	int threads;
	struct thread_arg *ta;
	void **ids;
	double counter;
	int done_threads;
	int count;
	int verbose;
	int i;
	kt_sem sema;
	

	/*
	 * defaults
	 */
	threads = 1;
	counter = 0;
	count = 1;
	while((c = getopt(argc,argv,ARGS)) != EOF) {
		switch(c) {
			case 'C':
				count = atoi(optarg);
				break;
			case 't':
				threads = atoi(optarg);
				break;
			case 'V':
				verbose = 1;
				break;
			default:
				fprintf(stderr,
					"unrecognized command %c\n",
						(char)c);
				fprintf(stderr,"usage: %s",Usage);
				exit(1);
		}
	}

	ids = (void **)malloc(threads*sizeof(void *));
	if(ids == NULL) {
		exit(1);
	}

	ta = (struct thread_arg *)malloc(threads*sizeof(struct thread_arg));
	if(ta == NULL) {
		exit(1);
	}

	sema = make_kt_sem(0);
	if(sema == NULL) {
		exit(1);
	}

	done_threads = 0;
	for(i=0; i < threads; i++) {
		ta[i].id = i;
		ta[i].next = sema;
		ta[i].count = count;
		ta[i].shared_counter = &counter;
		ta[i].finished = &done_threads;
		ta[i].verbose = verbose;
		ids[i] = kt_fork(PingPongThread,(void *)&ta[i]);
		if(ids[i] == NULL) {
			fprintf(stderr,"thread create %d failed\n",i);
			exit(1);
		}
	}

	/*
 	 * loop showing progress
 	 */
	while(done_threads < threads) {
		V_kt_sem(sema);	/* enable a thread */
		printf("shared counter: %f\n",counter);
		fflush(stdout);
		kt_joinall(); /* gets here when all else is stopped */
	}

	/*
	 * join with the threads after exit
	 */
	for(i=0; i < threads; i++) {
		kt_join(ids[i]);
	}

	kill_kt_sem(sema);
	free(ta);
	free(ids);

	printf("final count: %f\n",counter);

	return(0);
}
		


		
		
		

	
	



	
	

