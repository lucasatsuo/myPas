/**@<lexer.h>::**/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mytype.h>
#include <tokens.h>
#include <keywords.h>

#define MAXIDLEN 0x100

extern char lexeme[MAXIDLEN + 1];

char lexeme[MAXIDLEN + 1];

token_t gettoken(FILE * tape);
