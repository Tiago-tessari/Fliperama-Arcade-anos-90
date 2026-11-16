// ============================================================================
// arcade.h - Cabecalho compartilhado do projeto "Arcade Anos 90"
// Contem as structs (escopo global) e os prototipos usados entre os modulos.
// Quem sobe este arquivo primeiro no Git: Tiago
// ============================================================================
#ifndef ARCADE_H
#define ARCADE_H

#define MAX 50   // maximo de registros por modulo

// Struct do Tiago: jogadores do clube do fliperama
struct Jogador {
    int idJogador;
    char nome[50];
    char iniciais[4];      // 3 letras do placar, ex: "TIA" (+1 posicao para o '\0')
    int fichasAcumuladas;  // fichas compradas no caixa
    int ativo;             // 1 = Ativo, 0 = Removido
};

// Struct da Ludmila: maquinas / jogos do fliperama
struct Maquina {
    int idMaquina;
    char nomeJogo[40];
    int fichasPorPartida;
    float precoFicha;      // preco de cada ficha em R$
    int ativa;             // 1 = Ativa, 0 = Removida
};

// Struct do Paulo: partidas jogadas e pontuacoes (high scores)
struct Partida {
    int idPartida;
    int idJogador;         // ID de um jogador (cruza com o modulo do Tiago)
    int idMaquina;         // ID de uma maquina (cruza com o modulo da Ludmila)
    int pontuacao;
    float valorGasto;      // custo da partida em R$
    char data[11];         // formato dd/mm/aaaa (+1 posicao para o '\0')
    int ativa;             // 1 = Valida, 0 = Removida
};

// Vetores globais (cada um e definido no arquivo do seu autor)
extern struct Jogador listaJogadores[MAX];
extern int totalJogadores;
extern struct Maquina catalogoMaquinas[MAX];
extern int totalMaquinas;
extern struct Partida historicoPartidas[MAX];
extern int totalPartidas;

// Funcoes de leitura do teclado (definidas no tiago.c)
int   lerInt(const char *msg);     // le um inteiro (repete se digitar errado)
float lerFloat(const char *msg);   // le um decimal (repete se digitar errado)

// Arquivos .csv (cada autor grava e le o seu)
void carregarJogadoresCSV(void);   // Tiago
void salvarJogadoresCSV(void);     // Tiago
void carregarMaquinasCSV(void);    // Ludmila
void salvarMaquinasCSV(void);      // Ludmila
void carregarPartidasCSV(void);    // Paulo
void salvarPartidasCSV(void);      // Paulo

// Modulos (chamados pelo main)
void moduloJogadores(void);    // Tiago
void moduloMaquinas(void);     // Ludmila
void moduloPartidas(void);     // Paulo

// Funcoes de busca usadas por mais de um modulo
int buscarJogadorPorId(int id);   // Tiago - retorna posicao no vetor ou -1
int buscarMaquinaPorId(int id);   // Ludmila - retorna posicao no vetor ou -1

#endif
