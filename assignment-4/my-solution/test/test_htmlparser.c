#include "article.h"
#include "data.h"
#include "curlconn.h"
#include "rssparser.h"
#include "htmlparser.h"
#include <stdio.h>

/**
 * Give me a rss news xml link, I return a vector of articles.
 * Hide intermediary data details from search engine.
 */
static void t_crawl_articles(vector *articles, const char *rsslink) { 
	data buff;
	new_data(&buff);
	/* dump rss xml file into memory buffer */
	dump_url(rsslink, &buff);
	/* extract articles from memory buffer */
	parserss(articles, &buff);
	dispose_data(&buff);
}

/**
 * Give me an article, I download the html page into memory,
 * and parse html into a bag of words.
 * Hide html details from search engine.
 */
static const size_t kTestHtmlParserLinkBufferSize = 2048;
static void t_crawl_article_cleantext(article *a, data *cleandata) {	
	data page;
	new_data(&page);
	char linkbuff[kTestHtmlParserLinkBufferSize];
	get_link(a, linkbuff, kTestHtmlParserLinkBufferSize);
	dump_url(linkbuff, &page);
	clean_text(&page, cleandata);
	dispose_data(&page);
}

/**
 * VectorMapFunction<article>
 */
static void map_article_to_cleantext(void *elemAddr, void *auxData) {
	article *a = (article *)elemAddr;
	data *cleandata = (data *)auxData;
	char linkbuff[kTestHtmlParserLinkBufferSize];
	get_link(a, linkbuff, kTestHtmlParserLinkBufferSize);
	printf("Extracting clean text from %s ...", linkbuff);
	new_data(cleandata);
	t_crawl_article_cleantext(a, cleandata);
	printf("[DONE]\n%s\n", cleandata->stream);
	dispose_data(cleandata);
}


static void test_htmlparser(char *rsslink) {
	vector articles;
	VectorNew(&articles, sizeof(article), dispose_article, 128);	
	t_crawl_articles(&articles, rsslink);
	data cleandata;
	VectorMap(&articles, map_article_to_cleantext, &cleandata);
	VectorDispose(&articles);
}

int main(void) {
	char *bbc_rss = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
	test_htmlparser(bbc_rss);
}