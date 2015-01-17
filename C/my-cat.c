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

	my_id = getpid();
	fprintf(stderr,"pid: %d -- I am my-cat and I have started\n",my_id);

	memset(buffer,0,sizeof(buffer));

	while(read(0,buffer,sizeof(buffer)) > 0) {
		fprintf(stderr,"pid: %d read some data\n",getpid());
		buffer[4095] = 0; /* safety first */
		write(1,buffer,strlen(buffer));
		fprintf(stderr,"pid: %d wrote some data\n",getpid());
		memset(buffer,0,sizeof(buffer));
	}

	fprintf(stderr,"pid: %d -- I am my-cat and I am exiting\n",my_id);

	exit(0);

}


