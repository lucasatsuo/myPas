/**@<parser.h>::**/

#pragma once

extern token_t lookahead;
void match(token_t);
/** procedures abstracting internal symbols: **/
void mypas(void);
void header(void); 
void body(void);
void varmodel(void);
void procmodel(void);
void funcmodel(void);
void vargroup(void);
void varlist(void); 
void pretype(void);

