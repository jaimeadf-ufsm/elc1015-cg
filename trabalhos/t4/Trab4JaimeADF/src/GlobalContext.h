/**
 * @file GlobalContext.h
 * @brief Global context for sharing mesh data across the application
 */

#pragma once

#include "Mesh.h"

/**
 * @class GlobalContext
 * @brief Singleton-like class for managing global mesh data
 */
class GlobalContext
{
public:
    static Mesh& GetMesh();
    static void SetMesh(const Mesh& mesh);

private:
    static Mesh s_Mesh;

};