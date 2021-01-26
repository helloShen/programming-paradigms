/**
 * For any simple test
 */
#include <stdio.h>
#include <string.h>

static void TestGetc(void) {
	FILE *fp = fopen("./temptest.c", "r");
	char ch;
	while ((ch = getc(fp)) != EOF) {
		printf("%c", ch);
	}
	printf("\n");
	fclose(fp);
}

static void ungetstr(const char *str, FILE *stream) {
	for (int i = strlen(str) - 1; i >= 0; i--) {
		ungetc(*(str + i), stream);
	}
}

static const char STATRISK = '*';
static const char *POINTER = "[pointer]";
static void TestUngetc(void) {
	FILE *fp = fopen("./temptest.c", "r");
	char ch;
	while ((ch = getc(fp)) != EOF) {
		if (ch == STATRISK) {
			ungetstr(POINTER, fp);	
		} else {
			printf("%c", ch);
		}
	}
	printf("\n");
	fclose(fp);
}

int main(void) {
	TestGetc();
	TestUngetc();
}