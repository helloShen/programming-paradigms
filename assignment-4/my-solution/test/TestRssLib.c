/**
 * Test handout static library lib/librssnews.a
 */
#include "streamtokenizer.h"
#include <stdio.h>

static const char *const DELIMITERS = " \t\n\r\b!@$%^*()_+={[}]|\\'\":;/?.>,<~`";

/**
 * Lecture give away library: librssnews.a is for linux not MACOS(darwin)
 * ----------------------------------------------------------------------
 * Here's the compile error:
 *	   gcc -g -Wall -o testlib TestRssLib.o 
 *	  		-lcurl 
 *	  		-L/Users/Wei/github/letsplayc/src/programming-paradigms/assignment-4/my-solution/lib 
 *	  		-lrssnews
 *
 *	   ld: warning: ignoring file 
 *	   	/Users/Wei/github/letsplayc/src/programming-paradigms/assignment-4/my-solution/lib/librssnews.a, 
 *	   	building for macOS-x86_64 but attempting to link with file built for unknown-unsupported file format 
 *	  	( 0x21 0x3C 0x61 0x72 0x63 0x68 0x3E 0x0A 0x2F 0x20 0x20 0x20 0x20 0x20 0x20 0x20 )
 */
static void TestStreamtokenizer(void) {
	FILE *fp = fopen("TestLib.c", "r");
	streamtokenizer st;
	STNew(&st, fp, DELIMITERS, false);
	int bufferLen = 1024;
	char buffer[bufferLen];
	while(STNextToken(&st, buffer, bufferLen)) {
		fprintf(stdout, "%s\n", buffer);
	}
}

int main(void) {
	TestStreamtokenizer();
}