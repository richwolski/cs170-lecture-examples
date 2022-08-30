#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "jval.h" // header for Jval API

int main(int argc, char **argv)
{

	Jval my_jval_1; // declare a Jval variable called my_jval_1
	Jval my_jval_2; // declare a Jval variable called my_jval_2
	Jval my_jval_3; // declare a Jval variable called my_jval_3

	my_jval_1 = new_jval_i(7); // create an integer jval and load 7 into it
	printf("the integer value of my_jval is %d\n",jval_i(my_jval_1)); // print it out using the accessor function for integers

	my_jval_2 = new_jval_d(3.141596); // assign a double to a different jval
	printf("the double value of my_jval is %f\n",jval_d(my_jval_2)); // print it out using the accessor

	my_jval_3 = new_jval_s("I love OS class more than any other class!");
	printf("the string value of my_jval is %s\n",jval_s(my_jval_3)); // print it out

	exit(0);
}

