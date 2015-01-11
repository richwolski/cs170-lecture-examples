#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void foo(int *foo_out)
{
	*foo_out = 17;
        return;
}

void bar(int *bar_out)
{
	*bar_out = 18;
        return;
}

void foobar(int *foobar_out)
{
	int foo_val;
	int bar_val;

	foo(&foo_val);
	bar(&bar_val);

	*foobar_out = foo_val+bar_val;

	return;
}

int main(int argc, char **argv)
{
        int foobar_val;

	foobar(&foobar_val);

	printf("foobar_val: %d\n",foobar_val);

        return(0);
}


