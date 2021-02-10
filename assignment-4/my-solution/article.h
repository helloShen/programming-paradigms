/**
 * Data container must be a vector of char. Because datas are aplit into a sequence of
 * short blocks.
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
#include "vector.h"
#include <stdio.h>

typedef struct {
	long id;		// nanosecond scince unix epoch
	vector title;
	vector link;
	vector description;
} article;

/* Constructor */
void new_article(article *a);

/** 
 * update one of the article field. 
 * it will delete the old one if it exists.
 */
void append_article(article *a, const char *title, int size, rsstag tag);

/* Free memory */
void dispose_article(void *elemAddr);

/* Print article */
void print_article(void *elemAddr, void *auxData);

/**
 * Copy each character of title field into a string buffer.
 */
void get_title(article *a, char *buff, size_t buffsize);

/**
 * Copy each character of link field into a string buffer.
 */
void get_link(article *a, char *buff, size_t buffsize);

/**
 * Copy each character of description field into a string buffer.
 */
void get_description(article *a, char *buff, size_t buffsize);
#endif