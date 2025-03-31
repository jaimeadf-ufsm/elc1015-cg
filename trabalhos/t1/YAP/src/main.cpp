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


int screenWidth = 1280, screenHeight = 720;
int mouseX, mouseY;

yap::RenderingContext context;
std::shared_ptr<yap::Box> root = std::make_shared<yap::Box>();
std::shared_ptr<yap::Box> child1 = std::make_shared<yap::Box>();
std::shared_ptr<yap::Box> child2 = std::make_shared<yap::Box>();
std::shared_ptr<yap::Box> child3 = std::make_shared<yap::Box>();

void render()
{
   context.ClearCommands();

   const std::vector<yap::RenderingCommand>& commands = context.GetCommands();

   root->ComputeIndependentDimensions();
   root->ComputeResponsiveDimensions();
   root->ComputePosition();
   root->Draw(context);

   for (const auto& command : commands)
   {
      switch (command.GetKind())
      {
      case yap::RenderingCommandKind::Color:
         {
            const auto& args = command.GetColorArgs();
            CV::color(args.R, args.G, args.B, args.A);

            printf(
               "Color(R = %.2f, G = %.2f, B = %.2f, A = %.2f)\n",
               args.R,
               args.G,
               args.B,
               args.A
            );
         }

         break;
      case yap::RenderingCommandKind::FillRectangle:
         {
            const auto& args = command.GetFillRectangleArgs();
            
            for (int x = args.X; x < args.X + args.Width; x++)
            {
               for (int y = args.Y; y < args.Y + args.Height; y++)
               {
                  CV::color(1.0f, 0.0f, 0.0f, 1.0f);
                  CV::point((float)x, (float)y);
               }
            }

            // CV::rectFill(args.X, args.Y, args.X + args.Width, args.Y + args.Height);

            printf(
               "FillRectangle(X = %d, Y = %d, Width = %d, Height = %d)\n",
               args.X,
               args.Y,
               args.Width,
               args.Height
            );
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
   root->Width = yap::SizingRule::Fixed(500);
   root->Height = yap::SizingRule::Fit();
   root->Direction = yap::BoxDirection::Row;
   root->Position = yap::PositioningRule::Float(200, 200);

   root->Background = yap::ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f);

   child1->Width = yap::SizingRule::Fixed(200);
   child1->Height = yap::SizingRule::Fixed(200);
   child1->Background = yap::ColorRGBA(1.0f, 0.0f, 0.0f, 1.0f);

   child2->Width = yap::SizingRule::Fill();
   child2->Height = yap::SizingRule::Fixed(100);
   child2->Background = yap::ColorRGBA(0.0f, 1.0f, 0.0f, 1.0f);

   child3->Width = yap::SizingRule::Fill();
   child3->Height = yap::SizingRule::Fixed(100);
   child3->Background = yap::ColorRGBA(0.0f, 1.0f, 1.0f, 0.5f);

   root->AddChild(child1);
   root->AddChild(child2);
   root->AddChild(child3);

   CV::init(screenWidth, screenHeight, "YAP - Yet Another Paint (Jaime Antonio Daniel Filho)");
   CV::run();
}
