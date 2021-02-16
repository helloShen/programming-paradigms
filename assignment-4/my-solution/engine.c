#include "curlconn.h"
#include "article.h"
#include "data.h"
#include "rssparser.h"
#include "htmlparser.h"
#include "tokenizer.h"
#include "index.h"
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

/**
 * Build index and article table by using given remote rss xml file link.
 */
static const size_t kEngineArticleVectorSize = 256;
void build_index(idx *i, articles *as, const char *link) {
	/* download rss.xml by url */
	data buffer;
	new_data(&buffer);
	dump_url(link, &buffer);

	/* rssparser parse articles from rss.xml */
	vector vas;
	VectorNew(&vas, sizeof(article), dispose_article, kEngineArticleVectorSize);
	parserss(&vas, &buffer);
	dispose_data(&buffer);

	/* load stopwords */
	hashset *stopwords = load_stopwords();

	article *a;
	data article_buffer;
	data clean_buffer;
	/* for each article */
	for (int j = 0; j < VectorLength(&vas); j++) {
		/* update article table */
		a = VectorNth(&vas, j);
		add_article(as, a);

		/* download article raw text by url */
		new_data(&article_buffer);
		new_data(&clean_buffer);
		dump_url(a->link, &article_buffer);

		/* htmlparser extract clean text from .html web page. */
		clean_text(&article_buffer, &clean_buffer);

		/* tokenizer cut clean text into vector of words */
		bag_of_words *bag = to_bagofwords(a->id, clean_buffer.stream, stopwords);

		/* update index by bag_of_words */
		bag_into_index(i, bag);

		dispose_bagofwords(bag); // I have copy everything in bag_of_words into index. So I can dispose it.
		dispose_data(&article_buffer);
		dispose_data(&clean_buffer);
	}

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


