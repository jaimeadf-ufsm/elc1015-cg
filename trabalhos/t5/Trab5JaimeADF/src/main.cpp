// Jaime Antonio Daniel Filho

// ## Requisitos implementados

// ### Requisitos básicos
// * [x] Camêra com animação suave, utilizando delta time entre frames.
// * [x] Asteroides modelados como formas simples (esferas)
// * [x] Modo de visualização wireframe e com preenchimento.
// * [x] Controle de FPS.

// ### Requisitos extras
// * [x] Nível de detalhe em função da distância: LOD (1 pt).
// * [x] Carregamento de texturas BMP (1 pt).
// * [x] Carregamento de modelos 3D usando o tipo OBJ (até 2pt).
// * [x] Modelagem de asteroides elaborados (até 4pt).

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>

#include "Mathematics.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Body.h"
#include "Time.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int ASTEROID_COUNT = 1000;
const float ASTEROID_INNER_RADIUS = 15.0f;
const float ASTEROID_OUTER_RADIUS = 100.0f;

std::shared_ptr<Model> sunModel;
std::vector<std::shared_ptr<Model>> asteroidModels;

Camera camera(Vector3(0, 0, 20));
Light light(GL_LIGHT0);

Body sun;
std::vector<Body> asteroids;

bool firstMouse = true;
bool keys[256] = {false};

int windowCenterX = SCREEN_WIDTH / 2;
int windowCenterY = SCREEN_HEIGHT / 2;

bool wireframeMode = false;

void InitializeOpenGL();
void InitializeModels();
void InitializeScene();
void Update();
void Render();
void HandleKeyboardDown(unsigned char key, int x, int y);
void HandleKeyboardUp(unsigned char key, int x, int y);
void HandleMouse(int x, int y);
void HandleMouseButton(int button, int state, int x, int y);
void Reshape(int width, int height);
void Idle();

void InitializeOpenGL()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

    srand(static_cast<unsigned int>(time(nullptr)));
}

void InitializeModels()
{
    auto sunTexture = std::make_shared<Texture>();
    sunTexture->LoadFromBMP("Trab5JaimeADF/assets/models/sun/sun.bmp");

    auto asteroid2bTexture = std::make_shared<Texture>();
    asteroid2bTexture->LoadFromBMP("Trab5JaimeADF/assets/models/asteroid_2b/asteroid_2b.bmp");

    auto asteroid2dTexture = std::make_shared<Texture>();
    asteroid2dTexture->LoadFromBMP("Trab5JaimeADF/assets/models/asteroid_2d/asteroid_2d.bmp");

    auto sunMaterial = std::make_shared<Material>(
        Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f),
        Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        0.0f
    );

    auto asteroid2bMaterial = std::make_shared<Material>();
    asteroid2bMaterial->LoadFromMTL("Trab5JaimeADF/assets/models/asteroid_2b/asteroid_2b.mtl");

    auto asteroid2dMaterial = std::make_shared<Material>();
    asteroid2dMaterial->LoadFromMTL("Trab5JaimeADF/assets/models/asteroid_2d/asteroid_2d.mtl");

    auto sunMeshHigh = std::make_shared<Mesh>();
    sunMeshHigh->LoadSphere(3.0f, 20);
    auto sunMeshMed = std::make_shared<Mesh>();
    sunMeshMed->LoadSphere(3.0f, 12);
    auto sunMeshLow = std::make_shared<Mesh>();
    sunMeshLow->LoadSphere(3.0f, 8);

    auto asteroid2bLod0 = std::make_shared<Mesh>();
    asteroid2bLod0->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2b/lod0.obj");
    auto asteroid2bLod1 = std::make_shared<Mesh>();
    asteroid2bLod1->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2b/lod1.obj");
    auto asteroid2bLod2 = std::make_shared<Mesh>();
    asteroid2bLod2->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2b/lod2.obj");
    auto asteroid2bLod3 = std::make_shared<Mesh>();
    asteroid2bLod3->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2b/lod3.obj");
    auto asteroid2bLod4 = std::make_shared<Mesh>();
    asteroid2bLod4->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2b/lod4.obj");
    auto asteroid2bLod5 = std::make_shared<Mesh>();
    asteroid2bLod5->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2b/lod5.obj");

    auto asteroid2dLod0 = std::make_shared<Mesh>();
    asteroid2dLod0->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2d/lod0.obj");
    auto asteroid2dLod1 = std::make_shared<Mesh>();
    asteroid2dLod1->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2d/lod1.obj");
    auto asteroid2dLod2 = std::make_shared<Mesh>();
    asteroid2dLod2->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2d/lod2.obj");
    auto asteroid2dLod3 = std::make_shared<Mesh>();
    asteroid2dLod3->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2d/lod3.obj");
    auto asteroid2dLod4 = std::make_shared<Mesh>();
    asteroid2dLod4->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2d/lod4.obj");
    auto asteroid2dLod5 = std::make_shared<Mesh>();
    asteroid2dLod5->LoadFromOBJ("Trab5JaimeADF/assets/models/asteroid_2d/lod5.obj");

    auto sphereMesh = std::make_shared<Mesh>();
    sphereMesh->LoadSphere(1.0f, 8);

    sunModel = std::make_shared<Model>();
    sunModel->Texture = sunTexture;
    sunModel->Material = sunMaterial;
    sunModel->RegisterLOD(10.0f, sunMeshHigh);
    sunModel->RegisterLOD(25.0f, sunMeshMed);
    sunModel->RegisterLOD(1000.0f, sunMeshLow);

    auto asteroid2bModel = std::make_shared<Model>();
    asteroid2bModel->Texture = asteroid2bTexture;
    asteroid2bModel->Material = asteroid2bMaterial;
    asteroid2bModel->RegisterLOD(5.0f, asteroid2bLod0);
    asteroid2bModel->RegisterLOD(10.0f, asteroid2bLod1);
    asteroid2bModel->RegisterLOD(20.0f, asteroid2bLod2);
    asteroid2bModel->RegisterLOD(35.0f, asteroid2bLod3);
    asteroid2bModel->RegisterLOD(50.0f, asteroid2bLod4);
    asteroid2bModel->RegisterLOD(75.0f, asteroid2bLod5);
    asteroid2bModel->RegisterLOD(5000.0f, sphereMesh);
    asteroidModels.push_back(asteroid2bModel);

    auto asteroid2dModel = std::make_shared<Model>();
    asteroid2dModel->Texture = asteroid2dTexture;
    asteroid2dModel->Material = asteroid2dMaterial;
    asteroid2dModel->RegisterLOD(5.0f, asteroid2dLod0);
    asteroid2dModel->RegisterLOD(10.0f, asteroid2dLod1);
    asteroid2dModel->RegisterLOD(20.0f, asteroid2dLod2);
    asteroid2dModel->RegisterLOD(35.0f, asteroid2dLod3);
    asteroid2dModel->RegisterLOD(50.0f, asteroid2dLod4);
    asteroid2dModel->RegisterLOD(75.0f, asteroid2dLod5);
    asteroid2dModel->RegisterLOD(5000.0f, sphereMesh);
    asteroidModels.push_back(asteroid2dModel);
}

void InitializeScene()
{
    light.Position = Vector4(0, 0, 0, 1.0f);

    light.Ambient = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
    light.Diffuse = Vector4(1.0f, 1.0f, 0.9f, 1.0f);
    light.Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    sun.Model = sunModel;
    sun.Transform.Position = Vector3(0, 0, 0);

    for (int i = 0; i < ASTEROID_COUNT; i++)
    {
        int modelIndex = rand() % asteroidModels.size();

        asteroids.emplace_back();

        Body &asteroid = asteroids.back();
        asteroid.Model = asteroidModels[modelIndex];
        asteroid.Transform.Position = RandomPointOnShell(ASTEROID_INNER_RADIUS, ASTEROID_OUTER_RADIUS);
        asteroid.Transform.Scale = Vector3(0.1f + (rand() / (float)RAND_MAX) * 2.0f);
        asteroid.AngularVelocity = Vector3(
            (rand() / (float)RAND_MAX - 0.5f) * 50.0f,
            (rand() / (float)RAND_MAX - 0.5f) * 50.0f,
            (rand() / (float)RAND_MAX - 0.5f) * 50.0f
        );
    }
}

void Update()
{
    Time::Update();

    if (keys['w'] || keys['W'])
        camera.ProcessKeyboard('w');
    if (keys['s'] || keys['S'])
        camera.ProcessKeyboard('s');
    if (keys['a'] || keys['A'])
        camera.ProcessKeyboard('a');
    if (keys['d'] || keys['D'])
        camera.ProcessKeyboard('d');

    for (auto &asteroid : asteroids)
    {
        asteroid.Update();
    }
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.Apply();

    light.Enable();

    sun.Render(camera.Position);

    for (const auto &asteroid : asteroids)
    {
        asteroid.Render(camera.Position);
    }

    glutSwapBuffers();
}

void HandleKeyboardDown(unsigned char key, int x, int y)
{
    keys[key] = true;

    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'x':
    case 'X':
        wireframeMode = !wireframeMode;
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }
}

void HandleKeyboardUp(unsigned char key, int x, int y)
{
    keys[key] = false;
}

void HandleMouse(int x, int y)
{
    if (firstMouse)
    {
        firstMouse = false;
        glutWarpPointer(windowCenterX, windowCenterY);
        return;
    }

    float xOffset = x - windowCenterX;
    float yOffset = windowCenterY - y;

    camera.ProcessMouseMovement(xOffset, yOffset);
    
    glutWarpPointer(windowCenterX, windowCenterY);
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    windowCenterX = width / 2;
    windowCenterY = height / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void Idle()
{
    Update();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("T5 - Jaime Antonio Daniel Filho");

    InitializeOpenGL();

    InitializeModels();
    InitializeScene();

    glutDisplayFunc(Render);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(HandleKeyboardDown);
    glutKeyboardUpFunc(HandleKeyboardUp);
    glutPassiveMotionFunc(HandleMouse);
    glutMotionFunc(HandleMouse);
    glutIdleFunc(Idle);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();

    return 0;
}
