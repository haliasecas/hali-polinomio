Gram=polinomio_cal.tab.c polinomio_cal.tab.h

all: $(Gram) lex.yy.c polinomio_cal.c
	@gcc -o Polinomio polinomio_cal.tab.c lex.yy.c polinomio_cal.c
	@echo Compiled

polinomio_cal.c:
	@echo "Si esta"

$(Gram): polinomio_cal.y
	@bison -d polinomio_cal.y

lex.yy.c: polinomio_cal.l
	@flex polinomio_cal.l

clean:
	@rm -f *.out lex.yy.c *.tab.* Polinomio
	@echo Clean
