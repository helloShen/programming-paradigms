#include "../curlconn.c"
#include "testutils.h"
#include <assert.h>
#include <string.h>

static void test_dump_url() {
	tstart("curlconn::dump_url()");

	char *rss[] = { "https://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml", 
					"http://newsrss.bbc.co.uk/rss/newsonline_world_edition/front_page/rss.xml", 
					"https://www.seattletimes.com/feed/" };

	data *buffer;
	for (int i = 0; i < 3; i++) {
		buffer = dump_url(rss[i]);
		assert(strncmp(buffer->stream, "<?xml ", 5) == 0);
		dispose_data(buffer);
		free(buffer);
	}

	tpass();
}

int main(void) {
	test_dump_url();
}