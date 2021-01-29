#include "../tokenizer.h"
// #include "../tokenizer.c" // last resort for testing static function
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * To test static function, we have to call local test function in "tokenizer.c"
 */
static void call_testtokenize(void) {
	FILE *infile = fopen("data/clean-bbc-news.txt", "r");
	FILE *outfile = fopen("data/bbc-news-words-vector.txt", "w");
	assert(infile);
	assert(outfile);
	testtokenize(infile, outfile);
	fclose(infile);
	fclose(outfile);
}

static void test_to_bagofwords_no_stopwords(void) {
	FILE *infile = fopen("data/clean-bbc-news.txt", "r");
	FILE *outfile = fopen("data/bbc-bag-of-words.txt", "w");
	assert(infile);
	assert(outfile);
	bag_of_words bag;
	new_bagofwords(&bag);
	to_bagofwords(&bag, infile, NULL, false);
	print_bagofwords(&bag, outfile);
	dispose_bagofwords(&bag);
	fclose(infile);
	fclose(outfile);
}

static void test_to_bagofwords_with_stopwords(void) {
	FILE *infile = fopen("data/clean-bbc-news.txt", "r");
	FILE *outfile = fopen("data/bbc-bag-of-words-with-stopwords.txt", "w");
	FILE *stopfile = fopen("../data/stop-words.txt", "r");
	assert(infile);
	assert(outfile);
	assert(stopfile);
	bag_of_words bag;
	new_bagofwords(&bag);
	hashset *stopwords = init_stopwords();
	load_stopwords(stopwords, stopfile);
	assert(stopwords != NULL);
	// print_stopwords(stopwords, stdout);
	to_bagofwords(&bag, infile, stopwords, true);
	print_bagofwords(&bag, outfile);
	dispose_bagofwords(&bag);
	dispose_stopwords(stopwords);
	fclose(infile);
	fclose(outfile);
}

int main(void) {
	call_testtokenize();
	test_to_bagofwords_no_stopwords();
	test_to_bagofwords_with_stopwords();
}