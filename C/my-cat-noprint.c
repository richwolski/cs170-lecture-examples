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

	memset(buffer,0,sizeof(buffer));

	while(read(0,buffer,sizeof(buffer)) > 0) {
		buffer[4095] = 0; /* safety first */
		write(1,buffer,strlen(buffer));
		memset(buffer,0,sizeof(buffer));
	}

	exit(0);

}


