#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int A;
int *global_p;

void foo(int *param_a)
{
	int *local_p;

	local_p = (int *)malloc(sizeof(int));
	global_p = local_p;

	printf("address of local variable from main(): %p\n",
			param_a);
	printf("address of the first parameter in foo(): %p\n",
			&param_a);
	printf("address of local variable in foo(): %p\n",
		&local_p);
	printf("address of variable on the heap: %p\n",local_p);
	printf("address of variable on the heap through global: %p\n",global_p);
	printf("address of global variable: %p\n",&A);
	if(local_p == global_p) {
		printf("local_p and global_p are equal\n");
	} else {
		printf("local_p and global_p are not equal\n");
	}

	free(local_p);
        return;
}

int main(int argc, char **argv)
{
	int main_a;

	foo(&main_a);
	return(0);
}


