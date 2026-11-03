// ============================================================================
// paulo.c - Modulo 3: PARTIDAS, HIGH SCORES E RELATORIOS
// Autor: Paulo Marques (RGM: 47828439)
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Partida historicoPartidas[MAX];
int totalPartidas = 0;

// Autor: Paulo
int buscarPartidaPorId(int id) {
    int i;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idPartida == id && historicoPartidas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

// Autor: Paulo
// Cruza Jogador + Maquina: desconta as fichas e grava a pontuacao
void registrarPartida() {
    int idJ, idM, posJ, posM, custo;
    if (totalPartidas >= MAX) {
        printf("\n[ERRO] Limite de partidas atingido!\n");
        return;
    }
    printf("\nID do jogador: ");
    scanf("%d", &idJ);
    posJ = buscarJogadorPorId(idJ);
    if (posJ == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    printf("ID da maquina: ");
    scanf("%d", &idM);
    posM = buscarMaquinaPorId(idM);
    if (posM == -1) {
        printf("\n[ERRO] Maquina nao encontrada.\n");
        return;
    }
    custo = catalogoMaquinas[posM].fichasPorPartida;
    if (listaJogadores[posJ].fichasAcumuladas < custo) {
        printf("\n[ERRO] Fichas insuficientes! Precisa de %d, tem %d.\n",
               custo, listaJogadores[posJ].fichasAcumuladas);
        return;
    }
    historicoPartidas[totalPartidas].idPartida = totalPartidas + 1;
    historicoPartidas[totalPartidas].idJogador = idJ;
    historicoPartidas[totalPartidas].idMaquina = idM;
    printf("Pontuacao feita na partida: ");
    scanf("%d", &historicoPartidas[totalPartidas].pontuacao);
    historicoPartidas[totalPartidas].fichasGastas = custo;
    historicoPartidas[totalPartidas].ativa = 1;
    listaJogadores[posJ].fichasAcumuladas -= custo;
    printf("\nPartida registrada! ID = %d\n", historicoPartidas[totalPartidas].idPartida);
    totalPartidas++;
}

// Autor: Paulo
void listarOuBuscarPartidas() {
    int modo, id, i, achou = 0;
    printf("\n1. Listar todas | 2. Partidas de um jogador: ");
    scanf("%d", &modo);
    if (modo == 1) {
        for (i = 0; i < totalPartidas; i++) {
            if (historicoPartidas[i].ativa == 1) {
                printf("Partida %d | Jogador %d | Maquina %d | %d pts | %d ficha(s)\n",
                       historicoPartidas[i].idPartida, historicoPartidas[i].idJogador,
                       historicoPartidas[i].idMaquina, historicoPartidas[i].pontuacao,
                       historicoPartidas[i].fichasGastas);
                achou = 1;
            }
        }
    } else if (modo == 2) {
        printf("ID do jogador: ");
        scanf("%d", &id);
        for (i = 0; i < totalPartidas; i++) {
            if (historicoPartidas[i].ativa == 1 && historicoPartidas[i].idJogador == id) {
                printf("Partida %d | Maquina %d | %d pts | %d ficha(s)\n",
                       historicoPartidas[i].idPartida, historicoPartidas[i].idMaquina,
                       historicoPartidas[i].pontuacao, historicoPartidas[i].fichasGastas);
                achou = 1;
            }
        }
    } else {
        printf("\n[ERRO] Opcao invalida!\n");
        return;
    }
    if (achou == 0) printf("\nNenhuma partida encontrada.\n");
}

// Autor: Paulo
void editarPontuacao() {
    int id, pos;
    printf("\nID da partida a corrigir: ");
    scanf("%d", &id);
    pos = buscarPartidaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Partida nao encontrada.\n");
        return;
    }
    printf("Nova pontuacao: ");
    scanf("%d", &historicoPartidas[pos].pontuacao);
    printf("\nPontuacao corrigida!\n");
}

// Autor: Paulo
// Estorno: devolve as fichas ao jogador e invalida a partida
void estornarPartida() {
    int id, pos, posJ;
    printf("\nID da partida a estornar: ");
    scanf("%d", &id);
    pos = buscarPartidaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Partida nao encontrada.\n");
        return;
    }
    posJ = buscarJogadorPorId(historicoPartidas[pos].idJogador);
    if (posJ != -1) {
        listaJogadores[posJ].fichasAcumuladas += historicoPartidas[pos].fichasGastas;
    }
    historicoPartidas[pos].ativa = 0;
    printf("\nPartida estornada e fichas devolvidas.\n");
}

// Autor: Paulo
void relatorioHighScore() {
    int idM, i, melhor = -1, posJ;
    printf("\nID da maquina: ");
    scanf("%d", &idM);
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].ativa == 1 && historicoPartidas[i].idMaquina == idM) {
            if (melhor == -1 || historicoPartidas[i].pontuacao > historicoPartidas[melhor].pontuacao) {
                melhor = i;
            }
        }
    }
    if (melhor == -1) {
        printf("\nNenhuma partida registrada nessa maquina.\n");
    } else {
        posJ = buscarJogadorPorId(historicoPartidas[melhor].idJogador);
        if (posJ != -1) {
            printf("\nHIGH SCORE: %s - %d pontos\n",
                   listaJogadores[posJ].iniciais, historicoPartidas[melhor].pontuacao);
        } else {
            printf("\nHIGH SCORE: %d pontos (jogador removido)\n", historicoPartidas[melhor].pontuacao);
        }
    }
}

// Autor: Paulo
void relatorioJogadorMaisPartidas() {
    int i, j, contagem, melhor = -1, maiorContagem = 0;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].ativo == 1) {
            contagem = 0;
            for (j = 0; j < totalPartidas; j++) {
                if (historicoPartidas[j].ativa == 1 &&
                    historicoPartidas[j].idJogador == listaJogadores[i].idJogador) {
                    contagem++;
                }
            }
            if (contagem > maiorContagem) {
                maiorContagem = contagem;
                melhor = i;
            }
        }
    }
    if (melhor == -1) {
        printf("\nAinda nao ha partidas registradas.\n");
    } else {
        printf("\nJogador mais ativo: %s (%d partidas)\n", listaJogadores[melhor].nome, maiorContagem);
    }
}

// Autor: Paulo
void moduloPartidas() {
    int opcao;
    do {
        printf("\n--- [MODULO 3: PARTIDAS E HIGH SCORES (Paulo)] ---\n");
        printf("1. Registrar Nova Partida (Cruza Jogador + Maquina)\n");
        printf("2. Listar / Buscar Partidas\n");
        printf("3. Corrigir Pontuacao de Partida\n");
        printf("4. Estornar Partida\n");
        printf("5. [RELATORIO] High Score de uma Maquina\n");
        printf("6. [RELATORIO] Jogador com Mais Partidas\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: registrarPartida(); break;
            case 2: listarOuBuscarPartidas(); break;
            case 3: editarPontuacao(); break;
            case 4: estornarPartida(); break;
            case 5: relatorioHighScore(); break;
            case 6: relatorioJogadorMaisPartidas(); break;
            case 0: printf("\nRetornando ao menu principal...\n"); break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Partidas!\n");
        }
    } while (opcao != 0);
}
