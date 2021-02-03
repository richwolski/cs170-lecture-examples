#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/*
 * open and read the contents of a file, printing them out as ascii
 * characters
 */
int main(int argc, char **argv)
{
	int read_fd;
	int write_fd;
	char file_name[4096];
	char buffer[4096];
	char read_back_buffer[4096];
	int r;
	int w;

	if(argc < 2) {
		printf("need to specify a file name as the first argument\n");
		exit(1);
	}

	/*
	 * zero out the buffer for the file name
	 */
	memset(file_name,0,sizeof(file_name));

	/*
	 * copy the argument into a local buffer
	 */
	strncpy(file_name,argv[1],sizeof(file_name));
	file_name[sizeof(file_name)-1] = 0;

	write_fd = open(file_name,O_CREAT | O_WRONLY,0600);
	if(write_fd < 0) {
		printf("failed to open %s for writing\n");
		exit(1);
	}

	/*
	 * try and open the file for reading
	 */
	read_fd = open(file_name,O_RDONLY,0);
	if(read_fd < 0) {
		printf("failed to open %s for reading\n",file_name);
		exit(1);
	}


	/*
 	 * read from the console
 	 */
	memset(buffer,0,sizeof(buffer));
	r = read(0,buffer,sizeof(buffer)-1);
	while(r > 0) {
		/*
 		 * write the string to a file
 		 */
		w = write(write_fd,buffer,strlen(buffer));
		if(w < 0) {
			printf("write failed for string %s\n",buffer);
			exit(1);
		}
		/*
 		 * read it back
 		 */
		memset(read_back_buffer,0,sizeof(read_back_buffer));
		r = read(read_fd,read_back_buffer,sizeof(read_back_buffer));
		if(r <= 0) {
			printf("read back failed for string %s\n",buffer);
			exit(1);
		}
		if(strcmp(buffer,read_back_buffer) != 0) {
			printf("read back fails to compare\n");
			printf("write: %s\n",buffer);
			printf("read back: %s\n",read_back_buffer);
			exit(1);
		} 
		memset(buffer,0,sizeof(buffer));
		r = read(0,buffer,sizeof(buffer)-1);
	}

	close(read_fd);
	close(write_fd);
	printf("program exiting\n");

	return(0);

}


