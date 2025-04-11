#pragma once

#include "Project.h"
#include "Element.h"

namespace yap
{
    class ViewportSpace
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<Element> m_ViewportPreview;
    
    public:
        ViewportSpace(const std::shared_ptr<Project>& project, const std::shared_ptr<Element>& viewportPreview)
            : m_Project(project), m_ViewportPreview(viewportPreview)
        {
        }
        
        Vec2 ConvertScreenToCanvasCoordinates(const Vec2& screenPosition) const
        {
            return screenPosition - m_ViewportPreview->Position;
        }

        Vec2 ConvertCanvasToScreenCoordinates(const Vec2& canvasPosition) const
        {
            return canvasPosition + m_ViewportPreview->Position;
        }
    };
}