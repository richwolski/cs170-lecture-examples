#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int pid;
	int cpid;
	int i;

	pid = fork();
	if(pid != 0) {
		printf("parent: %d, child: %d\n",getpid(),pid);
		sleep(2);
		printf("parent: %d, exiting\n",getpid());
	} else {
		cpid = fork();
		if(cpid != 0) {
			for(i=0; i < 10; i++) {
				printf("child: %d, parent: %d\n",getpid(),getppid());
				sleep(1);
			}
		} else {
			for(i=0; i < 15; i++) {
				printf("grandchild: %d, grandchild-parent: %d\n",getpid(),getppid());
				sleep(1);
			}
		}
	}
	exit(0);
}

