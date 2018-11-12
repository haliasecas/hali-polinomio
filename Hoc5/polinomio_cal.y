%{
#include <stdio.h>
#include <stdlib.h>
#include "polinomio_cal.h"
#define code2(c1, c2)		code(c1); code(c2);
#define code3(c1, c2, c3)	code(c1); code(c2); code(c3);

int yylex(void);
int yyerror(const char*);
extern void init();
extern void initcode();
extern void execute(Inst *);
extern int code(void *);
NodoL *cab;
%}
%union {
	NodoL *val;
	Termino *term;
	Polinomio *polino;
	Simbolo *sim;
	Inst *inst;
}

%token <sim> VAR INDEF TERMINO BLTIN2 BLTIN1 PRINT WHILE IF ELSE
%type <term> termino
%type <val> terminos
%type <polino> poli
%type <inst> stmt stmtlist cond while if end expr asgn

%right '='
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/'
%left UM NOT
%nonassoc '(' ')'
%%
line
	:
	| line '\n'
	| line asgn '\n' { code2(pop, STOP); return 1; }
	| line stmt '\n' { code(STOP); return 1; }
	| line expr '\n' { code2(imprime, STOP); return 1; }
	| line error '\n' { yyerrok; }
	;

terminos
	: termino { cab = NULL; insertaOrdA($1, &cab, cmpTermino); }
	| termino terminos { insertaOrdA($1, &cab, cmpTermino); }
	;

termino
	: TERMINO {}
	;

poli
	: '(' terminos ')' { $$ = creaPolinomio(0, cab, 1); simplifica($$); }
	| terminos { $$ = creaPolinomio(0, cab, 1); simplifica($$); }
	;

asgn
	: VAR '=' expr { $$ = $3; code3(varpush, (Inst)$1, asigna); }
	;

stmt
	: expr { code(pop); }
	| PRINT expr { code(prexpr); $$ = $2; }
	| while cond stmt end {
		($1)[1] = (Inst)$3;
		($1)[2] = (Inst)$4;
	}
	if cond stmt end {
		($1)[1] = (Inst)$3;
		($1)[2] = (Inst)$4;
	}
	if cond stmt end ELSE stmt end {
		($1)[1] = (Inst)$3;
		($1)[2] = (Inst)$6;
		($1)[3] = (Inst)$7;
	}
	| '{' stmtlist '}' { $$ = $2; }
	;

cond
	: '(' expr ')' { code(STOP); $$ = $2; }
	;

while
	: WHILE { $$ = code3(whilecode, STOP, STOP); }
	;

if
	: IF { $$ = code(ifcode); code3(STOP, STOP, STOP); }
	;

end
	: { code(STOP); $$ = progp; }
	;

stmtlist
	: { $$ = progp; }
	| stmtlist '\n'
	| stmtlist stmt
	;

expr
	: poli { simplifica($1); $$ = code2(constpush, (Inst)$1); }
	| VAR { $$ = code3(varpush, (Inst)$1, evalua); }
	| asgn
	| BLTIN2 '(' expr ',' expr ')' { $$ = $3; code2(bltin2, (Inst)$1); }
	| BLTIN1 '(' expr ')' { $$ = $3; code2(bltin1, (Inst)$1); }
	| expr '+' expr { code(suma); }
	| expr '-' expr { code(resta); }
	| expr '*' expr { code(multiplica); }
	| '-' expr %prec UM { code(niega); }
	| expr GT expr { code(gt); }
	| expr GE expr { code(ge); }
	| expr LT expr { code(lt); }
	| expr LE expr { code(le); }
	| expr EQ expr { code(eq); }
	| expr NE expr { code(ne); }
	| expr AND expr { code(and); }
	| expr OR expr { code(or); }
	| NOT expr { $$ = $2; code(not); }
	;
%%
int main() {
	init();
	for (initcode(); yyparse(); initcode())
		execute(prog);

	return 0;
}

int yyerror(const char* s) {
	printf("%s\n", s);
	return 0;
}
