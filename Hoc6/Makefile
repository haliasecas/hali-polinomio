Gram=polinomio.tab.c polinomio.tab.h

all: $(Gram) lex.yy.c polinomio.c
	@gcc -c polinomio.c -Wall
	@gcc -o Polinomio polinomio.tab.c lex.yy.c polinomio.o
	@echo Compilado

polinomio.c:
	@echo Si esta

$(Gram): polinomio.y
	@bison -d polinomio.y

lex.yy.c: polinomio.l
	@flex polinomio.l

clean:
	@rm -f *.out lex.yy.c Polinomio *tab* polinomio.o
	@echo Limpio
