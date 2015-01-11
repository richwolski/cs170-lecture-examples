#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int *foo()
{
	int foo_local;

	foo_local = 17;

        return(&foo_local);
}

int *bar()
{
	int bar_local;

	bar_local = 18;

        return(&bar_local);
}

void foobar(int *foobar_out)
{
	int* foo_ptr;
	int *bar_ptr;

	foo_ptr = foo();
	bar_ptr = bar();

	*foobar_out = *foo_ptr + *bar_ptr;

	return;
}

int main(int argc, char **argv)
{
        int foobar_val;

	foobar(&foobar_val);

	printf("foobar_val: %d\n",foobar_val);

        return(0);
}


