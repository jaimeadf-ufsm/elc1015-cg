/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         02/2025
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.1
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <chrono>

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Benchmark.h"

#include "BMP.h"
#include "Bitmap.h"

#include "RenderingContext.h"
#include "RenderingEngine.h"
#include "Screen.h"

#include "Workspace.h"

/*
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
*/

std::shared_ptr<yap::Screen> screen;

yap::RenderingContext renderingContext;
yap::RenderingEngine renderingEngine;

std::shared_ptr<yap::Benchmark> frameBenchmark;
std::shared_ptr<yap::Benchmark> processBenchmark;
std::shared_ptr<yap::Benchmark> renderBenchmark;

int windowWidth = 1280;
int windowHeight = 720;

void render()
{
   frameBenchmark->Stop();
   frameBenchmark->Start();

   renderingContext.ClearCommands();

   renderBenchmark->Start();
   screen->Resize(windowWidth, windowHeight);
   screen->Render(renderingContext);
   renderBenchmark->Stop();

   processBenchmark->Start();
   renderingEngine.ExecuteCommands(renderingContext.GetCommands());
   processBenchmark->Stop();

   if (frameBenchmark->GetSamples() % 100 == 0)
   {
      printf(
         "FPS: %.2lf (Render: %.2lfms, Execute: %.2lfms)\n",
         1.0 / frameBenchmark->GetAverageTime(),
         renderBenchmark->GetAverageTime() * 1000.0,
         processBenchmark->GetAverageTime() * 1000.0
      );

      frameBenchmark->Reset();
      renderBenchmark->Reset();
      processBenchmark->Reset();
   }
}

void keyboard(int key)
{
   // printf("\nTecla: %d" , key);
   screen->ProcessKeyboardDown(key);
}

void keyboardUp(int key)
{
   // printf("\nLiberou: %d" , key);
   screen->ProcessKeyboardUp(key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if (button == -2 && state == -2 && wheel == -2 && direction == -2)
   {
      screen->ProcessMouseMove(x, y);
   }
   else if (button != -2)
   {
      if (state == 0)
      {
         screen->ProcessMouseDown((yap::MouseButton)button);
      }
      else
      {
         screen->ProcessMouseUp((yap::MouseButton)button);
      }
   }
   else if (wheel != -2 && direction != -2)
   {
      screen->ProcessMouseScroll((yap::MouseScrollDirection)direction);
   }
}

int main(void)
{
   screen = std::make_shared<yap::Screen>();

   frameBenchmark = std::make_shared<yap::Benchmark>();
   processBenchmark = std::make_shared<yap::Benchmark>();
   renderBenchmark = std::make_shared<yap::Benchmark>();

   screen->Init();
   screen->Root->AddChild(std::make_shared<yap::Workspace>());

   CV::init(&windowWidth, &windowHeight, "YAP - Yet Another Paint (Jaime Antonio Daniel Filho)");
   CV::run();
}
