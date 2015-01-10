#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * print out the argument strings and their lengths
 * by stpeeing through each string a character at a time
 */

int main(int argc, char **argv)
{
        int i;
	char *p;
	int j;

        for(i=0; i < argc; i++) {
                printf("arg %d: ",i);
		p = argv[i];
		j = 0;
		while(p[j] != 0) {
			printf("%c",p[j]);
			j++;
		}
		printf(" len: %ld, j: %d\n", strlen(argv[i]),j);
		
        }

        return(0);
}


