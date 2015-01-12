#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * print out some simple pointer values to see alignment
 */

char C_array[10];
int I_array[10];
double D_array[10];

int
main(int argc, char *argv[])
{
	int *i_p;
	char *c_p;
	double *d_p;

	c_p = &(C_array[0]);
	printf("addr of first element in char array: %p\n",c_p);

	c_p = &(C_array[1]);
	printf("addr of second element in char array: %p\n",c_p);
	
	i_p = &(I_array[0]);
	printf("addr of first element in int array: %p\n",i_p);

	i_p = &(I_array[1]);
	printf("addr of second element in int array: %p\n",i_p);

	d_p = &(D_array[0]);
	printf("addr of first element in double array: %p\n",d_p);

	d_p = &(D_array[1]);
	printf("addr of second element in double array: %p\n",d_p);

	return(0);
}



