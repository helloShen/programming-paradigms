/**
 * title, link and descritption are NULL terminated.
 */
#include "article.h"
#include "rsstag.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * nanosecond since unix epoch, used as article id.
 */
static long getnano(void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

// static void test_getnano(void) {
// 	printf("Timestamp: %ld\n", getnano());	
// }

/* Constructor */
static const size_t kArticleFieldLength = 256;
void new_article(article *a) {
	a->id = getnano();
	vector *v_title = (vector *)malloc(sizeof(vector));
	vector *v_link = (vector *)malloc(sizeof(vector));
	vector *v_description = (vector *)malloc(sizeof(vector));
	VectorNew(v_title, sizeof(char), NULL, kArticleFieldLength);
	VectorNew(v_link, sizeof(char), NULL, kArticleFieldLength);
	VectorNew(v_description, sizeof(char), NULL, kArticleFieldLength);
	a->title = *v_title;
	a->link = *v_link;
	a->description = *v_description;
}

/**
 * Prevent expat from appending a series of empty spaces at the end of data.
 */
static int isallspace(const char *data, int size) {
	while (size-- > 0) {
		if (*data++ != ' ') return 0;
	}
	return 1;
}

static int isenter(const char *data, int size) {
	return (size == 1 && *data == '\n');
}

/** 
 * Append one of the article field. 
 * Because expat CharacterDataHandler will call this function more than 
 * one time to concatenate a bunch of strings.
 * Each field is NON null-terminated.
 */
void append_article(article *a, const char *data, const int size, rsstag tag) {
	if (isallspace(data, size) || isenter(data, size)) return;
	vector *toAppend = NULL;
	switch (tag) {
		case rss_title: 
			toAppend = &a->title;
			break;
		case rss_link:
			toAppend = &a->link;
			break;
		case rss_description:
			toAppend = &a->description;
			break;
		default: break;
	}
	if (toAppend != NULL) {
		for (int i = 0; i < size; i++) {
			VectorAppend(toAppend, data + i);
		}
	}
}

/**
 * VectorFreeFunction<article>
 * Free memory
 */
void dispose_article(void *elemAddr) {
	article *a = (article *)elemAddr;
	VectorDispose(&(a->title));
	VectorDispose(&(a->link));
	VectorDispose(&(a->description));
}

/**
 * VectorMapFunction<char>
 */
static void printchar(void *elemAddr, void *auxData) {
	FILE *fp = (FILE *)auxData;
	fprintf(fp, "%c", *(char *)elemAddr);
	fflush(fp);
}

/**
 * VectorMapFunction<article>
 * Print article
 */
void print_article(void *elemAddr, void *auxData) {
	FILE *out = (FILE *)auxData;
	article *a = (article *)elemAddr;
	fprintf(out, "\nArticle:[%ld]", a->id);
	fprintf(out, "\n%s: ", "[title]");
	VectorMap(&a->title, printchar, out);
	fprintf(out, "\n%s: ", "[link]");
	VectorMap(&a->link, printchar, out);
	fprintf(out, "\n%s: ", "[description]");
	VectorMap(&a->description, printchar, out);
}

/**
 * Copy a field to the buffer char by char.
 * String in buffer is null-terminated.
 */
static void getfield(vector *field, char *buff, size_t buffsize) {
	int len = VectorLength(field);
	assert(len < buffsize);
	int i;
	for (i = 0; i < len; i++) {
		*(buff + i) = *(char *)VectorNth(field, i);
	}
	*(buff + i) = '\0';
}

/**
 * Copy each character of title field into a string buffer.
 */
void get_title(article *a, char *buff, size_t buffsize) {
	getfield(&(a->title), buff, buffsize);
}

/**
 * Copy each character of link field into a string buffer.
 */
void get_link(article *a, char *buff, size_t buffsize) {
	getfield(&(a->link), buff, buffsize);
}

/**
 * Copy each character of description field into a string buffer.
 */
void get_description(article *a, char *buff, size_t buffsize) {
	getfield(&(a->description), buff, buffsize);
}