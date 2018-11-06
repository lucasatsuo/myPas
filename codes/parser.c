/**@<parser.c>::**/
#include <parser.h>
	
/***************************************************************************
*
* a palavra "mod" em keywords nunca foi usada, deve ser implementada?
* e quanto a FILE, TEXT, LOGIC, que tem na lista de 2014, ex 6 e 7
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Syntax definitions for the academic project My Pascal,
which is a simplified / modified Pascal compiler,
according to the following EBNF grammar:

Start symbol:: mypas

Production Rules::

mypas -> PROGRAM ID ; header body .
***************************************************************************/
void mypas(void)
{
	match(PROGRAM); 
	match(ID); 
	match(';'); 
	header(); 
	body();
	match('.');
}
/***************************************************************************
Declarative scope::

header -> varmodel { procmodel | funcmodel }
***************************************************************************/
flag_t issubroutine(void){
	switch(lookahead){
		case PROCEDURE:
		case FUNCTION:
			return lookahead;
	}
	return 0;
}

void header(void){
	varmodel();
	while(issubroutine()){
		if(lookahead == PROCEDURE)
			procmodel();
		else if(lookahead == FUNCTION)
			funcmodel();
	}
}
/***************************************************************************

varmodel -> [ VAR  vargroup { vargroup } ]

***************************************************************************/
void varmodel(void){
	if(lookahead == VAR){
		match(VAR);
		vargroup();
		while(lookahead == ID){
			vargroup();
		}
	}
}
/***************************************************************************

vargroup -> varlist : pretype ;

***************************************************************************/
void vargroup(void){
	varlist();
	match(':');
	pretype();
	match(';');
}
/***************************************************************************

varlist -> ID { , ID }

***************************************************************************/
void varlist(void){
	match(ID);
	while(lookahead == ','){
		match(',');
		match(ID);
	}
}
/***************************************************************************

pretype -> BOOLEAN | INTEGER | REAL | DOUBLE

***************************************************************************/
void pretype(void){
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
/***************************************************************************

procmodel -> PROCEDURE ID formalparms ; header body ;

***************************************************************************/
void procmodel(void){
	match(PROCEDURE);
	match(ID);
	formalparms();
	match(';');
	header();
	body();
	match(';');
}
/***************************************************************************

funcmodel -> FUNCTION ID formalparms : pretype ; header body ;

***************************************************************************/
void funcmodel(void){
	match(FUNCTION);
	match(ID);
	formalparms();
	match(':');
	pretype();
	match(';');
	header();
	body();
	match(';');
}
/***************************************************************************

formalparms -> [ ( arglist ) ]

***************************************************************************/
void formalparms(void){
	if(lookahead == '('){
		match('(');
		arglist();
		match(')');
	}
}
/***************************************************************************

arglist -> argmodel { ; argmodel }

***************************************************************************/
void arglist(void){
	argmodel();
	while(lookahead == ';'){
		match(';');
		argmodel();
	}

}
/***************************************************************************

argmodel -> [ VAR ] varlist : pretype

***************************************************************************/
void argmodel(void){
	if(lookahead == VAR){
		match(lookahead);
	}
	varlist();
	match(':');
	pretype();
}
/***************************************************************************
Imperative scope::

body -> BEGIN stmtlist END
***************************************************************************/
void body(void){
	match(BEGIN);
	stmtlist();
	match(END);
}
/***************************************************************************

stmtlist -> stmt { ; stmt }

***************************************************************************/
void stmtlist(void){
	stmt();
	while(lookahead == ';'){
		match(';');
		stmt();
	}
}
/***************************************************************************
stmt ->   body
| ifstmt
| whlstmt
| repstmt
| smpexpr
| <empty>
***************************************************************************/
void stmt(void){
	switch(lookahead){
		case BEGIN:
			body();
			break;
		case IF:
			ifstmt();
			break;
		case WHILE:
			whlstmt();
			break;
		case REPEAT:
			repstmt();
			break;
		default:
			switch(lookahead){ // abstrai FIRST(smpexpr)
				case '+': case '-': case '(':
				case NOT: case ID: case UINT: case FLTP:
				case TRUE: case FALSE:
					smpexpr();
				break;
				default:
					;
			}
	}
}
/***************************************************************************

ifstmt -> IF expr THEN stmt [ ELSE stmt ]
***************************************************************************/
void ifstmt(void){
	match(IF);
	expr();
	match(THEN);
	stmt();
	if(lookahead == ELSE){
		match(ELSE);
		stmt();
	}
}
/***************************************************************************

expr -> smpexpr [ relop smpexpr ]

relop ->  = 
	| > 
	| ">="	// GEQ
	| < 
	| "<="	// LEQ 
	| "<>"	// NEQ


***************************************************************************/
void expr(void){
	smpexpr();
	switch(lookahead){
		case '=':
		case '>':
		case '<':
		case GEQ:
		case LEQ:
		case NEQ:
			match(lookahead);
			smpexpr();
		default:
			;	
	}
}
/***************************************************************************

smpexpr -> [ + | - | NOT ] fact { otimes fact } { oplus fact { otimes fact }}

otimes -> AND | DIV | * | /

oplus  -> OR | + | -

***************************************************************************/
void smpexpr(void){
	flag_t             isneg  =  0;
	flag_t             otimes =  0;
	flag_t             oplus  =  0;

	oplus = lookahead;
	if (oplus == '+' || oplus == '-' || oplus == NOT) {
		match(oplus);
	}

	T_begin:

	F_begin:

	factor();

	otimes = lookahead;
	if (otimes == '*' || otimes == '/' || otimes == AND || otimes == DIV) {
		match(otimes);
		goto F_begin;
	}

	oplus = lookahead;
	if (oplus == '+' || oplus == '-' || oplus == OR) { 
		match(oplus);
		goto T_begin;            
	}
}
/***************************************************************************
factor ->   ID [ ASGN expr | ( exprlist ) ] 
  | UINT
  | FLTP
  | TRUE
  | FALSE
  | ( expr )
***************************************************************************/
void factor(void)
{
	switch (lookahead) {
		case ID:
		match(ID);
			if(lookahead == ASGN){
				match(ASGN);
				expr();
			} else if(lookahead == '(') {
				match('(');
				exprlist();
				match(')');
			}
			break;
		case UINT:
		case FLTP:
			match(lookahead);
			break;
		case TRUE:
		case FALSE:
			match(lookahead);
			break;
		default:
		match('('); expr(); match(')'); 
	}
}
/***************************************************************************

exprlist -> expr { , expr }

***************************************************************************/
void exprlist(void){
	expr();
	while(lookahead==','){
		match(',');
		expr();
	}

}
/***************************************************************************

whlstmt -> WHILE expr DO stmt

***************************************************************************/
void whlstmt(void){
	match(WHILE);
	expr();
	match(DO);
	stmt();
}
/***************************************************************************

repstmt -> REPEAT stmtlist UNTIL expr

***************************************************************************/
void repstmt(void){
	match(REPEAT);
	stmtlist();
	match(UNTIL);
	expr();
}
