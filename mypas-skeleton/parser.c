/**@<parser.c>::**/

#include <keywords.h>

#include <tokens.h>

#include <mytype.h>

/***************************************************************************
Syntax definitions for the academic project My Pascal,
which is a simplified / modified Pascal compiler,
according to the following EBNF grammar:

Start symbol:: mypas

Production Rules::

mypas -> PROGRAM ID ';' header body '.' */
void mypas(void)
{
   match(PROGRAM); match(ID); match(';'); header(); body(); match('.');
}
flag_t issubroutine(void)
{
    switch(lookahead){
    case PROCEDURE:case FUNCTION:
	    return lookahead;
    }
    return 0;
}
/**************************************************************************
Declarative scope::

header -> varmodel { procmodel | funcmodel } */
void header(void)
{
	varmodel(); 
	while (issubroutine) { 
		if(lookahead==PROCEDURE){
			procmodel();
		} else {
			funcmodel();
		}
	}
}
/**************************************************************************
varmodel -> [ VAR  vargroup { vargroup } ] */
void varmodel (void)
{
   if(lookahead==VAR){
	  match(VAR);
	  vargroup(); 
	  while(lookahead==ID){ 
		  vargroup();
	  }
   }
}
/**************************************************************************
vargroup -> varlist ':' pretype ';' */
void vargroup(void)
{
    varlist(); match(':'); pretype(); match(';');
}

/**************************************************************************
varlist -> ID { ',' ID } */
void varlist(void)
{
    match(ID); while(lookahead==','){ match(','); match(ID); }
}

/**************************************************************************
pretype -> BOOLEAN | INTEGER | REAL | DOUBLE */
void pretypeg(void)
{
	switch(lookahead){
	case BOOLEAN:
		match(BOOLEAN);
		break;
	case INTEGER:
		match(INTEGER);
		break;
	case REAL:
		match(REAL);
		break;
	default:
		match(DOUBLE);
	}
}

/**************************************************************************
procmodel -> PROCEDURE ID formalparms ';' header body ;

funcmodel -> FUNCTION ID formalparms ':' pretype ';' header body ;

formalparms -> [ ( arglist ) ]

arglist -> argmodel { ';' argmodel }

argmodel -> [ VAR ] varlist ':' pretype

Imperative scope::

body -> BEGIN stmtlist END

stmtlist -> stmt { ';' stmt }

stmt ->   body
	| ifstmt
	| whlstmt
	| repstmt
	| smpexpr
	| <empty>

ifstmt -> IF expr THEN stmt [ ELSE stmt ]

expr -> smpexpr [ relop smpexpr ]

relop ->  = 
	| > 
	| ">="	// GEQ
	| < 
	| "<="	// LEQ 
	| "<>"	// NEQ

smpexpr -> [ + | - ] term { oplus term }

term -> factor { otimes factor }

factor ->   ID [ ":=" expr  | ( exprlist ) ]
  | UINT
  | FLTP
  | ( expr )

exprlist -> expr { ',' expr }

whlstmt -> WHILE expr DO stmt

repstmt -> REPEAT stmtlist UNTIL expr

*/
