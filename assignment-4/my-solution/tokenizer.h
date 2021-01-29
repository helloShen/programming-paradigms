/**
 * Interface of tokenizer.c that exposed to user.
 */
#include "hashset.h"
#include <stdio.h>


#ifndef _BAG_OF_WORDS_
#define _BAG_OF_WORDS_

/**
 * hashset<freq>
 */
typedef struct {
	hashset freqs;
} bag_of_words;

void dispose_bagofwords(bag_of_words *bag);

#endif

bag_of_words *scan_article(FILE *infile);
