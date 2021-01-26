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

1. CURL works
2. Handout "librssnews.a" works only on linux, not on MACOS(darwin). 
3. Decided to use CURL + expat + streamtokenizer + hashset/vector, maybe gumbo-parser for HTML.


#################
#   24/01/2021  #
#################

1. expat works