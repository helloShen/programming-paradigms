/**
 * Interface of tokenizer.c that exposed to user.
 */
#include "hashset.h"
#include "bool.h"
#include <stdio.h>


#ifndef _BAG_OF_WORDS_
#define _BAG_OF_WORDS_

/**
 * hashset<freq>
 */
typedef struct {
	hashset freqs;
} bag_of_words;

void new_bagofwords(bag_of_words *bag);

void print_bagofwords(bag_of_words *bag, FILE *outfile);

void dispose_bagofwords(bag_of_words *bag);
#endif

hashset *init_stopwords(void);

void load_stopwords(hashset *stopwords, FILE *stopfile);

void print_stopwords(hashset *stopwords, FILE *outfile);

void dispose_stopwords(hashset *stopwords);

void to_bagofwords(bag_of_words *bag, FILE *infile, hashset *stopwords, bool usestopwords);

/**
 * Interfaces below are static function unit test entries.
 * Users are not supposed use them.
 */
void testtokenize(FILE *infile, FILE *outfile);
