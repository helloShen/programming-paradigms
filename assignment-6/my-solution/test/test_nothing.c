#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	char *str1 = (char *)malloc(20);
	strncpy(str1, "hello ", 6);
	str1[6] = '\0';
	char *str2 = (char *)malloc(20);
	strncpy(str2, "world!", 6);
	str2[6] = '\0';

	char *str3 = (char *)malloc(20);
	strncat(str3, str1, 6);
	strncat(str3 + 6, str2, 6);
	str3[12] = '\0';

	free(str1);
	free(str2);
	printf("str3 = %s\n", str3);
	free(str3);
}