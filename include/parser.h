/**@parser.h::**/
#include <stdio.h>
#include <stdlib.h>
#include <mytype.h>
#include <keywords.h>
#include <tokens.h>
#include <interface.h>

void mypas(void);
void header(void); 
void body(void);
void procmodel(void);
void funcmodel(void);
void varmodel(void);
void vargroup(void);
void varlist(void);
void pretype(void);
void procmodel(void);
void funcmodel(void);
void formalparms(void);
void arglist(void);
void argmodel(void);
void body(void);
void stmtlist(void);
void stmt(void);
void ifstmt(void);
void expr(void);
void smpexpr(void);
void factor(void);
void exprlist(void);
void whlstmt(void);
void repstmt(void);

extern FILE *source;
