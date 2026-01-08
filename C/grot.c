#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct array_struct
{
	int size;
	int *A;
	int rows;
	int cols;
};

struct array_struct *MakeArray(int rows, int cols)
{
	struct array *as;

	as = (struct array_struct *)malloc(sizeof(struct array_struct));
	as->A = (int *)malloc(rows*cols*sizeof(int));
	as->rows = rows;
	as->cols = cols;
	as->size = rows*cols;
	return(as);
}
	

int
main(int argc, char *argv[])
{
	int size = 0;
	int A[];

	size = atoi(argv[1]);
	A = (int *)malloc(sizeof(int)*b);

	printf("size: %d\n",size);

	return(0);
}



