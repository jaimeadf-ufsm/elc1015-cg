#pragma once

#include "Modal.h"

/**
 * @file ModalStack.h
 * @brief Defines the ModalStack class, which manages a stack of modals and handles their lifecycle.
 */

namespace yap
{
    /**
     * @class ModalStack
     * @brief A stack-based manager for modals, allowing for push and pop operations with lifecycle handling.
     *
     * The ModalStack class maintains a stack of modals, providing functionality to add (push) and remove (pop) modals.
     * It also supports a callback mechanism to notify when the active modal changes.
     */
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