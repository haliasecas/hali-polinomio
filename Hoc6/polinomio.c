#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"
#include "polinomio.tab.h"

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

Polinomio *niegaPolinomio(Polinomio *p) {
	NodoL *q = 0;
	Polinomio *nvo = copiaPolinomio(p);
	for (q = nvo->cab; q; q = q->sig) {
		Termino *tr = (Termino *)q->dato;
		tr->coefi = -tr->coefi;
	}
	return nvo;
}

void borraTermino(NodoL *p, NodoL *h) {
	p->sig = h->sig;
}

void imprimeTermino(void *dato, int fin) {
	Termino *t = (Termino *)dato;
	if (t->coefi != 0) {
		if (t->expo != 0) {
			if (fin == 0) printf("%+d x^%d ", t->coefi, t->expo);
			else if (fin == 1) printf("%+d x^%d\n", t->coefi, t->expo);
			else printf("%d x^%d ", t->coefi, t->expo);
		}
		else {
			if (fin == 0) printf("%+d ", t->coefi);
			else if (fin == 1) printf("%+d\n", t->coefi);
			else printf("%d ", t->coefi);
		}
	}
	else if (fin == 1) printf("+0\n");
	else if (fin == 2) printf("0 ");
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
		return (Polinomio *)0;
	}
	nvo->cab = cab;
	nvo->grado = grado;
	for (q = nvo->cab; q; q = q->sig) {
		Termino *tr = (Termino *)q->dato;
		tr->coefi = tr->coefi * sgn;
		nvo->grado = max(nvo->grado, tr->expo);
	}
	simplifica(nvo);
	return nvo;
}

void simplifica(Polinomio *pol) {
	NodoL *p, *cabb = pol->cab;
	Termino *act, *cont;
	int maxgrad = 0;
	if (pol) {
		for (p = pol->cab; p; p = p->sig) {
			act = p->dato;
			if (act->coefi != 0) maxgrad = max(maxgrad, act->expo);

			if (p->sig) cont = p->sig->dato;
			else break;

			while (act->expo == cont->expo) {
				act->coefi += cont->coefi;
				p->sig = p->sig->sig;
				if (p->sig) cont = p->sig->dato;
				else break;
			}
		}

		NodoL *q = pol->cab;
		for (p = pol->cab; p; p = p->sig) {
			act = p->dato;
			if (!act->coefi) borraTermino(q, p);
			q = p;
		}
	}
	pol->grado = maxgrad;
	pol->cab = cabb;
}

NodoL *insertaOrdA(void *dato, NodoL **cab, int (*cmp)(void *, void *)) {
	NodoL *p, *q;
	if (!(*cab)) {
		*cab = creaNodoL(dato, (NodoL *)0);
		return *cab;
	}
	if (cmp((*cab)->dato, dato) >= 0) {
		*cab = creaNodoL(dato, *cab);
		return *cab;
	}
	for (p = *cab,q = p->sig; q && cmp(q->dato, dato) < 0 ;p = q, q = q->sig);
	q = creaNodoL(dato, q);
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
	NodoL *suma = 0;
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
						(void *)creaTermino(tp1->coefi+tp2->coefi,tp1->expo), &suma, cmpTermino);
				tp1->band = 1;
				tp2->band = 1;
			}
		}
		if (tp1->band == 0)
			insertaOrdA((void *)creaTermino(tp1->coefi,tp1->expo), &suma, cmpTermino);
	}
	for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
		tp2 = (Termino *)aux2->dato;
		if (tp2->band == 0)
			insertaOrdA((void *)creaTermino(tp2->coefi,tp2->expo), &suma, cmpTermino);
	}
	return creaPolinomio(max(p->grado,q->grado), suma, 1);
}

Polinomio *restaPolinomio(Polinomio *p, Polinomio *q) {
	NodoL *resta = 0;
	NodoL *aux;
	NodoL *aux2;
	Termino *tp1;
	Termino *tp2;
	for (aux = p->cab; aux; aux = aux->sig) {
		tp1 = (Termino *)aux->dato;
		for (aux2 = q->cab; aux2; aux2 = aux2->sig) {
			tp2 = (Termino *)aux2->dato;
			if ((tp1->expo == tp2->expo) ) {
				insertaOrdA((void *)creaTermino(tp1->coefi - tp2->coefi, tp1->expo), &resta, cmpTermino);
				tp1->band = 1; tp2->band = 1;
			}
		}
		if (tp1->band == 0)
			insertaOrdA((void *)creaTermino(tp1->coefi, tp1->expo), &resta, cmpTermino);
	}
	for (aux2 = q->cab;aux2;aux2 = aux2->sig) {
		tp2 = (Termino *)aux2->dato;
		if (tp2->band == 0)
			insertaOrdA((void *)creaTermino(-tp2->coefi, tp2->expo), &resta, cmpTermino);
	}
	return creaPolinomio(max(p->grado, q->grado), resta, 1);
}

Polinomio *multiplicaPolinomio(Polinomio *p, Polinomio *q) {
	NodoL *producto = 0;
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
	NodoL *copia = 0;
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

Polinomio *binomio(Polinomio *p, Polinomio *d) {
	Termino *ter = (Termino *)(d->cab->dato);
	NodoL *nvo = 0;
	if (!ter) return creaPolinomio(0, nvo, 1);
	int n = ter->coefi;
	int tab[2][n + 1], cnt = 0;
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

Polinomio *geometrico(Polinomio *poli) {
	NodoL *cab = 0;
	int i;
	Termino *term = (Termino *)(poli->cab->dato);
	if (!term) {
		puts("Error");
		return creaPolinomio(0, cab, 1);
	}

	int n = term->coefi;
	for (i = 0; i <= n; ++i)
		insertaOrdA((void *)creaTermino(1, i), &cab, cmpTermino);

	return creaPolinomio(i - 1, cab, 1);
}

Polinomio *deriva(Polinomio *poli) {
	NodoL *cab = 0;
	NodoL *p = 0;
	int coef, expo;

	for (p = poli->cab; p; p = p->sig) {
		Termino *ter = (Termino *)p->dato;
		coef = ter->coefi * ter->expo;
		expo = ter->expo - 1;
		if (ter->expo > 0) insertaOrdA((void *)creaTermino(coef, expo), &cab, cmpTermino);
	}

	Polinomio *rtn = creaPolinomio(expo, cab, 1);
	simplifica(rtn);

	return rtn;
}

#define NSTACK 256
#define NPROG 2000
static Datum stack[NSTACK];
static Datum *stackp;

Inst prog[NPROG];
Inst *pc;

static struct {
	char *nombre;
	int kval;
} keywords[] = {
	{"if", IF},
	{"else", ELSE},
	{"while", WHILE},
	{"print", PRINT},
	{0, 0}
};

void init() {
	Simbolo *s;
	int i;

	s = instalar("BN", BLTIN2, NULL);
	s->u.f2 = binomio;
	s = instalar("GEOM", BLTIN1, 0);
	s->u.f1 = geometrico;
	s = instalar("DER", BLTIN1, creaPolinomio(0, head, 1));
	s->u.f1 = deriva;
	for (i = 0; keywords[i].nombre; ++i)
		instalar(keywords[i].nombre, keywords[i].kval, NULL);
}

void initcode() {
	stackp = stack;
	progp = prog;
}

void push(Datum d) {
	if (stackp >= &stack[NSTACK])
		execerror("Stack overflow", (char *)0);
	*(stackp++) = d;
}

Datum pop() {
	if (stackp < stack)
		execerror("No hay nada en la pila", (char *)0);
	return *(--stackp);
}

void pop1() {
	if (stackp < stack)
		execerror("No hay nada en la pila", (char *)0);
	--stackp;
}

Inst *code(Inst f) {
	// if (f == STOP) puts("Guardando STOP");
	Inst *oprogp = progp;
	if (progp >= &prog[NPROG])
		execerror("Programa muy grande", (char *)0);
	*progp++ = f;
	return oprogp;
}

void execute(Inst *p) {
	for (pc = p; *pc != STOP; ) {
		(*(*pc++))();
	}
}

void constpush() {
	Datum d;
	d.poli = ((Polinomio *)*pc++);
	push(d);
}

void varpush() {
	Datum d;
	d.sim = (Simbolo *)(*pc++);
	push(d);
}

void suma() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.poli = sumaPolinomio(d1.poli, d2.poli);
	push(d1);
}

void resta() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.poli = restaPolinomio(d1.poli, d2.poli);
	push(d1);
}

void multiplica() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.poli = multiplicaPolinomio(d1.poli, d2.poli);
	push(d1);
}

void evalua() {
	Datum d;
	d = pop();
	if (d.sim->tipo == INDEF)
		execerror("Variable indefinida", (char *)0);
	d.poli = d.sim->u.poli;
	push(d);
}

void asigna() {
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sim->tipo != VAR && d1.sim->tipo != INDEF)
		execerror("Asignacion a constante", (char *)0);
	d1.sim->u.poli = d2.poli;
	d1.sim->tipo = VAR;
	push(d2);
}

void imprime() {
	Datum d;
	d = pop();
	if (d.poli) imprimePolinomio(d.poli);
}

Termino *ultimoTerm(Polinomio *p) {
	NodoL *tm = p->cab;
	while (tm->sig) tm = tm->sig;
	return tm->dato;
}

int lessthan(Polinomio *p, Polinomio *q) {
	if (p->grado == q->grado) {
		Termino *up = ultimoTerm(p);
		Termino *uq = ultimoTerm(q);
		return up->coefi < uq->coefi;
	}
	return p->grado < q->grado;
}

int greatthan(Polinomio *p, Polinomio *q) {
	if (p->grado == q->grado) {
		Termino *up = ultimoTerm(p);
		Termino *uq = ultimoTerm(q);
		return up->coefi > uq->coefi;
	}
	return p->grado > q->grado;
}

int equalthan(Polinomio *p, Polinomio *q) {
	NodoL *np, *nq;
	for (np = p->cab, nq = q->cab; np || nq; np = np->sig, nq = nq->sig);
	return np == 0 && nq == 0;
}

void bltin1() {
	Datum d;
	d = pop();
	Simbolo *fun = (Simbolo *)(*pc++);
	d.poli = (fun->u.f1)(d.poli);
	push(d);
}

void bltin2() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	Simbolo *fun = (Simbolo *)(*pc++);
	d1.poli = (fun->u.f2)(d1.poli, d2.poli);
	push(d1);
}

void niega() {
	Datum d;
	d = pop();
	d.poli = niegaPolinomio(d.poli);
	push(d);
}

void gt() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	Termino *t = creaTermino(greatthan(d1.poli, d2.poli), 0);
	d1.poli->cab = creaNodoL(t, 0);
	push(d1);
}

void lt() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	Termino *t = creaTermino(lessthan(d1.poli, d2.poli), 0);
	d1.poli->cab = creaNodoL(t, 0);
	push(d1);
}

void eq() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	Termino *t = creaTermino(d1.poli->grado > d2.poli->grado, 0);
	d1.poli->cab = creaNodoL(t, 0);
	push(d1);
}

void ge() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	Termino *t = creaTermino(greatthan(d1.poli, d2.poli) || equalthan(d1.poli, d2.poli), 0);
	d1.poli->cab = creaNodoL(t, 0);
	push(d1);
}

void le() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	Termino *t = creaTermino(lessthan(d1.poli, d2.poli) || equalthan(d1.poli, d2.poli), 0);
	d1.poli->cab = creaNodoL(t, 0);
	push(d1);
}

void not() {
}

void whilecode() {
	Datum d;
	Inst *savepc = pc;

	execute(savepc + 2);
	d = pop();
	Termino *t = d.poli->cab->dato;
	while (t->coefi) {
		execute(*((Inst **)(savepc)));
		execute(savepc + 2);
		d = pop();
		t = d.poli->cab->dato;

	}
	pc = *((Inst **)(savepc + 1));
}

void ifcode() {
	Datum d;
	Inst *savepc = pc;

	execute(savepc + 3);
	d = pop();
	Polinomio *poli = d.poli;
	Termino *term = (Termino *)(poli->cab->dato);
	Inst *tin = *((Inst **)(savepc));

	if (term->coefi) execute(tin);
	else if (*((Inst **)(savepc + 1))) execute(*((Inst **)(savepc + 1)));
	pc = (*((Inst **)(savepc + 2)));
}

void prexpr() {
	Datum d;
	d = pop();
	imprimePolinomio(d.poli);
}
