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
#include "hashset.h"
#include "stack.h"
#include "strhash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


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
int hashstr(const void *elemAddr, int numBuckets) {
	char *str = *(char **)elemAddr;
	return string_hash(str, numBuckets); 
}

/**
 * HashSetCompareFunction<char *>
 */
int compstr(const void *elemAddr1, const void *elemAddr2) {
	char *str1 = *(char **)elemAddr1;
	char *str2 = *(char **)elemAddr2;
	return strcmp(str1, str2);
}

/**
 * HashSetFreeFunction<char *>
 */
void freestr(void *elemAddr) {
	char **strp = (char **)elemAddr;
	free(*strp);
	*strp = NULL;
}

/**
 * char *fq->word is null-terminated.
 * result of snprintf() will be terminated with a null character, 
 * unless buf_size is zero
 */
static char *freq_tostring(const freq *fq) {
	size_t wordlen = strlen(fq->word);
	char *buffer = (char *)malloc(wordlen + 16); // maximum integer length = 10 digits
	snprintf(buffer, wordlen + 16, "[%s, %d]\n", fq->word, fq->freq);
	return buffer;
}

/**
 * HashSetMapFunction<mock_freq>
 * Help to test HashSetMap().
 */
static void freq_maptostring(void *elemAddr, void *auxData) {
	char *word = freq_tostring((freq *)elemAddr);
	size_t wordsize = strlen(word);
	size_t strsize = strlen(*(char **)auxData);
	char *temp = (char *)realloc(*(char **)auxData, strsize + wordsize + 1);
	strcpy(temp + strsize, word);
	temp[strsize + wordsize] = '\0';
	*(char **)auxData = temp;
	free(word);
}

/**
 * HashSetMapFunction<freq>
 */
static void print_freq(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	freq *fq = (freq *)elemAddr;
	fprintf(out, "[%s, %d]\n", fq->word, fq->freq);
	fflush(out);
} 

/**
 * Word need to be null-terminated c string
 */ 
static void new_freq(freq *fq, char *word, const int freq) {
	fq->word = (char *)malloc(strlen(word) + 1);
	strcpy(fq->word, word);
	fq->freq= freq;
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
static int comp_freq(const void *fq1, const void *fq2) {
	return strcmp(((freq *)fq1)->word, ((freq *)fq2)->word);
}

/**
 * HashSetFreeFunction<freq>
 * VectorCFreeFunction<freq>
 */
static void dispose_freq(void *fq) {
	free(((freq *)fq)->word);
	((freq *)fq)->word = NULL;
}

/**
 * Dump current string into vector as a new word.
 * Strings are null-terminated.
 */
static void dumpstack(stack *s, vector *v) {
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
 * Note: 
 * 	==> 1. input stream should be null-terminated.
 * 	==> 2. each token in return vector is null-terminated.
 * 
 */
static const size_t kTokenStackDefaultSize = 64;
void tokenize(vector *words, const char *stream) {
	stack s;
	new_stack(&s, kTokenStackDefaultSize, sizeof(char), NULL);
	size_t len = strlen(stream);
	bool begin = false;
	char c;
	for (int i = 0; i < len; i++) {
		c = stream[i];
		if (isalpha(c) || isdigit(c)) {
			if (begin == false) begin = true;
			char lower = tolower(c);
			push_stack(&s, &lower, sizeof(char));
		} else if (c == '-') {
			if (begin == true) { // case: covid-19
				push_stack(&s, &c, sizeof(char));
			} else {
				if (i < len - 1 && isdigit(stream[i + 1])) { // case: -9
					begin = true;
					push_stack(&s, &c, sizeof(char));
				} else {
					if (begin == true) {
						dumpstack(&s, words);
						begin = false;
					}
				}
			}
		} else if (c == '.' && begin == true) { // case: 3.14
			if (isdigit(stream[i - 1])) {
				push_stack(&s, &c, sizeof(char));
			} else {
				if (begin == true) {
					dumpstack(&s, words);
					begin = false;
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
	dispose_stack(&s);
}

static const size_t kTokenizerBagofwordsDefaultBuckets = 128;
void new_bagofwords(bag_of_words *bag, const long docid) {
	bag->docid = docid;
	bag->freqs = (hashset *)malloc(sizeof(hashset));
	HashSetNew(bag->freqs, sizeof(freq), kTokenizerBagofwordsDefaultBuckets, hash_freq, comp_freq, dispose_freq);
}

void enter_bagofwords(bag_of_words *bag, char *word) {
	freq newfreq; // new_freq() will make a copy of word field, no need to malloc new space.
	new_freq(&newfreq, word, 1);
	freq *history = (freq *)HashSetLookup(bag->freqs, &newfreq);
	if (history != NULL) {
		history->freq++;
		dispose_freq(&newfreq);
	} else {
		HashSetEnter(bag->freqs, &newfreq);
	}
}

static void print_bagofwords(bag_of_words *bag, FILE *outfile) {
	HashSetMap(bag->freqs, print_freq, outfile);
}

/**
 * Interface for user to free bag_of_words memory.
 */ 
void dispose_bagofwords(bag_of_words *bag) {
	HashSetDispose(bag->freqs);
	free(bag->freqs);
	bag->freqs = NULL;
}

/**
 * Load stopwords.
 * ---------------
 * stopwords in array kTokenizerStopwordsArray are pre-generated into data/stopwords.data
 * from data/stop-words.in by genstopwords.py.
 * Do NOT edit by hand anything in the data/stopwords.data 
 */
static const char *kTokenizerStopwordsArray[] = {
#include "data/stopwords.data"
};

static const size_t kTokenizerStopwordsBucket = 256;
hashset *load_stopwords(void) {
	hashset *stopwords = (hashset *)malloc(sizeof(hashset));
	HashSetNew(stopwords, sizeof(char *), kTokenizerStopwordsBucket, hashstr, compstr, NULL); // freefn is NULL, because we didn't malloc space for each stopword.
	size_t arrsize = sizeof(kTokenizerStopwordsArray)/sizeof(char *);
	for (int i = 0; i < arrsize; i++) {
		HashSetEnter(stopwords, kTokenizerStopwordsArray + i);
	}
	return stopwords;
}

void dispose_stopwords(hashset *stopwords) {
	HashSetDispose(stopwords);
}

/**
 * Use output words vector of tokenize() function as input, count the frequency of each word.
 */
static void count(bag_of_words *bag, const vector *words, const hashset *stopwords) {
	for (int i = 0; i < VectorLength(words); i++) {
		char *word = *(char **)VectorNth(words, i);
		if (stopwords != NULL && HashSetLookup(stopwords, &word) != NULL) continue;
		enter_bagofwords(bag, word);
	}
}

/**
 * Read the input stream, return words frequencies of that file.
 */
static const size_t kTokenizerVectorSize = 1024;
bag_of_words *to_bagofwords(const long docid, const char *stream, const hashset *stopwords) {
	bag_of_words *bag = (bag_of_words *)malloc(sizeof(bag_of_words));
	new_bagofwords(bag, docid);
	vector words;
	VectorNew(&words, sizeof(char *), freestr, kTokenizerVectorSize);
	tokenize(&words, stream);
	count(bag, &words, stopwords);
	VectorDispose(&words);
	return bag;
}


