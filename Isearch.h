/*
 * search for number: 
 * 		Isearch() calls Intcmp()
 * search for string: 
 * 		Isearch() calls Strcmp() calls strcmp()
 */

void *Isearch(void *key, void *base,
			int n, int elemSize,
			int (*cmp)(void *, void *));

int Intcmp(void *elem1, void *elem2);
int Strcmp(void *elem1, void *elem2);
