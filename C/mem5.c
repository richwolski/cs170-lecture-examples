#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int foo(int a, int b, int c)
{
	a = a + 10;
        return(a+b+c);
}

int main(int argc, char **argv)
{
	int foo_return;

	foo_return = foo(100,101,102);

	printf("foo_return: %d\n",foo_return);

        return(0);
}


