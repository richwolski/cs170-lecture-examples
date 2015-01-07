#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "int_add.h"
#include "int_sub.h"

int
main(int argc, char *argv[])
{
	int first_num = 10;
	int second_num = 20;

	printf("the sum of %d and %d is %d\n",
			first_num,
			second_num,
			IntegerAdd(first_num,second_num));
	printf("the difference between %d and %d is %d\n",
			first_num,
			second_num,
			IntegerSub(first_num,second_num));

	return(0);
}



