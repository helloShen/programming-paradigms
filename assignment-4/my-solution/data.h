#ifndef _DATA_
#define _DATA_

#include <ctype.h>
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
 * data->stream is always null-terminated.
 */
void append_data(data *d, const char *ptr, const size_t size);

/**
 * Free memory
 */
void dispose_data(data *d);
#endif