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