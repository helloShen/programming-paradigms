/**
 * [input]:		plain text of article
 * [output]:	bag_of_words
 * bag_of_words is a hashset of words with a frequency for each.
 * ----------------------------
 * It works in two steps:
 * 		1. Tokenize article text into a list of single word.
 *   	2. Then count the frequency of each word
 * For example, give me the text: 
 * 		"This apple is a sweet apple."
 * tokenize() function cut words:
 *		this
 *   	apple
 *   	is
 *   	a
 *   	sweet
 *   	apple
 * count() function calculates words frequencies:
 *   	this 	1
 *   	apple 	2
 *   	is		1
 *   	a 		1
 *   	sweet	1
 */
#include "tokenizer.h"
#include "vector.h"
#include "stack.h"
#include "bool.h"
#include "strhash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifndef _FREQ_
#define _FREQ_

/**
 * word is null-terminated c string
 * hide from user
 */
typedef struct {
	char *word;
	int frequency;
} freq;

/**
 * HashSetMapFunction<char *>
 */
static void printstr(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	fprintf(out, "[%s]\n", *(char **)elemAddr);
	fflush(out);
}

/**
 * HashSetHashFunction<char *>
 */
static int hashstr(const void *elemAddr, int numBuckets) {
	char *str = *(char **)elemAddr;
	return string_hash(str, numBuckets); 
}

/**
 * HashSetCompareFunction<char *>
 */
static int compstr(const void *elemAddr1, const void *elemAddr2) {
	char *str1 = *(char **)elemAddr1;
	char *str2 = *(char **)elemAddr2;
	return strcmp(str1, str2);
}

/**
 * HashSetFreeFunction<char *>
 */
static void freestr(void *elemAddr) {
	char *str = *(char **)elemAddr;
	free(str);
}

/**
 * Word need to be null-terminated c string
 */
static void new_freq(freq *fq, const char *word, const int frequency) {
	fq->word = (char *)malloc((strlen(word) + 1) * sizeof(char)); // +1 for null-terminator
	strcpy(fq->word, word);
	fq->frequency = frequency;
}

/**
 * HashSetHashFunction<freq>
 */
static int hash_freq(const void *fq, const int numBuckets) {
	return string_hash(((freq *)fq)->word, numBuckets);
}

/**
 * HashSetCompareFunction<freq>
 */
static int compare_freq(const void *fq1, const void *fq2) {
	return strcmp(((freq *)fq1)->word, ((freq *)fq2)->word);
}

/**
 * HashSetMapFunction<freq>
 */
static void print_freq(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	freq *fq = (freq *)elemAddr;
	fprintf(out, "[%s, %d]\n", fq->word, fq->frequency);
	fflush(out);
}

/**
 * HashSetFreeFunction<freq>
 * VectorCFreeFunction<freq>
 */
static void dispose_freq(void *fq) {
	free(((freq *)fq)->word);
}
#endif


static const size_t kTokenizerDefaultBuckets = 128;
void new_bagofwords(bag_of_words *bag) {
	HashSetNew(&bag->freqs, sizeof(freq), kTokenizerDefaultBuckets, hash_freq, compare_freq, dispose_freq);
}

static void bag_add_word(bag_of_words *bag, const char *word) {
	freq *target = (freq *)HashSetLookup(&(bag->freqs), &word);
	if (target != NULL) {
		target->frequency++;
	} else {
		freq newfreq;
		new_freq(&newfreq, word, 1);
		HashSetEnter(&(bag->freqs), &newfreq);
	}
}

/**
 * Exposed to user.
 */
void print_bagofwords(bag_of_words *bag, FILE *outfile) {
	HashSetMap(&(bag->freqs), print_freq, outfile);
}

/**
 * Interface for user to free bag_of_words memory.
 */
void dispose_bagofwords(bag_of_words *bag) {
	HashSetDispose(&(bag->freqs));
}

/**
 * Dump current characters into vector as a new word.
 */
void dumpstack(stack *s, vector *v) {
	size_t len = stack_size(s);
	char *word = (char *)malloc((len + 1) * sizeof(char)); // +1 for null-terminator
	for (int i = len - 1; i >= 0; i--) {
		 pop_stack(s, word + i * sizeof(char));
	}
	word[len] = '\0';
	VectorAppend(v, &word);
	clear_stack(s);
}

/**
 * Extract contiguous sequences of alphabets or digits,
 * and push them into vector.
 * Note: stream should be null-terminated.
 */
static const size_t kTokenStackDefaultSize = 16;
static void tokenize(vector *words, char *stream) {
	stack s;
	new_stack(&s, kTokenStackDefaultSize, sizeof(char), NULL);
	bool begin = false;
	char c, lastc, lower;
	for (int i = 0; i < strlen(stream); i++) {
		c = stream[i];
		if (isalpha(c) || isdigit(c) || c == '-') {
			if (begin == false) begin = true;
			lower = tolower(c);
			push_stack(&s, &lower, sizeof(char));
		} else if (c == '.') { // for float number, like 3.14
			if (begin == true) {
				pop_stack(&s, &lastc);
				push_stack(&s, &lastc, sizeof(char));
				if (isdigit(lastc)) {
					push_stack(&s, &c, sizeof(char));
				} else {
					if (begin) {
						dumpstack(&s, words);
						begin = false;
					}
				}
			}
		} else {
			if (begin == true) {
				dumpstack(&s, words);
				begin = false;
			}
		}
	}
	if (begin == true) {
		dumpstack(&s, words);
		begin = false;
	}
}

/**
 * unit test of tokenize() function
 */
void testtokenize(FILE *infile, FILE *outfile) {	
	assert(infile);
	assert(outfile);
	char line[2048];
	vector words;
	VectorNew(&words, sizeof(char *), freestr, 1024);
	while (fgets(line, 2048, infile) != NULL) {
		tokenize(&words, line);
	}
	VectorMap(&words, printstr, outfile);
	VectorDispose(&words);
}

/**
 * Update the words frequencies: bag_of_words,
 * using the news vector of words.
 * [words] is vector<char *>
 */
static void count(bag_of_words *bag, vector *words, hashset *stopwords, bool usestopwords) {
	if (usestopwords == true) assert(stopwords != NULL);
	for (int i = 0; i < VectorLength(words); i++) {
		char *word = *(char **)VectorNth(words, i);
		if (usestopwords == true && HashSetLookup(stopwords, &word) != NULL) continue;
		bag_add_word(bag, word);
	}
}

/**
 * Help user to initialize stopwords hashset.
 * stopwords is a hashset<char *>
 */
static const size_t kStopwordsBucketsSize = 128;
hashset *init_stopwords(void) {
	hashset *stopwords = (hashset *)malloc(sizeof(hashset));
	HashSetNew(stopwords, sizeof(char *), kStopwordsBucketsSize, hashstr, compstr, freestr);
	return stopwords;
}

/**
 * Suppose that stopwords are listed as follow (1 word per line):
 * 		a
 * 		ii
 * 		about
 * 		above
 * 		according
 * 		across
 * 		actually
 * 		ad
 * 		...
 */
static const size_t kStopwordsMaxLength = 32;
void load_stopwords(hashset *stopwords, FILE *stopfile) {
	assert(stopfile);
	char word[32];
	int len;
	while (fgets(word, kStopwordsMaxLength, stopfile) != NULL) {
		len = strlen(word);
		char *copy = (char *)malloc(len * sizeof(char)); 
		strncpy(copy, word, len - 1);
		copy[len] = '\0';
		HashSetEnter(stopwords, &copy);
	}
}

void print_stopwords(hashset *stopwords, FILE *outfile) {
	HashSetMap(stopwords, printstr, outfile);
}

/**
 * Help user to free memory of stopwords hashset
 * stopwords is a hashset<char *>
 */
void dispose_stopwords(hashset *stopwords) {
	HashSetDispose(stopwords);
}

/**
 * Open a file, return words frequencies of that file.
 */
static const size_t kTokenizerVectorSize = 1024;
static const size_t kTokenizerLineSize = 2048;
void to_bagofwords(bag_of_words *bag, FILE *infile, hashset *stopwords, bool usestopwords) {
	assert(infile);
	vector words;
	VectorNew(&words, sizeof(char *), freestr, kTokenizerVectorSize);
	char line[kTokenizerLineSize];
	while (fgets(line, kTokenizerLineSize, infile) != NULL) {
		tokenize(&words, line);
	}
	count(bag, &words, stopwords, usestopwords);
	VectorDispose(&words);
}


