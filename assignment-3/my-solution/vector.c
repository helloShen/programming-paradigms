/**
 * Simple container - vector
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"


/**
 * Reallocate a new memory of twice of original size
 * return 1 if reallocation success, otherwise return -1.
 */
static void DoubleMemory(vector *v) {
	void *tmp = realloc(v->elems, v->capacity * v->elemSize * 2);
	assert(tmp != NULL);
	v->elems = tmp;
	v->capacity *= 2;
	// fprintf(stdout, "Vector space expand to %d.\n", v->capacity);
	// fflush(stdout);
}

/**
 * Constructor
 */
void VectorNew(vector *v, int elemSize, VectorFreeFunction freefn, int initialAllocation) {
	v->elems = malloc(initialAllocation * elemSize);
	assert(v->elems != NULL);
	v->elemSize = elemSize;
	v->elemNum = 0;
	v->capacity = initialAllocation;
	v->freefn = freefn;
}

/**
 * Frees up all the memory of the specified vector.
 */
void VectorDispose(vector *v) {
	if (v->freefn != NULL) {
		for (; v->elemNum > 0; v->elemNum--) {
			void *elemAddr = (char *)v->elems + (v->elemNum - 1) * v->elemSize;
			//printf("to free element@%p\n", *(char *)elemAddr);
			v->freefn(elemAddr);
		}
	}
	free(v->elems);
}

/**
 * Returns the logical length of the vector.
 */
int VectorLength(const vector *v) {
	return v->elemNum;	
}

/**
 * Returns the pointer to the Nth element in the specified vector.
 */
void *VectorNth(const vector *v, int position) {
	assert(position >= 0 && position < v->elemNum);
	return (char *)v->elems + position * v->elemSize;
}

/**
 * Inserts a new element into the vector to a specified position.
 */
void VectorInsert(vector *v, const void *elemAddr, int position) {
	assert(position >= 0 && position <= v->elemNum);	
	/* double size if neccessary */
	if (v->elemNum == v->capacity) DoubleMemory(v);
	void *posAddr = (char *)v->elems + position * v->elemSize;
	/* move following elements to the left by one element size */
	memmove((char *)posAddr + v->elemSize, posAddr, (v->elemNum - position) * v->elemSize);
	memcpy(posAddr, elemAddr, v->elemSize);
	v->elemNum++;
}

/**
 * Appends a new element to the end of the specified vector.
 */
void VectorAppend(vector *v, const void *elemAddr) {
	/* double size if neccessary */
	if (v->elemNum == v->capacity) DoubleMemory(v);
	memcpy((char *)v->elems + v->elemNum * v->elemSize, elemAddr, v->elemSize);
	v->elemNum++;
}

/**
 * Overwrites the element at the specified position.
 */
void VectorReplace(vector *v, const void *elemAddr, int position) {
	assert(position >= 0 && position < v->elemNum);
	void *posAddr = (char *)v->elems + position * v->elemSize;
	/* free the memory of old element first */
	if (v->freefn != NULL) v->freefn(posAddr);
	memcpy(posAddr, elemAddr, v->elemSize);
}

/**
 * Deletes the element at the specified position.
 */
void VectorDelete(vector *v, int position) {
	assert(position >= 0 && position < v->elemNum);
	void *posAddr = (char *)v->elems + position * v->elemSize;
	/* free the memory first */
	if (v->freefn != NULL) v->freefn(posAddr);
	memmove(posAddr, (char *)posAddr + v->elemSize, (v->elemNum - position) * v->elemSize);
	v->elemNum--;
}

/**
 * Search the specified vector for an element whose contents match the element passed as the key.
 */
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchfn, int startIndex, bool isSorted) {
	assert(key && searchfn);
	if (v->elemNum == 0) return -1;
	assert(startIndex >= 0 && startIndex < v->elemNum);
	if (isSorted) {
		/* binary search */
		void *startAddr = (char *)v->elems + startIndex * v->elemSize;
		int size = v->elemNum - startIndex;
		void *resAddr = bsearch(key, startAddr, size, v->elemSize, searchfn);
		return (resAddr != NULL)? ((char *)resAddr - (char *)v->elems) / v->elemSize : -1;
	} else {
		/* linear search */
		for (int i = 0; i < v->elemNum; i++) {
			if (searchfn((char *)v->elems + i * v->elemSize, key) == 0) {
				return i;
			}
		}
		return -1;
	}
}

/**
 * Sorts the vector into ascending order according to the supplied comparator.
 */
void VectorSort(vector *v, VectorCompareFunction searchfn) {
	assert(searchfn);
	qsort(v->elems, v->elemNum, v->elemSize, searchfn);
}

/**
 * Output the entire vector by calling the supplied VectorMapFunction.
 */
void VectorMap(vector *v, VectorMapFunction mapfn, void *auxData) {
	assert(mapfn);
	for (int i = 0; i < v->elemNum; i++) {
		mapfn((char *)v->elems + i * v->elemSize, auxData);
	} // No '\n' to the end. Caller can add it themself.
}
