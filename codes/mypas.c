/**@<mypas.c>::**/
#include <mypas.h>

void main(void){
	source = stdin;
	object = stdout;
	lookahead = gettoken(source);
	mypas();
}