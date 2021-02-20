#include "strhash.h"
#include <string.h>
#include <ctype.h>

/**
 * This function adapted from Eric Roberts' "The Art and Science of C"
 */
static const signed long kHashMultiplier = -1664117991L;
int string_hash(const char *s, const int numBuckets) {
	unsigned long hashcode = 0;
	for (int i = 0; i < strlen(s); i++) {
	    hashcode = hashcode * kHashMultiplier + tolower(s[i]);  
	}
  	return hashcode % numBuckets;                                
}