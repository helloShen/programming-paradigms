/**
 * printArray() can print the numbers declareAndInitArray() just put into the stack.
 *
 * When declareAndInitArray() function is called,
 * it push 400 bytes data into the stack (SP is 
 * the Stack Pointer):
 * 		SP = SP + 100 * sizeof(int);
 *
 * When it returns, the stack pointer goes back:
 * 		SP = SP - 100 * sizeof(int);
 * But the datas in the stack are not erased.
 *
 * When printArray() function is called, it increase
 * the stack pointer by 404:
 * 		SP = SP + 100 * sizeof(int); 	//arr[100];
 * 		SP = SP + 1 * sizeof(int); 		//int i; 
 * So printArray() just happends to access the datas
 * declareAndInitArray() left in the stack.
 */

#include <stdio.h>

void declareAndInitArray(void);
void printArray(void);

int main(void) {
	declareAndInitArray();
	printArray();
}

void declareAndInitArray(void) {
	int arr[100];
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
	}
}

void printArray(void) {
	int arr[100];
	int i;
	for (i = 0; i < 100; i++) {
		printf("arr[%d] = %d\n", arr[i] = i, i);
	}
}

