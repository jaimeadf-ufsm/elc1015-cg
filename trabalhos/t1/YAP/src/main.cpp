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

#include "BMP.h"
#include "Bitmap.h"

#include "RenderingContext.h"
#include "RenderingEngine.h"
#include "Screen.h"

#include "Image.h"

yap::Screen screen;

yap::RenderingContext renderingContext;
yap::RenderingEngine renderingEngine;

int windowWidth = 800;
int windowHeight = 600;

void render()
{
   renderingContext.ClearCommands();

   screen.Resize(windowWidth, windowHeight);
   screen.Render(renderingContext);

   renderingEngine.ProcessCommands(renderingContext.GetCommands());

   Sleep(10);
}

void keyboard(int key)
{
   // printf("\nTecla: %d" , key);
   screen.ProcessKeyboardDown(key);
}

void keyboardUp(int key)
{
   // printf("\nLiberou: %d" , key);
   screen.ProcessKeyboardUp(key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if (button == -2 && state == -2 && wheel == -2 && direction == -2)
   {
      screen.ProcessMouseMove(x, y);
   }
   else if (button != -2)
   {
      if (state == 0)
      {
         screen.ProcessMouseDown((yap::MouseButton)button);
      }
      else
      {
         screen.ProcessMouseUp((yap::MouseButton)button);
      }
   }
   else if (wheel != -2 && direction != -2)
   {
      screen.ProcessMouseScroll((yap::MouseScrollDirection)direction);
   }
}

int main(void)
{
   std::shared_ptr<yap::Bitmap> a = std::make_shared<yap::Bitmap>(std::move(yap::BMP::Load("YAP/images/a.bmp")));

   yap::BMP::Save("YAP/images/b.bmp", *a);

   yap::Box &root = screen.GetRoot();
   root.Padding = yap::BoxPadding(16);
   root.Background = yap::ColorRGB(1.0f, 1.0f, 1.0f);
   root.Alignment.Horizontal = yap::BoxAxisAlignment::Center;
   root.Alignment.Vertical = yap::BoxAxisAlignment::Center;

   std::shared_ptr<yap::Image> image = std::make_shared<yap::Image>();
   image->Source = a;

   root.AddChild(image);

   CV::init(&windowWidth, &windowHeight, "YAP - Yet Another Paint (Jaime Antonio Daniel Filho)");
   CV::run();
}
