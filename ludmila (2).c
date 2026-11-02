// ============================================================================
// ludmila.c - Modulo 2: MAQUINAS / JOGOS DO FLIPERAMA
// Autor: Ludmila (Sobrenome e RGM: ________)
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Maquina catalogoMaquinas[MAX];
int totalMaquinas = 0;

// Autor: Ludmila
int buscarMaquinaPorId(int id) {
    int i;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].idMaquina == id && catalogoMaquinas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

// Autor: Ludmila
void cadastrarMaquina() {
    if (totalMaquinas >= MAX) {
        printf("\n[ERRO] Limite de maquinas atingido!\n");
        return;
    }
    catalogoMaquinas[totalMaquinas].idMaquina = totalMaquinas + 1;
    printf("\nNome do jogo (sem espacos): ");
    scanf("%39s", catalogoMaquinas[totalMaquinas].nomeJogo);
    printf("Fichas por partida: ");
    scanf("%d", &catalogoMaquinas[totalMaquinas].fichasPorPartida);
    printf("Preco de cada ficha (R$): ");
    scanf("%f", &catalogoMaquinas[totalMaquinas].precoFicha);
    catalogoMaquinas[totalMaquinas].ativa = 1;
    printf("\nMaquina cadastrada! ID = %d\n", catalogoMaquinas[totalMaquinas].idMaquina);
    totalMaquinas++;
}

// Autor: Ludmila
void listarOuBuscarMaquina() {
    int modo, id, i, pos, achou = 0;
    printf("\n1. Listar todas | 2. Buscar por ID: ");
    scanf("%d", &modo);
    if (modo == 1) {
        for (i = 0; i < totalMaquinas; i++) {
            if (catalogoMaquinas[i].ativa == 1) {
                printf("ID %d | %s | %d ficha(s)/partida | R$ %.2f/ficha\n",
                       catalogoMaquinas[i].idMaquina, catalogoMaquinas[i].nomeJogo,
                       catalogoMaquinas[i].fichasPorPartida, catalogoMaquinas[i].precoFicha);
                achou = 1;
            }
        }
        if (achou == 0) printf("\nNenhuma maquina cadastrada.\n");
    } else if (modo == 2) {
        printf("ID da maquina: ");
        scanf("%d", &id);
        pos = buscarMaquinaPorId(id);
        if (pos == -1) {
            printf("\n[ERRO] Maquina nao encontrada.\n");
        } else {
            printf("ID %d | %s | %d ficha(s)/partida | R$ %.2f/ficha\n",
                   catalogoMaquinas[pos].idMaquina, catalogoMaquinas[pos].nomeJogo,
                   catalogoMaquinas[pos].fichasPorPartida, catalogoMaquinas[pos].precoFicha);
        }
    } else {
        printf("\n[ERRO] Opcao invalida!\n");
    }
}

// Autor: Ludmila
void editarMaquina() {
    int id, pos;
    printf("\nID da maquina a editar: ");
    scanf("%d", &id);
    pos = buscarMaquinaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Maquina nao encontrada.\n");
        return;
    }
    printf("Novas fichas por partida: ");
    scanf("%d", &catalogoMaquinas[pos].fichasPorPartida);
    printf("Novo preco da ficha (R$): ");
    scanf("%f", &catalogoMaquinas[pos].precoFicha);
    printf("\nMaquina atualizada!\n");
}

// Autor: Ludmila
void removerMaquina() {
    int id, pos;
    printf("\nID da maquina a remover: ");
    scanf("%d", &id);
    pos = buscarMaquinaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Maquina nao encontrada.\n");
        return;
    }
    catalogoMaquinas[pos].ativa = 0;
    printf("\nMaquina removida (fora de operacao).\n");
}

// Autor: Ludmila
// Cruza dados: conta quantas partidas validas cada maquina teve
void relatorioJogoMaisJogado() {
    int i, j, contagem, melhor = -1, maiorContagem = 0;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].ativa == 1) {
            contagem = 0;
            for (j = 0; j < totalPartidas; j++) {
                if (historicoPartidas[j].ativa == 1 &&
                    historicoPartidas[j].idMaquina == catalogoMaquinas[i].idMaquina) {
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
        printf("\nJogo mais jogado: %s (%d partidas)\n",
               catalogoMaquinas[melhor].nomeJogo, maiorContagem);
    }
}

// Autor: Ludmila
void moduloMaquinas() {
    int opcao;
    do {
        printf("\n--- [MODULO 2: MAQUINAS / JOGOS (Ludmila)] ---\n");
        printf("1. Cadastrar Nova Maquina\n");
        printf("2. Listar / Buscar Maquinas\n");
        printf("3. Editar Fichas por Partida / Preco\n");
        printf("4. Remover Maquina\n");
        printf("5. [RELATORIO] Jogo Mais Jogado\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: cadastrarMaquina(); break;
            case 2: listarOuBuscarMaquina(); break;
            case 3: editarMaquina(); break;
            case 4: removerMaquina(); break;
            case 5: relatorioJogoMaisJogado(); break;
            case 0: printf("\nRetornando ao menu principal...\n"); break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Maquinas!\n");
        }
    } while (opcao != 0);
}
