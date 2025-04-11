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
        /**
         * @brief A vector storing the stack of modals.
         */
        std::vector<std::shared_ptr<Modal>> m_Modals;
    
    public:
        /**
         * @brief Callback function triggered when the active modal changes.
         *
         * This function is called whenever a modal is pushed or popped from the stack.
         * If the stack becomes empty, the callback is invoked with a `nullptr` modal.
         */
        std::function<void(ModalStack&, const std::shared_ptr<Modal>&)> OnModal;

        /**
         * @brief Pushes a new modal onto the stack.
         *
         * Adds a modal to the stack and sets up its `OnClose` callback to automatically pop it from the stack
         * when it is closed. If the `OnModal` callback is set, it is triggered with the newly added modal.
         *
         * @param modal The modal to be added to the stack.
         */
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

        /**
         * @brief Pops the top modal from the stack.
         *
         * Removes the top modal from the stack. If the stack is not empty after the removal, the `OnModal` callback
         * is triggered with the new top modal. If the stack becomes empty, the callback is triggered with `nullptr`.
         */
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