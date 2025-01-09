#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int I_global;

int main(int argc, char **argv)
{
	int *pointer_to_int;

	printf("address of I_global is %p\n",&I_global);

	pointer_to_int = &I_global;

	printf("contents of pointer_to_int are %p\n",pointer_to_int);

	return(0);
}


