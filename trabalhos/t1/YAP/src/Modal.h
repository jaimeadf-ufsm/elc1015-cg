#pragma once

#include "Box.h"

namespace yap
{
    class Modal : public Box
    {
    public:
        std::function<void(Modal &modal)> OnClose;

        Modal()
        {
            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(512), AxisSizingRule::Fixed(512))
                    .WithBackground(BoxBackground::Solid(ColorRGBA(0, 0, 0)))
            );

            OnMousePress = [this](Element& element)
            {
                Close();
            };
        }

        void Close()
        {
            if (OnClose)
            {
                OnClose(*this);
            }
        }
    
    protected:
        std::shared_ptr<Box> CreateBody()
        {
            return std::make_shared<Box>();
        }
    };
}