#include "../index.c"
#include "testutils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


/**
 * void new_docfreq(doc_freq *df, long docid, int freq);
 * int hash_docfreq(const void *elemAddr, int numBuckets);
 * int comp_docfreq(const void *elemAddr1, const void *elemAddr2);
 * char *docfreq_tostring(doc_freq *df);
 */
static void test_docfreq(void) {
	tstart("index::new_docfreq()::hash_docfreq()::comp_docfreq()::docfreq_tostring()");

	doc_freq dq_764000_1;
	new_docfreq(&dq_764000_1, 1611742826915764000L, 5);
	assert(dq_764000_1.docid > 0L);
	assert(dq_764000_1.freq >= 0);

	int hash = hash_docfreq(&dq_764000_1, 16);
	assert(hash >= 0 && hash < 16);
	assert(strcmp(docfreq_tostring(&dq_764000_1), "[docid=1611742826915764000, freq=5]") == 0);

	doc_freq dq_764000_2;
	new_docfreq(&dq_764000_2, 1611742826915764000L, 2);
	doc_freq dq_538000;
	new_docfreq(&dq_538000, 1611742826915538000L, 3);
	assert(comp_docfreq(&dq_764000_1, &dq_764000_2) == 0);
	assert(comp_docfreq(&dq_764000_1, &dq_538000) != 0);

	tpass();
}

/**
 * apple:	[1611742826915764000, 5][1611742826915538000, 10]
 * Caller need to free the apple object.
 */
static word_index *apple_index(void) {
	word_index *apple = (word_index *)malloc(sizeof(word_index));
	new_wordindex(apple, "apple");
	enter_wordindex(apple, 1611742826915764000L, 5);
	enter_wordindex(apple, 1611742826915538000L, 10);
	return apple;
}

/**
 * banana:	[1611742826915764000, 3][1611742826916710000, 6]
 * Caller need to free the banana object.
 */
static word_index *banana_index(void) {
	word_index *banana = (word_index *)malloc(sizeof(word_index));
	new_wordindex(banana, "banana");
	enter_wordindex(banana, 1611742826915764000L, 3);
	enter_wordindex(banana, 1611742826916710000L, 6);
	return banana;
}

/**
 * static void new_wordindex(word_index *wi, const char *word);
 * void enter_wordindex(word_index *wi, const long docid, const int frequency);
 * static int hash_wordindex(const void *elemAddr, int numBuckets);
 * static char *wordindex_tostring(word_index *wi);
 * static void dispose_wordindex(void *elemAddr);
 */
static void test_wordindex(void) {
	tstart("index::new_wordindex()::enter_wordindex()::hash_wordindex()::wordindex_tostring()::dispose_wordindex()");

	word_index *apple = apple_index();
	assert(strcmp(apple->word, "apple") == 0);
	assert(HashSetCount(apple->freqs) == 2);

	doc_freq df_764000;
	new_docfreq(&df_764000, 1611742826915764000L, 1);
	doc_freq df_538000;
	new_docfreq(&df_538000, 1611742826915538000L, 1);
	assert(HashSetLookup(apple->freqs, &df_764000) != NULL);
	assert(HashSetLookup(apple->freqs, &df_538000) != NULL);

	int hash = hash_wordindex(apple, 16);
	assert(hash >= 0 && hash < 16);

	char *str = wordindex_tostring(apple);
	assert(strcmp(str, "apple:	[docid=1611742826915538000, freq=10][docid=1611742826915764000, freq=5]") == 0);

	dispose_wordindex(apple);
	assert(apple->word == NULL);
	assert(apple->freqs == NULL);
	free(apple);

	tpass();
}


/**
 * static int comp_wordindex(const void *elemAddr1, const void *elemAddr2);
 */
static void test_comp_wordindex(void) {
	tstart("index::comp_wordindex()");

	word_index *apple1 = apple_index();
	word_index *apple2 = apple_index();
	word_index *banana = banana_index();

	assert(comp_wordindex(apple1, apple2) == 0);
	assert(comp_wordindex(apple1, banana) != 0);

	dispose_wordindex(apple1);
	dispose_wordindex(apple2);
	dispose_wordindex(banana);
	free(apple1);
	free(apple2);
	free(banana);

	tpass();
}

/**
 * 
 * void merge_wordindex(word_index *wi1, const word_index *wi2);
 */
static void test_merge_wordindex(void) {
	tstart("index::merge_wordindex()");

	word_index empty;
	new_wordindex(&empty, "");
	word_index *apple = apple_index();
	word_index *banana = banana_index();
	merge_wordindex(&empty, apple);
	char *str = wordindex_tostring(&empty);
	char *answer1 = "apple:\t[docid=1611742826915538000, freq=10][docid=1611742826915764000, freq=5]";
	assert(strcmp(str, answer1) == 0);
	merge_wordindex(apple, banana);
	str = wordindex_tostring(apple);
	char *answer2 = "apple banana:\t[docid=1611742826915538000, freq=10][docid=1611742826915764000, freq=8][docid=1611742826916710000, freq=6]";
	assert(strcmp(str, answer2) == 0);
	dispose_wordindex(&empty);
	dispose_wordindex(apple);
	dispose_wordindex(banana);
	free(apple);
	free(banana);

	tpass();
}

/**
 * static doc_freq *search_in_wordindex(const word_index *wi, const long docid);
 */
static void test_search_in_wordindex(void) {
	tstart("index::search_in_wordindex()");

	word_index *apple = apple_index();
	doc_freq *df_764000 = search_in_wordindex(apple, 1611742826915764000);
	assert(df_764000 != NULL);
	assert(df_764000->freq == 5);
	doc_freq *df_763000 = search_in_wordindex(apple, 1611742826915763000);
	assert(df_763000 == NULL);

	dispose_wordindex(apple);
	free(apple);

	tpass();
}

/**
 * void new_index(idx *i);
 * void dispose_index(idx *i);
 */
static void test_idx(void) {
	tstart("index::new_index()::dispose_index()");

	idx *i = (idx *)malloc(sizeof(idx));
	new_index(i);
	assert(i->index != NULL);
	assert(i->index->buckets != NULL);
	assert(i->index->bucketNum == kIndexHashSetBuckets);

	dispose_index(i);
	assert(i->index == NULL);
	free(i);

	tpass();
}

/**
 * void enter_index(idx *i, const long docid, const char *word, const int freq);
 */
static void test_enter_index(void) {
	tstart("index:enter_index()");

	idx i;
	new_index(&i);
	enter_index(&i, 1611742826915764000, "apple", 5);
	assert(HashSetCount(i.index) == 1);
	word_index apple_wi;
	new_wordindex(&apple_wi, "apple");
	word_index *wi_record = (word_index *)HashSetLookup(i.index, &apple_wi);
	assert(wi_record != NULL);
	assert(HashSetCount(wi_record->freqs) == 1);
	doc_freq apple_df;
	new_docfreq(&apple_df, 1611742826915764000, 0);
	doc_freq *df_record = (doc_freq *)HashSetLookup(wi_record->freqs, &apple_df);
	assert(df_record != NULL);
	assert(df_record->docid == 1611742826915764000);
	assert(df_record->freq == 5);

	enter_index(&i, 1611742826915538000, "apple", 7);
	assert(HashSetCount(i.index) == 1);
	assert(HashSetCount(wi_record->freqs) == 2);
	doc_freq apple_df2;
	new_docfreq(&apple_df2, 1611742826915538000, 0);
	df_record = (doc_freq *)HashSetLookup(wi_record->freqs, &apple_df2);
	assert(df_record != NULL);
	assert(df_record->docid == 1611742826915538000);
	assert(df_record->freq == 7);

	enter_index(&i, 1611742826915764000, "apple", 10);
	assert(HashSetCount(i.index) == 1);
	assert(HashSetCount(wi_record->freqs) == 2);
	df_record = (doc_freq *)HashSetLookup(wi_record->freqs, &apple_df);
	assert(df_record->docid == 1611742826915764000);
	assert(df_record->freq == 10);

	enter_index(&i, 1611742826915764000, "banana", 3);
	assert(HashSetCount(i.index) == 2);
	word_index banana_wi;
	new_wordindex(&banana_wi, "banana");
	wi_record = (word_index *)HashSetLookup(i.index, &banana_wi);
	assert(wi_record != NULL);
	assert(HashSetCount(wi_record->freqs) == 1);
	doc_freq banana_df;
	new_docfreq(&banana_df, 1611742826915764000, 0);
	df_record = (doc_freq *)HashSetLookup(wi_record->freqs, &banana_df);
	assert(df_record != NULL);
	assert(df_record->docid == 1611742826915764000);
	assert(df_record->freq == 3);

	dispose_index(&i);
	dispose_wordindex(&apple_wi);
	dispose_wordindex(&banana_wi);

	tpass();
}

/**
 * word_index *search_index(const idx *i, const char *word);
 */
static void test_search_in_index(void) {
	tstart("index::search_word_in_index()");

	idx i;
	new_index(&i);
	vector query;
	VectorNew(&query, sizeof(char *), NULL, 4);
	char *apple = "apple";
	char *banana = "banana";
	VectorAppend(&query, &apple);
	VectorAppend(&query, &banana);
	word_index *result = search_in_index(&i, &query);
	assert(result != NULL);
	assert(strcmp(result->word, "") == 0);
	assert(HashSetCount(result->freqs) == 0);
	enter_index(&i, 1611742826915764000, "apple", 5);
	enter_index(&i, 1611742826915538000, "banana", 10);
	result = search_in_index(&i, &query);
	char *result_str = wordindex_tostring(result);
	char *answer = "apple banana:\t[docid=1611742826915538000, freq=10][docid=1611742826915764000, freq=5]";
	assert(strcmp(result_str, answer) == 0);
	dispose_index(&i);
	VectorDispose(&query);

	tpass();
}

int main(void) {
	test_docfreq();
	test_wordindex();
	test_comp_wordindex();
	test_search_in_wordindex();
	test_merge_wordindex();
	test_idx();
	test_enter_index();
	test_search_in_index();
}

