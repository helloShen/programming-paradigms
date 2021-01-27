/**
 * title, link and descritption are NULL terminated.
 */
#include "article.h"
#include "rsstag.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char SPACE = ' ';

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

void new_article(article *a) {
	a->id = getnano();
	vector *v_title = (vector *)malloc(sizeof(vector));
	vector *v_link = (vector *)malloc(sizeof(vector));
	vector *v_description = (vector *)malloc(sizeof(vector));
	VectorNew(v_title, sizeof(char), NULL, 128);
	VectorNew(v_link, sizeof(char), NULL, 128);
	VectorNew(v_description, sizeof(char), NULL, 256);
	a->title = *v_title;
	a->link = *v_link;
	a->description = *v_description;
}

void add_data(article *a, const char *data, const int size, rsstag tag) {
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
		VectorAppend(toAppend, &SPACE);
	}
}

void dispose_article(article *a) {
	VectorDispose(&a->title);
	VectorDispose(&a->link);
	VectorDispose(&a->description);
}

static void mapchar(void *elemAddr, void *auxData) {
	FILE *fp = (FILE *)auxData;
	fprintf(fp, "%c", *(char *)elemAddr);
	fflush(fp);
}

void map_article(article *a, FILE *out) {
	fprintf(out, "Article:[%ld]\n", a->id);
	fprintf(out, "%s: ", "[title]");
	VectorMap(&a->title, mapchar, out);
	fprintf(out, "%s: ", "[link]");
	VectorMap(&a->link, mapchar, out);
	fprintf(out, "%s: ", "[description]");
	VectorMap(&a->description, mapchar, out);
}

