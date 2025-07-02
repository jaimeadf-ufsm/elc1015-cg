#pragma once

#include <memory>
#include <algorithm>
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

class Model
{
public:
    std::shared_ptr<Texture> ModelTexture;
    std::shared_ptr<Material> ModelMaterial;
    std::vector<std::pair<float, std::shared_ptr<Mesh>>> Meshes;

    void RegisterLOD(float distance, std::shared_ptr<Mesh> mesh)
    {
        Meshes.push_back(std::make_pair(distance, mesh));
        // Sort by distance to ensure proper LOD selection
        std::sort(Meshes.begin(), Meshes.end(), 
                  [](const std::pair<float, std::shared_ptr<Mesh>>& a, 
                     const std::pair<float, std::shared_ptr<Mesh>>& b) { 
                      return a.first < b.first; 
                  });
    }

    void Render(float distance) const
    {
        if (Meshes.empty()) return;

        // Find appropriate LOD based on distance
        std::shared_ptr<Mesh> selectedMesh = Meshes.back().second; // Default to lowest detail
        
        for (const auto& lod : Meshes)
        {
            if (distance <= lod.first)
            {
                selectedMesh = lod.second;
                break;
            }
        }

        // Apply material and texture
        if (ModelMaterial)
        {
            ModelMaterial->Apply();
        }

        if (ModelTexture)
        {
            glEnable(GL_TEXTURE_2D);
            ModelTexture->Bind();
        }

        // Render the selected mesh
        if (selectedMesh)
        {
            selectedMesh->Render();
        }

        if (ModelTexture)
        {
            ModelTexture->Unbind();
            glDisable(GL_TEXTURE_2D);
        }
    }
};