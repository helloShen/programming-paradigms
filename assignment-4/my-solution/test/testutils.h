/**
 * Print test info
 */
void tstart(char *spacename);

/**
 * Print pass
 */
void tpass(void);


int t_string_hash(const char *s, const int numBuckets);

/**
 * HashSetMapFunction<char *>
 */
void t_print_string(void *elemAddr, void *auxData);

#ifndef _MOCK_FREQ_
#define _MOCK_FREQ_
typedef struct {
	char *word;
	int freq;
} mock_freq;


/**
 * char *fq->word is null-terminated.
 * result of snprintf() will be terminated with a null character, 
 * unless buf_size is zero
 */
char *t_freq_tostring(mock_freq *fq);

/**
 * HashSetMapFunction<freq>
 * Append string of freq to a long string.
 */
void t_freq_maptostring(void *elemAddr, void *auxData);

/**
 * HashSetMapFunction<freq>
 */
void t_print_freq(void *elemAddr, void *auxData);

/**
 * Word need to be null-terminated c string
 */ 
void t_new_freq(mock_freq *fq, const char *word, const int freq);

/**
 * HashSetHashFunction<freq>
 */
int t_hash_freq(const void *fq, const int numBuckets);

/**
 * HashSetCompareFunction<freq>
 */
int t_comp_freq(const void *fq1, const void *fq2);

/**
 * HashSetFreeFunction<freq>
 * VectorCFreeFunction<freq>
 */
void t_dispose_freq(void *fq);
#endif // _MOCK_FREQ_


/**
 * Read the entire file into a long string.
 * The return stream is null-terminated.
 */
char *t_read_whole_file(const char *filepath);
