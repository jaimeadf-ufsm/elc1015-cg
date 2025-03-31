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
#include "Box.h"


int screenWidth = 500, screenHeight = 500;
int mouseX, mouseY;

yap::RenderingContext context;
yap::Box root;

void render()
{
   context.ClearCommands();

   const std::vector<yap::RenderingCommand>& commands = context.GetCommands();

   for (const auto& command : commands)
   {
      switch (command.Kind)
      {
      case yap::RenderingCommandKind::Color:
         {
            const auto& args = command.ColorArgs;
            CV::color(args.R, args.G, args.B, args.A);
         }
         break;
      case yap::RenderingCommandKind::FillRectangle:
         {
            const auto& args = command.FillRectangleArgs;
            CV::rectFill(args.X, args.Y, args.X + args.Width, args.Y + args.Height);
         }
         break;
      default:
         break;
      }
   }

   

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
   auto a = yap::AxisSizingRule::Fixed(200);
   a = yap::AxisSizingRule::Fit();

   yap::SizingRule t = yap::SizingRule(yap::AxisSizingRule::Fixed(200), yap::AxisSizingRule::Fixed(200));
   t = yap::SizingRule(yap::AxisSizingRule::Fill(), yap::AxisSizingRule::Fill());

   root.Background = yap::ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f);

   CV::init(&screenWidth, &screenHeight, "YAP - Yet Another Paint (Jaime Antonio Daniel Filho)");
   CV::run();
}
