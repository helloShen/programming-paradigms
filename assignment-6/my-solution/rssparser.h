#ifndef _RSSPARSER_
#define _RSSPARSER_

#include "vector.h"
#include "data.h"

/**
 * articles is vector<article>
 * data is [char *stream, size_t size] pair
 * ----------------------------------------
 * curlconn.c dump_url() function download remote rss xml file 
 * into memory, return type "data". Our parser use this data 
 * as input, and generate vector of articles from it.
 */
void parserss(vector *articles, data *rssdata);
#endif