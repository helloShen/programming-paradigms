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
#include <stdlib.h>
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
 * Prevent curl to print in stdout.
 */
static size_t silent_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	int realsize = size * nmemb;
	return realsize;
}

/**
 * Request the web page by url, and store entire data stream into a string buffer.
 * -------------------------------------------------------------------------------
 * The maximum amount of body data that will be passed to the write callback is 
 * defined in the curl.h header file: CURL_MAX_WRITE_SIZE (the usual default is 16K).
 */
static data *real_dump_url(const char *url) {
	data *buffer = (data *)malloc(sizeof(data));
	new_data(buffer);
	fprintf(stdout, "Request for url: <%s> ...", url);
	fflush(stdout);	
	CURL *curl = curl_easy_init();
	if(curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, url);						// set url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);				// set recieve data stream
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); 	// set write stream call back function
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stdout, "In dump_url(), curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			return NULL;
		}
		curl_easy_cleanup(curl);
	}
	fprintf(stdout, "[DONE]\n");
	fflush(stdout);
	return buffer;
}

/**
 * Request the web page by url, and store entire data stream into a string buffer.
 * This function only treats redirect, it calls dump_url() to download real xml.
 * -------------------------------------------------------------------------------
 * The maximum amount of body data that will be passed to the write callback is 
 * defined in the curl.h header file: CURL_MAX_WRITE_SIZE (the usual default is 16K).
 *
 * A typical http redirection response implies a 3xx response code,
 * We extract new url from the "Location" field, and redirect the page.
 * --------------------------------------------------------------------
 * HTTP/1.1 301 Moved Permanently
 * Server: Apache
 * Location: http://feeds.bbci.co.uk/news/rss.xml?edition=int 		<== redirect to a new url
 * Content-Length: 256
 * Content-Type: text/html; charset=iso-8859-1
 * Cache-Control: max-age=87997561
 * Expires: Sat, 02 Dec 2023 05:13:33 GMT
 * Date: Tue, 16 Feb 2021 17:27:32 GMT
 * Connection: keep-alive
 * Timing-Allow-Origin: https://www.bbc.co.uk, https://www.bbc.com
 * 
 * <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
 * <html><head>
 * <title>301 Moved Permanently</title>
 * </head><body>
 * <h1>Moved Permanently</h1>
 * <p>The document has moved <a href="http://feeds.bbci.co.uk/news/rss.xml?edition=int">here</a>.</p>
 * </body></html>
 */
data *dump_url(const char *url) {
	CURL *curl;
	CURLcode res;
	char *location;
	long response_code;

	fprintf(stdout, "Check for url: <%s> ...", url);
	fflush(stdout);	
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);							// set url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);					// set recieve data stream
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, silent_callback); 	// set write stream call back function
		res = curl_easy_perform(curl);

		/* Check for errors and redirection */
		if (res != CURLE_OK) {
			fprintf(stdout, "In dump_url_with_redirect(), curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			return NULL;
		} else {
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			/* a redirect implies a 3XX response code */
			if ((res == CURLE_OK) && (response_code / 100) != 3) {
				fprintf(stdout, "Not a redirect.\n");
				curl_easy_cleanup(curl);
				return real_dump_url(url);
			} else { // a redirect
				res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
				if ((res == CURLE_OK) && location) {
					fprintf(stdout, "Redirect to \"%s\"\n", location);
					return dump_url(location);	// maybe 2nd, 3rd redirect
				} else if (res == CURLE_OK && !location) {
					fprintf(stdout, "A redirect has no new url location.\n");
					curl_easy_cleanup(curl);
					return NULL;
				}
			}
		}
		curl_easy_cleanup(curl);
	}
	return NULL;
}

