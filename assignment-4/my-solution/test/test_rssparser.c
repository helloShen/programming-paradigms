#include "vector.h"
#include "data.h"
#include "article.h"
#include "curlconn.h"
#include "rssparser.h"


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

static void test_rssparser(const char *rsslink) {
	vector articles;
	VectorNew(&articles, sizeof(article), dispose_article, 128);	
	t_crawl_articles(&articles, rsslink);
	VectorMap(&articles, print_article, stdout);
	VectorDispose(&articles);
}

int main(void) {
	char *bbc_rss = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
	char *nyt_rss = "https://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml";
	test_rssparser(bbc_rss);
	test_rssparser(nyt_rss);
}
