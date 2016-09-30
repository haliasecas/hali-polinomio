%{
#include <stdio.h>
#include <stdlib.h>
#include "polinomio_cal.h"

//gcc lex.yy.c polinomio_cal.c y.tab.c
// ./a.out (33x0+34x3+56x7)+(22x0+44x2+77x10)
int yylex(void);
int yyerror(const char*);
NodoL *cab;
%}
%union {
	NodoL *val;
	Termino *term;
	Polinomio *polino;
	Simbolo *sim;
}

%token <term> TERMINO
%token <sim> VAR INDEF
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
	| line asgn '\n'
	| line expr '\n' { imprimePolinomio($2); }
	;

poli
	: '(' terminos ')' { 
		$$ = creaPolinomio(0, cab, 1);
		simplifica($$);
	}
	| '-' '(' terminos ')' { 
		$$ = creaPolinomio(0, cab, -1);
		simplifica($$);
	}
	;

asgn
	: VAR '=' expr { $$ = $1->u.poli = $3; $1->tipo = VAR; }
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
		if ($1->tipo == INDEF) puts("Variable indefinida");
		else $$ = $1->u.poli;
	}
	| expr '+' expr { simplifica($1); simplifica($3); $$ = sumaPolinomio($1, $3); simplifica($$); }
	| expr '-' expr { simplifica($1); simplifica($3); $$ = restaPolinomio($1, $3); simplifica($$); }
	| expr '*' expr { simplifica($1); simplifica($3); $$ = multiplicaPolinomio($1, $3); simplifica($$); }
	; 
%%
int main() { return yyparse(); }

int yyerror(const char* s) { 
	printf("%s\n", s); 
	return 0; 
}
