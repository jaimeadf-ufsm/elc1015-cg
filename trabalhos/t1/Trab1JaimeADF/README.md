# YAP (Yet Another Paint)

Editor de imagens desenvolvido por Jaime Antonio Daniel Filho utilizando a biblioteca Canvas 2D.

## Requisitos implementados

Todos os requisitos obrigatórios e extras especificados do trabalho foram implementados:

### Requisitos obrigatórios
- Carregamento de imagens em formato BMP sem compactação e com 24 bits.
- Menu lateral com botões, sliders e checkboxes.
- Gerenciamento de camadas, incluindo criação, exclusão, reordenação e controle
de visiblidade.
- Suporte para transparência binária, mas também com suporte para transparência
com canal alfa.
- Plano de fundo quadriculado para identificar a transparência.
- Desenhos com o mouse na camada considerada ativa, sendo possível alterar a sua cor.
- Flip horizontal e vertical.
- Controle de brilho.

### Requisitos extras especificados
- Tons de Cinza (até 0.5 pt).
- Correção de Gama (até 0.5 pt).
- Contraste (até 0.5 pt).
- Desfoque Gaussiano (até 2 pt).
- Redimensionamento e movimentação (até 2 pt).
- Rotação de qualquer o ângulo com mouse (até 2 pt).
- Diferentes tipos de pincel/formas para desenhar com o mouse (até 2 pts).
- Salvar e carregar estado atual do programa, incluindo imagens, posição,
- ordem e visibilidade, entre outras variáveis (até 3 pt).
  
### Requisitos extras não-especificados
(Efeitos)
- Pixelar
- Ruído aleatório
- Sepia

(Ferramentas)
- Borracha
- Balde de tinta (Flood fill)
- Seletor de cores

(Outros)
- Salvar o projeto em BMP de 24 bits ou de 32 bits (com canal alfa).
- Leitura de BMP com 32 bits (com canal alfa).
- Abrir imagens utilizando um explorador de arquivos.
- Transparência total com canal alfa.
- Seletor de cores HSV com sliders.
- Preview do conteúdo de cada camada.
- Preview e controle dos efeitos.
- Interface responsiva de acordo com a resolução.

### Quickstart

Ao iniciar o programa, você se deparará com um projeto vazio. Você pode abrir a
imagens como camadas clicando no botão com o ícone de um arquivo no canto superior direito.
Ao selecionar arquivos ".bmp", você abrirá uma imagem como uma nova camada e,
ao abrir arquivos ".yap", você abrirá um estado anterior do programa. Na pasta
imagens, encontra-se um arquivo de projeto "demo.yap" para ser aberto, que
contêm as imagens "a.bmp", "b.bmp" e "c.bmp" previamente importadas.

OBS: No explorador de arquivos, o item ".." serve para navegar para a pasta
anterior e também é possível digitar manualmente o caminho na caixa de texto.

Para ativar uma camada, basta clicar sobre ela no menu lateral direito. Tendo uma
camada selecionada, é possível clicar sobre uma das ferramentas no menu lateral
esquerdo para selecioná-la e aplicá-la pressionando e arrastando o mouse na viewport.
A cor utilizada pode ser alterado no seletor no menu lateral direito.

As ações sobre a camada como flip horizontal, flip vertical e efeitos se encontram
no canto inferior esquerdo e podem ser aplicadas sob demanda.

Para mais informações, refira-se a seção da barra de ferramentas da interface.

Para salvar um projeto como um arquivo ".yap", basta clicar no botão com o ícone
de disquete no cabeçalho, navegar para o local desejado e digitar o nome e,
por fim, pressionar o botão de salvar.

Para exportar um projeto como um arquivo ".bmp", basta clicar no botão com o ícone
de compartilhar no cabeçalho e seguir os mesmos passos descritos para salvar um
projeto.

### Interface

A interface do programa é subdividida em quatro regiões:
- Cabeçalho
- Barra de ferramentas
- Viewport
- Menu lateral

Cada região

#### Cabeçalho

No cabeçalho, encontram-se os botões para importar arquivos ou exportar arquivos
do projetos. Na ordem da direita para esquerda, os botões são:

- Abrir arquivo (Ícone de um arquivo): Abre um popup para carregar imagens .bmp
ou projetos .yap. O usuário deve navegar pelo explorador de arquivos e selecionar
o item que deseja carregar. Ao clicar em "Abrir" com um arquivo com a extensão ".bmp"
selecionado, o programa irá carregar a imagem como uma nova camada. Por outro lado, ao
realizar a mesma ação com um arquivo co am extensão ".yap", o programa carregará
um projeto do editor, substituindo o estado atual do programa.

- Salvar projeto (Ícone de um disquete): Abre um popup com um explorador de
arquivos. O usuário deverá navegar para o diretório onde deseja salvar o projeto 
e digitar o nome do arquivo no campo de texto. Ao clicar em "Salvar", o projeto 
será escrito no local indicado.

- Exportar projeto (Ícone de compartilhar): Abre um popup com o explorador de
arquivos similar ao "Salvar projeto". 

OBS: No explorador de arquivos, é possível digitar manualmente o caminho na barra
superior. No entanto, somente caminhos relativos são suportados. Observe também
que só são mostrados 10 arquivos por vez e você pode mudar a página na barra
superior.

#### Barra de ferramentas

A barra de ferramentas encontra-se no lado esquerdo da interface. Os botões na
sua parte superior indicam as reais ferramentas que podem ser selecionadas e
utilizadas com o mouse na Viewport, ao passo que, na parte inferior, os botões
indicam ações, isto é, comportamentos que serão executados apenas uma únicas vez.

Entre as ferramentas, temos na ordem de cima para baixo:

OBS: As ferramentas e as ações em negrito exigem uma camada ativa sobre a qual irão atuar.

- **Mover** (Ícone de cruz): Aperte em qualquer lugar da viewport e arraste com
o mouse para alterar a posição da camada selecionada.

- **Escala** (Ícone de um retângulo): Pressione os pontos de âncora e arraste com
o mouse para redimensionar a imagem na direção desejada.

- **Rotação** (Ícone de rotação): Aperte em qualquer lugar da viewport e arraste
com o mouse para rotacionar a camada.

- **Pincél** (Ícone de pincel): Clique ou arraste sobre o região da da camada ativa
para pintar com a cor selecionado no menu lateral. É possível modificar o tamanho
píncel e o formato na barra superior da viewport.

- **Borracha** (Ícone de borracha): Possui o mesmo modo de uso do píncel.

- **Lata de tinta** (Ícone de lata): Aplica o algoritmo flood fill substituindo
a cor da posição pressionada na camada pela seleciona no menu lateral.

- Seletor de cor (Ícone de conta gotas): Modifica a cor do seletor pela a da
posição selecionada. Note que essa opção não exige uma camada.

Entre as ações, temos na ordem de cima para baixo:

- **Flip horizontal**: Aplica um espelho horizontal na camada ativa.

- **Flip vertical**: Aplica um espelho vertical na camada ativa.

- **Efeitos**: Abre o popup para aplicar efeitos no qual o efeito é escolhido
utilizando os botões de chevron (< e >) na parte superior do popup. Cada efeito
possui suas opções e é somente aplicado na camada ao pressionar o botão "Aplicar".

#### Viewport

No centro da viewport, encontra-se a área do canvas com dimensões de 640x480. É
nessa região sobre a qual as ferramentas atuam.

#### Menu Lateral

O menu lateral é composto pelo seletor de cores HSVA na parte superior e o
controle das camadas na parte inferior.

O seletor possui um retângulo principal para controlar a saturação e o valor e 
dois sliders para controlar a matiz e o canal alfa, respectivamente. 

No painél das camadas, é possível ocultar ou mostrar uma camada clicando sobre
o checkbox na sua parte esquerda e a ativar clicando sobre sua parte direita. 

As camadas são controlados pelos botões na parte inferior do painél, descritos
abaixo da direita para a esquerda:

- Criar camada (Ícone de mais): Crie uma camada vazia do mesmo tamanho do canvas.

- **Excluir camada** (Ícone de X): Remove a camada ativa.

- **Mover para cima** (Ícone de chevron para cima): Reordena a camada para cima.

- **Mover para baixo** (Ícone de chevron para baixo): Reordena a camada ativa para baixo.

