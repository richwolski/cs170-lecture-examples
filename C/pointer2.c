#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * simple but pointless usages of pointers
 * basic data types using different memory allocations
 * demonstrates the compiler's use of type information for pointers
 */

int I_global;

int
main(int argc, char *argv[])
{
	int i_stack;
	int *i;

	/*
	 * first pint the pointers at the global variables
	 */
	i = &I_global;

	/*
	 * store a value in each global through the pointer
	 */
	*i = 17;

	printf("i: %p, *i: %d, I_global: %d\n", i,*i,I_global);

	/*
 	 * now do some math
	 */
	*i = *i + 5;
	printf("i: %p *i: %d after integer add\n",i,*i);


	i = &I_global;
	i = i + 10;

	printf("i: %p, I_global: %d\n",i,I_global);



	return(0);
}



