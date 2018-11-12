%option noyywrap
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "polinomio.h"
	#include "polinomio.tab.h"
	void RmWs(char* str);
	extern YYSTYPE yylval;
	NodoL *cab = 0;
%}
op [-=+*/(),]
ws [ \t]+
digits [0-9]
number (0|[1-9]+{digits}*)
im [i]
fr [+]
termino {ws}*[+|-]?{ws}*{number}{ws}*x[+|-]*{ws}*{number}{ws}*
termino1 {ws}*{number}{ws}*
polinomio {im}{termino1}*{im}
variable [a-zA-Z]+({digits}|[a-zA-Z])*
llaves [{}]
lt <
gt >
%%
{fr} { return *yytext; }
{number} {
	RmWs(yytext);
	// puts("Number");
	// puts(yytext);
	yylval.term = creaTermino(atoi(yytext), 0);
	return TERMINO;
}

{termino} {
	int coefi, expo;
	RmWs(yytext);
	// puts("Termino");
	// puts(yytext);
	sscanf(yytext,"%dx%d", &coefi, &expo);
	yylval.term = creaTermino(coefi, expo);
	return TERMINO;
}

{variable} {
	Simbolo *sp;
	if ((sp = encontrar(yytext)) == (Simbolo *)0)
		sp = instalar(yytext, INDEF, 0);
	yylval.sim = sp;
	// puts("Var");
	// puts(yytext);
	if (sp->tipo == INDEF) return VAR;
	return sp->tipo;
}

{op} { return *yytext; }
\n { return '\n'; }
{llaves} { return *yytext; }
{lt} { return LT; }
{gt} { return GT; }

. {}
%%
void RmWs(char* str) {
	int i = 0, j = 0;
	char *temp = strdup(str);
	while (temp[i] != '\0') {
		while (temp[i] == ' ')
			i++;
		str[j] = temp[i];
		i++;
		j++;
	}
	str[j] = '\0';
}
