// ============================================================================
// arcade.h - Cabecalho compartilhado do projeto "Arcade Anos 90"
// Contem as structs (escopo global) e os prototipos usados entre os modulos.
// Quem sobe este arquivo primeiro no Git: Tiago
// ============================================================================
#ifndef ARCADE_H
#define ARCADE_H

#define MAX 50

// Struct do Tiago: jogadores do clube do fliperama
struct Jogador {
    int idJogador;
    char nome[50];
    char iniciais[4];      // 3 letras do placar, ex: "TIA"
    int fichasAcumuladas;
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
    int ativa;             // 1 = Valida, 0 = Estornada
};

// Vetores globais (cada um e definido no arquivo do seu autor)
extern struct Jogador listaJogadores[MAX];
extern int totalJogadores;
extern struct Maquina catalogoMaquinas[MAX];
extern int totalMaquinas;
extern struct Partida historicoPartidas[MAX];
extern int totalPartidas;

// Modulos (chamados pelo main)
void moduloJogadores();    // Tiago
void moduloMaquinas();     // Ludmila
void moduloPartidas();     // Paulo

// Funcoes de busca usadas por mais de um modulo
int buscarJogadorPorId(int id);   // Tiago - retorna posicao no vetor ou -1
int buscarMaquinaPorId(int id);   // Ludmila - retorna posicao no vetor ou -1

#endif
