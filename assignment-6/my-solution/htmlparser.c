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
#include "data.h"
#include <string.h>

/**
 * Extract clean text using Gumbo interface
 * Return string in buffer are always null-terminated.
 */
static const char kHtmlParserChangeLine = '\n';
static void gumbo_clean_text(GumboNode *node, data *buff) {
	if (node->type == GUMBO_NODE_TEXT) {
		const char *text = node->v.text.text;
		append_data(buff, text, strlen(text));
		append_data(buff, &kHtmlParserChangeLine, 1);
	} else if (node->type == GUMBO_NODE_ELEMENT &&
			   node->v.element.tag != GUMBO_TAG_SCRIPT &&
			   node->v.element.tag != GUMBO_TAG_STYLE) {
		GumboVector *children = &node->v.element.children;
		for (unsigned int i = 0; i < children->length; ++i) {
			gumbo_clean_text((GumboNode *)children->data[i], buff);
		}
	}
}

/**
 * A in-memory version.
 * ----------------------------------------
 * Both data->stream are null-terminated.
 */
void clean_text(data *raw, data *clean) {
	if (raw != NULL) {
		/* call CleanText() to parse content */
		GumboOutput *output = gumbo_parse(raw->stream);
		gumbo_clean_text(output->root, clean);
	}
}