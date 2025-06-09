#pragma once

#include <vector>
#include <memory>

#include "Panel.h"

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