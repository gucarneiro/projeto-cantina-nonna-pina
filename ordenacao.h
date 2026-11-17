#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "delivery.h"

typedef struct {
	long long comparacoes;
	long long trocas;
	double tempoMs;
} Metricas;

int comparar(const Pedido *a, const Pedido *b, int criterio, Metricas *m);
void trocar(Pedido *a, Pedido *b, Metricas *m);
void bubbleSort(Pedido v[], int n, int crit, Metricas *m);
void selectionSort(Pedido v[], int n, int crit, Metricas *m);
void quickSort(Pedido v[], int n, int crit, Metricas *m);

#endif