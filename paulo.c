// ============================================================================
// paulo.c - Modulo 3: PARTIDAS, HIGH SCORES E RELATORIOS
// Autor: Paulo (Sobrenome e RGM: ________)
// ============================================================================
#include <stdio.h>
#include <string.h>
#include "arcade.h"

struct Partida historicoPartidas[MAX];
int totalPartidas = 0;

// ============================================================================
// ARQUIVO CSV DAS PARTIDAS
// Formato de cada linha:  id;idJogador;idMaquina;pontuacao;valor;data;ativa
// ============================================================================

// Abre para LEITURA ("r") e copia cada linha do arquivo para o vetor.
void carregarPartidasCSV(void) {
    FILE *arq = fopen("partidas.csv", "r");
    char cabecalho[100];
    totalPartidas = 0;
    if (arq == NULL) return;
    fgets(cabecalho, sizeof(cabecalho), arq);    // pula a linha de titulos
    // %d = inteiro | %f = decimal | %10[^;] = texto de ate 10 letras ate o ';'
    // O fscanf devolve quantos campos leu: 7 = linha completa.
    while (totalPartidas < MAX &&
           fscanf(arq, "%d;%d;%d;%d;%f;%10[^;];%d\n",
                  &historicoPartidas[totalPartidas].idPartida,
                  &historicoPartidas[totalPartidas].idJogador,
                  &historicoPartidas[totalPartidas].idMaquina,
                  &historicoPartidas[totalPartidas].pontuacao,
                  &historicoPartidas[totalPartidas].valorGasto,
                  historicoPartidas[totalPartidas].data,
                  &historicoPartidas[totalPartidas].ativa) == 7) {
        totalPartidas++;
    }
    fclose(arq);
}

// Abre para ESCRITA ("w"): zera o arquivo e regrava o vetor inteiro.
void salvarPartidasCSV(void) {
    int i;
    FILE *arq = fopen("partidas.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar partidas.csv\n");
        return;
    }
    fprintf(arq, "id;idJogador;idMaquina;pontuacao;valor;data;ativa\n");
    for (i = 0; i < totalPartidas; i++) {
        fprintf(arq, "%d;%d;%d;%d;%.2f;%s;%d\n",
                historicoPartidas[i].idPartida, historicoPartidas[i].idJogador,
                historicoPartidas[i].idMaquina, historicoPartidas[i].pontuacao,
                historicoPartidas[i].valorGasto, historicoPartidas[i].data,
                historicoPartidas[i].ativa);
    }
    fclose(arq);
}

// ============================================================================
// MODULO DE PARTIDAS
// ============================================================================

static int buscarPartidaPorId(int id) {
    int i;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idPartida == id && historicoPartidas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

// Mostra a partida usando as INICIAIS do jogador e o NOME do jogo (cruzamento)
static void imprimirPartida(int i) {
    int posJ = buscarJogadorPorId(historicoPartidas[i].idJogador);
    int posM = buscarMaquinaPorId(historicoPartidas[i].idMaquina);
    printf("Partida %d | %s | %s | %d pts | R$ %.2f | %s\n",
           historicoPartidas[i].idPartida,
           posJ != -1 ? listaJogadores[posJ].iniciais : "???",
           posM != -1 ? catalogoMaquinas[posM].nomeJogo : "(removida)",
           historicoPartidas[i].pontuacao, historicoPartidas[i].valorGasto,
           historicoPartidas[i].data);
}

// CRUZA os modulos: valida o ID do jogador (Tiago) e o ID da maquina (Ludmila)
// e calcula o valor da partida = fichas por partida x preco da ficha.
void registrarPartida(void) {
    int idJ, idM, posJ, posM, pos;
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
    pos = totalPartidas;
    historicoPartidas[pos].idPartida = pos + 1;
    historicoPartidas[pos].idJogador = idJ;
    historicoPartidas[pos].idMaquina = idM;
    historicoPartidas[pos].pontuacao = lerInt("Pontuacao feita na partida: ");
    printf("Data da partida (dd/mm/aaaa): ");
    scanf("%10s", historicoPartidas[pos].data);
    historicoPartidas[pos].valorGasto = catalogoMaquinas[posM].fichasPorPartida *
                                        catalogoMaquinas[posM].precoFicha;
    historicoPartidas[pos].ativa = 1;
    totalPartidas++;
    salvarPartidasCSV();
    printf("\nPartida registrada! ID = %d | Valor: R$ %.2f\n",
           historicoPartidas[pos].idPartida, historicoPartidas[pos].valorGasto);
}

// Listar todas, listar de um jogador ou buscar por parte da data (strstr)
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
        printf("Data ou parte dela (ex: 05/11 ou /11/2026): ");
        scanf("%10s", termo);
        for (i = 0; i < totalPartidas; i++) {
            // strstr procura "termo" dentro da data; devolve NULL se nao achar
            if (historicoPartidas[i].ativa == 1 && strstr(historicoPartidas[i].data, termo) != NULL) {
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

// Remocao logica: ativa = 0 (a partida deixa de aparecer e de contar nos relatorios)
void removerPartida(void) {
    int id, pos;
    id = lerInt("\nID da partida a remover: ");
    pos = buscarPartidaPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Partida nao encontrada.\n");
        return;
    }
    historicoPartidas[pos].ativa = 0;
    salvarPartidasCSV();
    printf("\nPartida removida.\n");
}

// RELATORIO: maior pontuacao de uma maquina (o "high score" do fliperama)
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

// RELATORIO que cruza dados: conta as partidas validas de cada jogador
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

// MATRIZ 2D: cada LINHA e um jogador e cada COLUNA e uma maquina.
// matriz[p][m] = quantas partidas o jogador da posicao p fez na maquina da posicao m.
void relatorioMatrizJogadorMaquina(void) {
    int matriz[MAX][MAX] = {{0}};     // comeca tudo zerado
    int i, j, posJ, posM;
    // 1) preenche a matriz percorrendo as partidas
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].ativa == 1) {
            posJ = buscarJogadorPorId(historicoPartidas[i].idJogador);
            posM = buscarMaquinaPorId(historicoPartidas[i].idMaquina);
            if (posJ != -1 && posM != -1) matriz[posJ][posM]++;
        }
    }
    // 2) mostra: legenda das colunas, cabecalho e uma linha por jogador
    printf("\n=== MATRIZ: PARTIDAS POR JOGADOR x MAQUINA ===\n");
    for (j = 0; j < totalMaquinas; j++) {
        if (catalogoMaquinas[j].ativa == 1) {
            printf("M%d = %s\n", catalogoMaquinas[j].idMaquina, catalogoMaquinas[j].nomeJogo);
        }
    }
    printf("\n    ");
    for (j = 0; j < totalMaquinas; j++) {
        if (catalogoMaquinas[j].ativa == 1) printf("%4d", catalogoMaquinas[j].idMaquina);
    }
    printf("   (numero da maquina)\n");
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].ativo == 1) {
            printf("%-4s", listaJogadores[i].iniciais);
            for (j = 0; j < totalMaquinas; j++) {
                if (catalogoMaquinas[j].ativa == 1) printf("%4d", matriz[i][j]);
            }
            printf("\n");
        }
    }
}

void moduloPartidas(void) {
    int opcao;
    do {
        printf("\n--- [MODULO 3: PARTIDAS E HIGH SCORES (Paulo)] ---\n");
        printf("1. Registrar Nova Partida (Cruza Jogador + Maquina)\n");
        printf("2. Listar / Buscar Partidas\n");
        printf("3. Corrigir Pontuacao de Partida\n");
        printf("4. Remover Partida\n");
        printf("5. [RELATORIO] High Score de uma Maquina\n");
        printf("6. [RELATORIO] Jogador com Mais Partidas\n");
        printf("7. [RELATORIO] Matriz Jogador x Maquina\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInt("Escolha uma acao: ");
        switch (opcao) {
            case 1: registrarPartida(); break;
            case 2: listarOuBuscarPartidas(); break;
            case 3: editarPontuacao(); break;
            case 4: removerPartida(); break;
            case 5: relatorioHighScore(); break;
            case 6: relatorioJogadorMaisPartidas(); break;
            case 7: relatorioMatrizJogadorMaquina(); break;
            case 0: printf("\nRetornando ao menu principal...\n"); break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Partidas!\n");
        }
    } while (opcao != 0);
}
