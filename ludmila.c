// ============================================================================
// ludmila.c - Modulo 2: MAQUINAS / JOGOS DO FLIPERAMA
// Autor: Ludmila Ladislau
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Maquina catalogoMaquinas[MAX];
int totalMaquinas = 0;

// ============================================================================
// ARQUIVO CSV DAS MAQUINAS
// Formato de cada linha:  id;nomeJogo;fichasPorPartida;precoFicha;ativa
// ============================================================================

// Abre para LEITURA ("r") e copia cada linha do arquivo para o vetor.
void carregarMaquinasCSV(void) {
    FILE *arq = fopen("maquinas.csv", "r");
    char cabecalho[100];
    totalMaquinas = 0;
    if (arq == NULL) return;
    fgets(cabecalho, sizeof(cabecalho), arq);    // pula a linha de titulos
    // %d = inteiro | %39[^;] = texto ate o ';' | %f = numero decimal
    // O fscanf devolve quantos campos leu: 5 = linha completa.
    while (totalMaquinas < MAX &&
           fscanf(arq, "%d;%39[^;];%d;%f;%d\n",
                  &catalogoMaquinas[totalMaquinas].idMaquina,
                  catalogoMaquinas[totalMaquinas].nomeJogo,
                  &catalogoMaquinas[totalMaquinas].fichasPorPartida,
                  &catalogoMaquinas[totalMaquinas].precoFicha,
                  &catalogoMaquinas[totalMaquinas].ativa) == 5) {
        totalMaquinas++;
    }
    fclose(arq);
}

// Abre para ESCRITA ("w"): zera o arquivo e regrava o vetor inteiro.
void salvarMaquinasCSV(void) {
    int i;
    FILE *arq = fopen("maquinas.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar maquinas.csv\n");
        return;
    }
    fprintf(arq, "id;nomeJogo;fichasPorPartida;precoFicha;ativa\n");
    for (i = 0; i < totalMaquinas; i++) {
        fprintf(arq, "%d;%s;%d;%.2f;%d\n",
                catalogoMaquinas[i].idMaquina, catalogoMaquinas[i].nomeJogo,
                catalogoMaquinas[i].fichasPorPartida, catalogoMaquinas[i].precoFicha,
                catalogoMaquinas[i].ativa);
    }
    fclose(arq);
}

// ============================================================================
// MODULO DE MAQUINAS
// ============================================================================

// Retorna a posicao da maquina no vetor (ou -1 se nao existir / foi removida)
int buscarMaquinaPorId(int id) {
    int i;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].idMaquina == id && catalogoMaquinas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

static void imprimirMaquina(int pos) {
    printf("ID %d | %s | %d ficha(s)/partida | R$ %.2f/ficha\n",
           catalogoMaquinas[pos].idMaquina, catalogoMaquinas[pos].nomeJogo,
           catalogoMaquinas[pos].fichasPorPartida, catalogoMaquinas[pos].precoFicha);
}

void cadastrarMaquina(void) {
    int pos;
    if (totalMaquinas >= MAX) {
        printf("\n[ERRO] Limite de maquinas atingido!\n");
        return;
    }
    pos = totalMaquinas;
    catalogoMaquinas[pos].idMaquina = pos + 1;
    printf("\nNome do jogo (sem espacos, use _): ");
    scanf("%39s", catalogoMaquinas[pos].nomeJogo);
    catalogoMaquinas[pos].fichasPorPartida = lerInt("Fichas por partida: ");
    catalogoMaquinas[pos].precoFicha = lerFloat("Preco de cada ficha (R$): ");
    catalogoMaquinas[pos].ativa = 1;
    totalMaquinas++;
    salvarMaquinasCSV();
    printf("\nMaquina cadastrada! ID = %d\n", catalogoMaquinas[pos].idMaquina);
}

// Listar todas, buscar por ID ou buscar por parte do nome (strstr)
void listarOuBuscarMaquina(void) {
    int modo, id, i, pos, achou = 0;
    char termo[40];
    printf("\n1. Listar todas | 2. Buscar por ID | 3. Buscar por nome\n");
    modo = lerInt("Escolha: ");
    if (modo == 1) {
        for (i = 0; i < totalMaquinas; i++) {
            if (catalogoMaquinas[i].ativa == 1) {
                imprimirMaquina(i);
                achou = 1;
            }
        }
        if (achou == 0) printf("\nNenhuma maquina cadastrada.\n");
    } else if (modo == 2) {
        id = lerInt("ID da maquina: ");
        pos = buscarMaquinaPorId(id);
        if (pos == -1) printf("\n[ERRO] Maquina nao encontrada.\n");
        else imprimirMaquina(pos);
    } else if (modo == 3) {
        printf("Parte do nome do jogo (respeite maiusculas): ");
        scanf("%39s", termo);
        for (i = 0; i < totalMaquinas; i++) {
            // strstr procura "termo" dentro do nome; devolve NULL se nao achar
            if (catalogoMaquinas[i].ativa == 1 && strstr(catalogoMaquinas[i].nomeJogo, termo) != NULL) {
                imprimirMaquina(i);
                achou = 1;
            }
        }
        if (achou == 0) printf("\nNenhum jogo com esse nome.\n");
    } else {
        printf("\n[ERRO] Opcao invalida!\n");
    }
}

void editarMaquina(void) {
    int id, pos;
    id = lerInt("\nID da maquina a editar: ");
    pos = buscarMaquinaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Maquina nao encontrada.\n");
        return;
    }
    catalogoMaquinas[pos].fichasPorPartida = lerInt("Novas fichas por partida: ");
    catalogoMaquinas[pos].precoFicha = lerFloat("Novo preco da ficha (R$): ");
    salvarMaquinasCSV();
    printf("\nMaquina atualizada!\n");
}

// Remocao logica: ativa = 0 (o registro fica no CSV para nao quebrar as partidas antigas)
void removerMaquina(void) {
    int id, pos;
    id = lerInt("\nID da maquina a remover: ");
    pos = buscarMaquinaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Maquina nao encontrada.\n");
        return;
    }
    catalogoMaquinas[pos].ativa = 0;
    salvarMaquinasCSV();
    printf("\nMaquina removida (fora de operacao).\n");
}

// RELATORIO que cruza dados: conta quantas partidas validas cada maquina teve
void relatorioJogoMaisJogado(void) {
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

void moduloMaquinas(void) {
    int opcao;
    do {
        printf("\n--- [MODULO 2: MAQUINAS / JOGOS (Ludmila)] ---\n");
        printf("1. Cadastrar Nova Maquina\n");
        printf("2. Listar / Buscar Maquinas\n");
        printf("3. Editar Fichas por Partida / Preco\n");
        printf("4. Remover Maquina\n");
        printf("5. [RELATORIO] Jogo Mais Jogado\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInt("Escolha uma acao: ");
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
