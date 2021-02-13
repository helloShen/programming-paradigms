#include "../data.c"
#include "testutils.h"
#include <assert.h>
#include <stdio.h>

/**
 * void new_data(data *d);
 * void append_data(data *d, const char *ptr, const size_t size);
 * void dispose_data(data *d);
 */
static void test_data(void) {
	tstart("data::new_data()::append_data()::dispose_data()");

	data d;
	new_data(&d);
	char *str = "hello world!";
	append_data(&d, str, strlen(str));
	assert(strlen(d.stream) == strlen(str));
	assert(d.size == strlen(d.stream));
	dispose_data(&d);
	assert(d.stream == NULL);

	tpass();
}

int main(void) {
	test_data();
}
