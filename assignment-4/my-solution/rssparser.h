#ifndef _rssparser_
#define _rssparser_

#include "vector.h"
#include "curlconn.h"
#include <stdio.h>

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