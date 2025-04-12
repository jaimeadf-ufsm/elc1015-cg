#pragma once

#include "Project.h"
#include "Element.h"

/**
 * @file ViewportSpace.h
 * @brief Defines the ViewportSpace class, which provides functionality to convert coordinates
 * between screen space and canvas space within a viewport.
 */

namespace yap
{
    /**
     * @class ViewportSpace
     * @brief Manages the relationship between screen space and canvas space for a viewport.
     * 
     * This class provides methods to convert coordinates between screen space and canvas space,
     * using the position of a viewport preview element.
     */
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