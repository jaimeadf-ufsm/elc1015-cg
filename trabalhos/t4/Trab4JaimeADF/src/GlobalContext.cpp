#include "GlobalContext.h"

Mesh GlobalContext::s_Mesh;

Mesh& GlobalContext::GetMesh()
{
    return s_Mesh;
}

void GlobalContext::SetMesh(const Mesh& mesh)
{
    s_Mesh = mesh;
}