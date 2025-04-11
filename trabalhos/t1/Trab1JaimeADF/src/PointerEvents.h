#pragma once

/**
 * @file PointerEvents.h
 * @brief Defines the PointerEvents enumeration used to specify pointer interaction behavior.
 */

namespace yap
{
    /**
     * @enum PointerEvents
     * @brief Specifies the pointer interaction behavior for UI elements.
     * 
     * - None: Disables pointer interactions.
     * - Auto: Enables default pointer interactions.
     */
    enum class PointerEvents
    {
        None, ///< Disables pointer interactions.
        Auto  ///< Enables default pointer interactions.
    };
}