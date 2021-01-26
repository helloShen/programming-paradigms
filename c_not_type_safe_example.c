/**
 * here are two good example of why c is not a 
 * type-safe programming language
 */

#include <stdio.h>

void no_parameters_check(char * str, int num);
void no_array_boundary_check(void);
/**
 * just compile that I have a function like that
 * in my source code.
 */
int strlen(char *str, int num);

int main(void) {
	no_parameters_check("strlen=8", 100);
	no_array_boundary_check();
}

/**
 * correct prototype of 'strlen' in library is:
 * 		unsigned long strlen(const char *);
 *
 * only pop 1 warning: incompatible redeclaration 
 * of library function 'strlen' 
 * 
 * compiler will link the only 'strlen' function
 * in library any way, no matter what parameters
 * are given here.
 *
 * strlen here just deels with the first parameter.
 */
void no_parameters_check(char * str, int num) {
	printf("string length = %d\n", strlen(str, num));
}

/**
 * arr[4] just looks for data at: 
 * 		&arr + 4 * sizeof(int) 
 *
 * no error or warning popped
 * no array boundary check at all
 */
void no_array_boundary_check(void) {
	int i;
	int arr[4];
	for (i = 0; i <=4; i++) { //arr boundary exceeded 
		arr[i] = 0;
		printf("arr[%d] = %d\n", i, arr[i]);
	}
}
