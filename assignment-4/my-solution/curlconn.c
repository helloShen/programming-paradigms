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
#include "data.h"
#include <curl/curl.h>
#include <stdio.h>
#include <assert.h>

/**
 * Curl CURLOPT_WRITEFUNCTION callback function
 * 		@ptr: 		data address passed by curl, NON null-terminated
 *		@nmemb:		size of data pointed by ptr
 *		@userdata:	my buffer, always null-terminated
 *		@size:		element size, always be 1, size of char
 * -----------------------------------------------------------------
 * For more information, please check curl doc:
 * 		--> https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 */
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	int realsize = size * nmemb;
	append_data((data *)userdata, ptr, realsize);
	return realsize;
}

/**
 * Request the web page by url, and store entire data stream into a string buffer.
 * -------------------------------------------------------------------------------
 * The maximum amount of body data that will be passed to the write callback is 
 * defined in the curl.h header file: CURL_MAX_WRITE_SIZE (the usual default is 16K).
 */
void dump_url(const char *url, data *buff) {
	fprintf(stdout, "Request for url: <%s> ...", url);
	fflush(stdout);	
	CURL *curl = curl_easy_init();
	if(curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, url);			// set url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buff);	// set recieve data stream
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // set write stream call back function
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	fprintf(stdout, "[DONE]\n");
	fflush(stdout);
}