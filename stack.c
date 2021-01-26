/*
 * imitate stack class in c
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

/* contributor */
void stackNew(stack * s) {
	int max = 4;
	s->size = max; 
	s->ite = 0;
	s->elems = (int *)malloc(max * sizeof(int));
}

/* free space */
void stackDispose(stack * s) {
	free(s->elems);
}

/* push new element into the stack */
void stackPush(stack * s, int n) {
	if (s->ite == s->size) {
		s->size *= 2;
		int *elems = (int *)realloc(s->elems, s->size * sizeof(int));
		assert(s->elems != NULL); // leave some clues when error happends
	}
	s->elems[s->ite++] = n;
}

/* pop out the first element on the top of the stack */
int stackPop(stack *s) {
	if (s->ite > 0) {
		return s->elems[--s->ite];
	} else {
		printf("stack is empty!\n");
		return 0;
	}
}
