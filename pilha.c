#include "pilha.h" 
void pilhaInicializar(Pilha *p) { p->topo = -1; }
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