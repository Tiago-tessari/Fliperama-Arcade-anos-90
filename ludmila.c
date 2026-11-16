// ============================================================================
// ludmila.c - Modulo 2: MAQUINAS / JOGOS DO FLIPERAMA
// Autor: Ludmila Ladislau
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Maquina catalogoMaquinas[MAX];
int totalMaquinas = 0;

// ---------------------------- ARQUIVO CSV ----------------------------------

// Formato de cada linha: id;nomeJogo;fichasPorPartida;precoFicha;ativa
void carregarMaquinasCSV(void) {
    FILE *arq = fopen("maquinas.csv", "r");
    char cabecalho[200];
    totalMaquinas = 0;
    if (arq == NULL) return;
    if (fgets(cabecalho, sizeof(cabecalho), arq) == NULL) { fclose(arq); return; }
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

void salvarMaquinasCSV(void) {
    int i;
    FILE *arq = fopen("maquinas.csv", "w");   // "w" zera o arquivo e recria limpo
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

// ------------------------------ BUSCAS -------------------------------------

int buscarMaquinaPorId(int id) {
    int i;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].idMaquina == id && catalogoMaquinas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

static int proximoIdMaquina(void) {
    int i, maior = 0;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].idMaquina > maior) maior = catalogoMaquinas[i].idMaquina;
    }
    return maior + 1;
}

static void imprimirMaquina(int pos) {
    printf("ID %d | %s | %d ficha(s)/partida | R$ %.2f/ficha\n",
           catalogoMaquinas[pos].idMaquina, catalogoMaquinas[pos].nomeJogo,
           catalogoMaquinas[pos].fichasPorPartida, catalogoMaquinas[pos].precoFicha);
}

// ------------------------------- CRUD --------------------------------------

void cadastrarMaquina(void) {
    int pos, fichas;
    float preco;
    if (totalMaquinas >= MAX) {
        printf("\n[ERRO] Limite de maquinas atingido!\n");
        return;
    }
    pos = totalMaquinas;
    catalogoMaquinas[pos].idMaquina = proximoIdMaquina();
    lerTexto("\nNome do jogo: ", catalogoMaquinas[pos].nomeJogo, 40);
    do {
        fichas = lerInt("Fichas por partida: ");
        if (fichas <= 0) printf("[ERRO] Precisa ser maior que zero.\n");
    } while (fichas <= 0);
    do {
        preco = lerFloat("Preco de cada ficha (R$): ");
        if (preco <= 0) printf("[ERRO] Precisa ser maior que zero.\n");
    } while (preco <= 0);
    catalogoMaquinas[pos].fichasPorPartida = fichas;
    catalogoMaquinas[pos].precoFicha = preco;
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
        lerTexto("Parte do nome do jogo: ", termo, 40);
        for (i = 0; i < totalMaquinas; i++) {
            if (catalogoMaquinas[i].ativa == 1 && contemTexto(catalogoMaquinas[i].nomeJogo, termo)) {
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
    int id, pos, fichas;
    float preco;
    id = lerInt("\nID da maquina a editar: ");
    pos = buscarMaquinaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Maquina nao encontrada.\n");
        return;
    }
    do {
        fichas = lerInt("Novas fichas por partida: ");
        if (fichas <= 0) printf("[ERRO] Precisa ser maior que zero.\n");
    } while (fichas <= 0);
    do {
        preco = lerFloat("Novo preco da ficha (R$): ");
        if (preco <= 0) printf("[ERRO] Precisa ser maior que zero.\n");
    } while (preco <= 0);
    catalogoMaquinas[pos].fichasPorPartida = fichas;
    catalogoMaquinas[pos].precoFicha = preco;
    salvarMaquinasCSV();
    printf("\nMaquina atualizada!\n");
}

// Remocao logica (ativa = 0): preserva o historico das partidas antigas.
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

// ----------------------------- RELATORIOS ----------------------------------

// Cruza dados: conta quantas partidas validas cada maquina teve
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

// Ranking de faturamento por maquina com BUBBLE SORT (maior receita primeiro).
void rankingMaquinasPorReceita(void) {
    int indices[MAX], n = 0, i, j, aux;
    float receita[MAX] = {0};     // receita[i] = total arrecadado pela maquina na posicao i
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].ativa == 1) {
            indices[n++] = i;
            for (j = 0; j < totalPartidas; j++) {
                if (historicoPartidas[j].ativa == 1 &&
                    historicoPartidas[j].idMaquina == catalogoMaquinas[i].idMaquina) {
                    receita[i] += historicoPartidas[j].valorGasto;
                }
            }
        }
    }
    if (n == 0) {
        printf("\nNenhuma maquina cadastrada.\n");
        return;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (receita[indices[j]] < receita[indices[j + 1]]) {
                aux = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = aux;
            }
        }
    }
    printf("\n=== RANKING: MAQUINAS QUE MAIS FATURAM ===\n");
    for (i = 0; i < n; i++) {
        printf("%2do | %-24s | R$ %.2f\n", i + 1,
               catalogoMaquinas[indices[i]].nomeJogo, receita[indices[i]]);
    }
}

// ------------------------------- MENU --------------------------------------

void moduloMaquinas(void) {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 2: MAQUINAS / JOGOS (Ludmila)] ---\n");
        printf("1. Cadastrar Nova Maquina\n");
        printf("2. Listar / Buscar Maquinas\n");
        printf("3. Editar Fichas por Partida / Preco\n");
        printf("4. Remover Maquina\n");
        printf("5. [RELATORIO] Jogo Mais Jogado\n");
        printf("6. [RELATORIO] Ranking de Faturamento (Bubble Sort)\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInt("Escolha uma acao: ");
        switch (opcao) {
            case 1: cadastrarMaquina(); break;
            case 2: listarOuBuscarMaquina(); break;
            case 3: editarMaquina(); break;
            case 4: removerMaquina(); break;
            case 5: relatorioJogoMaisJogado(); break;
            case 6: rankingMaquinasPorReceita(); break;
            case 0: break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Maquinas!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}
