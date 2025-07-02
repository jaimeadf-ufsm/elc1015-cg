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

    Mesh CreateCube(float size)
    {
        Mesh mesh;

        float halfSize = size / 2.0f;

        mesh.Vertices = {
            { { -halfSize, -halfSize, -halfSize }, { 0, 0, -1 }, { 0, 0 } },
            { { halfSize, -halfSize, -halfSize }, { 0, 0, -1 }, { 1, 0 } },
            { { halfSize, halfSize, -halfSize }, { 0, 0, -1 }, { 1, 1 } },
            { { -halfSize, halfSize, -halfSize }, { 0, 0, -1 }, { 0, 1 } },
            { { -halfSize, -halfSize, halfSize }, { 0, 0, 1 }, { 0, 0 } },
            { { halfSize, -halfSize, halfSize }, { 0, 0, 1 }, { 1, 0 } },
            { { halfSize, halfSize, halfSize }, { 0, 0, 1 }, { 1, 1 } },
            { { -halfSize, halfSize, halfSize }, { 0, 0, 1 }, { 0, 1 } },
        };

        mesh.Indices = {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 6, 5,
            6, 4, 7,

            // Left face
            0, 3, 4,
            4, 7, 3,

            // Right face
            1, 5, 6,
            6, 2, 1,

            // Top face
            3, 2, 6,
            6, 7, 3,

            // Bottom face
            0, 4, 5,
            5, 1, 0
        };

        return mesh;
    }

    Mesh CreateSphere(float radius, int subdivisions)
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