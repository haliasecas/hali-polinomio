#include <string.h>

typedef struct nodoL {
	void *dato;
	struct nodoL *sig;
} NodoL;

typedef struct termino {
	int coefi;
	unsigned expo;
	unsigned band;
} Termino;

typedef struct polinomio {
	int grado;
	NodoL *cab;
} Polinomio;
typedef struct polinomio *PolinomioAP;

NodoL *creaNodoL(void *, NodoL *);
NodoL *insertaOrdA(void *, NodoL **, int (*)(void *, void *));
void imprimeL(NodoL *inicio, void (*f)(void *, int));
int igualNodoL(NodoL *cab1, NodoL *, int (*cmp)(void *, void *));
Termino *creaTermino(int coefi, int);
void imprimeTermino(void *dato, int);
int cmpTermino(void *t1, void *t2);
Polinomio *creaPolinomio(int grado, NodoL *cab, int sgn);
void simplifica(Polinomio *cab);

Polinomio *sumaPolinomio(Polinomio *r, Polinomio *s);
Polinomio *restaPolinomio(Polinomio *r, Polinomio *s);
Polinomio *multiplicaPolinomio(Polinomio *r, Polinomio *s);
int esIgualPolinomio(Polinomio *r, Polinomio *s);
void imprimePolinomio(Polinomio *p);
Polinomio *copiaPolinomio(Polinomio *r);
//#define YYSTYPE PolinomioAP
