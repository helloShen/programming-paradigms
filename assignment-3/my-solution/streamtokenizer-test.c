#include "streamtokenizer.h"
#include <stdio.h>
#include <assert.h>

static void TestNextToken(void) {
	char *delimiter = "\t\n ";
	FILE *fp = fopen("streamtokenizer.c", "r");
	assert(fp);
	streamtokenizer st;
	STNew(&st, fp, delimiter, false);
	int bufferSize = 100;
	char buffer[bufferSize];
	while (STNextToken(&st, buffer, bufferSize)) {
		fprintf(stdout, "%s\n", buffer);
	}
	fflush(stdout);
	fclose(fp);
}

int main(void) {
	TestNextToken();
}
