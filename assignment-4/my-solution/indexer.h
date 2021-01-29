#include "hashset.h"
#include "tokenizer.h"


#ifndef _INDEX_
#define _INDEX_

/**
 * index is a hashset<wordfreqs>
 * 		[apple]: 	[doc1, 5], [doc3, 10], [doc5, 7]
 * 		[banana]: 	[doc1, 3], [doc2, 12], [doc4, 8]
 *		...			...
 */
typedef struct {
	hashset wordsindex;
} index;

void dispose_wordfreqs(wordfreqs *wf);

void dispose_index(index *i);

/**
 * constructor
 */
void init_index(index *i);

/**
 * Update index by adding infos from a bag_of_words.
 */
void add_to_index(index *i, bag_of_words *bag);

#endif
