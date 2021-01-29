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
 * It returns hashset in following format:
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef _FREQ_
#define _FREQ_

/**
 * word is null-terminated c string
 */
typedef struct {
	char *word;
	int frequency;
} freq;

/**
 * Word need to be null-terminated c string
 */
static void new_freq(freq *fq, const char *word, const int frequency) {
	fq->word = (char *)malloc((strlen(word) + 1) * sizeof(char)); // +1 for null-terminator
	strcpy(fq->word, word);
	fq->frequency = frequency;
}

static const signed long kHashMultiplier = -1664117991L;
static int string_hash(const char *s, int numBuckets) {
	unsigned long hashcode = 0;
	for (int i = 0; i < strlen(s); i++) {
	    hashcode = hashcode * kHashMultiplier + tolower(s[i]);  
	}
  	return hashcode % numBuckets;                                
}

/**
 * HashSetHashFunction
 */
static int hash_freq(const void *fq, int numBuckets) {
	return string_hash(((freq *)fq)->word, numBuckets);
}

/**
 * HashSetCompareFunction
 */
static int compare_freq(const void *fq1, const void *fq2) {
	return strcmp(((freq *)fq1)->word, ((freq *)fq2)->word);
}

/**
 * HashSetFreeFunction
 * VectorCFreeFunction
 */
static void dispose_freq(void *fq) {
	free(((freq *)fq)->word);
}
#endif


static const size_t kTokenizerDefaultBuckets = 128;
static void new_bagofwords(bag_of_words *bag) {
	HashSetNew(&bag->freqs, sizeof(freq), kTokenizerDefaultBuckets, hash_freq, compare_freq, dispose_freq);
}

static void bag_add_word(bag_of_words *bag, char *word) {
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
			printf("[%c] is alphabet or digit!\n", c);
			if (begin == false) begin = true;
			lower = tolower(c);
			push_stack(&s, &lower, sizeof(char));
		} else if (c == '.') { // for float number, like 3.14
			if (begin == true) {
				pop_stack(&s, &lastc);
				push_stack(&s, &lastc, sizeof(char));
				if (isdigit(lastc)) {
					printf("[%c] is good dot!\n", c);
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
 * Update the words frequencies: bag_of_words,
 * using the news vector of words.
 * [words] is vector<char *>
 */
static void count(bag_of_words *bag, vector *words) {
	for (int i = 0; i < VectorLength(words); i++) {
		char *word = *(char **)VectorNth(words, i);
		bag_add_word(bag, word);	
	}
}

/**
 * Open a file, return words frequencies of that file.
 */
bag_of_words *scan_article(FILE *infile) {
	bag_of_words *bag = (bag_of_words *)malloc(sizeof(bag_of_words));
	new_bagofwords(bag);
	// to be continued ...
	return bag;
}


