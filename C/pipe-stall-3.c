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
#define READSIZE (1)
#define WRITESIZE (16)
#define WRITERS (10)
#define READERS (1)

/*
 * simple program creating a pipe between a set of processes
 */
int main(int argc, char **argv, char **envp)
{
	pid_t pid;
	int pd[2];
	char read_buffer[READSIZE];
	char write_buffer[WRITESIZE];
	int err;
	int size;
	struct timespec req;
	int i;
	int j;
	int k;
	int w;
	

	signal(SIGPIPE,SIG_IGN);
	err = pipe(pd);
	if(err < 0) {
		perror("pipe ");
		exit(1);
	}

	w = 0;
	for(i=0; i < WRITERS; i++) {
		pid = fork();
		if(pid < 0) {
			perror("fork: ");
			exit(1);
		}

		if(pid == 0) {
			for(k=0; k < sizeof(write_buffer); k++) {
				write_buffer[k] = '1' + w;
			}
			printf("pid: %d before writes of size %d\n",getpid(),
				sizeof(write_buffer));
			j = 0;
			while(j < PIPESIZE) {
				err = write(pd[1],write_buffer,sizeof(write_buffer));
				if(err != sizeof(write_buffer)) {
					printf("write return %d\n",err);
					fflush(stdout);
					perror("pipe write ");
					exit(1);
				}
				printf("write (pid: %d) %d\n",getpid(),j);
				fflush(stdout);
				j += sizeof(write_buffer);
			}
			printf("pid: %d wrote %d done\n",getpid(),j);
			fflush(stdout);
			close(pd[1]);
			exit(0);
		}
		w++;
	} // done with forking writers

	for(i=0; i < READERS; i++) {
		pid = fork();
		if(pid < 0) {
			perror("read fork");
			exit(0);
		}
		if(pid == 0) {
			close(pd[1]);
			req.tv_sec = 0;
			req.tv_nsec = (50000000*2); // 100ms
			printf("pid: %d before reads\n",getpid());
			fflush(stdout);
			memset(read_buffer,0,sizeof(read_buffer));
			size = read(pd[0],read_buffer,sizeof(read_buffer));
			while(size > 0) {
				for(j=0; j < size; j++) {
					printf("%c",read_buffer[j]);
					fflush(stdout);
				}
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
		}
	} // done with readers

	// so children exit
	close(pd[0]);
	close(pd[1]);
	// parent waits for all
	while(wait(NULL) > 0);

	printf("parent exiting\n");
	

	return(0);
}


	
