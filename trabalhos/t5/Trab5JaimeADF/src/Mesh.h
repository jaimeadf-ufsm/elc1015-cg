#pragma once

#include <vector>
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
            for (int j = 0; j < 3; j++)
            {
                const Vertex& vertex = Vertices[Indices[i + j]];
                glNormal3f(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
                glTexCoord2f(vertex.TexCoord.X, vertex.TexCoord.Y);
                glVertex3f(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
            }
        }
        glEnd();
    }

    static Mesh CreateCube(float size)
    {
        Mesh mesh;

        float halfSize = size / 2.0f;

        mesh.Vertices = {
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

        mesh.Indices = {
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

        return mesh;
    }

    static Mesh CreateSphere(float radius, int subdivisions)
    {
        Mesh mesh;

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

                mesh.Vertices.push_back(vertex);
            }
        }

        for (int lat = 0; lat < latitudeCount; ++lat)
        {
            for (int lon = 0; lon < longitudeCount; ++lon)
            {
                int current = lat * (longitudeCount + 1) + lon;
                int next = current + longitudeCount + 1;

                mesh.Indices.push_back(current);
                mesh.Indices.push_back(next);
                mesh.Indices.push_back(current + 1);

                mesh.Indices.push_back(current + 1);
                mesh.Indices.push_back(next);
                mesh.Indices.push_back(next + 1);
            }
        }

        return mesh;
    }
};