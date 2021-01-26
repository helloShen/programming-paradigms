/*
 * compare value of two integer
 */

#include <stdio.h>

int Intcmp(void *elem1, void *elem2) {
	int *n1 = elem1;
	int *n2 = elem2;
	return *n1 - *n2;
}
