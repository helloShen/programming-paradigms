#include "vector.h"
#include "hashset.h"
#include <stdio.h>
#include <pthread.h>


#ifndef _DOC_FREQ_
#define _DOC_FREQ_

/****************************************************************
 *
 * doc_freq is a key-value pair of [documentid, frequency]
 * It's not supposed to be exposed to user or search engine.
 * -----------------------------------------------------------
 * It looks like:
 *		[1611742826915764000]	[4      ]  
 *		|-------------------|  	|-------|
 *		 docid 					 freq
 ***************************************************************/
typedef struct {
	long docid;
	int freq;
} doc_freq;

#endif // _DOC_FREQ_



#ifndef _WORD_INDEX_
#define _WORD_INDEX_

/****************************************************************
 *
 * word_index is a index of a single word.
 * ---------------------------------------
 * A typical word_index looks like:
 * 		[apple]: [doc1, 5], [doc3, 10], [doc5, 7]
 *		|-----|  |------------------------------|
 *		 word     freqs
 ***************************************************************/
typedef struct {
	char *word;
	hashset *freqs; // hashset<doc_freq>
} word_index;

void merge_wordindex(word_index *wi1, const word_index *wi2);

#endif // _WORD_INDEX_



#ifndef _IDX_
#define _IDX_
/**
 * index is a hashset<word_index>
 * 		[apple]: 	[doc1, 5], [doc3, 10], [doc5, 7]
 * 		[banana]: 	[doc1, 3], [doc2, 12], [doc4, 8]
 *		...			...
 */
typedef struct {
	hashset *index;
	pthread_mutex_t *lock;
} idx;

/**
 * Constructor
 */
void new_index(idx *i);

/**
 * Update idx, replace the old doc_freq if exist.
 */
void enter_index(idx *i, const long docid, const char *word, const int freq);

/**
 * Main entry for search engine.
 */
word_index *search_in_index(const idx *i, const vector *query);

/**
 * Useful for unit-test.
 */
void print_index(const idx *i, FILE *outfile);

/**
 * Free memory
 */
void dispose_index(void *i);

#endif // _INDEX_
