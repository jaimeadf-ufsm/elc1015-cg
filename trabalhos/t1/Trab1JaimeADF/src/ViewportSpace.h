#pragma once

#include "Project.h"
#include "Element.h"

/**
 * @file ViewportSpace.h
 * @brief Defines the ViewportSpace class, which provides functionality to convert coordinates
 *        between screen space and canvas space within a viewport.
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
        std::shared_ptr<Project> m_Project; ///< The project associated with this viewport space.
        std::shared_ptr<Element> m_ViewportPreview; ///< The viewport preview element used for coordinate transformations.
    
    public:
        /**
         * @brief Constructs a ViewportSpace object.
         * 
         * @param project A shared pointer to the associated project.
         * @param viewportPreview A shared pointer to the viewport preview element.
         */
        ViewportSpace(const std::shared_ptr<Project>& project, const std::shared_ptr<Element>& viewportPreview)
            : m_Project(project), m_ViewportPreview(viewportPreview)
        {
        }
        
        /**
         * @brief Converts screen space coordinates to canvas space coordinates.
         * 
         * @param screenPosition The position in screen space.
         * @return The corresponding position in canvas space.
         */
        Vec2 ConvertScreenToCanvasCoordinates(const Vec2& screenPosition) const
        {
            return screenPosition - m_ViewportPreview->Position;
        }

        /**
         * @brief Converts canvas space coordinates to screen space coordinates.
         * 
         * @param canvasPosition The position in canvas space.
         * @return The corresponding position in screen space.
         */
        Vec2 ConvertCanvasToScreenCoordinates(const Vec2& canvasPosition) const
        {
            return canvasPosition + m_ViewportPreview->Position;
        }
    };
}