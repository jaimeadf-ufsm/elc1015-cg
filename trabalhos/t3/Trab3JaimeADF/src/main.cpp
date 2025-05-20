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
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <iostream>
#include <chrono>
#include <thread>

#include "gl_canvas2d.h"

#include "Game.h"
#include "MainScene.h"
#include "EditorScene.h"

#include "Benchmark.h"

#include "DrawingContext.h"
#include "DrawingEngine.h"

using frame_clock = std::chrono::steady_clock;

#define FPS 99999
#define FILENAME "Trab3JaimeADF/save.txt"

int screenWidth = 1280, screenHeight = 720;

Game game;

DrawingContext drawingContext;
DrawingEngine drawingEngine;

Benchmark frameBenchmark;

Benchmark updateBenchmark;
Benchmark renderBenchmark;
Benchmark executeBenchmark;

frame_clock::time_point lastFrameTime;

void render()
{
    frame_clock::time_point previousFrameTime = lastFrameTime;
    frame_clock::time_point currentFrameTime = frame_clock::now();

    lastFrameTime = currentFrameTime;

    if (previousFrameTime == std::chrono::steady_clock::time_point())
    {
        return;
    }

    float deltaTime = std::chrono::duration<float>(currentFrameTime - previousFrameTime).count();

    frameBenchmark.Stop();
    frameBenchmark.Start();

    drawingContext.ClearCommands();

    updateBenchmark.Start();
    game.Update(deltaTime);
    updateBenchmark.Stop();

    renderBenchmark.Start();
    game.Draw(drawingContext);
    renderBenchmark.Stop();

    executeBenchmark.Start();
    drawingEngine.ExecuteCommands(drawingContext.GetCommands());
    executeBenchmark.Stop();

    if (frameBenchmark.GetSamples() == 100)
    {
        std::cout << 1.0 / frameBenchmark.GetAverageTime() << " FPS ";
        std::cout << "(";
        std::cout << "Update: " << updateBenchmark.GetAverageTime() * 1000.0f << " ms, ";
        std::cout << "Draw: " << renderBenchmark.GetAverageTime() * 1000.0f << " ms, ";
        std::cout << "Execute: " << executeBenchmark.GetAverageTime() * 1000.0f << " ms";
        std::cout << ")";
        std::cout << std::endl;

        frameBenchmark.Reset();
        updateBenchmark.Reset();
        renderBenchmark.Reset();
        executeBenchmark.Reset();
    }

    frame_clock::time_point renderEndTime = frame_clock::now();
    float renderDuration = (renderEndTime - currentFrameTime).count();

    if (renderDuration < 1.0f / FPS)
    {
        std::this_thread::sleep_for(std::chrono::duration<float>(1.0f / FPS - renderDuration));
    }
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    // printf("\nTecla: %d" , key);
    game.HandleEvent(Event::CreateKeyPressEvent(key));
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    // printf("\nLiberou: %d" , key);
    game.HandleEvent(Event::CreateKeyReleaseEvent(key));
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    Vector2 position(x, y);

    if (button == -2 && state == -2 && wheel == -2 && direction == -2)
    {
        game.HandleEvent(Event::CreateMouseMoveEvent(position));
    }

    if (button != -2 && state != -2)
    {
        if (state == 0)
        {
            game.HandleEvent(Event::CreateMouseButtonPressEvent(position, button));
        }
        else
        {
            game.HandleEvent(Event::CreateMouseButtonReleaseEvent(position, button));
        }
    }
}

void exiting() {
    std::cout << "Saving game state..." << std::endl;
    game.GetState().Save(FILENAME);
}

int main(void)
{
    srand(time(NULL));

    std::atexit(exiting);

    game.GetState().Load(FILENAME);
    game.SwitchToScene<EditorScene>();

    CV::init(&screenWidth, &screenHeight, "T3 (Jaime Antonio Daniel Filho)");
    CV::run();
}
