#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * do some argument parsing with strncmp() and strncpy()
 */

int main(int argc, char **argv)
{
        int i;
	int next;
	char *p;
	char *a;
	char arg1[4096];
	int found = 0;

	for(i=1; i < argc; i++) {
		p = argv[i];
		if(strncmp(p,"arg1:",strlen("arg1:")) == 0) {
			next = i + 1;
			if(next >= argc) {
				printf("arg1 value missing\n");
				exit(1);
			}
			a = argv[next];
			strncpy(arg1,a,sizeof(arg1));
			arg1[strlen(a)] = 0;
			found = 1;
		}
	}

	if(found == 1) {
		printf("arg1 found with value %s\n",
				arg1);
	} else {
		printf("no arg1 value found\n");
	}

	return(0);
}
			
