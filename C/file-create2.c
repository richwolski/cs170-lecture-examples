#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/*
 * open to create a couple of files and write a strings into each
 */
int main(int argc, char **argv)
{
	int fd1;
	int fd2;
	char file_name1[4096];
	char file_name2[4096];
	char write_buffer[4096];
	int write_length;
	char *string;
	int i;
	int w;

	if(argc < 3) {
		printf("need to specify two file names as the first and second arguments\n");
		exit(1);
	}

	/*
	 * zero out the buffers for the file names
	 */
	for(i=0; i < sizeof(file_name1); i++) {
		file_name1[i] = 0;
		file_name2[i] = 0;
	}

	/*
	 * copy the first argument into a local buffer
	 */
	strncpy(file_name1,argv[1],sizeof(file_name1));
	file_name1[sizeof(file_name1)-1] = 0;

	/*
	 * and the second
	 */
	strncpy(file_name2,argv[2],sizeof(file_name2));
	file_name2[sizeof(file_name2)-1] = 0;

	/*
	 * try and open the first file for creation
	 */
	fd1 = open(file_name1,O_CREAT | O_WRONLY, 0600);
	if(fd1 < 0) {
		printf("failed to open %s for creation\n",file_name1);
		exit(1);
	}

	/*
	 * and the second
	 */
	fd2 = open(file_name2,O_CREAT | O_WRONLY,  0600);
	if(fd2 < 0) {
		printf("failed to open %s for creation\n",file_name2);
		close(fd1);
		exit(1);
	}

	/*
	 * both files are open, write a string into first
	 */
	string = 
         "This program brought to you by the council for better C programming";
	for(i=0; i < sizeof(write_buffer); i++) {
		write_buffer[i] = 0;
	}
	strncpy(write_buffer,string,sizeof(write_buffer));

	write_length = strlen(write_buffer);

	w = write(fd1,write_buffer,write_length);

	if(w != write_length) {
		printf("write didn't complete, w: %d, length: %d in %s\n",
			w,
			write_length,
			file_name1);
		close(fd1);
		close(fd2);
		exit(1);
	}

	/*
	 * and the second
	 */
	string = "C programming is both nutritious and great tasting.";
	for(i=0; i < sizeof(write_buffer); i++) {
		write_buffer[i] = 0;
	}
	strncpy(write_buffer,string,sizeof(write_buffer));

	write_length = strlen(write_buffer);

	w = write(fd2,write_buffer,write_length);

	if(w != write_length) {
		printf("write didn't complete, w: %d, length: %d in %s\n",
			w,
			write_length,
			file_name2);
		close(fd1);
		close(fd2);
		exit(1);
	}

	close(fd1);
	close(fd2);

	return(0);

}


