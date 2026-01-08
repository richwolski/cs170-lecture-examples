#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
 * run a program using fork and execve
 */
int main(int argc, char **argv, char **envp)
{
	pid_t child_id;
	pid_t my_id;
	int child_status;
	int i;
	char file_name[4096];
	int err;

	if(argc < 2) {
		printf("must specify file name as first argument\n");
		exit(1);
	}

	memset(file_name,0,sizeof(file_name));
	strncpy(file_name,argv[1],sizeof(file_name));

	child_id = fork();
	if(child_id != 0) {
		my_id = getpid();
		printf("pid: %d -- I forked pid: %d for: %s\n",
			my_id,
			child_id,
			file_name);
		wait(&child_status);
		printf("pid: %d -- %s has completed with status: %d\n",
			(int)my_id,
			file_name,
			child_status);
	} else {
		my_id = getpid();
		printf("pid: %d -- I am the child and I am going to exec %s\n",
			(int)my_id,
			file_name);
		err = execve(file_name,&(argv[1]),envp);
		/*
		 * not reached if execve is successful
		 */
		printf("pid: %d -- execve of %s failed with error %d\n",
			(int)my_id,
			file_name,
			err);

	}

	printf("pid: %d -- I am exiting\n",my_id);
	exit(0);

}


