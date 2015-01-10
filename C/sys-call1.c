#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * call getpid() as an example system call
 */

int main(int argc, char **argv)
{
	pid_t my_id;

	my_id = getpid();
	printf("my process id is %ld\n",(long)my_id);

        return(0);
}


