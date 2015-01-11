#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int A;

int foo()
{
        int A;
        A = 15;
        return(A);
}

int bar()
{
        int A;
        A = 20;
        return(A);
}

int main(int argc, char **argv)
{
        int foo_val;
        int bar_val;

        A = 7;
        printf("A: %d\n",A);
        foo_val = foo();
        printf("A: %d, foo_val: %d\n",A,foo_val);
        bar_val = bar();
        printf("A: %d, bar_val: %d\n",A,bar_val);

        return(0);
}


