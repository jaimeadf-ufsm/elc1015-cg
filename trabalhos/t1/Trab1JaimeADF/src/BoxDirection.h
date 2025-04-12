#pragma once

/**
 * @file BoxDirection.h
 * @brief Defines the BoxDirection enumeration used to specify layout directions.
 */

namespace yap
{
    /**
     * @enum BoxDirection
     * @brief Represents the direction of a layout box.
     * 
     * This enumeration is used to specify whether a layout box arranges its
     * elements in a row or a column.
     */
    enum class BoxDirection
    {
        Row,
        Column
    };
}