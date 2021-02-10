/**
 * Interface of tokenizer.c that exposed to user.
 */
#include "vector.h"
#include "hashset.h"


#ifndef _BAG_OF_WORDS_
#define _BAG_OF_WORDS_

#include <stdio.h>
/**
 * hashset<freq>
 */
typedef struct {
	hashset *freqs;
} bag_of_words;

void new_bagofwords(bag_of_words *bag);

void print_bagofwords(bag_of_words *bag, FILE *outfile);

void dispose_bagofwords(bag_of_words *bag);
#endif

/**
 * Return a hashset<char *>, each element is a stopword.
 * Use dispose_stopwords() to free the memory after using it.
 */
hashset *load_stopwords(void);

void dispose_stopwords(hashset *stopwords);

/**
 * It tokenize the input string into a list of words, then count the
 * words frequencies.
 * If stopwords hashset is NULL, it counts words frequencies without 
 * stopwords.
 */
void to_bagofwords(bag_of_words *bag, char *stream, hashset *stopwords);

/**
 * Allow unit test to test static tokenize() function
 */
void test_tokenize_helper(vector *words, char *stream);
