#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "delivery.h"
#include "fila.h"
#include "pilha.h"
#include "ordenacao.h"

/* Estruturas estáticas globais ao módulo (Seção 2.1 e 5.2) */
static Pedido pedidos[MAX_PEDIDOS];
static int qtdPedidos = 0;

static Fila cozinha;
static Pilha prateleira;
static Pilha cancelados;

/* Arrays auxiliares estáticos para não estourar a pilha de execução */
static Pedido copiaRelatorio[MAX_PEDIDOS];
static Pedido massaOriginal[MAX_PEDIDOS];
static Pedido massaCopia[MAX_PEDIDOS];

/* --- Funções auxiliares de leitura segura e formatação --- */

static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void lerTextoObrigatorio(const char *prompt, char *dest, int tam) {
    while (1) {
        printf("%s", prompt);
        if (fgets(dest, tam, stdin) != NULL) {
            size_t len = strlen(dest);
            if (len > 0 && dest[len - 1] == '\n') {
                dest[len - 1] = '\0';
            } else {
                limparBuffer();
            }
            /* Verifica se não está vazio ou só com espaços (VG02 / RN01) */
            int temCaractere = 0;
            for (size_t i = 0; dest[i] != '\0'; i++) {
                if (dest[i] != ' ' && dest[i] != '\t') {
                    temCaractere = 1;
                    break;
                }
            }
            if (temCaractere) return;
        }
        printf("Texto obrigatorio! Tente novamente.\n");
    }
}

static double lerDoubleFaixa(const char *prompt, double min, double max) {
    double val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &val) == 1) {
            limparBuffer();
            if (val >= min && val <= max) return val;
            printf("Valor fora da faixa permitida (%.2f a %.2f).\n", min, max);
        } else {
            limparBuffer();
            printf("Entrada invalida! Digite um numero valido.\n");
        }
    }
}

static int lerIntFaixa(const char *prompt, int min, int max) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1) {
            limparBuffer();
            if (val >= min && val <= max) return val;
            printf("Numero fora da faixa permitida (%d a %d).\n", min, max);
        } else {
            limparBuffer();
            printf("Entrada invalida! Digite um numero inteiro.\n");
        }
    }
}

static int lerHorarioMinutos(void) {
    int hh, mm;
    while (1) {
        printf("Horario do pedido (HH:MM): ");
        if (scanf("%d:%d", &hh, &mm) == 2) {
            limparBuffer();
            if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59) {
                return hh * 60 + mm;
            }
            printf("Horario invalido! Use formato 00:00 a 23:59.\n");
        } else {
            limparBuffer();
            printf("Formato invalido! Digite no formato HH:MM (ex: 19:30).\n");
        }
    }
}

static const char *nomeSituacao(int sit) {
    switch (sit) {
        case 1: return "Cozinha";
        case 2: return "Embalado";
        case 3: return "Saiu";
        case 4: return "Cancelado";
        default: return "Desconhecido";
    }
}

/* --- Inicialização e consultas de estado --- */

void inicializarSistema(void) {
    qtdPedidos = 0;
    filaInicializar(&cozinha);
    pilhaInicializar(&prateleira);
    pilhaInicializar(&cancelados);
}

int getQtdCozinha(void) {
    return filaTamanho(&cozinha);
}

int getQtdPrateleira(void) {
    return pilhaTamanho(&prateleira);
}

/* --- Requisitos Funcionais (RF01 a RF08) --- */

/* RF01 Registrar pedido */
void registrarPedido(void) {
    if (qtdPedidos >= MAX_PEDIDOS) {
        printf("Limite maximo de pedidos cadastrados atingido (VG03).\n");
        return;
    }
    if (filaCheia(&cozinha)) {
        printf("Cozinha cheia (30/30)! Pedido recusado (RN02 / VG04).\n");
        return;
    }

    Pedido p;
    p.numero = qtdPedidos + 1;
    lerTextoObrigatorio("Nome do cliente: ", p.cliente, sizeof(p.cliente));
    p.valor = lerDoubleFaixa("Valor do pedido (R$ 10.00 a 999.99): ", 10.00, 999.99);
    p.distanciaKm = lerDoubleFaixa("Distancia em km (0.1 a 15.0): ", 0.1, 15.0);
    p.horario = lerHorarioMinutos();
    p.situacao = 1; /* 1 = cozinha */

    pedidos[qtdPedidos] = p;
    filaEnfileirar(&cozinha, qtdPedidos);
    qtdPedidos++;

    printf("Pedido n. %d (%s) registrado na cozinha!\n", p.numero, p.cliente);
}

/* RF02 Exibir situacao */
void exibirSituacao(void) {
    printf("\n--- FILA DA COZINHA (%d/%d) [Primeiro -> Ultimo] ---\n",
           filaTamanho(&cozinha), MAX_FILA);
    if (filaVazia(&cozinha)) {
        printf("Nenhum pedido na cozinha.\n");
    } else {
        for (int k = 0; k < filaTamanho(&cozinha); k++) {
            int idx = filaElemento(&cozinha, k);
            Pedido *p = &pedidos[idx];
            printf("%d. Pedido n. %d | %-20s | R$ %6.2f | %.1f km | %02d:%02d\n",
                   k + 1, p->numero, p->cliente, p->valor, p->distanciaKm,
                   p->horario / 60, p->horario % 60);
        }
    }

    printf("\n--- PRATELEIRA DE EMBALADOS (%d/%d) [Topo -> Base] ---\n",
           pilhaTamanho(&prateleira), MAX_PILHA);
    if (pilhaVazia(&prateleira)) {
        printf("Nenhum pedido na prateleira.\n");
    } else {
        for (int k = pilhaTamanho(&prateleira) - 1; k >= 0; k--) {
            int idx = pilhaElemento(&prateleira, k);
            Pedido *p = &pedidos[idx];
            printf("[Topo-%d] Pedido n. %d | %-20s | R$ %6.2f | %.1f km | %02d:%02d\n",
                   pilhaTamanho(&prateleira) - 1 - k,
                   p->numero, p->cliente, p->valor, p->distanciaKm,
                   p->horario / 60, p->horario % 60);
        }
    }
}

/* RF03 Embalar proximo */
void embalarProximo(void) {
    if (filaVazia(&cozinha)) {
        printf("Cozinha vazia! Nenhum pedido para embalar (VG04).\n");
        return;
    }
    /* RN03: verifica antes de tirar da cozinha */
    if (pilhaCheia(&prateleira)) {
        printf("Prateleira cheia (10/10)! Despache uma entrega antes de embalar (RN03 / VG05).\n");
        return;
    }

    int idx;
    filaDesenfileirar(&cozinha, &idx);
    pedidos[idx].situacao = 2; /* 2 = embalado */
    pilhaEmpilhar(&prateleira, idx);

    printf("Pedido n. %d (%s) embalado e colocado na prateleira! Prateleira: %d/%d\n",
           pedidos[idx].numero, pedidos[idx].cliente,
           pilhaTamanho(&prateleira), MAX_PILHA);
}

/* RF04 Despachar entrega */
void despacharEntrega(void) {
    if (pilhaVazia(&prateleira)) {
        printf("Prateleira vazia! Nenhum pedido pronto para despacho (VG05).\n");
        return;
    }

    int idx;
    pilhaDesempilhar(&prateleira, &idx);
    pedidos[idx].situacao = 3; /* 3 = saiu */

    printf("Saiu para entrega: pedido n. %d %s (%.1f km). Prateleira: %d/%d\n",
           pedidos[idx].numero, pedidos[idx].cliente, pedidos[idx].distanciaKm,
           pilhaTamanho(&prateleira), MAX_PILHA);
}

/* RF05 Cancelar pedido embalado */
void cancelarPedido(void) {
    if (pilhaVazia(&prateleira)) {
        printf("Prateleira vazia! Nenhum pedido embalado para cancelar (RN04 / VG05).\n");
        return;
    }
    if (pilhaCheia(&cancelados)) {
        printf("Pilha de cancelamentos cheia (10/10)! Cancelamento recusado (RN05 / VG05).\n");
        return;
    }

    int idx;
    pilhaDesempilhar(&prateleira, &idx);
    pedidos[idx].situacao = 4; /* 4 = cancelado */
    pilhaEmpilhar(&cancelados, idx);

    printf("Pedido n. %d (%s) cancelado e movido para a pilha de cancelamentos.\n",
           pedidos[idx].numero, pedidos[idx].cliente);
}

/* RF06 Desfazer cancelamento */
void desfazerCancelamento(void) {
    if (pilhaVazia(&cancelados)) {
        printf("Nenhum cancelamento para desfazer (RN05 / VG05).\n");
        return;
    }
    if (pilhaCheia(&prateleira)) {
        printf("Prateleira cheia (10/10)! Nao e possivel devolver o pedido agora (VG05).\n");
        return;
    }

    int idx;
    pilhaDesempilhar(&cancelados, &idx);
    pedidos[idx].situacao = 2; /* volta para 2 = embalado */
    pilhaEmpilhar(&prateleira, idx);

    printf("Cancelamento do pedido n. %d (%s) desfeito! Voltou para a prateleira.\n",
           pedidos[idx].numero, pedidos[idx].cliente);
}

/* RF07 Relatorio ordenado */
void relatorioOrdenado(void) {
    if (qtdPedidos == 0) {
        printf("Nenhum registro cadastrado para gerar relatorio (VG07).\n");
        return;
    }

    int crit = lerIntFaixa("Criterio (1-Valor [maior] | 2-Distancia [menor] | 3-Horario [antigo]): ", 1, 3);
    int alg  = lerIntFaixa("Algoritmo (1-Bubble | 2-Selection | 3-Quick): ", 1, 3);

    for (int i = 0; i < qtdPedidos; i++) {
        copiaRelatorio[i] = pedidos[i];
    }

    Metricas m = {0, 0, 0.0};
    clock_t ini = clock();
    if (alg == 1)      bubbleSort(copiaRelatorio, qtdPedidos, crit, &m);
    else if (alg == 2) selectionSort(copiaRelatorio, qtdPedidos, crit, &m);
    else               quickSort(copiaRelatorio, qtdPedidos, crit, &m);
    clock_t fim = clock();
    m.tempoMs = 1000.0 * (fim - ini) / CLOCKS_PER_SEC;

    double faturamento = 0.0;
    int ativos = 0;

    printf("\n%-5s | %-20s | %-9s | %-7s | %-5s | %-10s\n",
           "NUM", "CLIENTE", "VALOR", "DIST.", "HORA", "SITUACAO");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < qtdPedidos; i++) {
        Pedido *p = &copiaRelatorio[i];
        printf("%-5d | %-20s | R$ %6.2f | %4.1f km | %02d:%02d | %-10s\n",
               p->numero, p->cliente, p->valor, p->distanciaKm,
               p->horario / 60, p->horario % 60, nomeSituacao(p->situacao));

        if (p->situacao != 4) { /* soma apenas pedidos nao cancelados */
            faturamento += p->valor;
            ativos++;
        }
    }

    printf("----------------------------------------------------------------------\n");
    printf("Faturamento (pedidos nao cancelados: %d/%d): R$ %.2f\n", ativos, qtdPedidos, faturamento);
    printf("Metricas -> Comparacoes: %lld | Trocas: %lld | Tempo: %.3f ms\n",
           m.comparacoes, m.trocas, m.tempoMs);
}

/* Função auxiliar de geração de massa para RF08 (Seção 2.6.2 e 5.5) */
static void gerarMassaPedidos(Pedido v[], int n, int dist) {
    /* dist: 1 = Aleatoria, 2 = Ordenada (maior -> menor), 3 = Invertida (menor -> maior) */
    srand(2026);
    for (int i = 0; i < n; i++) {
        v[i].numero = i + 1;
        sprintf(v[i].cliente, "Cliente %04d", i + 1);
        v[i].distanciaKm = 1.0 + (rand() % 140) / 10.0;
        v[i].horario = 600 + (i % 720);
        v[i].situacao = 3;

        if (dist == 1) {
            /* Aleatoria: 10.00 a 999.99 */
            v[i].valor = 10.0 + (rand() % 99000) / 100.0;
        } else if (dist == 2) {
            /* Ordenada pelo criterio 1 (valor decrescente: maior -> menor) */
            v[i].valor = 999.99 - (i * (989.0 / n));
        } else {
            /* Invertida pelo criterio 1 (valor crescente: menor -> maior) */
            v[i].valor = 10.00 + (i * (989.0 / n));
        }
    }
}

/* RF08 Bateria de analise (27 medicoes) */
void bateriaAnalise(void) {
    int tamanhos[3] = {100, 1000, 5000};
    const char *nomesDist[3] = {"Aleatoria", "Ordenada", "Invertida"};
    const char *nomesAlg[3]  = {"Bubble", "Selection", "Quick"};

    printf("\n%-10s | %-10s | %-6s | %-13s | %-12s | %-10s\n",
           "ALGORITMO", "DISTRIB.", "N", "COMPARACOES", "TROCAS", "TEMPO (ms)");
    printf("-------------------------------------------------------------------------\n");

    for (int t = 0; t < 3; t++) {
        int n = tamanhos[t];
        for (int d = 0; d < 3; d++) {
            gerarMassaPedidos(massaOriginal, n, d + 1);

            for (int a = 0; a < 3; a++) {
                /* Recopia a massa original antes de cada algoritmo */
                for (int i = 0; i < n; i++) {
                    massaCopia[i] = massaOriginal[i];
                }

                Metricas m = {0, 0, 0.0};
                clock_t ini = clock();
                if (a == 0)      bubbleSort(massaCopia, n, 1, &m);
                else if (a == 1) selectionSort(massaCopia, n, 1, &m);
                else             quickSort(massaCopia, n, 1, &m);
                clock_t fim = clock();
                m.tempoMs = 1000.0 * (fim - ini) / CLOCKS_PER_SEC;

                printf("%-10s | %-10s | %-6d | %-13lld | %-12lld | %-10.3f\n",
                       nomesAlg[a], nomesDist[d], n, m.comparacoes, m.trocas, m.tempoMs);
            }
        }
    }
    printf("-------------------------------------------------------------------------\n");
}