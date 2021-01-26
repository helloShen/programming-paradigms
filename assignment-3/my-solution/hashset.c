#include "hashset.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Leave it here for debugging
 */
// static void PrintFreq(const void *elemAddr, void *auxData) {
// 	FILE *fp = (FILE *)auxData;
// 	char *ch = (char *)elemAddr;
// 	int *freq = (int *)((char *)elemAddr + 1);
// 	fprintf(fp, "frequence of [%c] = %d\n", *ch, *freq);
// 	fflush(fp);
// }

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn) {
	assert(elemSize > 0 && numBuckets > 0 && hashfn != NULL && comparefn != NULL);
	h->buckets = (vector *)malloc(numBuckets * sizeof(vector));
	assert(h->buckets != NULL);
	for (int i = 0; i < numBuckets; i++) {
		vector *bucket = (vector *)((char *)h->buckets + i * sizeof(vector));
		VectorNew(bucket, elemSize, freefn, 4);
	}
	h->bucketNum = numBuckets;
	h->elemSize = elemSize;
	h->elemNum = 0;
	h->hashfn = hashfn;
	h->compfn = comparefn;
	h->freefn = freefn;
}

void HashSetDispose(hashset *h) {
	for (int i = 0; i < h->bucketNum; i++) {
		vector *bucket = (vector *)((char *)h->buckets + i * sizeof(vector));
		VectorDispose(bucket);
	}
	free(h->buckets);
}

int HashSetCount(const hashset *h) {
	return h->elemNum; 
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData) {
	for (int i = 0; i < h->bucketNum; i++) {
		vector *bucket = (vector *)((char *)h->buckets + i * sizeof(vector));
		VectorMap(bucket, mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr) {
	int hash = h->hashfn(elemAddr, h->bucketNum);
	vector *bucket = (vector *)((char *)h->buckets + hash * sizeof(vector));
	// search in the hash set first
	int pos = VectorSearch(bucket, elemAddr, h->compfn, 0, false);
	if (pos != -1) {
		// replace the old one if find a match
		VectorReplace(bucket, elemAddr, pos);
	} else {
		// otherwise insert the new one
		VectorAppend(bucket, elemAddr);
	}
}

void *HashSetLookup(const hashset *h, const void *elemAddr) {
	int hash = h->hashfn(elemAddr, h->bucketNum);
	vector *bucket = (vector *)((char *)h->buckets + hash * sizeof(vector));
	int idx = VectorSearch(bucket, elemAddr, h->compfn, 0, false);
	return (idx != -1)? ((char *)VectorNth(bucket, 0) + idx * h->elemSize) : NULL;
}

