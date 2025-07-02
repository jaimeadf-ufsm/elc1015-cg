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

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Asteroid.h"
#include "Time.h"

// Constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int NUM_ASTEROIDS = 100;
const float ASTEROID_INNER_RADIUS = 15.0f;
const float ASTEROID_OUTER_RADIUS = 50.0f;

// Global objects
Camera camera(Vector3(0, 0, 20));
Light sunLight(GL_LIGHT0);
std::vector<Asteroid> asteroids;
std::vector<std::shared_ptr<Model>> asteroidModels;

// Sun object
std::shared_ptr<Model> sunModel;

// Input state
bool keys[256] = { false };
bool wireframeMode = false;
int lastMouseX = SCREEN_WIDTH / 2;
int lastMouseY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

// Function prototypes
void InitializeOpenGL();
void InitializeModels();
void CreateAsteroids();
void Update();
void Render();
void RenderSun();
void RenderAsteroids();
void HandleKeyboard(unsigned char key, int x, int y);
void HandleKeyboardUp(unsigned char key, int x, int y);
void HandleMouse(int x, int y);
void HandleMouseButton(int button, int state, int x, int y);
void Reshape(int width, int height);
void Idle();

std::shared_ptr<Material> CreateMaterial(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shininess)
{
    auto material = std::make_shared<Material>();
    material->Ambient = ambient;
    material->Diffuse = diffuse;
    material->Specular = specular;
    material->Emission = Vector4(0, 0, 0, 1);
    material->Shininess = shininess;
    return material;
}

void InitializeOpenGL()
{
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    
    // Set clear color to black (space)
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    
    // Set up lighting properties (position will be set in render loop)
    sunLight.Ambient = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
    sunLight.Diffuse = Vector4(1.0f, 1.0f, 0.9f, 1.0f);
    sunLight.Specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
}

void InitializeModels()
{
    // Create textures
    auto sunTexture = std::make_shared<Texture>();
    sunTexture->CreateCheckerboard(256, 256, Vector3(1.0f, 0.8f, 0.2f), Vector3(1.0f, 0.5f, 0.0f), 16);
    
    auto asteroidTexture1 = std::make_shared<Texture>();
    asteroidTexture1->CreateCheckerboard(128, 128, Vector3(0.6f, 0.6f, 0.6f), Vector3(0.4f, 0.4f, 0.4f), 8);
    
    auto asteroidTexture2 = std::make_shared<Texture>();
    asteroidTexture2->CreateCheckerboard(128, 128, Vector3(0.5f, 0.3f, 0.2f), Vector3(0.3f, 0.2f, 0.1f), 6);
    
    // Create materials
    auto sunMaterial = CreateMaterial(
        Vector4(1.0f, 0.8f, 0.3f, 1.0f),  // Bright ambient
        Vector4(1.0f, 0.8f, 0.3f, 1.0f),  // Bright diffuse
        Vector4(0.0f, 0.0f, 0.0f, 1.0f),  // No specular (suns don't have shiny spots)
        0.0f                              // No shininess
    );
    // Make the sun emit light (glow)
    sunMaterial->Emission = Vector4(1.0f, 0.8f, 0.3f, 1.0f);
    
    auto asteroidMaterial1 = CreateMaterial(
        Vector4(0.3f, 0.3f, 0.3f, 1.0f),
        Vector4(0.6f, 0.6f, 0.6f, 1.0f),
        Vector4(0.2f, 0.2f, 0.2f, 1.0f),
        10.0f
    );
    
    auto asteroidMaterial2 = CreateMaterial(
        Vector4(0.2f, 0.1f, 0.05f, 1.0f),
        Vector4(0.4f, 0.2f, 0.1f, 1.0f),
        Vector4(0.1f, 0.1f, 0.1f, 1.0f),
        5.0f
    );
    
    // Create meshes
    auto sunMeshHigh = std::make_shared<Mesh>(Mesh::CreateSphere(3.0f, 20));
    auto sunMeshMed = std::make_shared<Mesh>(Mesh::CreateSphere(3.0f, 12));
    auto sunMeshLow = std::make_shared<Mesh>(Mesh::CreateSphere(3.0f, 8));
    
    auto sphereHighDetail = std::make_shared<Mesh>(Mesh::CreateSphere(1.0f, 12));
    auto sphereMedDetail = std::make_shared<Mesh>(Mesh::CreateSphere(1.0f, 8));
    auto sphereLowDetail = std::make_shared<Mesh>(Mesh::CreateSphere(1.0f, 6));
    
    auto cubeHighDetail = std::make_shared<Mesh>(Mesh::CreateCube(1.8f));
    auto cubeMedDetail = std::make_shared<Mesh>(Mesh::CreateCube(1.8f));
    auto cubeLowDetail = std::make_shared<Mesh>(Mesh::CreateCube(1.8f));
    
    // Create sun model
    sunModel = std::make_shared<Model>();
    sunModel->ModelTexture = sunTexture;
    sunModel->ModelMaterial = sunMaterial;
    sunModel->RegisterLOD(10.0f, sunMeshHigh);
    sunModel->RegisterLOD(25.0f, sunMeshMed);
    sunModel->RegisterLOD(1000.0f, sunMeshLow);
    
    // Create asteroid models
    // Sphere asteroid model
    auto sphereModel = std::make_shared<Model>();
    sphereModel->ModelTexture = asteroidTexture1;
    sphereModel->ModelMaterial = asteroidMaterial1;
    sphereModel->RegisterLOD(5.0f, sphereHighDetail);
    sphereModel->RegisterLOD(15.0f, sphereMedDetail);
    sphereModel->RegisterLOD(1000.0f, sphereLowDetail);
    asteroidModels.push_back(sphereModel);
    
    // Cube asteroid model
    auto cubeModel = std::make_shared<Model>();
    cubeModel->ModelTexture = asteroidTexture2;
    cubeModel->ModelMaterial = asteroidMaterial2;
    cubeModel->RegisterLOD(5.0f, cubeHighDetail);
    cubeModel->RegisterLOD(15.0f, cubeMedDetail);
    cubeModel->RegisterLOD(1000.0f, cubeLowDetail);
    asteroidModels.push_back(cubeModel);
}

void CreateAsteroids()
{
    for (int i = 0; i < NUM_ASTEROIDS; i++)
    {
        // Generate random position in spherical coordinates
        float theta = (rand() / (float)RAND_MAX) * 2.0f * M_PI;
        float phi = (rand() / (float)RAND_MAX) * M_PI;
        float radius = ASTEROID_INNER_RADIUS + (rand() / (float)RAND_MAX) * (ASTEROID_OUTER_RADIUS - ASTEROID_INNER_RADIUS);
        
        Vector3 position;
        position.X = radius * sinf(phi) * cosf(theta);
        position.Y = radius * cosf(phi);
        position.Z = radius * sinf(phi) * sinf(theta);
        
        // Select random model
        int modelIndex = rand() % asteroidModels.size();
        
        asteroids.emplace_back(position, asteroidModels[modelIndex]);
    }
}

void Update()
{
    Time::Update();
    
    // Process continuous keyboard input
    if (keys['w'] || keys['W']) camera.ProcessKeyboard('w', Time::DeltaTime);
    if (keys['s'] || keys['S']) camera.ProcessKeyboard('s', Time::DeltaTime);
    if (keys['a'] || keys['A']) camera.ProcessKeyboard('a', Time::DeltaTime);
    if (keys['d'] || keys['D']) camera.ProcessKeyboard('d', Time::DeltaTime);
    if (keys['q'] || keys['Q']) camera.ProcessKeyboard('q', Time::DeltaTime);
    if (keys['e'] || keys['E']) camera.ProcessKeyboard('e', Time::DeltaTime);
    
    // Update asteroids
    for (auto& asteroid : asteroids)
    {
        asteroid.Update(Time::DeltaTime);
    }
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set camera
    camera.LookAt();
    
    // Set light position after camera transformation
    // This ensures the light is positioned at the sun's location in world space
    sunLight.SetPosition(Vector3(0, 0, 0));
    sunLight.Apply();
    
    // Render sun
    RenderSun();
    
    // Render asteroids
    RenderAsteroids();
    
    glutSwapBuffers();
}

void RenderSun()
{
    glPushMatrix();
    
    // Disable lighting for the sun so it appears to glow
   //  glDisable(GL_LIGHTING);
    
    // Set a bright color for the sun
    glColor3f(1.0f, 0.8f, 0.3f);
    
    // Calculate distance from camera to sun for LOD
    Vector3 distance = Vector3(0, 0, 0) - camera.Position;
    float distanceLength = distance.Length();
    
    sunModel->Render(distanceLength);
    
    // Re-enable lighting for other objects
   //  glEnable(GL_LIGHTING);
    
    glPopMatrix();
}

void RenderAsteroids()
{
    for (const auto& asteroid : asteroids)
    {
        asteroid.Render(camera.Position);
    }
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
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }
    
    float xOffset = x - lastMouseX;
    float yOffset = lastMouseY - y; // Reversed since y-coordinates go from bottom to top
    
    lastMouseX = x;
    lastMouseY = y;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}

void HandleMouseButton(int button, int state, int x, int y)
{
    // Handle mouse button events if needed
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    
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

int main(int argc, char** argv)
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
