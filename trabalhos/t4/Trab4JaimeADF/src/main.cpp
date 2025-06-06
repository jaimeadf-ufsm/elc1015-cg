#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

int screenWidth = 1280, screenHeight = 720;

void render()
{
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
   CV::init(&screenWidth, &screenHeight, "T3 (Jaime Antonio Daniel Filho)");
   CV::run();
}
