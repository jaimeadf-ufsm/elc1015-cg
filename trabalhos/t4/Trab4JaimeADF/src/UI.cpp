#include "UI.h"

std::vector<std::shared_ptr<Panel>> UI::s_Panels;

void UI::RegisterPanel(std::shared_ptr<Panel> panel)
{
    s_Panels.push_back(panel);
}

void UI::Dispatch(const Event& event)
{
    for (const auto& panel : s_Panels)
    {
        panel->Process(event);
    }
}

void UI::Update()
{
    for (const auto& panel : s_Panels)
    {
        panel->Update();
    }
}

void UI::Draw()
{
    for (const auto& panel : s_Panels)
    {
        panel->Draw();
    }
}