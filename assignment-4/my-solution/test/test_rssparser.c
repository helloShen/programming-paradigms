#include "../rssparser.h"
#include "../vector.h"
#include "../article.h"
#include "../curlconn.h"
#include <stdio.h>
#include <assert.h>


static void generate_articles(char *rsslink) { 
	vector bbc_articles;
	VectorNew(&bbc_articles, sizeof(article), dispose_article, 128);	
	data buff;
	new_data(&buff);
	/* dump rss xml file into buff */
	dump_url(rsslink, &buff);
	/* extract articles from buff in memory */
	parserss(&bbc_articles, &buff);
	VectorMap(&bbc_articles, print_article, stdout);
	VectorDispose(&bbc_articles);
}

int main(void) {
	char *bbc_rss = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
	char *nytimes_rss = "https://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml";
	generate_articles(bbc_rss);
	generate_articles(nytimes_rss);
}
