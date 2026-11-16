// ============================================================================
// paulo.c - Modulo 3: PARTIDAS, HIGH SCORES E RELATORIOS
// Autor: Paulo (Sobrenome e RGM: ________)
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Partida historicoPartidas[MAX];
int totalPartidas = 0;

// ---------------------------- ARQUIVO CSV ----------------------------------

// Formato: id;idJogador;idMaquina;pontuacao;fichas;valor;data;ativa
void carregarPartidasCSV(void) {
    FILE *arq = fopen("partidas.csv", "r");
    char cabecalho[200];
    totalPartidas = 0;
    if (arq == NULL) return;
    if (fgets(cabecalho, sizeof(cabecalho), arq) == NULL) { fclose(arq); return; }
    while (totalPartidas < MAX &&
           fscanf(arq, "%d;%d;%d;%d;%d;%f;%10[^;];%d\n",
                  &historicoPartidas[totalPartidas].idPartida,
                  &historicoPartidas[totalPartidas].idJogador,
                  &historicoPartidas[totalPartidas].idMaquina,
                  &historicoPartidas[totalPartidas].pontuacao,
                  &historicoPartidas[totalPartidas].fichasGastas,
                  &historicoPartidas[totalPartidas].valorGasto,
                  historicoPartidas[totalPartidas].data,
                  &historicoPartidas[totalPartidas].ativa) == 8) {
        totalPartidas++;
    }
    fclose(arq);
}

void salvarPartidasCSV(void) {
    int i;
    FILE *arq = fopen("partidas.csv", "w");   // "w" zera o arquivo e recria limpo
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar partidas.csv\n");
        return;
    }
    fprintf(arq, "id;idJogador;idMaquina;pontuacao;fichas;valor;data;ativa\n");
    for (i = 0; i < totalPartidas; i++) {
        fprintf(arq, "%d;%d;%d;%d;%d;%.2f;%s;%d\n",
                historicoPartidas[i].idPartida, historicoPartidas[i].idJogador,
                historicoPartidas[i].idMaquina, historicoPartidas[i].pontuacao,
                historicoPartidas[i].fichasGastas, historicoPartidas[i].valorGasto,
                historicoPartidas[i].data, historicoPartidas[i].ativa);
    }
    fclose(arq);
}

// ------------------------------ BUSCAS -------------------------------------

static int buscarPartidaPorId(int id) {
    int i;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idPartida == id && historicoPartidas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

static int proximoIdPartida(void) {
    int i, maior = 0;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idPartida > maior) maior = historicoPartidas[i].idPartida;
    }
    return maior + 1;
}

// Formato simples: 10 caracteres com '/' nas posicoes 2 e 5 (dd/mm/aaaa)
static int dataValida(const char *d) {
    return strlen(d) == 10 && d[2] == '/' && d[5] == '/';
}

static void imprimirPartida(int i) {
    int posJ = buscarJogadorPorId(historicoPartidas[i].idJogador);
    int posM = buscarMaquinaPorId(historicoPartidas[i].idMaquina);
    printf("Partida %d | %s | %s | %d pts | %d ficha(s) | R$ %.2f | %s\n",
           historicoPartidas[i].idPartida,
           posJ != -1 ? listaJogadores[posJ].iniciais : "???",
           posM != -1 ? catalogoMaquinas[posM].nomeJogo : "(removida)",
           historicoPartidas[i].pontuacao, historicoPartidas[i].fichasGastas,
           historicoPartidas[i].valorGasto, historicoPartidas[i].data);
}

// ------------------------------- CRUD --------------------------------------

// CRUZA Jogador + Maquina: valida os dois IDs, calcula o valor, da BAIXA nas
// fichas do jogador e grava a pontuacao.
void registrarPartida(void) {
    int idJ, idM, posJ, posM, custo, pos;
    float valor;
    char data[11];
    if (totalPartidas >= MAX) {
        printf("\n[ERRO] Limite de partidas atingido!\n");
        return;
    }
    idJ = lerInt("\nID do jogador: ");
    posJ = buscarJogadorPorId(idJ);
    if (posJ == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    idM = lerInt("ID da maquina: ");
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
    pos = totalPartidas;
    historicoPartidas[pos].idPartida = proximoIdPartida();
    historicoPartidas[pos].idJogador = idJ;
    historicoPartidas[pos].idMaquina = idM;
    historicoPartidas[pos].pontuacao = lerInt("Pontuacao feita na partida: ");
    do {
        lerTexto("Data da partida (dd/mm/aaaa): ", data, 11);
        if (!dataValida(data)) printf("[ERRO] Use o formato dd/mm/aaaa.\n");
    } while (!dataValida(data));
    strcpy(historicoPartidas[pos].data, data);
    valor = custo * catalogoMaquinas[posM].precoFicha;
    historicoPartidas[pos].fichasGastas = custo;
    historicoPartidas[pos].valorGasto = valor;
    historicoPartidas[pos].ativa = 1;
    listaJogadores[posJ].fichasAcumuladas -= custo;   // baixa nas fichas
    listaJogadores[posJ].gastoTotal += valor;
    totalPartidas++;
    salvarPartidasCSV();
    salvarJogadoresCSV();       // as fichas do jogador mudaram, entao grava tambem
    printf("\nPartida registrada! ID = %d\n", historicoPartidas[pos].idPartida);
}

// Listar todas, por jogador ou buscar por parte da data (strstr)
void listarOuBuscarPartidas(void) {
    int modo, id, i, achou = 0;
    char termo[11];
    printf("\n1. Listar todas | 2. Partidas de um jogador | 3. Buscar por data\n");
    modo = lerInt("Escolha: ");
    if (modo == 1) {
        for (i = 0; i < totalPartidas; i++) {
            if (historicoPartidas[i].ativa == 1) {
                imprimirPartida(i);
                achou = 1;
            }
        }
    } else if (modo == 2) {
        id = lerInt("ID do jogador: ");
        for (i = 0; i < totalPartidas; i++) {
            if (historicoPartidas[i].ativa == 1 && historicoPartidas[i].idJogador == id) {
                imprimirPartida(i);
                achou = 1;
            }
        }
    } else if (modo == 3) {
        lerTexto("Data ou parte dela (ex: 05/11 ou /11/2026): ", termo, 11);
        for (i = 0; i < totalPartidas; i++) {
            if (historicoPartidas[i].ativa == 1 && contemTexto(historicoPartidas[i].data, termo)) {
                imprimirPartida(i);
                achou = 1;
            }
        }
    } else {
        printf("\n[ERRO] Opcao invalida!\n");
        return;
    }
    if (achou == 0) printf("\nNenhuma partida encontrada.\n");
}

void editarPontuacao(void) {
    int id, pos;
    id = lerInt("\nID da partida a corrigir: ");
    pos = buscarPartidaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Partida nao encontrada.\n");
        return;
    }
    historicoPartidas[pos].pontuacao = lerInt("Nova pontuacao: ");
    salvarPartidasCSV();
    printf("\nPontuacao corrigida!\n");
}

// Estorno: devolve as fichas ao jogador e invalida a partida (ativa = 0)
void estornarPartida(void) {
    int id, pos, posJ;
    id = lerInt("\nID da partida a estornar: ");
    pos = buscarPartidaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Partida nao encontrada.\n");
        return;
    }
    posJ = buscarJogadorPorId(historicoPartidas[pos].idJogador);
    if (posJ != -1) {
        listaJogadores[posJ].fichasAcumuladas += historicoPartidas[pos].fichasGastas;
        listaJogadores[posJ].gastoTotal -= historicoPartidas[pos].valorGasto;
    }
    historicoPartidas[pos].ativa = 0;
    salvarPartidasCSV();
    salvarJogadoresCSV();
    printf("\nPartida estornada e fichas devolvidas.\n");
}

// ----------------------------- RELATORIOS ----------------------------------

void relatorioHighScore(void) {
    int idM, i, melhor = -1, posJ;
    idM = lerInt("\nID da maquina: ");
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

void relatorioJogadorMaisPartidas(void) {
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

// MATRIZ 2D: linhas = jogadores, colunas = maquinas.
// matriz[p][m] = quantas partidas o jogador da posicao p fez na maquina da posicao m.
void relatorioMatrizJogadorMaquina(void) {
    int matriz[MAX][MAX] = {{0}};
    int i, j, posJ, posM;
    char rotulo[12];
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].ativa == 1) {
            posJ = buscarJogadorPorId(historicoPartidas[i].idJogador);
            posM = buscarMaquinaPorId(historicoPartidas[i].idMaquina);
            if (posJ != -1 && posM != -1) matriz[posJ][posM]++;
        }
    }
    printf("\n=== MATRIZ: PARTIDAS POR JOGADOR x MAQUINA ===\n");
    for (j = 0; j < totalMaquinas; j++) {
        if (catalogoMaquinas[j].ativa == 1) {
            printf("M%d = %s\n", catalogoMaquinas[j].idMaquina, catalogoMaquinas[j].nomeJogo);
        }
    }
    printf("\n%-6s", "");
    for (j = 0; j < totalMaquinas; j++) {
        if (catalogoMaquinas[j].ativa == 1) {
            sprintf(rotulo, "M%d", catalogoMaquinas[j].idMaquina);
            printf("%5s", rotulo);
        }
    }
    printf("\n");
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].ativo == 1) {
            printf("%-6s", listaJogadores[i].iniciais);
            for (j = 0; j < totalMaquinas; j++) {
                if (catalogoMaquinas[j].ativa == 1) printf("%5d", matriz[i][j]);
            }
            printf("\n");
        }
    }
}

// Top 5 maiores pontuacoes (de todas as maquinas) usando BUBBLE SORT.
void rankingPontuacoes(void) {
    int indices[MAX], n = 0, i, j, aux, limite;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].ativa == 1) indices[n++] = i;
    }
    if (n == 0) {
        printf("\nAinda nao ha partidas registradas.\n");
        return;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (historicoPartidas[indices[j]].pontuacao < historicoPartidas[indices[j + 1]].pontuacao) {
                aux = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = aux;
            }
        }
    }
    limite = n < 5 ? n : 5;
    printf("\n=== TOP %d MAIORES PONTUACOES ===\n", limite);
    for (i = 0; i < limite; i++) {
        printf("%do | ", i + 1);
        imprimirPartida(indices[i]);
    }
}

// ------------------------------- MENU --------------------------------------

void moduloPartidas(void) {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 3: PARTIDAS E HIGH SCORES (Paulo)] ---\n");
        printf("1. Registrar Nova Partida (Cruza Jogador + Maquina)\n");
        printf("2. Listar / Buscar Partidas\n");
        printf("3. Corrigir Pontuacao de Partida\n");
        printf("4. Estornar Partida\n");
        printf("5. [RELATORIO] High Score de uma Maquina\n");
        printf("6. [RELATORIO] Jogador com Mais Partidas\n");
        printf("7. [RELATORIO] Matriz Jogador x Maquina\n");
        printf("8. [RELATORIO] Top 5 Pontuacoes (Bubble Sort)\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInt("Escolha uma acao: ");
        switch (opcao) {
            case 1: registrarPartida(); break;
            case 2: listarOuBuscarPartidas(); break;
            case 3: editarPontuacao(); break;
            case 4: estornarPartida(); break;
            case 5: relatorioHighScore(); break;
            case 6: relatorioJogadorMaisPartidas(); break;
            case 7: relatorioMatrizJogadorMaquina(); break;
            case 8: rankingPontuacoes(); break;
            case 0: break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Partidas!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}
