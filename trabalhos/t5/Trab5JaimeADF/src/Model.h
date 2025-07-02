#pragma once

#include <memory>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
    std::shared_ptr<Texture> Texture;
    std::vector<std::pair<float, std::shared_ptr<Mesh>>> Meshes;

    void RegisterLOD(float distance, std::shared_ptr<Mesh> mesh)
    {
        Meshes.push_back(std::make_pair(distance, mesh));
    }
};