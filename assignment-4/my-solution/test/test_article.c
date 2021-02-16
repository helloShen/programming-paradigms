#include "../article.c"
#include "testutils.h"
#include <assert.h>


static const long kTestArticleMinimumId = 50 * 365 *24 * 60 * 60; 	// at least 50 years scince unix epoch 1970-01-01

/**
 * static long getnano(void);
 */
static void test_getnano(void) {
	tstart("article::getnano()");

	long id1 = getnano();
	long id2 = getnano();
	assert(id1 > kTestArticleMinimumId);
	assert(id2 > kTestArticleMinimumId);
	assert(id1 < id2);

	tpass();
}

/**
 * void new_article(article *a);
 * void update_article(article *a, const char *title, int size, rsstag tag);
 * void dispose_article(void *elemAddr);
 * char *article_tostring(article *a);
 */
static void test_article(void) {
	tstart("article::new_article()::update_article()::dispose_article()::article_tostring()");

	article a;
	new_article(&a);
	assert(a.id > kTestArticleMinimumId);
	char buffer[32];
	snprintf(buffer, 32, "%ld", a.id);
	assert(strlen(buffer) == 19);	// id always has 19 digits
	char *title = "Myanmar coup: Aung San Suu Kyi detained as military seizes control";
	char *link = "https://www.bbc.co.uk/news/world-asia-55882489";
	char *description = "The army has seized power after alleging election fraud in the democratic vote held in November.";
	update_article(&a, title, strlen(title), rss_title);
	update_article(&a, link, strlen(link), rss_link);
	update_article(&a, description, strlen(description), rss_description);
	assert(strlen(title) == 66);
	assert(strlen(link) == 46);
	assert(strlen(description) == 96);
	dispose_article(&a);

	tpass();

}

/**
 * static void new_article_with_docid(article *a, const long docid);
 */
static void test_new_article_with_docid(void) {
	tstart("article::new_article_with_docid()");

	article a;
	new_article_with_docid(&a, 100L);
	assert(a.id == 100L);

	tpass();	
}

/**
 * static int hash_article(const void *elemAddr, int numBuckets);
 */
static void test_hash_article(void) {
	tstart("article::hash_article()");

	article a;
	new_article(&a);
	int hash = hash_article(&a, 16);
	assert(hash >= 0 && hash < 16);

	dispose_article(&a);

	tpass();
}

/**
 * static int comp_article(const void *elemAddr1, const void *elemAddr2);
 */
static void test_comp_article(void) {
	tstart("article::comp_article()");

	article a, b;
	new_article(&a);
	new_article(&b);
	assert(comp_article(&a, &b) < 0);

	dispose_article(&a);
	dispose_article(&b);

	tpass();	
}

/**
 * HashSetMapFunction<article>
 * Used to print articles.
 */
static void map_articles(void *elemAddr, void *auxData) {
	FILE *outfile = (FILE *)auxData;
	article *a = (article *)elemAddr;
	print_article(a, outfile);
}

/**
 * void new_articles(articles *a);
 * void add_articles(articles *as, article *a);
 * article *search_article(const articles *a, const long docid);
 * void dispose_articles(articles *a);
 */
static void test_articles(void) {
	tstart("article::new_articles()::add_articles()::search_article()::dispose_articles()");

	articles as;
	new_articles(&as);
	assert(as.table != NULL);

	article a;
	new_article_with_docid(&a, 1L);
	update_article(&a, "apple", 5, rss_title);
	update_article(&a, "www.apple.com", 13, rss_link);
	update_article(&a, "I love apple.", 13, rss_description);
	add_article(&as, &a);

	assert(HashSetCount(as.table) == 1);
	article *result = search_article(&as, a.id);
	assert(result != NULL);
	char *result_str = article_tostring(&a);
	char *answer_str = "[id]:0000000000000000001\n[title]:apple\n[link]:www.apple.com\n[description]:I love apple.\n";
	assert(strcmp(result_str, answer_str) == 0);

	dispose_articles(&as);
	assert(as.table == NULL);

	tpass();
}

int main(void) {
	test_getnano();
	test_article();
	test_new_article_with_docid();
	test_hash_article();
	test_comp_article();
	test_articles();
}