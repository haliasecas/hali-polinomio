%{
#include <stdio.h>
#include <stdlib.h>
#include "polinomio_cal.h"

int yylex(void);
int yyerror(const char*);
NodoL *cab;
%}
%union {
	int n;
	NodoL *val;
	Termino *term;
	Polinomio *polino;
	Simbolo *sim;
}

%token <n> ENTERO
%token <term> TERMINO
%token <sim> VAR INDEF BLTIN GEOM
%type <term> termino
%type <val> terminos
%type <polino> expr poli asgn

%right '='
%left '+' '-'
%left '*' '/'
%left UNARYM
%nonassoc '(' ')'
%%
line
	:
	| line '\n'
	| line asgn '\n'
	| line expr '\n' { imprimePolinomio($2); }
	| line error '\n' { yyerrok; }
	;

poli
	: '(' terminos ')' {
		$$ = creaPolinomio(0, cab);
		simplifica($$);
	}
	| terminos {
		$$ = creaPolinomio(0, cab);
		simplifica($$);
	}
	| '-' '(' terminos ')' {
		$$ = niegaPolinomio(creaPolinomio(0, cab));
		simplifica($$);
	}
	;

asgn
	: VAR '=' expr { simplifica($3); $$ = $1->u.poli = $3; $1->tipo = VAR; }
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
	: poli
	| VAR {
		if ($1->tipo == INDEF) {
			puts("Variable indefinida");
			$$ = (Polinomio *)malloc(sizeof(Polinomio));
		}
		else $$ = $1->u.poli;
	}
	| '-' VAR {
		if ($2->tipo == INDEF) {
			puts("Variable indefinida");
			$$ = (Polinomio *)malloc(sizeof(Polinomio));
		}
		else $$ = niegaPolinomio($2->u.poli);
	}
	| ENTERO {
		cab = 0;
		insertaOrdA((void *)creaTermino($1, 0), &cab, cmpTermino);
		$$ = creaPolinomio(0, cab);
	}
	| BLTIN '(' expr ',' ENTERO ')' { $$ = (*($1->u.f))($3, $5); }
	| GEOM '(' ENTERO ')' { $$ = (*($1->u.f))($3); }
	| expr '+' expr { simplifica($1); simplifica($3); $$ = sumaPolinomio($1, $3); simplifica($$); }
	| expr '-' expr { simplifica($1); simplifica($3); $$ = restaPolinomio($1, $3); simplifica($$); }
	| expr '*' expr { simplifica($1); simplifica($3); $$ = multiplicaPolinomio($1, $3); simplifica($$); }
	;
%%
int main() {
	init();
	return yyparse();
}

int yyerror(const char* s) {
	printf("%s\n", s);
	return 0;
}
