/**@<lexer.c>::**/
#include <lexer.h>

/* variaveis que serao usadas para fazer contagem das linhas e colunas no arquivo 
de entrada para apresenta-las ao encontrar um erro */
size_t linenumber = 1;
size_t collummnumber = 1;

/*
ignoreneutrals: 

            +-------+
-->----->-- |isspace| --.-----.--------------->-----------------.-->-- end
  |     |   +-------+   |     V                                 ^
  |     ^               V     '->- ('{') -.---->---.- ('}') ->--|   
  |     '-------<-------'                 |        |            |         
  ^                                       ^        V            V
  |                                       '-(^'}')-'            |
  |                                                             |
  '-----------------------------<-------------------------------'

*
* ignora caracteres que nao serao computados
* entrada: arquivo de entrada
* retorna: 0 ou -1 se encontrar EOF prematuro
*/
token_t ignoreneutrals(FILE * tape){
    int head;
_ignoreneutrals_start:

/* collummnumber sincroniza a cada getc() e ungetc() 
   linenumber atualiza a contagem a cada \n encontrado */
    while (collummnumber++ && isspace(head = getc(tape))) { /* ignora brancos*/
        if (head == '\n') {
            linenumber++;
            collummnumber = 1;
        }
    }
    if ( head == '{' ) { /* ignora comentarios */
        while ( collummnumber++ && (head = getc(tape)) != '}' ) {
            if (head == EOF) {
                return EOF;
            }
            if (head == '\n') {
                linenumber++;
                collummnumber = 1;
            }
        }
        goto _ignoreneutrals_start;
    }
    ungetc(head, tape);
    collummnumber--;
    return 0;
}

/**
* Predicado para checar se ha a string de atribuicao
* retornando o token ASGN se houver 
* 0 se nao for identificada a atribuicao 
*/
token_t isASGN(FILE * tape)
{
    /* 
    * lexeme e usado como backup dos caracteres lidos na string de entrada
    * caso a string nao seja aceita, todos os caracteres lidos sao devolvidos.
    */
    if ( (lexeme[0] = getc(tape)) == ':' ) {
        if ( (lexeme[1] = getc(tape)) == '=' ) {
            lexeme[2] = 0;
            collummnumber += 2;
            return ASGN;
        }
        ungetc(lexeme[1], tape);
    }
    ungetc(lexeme[0], tape);
    return lexeme[0] = 0;
}


/**
* Predicado que identifica e retorna tokens de operadores relacionais
* ou 0 caso nao encontre.
* isRELOP -> LEQ | NEQ | GEQ | > | < | =
*/
token_t isRELOP(FILE * tape)
{
    switch (lexeme[0] = getc(tape)) {
    case'<':
        lexeme[1] = getc(tape);
        lexeme[2] = 0;
        collummnumber += 2;
        switch (lexeme[1]) {
        case'=':
            return LEQ;
        case'>':
            return NEQ;
        }
        ungetc(lexeme[1], tape);
        lexeme[1] = 0;
        collummnumber--;
        return lexeme[0];
    case'>':
        lexeme[1] = getc(tape);
        collummnumber += 2;
        if (lexeme[1] == '=') {
            lexeme[2] = 0;
            return GEQ;
        } 
        ungetc(lexeme[1], tape);
        lexeme[1] = 0;
        collummnumber--;
        return lexeme[0];
    case'=':
        lexeme[1] = 0;
        collummnumber++;
        return lexeme[0];
    }
    ungetc(lexeme[0], tape);
    return lexeme[0] = 0;
}

/** checa se ha parte exponencial em um numero
* entradas: arquivo fonte e indice do lexeme atual, garantindo que lexeme nao sera reescrito
* saida: indice final apos identificacao da parte exponencial
*
* funcionamento definido pela regex:
* EE: [eE] ['+'|'-']? FRAC
* FRAC: [0-9]+
* 
**/
int
chk_EE(FILE * tape, int i0)
{
    int             i = i0;
    i++; /* deixa i sempre na ultima posicao vazia */
    if ( (lexeme[i] = toupper(getc(tape)) ) == 'E') {
        i++;
        /** abstrai ['+'|'-'] **/
        if ((lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-') {
            i++;
        } else {
            ungetc(lexeme[i], tape);
        }

        /** abstrai [0-9]+ **/
        if (isdigit(lexeme[i] = getc(tape))) {
            i++;
            while (isdigit(lexeme[i] = getc(tape)))
                i++;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            i--;
            return i;
        } else {
            /** o que foi lido NAN **/
            for (; i > i0; i--) {
                ungetc(lexeme[i], tape);
            }
            return i0;
        }
    }

    /** nao foi encontrada parte exponencial, tudo que foi lido e devolvido **/
    while(i>i0)
        ungetc(lexeme[i--], tape);
    return i0;

}

/**
* retorna token=uint ou token=fltp se for um numero
* retorna 0 caso contrario
*
* isNUM -> UINT | FLTP 
*/
token_t
isNUM(FILE * tape)
{
    int i = 0;
    token_t token = 0;

    int head = getc(tape);
    /** parte inteira **/
    if (isdigit(head)) {
        lexeme[i] = head;
        token = UINT;
        if (lexeme[i] == '0') {
            ;
        } else {
            while (isdigit(lexeme[++i] = getc(tape)));
            ungetc(lexeme[i], tape);
        }
    }else{ ungetc(head, tape); }
    /** parte nao inteira **/
    if ((head = getc(tape)) == '.') {
        lexeme[i] = head;
        if (token == UINT) {
            token = FLTP;
        } else {
            if (isdigit(lexeme[++i] = getc(tape))) {
                token = FLTP;
            } else {
                /** NAN **/
                ungetc(lexeme[i], tape);
                ungetc('.', tape);
                collummnumber += i;
                return token;
            }
        }
        while (isdigit(lexeme[++i] = getc(tape)));
        ungetc(lexeme[i], tape); i--;
    }else{
        ungetc(head, tape); i--;
    }
    if (token == 0) {
        /** nao e um numero, devolve a entrada. **/
        for (i; i > 0; --i)
        {
            ungetc(lexeme[i], tape);
        }
        return 0;
    }

    /** neste ponto ja temos um numero UINT ou FLTP 
        verifica se ha parte exponencial. 
        Atualiza o contador de colunas **/
    i = chk_EE(tape, i);
    lexeme[++i]=0;
    collummnumber += i;

    return token;
}

token_t isID(FILE * tape)
{
    /**
    * checa se a entrada tem o padrao de identificadores pascal
    * definido pela regex: [a-z|A-Z][a-z|A-Z|0-9|\_]*
    */ 
    int i = 0;
    int token;

    if (isalpha(lexeme[i] = getc(tape))) {
        i++;
        while (isalnum(lexeme[i] = getc(tape)) || lexeme[i] == '_')
            i++;
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        /* para em seguida verificar se e uma palavra reservada */
        token = iskeyword(lexeme);
        
        /* finalmente, atualiza o valor de collummnumber */
        if (token){
            collummnumber += i;
            return token;
        }
        collummnumber += i;
        /* retorna token ID se nao for uma palavra reservada */
        return ID;
    }

    ungetc(lexeme[i], tape);
    return 0;
}


token_t gettoken(FILE * tape)
{
    int             token;
    ignoreneutrals(tape);
    if(token = isID(tape))
        return token;
    if(token = isASGN(tape))
        return token;
    if(token = isRELOP(tape))
        return token;
    if(token = isNUM(tape))
        return token;

    /* atualiza collummnumber quando token lido for apenas um caractere */
    token = getc(tape);
    collummnumber++;
    return token;
}

