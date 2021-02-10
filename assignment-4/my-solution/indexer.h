#include "hashset.h"
#include "tokenizer.h"

#ifndef _WORD_INDEX_
#define _WORD_INDEX_
/**
 * word_index is a index of a single word.
 * ---------------------------------------
 * A typical word_index looks like:
 * 		[apple]: [doc1, 5], [doc3, 10], [doc5, 7]
 *		|-----|  |------------------------------|
 *		 word     freqs
 */
typedef struct {
	char *word;
	hashset freqs; // hashset<index_record>
} word_index;

/**
 * Used to build word_index
 */
void add_indexrecord(word_index *wi, const long docid, const int frequency);

/**
 * Add all index_record of wi2 into wi1
 * Interface exposed to search engine.
 * ------------------------------------
 * Search engine call this function to build a hashset<index_record> of entire query.
 *
 * 		[apple]:	[doc1, 5], [doc3, 10], [doc5, 7]
 *			+
 * 		[banana]: 	[doc1, 3], [doc2, 6], [doc5, 2]
 *			=
 *		[apple banana]:	[doc1, 8], [doc2, 6], [doc3, 10], [doc5, 10]
 */
void merge_wordindex(word_index *wi1, word_index *wi2);
#endif // _WORD_INDEX_



#ifndef _WORDS_INDEX_
#define _WORDS_INDEX_
/**
 * index is a hashset<word_index>
 * 		[apple]: 	[doc1, 5], [doc3, 10], [doc5, 7]
 * 		[banana]: 	[doc1, 3], [doc2, 12], [doc4, 8]
 *		...			...
 */
typedef struct {
	hashset index;
} words_index;

/**
 * Constructor
 */
void new_index(words_index *i);

/**
 * Used to build index with a bunch of bag_of_words
 */
void add_to_index(words_index *i, bag_of_words *bag, long docid);

/**
 * Main entry for user to lookup a key word.
 */
void search_word(words_index *i, const char *word);

/**
 * Free memory
 */
void dispose_index(words_index *i);
#endif // _WORDS_INDEX_
