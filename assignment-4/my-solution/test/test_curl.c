/**
 * Curllib Main page
 * -----------------
 *   --> <https://curl.se/libcurl/>
 *
 * Find out link flags
 * -------------------
 *  curl-config --cflags
 *  curl-config --libs
 *  curl-config --version
 * API
 * ---
 * 	 --> <https://curl.se/libcurl/c/>
 * Useful code example
 * -------------------
 * 	 --> <https://curl.se/libcurl/c/fopen.html>
 */
#include <curl/curl.h>
#include <stdio.h>
#include <assert.h>

/**
 * Request the page content of a url and write in a local file.
 */
static void CurlDumpUrl(const char *url, FILE *fp) {
	CURL *curl = curl_easy_init();
	if(curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

/**
 * Call CurlDumpUrl() 
 */
static void DumpUrl(const char *url, const char *filePath) {
	FILE *fp = fopen(filePath, "w");
	assert(fp);
	printf("Request for url: <%s> ...\n", url);
	CurlDumpUrl(url, fp);
	printf("[DONE] Check file -> %s\n", filePath);
	fclose(fp);
} 

/**
 * Hello World
 */
static void SimpleTest(const char *link) {
	CURL *curl = curl_easy_init();
	if(curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, link);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L); 	// Show http header. We don't need it.
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

int main(void) {
	/* sources */
	char *bbc_rss = "http://feeds.bbci.co.uk/news/rss.xml?edition=int";
	char *bbc_news = "https://www.bbc.co.uk/news/world-europe-55791389";
	char *seattle_rss = "http://seattletimes.nwsource.com/rss/home.xml";
	char *nytimes_rss = "https://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml";
	char *nytimes_news = "https://www.nytimes.com/2021/01/24/us/west-virginia-vaccine.html";

	/* SimpleTest() */
	// SimpleTest(bbc_rss);

	/* DumpLink() to files */
	DumpUrl(bbc_rss, "data/bbc-rss.txt");
	DumpUrl(bbc_news, "data/bbc-news.txt");
	DumpUrl(seattle_rss, "data/seattle-rss.txt");
	DumpUrl(nytimes_rss, "data/nytimes-rss.txt");
	DumpUrl(nytimes_news, "data/nytimes-news.txt");
}