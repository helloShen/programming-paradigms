#include <stdio.h>

#ifndef _DATA_
#define _DATA_
/**
 * output of dump_url() function
 */
typedef struct {
	char *stream;
	size_t size;
} data;

/**
 * Constructor
 */
void new_data(data *d);

/**
 * Free memory
 */
void dispose_data(data *d);
#endif

/**
 * Request web page by url and write in a local file.
 */
void download_url(const char *url, FILE *fp);

/**
 * Request the web page by url, and store entire data stream into a string buffer.
 */
void dump_url(const char *url, data *buff);

