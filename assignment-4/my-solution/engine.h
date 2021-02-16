/**
 * Build index by using given remote rss xml file link.
 */
void build_index(idx *i, articles *as, const char *link);

/**
 * Main entry for user to lookup a key word.
 */
word_index *search(const idx *i, const articles *as, const char *query);