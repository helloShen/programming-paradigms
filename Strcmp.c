/*
 * one of two compare functions called by Isearch.c as a pointer to function
 */

#include <stdio.h>
#include <string.h>

/*
 * compare two strings
 * 2 arguments will be given in form of:
 * 		elem1 is char **
 * 		elem2 is char **
 */
int Strcmp(void *elem1, void *elem2) {
	char *vp1 = *(char **)elem1;
	char *vp2 = *(char **)elem2;
	return strcmp(vp1, vp2);
}
