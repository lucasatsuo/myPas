/**@<keywords.c>::**/
#include <keywords.h>
#include <mytype.h>
 
char *keyword[] = {
	"begin",
	"end", 
	"if", 
	"then", 
	"else", 
	"while",
	"do",
	"repeat",
	"until",
	"program",
	"var",
	"procedure",
	"function",
	"integer",
	"real",
	"double",
	"boolean",
	"not",
	"or",
	"div",
	"mod",
	"and",
	"true",
	"false",
	NULL
};
 
token_t
iskeyword(const char *symbol)
{
    for (int i = 0; keyword[i]; i++) {
	    if (strcmp(keyword[i], symbol) == 0) return BEGIN + i;
    }
    return 0;
}


