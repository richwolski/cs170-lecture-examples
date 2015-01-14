#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/*
 * read from stdin and write to stdout
 */
int main(int argc, char **argv)
{
	char buffer[4096];

	memset(buffer,0,sizeof(buffer));

	read(0,buffer,sizeof(buffer));
	write(1,buffer,strlen(buffer));
	
	return(0);

}


