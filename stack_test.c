/*
 * test stack 
 */

#include <stdio.h>
#include "stack.h"

int main(void) {
	int nums[] = {4,5,66,74,23,44};
	stack s;
	stackNew(&s);
	for (int i = 0; i < 6; i++) {
		stackPush(&s, nums[i]);
	}
	int top = stackPop(&s);
	printf("top element is %d! (should be 44)\n", top);
	stackDispose(&s);
}
