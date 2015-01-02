#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

double CTimer() {
	struct timeval tm;

	gettimeofday(&tm,NULL);
	return((double)tm.tv_sec + (double)(tm.tv_usec/1000000.0));
}

