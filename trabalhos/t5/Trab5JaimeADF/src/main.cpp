/**
 * Space Asteroid Demo
 * OpenGL 2.1 with FreeGLUT
 * Features: Free camera, LOD system, procedural textures, asteroid field
 */

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
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
const int NUM_ASTEROIDS = 1000;
const float ASTEROID_INNER_RADIUS = 15.0f;
const float ASTEROID_OUTER_RADIUS = 100.0f;

std::shared_ptr<Model> sunModel;
std::vector<std::shared_ptr<Model>> asteroidModels;

Camera camera(Vector3(0, 0, 20));
Light light(GL_LIGHT0);

Body sun;
std::vector<Body> asteroids;

bool keys[256] = {false};
bool wireframeMode = false;
int windowCenterX = SCREEN_WIDTH / 2;
int windowCenterY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

// Function prototypes
void InitializeOpenGL();
void InitializeModels();
void CreateAsteroids();
void Update();
void Render();
void HandleKeyboard(unsigned char key, int x, int y);
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
        Vector4(1.0f, 0.8f, 0.3f, 1.0f), // Bright ambient
        Vector4(1.0f, 0.8f, 0.3f, 1.0f), // Bright diffuse
        Vector4(0.0f, 0.0f, 0.0f, 1.0f), // No specular (suns don't have shiny spots)
        Vector4(1.0f, 0.8f, 0.3f, 1.0f), // Emission (glow)
        0.0f                             // No shininess
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

    // Fallback procedural sphere mesh
    auto sphereMesh = std::make_shared<Mesh>();
    sphereMesh->LoadSphere(1.0f, 8);

    // Create sun model
    sunModel = std::make_shared<Model>();
    sunModel->Texture = sunTexture;
    sunModel->Material = sunMaterial;
    sunModel->RegisterLOD(10.0f, sunMeshHigh);
    sunModel->RegisterLOD(25.0f, sunMeshMed);
    sunModel->RegisterLOD(1000.0f, sunMeshLow);

    // Create asteroid models
    auto asteroid2bModel = std::make_shared<Model>();
    asteroid2bModel->Texture = asteroid2bTexture;
    asteroid2bModel->Material = asteroid2bMaterial;
    asteroid2bModel->RegisterLOD(5.0f, asteroid2bLod0);   // Closest - highest detail
    asteroid2bModel->RegisterLOD(10.0f, asteroid2bLod1);  // Close
    asteroid2bModel->RegisterLOD(20.0f, asteroid2bLod2);  // Medium
    asteroid2bModel->RegisterLOD(35.0f, asteroid2bLod3);  // Far
    asteroid2bModel->RegisterLOD(50.0f, asteroid2bLod4);  // Very far
    asteroid2bModel->RegisterLOD(75.0f, asteroid2bLod5);  // Farthest
    asteroid2bModel->RegisterLOD(5000.0f, sphereMesh);     // Fallback sphere for extreme distances
    asteroidModels.push_back(asteroid2bModel);

    auto asteroid2dModel = std::make_shared<Model>();
    asteroid2dModel->Texture = asteroid2dTexture;
    asteroid2dModel->Material = asteroid2dMaterial;
    asteroid2dModel->RegisterLOD(5.0f, asteroid2dLod0);   // Closest - highest detail
    asteroid2dModel->RegisterLOD(10.0f, asteroid2dLod1);  // Close
    asteroid2dModel->RegisterLOD(20.0f, asteroid2dLod2);  // Medium
    asteroid2dModel->RegisterLOD(35.0f, asteroid2dLod3);  // Far
    asteroid2dModel->RegisterLOD(50.0f, asteroid2dLod4);  // Very far
    asteroid2dModel->RegisterLOD(75.0f, asteroid2dLod5);  // Farthest
    asteroid2dModel->RegisterLOD(5000.0f, sphereMesh);     // Fallback sphere for extreme distances
    asteroidModels.push_back(asteroid2dModel);
}

void CreateAsteroids()
{
    light.Position = Vector4(0, 0, 0, 1.0f);

    light.Ambient = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
    light.Diffuse = Vector4(1.0f, 1.0f, 0.9f, 1.0f);
    light.Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    sun.Model = sunModel;
    sun.Transform.Position = Vector3(0, 0, 0);

    for (int i = 0; i < NUM_ASTEROIDS; i++)
    {
        float theta = (rand() / (float)RAND_MAX) * 2.0f * M_PI;
        float phi = (rand() / (float)RAND_MAX) * M_PI;

        float r_inner_cubed = ASTEROID_INNER_RADIUS * ASTEROID_INNER_RADIUS * ASTEROID_INNER_RADIUS;
        float r_outer_cubed = ASTEROID_OUTER_RADIUS * ASTEROID_OUTER_RADIUS * ASTEROID_OUTER_RADIUS;
        float random_volume = (rand() / (float)RAND_MAX);
        float radius_cubed = r_inner_cubed + random_volume * (r_outer_cubed - r_inner_cubed);
        float radius = powf(radius_cubed, 1.0f / 3.0f);

        Vector3 position;
        position.X = radius * sinf(phi) * cosf(theta);
        position.Y = radius * cosf(phi);
        position.Z = radius * sinf(phi) * sinf(theta);

        int modelIndex = rand() % asteroidModels.size();

        asteroids.emplace_back();

        Body &asteroid = asteroids.back();
        asteroid.Model = asteroidModels[modelIndex];
        asteroid.Transform.Position = position;
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

    // Process continuous keyboard input
    if (keys['w'] || keys['W'])
        camera.ProcessKeyboard('w');
    if (keys['s'] || keys['S'])
        camera.ProcessKeyboard('s');
    if (keys['a'] || keys['A'])
        camera.ProcessKeyboard('a');
    if (keys['d'] || keys['D'])
        camera.ProcessKeyboard('d');
    if (keys['q'] || keys['Q'])
        camera.ProcessKeyboard('q');
    if (keys['e'] || keys['E'])
        camera.ProcessKeyboard('e');

    // Update asteroids
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

void HandleKeyboard(unsigned char key, int x, int y)
{
    keys[key] = true;

    switch (key)
    {
    case 27: // ESC
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

void HandleMouseButton(int button, int state, int x, int y)
{
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
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Space Asteroid Demo - Use WASD+QE to move, Mouse to look, X for wireframe");

    // Initialize OpenGL
    InitializeOpenGL();

    // Initialize models and scene
    InitializeModels();
    CreateAsteroids();

    // Set up callbacks
    glutDisplayFunc(Render);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(HandleKeyboard);
    glutKeyboardUpFunc(HandleKeyboardUp);
    glutPassiveMotionFunc(HandleMouse);
    glutMotionFunc(HandleMouse);
    glutMouseFunc(HandleMouseButton);
    glutIdleFunc(Idle);

    // Hide cursor and warp to center
    glutSetCursor(GLUT_CURSOR_NONE);

    std::cout << "Space Asteroid Demo Controls:" << std::endl;
    std::cout << "WASD - Move camera horizontally" << std::endl;
    std::cout << "QE - Move camera up/down" << std::endl;
    std::cout << "Mouse - Look around" << std::endl;
    std::cout << "X - Toggle wireframe mode" << std::endl;
    std::cout << "ESC - Exit" << std::endl;

    // Start main loop
    glutMainLoop();

    return 0;
}
