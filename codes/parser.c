/**@<parser.c>::**/
#include <parser.h>

void match(token_t expected){
	if(expected == lookahead){
		lookahead = gettoken(source);
	}else{
		printf("Token mismatch expected %d, found %d\n", expected, lookahead);
		exit(1);
	}
}

/***************************************************************************
Syntax definitions for the academic project My Pascal,
which is a simplified / modified Pascal compiler,
according to the following EBNF grammar:

Start symbol:: mypas

Production Rules::

mypas -> PROGRAM ID ; header body .

Recursive LL(1) form
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

Vai ser necessario criar um predicado para controlar o fecho de kleene
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
| smpexpr // + - ( NOT ID UINT FLTP TRUE FALSE 
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
			switch(lookahead){
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

smpexpr -> [ + | - ] fact { otimes fact } { oplus fact { otimes fact }}
// tem que mudar o comentario para ascii art

***************************************************************************/
void smpexpr(void){
	flag_t             isneg  =  0;
	flag_t             otimes =  0;
	flag_t             oplus  =  0;

	oplus = lookahead;
	if (oplus == '+' || oplus == '-') {
	    /*1*/
	    /**
	    * Agenda operação de negação do acc
	    */
	    // isneg = (oplus == '-');/*1*/
		match(oplus);
	}
	// oplus = 0;

	T_begin:

	F_begin:

	factor();

    /*1'*/
    /**
    * Realiza a operação de negação
    */
	// if (isneg) {
		// acc = -acc;
		// isneg = 0;
	// }
	/*1'*/

    /*2'*/
    /**
    *  Realiza a operação de otimes
    */
	// if (otimes) {
		// basicops(otimes);
		// otimes = 0;
	// }
        /*2'*/

	otimes = lookahead;
	if (otimes == '*' || otimes == '/' || otimes == AND || otimes == DIV) {
    	/*2*/
    	/**
    	* Agenda operação de otimes
    	*/
    	// ++sp;stack[sp]=acc;/*2*/
		match(otimes);
		goto F_begin;
	}
	 // else { otimes = 0; }

    /*3'*/
	/**
	*  Realiza a operação de oplus
	*/
	// if (oplus) {
		// basicops(oplus);
		// oplus = 0;
	// }
        /*3'*/

	oplus = lookahead;
	if (oplus == '+' || oplus == '-' || oplus == OR) { // ver se eh isso mesmo ou se tem MOD
    	/*3*/
		/**
		* Agenda operação de oplus
		*/
    	// ++sp;stack[sp]=acc;/*3*/
		match(oplus);
		goto T_begin;            
	}
	 // else { oplus = 0; }
}
/***************************************************************************
// usar a versao melhor que a gente criou no myBC

term -> factor { otimes factor }
***************************************************************************/

/***************************************************************************

factor ->   ID [ ":=" expr  | ( exprlist ) ] // velho
factor ->   ID [ ASGN expr  | ( exprlist ) ] // fazer isso em todos os ":="
  | UINT
  | FLTP
  | ( expr )
***************************************************************************/
void factor(void)
{
	switch (lookahead) {
		case ID:
		match(ID);
			if(lookahead == ASGN){ // ID[=EXPR]
				// r_val
				// coloca a nova variavel na symtab
				// char variable[100];
				// strcpy(variable,lexeme);
				// match('='); expr();
				// STappend(variable,acc);
				match(ASGN);
				expr();
			} else if(lookahead == '(') {
				match('(');
				exprlist();
				match(')');
				// l_val
				// busca o valor do id e poe no acc
				// acc = valtab[STlookup(lexeme)];
			}
			break;

		case UINT:
		case FLTP:
	        /*4*/
			/**
			* Salva o valor do lexeme no acc
			*/
			// acc = atof(lexeme);
			match(lookahead);
	    	/*4*/
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
