/*
 * generic version of stack.h
 */

typedef struct {
	int size; 		// number of element allowed
	int ite;		// point to the current last element
	int elemSize; 	// size of each element (how many bytes)
	void *elems;	// stockage of elements
	void (*freeElem)(void *); // free memory allocated for each element if neccessary
} gstack;

/* constructor */
void newGstack(gstack *s, int elemSize, void (*freeFunc)(void *));
/* free memory */
void disposeGstack(gstack *s);
/* push one new element on the top */
void pushGstack(gstack *s, void *value, int elemSize);
/* pop our the element on the top */
void popGstack(gstack *s, void *res);
