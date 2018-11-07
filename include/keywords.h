/**@<keywords.h>::**/

#pragma once

#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <mytype.h>

token_t iskeyword(const char *);

/****************************
Keywords:: 
	BEGIN, END, IF, THEN, ELSE, WHILE, DO, REPEAT, UNTIL,
	PROGRAM, VAR, PROCEDURE, FUNCTION,
	INTEGER, REAL, DOUBLE, BOOLEAN,
	NOT, OR, DIV, MOD, AND,
	TRUE, FALSE

 ************************************/
enum {
	BEGIN = 8192, 
	END, 
	IF, 
	THEN, 
	ELSE, 
	WHILE,
	DO,
	REPEAT,
	UNTIL,
	PROGRAM,
	VAR,
	PROCEDURE,
	FUNCTION,
	INTEGER,
	REAL,
	DOUBLE,
	BOOLEAN,
	NOT,
	OR,
	DIV,
	MOD,
	AND,
	TRUE,
	FALSE,
};

