// #include "../tokenizer.h"
#include "../tokenizer.c" // last resort for testing static function
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void freestr(void *elemAddr) {
	free(*(char **)elemAddr);
}

static void printstr(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	fprintf(out, "[%s]\n", *(char **)elemAddr);
	fflush(out);
}

static void testtokenize(void) {
	FILE *in = fopen("data/clean-bbc-news.txt", "r");
	FILE *out = fopen("data/bbc-news-words-vector.txt", "w");
	assert(in);
	assert(out);
	char line[2048];
	vector words;
	VectorNew(&words, sizeof(char *), freestr, 1024);
	while (fgets(line, 2048, in) != NULL) {
		tokenize(&words, line);
	}
	VectorMap(&words, printstr, out);
	VectorDispose(&words);
	fclose(in);
	fclose(out);
}

int main(void) {
	testtokenize();
}