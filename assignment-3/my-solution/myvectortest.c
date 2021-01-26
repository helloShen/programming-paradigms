/**
 * vector unit test
 */

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"


/**
 * Insert a string into a char vector, char by char.
 */
static void MyInsertCharByChar(vector *v, char * str) {
	int i = 0;
	char c;
	while ((c = *(str + i)) != '\0') {
		VectorAppend(v, &c);
		printf("put[%c] ", c);
		i++;
	}
	printf("\n");
}

/**
 * Given a vector of char, print elements one by one.
 */
static void MyPrintCharByChar(vector *v) {
	for (int i = 0; i < VectorLength(v); i++) {
		printf("get[%c] ", *((char *)VectorNth(v, i)));
	}
	printf("\n");
}

static void MySimpleTest(void) {
	printf("------------------------ Starting the basic test...\n");
	vector alphabet;
	VectorNew(&alphabet, sizeof(char), NULL, 4);
	MyInsertCharByChar(&alphabet, "Hello Vecor!");
	MyPrintCharByChar(&alphabet);
	VectorDispose(&alphabet);
}

int main(void) {
	MySimpleTest();
}
