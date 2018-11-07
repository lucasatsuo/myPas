#pragma once
#include <stdio.h>
#include <string.h>
#include <mytype.h>
#include <parser.h>
#include <lexer.h>
#include <tokens.h>
#include <keywords.h>

token_t lookahead;

void match(token_t expected);
void translator(char *translated, token_t original);
