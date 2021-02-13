/**
 * Interface of tokenizer.c that exposed to user.
 */


#ifndef _FREQ_
#define _FREQ_

/**
 * word is null-terminated string
 * hide from user
 */
typedef struct {
	char *word;
	int frequency;
} freq;

#endif // _FREQ_



#ifndef _BAG_OF_WORDS_
#define _BAG_OF_WORDS_

#include "hashset.h"
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

#endif // _BAG_OF_WORDS_



/**
 * Return a hashset<char *>, each element is a stopword.
 * Use dispose_stopwords() to free the memory after using it.
 */
hashset *load_stopwords(void);

/**
 * Free memory
 */
void dispose_stopwords(hashset *stopwords);

/**
 * It tokenize the input string into a list of words, then count the
 * words frequencies.
 * If stopwords hashset is NULL, it counts words frequencies without 
 * stopwords.
 */
void to_bagofwords(bag_of_words *bag, char *stream, hashset *stopwords);
