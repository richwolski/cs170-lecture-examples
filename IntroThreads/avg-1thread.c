#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

/*
 * program to find the average value of a set of random numbers
 * which forks off a single thread to do the sum computation
 *
 * usage: avg-1thread count
 *
 * where count is the number of random values to generate
 */

char *Usage = "usage: avg-1thread count";

#define RAND() (drand48()) /* basic Linux random number generator */

/*
 * data type definition for arguments passed to thread
 */
struct arg_struct
{
	int size;
	double *data;
};

/*
 * data type definition for results passed back from threads
 */
struct result_struct
{
	double sum;
};

void *SumThread(void *arg)
{
	int i;
	double my_sum;
	struct arg_struct *my_args;
	int my_size;
	double *my_data;
	struct result_struct *result;

	printf("sum thread running\n");
	fflush(stdout);

	my_args = (struct arg_struct *)arg;
	result = (struct result_struct *)malloc(sizeof(struct result_struct));
	if(result == NULL) {
		exit(1);
	}

	my_size = my_args->size;
	my_data = my_args->data;

	/*
	 * free the arg structure since it isn't needed any more
	 */
	free(my_args);

	my_sum = 0.0;
	for(i=0; i < my_size; i++) {
		my_sum += my_data[i];
	}

	result->sum = my_sum;
	printf("sum thread done, returning\n");
	fflush(stdout);

	return((void *)result);
}

int main(int argc, char **argv)
{
	int i;
	int n;
	double *data;
	int count;
	struct arg_struct *args;
	pthread_t thread_id;
	struct result_struct *result;
	int err;

	/*
	 * santity check the arguments
	 */
	if(argc <= 1) {
		fprintf(stderr,"must specify count\n");
		fprintf(stderr,"%s",Usage);
		exit(1);
	}

	count = atoi(argv[1]);	/* count is first argument */ 

	if(count <= 0) {
		fprintf(stderr,"invalid count %d\n",count);
		exit(1);
	}

	/*
	 * make an array large enough to hold #count# doubles
	 */
	data = (double *)malloc(count * sizeof(double));
	if(data == NULL) {
		exit(1);
	}

	/*
	 * pick a bunch of random numbers
	 */
	for(i=0; i < count; i++) {
		data[i] = RAND();
	}

	/*
	 * make a argument structure to use to pass the arguments to the
	 * thread
	 */
	args = (struct arg_struct *)malloc(sizeof(struct arg_struct));
	if(args == NULL) {
		exit(1);
	}

	/*
	 * load into the arguments the size and the pointer to the data
	 */
	args->size = count;
	args->data = data;

	/*
	 * fork off a thread to get the sum from the data
	 */
	printf("main thread forking sum thread\n");
	fflush(stdout);
	err = pthread_create(&thread_id, NULL, SumThread, (void *)args);
	if(err != 0) {
		fprintf(stderr,"pthread create failed\n");
		exit(1);
	}

	printf("main thread running after sum thread created, about to call join\n");
	fflush(stdout);

	/*
	 * wait until the sum thread is finished and get the result back
	 */
	err = pthread_join(thread_id,(void **)&result);
	if(err != 0) {
		fprintf(stderr,"pthread_join failed\n");
		exit(1);
	}
	printf("main thread joined with sum thread\n");
	fflush(stdout);

	printf("the average over %d random numbers on (0,1) is %f\n",
			count, result->sum / (double)count);

	/*
	 * free the result structure
	 */
	free(result);

	/*
	 * free the data array -- it wasn't freed in the sum thread
	 */
	free(data);

	return(0);
}

