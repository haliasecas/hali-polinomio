%{
#include <stdio.h>
#include <stdlib.h>
#include "polinomio_cal.h"
#define code2(c1, c2) code(c1); code(c2);
#define code3(c1, c2, c3) code(c1); code(c2); code(c3);

int yylex(void);
int yyerror(const char*);
extern void init();
extern void initcode();
extern void execute(Inst *);
extern int code(void *);
NodoL *cab;
%}
%union {
	int n;
	NodoL *val;
	Termino *term;
	Polinomio *polino;
	Simbolo *sim;
	Inst *inst;
}

%token <term> TERMINO
%token <sim> ENTERO VAR INDEF BLTIN GEOM
%type <term> termino
%type <val> terminos
%type <polino> expr poli asgn

%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '(' ')'
%%
line
	:
	| line '\n'
	| line asgn '\n' { code2(pop, STOP); return 1; }
	| line expr '\n' { code2(imprime, STOP); return 1; }
	| line error '\n' { yyerrok; }
	;

poli
	: '(' terminos ')' {
		$$ = creaPolinomio(0, cab, 1);
		simplifica($$);
	}
	| terminos {
		$$ = creaPolinomio(0, cab, 1);
		simplifica($$);
	}
	| '-' '(' terminos ')' {
		$$ = creaPolinomio(0, cab, -1);
		simplifica($$);
	}
	;

asgn
	: VAR '=' expr { code3(varpush, (Inst)$1, asigna); }
	;

terminos
	: termino {
		cab = NULL;
		insertaOrdA($1, &cab, cmpTermino);
	}
	| termino terminos {
		insertaOrdA($1, &cab, cmpTermino);
	}
	;

termino
	: TERMINO {}
	;

expr
	: poli {
		simplifica($1); code2(constpush, (Inst)$1;
	}
	| VAR { code3(varpush, (Inst)$1, evalua); }
	| BLTIN '(' expr ',' ENTERO ')' {
		code2(bltin, (Inst)$1->u.f);
	}
	| GEOM '(' ENTERO ')' {
		code2(bltin, (Inst)$1->u.f);
	}
	| expr '+' expr {
		simplifica($1); simplifica($3);
		code(suma);
	}
	| expr '-' expr {
		simplifica($1); simplifica($3);
		code(resta);
	}
	| expr '*' expr {
		simplifica($1); simplifica($3);
		code(multiplica);
	}
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
