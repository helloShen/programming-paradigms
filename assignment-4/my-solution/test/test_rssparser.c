#include "../rssparser.h"
#include "../vector.h"
#include "../article.h"
#include <stdio.h>

static void mapfn(void *elemAddr, void *outfile) {
	FILE *output = (FILE *)outfile;
	map_article((article *)elemAddr, output);
	fprintf(output, "\n");
	fflush(output);
} 

int main(void) {
	FILE *bbc = fopen("data/bbc-rss.txt", "r");
	FILE *nyt = fopen("data/nytimes-rss.txt", "r");
	FILE *bbc_out = fopen("data/bbc-articles.txt", "w");
	FILE *nyt_out = fopen("data/nyt-articles.txt", "w");
	vector bbc_articles = parserss(bbc);
	vector nyt_articles = parserss(nyt);
	VectorMap(&bbc_articles, mapfn, bbc_out);
	VectorMap(&nyt_articles, mapfn, nyt_out);
	VectorDispose(&bbc_articles);
	VectorDispose(&nyt_articles);
}
