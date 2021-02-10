#include "../tokenizer.c" // last resort for testing static function
#include "testutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/**
 * static int hashstr(const void *elemAddr, int numBuckets);
 * static int compstr(const void *elemAddr1, const void *elemAddr2);
 * static void freestr(void *elemAddr);
 */
static void test_str(void) {
	tstart("tokenizer::str");

	char *apple1 = "apple";
	char *apple2 = "apple";
	char *banana = "banana";
	int hash = hashstr(&apple1, 16);
	assert(hash >= 0 && hash < 16);
	assert(compstr(&apple1, &apple2) == 0);
	assert(compstr(&apple1, &banana) != 0);

	tpass();
}

/**
 * typedef struct {
 *     char *word;
 *	   int frequency;
 * } freq;
 * 
 * static void new_freq(freq *fq, const char *word, const int frequency);
 * static void freq_tostring(freq *fq, char *buffer);
 * static void dispose_freq(void *fq);
 * static int comp_freq(const void *fq1, const void *fq2);
 * static int hash_freq(const void *fq, const int numBuckets);
 */
static void test_freq(void) {
	tstart("tokenizer::freq");

	freq apple1;
	new_freq(&apple1, "apple", 3);
	freq apple2;
	new_freq(&apple2, "apple", 5);
	freq banana;
	new_freq(&banana, "banana", 1);
	freq kiwi;
	new_freq(&kiwi, "kiwi", 6);

	int hash = hash_freq(&apple1, 16);
	assert(hash >= 0 && hash <= 16);
	assert(comp_freq(&apple1, &apple2) == 0);
	assert(comp_freq(&apple1, &banana) != 0);

	char *str = freq_tostring(&apple1);
	assert(strlen(str) == 11);
	assert(strncmp(str, "[apple, 3]\n", 11) == 0);
	free(str);
	
	char *buffer = (char *)malloc(1);
	buffer[0] = '\0';
	freq_maptostring(&apple1, &buffer);
	freq_maptostring(&banana, &buffer);
	freq_maptostring(&kiwi, &buffer);
	assert(strlen(buffer) == 33); 
    assert(strcmp(buffer, "[apple, 3]\n[banana, 1]\n[kiwi, 6]\n") == 0);
	free(buffer);

	dispose_freq(&apple1);
	dispose_freq(&apple2);
	dispose_freq(&banana);
	dispose_freq(&kiwi);
	assert(apple1.word == NULL && apple2.word == NULL && 
		banana.word == NULL && kiwi.word == NULL);

	tpass();
}

/**
 * static void tokenize(vector *words, char *stream);
 */
static void test_tokenize(void) {	
	tstart("tokenizer::tokenize()");

	char *sentence = "Covid-19: Top adviser warns France at 'emergency' virus moment - BBC News\nPi = 3.14\n-1 is negative.";
	vector words;
	VectorNew(&words, sizeof(char *), freestr, 256);
	tokenize(&words, sentence);
	char *musthave[] = {"covid-19", "top", "3.14", "-1"};
	char *musthavenot[] = {"-", "1"};
	assert(VectorLength(&words) == 16);
	for (int i = 0; i < sizeof(musthave)/sizeof(char *); i++) {
		assert(VectorSearch(&words, &musthave[i], compstr, 0, false) != -1);
	}
	for (int i = 0; i < sizeof(musthavenot)/sizeof(char *); i++) {
		assert(VectorSearch(&words, &musthavenot[i], compstr, 0, false) == -1);
	}
	VectorDispose(&words);

	tpass();
}

/**
 * typedef struct {
 *     hashset *freqs; // hashset<freq>
 * } bag_of_words;
 *
 * void new_bagofwords(bag_of_words *bag);
 * void enter_bagofwords(bag_of_words *bag, const char *word);
 * void dispose_bagofwords(bag_of_words *bag);
 */
static void test_bagofwords(void) {
	tstart("tokenizer::bagofwords");
	/* test new_bagofwords() */
	bag_of_words bag;
	new_bagofwords(&bag);
	assert(&bag != NULL);

	/* test enter_bagofwords */
	char *words[] = {"apple", "is", "green", "apple", "is", "red"};
	for (int i = 0; i < sizeof(words)/sizeof(char *); i++) {
		enter_bagofwords(&bag, words[i]);
	}
	assert(HashSetCount(bag.freqs) == 4);
	freq freq_apple;
	new_freq(&freq_apple, "apple", 1);
	freq *apple_record = HashSetLookup(bag.freqs, &freq_apple);
	assert(apple_record != NULL);
	assert(strcmp(apple_record->word, "apple") == 0);
	assert(apple_record->frequency == 2);
	dispose_freq(&freq_apple);
	freq freq_green;
	new_freq(&freq_green, "green", 2);
	freq *green_record = HashSetLookup(bag.freqs, &freq_green);
	assert(green_record != NULL);
	assert(green_record->frequency == 1);
	dispose_freq(&freq_green);

	/* test	dispose_bagofwords() */
	dispose_bagofwords(&bag);
	assert(bag.freqs == NULL);

	tpass();
}

/**
 * hashset load_stopwords(void);
 * void dispose_stopwords(hashset *stopwords);
 */
static void test_stopwords(void) {
	tstart("tokenizer::load_stopwords()::dispose_stopwords()");

	hashset *stopwords = load_stopwords();
	assert(HashSetCount(stopwords) > 0);
	dispose_stopwords(stopwords);
	assert(stopwords->buckets == NULL);

	tpass();
}

/**
 * void to_bagofwords(bag_of_words *bag, char *buffer, hashset *stopwords);
 */
static void test_to_bagofwords(void) {
	tstart("tokenizer::to_bagofwords()");

	/* mock bag_of_words */
	bag_of_words bag;
	new_bagofwords(&bag);
	/* mock input stream */
	char *sentence = "apple is red. apple is green.";
	/* mock hashset of stopwords */
	hashset stopwords;
	HashSetNew(&stopwords, sizeof(char *), 1, hashstr, compstr, freestr);
	char *stopstr = (char *)malloc(3);
	strcpy(stopstr, "is");
	HashSetEnter(&stopwords, &stopstr);
	/* test with stopwords */
	to_bagofwords(&bag, sentence, &stopwords);
	char *wordapple = "apple";
	char *wordis = "is";
	freq *freqapple = (freq *)HashSetLookup(bag.freqs, &wordapple);
	assert(freqapple != NULL);
	assert(freqapple->frequency == 2);
	assert(HashSetLookup(bag.freqs, &wordis) == NULL);
	/* test without stopwords */
	dispose_bagofwords(&bag);
	new_bagofwords(&bag);
	to_bagofwords(&bag, sentence, NULL);
	freq *freqis = (freq *)HashSetLookup(bag.freqs, &wordis);
	assert(freqis != NULL);
	assert(freqis->frequency == 2);
	/* free resources */
	dispose_bagofwords(&bag);
	HashSetDispose(&stopwords);

	tpass();
}

int main(void) {
	test_str();
	test_freq();
	test_tokenize();
	test_bagofwords();
	test_stopwords();
	test_to_bagofwords();
}