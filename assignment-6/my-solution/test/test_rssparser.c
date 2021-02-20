#include "../rssparser.c"
#include "testutils.h"


static const char *kTitle1 = "Biden calls for gun law reform on anniversary of Parkland shooting";
static const char *kLink1 = "https://www.bbc.co.uk/news/world-us-canada-56064770";
static const char *kDescription1 = "The US president asks Congress to pass new laws, including a ban on assault weapons.";
static const char *kTitle2 = "New Brunswick outbreak: How a smalltown doctor became a Covid pariah";
static const char *kLink2 = "https://www.bbc.co.uk/news/world-us-canada-54686672";
static const char *kDescription2 = "After being labelled \"patient zero\", a small-town physician in Canada was shunned.";
	
static void test_parserss(void) {
	tstart("rssparser::parserrss()");

	char *stream = t_read_whole_file("data/test-rss.xml");
	data d;
	new_data(&d);
	append_data(&d, stream, strlen(stream));
	vector articles;
	VectorNew(&articles, sizeof(article), dispose_article, 128);	
	parserss(&articles, &d);

	int pass = 0;
	for (int i = 0; i < VectorLength(&articles); i++) {
		article *a = VectorNth(&articles, i);
		if (strcmp(a->title, kTitle1) == 0 &&
			strcmp(a->link, kLink1) == 0 &&
			strcmp(a->description, kDescription1) == 0) pass++;
		if (strcmp(a->title, kTitle2) == 0 &&
			strcmp(a->link, kLink2) == 0 &&
			strcmp(a->description, kDescription2) == 0) pass++;
		for (int j = 0; j < articles.elemNum; j++) {
			if (j != i) {
				article *b = VectorNth(&articles, j);
				assert(strcmp(a->title, b->title) != 0);
				assert(strcmp(a->link, b->link) != 0);
			}
		}
	}
	assert(pass == 2);

	VectorDispose(&articles);

	tpass();
}

static void map_check(void *elemAddr, void *auxData) {
	article *a = (article *)elemAddr;
	int *pass = (int *)auxData;
	if (strcmp(a->title, kTitle1) == 0 &&
		strcmp(a->link, kLink1) == 0 &&
		strcmp(a->description, kDescription1) == 0) (*pass)++;
	if (strcmp(a->title, kTitle2) == 0 &&
		strcmp(a->link, kLink2) == 0 &&
		strcmp(a->description, kDescription2) == 0) (*pass)++;
}

static void test_parserss_with_articles(void) {
	tstart("rssparser::parserrss()");

	char *stream = t_read_whole_file("data/test-rss.xml");
	data d;
	new_data(&d);
	append_data(&d, stream, strlen(stream));
	vector v_as;
	VectorNew(&v_as, sizeof(article), dispose_article, 128);	
	parserss(&v_as, &d);

	articles h_as;
	new_articles(&h_as);
	for (int i = 0; i < VectorLength(&v_as); i++) {
		article *a = VectorNth(&v_as, i);
		add_article(&h_as, a);
	}

	int pass = 0;
	HashSetMap(h_as.table, map_check, &pass);
	assert(pass == 2);

	dispose_articles(&h_as);

	tpass();
}

int main(void) {
	test_parserss();
	test_parserss_with_articles();
}
