#include "../htmlparser.h"
#include <stdio.h>
#include <assert.h>

/**
 * test clean_text() function
 */
static void test_htmlparser() {	
	char *src = "data/bbc-news.txt";
	char *dest = "data/clean-bbc-news.txt";
	FILE *raw = fopen(src, "r");
	FILE *clean = fopen(dest, "w");
	assert(raw);
	assert(clean);
	printf("Extracting clean text from %s ...\n", src);
	clean_test(raw, clean);
	printf("[DONE] Check -> %s\n", dest);
	fclose(raw);
	fclose(clean);
}

int main(void) {
	test_htmlparser();
}