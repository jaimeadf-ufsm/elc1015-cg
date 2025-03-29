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
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"


int screenWidth = 500, screenHeight = 500;
int mouseX, mouseY;

void render()
{
   Sleep(10);
}

void keyboard(int key)
{
   printf("\nTecla: %d" , key);
}

void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x;
   mouseY = y;

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "YAP - Yet Another Paint");
   CV::run();
}
