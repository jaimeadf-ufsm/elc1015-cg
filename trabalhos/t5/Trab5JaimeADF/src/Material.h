/**
 * @file Material.h
 * @brief Material class for managing OpenGL material properties
 */

#pragma once

#include <GL/glut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Vector.h"

/**
 * @class Material
 * @brief Manages OpenGL material properties including ambient, diffuse, specular, and emission
 */
class Material
{
public:
    Vector4 Ambient;
    Vector4 Diffuse;
    Vector4 Specular;
    Vector4 Emission;
    float Shininess;

    Material(
        Vector4 ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f),
        Vector4 diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f),
        Vector4 specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector4 emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f),
        float shininess = 32.0f)
        : Ambient(ambient),
          Diffuse(diffuse),
          Specular(specular),
          Emission(emission),
          Shininess(shininess)
    {
    }

    void Apply() const
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission.ToArray());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
    }

    bool LoadFromMTL(const std::string &filename)
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "Error: Could not open MTL file: " << filename << std::endl;
            return false;
        }

        std::string line;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "Ka") // Ambient color
            {
                float r, g, b;
                iss >> r >> g >> b;
                Ambient = Vector4(r, g, b, 1.0f);
            }
            else if (prefix == "Kd") // Diffuse color
            {
                float r, g, b;
                iss >> r >> g >> b;
                Diffuse = Vector4(r, g, b, 1.0f);
            }
            else if (prefix == "Ks") // Specular color
            {
                float r, g, b;
                iss >> r >> g >> b;
                Specular = Vector4(r, g, b, 1.0f);
            }
            else if (prefix == "Ke") // Emission color
            {
                float r, g, b;
                iss >> r >> g >> b;
                Emission = Vector4(r, g, b, 1.0f);
            }
            else if (prefix == "Ns") // Shininess (specular exponent)
            {
                float shininess;
                iss >> shininess;
                Shininess = shininess;
            }
        }

        file.close();

        return true;
    }
};