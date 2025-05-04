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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "DrawingContext.h"
#include "DrawingEngine.h"
#include "Tesselator.h"

int screenWidth = 1280, screenHeight = 720;
float mx, my; //coordenadas do mouse

DrawingContext drawingContext;
DrawingEngine drawingEngine;

PolyLine polyline;
std::vector<Triangle> triangles;

void render()
{
   drawingContext.ClearCommands();

   polyline.Close();

   Tesselator::Stroke(polyline, triangles, 10.0f, 0.0f);

   for (const auto& triangle : triangles)
   {
      drawingContext.Color(ColorRGB::Red);
      drawingContext.BeginPolygon();
      drawingContext.Vertex(triangle.A);
      drawingContext.Vertex(triangle.B);
      drawingContext.Vertex(triangle.C);
      drawingContext.FillPolygon();

      // drawingContext.Color(ColorRGB::Blue);
      // drawingContext.BeginPolygon();
      // drawingContext.Vertex(triangle.A);
      // drawingContext.Vertex(triangle.B);
      // drawingContext.Vertex(triangle.C);
      // drawingContext.StrokePolygon();
   }

   for (const auto& vertex : polyline.GetPoints())
   {
      drawingContext.Color(ColorRGB::Black);
      drawingContext.FillRectangle(vertex - Vector2(2, 2), Vector2(4, 4));
   }

   drawingEngine.ExecuteCommands(drawingContext.GetCommands());
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if (button == 0 && state == 0) //botao esquerdo pressionado
   {
      polyline.AddPoint(Vector2(x, y));
   }

   mx = x;
   my = y;
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
