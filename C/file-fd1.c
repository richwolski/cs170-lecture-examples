#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/*
 * print out some file descriptors
 */
int main(int argc, char **argv)
{
	int my_file_desc;
	char file_name[4096];
	int i;
	int r;
	char *string;

	if(argc < 2) {
		printf("need to specify a file name\n");
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
	 * try and open the file for reading
	 */
	my_file_desc = open(file_name,O_RDONLY,0);
	if(my_file_desc < 0) {
		printf("failed to open %s for reading\n",file_name);
		exit(1);
	}

	printf("my_file_desc: %d\n",my_file_desc);
	
	string = "a string written to standard out\n";
	write(1,string,strlen(string));

	close(my_file_desc);

	return(0);

}


