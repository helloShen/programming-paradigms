#include <ctype.h>
/*
 * generic version of stack.h
 */

typedef void (*stack_freefn)(void *elemAddr);

typedef struct {
	size_t size; 				// number of element allowed
	int ite;					// point to the current last element
	size_t elemSize; 			// size of each element (how many bytes)
	void *elems;				// stockage of elements
	stack_freefn freefn; 		// free memory allocated for each element if necessary
} stack;

/* constructor */
void new_stack(stack *s, const size_t size, const size_t elemSize, stack_freefn);
/* free memory */
void dispose_stack(stack *s);
/* push one new element on the top */
void push_stack(stack *s, const void *value, const size_t elemSize);
/* pop our the element on the top */
void pop_stack(stack *s, void *res);
/* clear all elements in stack, for the purpose of reuse. */
void clear_stack(stack *s);
/* return current size of stack */
size_t stack_size(stack *s);
