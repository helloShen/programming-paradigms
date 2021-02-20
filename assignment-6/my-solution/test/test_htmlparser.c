#include "../htmlparser.c"
#include "testutils.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>


static void test_htmlparser(void) {
	tstart("htmlparser::clean_text()");

	char *stream = t_read_whole_file("data/test-webpage.html");
	data raw;
	new_data(&raw);
	append_data(&raw, stream, strlen(stream));
	data clean;
	new_data(&clean);
	clean_text(&raw, &clean);
	assert(strlen(clean.stream) > 0);

	tpass();
}

int main(void) {
	test_htmlparser();
}