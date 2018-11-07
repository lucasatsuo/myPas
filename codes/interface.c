#include <interface.h>

extern size_t linenumber;
extern size_t collummnumber;
extern char *keyword[];


/**
* Funcao que faz a ponte entre o parser e o lexer para identificar tokens no arquivo de entrada
* para serem processados pela gramatica
*/
void match(token_t expected){
	char stringexp[MAXIDLEN], stringlook[MAXIDLEN];

	if(expected == lookahead){
		lookahead = gettoken(source);
	}else{
		/* Ao encontrar um erro de sintaxe, recupera o texto do token translator()
		Exibe o que e esperado e o que foi encontrado
		e a localizacao do erro no codigo fonte */
		translator(stringlook, lookahead);
		translator(stringexp, expected);

		fprintf(stderr,"Token mismatch expected '%s', found '%s'\n", stringexp, stringlook);
		fprintf(stderr,"At line  : %ld\n", linenumber);
		fprintf(stderr,"At column: %ld\n", collummnumber);
		exit(1);
	}
}

/**
* Traduz o valor numerico do token para string com seu significado
*/
void translator(char *translated, token_t original){
	if(original == -1){
		strcpy(translated, "EOF");
	}else if(original < ID){ /* e um caractere ASCII */
		translated[0] = original;
		translated[1] = 0;
	}else if(original < BEGIN){ /* e um dos tokens de tokens.h */
		strcpy(translated, "TOKEN");
	}else{
		strcpy(translated, keyword[original-BEGIN]); /* senao so pode ser uma keyword */
	}
}
