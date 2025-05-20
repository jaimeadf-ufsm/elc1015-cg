# Top-Down Tank

Um jogo top-down de tanques desenvolvido por Jaime Antonio Daniel Filho utilizando a biblioteca Canvas 2D.

## Requisitos implementados

Todos os requisitos obrigatórios e extras especificados do trabalho foram implementados:

### Requisitos obrigatórios
- Pista desenhada com curvas de bezier cúbicas.
- Editor de pistas.
- Movimentação do tanque com o teclado (A/D) e do canhão com o mouse.
- Disparo de projéteis.
- Placar de pontaução.
- Colisão entre projéteis, alvos, pistas, etc...
- Controle de FPS.
- Barra de vida do tanque e danos na colisão.
- Todos os movimentos são feitas conforme o FPS.

### Requisitos extras especificados

- Alvos com diferentes lógicas. (até 1 pt)
- Alvos com diferentes visuais e colisão apropriada (estrela, barril e quadrado). (até 2 pt)
- Power-ups (cura, invincibilidade e disparos rápidos). (até 1 pt)
- Efeitos de explosão bonitos. (até 2 pt)
- Preenchimento colorido da pista com detalhes. (até 1 pt)
- Diferentes níveis de dificuldade ao explodir todos os alvos. (até 1 pt)

### Requisitos extras não-especificados
- Sons (Explosão e Tiro).
- Armazenamento de pistas e de pontuações em um arquivo.
- Histórico de pontuações.
- Limitador de FPS.
- Animações nos diferentes tipos de objetos.
- Todos os desenhos são feitos utilizado triângulos com as primitivas do Canvas 2D (nada é um bitmap).
- Triangularizador de polígonos convexos e concâvos.
- Parser de comandos utilizados em SVG para desenhar qualquer elemento na tela.
- Colisão genérica utilizando SAT (Separting Axis Theorem).

### Quickstart

Você inicia no Modo Editor e a partir dele entre nos outros modos. O FPS é controlado por um define no arquivo principal.

OBS: No jogo, não há botões. Todo o controle é feito utilizando as teclas do teclado e os movimentos do mouse.

#### Modo Editor

Mouse:
- Arraste os pontos da curva com o mouse para movimentá-los.
- Clique com o botão esquerdo sobre a curva para dividá-la em dois segmentos de bézier.
- Clique com o botão direito sobre um ponto da curva que não seja um ponto de controle para remover aquele segmento. Observe que você é impedido de remover se o restar somente dois segmentos.

Teclado:
- *0 a 9*: Escolha do slot de pista. 
- *M*: Aumenta a largura da pista.
- *N*: Diminui a largura da pista.
- *O*: Habilita ou desabilita a estilização da pista.
- *P*: Entra no Modo Jogatina com a pista selecionada.
- *S*: Entra no Modo Placar.

#### Modo Placar

Teclado:
- ESC: Volta para o Modo Editor.

#### Modo Jogativa

Mouse:
- Clique com o botão esquerdo para disparar o canhão.

Teclado:
- *A,D*: Movimentação da rotação do tanque.

