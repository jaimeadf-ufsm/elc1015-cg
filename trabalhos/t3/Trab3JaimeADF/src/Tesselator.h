#pragma once

#include <vector>

#include "Vector2.h"
#include "Path.h"
#include "Mesh.h"

class Tesselator
{
public:
    static void Stroke(Mesh& mesh, const Path& path, float width);
    static void Fill(Mesh& mesh, const Path& path);
};