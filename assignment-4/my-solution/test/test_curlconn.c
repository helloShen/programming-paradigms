#include "curlconn.h"
#include <curl/curl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* sources */
static const char *kTCC_bbc_rss = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
static const char *kTCC_bbc_news = "https://www.bbc.co.uk/news/world-europe-55791389";

static void dump_tofile(const char *url, FILE *out) {
	data buff;
	new_data(&buff);
	dump_url(kTCC_bbc_rss, &buff);
	fprintf(out, "%s", buff.stream);
	fflush(out);
	dispose_data(&buff);
}

static void test_dump_url() {
	/* output resources */
	FILE *bbc_rss_xml = fopen("data/bbc-dump-rss.txt", "w");
	FILE *bbc_news_html = fopen("data/bbc-dump-news.txt", "w");	
	assert(bbc_rss_xml);
	assert(bbc_news_html);
	dump_tofile(kTCC_bbc_rss, bbc_rss_xml);
	dump_tofile(kTCC_bbc_news, bbc_news_html);
}

int main(void) {
	test_dump_url();
}