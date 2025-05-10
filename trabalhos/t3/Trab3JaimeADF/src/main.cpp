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

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <iostream>

#include "gl_canvas2d.h"

#include "Game.h"

#include "Benchmark.h"

#include "DrawingContext.h"
#include "DrawingEngine.h"
#include "Tesselator.h"

int screenWidth = 1280, screenHeight = 720;

Game game;

DrawingContext drawingContext;
DrawingEngine drawingEngine;

Path path;

Benchmark frameBenchmark;

Benchmark updateBenchmark;
Benchmark renderBenchmark;
Benchmark executeBenchmark;

void render()
{
   frameBenchmark.Stop();

   drawingContext.ClearCommands();

   updateBenchmark.Start();
   game.Update(0.016f);
   updateBenchmark.Stop();

   renderBenchmark.Start();
   game.Render(drawingContext);
   renderBenchmark.Stop();

   executeBenchmark.Start();
   drawingEngine.ExecuteCommands(drawingContext.GetCommands());
   executeBenchmark.Stop();

   if (frameBenchmark.GetSamples() == 100)
   {
      // std::cout << 1.0 / frameBenchmark.GetAverageTime() << " FPS ";
      // std::cout << "(";
      // std::cout << "Update: " << updateBenchmark.GetAverageTime() * 1000.0f << " ms, ";
      // std::cout << "Render: " << renderBenchmark.GetAverageTime() * 1000.0f << " ms, ";
      // std::cout << "Execute: " << executeBenchmark.GetAverageTime() * 1000.0f << " ms, ";
      // std::cout << ")";
      // std::cout << std::endl;

      frameBenchmark.Reset();
      updateBenchmark.Reset();
      renderBenchmark.Reset();
      executeBenchmark.Reset();
   }

   frameBenchmark.Start();
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   // printf("\nTecla: %d" , key);
   game.HandleEvent(Event::CreateKeyPressEvent(key));
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   // printf("\nLiberou: %d" , key);
   game.HandleEvent(Event::CreateKeyReleaseEvent(key));
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   Vector2 position(x, y);

   if (button == -2 && state == -2 && wheel == -2 && direction == -2)
   {
      game.HandleEvent(Event::CreateMouseMoveEvent(position));
   }

   if (button != -2 && state != -2)
   {
      if (state == 0)
      {
         game.HandleEvent(Event::CreateMouseButtonPressEvent(position, button));
      }
      else
      {
         game.HandleEvent(Event::CreateMouseButtonReleaseEvent(position, button));
      }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
