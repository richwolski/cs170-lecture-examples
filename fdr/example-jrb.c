#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "jrb.h"

/*
 * example usage of Jim Plank's red-black tree
 * library
 */

int main(int argc, char **argv)
{
	JRB tree;
	JRB cursor;
	Jval payload;
	char *s;

	/*
	 * initialize an empty r-b tree
	 */
	tree = make_jrb();
	if(tree == NULL) {
		exit(1);
	}

	payload = new_jval_s("astring"); // create a Jval holding a string
	jrb_insert_int(tree,3,payload);  // insert it with key 3

	payload = new_jval_s("bstring"); // create a Jval holding another string
	jrb_insert_int(tree,1,payload);  // insert it with key 1
	
	payload = new_jval_s("cstring"); // create a Jval holding another string
	jrb_insert_int(tree,2,payload);      // insert it with key 2

	/*
	 * print the first element in the tree of elements sorted by their
	 * keys
	 */
	s = jval_s(jrb_val(jrb_first(tree)));
	printf("first element is %s\n",s);

	/*
	 * walk the tree and print out the values in order of their keys
	 */
	printf("forward traversal: ");
	jrb_traverse(cursor,tree) {
		s = jval_s(jrb_val(cursor));
		printf("%s ",s);
	}
	printf("\n");

	printf("reverse traversal: ");
	jrb_rtraverse(cursor,tree) {
		s = jval_s(jrb_val(cursor));
		printf("%s ",s);
	}
	printf("\n");

	/*
	 * delete the second node in sorted key order
	 */
	cursor = jrb_next(jrb_first(tree));
	jrb_delete_node(cursor);
	
	printf("forward traversal after delete: ");
	jrb_traverse(cursor,tree) {
		s = jval_s(jrb_val(cursor));
		printf("%s ",s);
	}
	printf("\n");

	/*
	 * test to see if the tree is empty
	 */
	if(jrb_empty(tree)) {
		printf("the tree is now empty\n");
	} else {
		printf("the tree is not empty\n");
	}

	/*
	 * search for a node with key 3
	 */
	cursor = jrb_find_int(tree,3);
	if(cursor != NULL) {
		printf("found string %s with key 3\n",
				jval_s(jrb_val(cursor)));
	} else {
		printf("could not find node with key 3\n");
	}

	/*
	 * search for a node with key 2
	 */
	cursor = jrb_find_int(tree,2);
	if(cursor != NULL) {
		printf("found string %s with key 2\n",
				jval_s(jrb_val(cursor)));
	} else {
		printf("could not find node with key 2\n");
	}

	/*
	 * now delete the two remaining nodes
	 */
	jrb_delete_node(jrb_first(tree));
	jrb_delete_node(jrb_first(tree));
	/*
	 * test to see if the tree is empty
	 */
	if(jrb_empty(tree)) {
		printf("the tree is now empty\n");
	} else {
		printf("the tree is not empty\n");
	}

	/*
	 * destroy the r-b tree
	 */
	jrb_free_tree(tree);

	exit(0);
}
