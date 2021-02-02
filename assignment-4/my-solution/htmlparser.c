/**
 * Document: <https://github.com/google/gumbo-parser>
 *
 * INSTALL:
 * --------
 *   brew install libtool m4 autoconf automake aclocal
 *   ./autogen.sh
 *   ./configure
 *   make
 *   sudo make install
 * 
 * Check the flags needed to link my program:
 * -----------------------------------------
 *   pkg-config --cflags gumbo         # print compiler flags
 *   pkg-config --libs gumbo           # print linker flags
 */
#include "gumbo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * Extract clean text using Gumbo interface
 */
static void gumbo_clean_text(GumboNode *node, char *buffer, size_t bufferSize) {
	if (node->type == GUMBO_NODE_TEXT) {
		const char *text = node->v.text.text;
		strncat(buffer, text, strlen(text));
		strncat(buffer, "\n", 1);
	} else if (node->type == GUMBO_NODE_ELEMENT &&
			   node->v.element.tag != GUMBO_TAG_SCRIPT &&
			   node->v.element.tag != GUMBO_TAG_STYLE) {
		GumboVector *children = &node->v.element.children;
		for (unsigned int i = 0; i < children->length; ++i) {
			gumbo_clean_text((GumboNode *)children->data[i], buffer, bufferSize);
		}
	}
}

/**
 * A FILE oriented interface to CleanText()
 * ----------------------------------------
 * @src: original HTML file
 * @dest: storage of clean text
 */
void clean_test(FILE *src, FILE *dest) {
	assert(src);
	assert(dest);
	/* calculate source file size */
	size_t fileSize;
	fseek(src, 0L, SEEK_END);
	fileSize = ftell(src);
	rewind(src);
	/* read entire file into buffer */
	char *buffer = (char *)malloc(fileSize * sizeof(char));
	int done = fread(buffer, fileSize, 1L, src);
	assert(done == 1);
	/* call CleanText() to parse content */
	GumboOutput *output = gumbo_parse(buffer);
	char *cleantext = malloc(fileSize * sizeof(char));
	gumbo_clean_text(output->root, cleantext, fileSize);
	/* write into local file */
	fwrite(cleantext, 1, strlen(cleantext), dest);
	/* dispose sources */
	free(cleantext);
	free(buffer);
}