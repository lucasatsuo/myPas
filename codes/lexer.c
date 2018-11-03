/**@<lexer.c>::**/
#include <lexer.h>

size_t linenumber = 1;

token_t ignoreneutrals(FILE * tape){
    int head;

_ignoreneutrals_start:
    while (isspace(head = getc(tape))) {
	    if (head == '\n') {
		    linenumber++;
	    }
    }
    if ( head == '{' ) {
	    while ( (head = getc(tape)) != '}' ) {
		    if (head == EOF) {
			    return EOF;
		    }
		    if (head == '\n') {
			    linenumber++;
		    }
	    }
	    goto _ignoreneutrals_start;
    }
    ungetc(head, tape);
    return 0;
}

token_t isASGN(FILE * tape)
{
	if ( (lexeme[0] = getc(tape)) == ':' ) {
		if ( (lexeme[1] = getc(tape)) == '=' ) {
			lexeme[2] = 0;
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	return lexeme[0] = 0;
}

token_t isRELOP(FILE * tape)
{
	switch (lexeme[0] = getc(tape)) {
	case'<':
		lexeme[1] = getc(tape);
		lexeme[2] = 0;
		switch (lexeme[1]) {
		case'=':
			return LEQ;
		case'>':
			return NEQ;
		}
		ungetc(lexeme[1], tape);
		lexeme[1] = 0;
		return lexeme[0];
	case'>':
		lexeme[1] = getc(tape);
		if (lexeme[1] == '=') {
			lexeme[2] = 0;
			return GEQ;
		} 
		ungetc(lexeme[1], tape);
		lexeme[1] = 0;
		return lexeme[0];
	case'=':
		lexeme[1] = 0;
		return lexeme[0];
	}
	ungetc(lexeme[0], tape);
	return lexeme[0] = 0;
}

/** EE = [eE] ['+''-']? FRAC

    FRAC = [0-9]+

    **/
int
chk_EE(FILE * tape, int i0)
{
    /** using lexeme[i] as a subtape to easy possible backtracking **/
    int             i = i0;
    i++; // deixa i sempre na ultima posicao + 1
    if ( (lexeme[i] = toupper(getc(tape)) ) == 'E') {
        i++;
        /** abstracts ['+''-'] **/
        if ((lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-') {
            i++;
        } else {
            ungetc(lexeme[i], tape);
        }

        /** abstracts [0-9]+ **/
        if (isdigit(lexeme[i] = getc(tape))) {
            i++;
            while (isdigit(lexeme[i] = getc(tape)))
                i++;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
         return i;
        } else {
            /** reaching out this line means this is NAN **/
            for (; i > i0; i--) {
                ungetc(lexeme[i], tape);
            }
            return i0;
        }
    }

    /** at this point, not even an EE-prefix was found **/
    while(i>i0)
        ungetc(lexeme[i--], tape);
    return i0;

}

// retorna token=uint ou token=fltp se for um numero
// retorna 0 caso contrario
/**
* isNUM -> UINT | FLTP 
*/
token_t
isNUM(FILE * tape)
{
    int i = 0;
    token_t token = 0;

    int head = getc(tape);
    /** integer part **/
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
                return token;
            }
        }
        while (isdigit(lexeme[++i] = getc(tape)));
        ungetc(lexeme[i], tape); i--;
    }else{
        ungetc(head, tape); i--;
    }
    if (token == 0) {
        /** this is not a number **/
        for (i; i > 0; --i)
        {
            ungetc(lexeme[i], tape);
        }
        return 0;
    }

    /** at this point we got a number, either UINT or FLTP **/
    /** check scientific notation: **/
    i = chk_EE(tape, i);
    lexeme[++i]=0;

    return token;
}




token_t isID(FILE * tape)
{
    int i = 0;
    int token;

    if (isalpha(lexeme[i] = getc(tape))) {
        i++;
        while (isalnum(lexeme[i] = getc(tape)))
            i++;
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        token = iskeyword(lexeme);
        if (token)
            return token;
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

    return token = getc(tape);
}
