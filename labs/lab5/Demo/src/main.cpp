#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Vector2.h"

int screenWidth = 500, screenHeight = 500;

void translate(Vector2 in[4], Vector2 out[4], const Vector2& offset)
{
   for (int i = 0; i < 4; i++)
   {
      out[i].X = in[i].X + offset.X;
      out[i].Y = in[i].Y + offset.Y;
   }
}

void rotate(Vector2 in[4], Vector2 out[4], float angle)
{
   for (int i = 0; i < 4; i++)
   {
      out[i].X = in[i].X * cos(angle) - in[i].Y * sin(angle);
      out[i].Y = in[i].X * sin(angle) + in[i].Y * cos(angle);
   }
}

void scale(Vector2 in[4], Vector2 out[4], const Vector2& scale)
{
   for (int i = 0; i < 4; i++)
   {
      out[i].X = in[i].X * scale.X;
      out[i].Y = in[i].Y * scale.Y;
   }
}

void draw(Vector2 in[4], int color)
{
   CV::color(color);

   for (int i = 0; i < 4; i++)
   {
      CV::line(in[i].X, in[i].Y, in[(i + 1) % 4].X, in[(i + 1) % 4].Y);
   }
}

void clone(Vector2 in[4], Vector2 out[4])
{
   for (int i = 0; i < 4; i++)
   {
      out[i].X = in[i].X;
      out[i].Y = in[i].Y;
   }
}

void render()
{
   CV::translate(screenWidth / 2, screenHeight / 2);

   CV::color(2);
   CV::line(0, 0, screenWidth / 2, 0);
   CV::line(0, 0, 0, screenWidth / 2);

   Vector2 in[4] = {
      Vector2(-20, -20),
      Vector2(20, -20),
      Vector2(20, 20),
      Vector2(-20, 20)
   };

   translate(in, in, Vector2(100, 100));

   Vector2 out1[4];
   Vector2 out2[4];
   Vector2 out3[4];
   Vector2 out4[4];

   draw(in, 2);

   translate(in, out1, Vector2(-100, -80));
   draw(out1, 3);

   scale(out1, out2, Vector2(1.5, 1.5));
   draw(out2, 4);

   rotate(out2, out3, PI / 4);
   draw(out3, 5);

   translate(out3, out4, Vector2(120, 120));
   draw(out4, 6);
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
   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
