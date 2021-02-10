#include "data.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/**
 * Constructor
 */
void new_data(data *d)  {
	d->stream = (char *)malloc(1);
	d->stream[0] = '\0';
	d->size = 0;
}

/**
 * data->stream is always null-terminated.
 * Input stream "*ptr" don't have to be null-terminated.
 */
static const char kDataNullTerminator = '\0';
void append_data(data *d, const char *ptr, const size_t size) {
	char *temp = (char *)realloc(d->stream, d->size + size + 1);
	assert(temp != NULL);
	strncat(temp, ptr, size);
	strncat(temp, &kDataNullTerminator, 1);	
	d->stream = temp;	
	d->size += (size + 1);
}

/**
 * Free memory
 */
void dispose_data(data *d) {
	free(d->stream);	
	d->stream = NULL;
}