#include "ordenacao.h"

int comparar(const Pedido *a, const Pedido *b, int criterio, Metricas *m) {
	int r = 0;
	m->comparacoes++;
	if (criterio == 1) {
		r = (a->valor < b->valor) - (a->valor > b->valor);
	} else if (criterio == 2) {
		r = (a->distanciaKm > b->distanciaKm) - (a->distanciaKm < b->distanciaKm);
	} else if (criterio == 3) {
		r = a->horario - b->horario;
	}
	if (r== 0 ) {
		r = a->numero - b->numero;
	}
	return r;
}
void trocar(Pedido *a, Pedido *b, Metricas *m) {
	Pedido aux = *a;
	*a = *b;
	*b = aux;
	m->trocas++;
}
void bubbleSort(Pedido v[], int n, int crit, Metricas *m) {
	int i, j, trocou;
	for (i = 0; i < n - 1; i++) {
		trocou = 0;
		for (j = 0; j < n - 1 - i; j++)
			if (comparar(&v[j], &v[j + 1], crit, m) > 0) {
				trocar(&v[j], &v[j + 1], m);
				trocou = 1;
			}
		if (!trocou) break; /* já ordenado: melhor caso O(n) */
	}
}
void selectionSort(Pedido v[], int n, int crit, Metricas *m) {
	int i, j, menor;
	for (i = 0; i < n - 1; i++) {
		menor = i;
		for (j = i + 1; j < n; j++)
			if (comparar(&v[j], &v[menor], crit, m) < 0) menor = j;
		if (menor != i) trocar(&v[i], &v[menor], m);
	}
}
static int particionar(Pedido v[], int ini, int fim, int crit, Metricas *m) {
	int i = ini - 1, j; /* pivô = v[fim] */
	for (j = ini; j < fim; j++)
		if (comparar(&v[j], &v[fim], crit, m) <= 0) {
			i++;
			if (i != j) trocar(&v[i], &v[j], m);
		}
	if (i + 1 != fim) trocar(&v[i + 1], &v[fim], m);
	return i + 1;
}
static void quickRec(Pedido v[], int ini, int fim, int crit, Metricas *m) {
	if (ini < fim) {
		int p = particionar(v, ini, fim, crit, m);
		quickRec(v, ini, p - 1, crit, m);
		quickRec(v, p + 1, fim, crit, m);
	}
}
void quickSort(Pedido v[], int n, int crit, Metricas *m) {
	quickRec(v, 0, n - 1,
	         crit, m);
}