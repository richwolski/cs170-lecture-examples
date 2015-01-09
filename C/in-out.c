#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void NewFunction(int *i1, int **p1)
{
	int *j;

	*i1 = 10;

	j = (int *)malloc(sizeof(int));
	*j = 12;
	*p1 = j;

	return;
}

int main(int argc, char **argv)
{
	int I;
	int *J;

	NewFunction(&I,&J);

	printf("I: %d, J: %p, *J: %d\n",I,J,*J);
	free(J);

	return(0);
}

