#ifndef PILHA_H
#define PILHA_H

#define MAX_PILHA 10

typedef int ItemPilha; /* índice no array de pedidos */

typedef struct {
    ItemPilha itens[MAX_PILHA];
    int topo;
} Pilha;

void pilhaInicializar(Pilha *p);
int pilhaVazia(const Pilha *p);
int pilhaCheia(const Pilha *p);
int pilhaEmpilhar(Pilha *p, ItemPilha x);     /* 1 = ok, 0 = cheia */
int pilhaDesempilhar(Pilha *p, ItemPilha *x); /* 1 = ok, 0 = vazia */
int pilhaTopo(const Pilha *p, ItemPilha *x);  /* consulta sem remover */
int pilhaTamanho(const Pilha *p);
ItemPilha pilhaElemento(const Pilha *p, int k); /* k = 0 é a base */

#endif