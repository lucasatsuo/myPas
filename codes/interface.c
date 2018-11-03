#include <interface.h>

void match(token_t expected){
	if(expected == lookahead){
		lookahead = gettoken(source);
	}else{
		printf("Token mismatch expected %d, found %d\n", expected, lookahead);
		exit(1);
	}
}