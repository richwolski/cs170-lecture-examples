#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
 * simple program creating a pipe between a set of processes
 * that read and write stdin and stdout
 * first child get stdin of parent
 * last child gets stdout of parent
 */
int main(int argc, char **argv, char **envp)
{
	pid_t child_id;
	int child_status;
	int pipe_desc[2];
	char read_buffer[4096];
	int err;
	char file_name[4096];
	int proc_count;
	int i;
	int last_stdout;

	if(argc < 3) {
		printf("usage: pipe-4 executable-file number-of-procs\n");
		exit(1);
	}

	/*
	 * get the arguments
	 */
	memset(file_name,0,sizeof(file_name));
	strncpy(file_name,argv[1],sizeof(file_name));
	proc_count = atoi(argv[2]);

	/*
	 * save the parent stdout for the last child
	 */
	last_stdout = dup(1);

	/*
	 * create pipes, fork and exec the processes
	 */
	for(i=0; i < proc_count; i++) {
		/*
	 	 * create the pipe
	 	 */
		err = pipe(pipe_desc);
		if(err < 0) {
			printf("error creating pipe\n");
			exit(1);
		}

		/*
	 	 * then fork
	 	 */
		child_id = fork();
		if(child_id == 0) {
			/*
			 * child closes standard out
			 */
			close(1);
			/*
			 * child dups the write end of the pipe
			 * if it is not the last child, otherwise
			 * it dups the last_stdout it got from the parent
			 * the closed stdout will be chosen as the
			 * target
			 */
			if(i < (proc_count - 1)) {
				dup2(pipe_desc[1],1);
			} else {
				dup2(last_stdout,1);
				close(pipe_desc[1]);
			}
			/*
		 	 * child runs the program
		 	 */
			err = execve(file_name,&argv[1],envp);
			printf("parent error: %s didn't exec\n",
				file_name);
			exit(1);
		} else {
			/*
			 * parent closes standard in
			 */
			close(0);
			/*
			 * parent dups the read end of the pipe for the
			 * next child
			 */
			if(i < (proc_count - 1)) {
				dup2(pipe_desc[0],0);
			}
			close(pipe_desc[0]);
			close(pipe_desc[1]);
		}
	}

	close(1);
	dup2(last_stdout,1);
	/*
	 * parent now waits for the children to exit
	 */
	for(i=0; i < proc_count; i++) {
		child_id = wait(&child_status);
	}
		

	exit(0);

}


