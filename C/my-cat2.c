#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>

/*
 * simple program implementing cat
 */
int main(int argc, char **argv)
{
	pid_t my_id;
	char buffer[4096];
	char msg[4096];

	my_id = getpid();
	fprintf(stderr,"pid: %d -- I am my-cat and I have started\n",my_id);

	memset(buffer,0,sizeof(buffer));
	memset(msg,0,sizeof(msg));

	while(read(0,buffer,sizeof(buffer)) > 0) {
		sprintf(msg,"pid: %d read some data\n",getpid());
		write(1,msg,strlen(msg));
		buffer[4095] = 0; /* safety first */
		write(1,buffer,strlen(buffer));
		memset(msg,0,sizeof(msg));
		sprintf(msg,"pid: %d wrote some data\n",getpid());
		write(1,msg,strlen(msg));
		memset(buffer,0,sizeof(buffer));
	}

	fprintf(stderr,"pid: %d -- I am my-cat and I am exiting\n",my_id);

	exit(0);

}


