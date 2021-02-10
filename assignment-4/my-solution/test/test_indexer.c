#include "curlconn.h"
#include "rssparser.h"
#include "htmlparser.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/**
 * VectorMapFunction<articles>
 * ---------------------------
 * Use VectorMap() to iterate vector members.
 * Find link of each articles and download them.
 */
static const char *kTestIndexerFileExtension = ".html";
static const size_t kTestIndexerLengthDocId = 19 + 1; // docid always has 19 digits, +1 null-termination
static const size_t kTestIndexerLinkBufferSize = 2048;
static void dump_article(void *elemAddr, void *auxData) { // auxData is NULL here, we create file ourselves
	article *a = (article *)elemAddr;

	/* extract article link */
	char linkbuff[2048];
	get_link(a, linkbuff, kTestIndexerLinkBufferSize);

	/* use docid as filename to build file path */
	char *dir = (char *)auxData;
	char filename[kTestIndexerLengthDocId]; 
	size_t lenfname = snprintf(filename, kTestIndexerLengthDocId, "%ld", a->id); // long to string
	size_t lendir = strlen(dir);
	size_t lenext = strlen(kTestIndexerFileExtension);
	char path[lendir + lenfname + lenext + 1];	
	strcpy(path, dir);
	strcpy(path + lendir, filename);
	strcpy(path + lendir + lenfname, kTestIndexerFileExtension);

	/* curlconn downloads file */
	FILE *fp = fopen(path, "w");
	download_url(linkbuff,fp); 
	fprintf(stdout, "Check file -> \"%s\"\n", path);
	fflush(stdout);
}

/**
 * Given a rss xml list, download all listed article pages.
 * dir is the root directory to store all downloaded webpage files.
 */
static const char *kTestIndexerTempRssFile = "tmp.xml";
static const size_t kTestIndexerArticleVectorSize = 64;
void dump_articles(const char *rss_link, char *dir) {
	/* download rss xml file */
	FILE *rss = fopen(kTestIndexerTempRssFile, "w");
	assert(rss);
	download_url(rss_link, rss);
	fclose(rss);

	/* rssparser extract article list: vector<articles> */
	rss = fopen(kTestIndexerTempRssFile, "r");
	assert(rss);
	vector articles;
	VectorNew(&articles, sizeof(article), dispose_article, kTestIndexerArticleVectorSize);	
	parserss(&articles, rss);

	/* download each article */
	VectorMap(&articles, download_article, dir);

	/* free resources */
	VectorDispose(&articles);
	fclose(rss);
	remove(kTestIndexerTempRssFile);
}

int main(void) {
	char *bbc_rss_link = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
	char *htmldir = "data/article_html/";
	dump_articles(bbc_rss_link, htmldir);
}

