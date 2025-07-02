#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glut.h>

#include "Vector.h"

struct Vertex
{
    Vector3 Position;
    Vector3 Normal;
    Vector2 TexCoord;
};

class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<int> Indices;

    void Render() const
    {
        glBegin(GL_TRIANGLES);

        for (size_t i = 0; i < Indices.size(); i += 3)
        {
            for (size_t j = 0; j < 3; j++)
            {
                const Vertex& vertex = Vertices[Indices[i + j]];

                glNormal3f(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
                glTexCoord2f(vertex.TexCoord.X, vertex.TexCoord.Y);
                glVertex3f(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
            }
        }

        glEnd();
    }

    void LoadCube(float size)
    {
        float halfSize = size / 2.0f;

        Vertices = {
            // Front face
            { { -halfSize, -halfSize, halfSize }, { 0, 0, 1 }, { 0, 0 } },
            { { halfSize, -halfSize, halfSize }, { 0, 0, 1 }, { 1, 0 } },
            { { halfSize, halfSize, halfSize }, { 0, 0, 1 }, { 1, 1 } },
            { { -halfSize, halfSize, halfSize }, { 0, 0, 1 }, { 0, 1 } },

            // Back face
            { { halfSize, -halfSize, -halfSize }, { 0, 0, -1 }, { 0, 0 } },
            { { -halfSize, -halfSize, -halfSize }, { 0, 0, -1 }, { 1, 0 } },
            { { -halfSize, halfSize, -halfSize }, { 0, 0, -1 }, { 1, 1 } },
            { { halfSize, halfSize, -halfSize }, { 0, 0, -1 }, { 0, 1 } },

            // Left face
            { { -halfSize, -halfSize, -halfSize }, { -1, 0, 0 }, { 0, 0 } },
            { { -halfSize, -halfSize, halfSize }, { -1, 0, 0 }, { 1, 0 } },
            { { -halfSize, halfSize, halfSize }, { -1, 0, 0 }, { 1, 1 } },
            { { -halfSize, halfSize, -halfSize }, { -1, 0, 0 }, { 0, 1 } },

            // Right face
            { { halfSize, -halfSize, halfSize }, { 1, 0, 0 }, { 0, 0 } },
            { { halfSize, -halfSize, -halfSize }, { 1, 0, 0 }, { 1, 0 } },
            { { halfSize, halfSize, -halfSize }, { 1, 0, 0 }, { 1, 1 } },
            { { halfSize, halfSize, halfSize }, { 1, 0, 0 }, { 0, 1 } },

            // Top face
            { { -halfSize, halfSize, halfSize }, { 0, 1, 0 }, { 0, 0 } },
            { { halfSize, halfSize, halfSize }, { 0, 1, 0 }, { 1, 0 } },
            { { halfSize, halfSize, -halfSize }, { 0, 1, 0 }, { 1, 1 } },
            { { -halfSize, halfSize, -halfSize }, { 0, 1, 0 }, { 0, 1 } },

            // Bottom face
            { { -halfSize, -halfSize, -halfSize }, { 0, -1, 0 }, { 0, 0 } },
            { { halfSize, -halfSize, -halfSize }, { 0, -1, 0 }, { 1, 0 } },
            { { halfSize, -halfSize, halfSize }, { 0, -1, 0 }, { 1, 1 } },
            { { -halfSize, -halfSize, halfSize }, { 0, -1, 0 }, { 0, 1 } }
        };

        Indices = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            12, 13, 14, 14, 15, 12,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 21, 22, 22, 23, 20
        };
    }

    void LoadSphere(float radius, int subdivisions)
    {
        int latitudeCount = subdivisions;
        int longitudeCount = subdivisions * 2;

        for (int lat = 0; lat <= latitudeCount; ++lat)
        {
            float v = (float)lat / latitudeCount;
            float theta = v * M_PI;

            for (int lon = 0; lon <= longitudeCount; ++lon)
            {
                float u = (float)lon / longitudeCount;
                float phi = u * 2.0f * M_PI;

                float x = sinf(theta) * cosf(phi);
                float y = cosf(theta);
                float z = sinf(theta) * sinf(phi);

                Vertex vertex;
                vertex.Position = { radius * x, radius * y, radius * z };
                vertex.Normal = { x, y, z };
                vertex.TexCoord = { u, 1.0f - v };

                Vertices.emplace_back(vertex);
            }
        }

        for (int lat = 0; lat < latitudeCount; ++lat)
        {
            for (int lon = 0; lon < longitudeCount; ++lon)
            {
                int current = lat * (longitudeCount + 1) + lon;
                int next = current + longitudeCount + 1;

                Indices.push_back(current);
                Indices.push_back(next);
                Indices.push_back(current + 1);

                Indices.push_back(current + 1);
                Indices.push_back(next);
                Indices.push_back(next + 1);
            }
        }
    }

    void LoadFromOBJ(const std::string& filename)
    {
        std::vector<Vector3> positions;
        std::vector<Vector2> texCoords;
        std::vector<Vector3> normals;
        
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "Error: Could not open OBJ file: " << filename << std::endl;
            return;
        }
        
        std::string line;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            
            if (prefix == "v")
            {
                Vector3 pos;
                iss >> pos.X >> pos.Y >> pos.Z;
                positions.push_back(pos);
            }
            else if (prefix == "vt")
            {
                Vector2 texCoord;
                iss >> texCoord.X >> texCoord.Y;
                texCoords.push_back(texCoord);
            }
            else if (prefix == "vn")
            {
                Vector3 normal;
                iss >> normal.X >> normal.Y >> normal.Z;
                normals.push_back(normal);
            }
            else if (prefix == "f")
            {
                std::string vertexStr;
                std::vector<int> faceVertices;
                
                while (iss >> vertexStr)
                {
                    std::istringstream vertexStream(vertexStr);
                    std::string token;
                    
                    std::vector<int> indices(3, -1);
                    int index = 0;
                    
                    while (std::getline(vertexStream, token, '/') && index < 3)
                    {
                        if (!token.empty())
                        {
                            indices[index] = std::stoi(token) - 1;
                        }

                        index++;
                    }
                    
                    Vertex vertex;
                    
                    if (indices[0] >= 0 && indices[0] < static_cast<int>(positions.size()))
                    {
                        vertex.Position = positions[indices[0]];
                    }
                    
                    if (indices[1] >= 0 && indices[1] < static_cast<int>(texCoords.size()))
                    {
                        vertex.TexCoord = texCoords[indices[1]];
                    }
                    else
                    {
                        vertex.TexCoord = Vector2(0.0f, 0.0f);
                    }
                    
                    if (indices[2] >= 0 && indices[2] < static_cast<int>(normals.size()))
                    {
                        vertex.Normal = normals[indices[2]];
                    }
                    else
                    {
                        vertex.Normal = Vector3(0.0f, 0.0f, 0.0f);
                    }
 
                    Vertices.push_back(vertex);
                    int vertexIndex = static_cast<int>(Vertices.size() - 1);
                    
                    faceVertices.push_back(vertexIndex);
                }
                
                if (faceVertices.size() >= 3)
                {
                    Indices.push_back(faceVertices[0]);
                    Indices.push_back(faceVertices[1]);
                    Indices.push_back(faceVertices[2]);
                    
                    if (faceVertices.size() == 4)
                    {
                        Indices.push_back(faceVertices[0]);
                        Indices.push_back(faceVertices[2]);
                        Indices.push_back(faceVertices[3]);
                    }
                }
            }
        }
        
        file.close();
    }

    void Clear()
    {
        Vertices.clear();
        Indices.clear();
    }
};