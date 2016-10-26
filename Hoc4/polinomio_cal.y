%{
#include <stdio.h>
#include <stdlib.h>
#include "polinomio_cal.h"
#define code2(c1, c2) code(c1); code(c2);
#define code3(c1, c2, c3) code(c1); code(c2); code(c3);
//#define TEST

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

%token <term> TERMINO
%token <sim> VAR INDEF BLTIN1
%token <sim> BLTIN2
%type <term> termino
%type <val> terminos
%type <polino> expr poli asgn

%right '='
%left '+' '-'
%left '*' '/'
%left UM
%nonassoc '(' ')'
%%
line
	:
	| line '\n'
	| line asgn '\n' {
		code2(pop, STOP);
		#ifdef TEST
			puts("POP1"); puts("STOP");
		#endif
		return 1;
	}
	| line expr '\n' {
		code2(imprime, STOP);
		#ifdef TEST
			puts("IMPRIME"); puts("STOP");
		#endif
		return 1;
	}
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
	;

asgn
	: VAR '=' expr {
		code3(varpush, (Inst)$1, asigna);
		#ifdef TEST
			puts("VARPUSH"); puts("$1"); puts("ASIGNA");
		#endif
	}
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
		simplifica($1);
		code2(constpush, (Inst)$1);
		#ifdef TEST
			puts("CONSTPUSH"); puts("$1");
		#endif
	}
	| VAR {
		code3(varpush, (Inst)$1, evalua);
		#ifdef TEST
			puts("VARPUSH"); puts("$1"); puts("EVALUA");
		#endif
	}
	| BLTIN2 '(' expr ',' expr ')' {
		code2(bltin2, (Inst)$1);
		#ifdef TEST
			puts("BLTIN2"); puts("INST$1");
		#endif
	}
	| BLTIN1 '(' expr ')' {
		code2(bltin1, (Inst)$1);
		#ifdef TEST
			puts("BLTIN1"); puts("INST$1");
		#endif
	}
	| expr '+' expr {
		code(suma);
		#ifdef TEST
			puts("SUMA");
		#endif
	}
	| expr '-' expr {
		code(resta);
		#ifdef TEST
			puts("RESTA");
		#endif
	}
	| expr '*' expr {
		code(multiplica);
		#ifdef TEST
			puts("MULTIPLICA");
		#endif
	}
	| '-' expr %prec UM {
		code(niega);
		#ifdef TEST
			puts("NIEGA");
		#endif
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
