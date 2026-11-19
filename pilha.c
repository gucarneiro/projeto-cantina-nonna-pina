#include "pilha.h"

void pilhaInicializar(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(const Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(const Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaTamanho(const Pilha *p) {
    return p->topo + 1;
}

int pilhaEmpilhar(Pilha *p, ItemPilha x) {
    if (pilhaCheia(p)) return 0;
    p->itens[++p->topo] = x;
    return 1;
}

int pilhaDesempilhar(Pilha *p, ItemPilha *x) {
    if (pilhaVazia(p)) return 0;
    *x = p->itens[p->topo--];
    return 1;
}

int pilhaTopo(const Pilha *p, ItemPilha *x) {
    if (pilhaVazia(p)) return 0;
    *x = p->itens[p->topo];
    return 1;
}

ItemPilha pilhaElemento(const Pilha *p, int k) {
    return p->itens[k];
}