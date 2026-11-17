# Fliperama-Arcade-anos-90
Projeto da faculdade - ARCADE PALACE (Fichas e High Scores - Anos 90)

Sistema em C para um fliperama: jogadores com fichas, catalogo de maquinas e
historico de partidas com high scores. Os dados ficam salvos em arquivos `.csv`.

## Arquivos

| Integrante | Arquivos |
|---|---|
| Tiago Tessari | `arcade.h`, `tiago.c` (jogadores + main), `jogadores.csv` |
| Ludmila Ladislau | `ludmila.c` (maquinas), `maquinas.csv` |
| Paulo | `paulo.c` (partidas), `partidas.csv` |

## O que o sistema faz

- Le os 3 arquivos `.csv` ao abrir e regrava apos cadastrar, editar ou remover.
- Cadastrar, Listar, Buscar (`strstr`), Editar e Remover em funcoes separadas.
- Cruzamento: a partida valida o jogador e a maquina e calcula o valor.
- Relatorios: mais fichas, jogo mais jogado, high score, jogador com mais partidas.
- Matriz 2D: partidas por jogador x maquina.
