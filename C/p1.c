#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "int_add.h"

int
main(int argc, char *argv[])
{
	int first_num = 10;
	int second_num = 20;

	printf("the sum of %d and %d is %d\n",
			first_num,
			second_num,
			IntegerAdd(first_num,second_num));

	return(0);
}



