#pragma once

#include <vector>

#include "Color.h"
#include "Vector2.h"
#include "Path.h"
#include "Mesh.h"

#include "DrawingCommand.h"

class DrawingContext
{
public:
    void Color(ColorRGB color);

    void FillRectangle(Vector2 position, Vector2 size);

    void BeginPolygon();

    void Vertex(Vector2 position);

    void StrokePolygon();
    void FillPolygon();

    void Text(Vector2 position, const std::string& text);

    const std::vector<DrawingCommand>& GetCommands() const;

    void ClearCommands();

private:
    std::vector<DrawingCommand> m_Commands;
};