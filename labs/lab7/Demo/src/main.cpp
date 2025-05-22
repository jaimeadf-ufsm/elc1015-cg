#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

#include "gl_canvas2d.h"

#include "Vector2.h"
#include "Vector3.h"

int screenWidth = 1280, screenHeight = 720;
float angle = 0.0f;

std::vector<Vector3> originalCube = {
   Vector3(1, 1, 1),
   Vector3(1, -1, 1),
   Vector3(-1, -1, 1),
   Vector3(-1, 1, 1),

   Vector3(1, 1, -1),
   Vector3(1, -1, -1),
   Vector3(-1, -1, -1),
   Vector3(-1, 1, -1),
};

void draw(const std::vector<Vector3>& cube)
{
   CV::color(0);
   CV::line(cube[0].X, cube[0].Y, cube[1].X, cube[1].Y);

   CV::color(1);
   CV::line(cube[1].X, cube[1].Y, cube[2].X, cube[2].Y);

   CV::color(2);
   CV::line(cube[2].X, cube[2].Y, cube[3].X, cube[3].Y);

   CV::color(3);
   CV::line(cube[3].X, cube[3].Y, cube[0].X, cube[0].Y);


   CV::color(4);
   CV::line(cube[4].X, cube[4].Y, cube[5].X, cube[5].Y);

   CV::color(5);
   CV::line(cube[5].X, cube[5].Y, cube[6].X, cube[6].Y);

   CV::color(6);
   CV::line(cube[6].X, cube[6].Y, cube[7].X, cube[7].Y);

   CV::color(7);
   CV::line(cube[7].X, cube[7].Y, cube[4].X, cube[4].Y);


   CV::color(8);
   CV::line(cube[0].X, cube[0].Y, cube[4].X, cube[4].Y);

   CV::color(9);
   CV::line(cube[1].X, cube[1].Y, cube[5].X, cube[5].Y);

   CV::color(10);
   CV::line(cube[2].X, cube[2].Y, cube[6].X, cube[6].Y);

   CV::color(11);
   CV::line(cube[3].X, cube[3].Y, cube[7].X, cube[7].Y);
}

Vector3 rotateY(Vector3 p, float angle)
{
   Vector3 result;
   result.X = p.X * cos(angle) + p.Z * sin(angle);
   result.Y = p.Y;
   result.Z = - p.X * sin(angle) + p.Z * cos(angle);

   return result;
}

Vector3 translateZ(Vector3 p, float offset)
{
   Vector3 result;
   result.X = p.X;
   result.Y = p.Y;
   result.Z = p.Z - offset;

   return result;
}

Vector3 projectA(Vector3 p, float d)
{
   Vector3 result;
   result.X = p.X * d / p.Z;
   result.Y = p.Y * d / p.Z;
   result.Z = 0;

   return result;
}

void render()
{
   std::vector<Vector3> cube;
   cube.resize(originalCube.size());

   float d = 100.0f;

   CV::translate(screenWidth / 2, screenHeight / 2);

   for (size_t i = 0; i < originalCube.size(); i++)
   {
      Vector3 p = originalCube[i];
      // p *= 2.0f;
      p = rotateY(p, angle);
      p = translateZ(p, 3);
      p = projectA(p, d);

      cube[i] = p;
   }

   draw(cube);

   angle += 0.001f;
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
