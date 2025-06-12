# Modelador e visualizador 3D com Curvas de Bézier

Aplicativo para modelagem e renderização de objetos 3D com curvas de Bézier, utilizando técnicas de revolução ou sweep helicoidal. Toda a renderização é feita via software (CPU), com geração de imagem pixel a pixel.

A interface é dividida em dois paineis principais:

* **Editor (à esquerda):** criação e configuração da curva de Bézier e do tipo de extrusão.
* **Visualizador (à direita):** renderização do objeto 3D final com opções de visualização e controle de câmera.

<br/>

> **Aviso:**
> * Na compilação, selecione o modo Release para melhor desempenho.

## Requisitos implementados

### Requisitos básicos

* [x] Modelagem do objeto com curvas de bézier (revolução e sweep) para gerar garrafas, donuts e molas.
* [x] Rotação e translação do objeto usando as teclas do teclado.
* [x] Visualização em wireframe de triângulos com projeção ortográfica e em perspectiva.
* [x] Parametrização do número de segmentos da curva e do número de segmentos da revolução ou do sweep para controlar o número de faces e de pontos do objeto gerado.

### Requisitos extras
* [x] Exibição de vetores normais de cada face (até 1 ponto).
* [x] Adição de mais de 4 pontos de controle por patch por meio do controle do grau da curva (até 1 ponto).
* [x] Adição de mais de um patch por meio da criação de vários pontos em sequência (até 1 ponto).
* [x] Remoção de faces ocultas sem preenchimento utilizando back-face culling (até 2 pontos).
* [x] Preenchimento do objeto com z-buffer e com scanline usando iluminação por pixel (até 5 pontos).
* [x] Câmera que pode andar, rotacionar ou aproximar o espaço.
* [x] Textura com imagens BMP.
* [x] Translação da luz para alterar a fonte da iluminação por pixel.
* [x] Diferentes tipos de materiais para iluminar o objeto (borracha, plástico e metal).
* [x] Refração e difusão.
* [x] Cena com vários modelos (esferas, flechas e o objeto).
* [x] Exibição de vetores normais de cada vértice.
* [x] Responsividade para qualquer resolução.

## Quickstart

* **Use o teclado** para controlar praticamente todas as funções.
* **Use o mouse** para interagir com pontos da curva no Editor e movimentar a câmera no Visualizador.

## Editor (Painel Esquerdo)

### Teclas:

#### **Edição da Curva de Bézier:**

* `z` / `Z`: aumenta / diminui os **segmentos da curva**.
* `x` / `X`: aumenta / diminui os **segmentos do caminho** (circunferência ou mola).
* `v` / `V`: aumenta / diminui o **grau da curva**.
* `c`: alterna entre **curva aberta e fechada**.
* `+`: imprime os pontos de controle.

#### **Configurações da Mola (Sweep):**

* `b` / `B`: aumenta / diminui o **raio da mola**.
* `n` / `N`: aumenta / diminui a **altura da mola**.
* `m` / `M`: aumenta / diminui a **frequência da mola**.

#### **Tipo de Objeto:**

* `g`: alterna entre **revolução** e **sweep (mola)**.

### Mouse:

* **Adicionar um ponto:** clique com o **botão esquerdo** em um espaço vazio.
* **Remover um ponto:** clique com o **botão direito** sobre o ponto desejado.
* **Mover um ponto:** clique e segure o **botão esquerdo** sobre um ponto e arraste.

<br/>

> **Importante:**
> * Os pontos devem ser definidos no sentido anti-horário (de baixo para cima, da direita para esquerda).
> * O grau da curva define quantos pontos são usados por patch. A quantidade de pontos de controle influencia diretamente o número de patches.

---

## 2. Visualizador (Painel Direito)

### Teclas:

#### **Projeção e Renderização:**

* `q`: alterna entre **projeção ortográfica** e **perspectiva**.
* `r`: alterna entre **wireframe**, **sólido** ou **iluminação por pixel**.
* `f`: alterna entre **exibição de normais** (**nenhum**, **por vértice** ou **por face**).
* `e`: ativa/desativa **remoção de faces ocultas (back-face culling)**.
* `espaço`: reseta a cena.

#### **Controle de Transformações:**

* `p`: alterna o **alvo de transformação** (**posição do objeto**, **rotação do objeto** ou **posição da luz**).
* `w`, `a`, `s`, `d`: movimenta a **câmera**.
* `i`, `j`, `k`, `l`, `u`, `o`: movimenta o **alvo selecionado**.

#### **Materiais, Texturas e Cores:**

* `y`: alterna entre **borracha**, **plástico** ou **metal**.
* `1`–`4`: aplica **cor sólida** ao objeto (**branca**, **vermelha**, **verde**, **azul**).
* `5`–`9`: aplica **textura de imagem** ao objeto (carregadas de **textures/X.bmp**).

### Mouse:

* **Mover a câmera:** clique e segure o **botão esquerdo** enquanto arrasta.
* **Zoom da câmera:** gire o **scroll** para alterar o FOV na projeção em perspectiva ou o tamanho da caixa na projeção ortográfica.
