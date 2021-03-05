#include "curlconn.h"
#include "article.h"
#include "data.h"
#include "rssparser.h"
#include "htmlparser.h"
#include "tokenizer.h"
#include "index.h"
#include "threads_pool.h"
#include <stdlib.h>
#include <assert.h>

/**
 * HashSetMapFunction<freq>
 * ------------------------
 * Update index with [word, frequency] pair.
 * docid is given by local variable mapFreqToIndexDocId
 */
static long local_docid;
static void freq_into_index(void *elemAddr, void *auxData) {
	idx *i = (idx *)auxData;
	freq *fq = (freq *)elemAddr;
	enter_index(i, local_docid, fq->word, fq->freq);
}

/**
 * Build index with a bunch of bag_of_words
 */
static void bag_into_index(idx *i, const bag_of_words *bag) {
	local_docid = bag->docid;
	HashSetMap(bag->freqs, freq_into_index, i);
	local_docid = 0L;
}

typedef struct {
	article *a;
	articles *as;
	idx *i;
	hashset *stopwords;
} thd_args;

static void new_thd_args(thd_args *args, article *a, articles *as, idx * i, hashset *stopwords) {
	args->a = a;
	args->as = as;
	args->i = i;
	args->stopwords = stopwords;
}

/**
 * Article treatement in a new thread
 * Implement void *(*start_routine) (void *)
 */
static void *index_article_thread(void *arg) {
	/* intermediate data */
	thd_args *args;
	data *article_buffer;
	data clean_buffer;
	bag_of_words *bag;
	/* cast parameters */
	args = (thd_args *)arg;
	/* update article table */
	add_article(args->as, args->a);
	/* download article raw text by url */
	article_buffer = dump_url(args->a->link);
	if (article_buffer != NULL) {
		/* htmlparser extract clean text from .html web page. */
		new_data(&clean_buffer);
		clean_text(article_buffer, &clean_buffer);
		/* tokenizer cut clean text into vector of words */
		bag = to_bagofwords(args->a->id, clean_buffer.stream, args->stopwords);
		/* update index by bag_of_words */
		bag_into_index(args->i, bag);
		/* free resources */
		dispose_bagofwords(bag); // I have copy everything in bag_of_words into index. So I can dispose it.
		dispose_data(article_buffer);
		free(article_buffer);
		dispose_data(&clean_buffer);
	}
	return NULL;
}

/**
 * Build index and article table by using given remote rss xml file link.
 */
static const size_t kEngineArticleVectorSize = 256;
void build_index(idx *i, articles *as, const char *link) {
	data *buffer;						// buffer for rss xml file
	vector vas;							// vector of article parsed from rss
	size_t asize;						// length of article vector
	hashset *stopwords;					// stopwords
	article *a;							// pointer to each article in vector vas
	/* download rss.xml by url */
	buffer = dump_url(link);
	/* rssparser parse articles from rss.xml */
	VectorNew(&vas, sizeof(article), dispose_article, kEngineArticleVectorSize);
	parserss(&vas, buffer);
	dispose_data(buffer);
	free(buffer);
	/* load stopwords */
	stopwords = load_stopwords();
	/* threads pool */
	threads_pool_init(10);
	/* for each article */
	asize = VectorLength(&vas);
	thd_args args[asize];				// parameters pass to each index_article_thread()
	for (int j = 0; j < asize; j++) {
		a = VectorNth(&vas, j);
		new_thd_args(&args[j], a, as, i, stopwords);
		create_thread(index_article_thread, &args[j]);
	}
	/* free resources */
	threads_pool_close();
	dispose_stopwords(stopwords);
}

/**
 * Map each doc_freq element in hashset<doc_freq> into a vector<doc_freq>.
 */
static void map_to_vector(void *elemAddr, void *auxData) {
	doc_freq *df = (doc_freq *)elemAddr;
	vector *v = (vector *)auxData;
	VectorAppend(v, df);
}

/**
 * Sort vector by doc_freq.freq
 */
static int comp_docfreq_byfreq(const void *elemAddr1, const void *elemAddr2) {
	return (((doc_freq *)elemAddr2)->freq - ((doc_freq *)elemAddr1)->freq);
}

/**
 * Print search result to screen.
 */
static void print_result(const article *a, int freq) {
	printf("\"%s\"\n\"%s\"\n[search terms occur %d times]\n", a->title, a->link, freq);
}

/**
 * Cut query into vector of words, and lookup in index.
 */
static const size_t kEngineQueryVectorSize = 16;
static const size_t kEngineResultVectorSize = 64;
void search(const idx *i, const articles *as, const char *query) {
	/* tokenize query */
	vector words;
	VectorNew(&words, sizeof(char *), freestr, kEngineQueryVectorSize);
	tokenize(&words, query);

	/* search in index and dump result into a vector<doc_freq> */
	word_index *result = search_in_index(i, &words);
	vector result_v;
	VectorNew(&result_v, sizeof(doc_freq), NULL, kEngineResultVectorSize);
	HashSetMap(result->freqs, map_to_vector, &result_v);
	VectorSort(&result_v, comp_docfreq_byfreq);

	for (int j = 0; j < VectorLength(&result_v); j++) {
		doc_freq *pair = VectorNth(&result_v, j);
		article phantom;
		new_article_with_docid(&phantom, pair->docid);
		article *a = HashSetLookup(as->table, &phantom);
		assert(a != NULL);
		print_result(a, pair->freq);
	}
}


