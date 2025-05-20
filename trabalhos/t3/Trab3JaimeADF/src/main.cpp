// ## Requisitos implementados

// Todos os requisitos obrigatórios e extras especificados do trabalho foram implementados:

// ### Requisitos obrigatórios
// - Pista desenhada com curvas de bezier cúbicas.
// - Editor de pistas.
// - Movimentação do tanque com o teclado (A/D) e do canhão com o mouse.
// - Disparo de projéteis.
// - Placar de pontaução.
// - Colisão entre projéteis, alvos, pistas, etc...
// - Controle de FPS.
// - Barra de vida do tanque e danos na colisão.
// - Todos os movimentos são feitas conforme o FPS.

// ### Requisitos extras especificados

// - Alvos com diferentes lógicas. (até 1 pt)
// - Alvos com diferentes visuais e colisão apropriada (estrela, barril e quadrado). (até 2 pt)
// - Power-ups (cura, invincibilidade e disparos rápidos). (até 1 pt)
// - Efeitos de explosão bonitos. (até 2 pt)
// - Preenchimento colorido da pista com detalhes. (até 1 pt)
// - Diferentes níveis de dificuldade ao explodir todos os alvos. (até 1 pt)

// ### Requisitos extras não-especificados
// - Sons (Explosão e Tiro).
// - Armazenamento de pistas e de pontuações em um arquivo.
// - Histórico de pontuações.
// - Limitador de FPS.
// - Animações nos diferentes tipos de objetos.
// - Todos os desenhos são feitos utilizado triângulos com as primitivas do Canvas 2D (nada é um bitmap).
// - Triangularizador de polígonos convexos e concâvos.
// - Parser de comandos utilizados em SVG para desenhar qualquer elemento na tela.
// - Colisão genérica utilizando SAT (Separting Axis Theorem).

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
