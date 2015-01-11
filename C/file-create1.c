#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/*
 * open to create a file and write a string into it
 */
int main(int argc, char **argv)
{
	int my_file_desc;
	char file_name[4096];
	char write_buffer[4096];
	int write_length;
	char *string;
	int i;
	int w;

	if(argc < 2) {
		printf("need to specify a file name as the first argument\n");
		exit(1);
	}

	/*
	 * zero out the buffer for the file name
	 */
	for(i=0; i < sizeof(file_name); i++) {
		file_name[i] = 0;
	}

	/*
	 * copy the argument into a local buffer
	 */
	strncpy(file_name,argv[1],sizeof(file_name));
	file_name[sizeof(file_name)-1] = 0;

	/*
	 * try and open the file for creation
	 */
	my_file_desc = open(file_name,O_CREAT | O_WRONLY,0600);
	if(my_file_desc < 0) {
		printf("failed to open %s for creation\n",file_name);
		exit(1);
	}

	/*
	 * file is open, write a string into it
	 */
	string = 
         "This program brought to you by the council for better C programming";
	for(i=0; i < sizeof(write_buffer); i++) {
		write_buffer[i] = 0;
	}
	strncpy(write_buffer,string,sizeof(write_buffer));

	write_length = strlen(write_buffer);

	w = write(my_file_desc,write_buffer,write_length);

	if(w != write_length) {
		printf("write didn't complete, w: %d, length: %d\n",
			w,
			write_length);
		close(my_file_desc);
		exit(1);
	}

	close(my_file_desc);

	return(0);

}


