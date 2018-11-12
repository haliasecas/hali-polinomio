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

typedef struct simbolo {
	char *nombre;
	int tipo;
	union {
		Polinomio *poli;
		Polinomio *(*f1)(Polinomio *);
		Polinomio *(*f2)(Polinomio *, Polinomio *);
	} u;
	struct simbolo *sig;
} Simbolo;

typedef union Datum {
	double val;
	Polinomio *poli;
	Simbolo *sim;
} Datum;

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
Polinomio *deriva(Polinomio *);

Simbolo *encontrar(char *s);
Simbolo *instalar(char *s, int t, Polinomio *p);
Polinomio *binomio(Polinomio *, Polinomio *);
void init();

extern Datum pop();
extern void pop1();
extern void execerror(char *, char *);
typedef void (*Inst)();
Inst *progp;
#define STOP (Inst) 0

extern Inst prog[];
extern void evalua(), suma(), resta(), multiplica();
extern void niega(), power(), prexpr();
extern void asigna(), bltin1(), imprime();
extern void constpush(), varpush(), bltin2(), ifcode(), whilecode();
extern void gt(), lt(), eq(), ge(), le(), ne(), and(), or(), not();
