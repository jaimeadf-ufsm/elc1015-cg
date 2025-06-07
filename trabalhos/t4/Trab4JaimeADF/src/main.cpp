#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "UI.h"
#include "EditorPanel.h"

int screenWidth = Window::GetWidth(), screenHeight = Window::GetHeight();

void render()
{
   if (screenWidth != static_cast<int>(Window::GetWidth()) || screenHeight != static_cast<int>(Window::GetHeight()))
   {
      Window::SetSize(screenWidth, screenHeight);
      UI::Dispatch(Event::CreateWindowResizeEvent(screenWidth, screenHeight));
   }

   UI::Draw();
}

void keyboard(int key)
{
   // printf("\nTecla: %d" , key);
   UI::Dispatch(Event::CreateKeyPressEvent(key));
}

void keyboardUp(int key)
{
   // printf("\nLiberou: %d" , key);
   UI::Dispatch(Event::CreateKeyReleaseEvent(key));
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   Mouse::SetPosition(Vector2(x, y));

   if (button == -2 && state == -2 && wheel == -2 && direction == -2)
   {
      UI::Dispatch(Event::CreateMouseMoveEvent(Vector2(x, y)));
   }

   if (button != -2 && state != -2)
   {
      if (state == 0)
      {
         UI::Dispatch(Event::CreateMouseButtonPressEvent(button, Vector2(x, y)));
      }
      else
      {
         UI::Dispatch(Event::CreateMouseButtonReleaseEvent(button, Vector2(x, y)));
      }
   }
}

int main(void)
{
   UI::RegisterPanel(std::make_shared<EditorPanel>());
   
   CV::init(&screenWidth, &screenHeight, "T3 (Jaime Antonio Daniel Filho)");
   CV::run();
}
