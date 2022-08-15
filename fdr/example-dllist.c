#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "dllist.h"

/*
 * example usage of Jim Plank's doubly linked list
 * library
 */

int main(int argc, char **argv)
{
	Dllist list;
	Dllist cursor;
	Jval payload;
	char *s;

	/*
	 * initialize an empty double linked list
	 */
	list = new_dllist();
	if(list == NULL) {
		exit(1);
	}

	payload = new_jval_s("first"); // create a Jval holding a string
	dll_append(list,payload); // add the string to the list

	payload = new_jval_s("second"); // create a Jval holding another string
	dll_append(list,payload); // add the string to the list
	
	payload = new_jval_s("third"); // create a Jval holding another string
	dll_append(list,payload); // add the string to the list

	/*
	 * print the first element -- dll_first returns a Dllist node.  Need
	 * to extract the string
	 */
	s = jval_s(dll_val(dll_first(list)));
	printf("first element is %s\n",s);

	

	/*
	 * walk the list and print out the values
	 */
	printf("forward traversal: ");
	dll_traverse(cursor,list) {
		s = jval_s(dll_val(cursor));
		printf("%s ",s);
	}
	printf("\n");

	printf("reverse traversal: ");
	dll_rtraverse(cursor,list) {
		s = jval_s(dll_val(cursor));
		printf("%s ",s);
	}
	printf("\n");

	/*
	 * delete the middle node
	 */
	cursor = dll_next(dll_first(list));
	dll_delete_node(cursor);
	
	printf("forward traversal after delete: ");
	dll_traverse(cursor,list) {
		s = jval_s(dll_val(cursor));
		printf("%s ",s);
	}
	printf("\n");
}
