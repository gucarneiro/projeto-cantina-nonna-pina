#include "fila.h"
void filaInicializar(Fila *f) { f->inicio = 0; f->fim = 0; f->qtd = 0; }
int filaVazia(const Fila *f) { return f->qtd == 0; }
int filaCheia(const Fila *f) { return f->qtd == MAX_FILA; }
int filaTamanho(const Fila *f) { return f->qtd; }
int filaEnfileirar(Fila *f, ItemFila x) {
if (filaCheia(f)) return 0;
f->itens[f->fim] = x;
f->fim = (f->fim + 1) % MAX_FILA;
f->qtd++;
return 1;
}
int filaDesenfileirar(Fila *f, ItemFila *x) {
if (filaVazia(f)) return 0;
*x = f->itens[f->inicio];
f->inicio = (f->inicio + 1) % MAX_FILA;
f->qtd--;
return 1;
}
ItemFila filaElemento(const Fila *f, int k) {
return f->itens[(f->inicio + k) % MAX_FILA];
}
