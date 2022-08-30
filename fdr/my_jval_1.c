#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "jval.h" // header for Jval API

int main(int argc, char **argv)
{
	Jval my_jval; // declare a Jval variable called my_jval

	my_jval.i = 7; // assign my_jval the integer 7 by accessing in i field

	printf("the integer value of my_jval is %d\n",my_jval.i); // print it out

	exit(0);
}

