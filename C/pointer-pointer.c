#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{

	int A;
	int *B;
	int **C;

	A = 10;
	B = &A;
	C = &B;

	printf("&A: %p, &B: %p, &C: %p\n",&A,&B,&C);
	printf("A: %d, B: %p, C: %p\n",A,B,C);
	printf("A: %d, B: %p, *C: %p\n",A,B,*C);
	printf("A: %d, *B: %d, **C: %d\n",A,*B,**C);

	return(0);


}


