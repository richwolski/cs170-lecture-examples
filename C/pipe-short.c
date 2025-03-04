#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

/*
 * simple program creating a pipe between a set of processes
 */
int main(int argc, char **argv, char **envp)
{
	pid_t pid;
	int pd[2];
	char read_buffer[4096];
	char write_buffer[4096];
	int err;
	int size;
	struct timespec req;
	

	err = pipe(pd);
	if(err < 0) {
		perror("pipe ");
		exit(1);
	}

	pid = fork();
	if(pid < 0) {
		perror("fork: ");
		exit(1);
	}
	if(pid == 0) {
		close(pd[1]);
		// child reads until pipe closes
		memset(read_buffer,0,sizeof(read_buffer));
		size = read(pd[0],read_buffer,sizeof(read_buffer));
		while(size > 0) {
			printf("pid: %d read (%d bytes) %s\n",getpid(),size,
				read_buffer);
			fflush(stdout);
			memset(read_buffer,0,sizeof(read_buffer));
			size = read(pd[0],read_buffer,sizeof(read_buffer));
		}
		if(size < 0) {
			perror("read error ");
			exit(1);
		}
		printf("pid: %d exiting\n",getpid());
		exit(0);
	} else {
		close(pd[0]);
		// first do two writes in succession
		printf("pid: %d first write of %s\n",getpid(),"12345");
		err = write(pd[1],"12345",strlen("12345"));
		if(err != strlen("12345")) {
			perror("pipe write 1 ");
			exit(1);
		}
		printf("pid: %d second write of %s\n",getpid(),"678910");
		err = write(pd[1],"678910",strlen("678910"));
		if(err != strlen("678910")) {
			perror("pipe write 1 ");
			exit(1);
		}
		printf("pid: %d two writes done\n",getpid());

		sleep(1);

		// now two two writes separated by a sleep
		printf("pid: %d third write of %s\n",getpid(),"abcdefg");
		err = write(pd[1],"abcdefg",strlen("abcdefg"));
		if(err != strlen("abcdefg")) {
			perror("pipe write 3 ");
			exit(1);
		}
	
		req.tv_sec = 0;
		req.tv_nsec = 500000000; //500 ms
		//req.tv_nsec = 10000; //0.1 ms
		nanosleep(&req,NULL);

		printf("pid: %d fourth write of %s\n",getpid(),"hijklm");
		err = write(pd[1],"hijklm",strlen("hijklm"));
		if(err != strlen("hijklm")) {
			perror("pipe write 4 ");
			exit(1);
		}
		printf("pid: %d writes three and four done\n",getpid());

		close(pd[1]);
		pid = wait(NULL);
		printf("pid: %d wait done exiting\n",getpid());
		exit(0);
	}

	return(0);
}


	

