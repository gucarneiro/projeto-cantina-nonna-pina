#include <stdio.h>
#include "delivery.h"
#include "fila.h"
#include "pilha.h"

static int lerOpcaoMenu(void) {
    int op;
    while (1) {
        printf("Opcao: ");
        if (scanf("%d", &op) == 1) {
            while (getchar() != '\n'); /* limpa o restante da linha */
            return op;
        }
        while (getchar() != '\n'); /* limpa entrada invalida (ex: letras) - VG02 */
        printf("Entrada invalida! Digite um numero inteiro.\n");
    }
}

int main(void) {
    int opcao;
    inicializarSistema();

    do {
        printf("\n==== CANTINA NONNA PINA ==== Cozinha: %d/%d | Prateleira: %d/%d\n",
               getQtdCozinha(), MAX_FILA, getQtdPrateleira(), MAX_PILHA);
        printf("1 Registrar pedido         5 Cancelar pedido embalado\n");
        printf("2 Exibir situacao          6 Desfazer cancelamento\n");
        printf("3 Embalar proximo          7 Relatorio ordenado\n");
        printf("4 Despachar entrega        8 Bateria de analise\n");
        printf("0 Sair\n");

        opcao = lerOpcaoMenu();

        switch (opcao) {
            case 1: registrarPedido();      break;
            case 2: exibirSituacao();       break;
            case 3: embalarProximo();       break;
            case 4: despacharEntrega();     break;
            case 5: cancelarPedido();       break;
            case 6: desfazerCancelamento(); break;
            case 7: relatorioOrdenado();    break;
            case 8: bateriaAnalise();       break;
            case 0: printf("Encerrando o sistema...\n"); break;
            default: printf("Opcao invalida.\n"); break; /* VG01 */
        }
    } while (opcao != 0);

    return 0;
}