#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * print out the argument strings and their lengths
 */

int main(int argc, char **argv)
{
        int i;

        for(i=0; i < argc; i++) {
                printf("arg %d: is %s with length %ld\n",
                        i,
                        argv[i],
                        strlen(argv[i]));
        }

        return(0);
}


