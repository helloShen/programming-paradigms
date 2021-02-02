#include "../curlconn.h"
#include <curl/curl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* sources */
static const char *kTCC_bbc_rss = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
static const char *kTCC_bbc_news = "https://www.bbc.co.uk/news/world-europe-55791389";
static const char *kTCC_seattle_rss = "http://seattletimes.nwsource.com/rss/home.xml";
static const char *kTCC_nytimes_rss = "https://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml";
static const char *kTCC_nytimes_news = "https://www.nytimes.com/2021/01/24/us/west-virginia-vaccine.html";

static void test_download_url() {
	/* files destination */
	FILE *bbc_rss_xml = fopen("data/bbc-rss.txt", "w");
	FILE *bbc_news_html = fopen("data/bbc-news.txt", "w");
	FILE *seattle_rss_xml = fopen("data/seattle-rss.txt", "w");
	FILE *nytimes_rss_xml = fopen("data/nytimes-rss.txt", "w");
	FILE *nytimes_news_html = fopen("data/nytimes-news.txt", "w");
	assert(bbc_rss_xml);
	assert(bbc_news_html);
	assert(seattle_rss_xml);
	assert(nytimes_rss_xml);
	assert(nytimes_news_html);

	/* download rss and news files to local repository */
	download_url(kTCC_bbc_rss, bbc_rss_xml);
	download_url(kTCC_bbc_news, bbc_news_html);
	download_url(kTCC_seattle_rss, seattle_rss_xml);
	download_url(kTCC_nytimes_rss, nytimes_rss_xml);
	download_url(kTCC_nytimes_news, nytimes_news_html);
	
	/* free resource */	
	fclose(bbc_rss_xml);
	fclose(bbc_news_html);
	fclose(seattle_rss_xml);
	fclose(nytimes_rss_xml);
	fclose(nytimes_news_html);
}

static void dump_helper(const char *url, FILE *out) {
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
	dump_helper(kTCC_bbc_rss, bbc_rss_xml);
	dump_helper(kTCC_bbc_news, bbc_news_html);
}

int main(void) {
	test_download_url();
	test_dump_url();
}