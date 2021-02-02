#################
#   22/01/2021  #
#################

A typical RSS feed as a sequence of "items":
```xml
<item>
  <title>At Installation Mass, New Pope Strikes a Tone of Openness</title>
  <link>http://www.nytimes.com/2005/04/24/international/worldspecial2/24cnd-pope.html</link>
  <description>The Mass, which drew 350,000 spectators, marked an important moment in the transformation of Benedict XVI.</description>
  <author>By IAN FISHER and LAURIE GOODSTEIN</author>
  <pubDate>Sun, 24 Apr 2005 00:00:00 EDT</pubDate>
  <guid isPermaLink="false">http://www.nytimes.com/2005/04/24/international/worldspecial2/24cnd-pope.html</guid>
</item>
```


1. need url / url connection, to establish the connection, and get news feed.
	-->libcurl
2. need xml parser, to extract titles, descriptions, links, and articles from xml news feed files. 
	-->expat
3. need a html parser, to analyse the web page and get the content of an article.
	-->(google)gumbo-parser
4. need tokenizer to segment words from articles, calculate the frequences and build index.
	-->streamtokenizer
5. the core index is the "word -> article" mapping.
6. this mapping can be stored in a hashset using word as the key.
	-->hashset/vector


#################
#   23/01/2021  #
#################

1. [done]CURL works
2. [done]Handout "librssnews.a" works only on linux, not on MACOS(darwin). 
3. [done]Decided to use CURL + expat + streamtokenizer + hashset/vector, maybe gumbo-parser for HTML.


#################
#   24/01/2021  #
#################

1. [done]expat test passed!


#################
#   25/01/2021  #
#################

1. [done]article done!
2. [done]rssparser done!

to be continued:
1. hashmap
2. article_scanner
  2.1 rewrite streamtokenizer
  2.2 count frequencies


#################
#   26/01/2021  #
#################
1. [done]hashmap cancle, hashset is good enough!
2. [done]word tokenization is done!
3. [done]frequence counting is done! 
4. [done]stopwords are loaded!

to be continue:
1. build index with bag of words.

#################
#   27/01/2021  #
#################
1. [done] indexer
2. [done] move curl connection module and gumbo parser module to working space. done!

to be continued:
1. test indexer


#################
#   28/01/2021  #
#################
1. [done] test_index.c download_articles()
2. [done] rewrite curlconne.c dump_url(), now it dumps web data into memory instead of local files.
3. [done] rssparser.c parserss() now accept input web page stream from memory.

to be continued:
0. optimize makefile source files dependencies
1. change htmlparser.c clean_test() so that it can also get input stream from memory.
2. establish the chain of data stream for index_article(index *idx, article *atc);


#################
#   29/01/2021  #
#################
1. [done] Optimize makefile source files dependencies.
2. [done] Create recursive makefile structure. Makefile in root directory will run 
sub-makefile under unit-test repository.

to be continued:
1. change htmlparser.c clean_test() so that it can also get input stream from memory.
2. establish the chain of data stream for index_article(index *idx, article *atc);