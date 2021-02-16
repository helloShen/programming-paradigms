/**
 * title, link and descritption are null-terminated strings.
 */
#include "article.h"
#include "hashset.h"
#include "rsstag.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * Nanosecond since unix epoch, used as article id.
 */
static long article_count = 0;	// prevent from generating identicle docid
static long getnano(void) {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return (long)ts.tv_sec * 1000000000L + ts.tv_nsec + ++article_count;
}

/** 
 * Constructor 
 */
void new_article(article *a) {
	a->id = getnano();
	a->title = (char *)malloc(1);
	a->link = (char *)malloc(1);
	a->description = (char *)malloc(1);
	*a->title = '\0';
	*a->link = '\0';
	*a->description = '\0';
}

void new_article_with_docid(article *a, const long docid) {
	a->id = docid;
	a->title = (char *)malloc(1);
	a->link = (char *)malloc(1);
	a->description = (char *)malloc(1);
	*a->title = '\0';
	*a->link = '\0';
	*a->description = '\0';
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
 * Update article fields. 
 * Because expat CharacterDataHandler will call this function more than 
 * one times to concatenate a bunch of strings.
 * Each field is NON null-terminated.
 */
void update_article(article *a, const char *data, const int size, rsstag tag) {
	if (isallspace(data, size) || isenter(data, size)) return;
	char **to_update = NULL;
	switch (tag) {
		case rss_title: 
			to_update = &a->title;
			break;
		case rss_link:
			to_update = &a->link;
			break;
		case rss_description:
			to_update = &a->description;
			break;
		default: break;
	}
	if (to_update != NULL) {
		int oldlen = strlen(*to_update);
		char *temp = (char *)realloc(*to_update, oldlen + size + 1);
		strncat(temp + oldlen, data, size);
		strncat(temp + oldlen + size, "\0", 1);
		*to_update = temp;
	}
}

/**
 * HashSetHashFunction<article>
 */
static int hash_article(const void *elemAddr, int numBuckets) {
	return (int)(((article *)elemAddr)->id % numBuckets);
}

/**
 * HashSetCompareFunction<article>
 */
static int comp_article(const void *elemAddr1, const void *elemAddr2) {
	return (int)(((article *)elemAddr1)->id - ((article *)elemAddr2)->id);
}

/**
 * VectorFreeFunction<article>
 * HashSetFreeFunction<article>
 */
void dispose_article(void *elemAddr) {
	article *a = (article *)elemAddr;
	free(a->title);
	free(a->link);
	free(a->description);
	a->title = NULL;
	a->link = NULL;
	a->description = NULL;
}

/**
 * Have to free memory after use the returned string.
 */
char *article_tostring(article *a) {
	char *buffer = (char *)malloc(64 + strlen(a->title) + strlen(a->link) + strlen(a->description)); 
	char *start = buffer;
	strcpy(start, "[id]:");
	start += 5;
	snprintf(start, 20, "%019ld", a->id); // docid always has 19 digits
	start += 19;
	strcpy(start, "\n[title]:");
	start += 9;
	strcpy(start, a->title);
	start += strlen(a->title);
	strcpy(start, "\n[link]:");
	start += 8;
	strcpy(start, a->link);
	start += strlen(a->link);
	strcpy(start, "\n[description]:");
	start += 15;
	strcpy(start, a->description);	
	start += strlen(a->description);
	strcpy(start, "\n\0");	
	return buffer;
}

/**
 * HashSetMapFunction<article>
 * VectorMapFunction<article>
 */
void print_article(void *elemAddr, void *auxData) {
	char *str = (article_tostring((article *)elemAddr));
	fprintf((FILE *)auxData, "%s\n", str);
	free(str);
}

/**
 * Constructor
 */
static const size_t kArticlesBucketNumber = 1024;
void new_articles(articles *a) {
	a->table = (hashset *)malloc(sizeof(hashset));
	HashSetNew(a->table, sizeof(article), kArticlesBucketNumber, hash_article, comp_article, dispose_article);
}

/**
 * Insert new element
 */
void add_article(articles *as, article *a) {
	HashSetEnter(as->table, a);
}

/**
 * Search article in article table by docid.
 */
article *search_article(const articles *a, const long docid) {
	article phantom;
	new_article_with_docid(&phantom, docid);
	return (article *)HashSetLookup(a->table, &phantom);
	dispose_article(&phantom);
}

void print_articles(articles *as, FILE * outfile) {
	HashSetMap(as->table, print_article, outfile);
} 

/**
 * Free memory
 */
void dispose_articles(articles *a) {
	HashSetDispose(a->table);
	free(a->table);
	a->table = NULL;
}



