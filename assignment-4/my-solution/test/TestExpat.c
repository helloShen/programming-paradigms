/**
 * Document
 * --------
 *   https://www.ghostscript.com/doc/expat/doc/reference.html#building
 *
 * Simple example
 * -----------------------
 * Check --> ./expat-demo
 */
#include <expat.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

static int Eventcnt = 0;
char Buff[512];

static void DefaultHandler(void *data, const char *s, int len) {
  fwrite(s, len, sizeof(char), stdout);
}

static void printcurrent(XML_Parser p) {
  XML_SetDefaultHandler(p, DefaultHandler);
  XML_DefaultCurrent(p);
  XML_SetDefaultHandler(p, (XML_DefaultHandler) 0);
} 

/**
 * data is the entire line
 * el is the tag name
 * atts is array of name/value pairs, terminated by 0;
 * names and values are 0 terminated.
 * ---------------------------------------------------
 * Ex, for line as follow,
 * 		<guid isPermaLink="false">http://www.nytimes.com/2005/04/24/national/24guns.html</guid>
 * el = "guid"
 * attr = ["isPermaLink","false"]
 * data = "<guid isPermaLink="false">http://www.nytimes.com/2005/04/24/national/24guns.html</guid>"
 * 
 * !Note: data is of "XML_Parser" data type.
 */
static void SimpleStartHandler(void *data, const char *el, const char **attr) {
	printf("\n%4d: Start tag %s - ", Eventcnt++, el);	
	printcurrent((XML_Parser)data);
}

static void SimpleEndHandler(void *data, const char *el) {
	printf("$\n%4d: End tag %s - ", Eventcnt++, el);
	printcurrent((XML_Parser)data);
}

static void SimpleCharHandler(void *data, const char *txt, int txtlen) {
	printf("\n%4d: Raw Text - ", Eventcnt++);
	fwrite(txt, txtlen, sizeof(char), stdout);
}

static void SimpleTest(void) {
	FILE *fp = fopen("data/simplexml.txt", "r");
	assert(fp);
	XML_Parser p = XML_ParserCreate(NULL);
	assert(p);
	XML_UseParserAsHandlerArg(p);
	XML_SetElementHandler(p, SimpleStartHandler, SimpleEndHandler);
	XML_SetCharacterDataHandler(p, SimpleCharHandler);
	while(fgets(Buff, sizeof(Buff), fp)) {
		XML_Parse(p, Buff, strlen(Buff), feof(fp));
	}
	printf("\n");
	fclose(fp);
}

int main(void) {
	SimpleTest();
}