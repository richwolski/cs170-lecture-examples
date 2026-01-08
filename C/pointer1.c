#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * simple but pointless usages of pointers
 * basic data types using different memory allocations
 */

int I_global;
double D_global;

int
main(int argc, char *argv[])
{
	int i_stack;
	double d_stack;
	int *i;
	double *d;
	void *m1;
	void *m2;

	/*
	 * first point the pointers at the global variables
	 */
	i = &I_global;
	d = &D_global;

	/*
	 * store a value in each global through the pointer
	 */
	*i = 17;
	*d = 3.1415;

	/*
	 * print out the values from the globals
	 */
	printf("I_global: %d\n",I_global);
	printf("D_global: %f\n",D_global);

	/*
	 * point the pointers to the stack variables
	 */
	i = &i_stack;
	d = &d_stack;
	
	/*
	 * store some different values
	 */
	*i = 43;
	*d = 2.71828;

	/*
	 * verify that the globals didn't change
	 */
	printf("I_global: %d i_stack: %d\n",I_global,i_stack);
	printf("D_global: %f d_stack: %f\n",D_global,d_stack);

	/*
	 * get some generic memory from malloc
	 */
	m1 = malloc(sizeof(int)); /* big enough to hold an int */
	m2 = malloc(sizeof(double)); /* big enough to hold a double */

	/*
	 * point the integer and double pointers at the malloed memory
	 * using a cast
	 */
	i = (int *)m1;
	d = (double *)m2;

	/*
	 * store another set of different values
	 */
	*i = 11;
	*d = 0.001;

	/*
	 * verify that the other values didn't change using the
	 * integer and double pointers
	 */
	printf("I_global: %d i_stack: %d *i: %d\n",I_global,i_stack,*i);
	printf("D_global: %f d_stack: %f *d: %f\n",D_global,d_stack,*d);

	/*
	 * verify that the generic memory contains these values
	 */
	printf("m1 points to an in with %d in it\n",*((int *)m1));
	printf("m2 points to a double with a %f in it\n",
			*((double *)m2));

	free(m1);
	free(m2);

	return(0);
}



