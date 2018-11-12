%{
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "polinomio.h"
#define code2(c1, c2) code(c1); code(c2);
#define code3(c1, c2, c3) code(c1); code(c2); code(c3);

jmp_buf begin;
int yylex(void);
int yyerror(const char*);
extern void init();
extern void initcode();
extern void execute(Inst *);
extern Inst *code(void *);
NodoL *cab;
%}
%union {
	Termino *term;
	Polinomio *polino;
	Simbolo *sim;
	Inst *inst;
}

%token<term> TERMINO
%type <polino> poli terminos termino
%token <sim> PRINT VAR BLTIN1 BLTIN2 INDEF WHILE IF ELSE
%type <inst> stmt asgn expr stmtlist cond while if end

%right '='
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/'
%left UM NOT
%%
line
	:
	| line '\n'
	| line asgn '\n' { code2(pop1, STOP); return 1; }
	| line stmt '\n' { code(STOP); return 1; }
	| line expr '\n' { code2(imprime, STOP); return 1; }
	| line error '\n' { yyerrok; }
	;

poli
	: '(' terminos ')' { $$ = creaPolinomio(0, cab, 1); }
	| terminos { $$ = creaPolinomio(0, cab, 1); }
	;

asgn
	: VAR '=' expr { $$ = $3; code3(varpush, (Inst)$1, asigna); }
	;

stmt
	: expr { code(pop1); }
	| PRINT expr { code(prexpr); $$ = $2; }
	| while cond stmt end {
		($1)[1] = (Inst)$3;
		($1)[2] = (Inst)$4;
	}
	| if cond stmt end {
		($1)[1] = (Inst)$3;
		($1)[3] = (Inst)$4;
	}
	| if cond stmt end ELSE stmt end {
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

terminos
	: termino { cab = NULL; insertaOrdA($1, &cab, cmpTermino); }
	| termino terminos { insertaOrdA($1, &cab, cmpTermino); }
	;

termino
	: TERMINO {}
	;

expr
	: poli { simplifica($1); $$ = code2(constpush, (Inst)$1); }
	| VAR { $$ = code3(varpush, (Inst)$1, evalua); }
	| asgn
	| '(' expr ')' { $$ = $2; }
	| BLTIN2 '(' expr ',' expr ')' { $$ = code2(bltin2, (Inst)$1); }
	| BLTIN1 '(' expr ')' { $$ = $3; code2(bltin1, (Inst)$1); }
	| expr '+' expr { code(suma); }
	| expr '-' expr { code(resta); }
	| expr '*' expr { code(multiplica); }
	| expr GT expr { code(gt); }
	| expr LT expr { code(lt); }
	| '-' expr %prec UM { $$ = $2; code(niega); }
	| NOT expr { $$ = $2; code(not); }
	;
%%
int main() {
	puts("Bienvenido a la calculadora de polinomios");
	setjmp(begin);
	init();
	for (initcode(); yyparse(); initcode())
		execute(prog);

	return 0;
}

void execerror(char *s, char *t) {
	puts(s);
	longjmp(begin, 0);
}

int yyerror(const char* s) {
	printf("%s\n", s);
	return 0;
}
