/*
 * imitate a stack class in c
 */

typedef struct {
	int *elems;
	int ite;
	int size;
} stack;

void stackNew(stack *s);
void stackDispose(stack *s);
void stackPush(stack *s, int n);
int stackPop(stack *s);

