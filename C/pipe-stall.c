#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#define PIPESIZE (1024*64)

/*
 * simple program creating a pipe between a set of processes
 */
int main(int argc, char **argv, char **envp)
{
	pid_t pid;
	int pd[2];
	char read_buffer[1];
	char write_buffer[2*PIPESIZE];
	int err;
	int size;
	struct timespec req;
	int i;
	int j;
	

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
		req.tv_sec = 0;
		req.tv_nsec = (50000000*2); // 100ms
		//req.tv_nsec = (50000*2); // 0.1ms
		printf("pid: %d before reads\n",getpid());
		fflush(stdout);
		memset(read_buffer,0,sizeof(read_buffer));
		size = read(pd[0],read_buffer,sizeof(read_buffer));
		while(size > 0) {
			printf("%c",read_buffer[0]);
			fflush(stdout);
			nanosleep(&req,NULL);
			memset(read_buffer,0,sizeof(read_buffer));
			size = read(pd[0],read_buffer,sizeof(read_buffer));
		}
		if(size < 0) {
			perror("read error ");
			exit(1);
		}
		printf("\n");
		fflush(stdout);
		printf("pid: %d after reads\n",getpid());
		fflush(stdout);
		exit(0);
	} else {
		close(pd[0]);
		// first do two writes in succession
		j = 0;
		for(i=0; i < PIPESIZE*2; i++) {
			write_buffer[i] = 'A' + j;
			j++;
			if(('A' + j) > 'z') {
				j = 0;
			}
		}
		printf("pid: %d before big write\n",getpid());
		fflush(stdout);
		err = write(pd[1],write_buffer,sizeof(write_buffer));
		if(err != sizeof(write_buffer)) {
			perror("pipe write ");
			exit(1);
		}
		printf("pid: %d write done\n",getpid());
		fflush(stdout);
		close(pd[1]);
		pid = wait(NULL);
		printf("pid: %d wait done exiting\n",getpid());
		fflush(stdout);
		exit(0);
	}

	return(0);
}


	

