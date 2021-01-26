/*
 * test stack 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gstack.h"

void testDouble(void);
void testString(void);
void freeStr(void *);

int main(void) {
	testDouble();
	testString();
}

/* put double into the stack */
void testDouble(void) {
	double nums[] = {4.0,5.4,66.22,74.0,23.23,44.098};
	gstack s;
	newGstack(&s, 8, NULL);
	for (int i = 0; i < 6; i++) {
		pushGstack(&s, nums + i, sizeof(double));
	}
	for (int i = 0; i < 6; i++) {
		double num = *(double *)(s.elems + i * 8);
		printf("nums[%d] = %lf\n", i, num);
	}
	double top;
	popGstack(&s, &top);
	printf("top element is %lf! (should be 44.098)\n", top);
	disposeGstack(&s);
}


/* put string into the stack */
void testString(void) {
	char *strs[] = {"Abby", "Lee", "David", "Black", "Tom", "Kim", "Tim"};
	gstack s;
	/* no need to write as "&freeStr", compiler know it's a pointer to the function */
	newGstack(&s, sizeof(char *), freeStr);
	for (int i = 0; i < 7; i++) {
		char *elemAddr = *(strs + i); 
		char *copy = strdup(elemAddr);
		printf("push %s into gstack\n", copy);
		/* put that pointer to the string into to stack */
		pushGstack(&s, &copy, sizeof(char *));
	}
	char *name;
	for (int i = 0; i < 3; i++) {
		popGstack(&s, &name);
		printf("pop name = %s\n", name);
		free(name);
	}
	disposeGstack(&s);
}

void freeStr(void * elemAddr) {
	printf("free string: %s\n", *(char **)elemAddr);
	free(*(char **)elemAddr);
}
