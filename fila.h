#ifndef FILA_H
#define FILA_H
#define MAX_FILA 30
typedef int ItemFila; /* em geral: índice no array do tema */
typedef struct {
ItemFila itens[MAX_FILA];
int inicio, fim, qtd;
} Fila;
void filaInicializar(Fila *f);
int filaVazia(const Fila *f);
int filaCheia(const Fila *f);
int filaEnfileirar(Fila *f, ItemFila x); /* 1 = ok, 0 = cheia */
int filaDesenfileirar(Fila *f, ItemFila *x); /* 1 = ok, 0 = vazia */
int filaTamanho(const Fila *f);
ItemFila filaElemento(const Fila *f, int k); /* k-ésimo, para exibir */
#endif
