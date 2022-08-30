#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "jval.h" // header for Jval API

int main(int argc, char **argv)
{

	Jval *my_jval; // declare a Jval pointer variable called my_jval
	my_jval = (Jval *)malloc(sizeof(Jval)); // allocate the storage set my_jval to point to it

	my_jval->i = 7; // assign my_jval the integer 7 by accessing in i field
	printf("the integer value of my_jval is %d\n",my_jval->i); // print it out

	my_jval->d = 3.141596; // assign a double
	printf("the double value of my_jval is %f\n",my_jval->d); // print it out

	my_jval->s = "I love OS class more than any other class!";
	printf("the string value of my_jval is %s\n",my_jval->s); // print it out

	free(my_jval);

	exit(0);
}

