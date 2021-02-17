#include "../curlconn.c"
#include "../article.c"
#include "../data.c"
#include "../rssparser.c"
#include "../htmlparser.c"
#include "../tokenizer.c"
#include "../index.c"
#include "../engine.c"
#include "testutils.h"

/**
 * static void freq_into_index(void *elemAddr, void *auxData);
 * static void bag_into_index(idx *i, const bag_of_words *bag);
 */
static void test_bag_into_index(void) {
	tstart("engine::freq_into_index()::bag_into_index()");

	bag_of_words bag1;
	bag_of_words bag2;
	new_bagofwords(&bag1, 1L);
	new_bagofwords(&bag2, 2L);
	for (int i = 0; i < 2; i++) enter_bagofwords(&bag1, "apple");
	for (int i = 0; i < 3; i++) enter_bagofwords(&bag1, "banana");
	for (int i = 0; i < 4; i++) enter_bagofwords(&bag2, "apple");
	for (int i = 0; i < 5; i++) enter_bagofwords(&bag2, "banana");

	idx i;
	new_index(&i);
	bag_into_index(&i, &bag1);
	bag_into_index(&i, &bag2);
	assert(HashSetCount(i.index) == 2);

	word_index apple_index;
	new_wordindex(&apple_index, "apple");
	word_index *apple_record = HashSetLookup(i.index, &apple_index);
	assert(apple_record != NULL);
	assert(HashSetCount(apple_record->freqs) == 2);
	char *apple_record_str = wordindex_tostring(apple_record);
	char *apple_answer = "apple:\t[docid=1, freq=2][docid=2, freq=4]";
	assert(strcmp(apple_record_str, apple_answer) == 0);

	word_index banana_index;
	new_wordindex(&banana_index, "banana");
	word_index *banana_record = HashSetLookup(i.index, &banana_index);
	assert(banana_record != NULL);
	assert(HashSetCount(banana_record->freqs) == 2);
	char *banana_record_str = wordindex_tostring(banana_record);
	char *banana_answer = "banana:\t[docid=1, freq=3][docid=2, freq=5]";
	assert(strcmp(banana_record_str, banana_answer) == 0);

	tpass();
}

/**
 * HashSetMapFunction<article>
 * Used to print articles.
 */
static void map_articles(void *elemAddr, void *auxData) {
	FILE *outfile = (FILE *)auxData;
	article *a = (article *)elemAddr;
	print_article(a, outfile);
}

/**
 * void build_index(idx *i, articles *as, const char *link);
 */
static void test_build_index(const char *link) {
	tstart("engine::build_index()");
	/* build index */
	idx i;
	new_index(&i);
	articles as;
	new_articles(&as);
	build_index(&i, &as, link);
	/* print result */
	print_index(&i, stdout);
	HashSetMap(as.table, map_articles, stdout);

	tpass();
}

static void test_search(const char *link, const char *query) {
	tstart("engine::search()");
	/* build index */
	idx i;
	new_index(&i);
	articles as;
	new_articles(&as);
	build_index(&i, &as, link);
	/* search and print result */
	search(&i, &as, query);

	tpass();
}

int main(void) {
	char *bbcrss = "http://feeds.bbci.co.uk/news/world/us_and_canada/rss.xml?edition=int";
	test_bag_into_index();
	test_build_index(bbcrss);
	test_search(bbcrss, "covid-19 virus");
}