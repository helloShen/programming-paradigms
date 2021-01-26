#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>


/**
 * VectorMapFunction passed to VectorMap
 */
static void PrintChar(void *elem, void *fp) {
	fprintf((FILE *)fp, "%c", *(char *)elem);
	fflush((FILE *)fp);
}

/**
 * Call VectorMap to print the entire vector to stdout
 */
static void PrintVector(vector *v) {
	VectorMap(v, PrintChar, stdout);
	fprintf(stdout, "\n");
}

/**
 * VectorCompareFunction
 */
static int CompareChar(const void *elemA, const void *elemB) {
	return (*(char *)elemA - *(char *)elemB);
}

static void TestAppend(vector *v) {
	char ch;
	int i;
	for (ch = 'Z'; ch >= 'A'; ch--) {
		VectorAppend(v, &ch);
	}
	fprintf(stdout, "First, here is the alphabet: ");
	PrintVector(v);
	for (i = 9; i >= 0; i--) {
		char n = '0' + i;
		VectorAppend(v, &n);
	}
	fprintf(stdout, "Now, here comes digits: ");
	PrintVector(v);	
}

static void TestSortSearch(vector *v) {
	VectorSort(v, CompareChar);
	fprintf(stdout, "After sort: ");
	PrintVector(v);
	char c = 'J';
	fprintf(stdout, "Search for %c --> @%d\n", c, VectorSearch(v, &c, CompareChar, 0, true));
	c = '$';
	fprintf(stdout, "Search for %c --> @%d\n", c, VectorSearch(v, &c, CompareChar, 0, true));
}

static void TestAt(vector *v) {
	int i;
	for (i = 0; i < VectorLength(v); i += 2) {
		char *elem = VectorNth(v, i);
		*elem = tolower(*elem);
	}
	fprintf(stdout, "After lowercase every other letter: ");
	PrintVector(v);
}

static void TestInsertDelete(vector *v) {
	char dash = '-';
	int i;
	for (i = 3; i < VectorLength(v); i+=4) {
		VectorInsert(v, &dash, i);	
	}
	fprintf(stdout, "After insert dashes: ");
	PrintVector(v);
	for (i = 3; i < VectorLength(v); i+=3) {
		VectorDelete(v, i);
	}
	fprintf(stdout, "After deleting dashes: ");
	PrintVector(v);
}

static void TestReplace(vector *v) {
	int found = 0;
	char toFind = 's', toReplace = '*';
	while ((found = VectorSearch(v, &toFind, CompareChar, found, false)) != -1) {
		VectorReplace(v, &toReplace, found);
	}
	fprintf(stdout, "After replace: ");
	PrintVector(v);
}

static void SimpleTest(void) {
	fprintf(stdout, "--------------------------- Starting the basic test...\n");	
	vector alphabet;
	VectorNew(&alphabet, sizeof(char), NULL, 4);
	TestAppend(&alphabet);
	TestSortSearch(&alphabet);
	TestAt(&alphabet);
	TestInsertDelete(&alphabet);
	TestReplace(&alphabet);
	VectorDispose(&alphabet);
}

static void InsertPermutationOfNumber(vector *numbers, long n, long d) {
	long k, residue;
	fprintf(stdout, "Generating all of the numbers between 0 and %ld (using some number theory). \n", d - 1);
	fflush(stdout); // force echo to the screen... 
	for (k = 0; k < d; k++) {
		residue = (long)((long long)k * (long long)n % d);
		VectorAppend(numbers, &residue);
		// fprintf(stdout, "append [%ld], vector lengh = %d, vector capacity = %d \n", residue, VectorLength(numbers), numbers->capacity);
		// fflush(stdout);
	}
	assert(VectorLength(numbers) == d);
	fprintf(stdout, "[All done]\n");
	fflush(stdout);
}

static int LongCompare(const void *a, const void *b) {
	long la = *(long *)a;
	long lb = *(long *)b;
	if (la < lb) {
		return -1;
	} else if (la > lb) {
		return 1;
	} else {
		return 0;
	}
}

static void SortPermutation(vector *vectorToSort)
{
    long residue, embeddedLong, lastEmbeddedLong;
    vector *sortedVector;
    fprintf(stdout, "Sorting all of those numbers. ");
    fflush(stdout);
    VectorSort(vectorToSort, LongCompare);
    fprintf(stdout, "[Done]\n");
    fflush(stdout);
    fprintf(stdout, "Confirming everything was properly sorted. ");
    fflush(stdout);
    sortedVector = vectorToSort; // need better name now that it's sorted... 
    for (residue = 0, lastEmbeddedLong = -1; residue < VectorLength(sortedVector); residue++) {
        embeddedLong = *(const long *) VectorNth(sortedVector, residue);
        // fprintf(stdout, "residue = %ld, embeddedLong = %ld\n", residue, embeddedLong);
        assert(embeddedLong > lastEmbeddedLong);
        lastEmbeddedLong = embeddedLong;
    }
    fprintf(stdout, "[Yep, it's sorted]\n");
    fflush(stdout);
}

/**
 * Function: DeleteEverythingVerySlowly
 * ------------------------------------
 * Empties out the vector in such a way that VectorDelete
 * is exercised to the hilt.  By repeatedly deleting from
 * within the vector, we ensure that the shifting over of
 * bytes is working properly.
 */

static void DeleteEverythingVerySlowly(vector *numbers)
{
    long largestOriginalNumber;
    fprintf(stdout, "Erasing everything in the vector by repeatedly deleting the 100th-to-last remaining element (be patient).\n");
    fflush(stdout);
    largestOriginalNumber = *(long *)VectorNth(numbers, VectorLength(numbers) - 1);
    while (VectorLength(numbers) >= 100) {
        VectorDelete(numbers, VectorLength(numbers) - 100);
        assert(largestOriginalNumber == *(long *)VectorNth(numbers, VectorLength(numbers) -1));
    }
    fprintf(stdout, "\t[Okay, almost done... deleting the last 100 elements... ");
    fflush(stdout);
    while (VectorLength(numbers) > 0) VectorDelete(numbers, 0);
    fprintf(stdout, "and we're all done... whew!]\n");
    fflush(stdout);
}

/**
 * Function: ChallengingTest
 * -------------------------
 * Uses a little bit of number theory to generate a very large vector
 * of four-byte values.  Some permutation of the numbers [0, 3021367)
 * is generated, and in the process the vector grows in such a way that
 * several realloc calls are likely made.  This will catch any errors
 * with the reallocation, particulatly those where the implementation
 * fails to catch realloc's return value.  The test then goes on the
 * sort the array, confirm that the sort succeeded, and then finally
 * delete all of the elements one by one.
 */
static const long kLargePrime = 1398269;
static const long kEvenLargerPrime = 3021377;
static void ChallengingTest(void) {
  	fprintf(stdout, "\n\n------------------------- Starting the more advanced tests...\n");  
  	vector lotsOfNumbers;
  	VectorNew(&lotsOfNumbers, sizeof(long), NULL, 4);
  	InsertPermutationOfNumber(&lotsOfNumbers, kLargePrime, kEvenLargerPrime);
  	SortPermutation(&lotsOfNumbers);
	DeleteEverythingVerySlowly(&lotsOfNumbers);
	VectorDispose(&lotsOfNumbers); 	
}

static void FreeString(void *elemAddr) {
	char *str = *(char **)elemAddr;
	free(str);
}

static void PrintString(void *elemAddr, void *auxData) {
	char *word = *(char **)elemAddr;
	FILE *fp = (FILE *)auxData;
	fprintf(fp, "\t[%s]", word);
	fflush(fp);
}

/**
 * Function: MemoryTest
 * --------------------
 * MemoryTest exercises the vector functionality by
 * populating one with pointers to dynamically allocated
 * memory.  The insertion process marks the transfer of
 * of responsibility from the client to the vector, so
 * we now need to specify a non-NULL VectorFreeFunction so
 * the a vector can apply it to the elements it inherits
 * from the client.  Make sure you understand why the
 * casts within the two functions above (FreeString, PrintString)
 * are char ** casts and not char *.  If you truly understand,
 * they you've learned what is probably the most difficult-to-
 * learn concept taught in CS107.
 */

static void MemoryTest()
{
    int i;
    const char * const kQuestionWords[] = {"who", "what", "where", "how", "why"};
    const int kNumQuestionWords = sizeof(kQuestionWords) / sizeof(kQuestionWords[0]);
    vector questionWords;
    char *questionWord;
    
    fprintf(stdout, "\n\n------------------------- Starting the memory tests...\n");
    fprintf(stdout, "Creating a vector designed to store dynamically allocated C-strings.\n");
    VectorNew(&questionWords, sizeof(char *), FreeString, kNumQuestionWords);
    fprintf(stdout, "Populating the char * vector with the question words.\n");
    for (i = 0; i < kNumQuestionWords; i++) {
        questionWord = malloc(strlen(kQuestionWords[i]) + 1);
        strcpy(questionWord, kQuestionWords[i]);
        VectorInsert(&questionWords, &questionWord, 0);  // why the ampersand? isn't questionWord already a pointer?
    }
    fprintf(stdout, "Mapping over the char * vector (ask yourself: why are char **'s passed to PrintString?!!)\n");
    VectorMap(&questionWords, PrintString, stdout);
    fprintf(stdout, "\nFinally, destroying the char * vector.\n");
    fflush(stdout);
    VectorDispose(&questionWords);
}

int main(void) {
	SimpleTest();
	ChallengingTest();
	MemoryTest();
}