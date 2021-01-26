/*
 * genetic version of stack.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gstack.h"

/* constructor */
void newGstack(gstack *s, int elemSize, void (*freeFunc)(void *)) {
	int max = 4;
	s->size = max;
	s->ite = 0;
	s->elemSize = elemSize;
	s->elems = malloc(max * elemSize);
	s->freeElem = *freeFunc;
}

/* free memory */
void disposeGstack(gstack *s) {
	if (s->freeElem != NULL) {
		while (s->ite > 0) {
			void *elemAddr = (char *)s->elems + --s->ite * s->elemSize;
			//printf("to free address = %u\n", elemAddr);
			s->freeElem(elemAddr);
		}
	}
	free(s->elems);
}

/* push one new element on the top */
void pushGstack(gstack *s, void *value, int elemSize) {
	if (s->ite == s->size) {
		s->size *= 2;
		s->elems = realloc(s->elems, s->size * s->elemSize);
	}
	void *elemAddr = (char *)s->elems + s->elemSize * s->ite++;
	memcpy(elemAddr, value, s->elemSize);
}

/* pop our the element on the top */
void popGstack(gstack *s, void *res) {
	if (s->ite > 0) {
		void *elemAddr = (char *)s->elems + ((s->ite - 1) * s->elemSize);
		memcpy(res, elemAddr, s->elemSize);
	} else {
		printf("gstack is empty!\n");
	}
	s->ite--;
}

