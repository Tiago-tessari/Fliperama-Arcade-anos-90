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
    char iniciais[4];      // 3 letras do placar, ex: "TIA" (+1 para o '\0')
    int fichasAcumuladas;
    float gastoTotal;      // total gasto em R$ nas partidas
    int ativo;             // 1 = Ativo, 0 = Removido
};

// Struct da Ludmila: maquinas / jogos do fliperama
struct Maquina {
    int idMaquina;
    char nomeJogo[40];
    int fichasPorPartida;
    float precoFicha;
    int ativa;             // 1 = Ativa, 0 = Removida
};

// Struct do Paulo: partidas jogadas e pontuacoes (high scores)
struct Partida {
    int idPartida;
    int idJogador;
    int idMaquina;
    int pontuacao;
    int fichasGastas;
    float valorGasto;      // custo da partida em R$
    char data[11];         // formato dd/mm/aaaa (+1 para o '\0')
    int ativa;             // 1 = Valida, 0 = Estornada
};

// Vetores globais (cada um e definido no arquivo do seu autor)
extern struct Jogador listaJogadores[MAX];
extern int totalJogadores;
extern struct Maquina catalogoMaquinas[MAX];
extern int totalMaquinas;
extern struct Partida historicoPartidas[MAX];
extern int totalPartidas;

// Funcoes de apoio (definidas no tiago.c, usadas por todos)
void limparTela(void);                                   // limpa o console
void pausar(void);                                       // espera ENTER
int   lerInt(const char *msg);                           // le inteiro com validacao
float lerFloat(const char *msg);                         // le decimal (aceita , ou .)
void  lerTexto(const char *msg, char *dest, int tam);    // le texto COM espacos
int   contemTexto(const char *texto, const char *busca); // strstr sem diferenciar maiuscula

// Arquivos .csv (cada autor grava/le o seu)
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
