#include "index.h"
#include "strhash.h"
#include "vector.h"
#include "hashset.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

static void new_docfreq(doc_freq *df, const long docid, const int freq) {
	df->docid = docid;
	df->freq = freq;
}

/**
 * HashSetHashFunction<doc_freq>
 */
static int hash_docfreq(const void *elemAddr, int numBuckets) {
	doc_freq *df = (doc_freq *)elemAddr;
	return (int)(df->docid % numBuckets);
}

/**
 * HashSetCompareFunction<doc_freq>
 */
static int comp_docfreq(const void *elemAddr1, const void *elemAddr2) {
	long id1 = ((doc_freq *)elemAddr1)->docid;
	long id2 = ((doc_freq *)elemAddr2)->docid;
	if (id1 < id2) {
		return -1;
	} else if (id1 > id2) {
		return 1;
	} else { // id1 == id2
		return 0;
	}
}

/**
 * In format: [docid=1611742826915764000, freq=5]
 * return string need to be freed.
 */
static char *docfreq_tostring(const doc_freq *df) {
	char *buffer = (char *)malloc(64);
	char *start = buffer;
	char id[32];
	int idlen = snprintf(id, sizeof(id), "%ld", df->docid);
	char freq[16];
	int freqlen = snprintf(freq, sizeof(freq), "%d", df->freq);
	char *glue1 = "[docid=";
	char *glue2 = ", freq=";
	char *glue3 = "]";
	strcpy(start, glue1);
	start += strlen(glue1);
	strcpy(start, id);
	start += idlen;
	strcpy(start, glue2);
	start += strlen(glue2);
	strcpy(start, freq);
	start += freqlen;
	strcpy(start, glue3);
	return buffer;
}

/**
 * HashSetMapFunction<doc_freq>
 */
static void print_docfreq(void *elemAddr, void *auxData) {
	FILE *outfile = (FILE *)auxData;
	doc_freq *df = (doc_freq *)elemAddr;
	fprintf(outfile, "%s", docfreq_tostring(df));
	fflush(outfile);
}

/**
 * HashSetFreeFunction<doc_freq>
 */
// dispose_docfreq() should be NULL

/**
 * Constructor
 * word should be null-terminated
 */
static const size_t kWordIndexHashSetBuckets = 64;
static void new_wordindex(word_index *wi, const char *word) {
	hashset *h = (hashset *)malloc(sizeof(hashset));
	HashSetNew(h, sizeof(doc_freq), kWordIndexHashSetBuckets, hash_docfreq, comp_docfreq, NULL);
	wi->freqs = h;
	size_t wordlen = strlen(word);
	wi->word = (char *)malloc(wordlen + 1); // +1 for null-termination
	strcpy(wi->word, word);
	(wi->word)[wordlen] = '\0';
}

/**
 * Mainly used to build a word_index.
 */
static void enter_wordindex(word_index *wi, const long docid, const int freq) {
	doc_freq df;	// hashset will copy the whole doc_freq element, no need to malloc memory.
	new_docfreq(&df, docid, freq);
	HashSetEnter(wi->freqs, &df);
}

/**
 * HashSetMapFunction<doc_freq>
 * ----------------------------
 * Merge each doc_freq into another hashset<doc_freq>.
 */
static void merge_docfreq(void *elemAddr, void *auxData) {
	doc_freq *toadd = (doc_freq *)elemAddr;
	hashset *dest = (hashset *)auxData;
	doc_freq *target = (doc_freq *)HashSetLookup(dest, toadd);
	if(target != NULL) {
		target->freq += toadd->freq;
	} else {
		HashSetEnter(dest, toadd);
	}
}

/**
 * Add all doc_freq of wi2 into wi1
 * --------------------------------
 * Search engine call this function to build a hashset<doc_freq> of entire query.
 *
 * 		[apple]:	[doc1, 5], [doc3, 10], [doc5, 7]
 *			+
 * 		[banana]: 	[doc1, 3], [doc2, 6], [doc5, 2]
 *			=
 *		[apple banana]:	[doc1, 8], [doc2, 6], [doc3, 10], [doc5, 10]
 */
void merge_wordindex(word_index *wi1, const word_index *wi2) {
	size_t len1 = strlen(wi1->word);
	size_t len2 = strlen(wi2->word);
	char *temp = (char *)realloc(wi1->word, len1 + len2 + 2); // +1 for middle space, +1 for null-terminator
	if (len1 > 0) {
		char space = ' ';
		strncat(temp + len1, &space, 1);
		strcat(temp + len1 + 1, wi2->word);
	} else {
		strcat(temp + len1, wi2->word);
	}
	wi1->word = temp;
	/* merge freqs */
	HashSetMap(wi2->freqs, merge_docfreq, wi1->freqs);
}

/**
 * HashSetHashFunction<word_index>
 */
static int hash_wordindex(const void *elemAddr, int numBuckets) {
	word_index *wi = (word_index *)elemAddr;
	return string_hash(wi->word, numBuckets);
}

/**
 * HashSetCompareFunction<word_index>
 */
static int comp_wordindex(const void *elemAddr1, const void *elemAddr2) {
	word_index *word1 = (word_index *)elemAddr1;
	word_index *word2 = (word_index *)elemAddr2;
	return strcmp(word1->word, word2->word);
}

/**
 * Search doc_freq in word_index by docid
 */
static doc_freq *search_in_wordindex(const word_index *wi, const long docid) {
	doc_freq target;
	new_docfreq(&target, docid, 0);
	return (doc_freq *)HashSetLookup(wi->freqs, &target);
}

/**
 * HashSetMapFunction<doc_freq>
 */
static void wordindex_tostring_helper(void *elemAddr, void *auxData) {
	doc_freq *df = (doc_freq *)elemAddr;
	char *from = docfreq_tostring(df);
	char **to = (char **)auxData;
	size_t fromsize = strlen(from);
	size_t tosize = strlen(*to);
	char *temp = (char *)realloc(*to, tosize + fromsize + 1);
	strcpy(temp + tosize, from);
	temp[tosize + fromsize] = '\0';
	*to = temp;
	free(from);
}

/**
 * In format: apple:	[1611742826915764000, 5][1611742826915538000, 10]
 * Return string need to be freed.
 */
static char *wordindex_tostring(const word_index *wi) {
	size_t wordlen = strlen(wi->word);
	char *buffer = (char *)malloc(wordlen + 2 + 1);
	strcpy(buffer, wi->word);
	strcpy(buffer + wordlen, ":\t");
	buffer[wordlen + 2] = '\0';
	HashSetMap(wi->freqs, wordindex_tostring_helper, &buffer);
	return buffer;
}

/**
 * HashSetMapFunction<word_index>
 */
static void print_wordindex(void *elemAddr, void *auxData) {
	FILE *outfile = (FILE *)auxData;
	word_index *wi = (word_index *)elemAddr;
	fprintf(outfile, "%s\n", wordindex_tostring(wi));
	fflush(outfile);
}

/**
 * HashSetFreeFunction<word_index>
 */
static void dispose_wordindex(void *elemAddr) {
	word_index *wi = (word_index *)elemAddr;
	free(wi->word);
	wi->word = NULL;
	HashSetDispose(wi->freqs);
	free(wi->freqs);
	wi->freqs = NULL;
}

/**
 * Constructor
 */
static const size_t kIndexHashSetBuckets = 1024;
void new_index(idx *i) {
	i->index = (hashset *)malloc(sizeof(*i->index));
	HashSetNew(i->index, sizeof(word_index), kIndexHashSetBuckets, hash_wordindex, comp_wordindex, dispose_wordindex);
	i->lock = (pthread_mutex_t *)malloc(sizeof(*i->lock));
}

/**
 * enter_index() copies the word, hashset copies docid and freq, so that the 
 * original bag_of_words can be freed after building index.
 * Update idx, replace the old doc_freq if exist.
 */
void enter_index(idx *i, const long docid, const char *word, const int freq) {
	word_index phantom; // hashset will copy phantom word_index, no need to malloc new space.
	new_wordindex(&phantom, word); // new_wordindex() will copy the word, no need to malloc new space for word.
	/* critical area */
	pthread_mutex_lock(i->lock);
	word_index *history = (word_index *)HashSetLookup(i->index, &phantom);
	if (history != NULL) {
		enter_wordindex(history, docid, freq);
		dispose_wordindex(&phantom);
	} else {
		enter_wordindex(&phantom, docid, freq);	
		HashSetEnter(i->index, &phantom);
	}
	pthread_mutex_unlock(i->lock);
}

/**
 * Main entry for search engine to lookup a key word.
 */
static void search_word_in_index(const idx *i, const char *word, word_index *result) {
	word_index target;
	new_wordindex(&target, word);
	word_index *local_result = (word_index *)HashSetLookup(i->index, &target);
	if (local_result != NULL) {
		merge_wordindex(result, local_result);
	}
	dispose_wordindex(&target);
}

word_index *search_in_index(const idx *i, const vector *query) {
	word_index *result = (word_index *)malloc(sizeof(word_index));
	new_wordindex(result, "");
	for (int j = 0; j < query->elemNum; j++) {
		char *word = *(char **)VectorNth(query, j);
		search_word_in_index(i, word, result);
	}
	return result;
}

void print_index(const idx *i, FILE *outfile) {
	HashSetMap(i->index, print_wordindex, outfile);
}

/**
 * Free memory
 */
void dispose_index(void *i) {
	idx *index = (idx *)i;
	HashSetDispose(index->index);
	free(index->index);
	index->index = NULL;
	pthread_mutex_destroy(index->lock);
	free(index->lock);
	index->lock = NULL;
}

