#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

/*
 * seek the file in the first argument to the location specified in the
 * second
 */
int main(int argc, char **argv)
{
	int my_file_desc;
	char file_name[4096];
	char read_buffer[4096];
	int i;
	int r;
	off_t offset;
	off_t where;

	if(argc < 3) {
		printf("need to specify a file name and an offset\n");
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
	 * get the offset from the second argument
	 */
	offset = (off_t)atoi(argv[2]);

	/*
	 * try and open the file for reading
	 */
	my_file_desc = open(file_name,O_RDONLY,0);
	if(my_file_desc < 0) {
		printf("failed to open %s for reading\n",file_name);
		exit(1);
	}

	/*
	 * seek to the offset specified in the second argument
	 */
	where = lseek(my_file_desc,offset,SEEK_SET);

	if(where < 0) {
		printf("lseek to %d in file %s failed\n",
			(int)offset,
			file_name);
		close(my_file_desc);
		exit(1);
	}

	for(i=0; i < sizeof(read_buffer); i++) {
		read_buffer[i] = 0;
	}
	r = read(my_file_desc,read_buffer,sizeof(read_buffer)-1);

	/*
	 * read and print until EOF
	 */
	while(r > 0) {
		printf("%s",read_buffer);
		for(i=0; i < sizeof(read_buffer); i++) {
			read_buffer[i] = 0;
		}
		r = read(my_file_desc,read_buffer,sizeof(read_buffer)-1);
	}
	close(my_file_desc);

	return(0);

}


