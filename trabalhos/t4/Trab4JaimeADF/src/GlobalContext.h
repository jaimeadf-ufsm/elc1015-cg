#pragma once

#include "Mesh.h"

class GlobalContext
{
public:
    static Mesh& GetMesh();
    static void SetMesh(const Mesh& mesh);

private:
    static Mesh s_Mesh;

};