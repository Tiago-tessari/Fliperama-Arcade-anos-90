// ============================================================================
// tiago.c - Modulo 1: JOGADORES (fichas) + leitura do teclado + main (menu)
// Autor: Tiago Tessari (RGM: 39969321)
// ============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "arcade.h"

struct Jogador listaJogadores[MAX];
int totalJogadores = 0;

// ============================================================================
// LEITURA DO TECLADO (usada por todos os modulos)
// Se o usuario digitar letra onde pede numero, o scanf falha: limpamos a
// linha errada e perguntamos de novo (evita loop infinito).
// ============================================================================
int lerInt(const char *msg) {
    int valor, ok, c;
    do {
        printf("%s", msg);
        ok = scanf("%d", &valor);
        if (ok != 1) {
            while ((c = getchar()) != '\n' && c != EOF) { }
            if (c == EOF) exit(0);
            printf("[ERRO] Digite um numero inteiro valido.\n");
        }
    } while (ok != 1);
    return valor;
}

float lerFloat(const char *msg) {
    float valor;
    int ok, c;
    do {
        printf("%s", msg);
        ok = scanf("%f", &valor);
        if (ok != 1) {
            while ((c = getchar()) != '\n' && c != EOF) { }
            if (c == EOF) exit(0);
            printf("[ERRO] Digite um numero valido (use ponto: 0.50).\n");
        }
    } while (ok != 1);
    return valor;
}

// ============================================================================
// ARQUIVO CSV DOS JOGADORES
// Formato de cada linha:  id;nome;iniciais;fichas;ativo
// ============================================================================

// Abre o arquivo para LEITURA ("r") e copia cada linha para o vetor.
void carregarJogadoresCSV(void) {
    FILE *arq = fopen("jogadores.csv", "r");     // FILE* aponta para o arquivo aberto
    char cabecalho[100];
    totalJogadores = 0;
    if (arq == NULL) return;                     // primeira execucao: arquivo ainda nao existe
    fgets(cabecalho, sizeof(cabecalho), arq);    // pula a 1a linha (titulos das colunas)
    // Mascaras do fscanf:  %d = inteiro | %49[^;] = texto ate encontrar ';' | %3[^;] = ate 3 letras
    // O retorno do fscanf e quantos campos ele leu: se for 5, a linha estava completa.
    while (totalJogadores < MAX &&
           fscanf(arq, "%d;%49[^;];%3[^;];%d;%d\n",
                  &listaJogadores[totalJogadores].idJogador,
                  listaJogadores[totalJogadores].nome,
                  listaJogadores[totalJogadores].iniciais,
                  &listaJogadores[totalJogadores].fichasAcumuladas,
                  &listaJogadores[totalJogadores].ativo) == 5) {
        totalJogadores++;
    }
    fclose(arq);
}

// Abre para ESCRITA ("w"): apaga o arquivo velho e o for regrava o vetor inteiro.
// Serve para salvar apos cadastrar, editar ou remover.
void salvarJogadoresCSV(void) {
    int i;
    FILE *arq = fopen("jogadores.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar jogadores.csv\n");
        return;
    }
    fprintf(arq, "id;nome;iniciais;fichas;ativo\n");
    for (i = 0; i < totalJogadores; i++) {
        fprintf(arq, "%d;%s;%s;%d;%d\n",
                listaJogadores[i].idJogador, listaJogadores[i].nome,
                listaJogadores[i].iniciais, listaJogadores[i].fichasAcumuladas,
                listaJogadores[i].ativo);
    }
    fclose(arq);
}

// ============================================================================
// MODULO DE JOGADORES
// ============================================================================

// Retorna a posicao do jogador no vetor (ou -1 se nao existir / foi removido)
int buscarJogadorPorId(int id) {
    int i;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador == id && listaJogadores[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

static void imprimirJogador(int pos) {
    printf("ID %d | %s (%s) | Fichas: %d\n",
           listaJogadores[pos].idJogador, listaJogadores[pos].nome,
           listaJogadores[pos].iniciais, listaJogadores[pos].fichasAcumuladas);
}

void cadastrarJogador(void) {
    int pos;
    if (totalJogadores >= MAX) {
        printf("\n[ERRO] Limite de jogadores atingido!\n");
        return;
    }
    pos = totalJogadores;
    listaJogadores[pos].idJogador = pos + 1;
    printf("\nNome do jogador (sem espacos, use _): ");
    scanf("%49s", listaJogadores[pos].nome);
    printf("Iniciais para o placar (3 letras): ");
    scanf("%3s", listaJogadores[pos].iniciais);
    listaJogadores[pos].fichasAcumuladas = lerInt("Fichas iniciais: ");
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
        printf("Parte do nome (respeite maiusculas): ");
        scanf("%49s", termo);
        for (i = 0; i < totalJogadores; i++) {
            // strstr procura "termo" dentro do nome; devolve NULL se nao achar
            if (listaJogadores[i].ativo == 1 && strstr(listaJogadores[i].nome, termo) != NULL) {
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
    int id, pos;
    id = lerInt("\nID do jogador a editar: ");
    pos = buscarJogadorPorId(id);
    if (pos == -1) {
        printf("\n[ERRO] Jogador nao encontrado.\n");
        return;
    }
    printf("Novo nome (sem espacos, use _): ");
    scanf("%49s", listaJogadores[pos].nome);
    listaJogadores[pos].fichasAcumuladas += lerInt("Fichas a adicionar (compra no caixa): ");
    salvarJogadoresCSV();
    printf("\nJogador atualizado! Fichas agora: %d\n", listaJogadores[pos].fichasAcumuladas);
}

// Remocao logica: ativo = 0 (o registro fica no CSV para nao quebrar as partidas antigas)
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

void moduloJogadores(void) {
    int opcao;
    do {
        printf("\n--- [MODULO 1: JOGADORES (Tiago)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar / Buscar Jogadores\n");
        printf("3. Editar Jogador / Comprar Fichas\n");
        printf("4. Remover Jogador\n");
        printf("5. [RELATORIO] Jogador com Mais Fichas\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInt("Escolha uma acao: ");
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
// ============================================================================
void exibirCabecalho(void) {
    printf("\n======================================================\n");
    printf("   ARCADE PALACE - FICHAS E HIGH SCORES (ANOS 90)     \n");
    printf("======================================================\n");
}

int main(void) {
    int opcaoPrincipal;
    setlocale(LC_CTYPE, "Portuguese");

    // Ao abrir o programa, carrega os 3 arquivos .csv para a memoria (vetores)
    carregarJogadoresCSV();
    carregarMaquinasCSV();
    carregarPartidasCSV();

    do {
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
            default: printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 3.\n");
        }
    } while (opcaoPrincipal != 0);
    return 0;
}
