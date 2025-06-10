/**
 * @file UI.h
 * @brief User interface management system for panels
 */

#pragma once

#include <vector>
#include <memory>

#include "Panel.h"

/**
 * @class UI
 * @brief Static class for managing and coordinating multiple UI panels
 */
class UI
{
public:
    static void RegisterPanel(std::shared_ptr<Panel> panel);

    static void Dispatch(const Event& event);
    static void Update();
    static void Draw();

private:
    static std::vector<std::shared_ptr<Panel>> s_Panels;
};