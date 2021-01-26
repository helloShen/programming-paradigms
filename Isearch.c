/*
 * Isearch serve to search a target string from a list of string.
 * Isearch() will call genetic_strcmp(), while the latter will call
 * stanadard built-in strcmp() function.
 */

#include <stdio.h>

/*
 * search key from a list of string (base) by using the comparation function
 * given as an argument.
 * 		n is the size of the base.
 * 		elemSize refers to how many bytes does datatype of key actually has.
 */
void *Isearch(void *key, void *base, 
			int n, int elemSize, 
			int (*cmp)(void *, void *)) {
	for (int i = 0; i < n; i++) {
		void *elemAddr = (char *)base + i * elemSize;
		if ((*cmp)(key, elemAddr) == 0) {
			return elemAddr;
		}
	}
	return NULL;
}
