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

std::shared_ptr<yap::Screen> screen;

yap::RenderingContext renderingContext;
yap::RenderingEngine renderingEngine;

yap::Benchmark frameBenchmark;
yap::Benchmark processBenchmark;
yap::Benchmark renderBenchmark;

int windowWidth = 1280;
int windowHeight = 720;

void render()
{
   frameBenchmark.Stop();
   frameBenchmark.Start();

   // renderingContext.ClearCommands();

   renderBenchmark.Start();
   // screen->Resize(windowWidth, windowHeight);
   // screen->Render(renderingContext);
   renderBenchmark.Stop();

   processBenchmark.Start();
   // renderingEngine.ProcessCommands(renderingContext.GetCommands());

   for (int i = 0; i < 400; ++i)
   {
      for (int j = 0; j < 400; ++j)
      {
         CV::color(0, 0, 0);
         CV::rectFill(i, j, i + 1, j + 1);
      }
   }
   processBenchmark.Stop();

   if (frameBenchmark.GetSamples() % 100 == 0)
   {
      printf(
         "FPS: %.2lf (Render: %.2lfms, Process: %.2lfms)\n",
         1.0 / frameBenchmark.GetAverageTime(),
         renderBenchmark.GetAverageTime() * 1000.0,
         processBenchmark.GetAverageTime() * 1000.0
      );

      frameBenchmark.Reset();
      renderBenchmark.Reset();
      processBenchmark.Reset();
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

   screen->Init();
   screen->Root->AddChild(std::make_shared<yap::Workspace>());

   CV::init(&windowWidth, &windowHeight, "YAP - Yet Another Paint (Jaime Antonio Daniel Filho)");
   CV::run();
}
