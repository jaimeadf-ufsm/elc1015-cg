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

#include "RenderingContext.h"
#include "RenderingEngine.h"
#include "Screen.h"

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
   printf("\nTecla: %d" , key);
   screen.ProcessKeyboardDown(key);
}

void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
   screen.ProcessKeyboardUp(key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
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

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

void OnMouseMove(yap::Element &element, yap::Vec2 position)
{
   printf("Mouse moved to (%f, %f)\n", position.X, position.Y);
}

void OnMouseEnter(yap::Element &element)
{
   element.Background = yap::ColorRGB(0.0f, 1.0f, 0.0f);
   printf("Mouse entered\n");
}

void OnMouseLeave(yap::Element &element)
{
   element.Background = yap::ColorRGB(1.0f, 0.0f, 0.0f);
   printf("Mouse left\n");
}

void OnPress(yap::Element &element)
{
   element.Background = yap::ColorRGB(0.0f, 0.0f, 1.0f);
   printf("Mouse pressed\n");
}

void OnRelease(yap::Element &element)
{
   element.Background = yap::ColorRGB(1.0f, 0.0f, 1.0f);
   printf("Mouse released\n");
}

int main(void)
{
   yap::Box &root = screen.GetRoot();
   root.Padding = yap::BoxPadding(16);
   root.Direction = yap::BoxDirection::Column;

   for (int i = 0; i < 3; i++)
   {
      std::shared_ptr<yap::Box> box = std::make_shared<yap::Box>();
      box->Position = i == 1 ? yap::PositioningRule::Relative(yap::Vec2(50, 50)) : yap::PositioningRule::Static();
      box->Size.Width = i == 1 ? yap::AxisSizingRule::Fill() : yap::AxisSizingRule::Fixed(200);
      box->Size.Height = yap::AxisSizingRule::Fixed(200);

      box->Background = yap::ColorRGB(1.0, 0.0, 0.0);

      box->OnMouseMove = OnMouseMove;
      box->OnMouseEnter = OnMouseEnter;
      box->OnMouseLeave = OnMouseLeave;
      box->OnPress = OnPress;
      box->OnRelease = OnRelease;

      root.AddChild(box);
   }

   CV::init(&windowWidth, &windowHeight, "YAP - Yet Another Paint (Jaime Antonio Daniel Filho)");
   CV::run();
}
