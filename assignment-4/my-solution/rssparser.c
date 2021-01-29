/**
 * Suppose input file is well formed xml in rss format.
 */
#include "article.h"
#include "rsstag.h"
#include "vector.h"
#include "hashset.h"
#include <expat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define ITEM "item"
#define TITL "title"
#define LINK "link"
#define DESC "description"

static rsstag itemtag;
static rsstag blocktag;
static article local_article;
static vector articles;
	
// static void DefaultHandler(void *data, const char *s, int len) {
//   fwrite(s, len, sizeof(char), stdout);
// }

// static void printcurrent(XML_Parser p) {
//   XML_SetDefaultHandler(p, DefaultHandler);
//   XML_DefaultCurrent(p);
//   XML_SetDefaultHandler(p, (XML_DefaultHandler) 0);
// } 

static void start_handler(void *data, const char *el, const char **attr) {
	if (strcmp(el, ITEM) == 0) {
		itemtag = rss_item;
		new_article(&local_article);
		return;
	}
	if (itemtag != rss_item) return;
	if (strcmp(el, TITL) == 0) {
		blocktag = rss_title;
	} else if (strcmp(el, LINK) == 0) {
		blocktag = rss_link;
	} else if (strcmp(el, DESC) == 0) {
		blocktag = rss_description;
	} else {
		blocktag = null_tag;
	}
}

static void end_handler(void *data, const char *el) {
	if (strcmp(el, ITEM) == 0) {
		VectorAppend(&articles, &local_article);
		itemtag = null_tag;
		blocktag = null_tag;
	}
}

static void data_handler(void *data, const char *txt, int txtlen) {
	if (itemtag == rss_item && blocktag != null_tag) {
		add_data(&local_article, txt, txtlen, blocktag);
	}
}

static void free_article(void *a) {
	dispose_article((article *)a);
}

static const size_t kRssParserLineBuffSize = 2048;
vector parserss(FILE *in) {
	assert(in);
	char buff[kRssParserLineBuffSize];
	itemtag = null_tag;
	blocktag = null_tag;
	VectorNew(&articles, sizeof(article), free_article, 4);
	XML_Parser p = XML_ParserCreate(NULL);
	assert(p);
	XML_SetElementHandler(p, start_handler, end_handler);
	XML_SetCharacterDataHandler(p, data_handler);
	while (fgets(buff, sizeof(buff), in)) {
		XML_Parse(p, buff, strlen(buff), feof(in));
	}
	return articles;
}

