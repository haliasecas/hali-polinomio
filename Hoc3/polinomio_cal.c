#include <stdio.h>
#include <stdlib.h>
#include "polinomio_cal.h"
#include "polinomio_cal.tab.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

Simbolo *symlist = 0;
NodoL *head = 0;

NodoL *creaNodoL(void *dato, NodoL *sig) {
	NodoL *nvo;
	nvo = (NodoL *)malloc(sizeof(NodoL));
	nvo->dato = dato;
	nvo->sig = sig;
	return nvo;
}

Termino *creaTermino(int coefi, int expo) {
	Termino *nvo;
	nvo = (Termino *)malloc(sizeof(Termino));
	nvo->coefi = coefi;
	nvo->expo = expo;
	return nvo;
}

void imprimeTermino(void *dato, int fin) {
	Termino *t = (Termino *)dato;
	if (t->coefi != 0) {
		if (fin == 0) printf("%+d x^%d ", t->coefi, t->expo);
		else if (fin == 1) printf("%+d x^%d\n", t->coefi, t->expo);
		else printf("%d x^%d ", t->coefi, t->expo);
	}
}

int cmpTermino(void *t1, void *t2) {
	Termino *tp1 = (Termino *)t1;
	Termino *tp2 = (Termino *)t2;
	return tp1->expo - tp2->expo;
}

Polinomio *creaPolinomio(int grado, NodoL *cab, int sgn) {
	Polinomio *nvo = (Polinomio *)malloc(sizeof(Polinomio));
	NodoL *q;
	if (!nvo) {
		puts("no hay memoria para crear Polinomio ");
		return (Polinomio *)NULL;
	}
	nvo->cab = cab;
	nvo->grado = grado;
	for (q = nvo->cab; q; q = q->sig) {
		Termino *tr = (Termino *)q->dato;
		tr->coefi = tr->coefi * sgn;
	}
	return nvo;
}

void simplifica(Polinomio *pol) {
	NodoL *p, *q, *head = pol->cab;
	if (pol) {
		for (p = pol->cab; p; p = p->sig) {
			Termino *act = (Termino *)p->dato;
			Termino *cont;
			if (p->sig) cont = (Termino *)p->sig->dato;
			else break;

			if (act->coefi == 0) {
				if (p->sig) p = p->sig;
				else p = 0;
				continue;
			}

			while (act->expo == cont->expo) {
				act->coefi += cont->coefi;
				p->sig = p->sig->sig;
				if (p->sig) cont = (Termino *)p->sig->dato;
				else break;
			}
		}
	}
	pol->cab = head;
}

NodoL *insertaOrdA(void *dato, NodoL **cab, int ( *cmp)(void *, void *)) {
	NodoL *p, *q;
	if (!(*cab)) {
		*cab = creaNodoL(dato,(NodoL *)NULL);
		return *cab;
	}
	if (cmp((*cab)->dato, dato) >= 0) {
		*cab = creaNodoL(dato, *cab);
		return *cab;
	}
	for (p = *cab,q = p->sig; q && cmp(q->dato, dato) < 0 ;p = q, q = q->sig)
		;
	q = creaNodoL(dato,q);
	p->sig = q;
	return *cab;
}

void imprimeL(NodoL *inicio, void ( *f)(void *, int)) {
	NodoL *p;
	if (inicio) {
		int flag = 2;
		for (p = inicio; p->sig; p = p->sig, flag = 0) f(p->dato, flag);
		if (flag == 2) { f(p->dato, 2); puts(""); }
		else f(p->dato, 1);
	}
}

int igualNodoL(NodoL *cab1, NodoL *cab2, int ( *cmp)(void *, void *)) {
	for (; cab1 && cab2; cab1 = cab1->sig,cab2 = cab2->sig) {
		if (cmp(cab1->dato, cab2->dato)) {
			return 0;
		}
	}
	if (!cab1 && !cab2)
		return 1;
	return 0;
}

Polinomio *sumaPolinomio(Polinomio *p, Polinomio *q) {
	NodoL *suma = NULL;
	NodoL *aux;
	NodoL *aux2;
	Termino *tp1;
	Termino *tp2;
	for (aux = p->cab; aux; aux = aux->sig) {
		tp1 = (Termino *)aux->dato;
		for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
			tp2 = (Termino *)aux2->dato;
			if ((tp1->expo == tp2->expo) ) {
				insertaOrdA(
						(void *)creaTermino(tp1->coefi+tp2->coefi,tp1->expo),
						&suma, cmpTermino);
				tp1->band = 1;
				tp2->band = 1;
			}
		}
		if (tp1->band == 0)
			insertaOrdA((void *)creaTermino(tp1->coefi,tp1->expo),
					&suma, cmpTermino);
	}
	for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
		tp2 = (Termino *)aux2->dato;
		if (tp2->band == 0)
			insertaOrdA((void *)creaTermino(tp2->coefi,tp2->expo),
					&suma, cmpTermino);
	}
	return creaPolinomio(max(p->grado,q->grado), suma, 1);
}

Polinomio *restaPolinomio(Polinomio *p, Polinomio *q) {
	NodoL *resta = NULL;
	NodoL *aux;
	NodoL *aux2;
	Termino *tp1;
	Termino *tp2;
	for (aux = p->cab; aux; aux = aux->sig) {
		tp1 = (Termino *)aux->dato;
		for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
			tp2 = (Termino *)aux2->dato;
			if ((tp1->expo == tp2->expo) ) {
				insertaOrdA(
						(void *)creaTermino(tp1->coefi-tp2->coefi,tp1->expo),
						&resta, cmpTermino);
				tp1->band = 1;
				tp2->band = 1;
			}
		}
		if (tp1->band == 0)
			insertaOrdA((void *)creaTermino(tp1->coefi,tp1->expo),
					&resta, cmpTermino);
	}
	for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
		tp2 = (Termino *)aux2->dato;
		if (tp2->band == 0)
			insertaOrdA((void *)creaTermino(tp2->coefi,tp2->expo), &resta, cmpTermino);
	}
	return creaPolinomio(max(p->grado,q->grado), resta, 1);
}

Polinomio *multiplicaPolinomio(Polinomio *p, Polinomio *q) {
	NodoL *producto = NULL;
	NodoL *aux;
	NodoL *aux2;
	Termino *tp1;
	Termino *tp2;
	for (aux = p->cab; aux; aux = aux->sig) {
		tp1 = (Termino *)aux->dato;
		for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
			tp2 = (Termino *)aux2->dato;
			insertaOrdA((void *)creaTermino(tp1->coefi *tp2->coefi, tp1->expo+tp2->expo), &producto, cmpTermino);
		}
	}
	return creaPolinomio(p->grado+q->grado, producto, 1);
}

int esIgualPolinomio(Polinomio *p, Polinomio *q) {
	return igualNodoL(p->cab, p->cab, cmpTermino);
}

void imprimePolinomio(Polinomio *p) {
	imprimeL(p->cab, imprimeTermino);
}

Polinomio *copiaPolinomio(Polinomio *p) {
	NodoL *copia = NULL;
	NodoL *aux;
	Termino *tp;
	for (aux = p->cab; aux ; aux = aux->sig) {
		tp = (Termino *)aux->dato;
		insertaOrdA((void *)creaTermino(tp->coefi,tp->expo), &copia, cmpTermino);
	}
	return creaPolinomio(p->grado, copia, 1);
}

Simbolo *encontrar(char *s) {
	Simbolo *sp;
	for (sp = symlist; sp != (Simbolo *)0; sp = sp->sig)
		if (!strcmp(s, sp->nombre)) return sp;
	return 0;
}

Simbolo *instalar(char *s, int t, Polinomio *p) {
	Simbolo *sp = (Simbolo *)malloc(sizeof(Simbolo));
	sp->nombre = strdup(s);
	sp->tipo = t;
	sp->u.poli = p;
	sp->sig = symlist;
	symlist = sp;
	return sp;
}

Termino *elevaTermino(Termino *t, int n) {
	int coef = t->coefi, tm = coef;
	int exp = t->expo;
	int N = n - 1;
	while ((N--) > 0) coef = coef * tm;
	exp = exp * n;
	return creaTermino(coef, exp);
}

Termino *multiplicaTermino(Termino *t1, Termino *t2) {
	return creaTermino(t1->coefi * t2->coefi, t1->expo + t2->expo);
}

Polinomio *binomio(Polinomio *p, int n) {
	int tab[2][n + 1], cnt = 0;
	NodoL *nvo = 0;
	for (nvo = p->cab; nvo; nvo = nvo->sig, cnt++);
	if (cnt != 2) return creaPolinomio(0, p->cab, 1);

	Termino *ta = (Termino *)p->cab->dato;
	Termino *tb = (Termino *)p->cab->sig->dato;

	NodoL *rsp = 0;

	if (n == 1) {
		return copiaPolinomio(p);
	}

	else {
		int i, j;
		for (i = 0; i < n + 1; ++i) {
			for (j = 0; j <= i; ++j) {
				if (j == 0 || j == i) tab[i % 2][j] = 1;
				else if ((j - 1) >= 0)
					tab[i % 2][j] = tab[(i + 1) % 2][j] + tab[(i + 1) % 2][j - 1];
			}
		}

		for (j = 0; j <= n; ++j) {
			int mult = tab[(i + 1) % 2][j];
			Termino *nvot = multiplicaTermino(elevaTermino(ta, n - j), elevaTermino(tb, j));
			nvot->coefi = nvot->coefi * mult;
			insertaOrdA((void *)nvot, &rsp, cmpTermino);
		}
	}

	return creaPolinomio(0, rsp, 1);
}

Polinomio *geometrico(int n) {
	NodoL *cab = 0;
	int i;

	for (i = 0; i <= n; ++i)
		insertaOrdA((void *)creaTermino(1, i), &cab, cmpTermino);

	return creaPolinomio(0, cab, 1);
}

void init() {
	Simbolo *s;

	s = instalar("BN", BLTIN, creaPolinomio(0, head, 1));
	s->u.f = binomio;
	s = instalar("GEOM", GEOM, creaPolinomio(0, head, 1));
	s->u.f = geometrico;

}
