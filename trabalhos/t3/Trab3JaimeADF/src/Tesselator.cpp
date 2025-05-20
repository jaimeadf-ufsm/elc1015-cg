#include "Tesselator.h"
#include "Geometry.h"

#include <algorithm>

struct Node
{
    size_t VertexIndex;
    Vector2 VertexPoint;
    size_t PreviousIndex;
    size_t NextIndex;
};

size_t CreateNodes(std::vector<Node>& nodes, const std::vector<Vector2>& points)
{
    nodes.clear();
    nodes.reserve(points.size());

    float orientation = 0.0f;

    for (size_t i = 0; i < points.size(); i++)
    {
        Vector2 start = points[i];
        Vector2 end = points[(i + 1) % points.size()];

        orientation += (end.X - start.X) * (end.Y + start.Y);
    }

    if (orientation > 0)
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            Node node = {
                .VertexIndex = 0,
                .VertexPoint = points[points.size() - 1 - i],
                .PreviousIndex = i == 0 ? points.size() - 1 : i - 1,
                .NextIndex = (i + 1) % points.size()
            };

            nodes.emplace_back(node);
        }
    }
    else
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            Node node = {
                .VertexIndex = 0,
                .VertexPoint = points[i],
                .PreviousIndex = i == 0 ? points.size() - 1 : i - 1,
                .NextIndex = (i + 1) % points.size()
            };

            nodes.emplace_back(node);
        }
    }

    nodes.front().PreviousIndex = nodes.size() - 1;
    nodes.back().NextIndex = 0;

    return 0;
}

size_t PruneColinearNodes(std::vector<Node>& nodes, size_t startNodeIndex)
{
    bool isInitialNode = true;
    size_t currentNodeIndex = startNodeIndex;

    while (isInitialNode || currentNodeIndex != startNodeIndex)
    {
        isInitialNode = false;

        Node& currentNode = nodes[currentNodeIndex];
        Node& previousNode = nodes[currentNode.PreviousIndex];
        Node& nextNode = nodes[currentNode.NextIndex];

        Vector2 a = previousNode.VertexPoint;
        Vector2 b = currentNode.VertexPoint;
        Vector2 c = nextNode.VertexPoint;

        if (currentNode.PreviousIndex == currentNode.NextIndex)
        {
            break;
        }

        if (ComputeTriangleArea(a, b, c) == 0)
        {
            previousNode.NextIndex = currentNode.NextIndex;
            nextNode.PreviousIndex = currentNode.PreviousIndex;

            if (currentNodeIndex == startNodeIndex)
            {
                isInitialNode = true;
                startNodeIndex = currentNode.NextIndex;
            }
        }

        currentNodeIndex = currentNode.NextIndex;
    }

    return startNodeIndex;
}

void IndexNodeVertices(std::vector<Node>& nodes, Mesh &mesh, size_t startNodeIndex)
{
    size_t currentNodeIndex = startNodeIndex;

    do
    {
        Node& node = nodes[currentNodeIndex];

        node.VertexIndex = mesh.Vertices.size();
        mesh.Vertices.emplace_back(node.VertexPoint);

        currentNodeIndex = node.NextIndex;
    } while (currentNodeIndex != startNodeIndex);
}

bool IsEar(std::vector<Node>& nodes, size_t earNodeIndex)
{
    Node& earCurrentNode = nodes[earNodeIndex];
    Node& earPreviousNode = nodes[earCurrentNode.PreviousIndex];
    Node& earNextNode = nodes[earCurrentNode.NextIndex];

    Vector2 a = earPreviousNode.VertexPoint;
    Vector2 b = earCurrentNode.VertexPoint;
    Vector2 c = earNextNode.VertexPoint;

    if (ComputeTriangleArea(a, b, c) < 0)
    {
        return false;
    }

    size_t currentNodeIndex = earNextNode.NextIndex;

    while (currentNodeIndex != earCurrentNode.PreviousIndex)
    {
        Node& node = nodes[currentNodeIndex];

        if (IsPointInTriangle(node.VertexPoint, a, b, c))
        {
            return false;
        }

        currentNodeIndex = node.NextIndex;
    }

    return true;
}

void CutEar(std::vector<Node>& nodes, Mesh &mesh, size_t earNodeIndex)
{
    Node& currentNode = nodes[earNodeIndex];
    Node& previousNode = nodes[currentNode.PreviousIndex];
    Node& nextNode = nodes[currentNode.NextIndex];

    previousNode.NextIndex = currentNode.NextIndex;
    nextNode.PreviousIndex = currentNode.PreviousIndex;

    mesh.Triangles.emplace_back(previousNode.VertexIndex);
    mesh.Triangles.emplace_back(currentNode.VertexIndex);
    mesh.Triangles.emplace_back(nextNode.VertexIndex);
}

void Tesselator::Stroke(Mesh& mesh, const Path& path, float width)
{
    static std::vector<Vector2> points;

    mesh.Clear();

    float halfWidth = width / 2.0f;

    points = path.GetPoints();

    if (points.size() < 2)
    {
        return;
    }

    size_t s = path.IsClosed() ? points.size() + 1 : points.size();
    size_t j = path.IsClosed() ? points.size() + 1 : points.size() - 1;

    for (size_t i = 1; i < s; i++)
    {
        Vector2 startPoint = points[i - 1];
        Vector2 endPoint = points[i % points.size()];

        Vector2 direction = (endPoint - startPoint).Normalized();
        Vector2 extrude = Vector2(-direction.Y, direction.X) * halfWidth;

        size_t leftStartIndex = mesh.Vertices.size();
        mesh.Vertices.push_back(startPoint - extrude);

        size_t rightStartIndex = mesh.Vertices.size();
        mesh.Vertices.push_back(startPoint + extrude);

        size_t leftEndIndex = mesh.Vertices.size();
        mesh.Vertices.push_back(endPoint - extrude);

        size_t rightEndIndex = mesh.Vertices.size();
        mesh.Vertices.push_back(endPoint + extrude);

        mesh.Triangles.push_back(leftStartIndex);
        mesh.Triangles.push_back(leftEndIndex);
        mesh.Triangles.push_back(rightEndIndex);

        mesh.Triangles.push_back(leftStartIndex);
        mesh.Triangles.push_back(rightEndIndex);
        mesh.Triangles.push_back(rightStartIndex);
    }

    for (size_t i = 1; i < j; i++)
    {
        Vector2 previousPoint = points[(i - 1)];
        Vector2 currentPoint = points[i % points.size()];
        Vector2 nextPoint = points[(i + 1) % points.size()];

        Vector2 incomingDirection = (currentPoint - previousPoint).Normalized();
        Vector2 outgoingDirection = (nextPoint - currentPoint).Normalized();

        Vector2 incomingExtrude = Vector2(-incomingDirection.Y, incomingDirection.X) * halfWidth;
        Vector2 outgoingExtrude = Vector2(-outgoingDirection.Y, outgoingDirection.X) * halfWidth;

        float orientation = incomingDirection.Cross(outgoingDirection);

        if (orientation > 0)
        {
            size_t intersectionIndex = mesh.Vertices.size();
            mesh.Vertices.push_back(currentPoint);

            size_t previousLeftEndIndex = mesh.Vertices.size();
            mesh.Vertices.push_back(currentPoint - incomingExtrude);

            size_t nextLeftStartIndex = mesh.Vertices.size();
            mesh.Vertices.push_back(currentPoint - outgoingExtrude);

            mesh.Triangles.push_back(intersectionIndex);
            mesh.Triangles.push_back(previousLeftEndIndex);
            mesh.Triangles.push_back(nextLeftStartIndex);
        }
        else if (orientation < 0)
        {
            size_t intersectionIndex = mesh.Vertices.size();
            mesh.Vertices.push_back(currentPoint);

            size_t previousRightEndIndex = mesh.Vertices.size();
            mesh.Vertices.push_back(currentPoint + incomingExtrude);

            size_t nextRightStartIndex = mesh.Vertices.size();
            mesh.Vertices.push_back(currentPoint + outgoingExtrude);

            mesh.Triangles.push_back(intersectionIndex);
            mesh.Triangles.push_back(nextRightStartIndex);
            mesh.Triangles.push_back(previousRightEndIndex);
        }
    }
}

void Tesselator::Fill(Mesh& mesh, const Path& path)
{
    static std::vector<Node> nodes;

    nodes.clear();
    mesh.Clear();

    const std::vector<Vector2>& points = path.GetPoints();

    if (points.size() < 3)
    {
        return;
    }

    size_t startNodeIndex, currentNodeIndex, stopNodeIndex;

    startNodeIndex = CreateNodes(nodes, points); 
    startNodeIndex = PruneColinearNodes(nodes, startNodeIndex);

    IndexNodeVertices(nodes, mesh, startNodeIndex);

    currentNodeIndex = startNodeIndex;
    stopNodeIndex = startNodeIndex;

    while (true)
    {
        Node& currentNode = nodes[currentNodeIndex];
        Node& nextNode = nodes[currentNode.NextIndex];

        if (currentNode.PreviousIndex == currentNode.NextIndex)
        {
            break;
        }

        if (IsEar(nodes, currentNodeIndex))
        {
            CutEar(nodes, mesh, currentNodeIndex);

            currentNodeIndex = nextNode.NextIndex;
            stopNodeIndex = nextNode.NextIndex;

            continue;
        }

        currentNodeIndex = currentNode.NextIndex;

        if (currentNodeIndex == stopNodeIndex)
        {
            CutEar(nodes, mesh, currentNodeIndex);

            currentNodeIndex = nextNode.NextIndex;
            stopNodeIndex = nextNode.NextIndex;
        }
    }
}
