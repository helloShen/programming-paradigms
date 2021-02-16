/**
 * Interface of tokenizer.c that exposed to user.
 */

/**
 * HashSetHashFunction<char *>
 */
int hashstr(const void *elemAddr, int numBuckets);

/**
 * HashSetCompareFunction<char *>
 */
int compstr(const void *elemAddr1, const void *elemAddr2);

/**
 * HashSetFreeFunction<char *>
 */
void freestr(void *elemAddr);

#ifndef _FREQ_
#define _FREQ_

/**
 * word is null-terminated string
 */
typedef struct {
	char *word;
	int freq;
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
	long docid;
	hashset *freqs;
} bag_of_words;

/**
 * Free memory
 */
void dispose_bagofwords(bag_of_words *bag);

#endif // _BAG_OF_WORDS_

/**
 * Extract contiguous sequences of alphabets or digits,
 * and push them into vector.
 * Note: 
 * 	==> 1. input stream should be null-terminated.
 * 	==> 2. each token in return vector is null-terminated.
 * 
 */
void tokenize(vector *words, const char *stream);

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
bag_of_words *to_bagofwords(const long docid, const char *stream, const hashset *stopwords);
