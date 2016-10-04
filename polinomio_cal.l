%option noyywrap
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "polinomio_cal.h"
	#include "polinomio_cal.tab.h"
	void RmWs(char* str);
	extern YYSTYPE yylval;
	NodoL *cab = 0;
%}
/* Add your Flex definitions here */
/* Some definitions are already provided to you*/
op [-+*/()=]
ws [ \t]+
digits [0-9]
number (0|[1-9]+{digits}*)
im [i]
fr [+]
termino {ws}*[+|-]*{ws}*{number}{ws}*x[+|-]*{ws}*{number}{ws}* 
termino1 {ws}*{number}{ws}*
polinomio {im}{termino1}*{im} { /*return POLINOMIO ;*/}
variable [a-zA-Z]+({digits}|[a-zA-Z])*
%%
{termino} {
	int coefi, expo;
	RmWs(yytext); 
	sscanf(yytext,"%dx%d", &coefi, &expo); 
	yylval.term = creaTermino(coefi, expo);
	return TERMINO;
}

{variable} {
	Simbolo *sp;
	if ((sp = encontrar(yytext)) == (Simbolo *)0)
		sp = instalar(yytext, INDEF, creaPolinomio(0, cab, 1));
	yylval.sim = sp;
	if (sp->tipo == INDEF) return VAR;
	return sp->tipo;
}

{op} |
\n {}

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

