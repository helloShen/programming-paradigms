/*
 * test the Isearch() function
 */

#include <stdio.h>
#include "Isearch.h"

void searchInt(void);
void searchStr(void);

int main(void) {
	//searchInt();
	searchStr();
}

void searchInt(void) {
	int key1 = 7;
	int base[] = {4, 6, 22, 12, 7, 10, 45};
	int *res = Isearch(&key1, base, 7, sizeof(int), Intcmp);
	if (res != NULL) {
		printf("find %d, pass test 1\n", *res);
	} else {
		printf("not pass test 1\n");
	}
	int key2 = 8;
	res = Isearch(&key2, base, 7, sizeof(int),	Intcmp);
	if (res == NULL) {
		printf("pass test 2\n");
	} else {
		printf("not pass test 2\n");
	}

}

void searchStr(void) {
	char *key1 = "Ab";
	char *base[] = {"Ab", "F#", "B", "6b", "D", "Eb"};
	char *res = Isearch(&key1, base, 6, sizeof(char *), Strcmp);
	if (res != NULL) {
		printf("find %c, pass test 1\n", *res);
	} else {
		printf("not pass test 1\n");
	}
	char *key2 = "K";
	res = Isearch(&key2, base, 6, sizeof(char *), Strcmp);
	if (res == NULL) {
		printf("pass test 2\n");
	} else {
		printf("not pass test 2\n");
	}
}

