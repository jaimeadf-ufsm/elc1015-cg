#pragma once

#include <memory>
#include <algorithm>
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

class Model
{
public:
    std::shared_ptr<::Texture> Texture;
    std::shared_ptr<::Material> Material;
    std::vector<std::pair<float, std::shared_ptr<Mesh>>> Meshes;

    void RegisterLOD(float distance, std::shared_ptr<Mesh> mesh)
    {
        Meshes.push_back(std::make_pair(distance, mesh));

        std::sort(
            Meshes.begin(),
            Meshes.end(),
            [](const std::pair<float, std::shared_ptr<Mesh>> &a, const std::pair<float, std::shared_ptr<Mesh>> &b)
            {
                return a.first < b.first;
            }
        );
    }

    void Render(float distance) const
    {
        if (Meshes.empty())
            return;

        std::shared_ptr<Mesh> mesh = FindAppropriateMesh(distance);

        if (Material)
        {
            Material->Apply();
        }

        if (Texture)
        {
            glEnable(GL_TEXTURE_2D);
            Texture->Bind();
        }

        if (mesh)
        {
            mesh->Render();
        }

        if (Texture)
        {
            Texture->Unbind();
            glDisable(GL_TEXTURE_2D);
        }
    }

    std::shared_ptr<Mesh> FindAppropriateMesh(float distance) const
    {
        for (const auto &lod : Meshes)
        {
            if (distance <= lod.first)
            {
                return lod.second;
            }
        }

        return Meshes.back().second;
    }
};