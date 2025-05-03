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

int screenWidth = 1280, screenHeight = 720;

DrawingContext drawingContext;
DrawingEngine drawingEngine;

void render()
{
   drawingContext.ClearCommands();

   drawingContext.Color(ColorRGB::Red);
   drawingContext.BeginPolygon();
   drawingContext.Vertex(Vector2(0, 0));
   drawingContext.Vertex(Vector2(100, 0));
   drawingContext.Vertex(Vector2(100, 100));
   drawingContext.Vertex(Vector2(0, 100));
   drawingContext.FillPolygon();

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
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
