/*
 * genetic version of stack.c
 */
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* constructor */
void new_stack(stack *s, const size_t size, const size_t elemSize, stack_freefn freefn) {
	s->size = size;
	s->ite = 0;
	s->elemSize = elemSize;
	s->elems = malloc(size * elemSize);
	s->freefn = freefn;
}

/* free memory */
void dispose_stack(stack *s) {
	if (s->freefn != NULL) {
		while (s->ite > 0) {
			void *elemAddr = (char *)s->elems + --s->ite * s->elemSize;
			s->freefn(elemAddr);
		}
	}
	free(s->elems);
}

/* push one new element on the top */
void push_stack(stack *s, const void *value, const size_t elemSize) {
	if (s->ite == s->size) {
		s->size *= 2;
		s->elems = realloc(s->elems, s->size * s->elemSize);
	}
	void *elemAddr = (char *)s->elems + s->elemSize * s->ite++;
	memcpy(elemAddr, value, s->elemSize);
}

/* pop our the element on the top */
void pop_stack(stack *s, void *res) {
	if (s->ite > 0) {
		void *elemAddr = (char *)s->elems + ((s->ite - 1) * s->elemSize);
		memcpy(res, elemAddr, s->elemSize);
		s->ite--;
	}
}

void clear_stack(stack *s) {
	if (s->freefn != NULL) {
		while (s->ite > 0) {
			void *elemAddr = (char *)s->elems + --s->ite * s->elemSize;
			s->freefn(elemAddr);
		}
	} else {
		s->ite = 0;
	}
}

size_t stack_size(stack *s) {
	return s->ite;
}

