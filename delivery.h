#ifndef DELIVERY_H
#define DELIVERY_H

#define MAX_PEDIDOS 5000

typedef struct {
    int numero;         /* gerado: 1, 2, 3, ... */
    char cliente[41];
    double valor;       /* R$ 10,00 a R$ 999,99 */
    double distanciaKm; /* 0,1 a 15,0 */
    int horario;        /* em minutos */
    int situacao;       /* 1 cozinha, 2 embalado, 3 saiu, 4 cancelado */
} Pedido;

#endif