#pragma once

#include "Modal.h"

namespace yap
{
    class ModalStack
    {
    private:
        std::vector<std::shared_ptr<Modal>> m_Modals;
    
    public:
        std::function<void(ModalStack&, const std::shared_ptr<Modal>&)> OnModal;

        void PushModal(const std::shared_ptr<Modal>& modal)
        {
            modal->OnClose = [this](Modal& modal)
            {
                PopModal();
            };

            m_Modals.push_back(modal);

            if (OnModal)
            {
                OnModal(*this, modal);
            }
        }

        void PopModal()
        {
            if (!m_Modals.empty())
            {
                m_Modals.pop_back();

                if (OnModal)
                {
                    if (!m_Modals.empty())
                    {
                        OnModal(*this, m_Modals.back());
                    }
                    else
                    {
                        OnModal(*this, nullptr);
                    }
                }
            }
        }
    };
}