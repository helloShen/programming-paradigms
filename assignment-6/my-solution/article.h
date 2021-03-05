/**
 * Contents in each <item> tag of rss news xml represent the information about a remote news page.
 * <item>
 * 		<title><![CDATA[Myanmar coup: Detained Aung San Suu Kyi faces charges]]></title>
 * 		<description><![CDATA[The elected civilian leader has not been heard from since she was detained on Monday.]]></description>
 * 		<link>https://www.bbc.co.uk/news/world-asia-55915354</link>
 * 		<guid isPermaLink="true">https://www.bbc.co.uk/news/world-asia-55915354</guid>
 * 		<pubDate>Wed, 03 Feb 2021 11:57:28 GMT</pubDate>
 * </item>
 *
 * We extract <title>, <link> and <description> fields, give it an id, and store in an
 * article structure shown as below.
 *
 * 		[id]:1612173563370986000
 * 		[title]: Myanmar coup: Aung San Suu Kyi detained as military seizes control
 * 		[link]: https://www.bbc.co.uk/news/world-asia-55882489
 * 		[description]: The army has seized power after alleging election fraud in the democratic vote held in November. 
 * 
 * Check "XML_CharacterDataHandler" in expat
 * --> https://www.ghostscript.com/doc/expat/doc/reference.html#XML_SetCharacterDataHandler
 * ----------------------------------------------------------------------------------------
 * A single block of contiguous text free of markup may still result in a sequence of 
 * calls to this handler. In other words, if you're searching for a pattern in the text, 
 * it may be split across calls to this handler. Note: Setting this handler to NULL may 
 * NOT immediately terminate call-backs if the parser is currently processing such a 
 * single block of contiguous markup-free text, as the parser will continue calling back 
 * until the end of the block is reached.
 */
#ifndef _article_
#define _article_

#include "rsstag.h"
#include "hashset.h"
#include <stdio.h>
#include <pthread.h>

typedef struct {
	long id;			// nanosecond scince unix epoch
	char *title;		// null-terminated
	char *link;			// null-terminated
	char *description;	// null-terminated
} article;

/* Constructor */
void new_article(article *a);

void new_article_with_docid(article *a, const long docid);

/** 
 * Update article fields. 
 * Because expat CharacterDataHandler will call this function more than 
 * one time to concatenate a bunch of strings.
 * Each field is NON null-terminated.
 */
void update_article(article *a, const char *title, int size, rsstag tag);

void print_article(void *elemAddr, void *auxData);

/**
 * VectorFreeFunction<article>
 * HashSetFreeFunction<article>
 */
void dispose_article(void *elemAddr);

#endif // _ARTICLE_



#ifndef _ARTICLES_
#define _ARTICLES_

/**
 * *table is hashset<article>
 */
typedef struct {
	hashset *table;
	pthread_mutex_t *lock;
} articles; 

/**
 * Constructor
 */
void new_articles(articles *a);

/**
 * Insert new element
 */
void add_article(articles *as, article *a);

/**
 * Search article in article table by docid.
 */
article *search_article(const articles *a, const long docid);

/**
 * Free memory
 */
void dispose_articles(articles *a);

#endif // _ARTICLES_