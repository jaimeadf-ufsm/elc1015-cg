#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <chrono>

#include "gl_canvas2d.h"

#include "UI.h"
#include "EditorPanel.h"
#include "PreviewPanel.h"
#include "Time.h"

using frame_clock = std::chrono::steady_clock;

int screenWidth = Window::GetWidth(), screenHeight = Window::GetHeight();

static frame_clock::time_point s_LastFrameTime;

void render()
{
   frame_clock::time_point lastFrameTime = s_LastFrameTime;
   frame_clock::time_point currentFrameTime = frame_clock::now();

   s_LastFrameTime = currentFrameTime;

   if (s_LastFrameTime.time_since_epoch().count() == 0)
   {
      return;
   }

   Time::SetDeltaTime(std::chrono::duration<float>(currentFrameTime - lastFrameTime).count());

   if (screenWidth != static_cast<int>(Window::GetWidth()) || screenHeight != static_cast<int>(Window::GetHeight()))
   {
      Window::SetSize(screenWidth, screenHeight);
      UI::Dispatch(Event::CreateWindowResizeEvent(screenWidth, screenHeight));
   }

   UI::Update();
   UI::Draw();
}

void keyboard(int key)
{
   // printf("\nTecla: %d" , key);
   Keyboard::PressKey(key);
   UI::Dispatch(Event::CreateKeyPressEvent(key));
}

void keyboardUp(int key)
{
   // printss("\nLiberou: %d" , key);
   Keyboard::ReleaseKey(key);
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

   if (wheel != -2 && direction != -2)
   {
      if (wheel == 0)
      {
         UI::Dispatch(Event::CreateMouseScrollEvent(Vector2(0.0f, direction), Vector2(x, y)));
      }
   }
}

int main(void)
{
   UI::RegisterPanel(std::make_shared<PreviewPanel>());
   UI::RegisterPanel(std::make_shared<EditorPanel>());
   
   CV::init(&screenWidth, &screenHeight, "T3 (Jaime Antonio Daniel Filho)");
   CV::run();
}
