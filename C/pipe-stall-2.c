#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#define PIPESIZE (1024*64)
#define WRITESIZE (16)

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
	

	signal(SIGPIPE,SIG_IGN);
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
		printf("pid: %d before reads\n",getpid());
		fflush(stdout);
		memset(read_buffer,0,sizeof(read_buffer));
		size = read(pd[0],read_buffer,sizeof(read_buffer));
		i = 0;
		while(size > 0) {
			printf("%c",read_buffer[0]);
			fflush(stdout);
			nanosleep(&req,NULL);
			memset(read_buffer,0,sizeof(read_buffer));
			size = read(pd[0],read_buffer,sizeof(read_buffer));
			i++;
			//if(i > 32) {
			//	close(pd[0]);
			//	exit(0);
			//}
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
		printf("pid: %d before writes of size %d\n",getpid(),
			WRITESIZE);
		fflush(stdout);
		i = 0;
		while(i < (2*PIPESIZE)) {
			err = write(pd[1],&write_buffer[i],WRITESIZE);
			if(err != WRITESIZE) {
				printf("write return %d\n",err);
				fflush(stdout);
				perror("pipe write ");
				exit(1);
			}
			printf("write %d done\n",i);
			fflush(stdout);
			i += WRITESIZE;
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


	

