#include "../curlconn.c"
#include "testutils.h"
#include <assert.h>
#include <string.h>

static void test_dump_url() {
	tstart("curlconn::dump_url()");

	char *link = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
	data buffer;
	new_data(&buffer);
	dump_url(link, &buffer);
	assert(strlen(buffer.stream) > 0);

	tpass();
}

int main(void) {
	test_dump_url();
}