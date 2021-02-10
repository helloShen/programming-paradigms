

/**
 * Suppose input file is well formed xml in rss format.
 * ----------------------------------------------------
 * Expat Document
 *   https://www.ghostscript.com/doc/expat/doc/reference.html
 *	 https://svn.apache.org/repos/asf/apr/apr-util/vendor/expat/1.95.7/doc/reference.html
 */
#include "rsstag.h"
#include "vector.h"
#include "hashset.h"
#include "article.h"
#include "data.h"
#include <expat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static rsstag itemtag;
static rsstag blocktag;
static article local_article;
static vector *local_articles; // share articles access with handlers

static void init_workingspace(vector *articles) {
	itemtag = null_tag;
	blocktag = null_tag;
	local_articles = articles; // share articles access with handlers
}

static void start_handler(void *data, const char *el, const char **attr) {
	if (strcmp(el, "item") == 0) {
		itemtag = rss_item;
		new_article(&local_article);
		return;
	}
	if (itemtag != rss_item) return;
	if (strcmp(el, "title") == 0) {
		blocktag = rss_title;
	} else if (strcmp(el, "link") == 0) {
		blocktag = rss_link;
	} else if (strcmp(el, "description") == 0) {
		blocktag = rss_description;
	} else {
		blocktag = null_tag;
	}
}

static void end_handler(void *data, const char *el) {
	if (strcmp(el, "item") == 0) {
		VectorAppend(local_articles, &local_article);
		itemtag = null_tag;
		blocktag = null_tag;
	}
}

static void data_handler(void *data, const char *txt, int txtlen) {
	if (itemtag == rss_item && blocktag != null_tag) {
		append_article(&local_article, txt, txtlen, blocktag);
	}
}

/**
 * curlconn.c dump_url() function download remote rss xml file 
 * into memory, return type "data". Our parser use this data 
 * as input, and generate vector of articles from it.
 */
void parserss(vector *articles, data *rssdata) {
	init_workingspace(articles);
	XML_Parser p = XML_ParserCreate(NULL);
	assert(p);
	XML_SetElementHandler(p, start_handler, end_handler);
	XML_SetCharacterDataHandler(p, data_handler);
	XML_Parse(p, rssdata->stream, rssdata->size, true);
	XML_ParserFree(p);
}



