#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** ==================================== VECTOR ======================================= */

typedef enum {
	true, false
} bool;

typedef int (*VectorCompareFunction)(const void *elemAddr1, const void *elemAddr2);
typedef void (*VectorFreeFunction)(void *elemAddr);

typedef struct {
	int elemSize;				//how many byte for each element
	int elemNum;				//number of current element in vector
	int capacity;				//maximum number of element vector can hold
	void *elems;				//pointer to data memory
	VectorFreeFunction freefn;	//pointer to the function used to free each element
} vector;

/**
 * Reallocate a new memory of twice of original size
 * return 1 if reallocation success, otherwise return -1.
 */
static void DoubleMemory(vector *v) {
	void *tmp = realloc(v->elems, v->capacity * v->elemSize * 2);
	assert(tmp != NULL);
	v->elems = tmp;
	v->capacity *= 2;
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
 * Appends a new element to the end of the specified vector.
 */
void VectorAppend(vector *v, const void *elemAddr) {
	/* double size if neccessary */
	if (v->elemNum == v->capacity) DoubleMemory(v);
	memcpy((char *)v->elems + v->elemNum * v->elemSize, elemAddr, v->elemSize);
	v->elemNum++;
}

/**
 * Search the specified vector for an element whose contents match the element passed as the key.
 */
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchfn, int startIndex, bool isSorted) {
	assert(key && searchfn);
	if (v->elemNum == 0) return -1;
	assert(startIndex >= 0 && startIndex < v->elemNum);
	if (isSorted == true) {
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
 * Overwrites the element at the specified position.
 */
void VectorReplace(vector *v, const void *elemAddr, int position) {
	assert(position >= 0 && position < v->elemNum);
	void *posAddr = (char *)v->elems + position * v->elemSize;
	/* free the memory of old element first */
	if (v->freefn != NULL) v->freefn(posAddr);
	memcpy(posAddr, elemAddr, v->elemSize);
}





/** ==================================== HASHSET ======================================= */

typedef int (*HashSetHashFunction)(const void *elemAddr, int numBuckets);
typedef int (*HashSetCompareFunction)(const void *elemAddr1, const void *elemAddr2);
typedef void (*HashSetFreeFunction)(void *elemAddr);

typedef struct {
	int elemNum;			//current element number
	int bucketNum;			//number of hash bucket
	int elemSize;			//how many byte each element has
	vector *buckets;		//array of vector
	HashSetHashFunction hashfn;
	HashSetCompareFunction compfn;
	HashSetFreeFunction freefn;
} hashset;

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
		h->elemNum++;
	}
}




/** ==================================== DOC_FREQ & WORD_INDEX ======================================= */

/****************************************************************
 *
 * doc_freq is a key-value pair of [documentid, frequency]
 * It's not supposed to be exposed to user or search engine.
 * -----------------------------------------------------------
 * It looks like:
 *		[1611742826915764000]	[4      ]  
 *		|-------------------|  	|-------|
 *		 docid 					 freq
 ***************************************************************/
typedef struct {
	long docid;
	int freq;
} doc_freq;

static void new_docfreq(doc_freq *df, long docid, int freq) {
	df->docid = docid;
	df->freq = freq;
}

/**
 * HashSetHashFunction<doc_freq>
 */
static int hash_docfreq(const void *elemAddr, int numBuckets) {
	doc_freq *df = (doc_freq *)elemAddr;
	return (int)(df->docid % numBuckets);
}

/**
 * HashSetCompareFunction<doc_freq>
 */
static int comp_docfreq(const void *elemAddr1, const void *elemAddr2) {
	long id1 = ((doc_freq *)elemAddr1)->docid;
	long id2 = ((doc_freq *)elemAddr2)->docid;
	if (id1 < id2) {
		return -1;
	} else if (id1 > id2) {
		return 1;
	} else { // id1 == id2
		return 0;
	}
}

/**
 * word_index is a index of a single word.
 * ---------------------------------------
 * A typical word_index looks like:
 * 		[apple]: [doc1, 5], [doc3, 10], [doc5, 7]
 *		|-----|  |------------------------------|
 *		 word     freqs
 */
typedef struct {
	char *word;
	hashset *freqs; // hashset<doc_freq>
} word_index;

static const size_t kWordIndexHashSetBuckets = 64;
static void new_wordindex(word_index *wi, const char *word) {
	hashset *h = (hashset *)malloc(sizeof(hashset));
	HashSetNew(h, sizeof(doc_freq), kWordIndexHashSetBuckets, hash_docfreq, comp_docfreq, NULL);
	wi->freqs = h;
	size_t wordlen = strlen(word);
	wi->word = (char *)malloc(wordlen + 1); // +1 for null-termination
	strcpy(wi->word, word);
	(wi->word)[wordlen] = '\0';
}

/**
 * Mainly used to build a word_index.
 */
void add_docfreq(word_index *wi, const long docid, const int frequency) {
	doc_freq df;
	new_docfreq(&df, docid, frequency);
	HashSetEnter(wi->freqs, &df);
}





/** ==================================== UNIT-TEST ======================================= */

int main(void) {
	/* apple:	[1611742826915764000, 5][1611742826915538000, 10] */
	word_index *apple = (word_index *)malloc(sizeof(word_index));
	new_wordindex(apple, "apple");
	add_docfreq(apple, 1611742826915764000L, 5);
	add_docfreq(apple, 1611742826915538000L, 10);
}