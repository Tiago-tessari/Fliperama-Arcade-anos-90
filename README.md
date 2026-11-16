# Fliperama-Arcade-anos-90
Projeto da faculdade - ARCADE PALACE (Fichas e High Scores - Anos 90)

Sistema em C para um fliperama: jogadores com fichas, catalogo de maquinas e
historico de partidas com high scores. Os dados ficam salvos em arquivos `.csv`.

## Arquivos

| Integrante | Arquivos |
|---|---|
| Tiago Tessari | `arcade.h`, `tiago.c` (jogadores + main + funcoes de apoio), `jogadores.csv` |
| Ludmila Ladislau | `ludmila.c` (maquinas), `maquinas.csv` |
| Paulo | `paulo.c` (partidas), `partidas.csv` |

## Como compilar e executar

Rode dentro da pasta (os `.csv` precisam ficar ao lado do executavel):

```
gcc -Wall -Wextra tiago.c ludmila.c paulo.c -o arcade
./arcade        (no Windows: arcade.exe)
```

## O que o sistema faz

- **Persistencia em CSV:** le os 3 arquivos ao abrir e regrava apos cadastrar, editar ou remover.
- **CRUD completo** nos 3 modulos, cada um em subfuncoes isoladas.
- **Busca textual** (`strstr`) por nome de jogador, nome do jogo e data da partida.
- **Cruzamento de modulos:** a partida valida jogador e maquina, calcula o valor e da baixa nas fichas.
- **Relatorios:** mais fichas, jogo mais jogado, high score, mais partidas.
- **Matriz 2D:** partidas por jogador x maquina.
- **Bubble Sort:** ranking de gastos, de faturamento das maquinas e top 5 pontuacoes.
