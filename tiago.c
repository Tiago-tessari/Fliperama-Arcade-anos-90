// ============================================================================
// tiago.c - Modulo 1: JOGADORES (fichas) + funcao main (menu principal)
// Autor: Tiago Tessari (RGM: 39969321)
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Jogador listaJogadores[MAX];
int totalJogadores = 0;

// Autor: Tiago Tessari
void exibirCabecalho() {
    printf("\n======================================================\n");
    printf("   ARCADE PALACE - FICHAS E HIGH SCORES (ANOS 90)     \n");
    printf("======================================================\n");
}

// Autor: Tiago Tessari
int buscarJogadorPorId(int id) {
    int i;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador == id && listaJogadores[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

// Autor: Tiago Tessari
void cadastrarJogador() {
    if (totalJogadores >= MAX) {
        printf("\n[ERRO] Limite de jogadores atingido!\n");
        return;
    }
    listaJogadores[totalJogadores].idJogador = totalJogadores + 1;
    printf("\nNome do jogador (sem espacos): ");
    scanf("%49s", listaJogadores[totalJogadores].nome);
    printf("Iniciais para o placar (3 letras): ");
    scanf("%3s", listaJogadores[totalJogadores].iniciais);
    printf("Fichas iniciais: ");
    scanf("%d", &listaJogadores[totalJogadores].fichasAcumuladas);
    listaJogadores[totalJogadores].gastoTotal = 0;
    listaJogadores[totalJogadores].ativo = 1;
    printf("\nJogador cadastrado! ID = %d\n", listaJogadores[totalJogadores].idJogador);
    totalJogadores++;
}

// Autor: Tiago Tessari
void listarOuBuscarJogador() {
    int modo, id, i, pos, achou = 0;
    printf("\n1. Listar todos | 2. Buscar por ID: ");
    scanf("%d", &modo);
    if (modo == 1) {
        for (i = 0; i < totalJogadores; i++) {
            if (listaJogadores[i].ativo == 1) {
                printf("ID %d | %s (%s) | Fichas: %d | Gasto: R$ %.2f\n",
                       listaJogadores[i].idJogador, listaJogadores[i].nome,
                       listaJogadores[i].iniciais, listaJogadores[i].fichasAcumuladas,
                       listaJogadores[i].gastoTotal);
                achou = 1;
            }
        }
        if (achou == 0) printf("\nNenhum jogador cadastrado.\n");
    } else if (modo == 2) {
        printf("ID do jogador: ");
        scanf("%d", &id);
        pos = buscarJogadorPorId(id);
        if (pos == -1) {
            printf("\n[ERRO] Jogador nao encontrado.\n");
        } else {
            printf("ID %d | %s (%s) | Fichas: %d | Gasto: R$ %.2f\n",
                   listaJogadores[pos].idJogador, listaJogadores[pos].nome,
                   listaJogadores[pos].iniciais, listaJogadores[pos].fichasAcumuladas,
                   listaJogadores[pos].gastoTotal);
        }
    } else {
        printf("\n[ERRO] Opcao invalida!\n");
    }
}

// Autor: Tiago Tessari
void editarJogador() {
    int id, pos, fichasExtras;
    printf("\nID do jogador a editar: ");
    scanf("%d", &id);
    pos = buscarJogadorPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    printf("Novo nome (sem espacos): ");
    scanf("%49s", listaJogadores[pos].nome);
    printf("Fichas a adicionar (compra no caixa): ");
    scanf("%d", &fichasExtras);
    listaJogadores[pos].fichasAcumuladas += fichasExtras;
    printf("\nJogador atualizado! Fichas agora: %d\n", listaJogadores[pos].fichasAcumuladas);
}

// Autor: Tiago Tessari
void removerJogador() {
    int id, pos;
    printf("\nID do jogador a remover: ");
    scanf("%d", &id);
    pos = buscarJogadorPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    listaJogadores[pos].ativo = 0;
    printf("\nJogador removido.\n");
}

// Autor: Tiago Tessari
void relatorioJogadorMaisFichas() {
    int i, melhor = -1;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].ativo == 1) {
            if (melhor == -1 || listaJogadores[i].fichasAcumuladas > listaJogadores[melhor].fichasAcumuladas) {
                melhor = i;
            }
        }
    }
    if (melhor == -1) {
        printf("\nNenhum jogador cadastrado.\n");
    } else {
        printf("\nJogador com mais fichas: %s (%d fichas)\n",
               listaJogadores[melhor].nome, listaJogadores[melhor].fichasAcumuladas);
    }
}

// Autor: Tiago Tessari
void moduloJogadores() {
    int opcao;
    do {
        printf("\n--- [MODULO 1: JOGADORES (Tiago)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar / Buscar Jogadores\n");
        printf("3. Editar Jogador / Comprar Fichas\n");
        printf("4. Remover Jogador\n");
        printf("5. [RELATORIO] Jogador com Mais Fichas\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: cadastrarJogador(); break;
            case 2: listarOuBuscarJogador(); break;
            case 3: editarJogador(); break;
            case 4: removerJogador(); break;
            case 5: relatorioJogadorMaisFichas(); break;
            case 0: printf("\nRetornando ao menu principal...\n"); break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Jogadores!\n");
        }
    } while (opcao != 0);
}

// ============================================================================
// MAIN - Despachante de menus
// Autor: Tiago Tessari
// ============================================================================
int main() {
    int opcaoPrincipal;
    do {
        exibirCabecalho();
        printf("SELECIONE A PERSPECTIVA DO SISTEMA:\n");
        printf("1. Modulo de Jogadores / Fichas (Tiago)\n");
        printf("2. Modulo de Maquinas / Jogos (Ludmila)\n");
        printf("3. Modulo de Partidas e High Scores (Paulo)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcaoPrincipal);
        switch (opcaoPrincipal) {
            case 1: moduloJogadores(); break;
            case 2: moduloMaquinas(); break;
            case 3: moduloPartidas(); break;
            case 0: printf("\nEncerrando o Arcade Palace... GAME OVER!\n"); break;
            default: printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 3.\n");
        }
    } while (opcaoPrincipal != 0);
    return 0;
}
