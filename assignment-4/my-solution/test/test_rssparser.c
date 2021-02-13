#include "../rssparser.c"
#include "testutils.h"

static void test_parserss(void) {
	tstart("rssparser::parserrss()");

	char *stream = t_read_whole_file("data/test-rss.xml");
	data d;
	new_data(&d);
	append_data(&d, stream, strlen(stream));
	vector articles;
	VectorNew(&articles, sizeof(article), dispose_article, 128);	
	parserss(&articles, &d);
	for (int i = 0; i < articles.elemNum; i++) {
		article *a = VectorNth(&articles, i);
		for (int j = 0; j < articles.elemNum; j++) {
			if (j != i) {
				article *b = VectorNth(&articles, j);
				assert(strcmp(a->title, b->title) != 0);
				assert(strcmp(a->link, b->link) != 0);
			}
		}
	}

	VectorDispose(&articles);

	tpass();
}

int main(void) {
	test_parserss();
}
