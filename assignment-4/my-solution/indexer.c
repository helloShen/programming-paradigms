#include "indexer.h"
#include <stdlib.h>

/**
 * [input]:		
 * [output]:	
 */
#ifndef _WORDFREQS_
#define _WORDFREQS_

/**
 * A typical wordfreqs looks like:
 * 		[apple]: [doc1, 5], [doc3, 10], [doc5, 7]
 *		|-----|  |------------------------------|
 *		 word     freqs
 */
typedef struct {
	char *word;
	hashset freqs;
} wordfreqs;

void dispose_wordfreqs(wordfreqs *wf) {
	// todo
}

#endif

void dispose_index(index *i) {
	// todo
}

void init_index(index *i) {
	// todo	
}

void add_to_index(index *i, bag_of_words *bag) {
	// todo
}

