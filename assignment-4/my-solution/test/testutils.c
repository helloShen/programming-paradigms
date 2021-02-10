#include "testutils.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Print test info
 */
void tstart(char *spacename) {
	printf("==>Test [%s] ...\n", spacename);
	fflush(stdout);
}

/**
 * Print pass
 */
void tpass(void) {
	printf("[PASS]!\n");
	fflush(stdout);
}

/**
 * This function adapted from Eric Roberts' "The Art and Science of C"
 */
static const signed long kTestUtilsHashMultiplier = -1664117991L;
int t_string_hash(const char *s, const int numBuckets) {
	unsigned long hashcode = 0;
	for (int i = 0; i < strlen(s); i++) {
	    hashcode = hashcode * kTestUtilsHashMultiplier + tolower(s[i]);  
	}
  	return hashcode % numBuckets;                                
}

/**
 * HashSetMapFunction<char *>
 */
void t_print_string(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	fprintf(out, "[%s]\n", *(char **)elemAddr);
	fflush(out);
}

/**
 * char *fq->word is null-terminated.
 * result of snprintf() will be terminated with a null character, 
 * unless buf_size is zero
 */
char *t_freq_tostring(mock_freq *fq) {
	size_t wordlen = strlen(fq->word);
	char *buffer = (char *)malloc(wordlen + 16); // maximum integer has 10 digits
	snprintf(buffer, wordlen + 16, "[%s, %d]\n", fq->word, fq->freq);
	return buffer;
}

/**
 * HashSetMapFunction<mock_freq>
 * Help to test HashSetMap().
 */
void t_freq_maptostring(void *elemAddr, void *auxData) {
	char *word = t_freq_tostring((mock_freq *)elemAddr);
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
void t_print_freq(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	mock_freq *fq = (mock_freq *)elemAddr;
	fprintf(out, "[%s, %d]\n", fq->word, fq->freq);
	fflush(out);
} 

/**
 * Word need to be null-terminated c string
 */ 
void t_new_freq(mock_freq *fq, const char *word, const int freq) {
	fq->word = (char *)malloc((strlen(word) + 1) * sizeof(char)); // +1 for null-terminator
	strcpy(fq->word, word);
	fq->freq = freq;
}

/**
 * HashSetHashFunction<freq>
 */
int t_hash_freq(const void *fq, const int numBuckets) {
	return t_string_hash(((mock_freq *)fq)->word, numBuckets);
}

/**
 * HashSetCompareFunction<freq>
 */
int t_comp_freq(const void *fq1, const void *fq2) {
	return strcmp(((mock_freq *)fq1)->word, ((mock_freq *)fq2)->word);
}

/**
 * HashSetFreeFunction<freq>
 * VectorCFreeFunction<freq>
 */
void t_dispose_freq(void *fq) {
	mock_freq *p = (mock_freq *)fq;
	free(p->word);
	p->word = NULL;
}

/**
 * Read the entire file into a long string.
 * The return stream is null-terminated.
 */
char *t_read_whole_file(const char *filepath) {
	FILE *fp = fopen(filepath, "r");
	assert(fp);
	fseek(fp, 0, SEEK_END);
	size_t fsize = ftell(fp);
	rewind(fp);
	char *buffer = (char *)malloc(fsize + 1); // +1 for null-termination
	fread(buffer, 1, fsize, fp);
	fclose(fp);
	buffer[fsize] = '\0';
	return buffer;
}
