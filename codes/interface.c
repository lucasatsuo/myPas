#include <string.h>
#include <interface.h>
#include <lexer.h>

extern size_t linenumber;
extern size_t collummnumber;
extern char *keyword[];

void translator(char *translated, token_t original);


/*****************************************************
*
* o contador de colunas conta ate o caractere \n o que faz com que 
* uma linha vazia tenha o tamanho 2, talvez nao cause problemas 
* Uma solucao seria printar collummnumber-1 na linha 37
*
* como nao temos um dicionario com os tokens, como temos com keywords
* o compilador aponta que foi esperado um token, mas sem dizer qual e
* deveriamos adicionar esse vetorzao como ha em keywords.c ?
*
* acho que isso e tudo
*
*****************************************************/

void match(token_t expected){
	char charexp[MAXIDLEN], charlook[MAXIDLEN];

	if(expected == lookahead){
		lookahead = gettoken(source);
	}else{
		translator(charlook, lookahead);
		translator(charexp, expected);

		printf("Token mismatch expected '%s', found '%s'\n", charexp, charlook);
		printf("At line  : %d\n", linenumber);
		printf("At column: %d\n",collummnumber);
		exit(1);
	}
}

void translator(char *translated, token_t original){
	if(original == -1){
		strcpy(translated, "EOF");
	}else if(original < 1024){
		translated[0] = original;
		translated[1] = 0;
	}else if(original < 8192){
		strcpy(translated, "TOKEN");
	}else{
		strcpy(translated, keyword[original-8192]);
	}
}
