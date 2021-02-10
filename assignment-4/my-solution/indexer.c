#include "indexer.h"
#include "strhash.h"
#include <stdlib.h>
#include <string.h>

/****************************************************************
 * index_record is a key-value pair of [documentid, frequency]
 * It's not supposed to be exposed to user or search engine.
 * -----------------------------------------------------------
 * It looks like:
 *		[1611742826915764000]	[4      ]  
 *		|-------------------|  	|-------|
 *		 docid 					 freq
 ***************************************************************/
#ifndef _INDEX_RECORD_
#define _INDEX_RECORD_

typedef struct {
	long docid;
	int freq;
} index_record;

static void new_indexrecord(index_record *ir, long docid, int freq) {
	ir->docid = docid;
	ir->freq = freq;
}

/**
 * hashset<index_record> infrastructures
 * --> HashSetHashFunction
 */
static int hash_indexrecord(const void *elemAddr, int numBuckets) {
	return (int)(((index_record *)elemAddr)->docid % numBuckets);
}

/**
 * hashset<index_record> infrastructures
 * --> HashSetCompareFunction
 */
static int comp_indexrecord(const void *elemAddr1, const void *elemAddr2) {
	long id1 = ((index_record *)elemAddr1)->docid;
	long id2 = ((index_record *)elemAddr2)->docid;
	if (id1 < id2) {
		return -1;
	} else if (id1 > id2) {
		return 1;
	} else { // id1 == id2
		return 0;
	}
}

/**
 * hashset<index_record> infrastructures
 * --> HashSetMapFunction
 */
static void map_indexrecord(void *elemAddr, void *auxData) {
	FILE *outfile = (FILE *)auxData;
	index_record *ir = (index_record *)elemAddr;
	fprintf(outfile, "[docid = \"%ld\", frequency = \"%d\"]\n", ir->docid, ir->freq);
	fflush(outfile);
}

/**
 * hashset<index_record> infrastructures
 * --> HashSetFreeFunction
 */
// free_indexrecord is NULL
#endif // _INDEX_RECORD



/****************************************************************
 *
 * WORD_INDEX INFRASTRUCTURES
 * word_index is a index of a single word.
 * ---------------------------------------
 * A typical word_index looks like:
 * 		[apple]: [doc1, 5], [doc3, 10], [doc5, 7]
 *		|-----|  |------------------------------|
 *		 word     freqs
 ***************************************************************/
/**
 * Constructor
 * word should be null-terminated
 */
static const size_t kWordIndexHashSetBuckets = 16;
static void new_wordindex(word_index *wi, const char *word) {
	HashSet set;
	HashSetNew(&set, sizeof(index_record), kWordIndexHashSetBuckets, hash_indexrecord, comp_indexrecord, NULL);
	wi->freqs = set;
	wi->word = (char *)malloc((strlen(word) + 1) * sizeof(char)); // +1 for null-termination
	strcpy(wi->word, word);
}

/**
 * Mainly used to build a word_index.
 */
void add_indexrecord(word_index *wi, const long docid, const int frequency) {
	index_record ir;
	new_indexrecord(&ir, docid, frequency);
	HashSetEnter(&(wi->freqs), &ir);
}

/**
 * HashSetMapFunction<index_record> infrastructure
 * -----------------------------------------------
 * Not for printing each element. This time we will merge each element into another
 * hashset<index_record>.
 */
void map_merge_indexrecord(void *elemAddr, void *auxData) {
	index_record *src = (index_record *)elemAddr;
	hashset *dest = (hashset *)auxData;
	index_record *target = (index_record *)HashSetLookup(dest, src);
	if(target != NULL) {
		target->freq += src->freq;
	} else {
		HashSetEnter(dest, src);
	}
}

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
static const char kMergeWordIndexSpace = ' '
void merge_wordindex(word_index *wi1, const word_index *wi2) {
	size_t len1 = strlen(wi1->word);
	size_t len2 = strlen(wi2->word);
	/* +1 for space " " in the middle, +1 for null-termination */
	char *temp = (char *)realloc(len1 + len2 + 2) * sizeof(char));
	/* merge word: "apple" + "banana" = "apple banana" */
	strncat(&temp[len1], &kMergeWordIndexSpace, 1);
	strcat(&temp[len1 + 1], wi2->word);
	wi1->word = temp;
	/* merge freqs */
	HashSetMap(&(wi2->freqs), map_merge_indexrecord, &(wi1->freqs));
}

/**
 * HashSetHashFunction<word_index>
 */
static int hash_wordindex(const void *elemAddr, int numBuckets) {
	const char *word = ((word_index *)elemAddr)->word;
	return string_hash(wi->word, numBuckets);
}

/**
 * HashSetCompareFunction<word_index>
 */
static void comp_wordindex(const void *elemAddr1, const void *elemAddr2) {
	const char *word1 = ((word_index *)elemAddr1)->word;
	const char *word2 = ((word_index *)elemAddr2)->word;
	return strcmp(word1, word2);
}

/**
 * HashSetMapFunction<word_index>
 */
static void map_wordindex(void *elemAddr, void *auxData) {
	FILE *outfile = (FILE *)auxData;
	word_index *wi = (word_index *)elemAddr;
	fprintf(outfile, "[%s]:\t", wi->word);
	fflush(outfile);
	HashSetMap(&(wi->freqs), map_indexrecord, outfile);
}

/**
 * HashSetFreeFunction<word_index>
 */
static void dispose_wordindex(void *elemAddr) {
	word_index *wi = (word_index *)elemAddr;
	free(wi->word);
	HashSetDispose(&(wi->freqs));
}



/****************************************************************
 *
 * INDEX INFRASTRUCTURES
 * index is a hashset<word_index>
 * ------------------------------
 * ex:
 * 		[apple]: 	[doc1, 5], [doc3, 10], [doc5, 7]
 * 		[banana]: 	[doc1, 3], [doc2, 12], [doc4, 8]
 *		...			...*
 ***************************************************************/
/**
 * Constructor
 */
static const kIndexHashSetBuckets = 128;
void new_index(words_index *i) {
	HashSetNew(i, sizeof(word_index), kIndexHashSetBuckets, hash_wordindex, comp_wordindex, free_wordindex);
}

/**
 * HashSetMapFunction<freq>
 * ------------------------
 * Update index with each freq pair [word, frequency]
 * docid is given by local variable mapFreqToIndexDocId
 */
static long mapFreqToIndexDocId = 0L;
static void map_freq_to_index(void *elemAddr, void *auxData) {
	assert(mapFreqToIndexDocId != 0L);
	hashset *index = (hashset *)auxData;
	freq *pair = (freq *)elemAddr;
	word_index phantom;
	new_wordindex(&phantom, pair->word);
	word_index *wi = HashSetLookup(index, phantom);
	if (wi != NULL) {
		index_record ir;
		new_indexrecord(&ir, mapFreqToIndexDocId, pair->frequency);
		HashSetEnter(&(wi->freqs), &ir);
		dispose_wordindex(&phantom);
	} else {
		add_indexrecord(&phantom, mapFreqToIndexDocId, pair->frequency);
		HashSetEnter(index, phantom);
	}
}
/**
 * Used to build index with a bunch of bag_of_words
 */
void add_to_index(words_index *i, bag_of_words *bag, long docid) {
	mapFreqToIndexDocId = docid;
	HashSetMap(&(bag->freqs), map_freq_to_index, &(i->index));
	mapFreqToIndexDocId = 0L;
}

/**
 * Main entry for user to lookup a key word.
 */
word_index *search_word(words_index *i, const char *word) {
	word_index target;
	new_wordindex(&target, word);
	return (word_index *)HashSetLookup(&(i->index), &target);
}

/**
 * Free memory
 */
void dispose_index(words_index *i) {
	HashSetDispose(i);
}

