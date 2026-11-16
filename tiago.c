// ============================================================================
// tiago.c - Modulo 1: JOGADORES (fichas) + funcoes de apoio + main (menu)
// Autor: Tiago Tessari (RGM: 39969321)
// ============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "arcade.h"

struct Jogador listaJogadores[MAX];
int totalJogadores = 0;

// ============================================================================
// FUNCOES DE APOIO (usadas por todos os modulos)
// ============================================================================
// Limpa o console (cls no Windows, clear no Linux/Mac)
void limparTela(void) {
#ifdef _WIN32
    int r = system("cls");
#else
    int r = system("clear");
#endif
    (void)r;
}

// Para a tela ate o usuario apertar ENTER (senao o limparTela apaga o resultado)
void pausar(void) {
    char buf[100];
    printf("\nPressione ENTER para continuar...");
    if (fgets(buf, sizeof(buf), stdin) == NULL) exit(0);
}

// Le uma linha inteira e converte para int. Repete ate o usuario digitar certo.
int lerInt(const char *msg) {
    char buf[100];
    int v;
    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) exit(0);
        if (sscanf(buf, "%d", &v) == 1) return v;
        printf("[ERRO] Digite um numero inteiro valido.\n");
    }
}

// Le um numero decimal. Troca virgula por ponto para aceitar "2,50" ou "2.50".
float lerFloat(const char *msg) {
    char buf[100];
    float v;
    int i;
    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) exit(0);
        for (i = 0; buf[i] != '\0'; i++) {
            if (buf[i] == ',') buf[i] = '.';
        }
        if (sscanf(buf, "%f", &v) == 1) return v;
        printf("[ERRO] Digite um numero valido.\n");
    }
}

// Le um texto com espacos usando fgets. O ';' vira espaco porque e o
// separador do CSV (se ficasse, quebraria as colunas do arquivo).
void lerTexto(const char *msg, char *dest, int tam) {
    char buf[200];
    int i;
    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) exit(0);
        buf[strcspn(buf, "\r\n")] = '\0';           // tira o ENTER do final
        for (i = 0; buf[i] != '\0'; i++) {
            if (buf[i] == ';') buf[i] = ' ';
        }
        if (strlen(buf) > 0) break;
        printf("[ERRO] O texto nao pode ficar vazio.\n");
    }
    strncpy(dest, buf, tam - 1);
    dest[tam - 1] = '\0';
}

// Busca textual: devolve 1 se "busca" aparece dentro de "texto" (strstr).
// Converte tudo para minusculo antes, para "mario" achar "Super Mario".
int contemTexto(const char *texto, const char *busca) {
    char a[200], b[200];
    int i;
    for (i = 0; texto[i] != '\0' && i < 199; i++) a[i] = (char)tolower((unsigned char)texto[i]);
    a[i] = '\0';
    for (i = 0; busca[i] != '\0' && i < 199; i++) b[i] = (char)tolower((unsigned char)busca[i]);
    b[i] = '\0';
    return strstr(a, b) != NULL;
}

// ============================================================================
// MODULO DE JOGADORES
// ============================================================================
// ---------------------------- ARQUIVO CSV ----------------------------------

// Formato de cada linha: id;nome;iniciais;fichas;gasto;ativo
// A 1a linha do arquivo e o cabecalho (pulada na leitura).
void carregarJogadoresCSV(void) {
    FILE *arq = fopen("jogadores.csv", "r");
    char cabecalho[200];
    totalJogadores = 0;
    if (arq == NULL) return;                         // primeira execucao: sem arquivo
    if (fgets(cabecalho, sizeof(cabecalho), arq) == NULL) { fclose(arq); return; }
    // %49[^;] = le texto ate encontrar ';' (aceita nomes com espaco)
    while (totalJogadores < MAX &&
           fscanf(arq, "%d;%49[^;];%3[^;];%d;%f;%d\n",
                  &listaJogadores[totalJogadores].idJogador,
                  listaJogadores[totalJogadores].nome,
                  listaJogadores[totalJogadores].iniciais,
                  &listaJogadores[totalJogadores].fichasAcumuladas,
                  &listaJogadores[totalJogadores].gastoTotal,
                  &listaJogadores[totalJogadores].ativo) == 6) {
        totalJogadores++;
    }
    fclose(arq);
}

// "w" apaga o arquivo antigo e o for reescreve o vetor atualizado inteiro.
// Serve para cadastro, edicao e remocao.
void salvarJogadoresCSV(void) {
    int i;
    FILE *arq = fopen("jogadores.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar jogadores.csv\n");
        return;
    }
    fprintf(arq, "id;nome;iniciais;fichas;gasto;ativo\n");
    for (i = 0; i < totalJogadores; i++) {
        fprintf(arq, "%d;%s;%s;%d;%.2f;%d\n",
                listaJogadores[i].idJogador, listaJogadores[i].nome,
                listaJogadores[i].iniciais, listaJogadores[i].fichasAcumuladas,
                listaJogadores[i].gastoTotal, listaJogadores[i].ativo);
    }
    fclose(arq);
}

// ------------------------------ BUSCAS -------------------------------------

int buscarJogadorPorId(int id) {
    int i;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador == id && listaJogadores[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

// Novo ID = maior ID ja usado + 1 (nao repete ID mesmo apos remocoes)
static int proximoIdJogador(void) {
    int i, maior = 0;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador > maior) maior = listaJogadores[i].idJogador;
    }
    return maior + 1;
}

static void imprimirJogador(int pos) {
    printf("ID %d | %s (%s) | Fichas: %d | Gasto: R$ %.2f\n",
           listaJogadores[pos].idJogador, listaJogadores[pos].nome,
           listaJogadores[pos].iniciais, listaJogadores[pos].fichasAcumuladas,
           listaJogadores[pos].gastoTotal);
}

// ------------------------------- CRUD --------------------------------------

void cadastrarJogador(void) {
    int pos, i, fichas;
    if (totalJogadores >= MAX) {
        printf("\n[ERRO] Limite de jogadores atingido!\n");
        return;
    }
    pos = totalJogadores;
    listaJogadores[pos].idJogador = proximoIdJogador();
    lerTexto("\nNome do jogador: ", listaJogadores[pos].nome, 50);
    lerTexto("Iniciais para o placar (3 letras): ", listaJogadores[pos].iniciais, 4);
    for (i = 0; listaJogadores[pos].iniciais[i] != '\0'; i++) {
        listaJogadores[pos].iniciais[i] = (char)toupper((unsigned char)listaJogadores[pos].iniciais[i]);
    }
    do {
        fichas = lerInt("Fichas iniciais: ");
        if (fichas < 0) printf("[ERRO] Nao pode ser negativo.\n");
    } while (fichas < 0);
    listaJogadores[pos].fichasAcumuladas = fichas;
    listaJogadores[pos].gastoTotal = 0;
    listaJogadores[pos].ativo = 1;
    totalJogadores++;
    salvarJogadoresCSV();
    printf("\nJogador cadastrado! ID = %d\n", listaJogadores[pos].idJogador);
}

// Listar todos, buscar por ID ou buscar por parte do nome (strstr)
void listarOuBuscarJogador(void) {
    int modo, id, i, pos, achou = 0;
    char termo[50];
    printf("\n1. Listar todos | 2. Buscar por ID | 3. Buscar por nome\n");
    modo = lerInt("Escolha: ");
    if (modo == 1) {
        for (i = 0; i < totalJogadores; i++) {
            if (listaJogadores[i].ativo == 1) {
                imprimirJogador(i);
                achou = 1;
            }
        }
        if (achou == 0) printf("\nNenhum jogador cadastrado.\n");
    } else if (modo == 2) {
        id = lerInt("ID do jogador: ");
        pos = buscarJogadorPorId(id);
        if (pos == -1) printf("\n[ERRO] Jogador nao encontrado.\n");
        else imprimirJogador(pos);
    } else if (modo == 3) {
        lerTexto("Parte do nome: ", termo, 50);
        for (i = 0; i < totalJogadores; i++) {
            if (listaJogadores[i].ativo == 1 && contemTexto(listaJogadores[i].nome, termo)) {
                imprimirJogador(i);
                achou = 1;
            }
        }
        if (achou == 0) printf("\nNenhum jogador com esse nome.\n");
    } else {
        printf("\n[ERRO] Opcao invalida!\n");
    }
}

void editarJogador(void) {
    int id, pos, fichasExtras;
    id = lerInt("\nID do jogador a editar: ");
    pos = buscarJogadorPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    lerTexto("Novo nome: ", listaJogadores[pos].nome, 50);
    do {
        fichasExtras = lerInt("Fichas a adicionar (compra no caixa): ");
        if (fichasExtras < 0) printf("[ERRO] Nao pode ser negativo.\n");
    } while (fichasExtras < 0);
    listaJogadores[pos].fichasAcumuladas += fichasExtras;
    salvarJogadoresCSV();
    printf("\nJogador atualizado! Fichas agora: %d\n", listaJogadores[pos].fichasAcumuladas);
}

// Remocao logica: ativo = 0. O registro continua no CSV para nao
// quebrar o historico de partidas que usam esse ID.
void removerJogador(void) {
    int id, pos;
    id = lerInt("\nID do jogador a remover: ");
    pos = buscarJogadorPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    listaJogadores[pos].ativo = 0;
    salvarJogadoresCSV();
    printf("\nJogador removido.\n");
}

// ----------------------------- RELATORIOS ----------------------------------

void relatorioJogadorMaisFichas(void) {
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

// Ranking por gasto total usando BUBBLE SORT (maior gasto primeiro).
// Ordena um vetor de POSICOES, assim o vetor original nao muda de ordem.
void rankingJogadoresPorGasto(void) {
    int indices[MAX], n = 0, i, j, aux;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].ativo == 1) indices[n++] = i;
    }
    if (n == 0) {
        printf("\nNenhum jogador cadastrado.\n");
        return;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (listaJogadores[indices[j]].gastoTotal < listaJogadores[indices[j + 1]].gastoTotal) {
                aux = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = aux;
            }
        }
    }
    printf("\n=== RANKING: QUEM MAIS GASTOU NO FLIPERAMA ===\n");
    for (i = 0; i < n; i++) {
        printf("%2do | %-20s (%s) | R$ %.2f\n", i + 1,
               listaJogadores[indices[i]].nome, listaJogadores[indices[i]].iniciais,
               listaJogadores[indices[i]].gastoTotal);
    }
}

// ------------------------------- MENU --------------------------------------

void moduloJogadores(void) {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 1: JOGADORES (Tiago)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar / Buscar Jogadores\n");
        printf("3. Editar Jogador / Comprar Fichas\n");
        printf("4. Remover Jogador\n");
        printf("5. [RELATORIO] Jogador com Mais Fichas\n");
        printf("6. [RELATORIO] Ranking de Gastos (Bubble Sort)\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInt("Escolha uma acao: ");
        switch (opcao) {
            case 1: cadastrarJogador(); break;
            case 2: listarOuBuscarJogador(); break;
            case 3: editarJogador(); break;
            case 4: removerJogador(); break;
            case 5: relatorioJogadorMaisFichas(); break;
            case 6: rankingJogadoresPorGasto(); break;
            case 0: break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Jogadores!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

// ============================================================================
// MAIN - Despachante de menus
// ============================================================================
static void exibirCabecalho(void) {
    printf("======================================================\n");
    printf("   ARCADE PALACE - FICHAS E HIGH SCORES (ANOS 90)     \n");
    printf("======================================================\n");
}

int main(void) {
    int opcaoPrincipal;
    setlocale(LC_CTYPE, "Portuguese");   // so LC_CTYPE: nao troca o ponto decimal do CSV

    // Carrega os 3 arquivos .csv para a memoria (RAM) ao abrir o programa
    carregarJogadoresCSV();
    carregarMaquinasCSV();
    carregarPartidasCSV();

    do {
        limparTela();
        exibirCabecalho();
        printf("SELECIONE A PERSPECTIVA DO SISTEMA:\n");
        printf("1. Modulo de Jogadores / Fichas (Tiago)\n");
        printf("2. Modulo de Maquinas / Jogos (Ludmila)\n");
        printf("3. Modulo de Partidas e High Scores (Paulo)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        opcaoPrincipal = lerInt("Digite a opcao desejada: ");
        switch (opcaoPrincipal) {
            case 1: moduloJogadores(); break;
            case 2: moduloMaquinas(); break;
            case 3: moduloPartidas(); break;
            case 0: printf("\nEncerrando o Arcade Palace... GAME OVER!\n"); break;
            default:
                printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 3.\n");
                pausar();
        }
    } while (opcaoPrincipal != 0);
    return 0;
}
