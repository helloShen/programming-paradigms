#include "article.h"
#include "index.h"
#include "engine.h"
#include <stdio.h>
#include <string.h>

int main(void) {
	char rsspath[1024];
	char line[1024];
	char query[512];
	int len;
	FILE *rssfile = NULL;
	idx i;
	new_index(&i);
	articles as;
	new_articles(&as);

	for (;;) {
		printf("Give me a file of rss news xml links...\n");
		scanf("%s", rsspath);
		rssfile = fopen(rsspath, "r");
		if (rssfile != NULL) {
			break;
		} else {
			printf("Wrong file!\n");
		}
	}

	while (fgets(line, sizeof(line), rssfile)) {
		len = strlen(line);
		if (line[len - 1] == '\n') line[len - 1] = '\0';
		build_index(&i, &as, line);
	}

	for (;;) {
		printf("Index is built! Give me a query...[enter \"quit\" to close engine]\n");
		fgets(query, 512, stdin);
		if (strncmp(query, "quit", 4) == 0) {
			break;
		}
		search(&i, &as, query);
	}

	fclose(rssfile);
	dispose_index(&i);
	dispose_articles(&as);

}