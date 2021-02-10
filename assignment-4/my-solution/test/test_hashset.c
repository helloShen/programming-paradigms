#include "hashset.h"
#include "testutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * void HashSetEnter(hashset *h, const void *elemAddr);
 * int HashSetCount(const hashset *h);
 * void HashSetDispose(hashset *h);
 */
static void test_HashSetEnterCountDispose(void) {
	tstart("hashset::HashSetEnter()::HashSetCount()::HashSetDispose()");

	hashset h;
	HashSetNew(&h, sizeof(mock_freq), 4, t_hash_freq, t_comp_freq, t_dispose_freq);
	mock_freq apple1;
	t_new_freq(&apple1, "apple", 3);
	HashSetEnter(&h, &apple1);
	assert(HashSetCount(&h) == 1);
	mock_freq apple2;
	t_new_freq(&apple2, "apple", 2);
	HashSetEnter(&h, &apple2);
	assert(HashSetCount(&h) == 1);
	mock_freq banana;
	t_new_freq(&banana, "banana", 5);
	HashSetEnter(&h, &banana);
	assert(HashSetCount(&h) == 2);
	HashSetDispose(&h);
	assert(h.buckets == NULL);

	tpass();
}

/**
 * void *HashSetLookup(const hashset *h, const void *elemAddr);
 */
static void test_HashSetLookup(void) {
	tstart("hashset::HashSetLookup()");

	hashset h;
	HashSetNew(&h, sizeof(mock_freq), 4, t_hash_freq, t_comp_freq, t_dispose_freq);
	mock_freq apple1;
	t_new_freq(&apple1, "apple", 3);
	mock_freq apple2;
	t_new_freq(&apple2, "apple", 1);
	mock_freq banana;
	t_new_freq(&banana, "banana", 2);
	HashSetEnter(&h, &apple1);
	assert(HashSetLookup(&h, &apple2) != NULL);
	assert(HashSetLookup(&h, &banana) == NULL);

	HashSetDispose(&h);
	t_dispose_freq(&apple2);
	t_dispose_freq(&banana);

	tpass();
}

/**
 * void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData);
 */
static void test_HashSetMap(void) {
	tstart("hashset::HashSetMap()");

	hashset h;
	HashSetNew(&h, sizeof(mock_freq), 4, t_hash_freq, t_comp_freq, t_dispose_freq);
	mock_freq apple;
	t_new_freq(&apple, "apple", 3);
	mock_freq banana;
	t_new_freq(&banana, "banana", 2);
	mock_freq kiwi;
	t_new_freq(&kiwi, "kiwi", 5);
	HashSetEnter(&h, &apple);
	HashSetEnter(&h, &banana);
	HashSetEnter(&h, &kiwi);
	char *buffer = (char *)malloc(1);
	buffer[0] = '\0';
	HashSetMap(&h, t_freq_maptostring, &buffer);
	assert(strlen(buffer) == 33); 
	assert(strcmp(buffer, "[kiwi, 5]\n[banana, 2]\n[apple, 3]\n") == 0);
	HashSetDispose(&h);
	free(buffer);

	tpass();
}

int main(void) {
	test_HashSetEnterCountDispose();
	test_HashSetLookup();
	test_HashSetMap();
}