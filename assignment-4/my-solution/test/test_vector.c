#include "vector.h"
#include "testutils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>


/**
 * void VectorDispose(vector *v);
 */
static void test_VectorDispose(void) {
	tstart("vector::VectorDispose()");

	vector v;
	VectorNew(&v, sizeof(mock_freq), t_dispose_freq, 4);
	mock_freq apple;
	t_new_freq(&apple, "apple", 3);
	VectorAppend(&v, &apple);
	assert(VectorLength(&v) == 1);
	VectorDispose(&v);
	assert(v.elems == NULL);
	/* the copy of apple.word in vector is NULL, but not the original apple.word */
	assert(apple.word != NULL); 

	tpass();
}

/**
 * int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchfn, int startIndex, bool isSorted);
 */
static void test_VectorSearch(void) {
	tstart("vector::VectorSearch()");

	vector v;
	VectorNew(&v, sizeof(mock_freq), t_dispose_freq, 4);
	mock_freq apple1;
	t_new_freq(&apple1, "apple", 3);
	VectorAppend(&v, &apple1);
	assert(VectorLength(&v) == 1);
	assert(VectorSearch(&v, &apple1, t_comp_freq, 0, false) != -1);
	mock_freq apple2;
	t_new_freq(&apple2, "apple", 2);
	assert(VectorSearch(&v, &apple2, t_comp_freq, 0, false) != -1);
	mock_freq banana;
	t_new_freq(&banana, "banana", 5);
	assert(VectorSearch(&v, &banana, t_comp_freq, 0, false) == -1);

	VectorDispose(&v);
	t_dispose_freq(&apple2);
	t_dispose_freq(&banana);

	tpass();
}

/**
 * void VectorMap(vector *v, VectorMapFunction mapfn, void *auxData);
 */
static void test_VectorMap(void) {
	tstart("vector:VectorMap()");

	vector v;
	VectorNew(&v, sizeof(mock_freq), t_dispose_freq, 4);
	mock_freq apple;
	t_new_freq(&apple, "apple", 3);
	mock_freq banana;
	t_new_freq(&banana, "banana", 2);
	mock_freq kiwi;
	t_new_freq(&kiwi, "kiwi", 5);
	VectorAppend(&v, &apple);
	VectorAppend(&v, &banana);
	VectorAppend(&v, &kiwi);
	char *buffer = (char *)malloc(1);
	buffer[0] = '\0';
	VectorMap(&v, t_freq_maptostring, &buffer);
	assert(strlen(buffer) == 33); 
	assert(strcmp(buffer, "[apple, 3]\n[banana, 2]\n[kiwi, 5]\n") == 0);

	tpass();
}

int main(void) {
	test_VectorDispose();
	test_VectorSearch();
	test_VectorMap();
}
