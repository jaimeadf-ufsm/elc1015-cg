#pragma once

#include <utility>
#include <cassert>

/**
 * @file Option.h
 * @brief Provides a lightweight implementation of an optional value container.
 * 
 * The `yap::Option` class is a custom implementation of an optional value container
 * that can hold a value of type `T` or represent the absence of a value. It avoids
 * dynamic memory allocation by using an internal buffer to store the value.
 */

namespace yap
{
    /**
     * @class Option
     * @brief A lightweight optional value container.
     * 
     * The `Option` class provides a way to store an optional value of type `T`.
     * It uses an internal buffer to store the value and avoids dynamic memory allocation.
     * The class supports copy and move semantics, as well as manual resetting of the value.
     * 
     * @tparam T The type of the value to be stored.
     */
    template <typename T>
    class Option {
    private:
        bool IsSet; ///< Indicates whether the option contains a value.
        alignas(T) unsigned char Payload[sizeof(T)]; ///< Internal buffer to store the value.

    public:
        /**
         * @brief Default constructor. Creates an empty `Option`.
         */
        Option() : IsSet(false)
        {
        }

        /**
         * @brief Constructs an `Option` with a copy of the given value.
         * @param value The value to store in the `Option`.
         */
        Option(const T& value) : IsSet(true)
        {
            new (Payload) T(value);
        }

        /**
         * @brief Constructs an `Option` with a moved value.
         * @param value The value to move into the `Option`.
         */
        Option(T&& value) : IsSet(true)
        {
            new (Payload) T(std::move(value));
        }

        /**
         * @brief Copy constructor. Copies the value from another `Option`.
         * @param other The `Option` to copy from.
         */
        Option(const Option& other) : IsSet(other.IsSet)
        {
            if (IsSet)
            {
                new (Payload) T(*other);
            }
        }

        /**
         * @brief Move constructor. Moves the value from another `Option`.
         * @param other The `Option` to move from.
         */
        Option(Option&& other) noexcept : IsSet(other.IsSet) {
            if (IsSet)
            {
                new (Payload) T(std::move(*other));
            }
        }

        /**
         * @brief Destructor. Destroys the stored value if it exists.
         */
        ~Option()
        {
            Reset();
        }

        /**
         * @brief Resets the `Option`, destroying the stored value if it exists.
         */
        void Reset() {
            if (IsSet) 
            {
                GetPointer()->~T();
                IsSet = false;
            }
        }

        /**
         * @brief Checks if the `Option` contains a value.
         * @return `true` if the `Option` contains a value, `false` otherwise.
         */
        operator bool() const
        {
            return IsSet;
        }

        /**
         * @brief Copy assignment operator. Copies the value from another `Option`.
         * @param other The `Option` to copy from.
         * @return A reference to this `Option`.
         */
        Option& operator=(const Option& other) {
            if (this != &other)
            {
                if (IsSet)
                {
                    Reset();
                }

                if (other.IsSet)
                {
                    new (Payload) T(*other);
                    IsSet = true;
                }
            }

            return *this;
        }

        /**
         * @brief Move assignment operator. Moves the value from another `Option`.
         * @param other The `Option` to move from.
         * @return A reference to this `Option`.
         */
        Option& operator=(Option&& other) noexcept {
            if (this != &other)
            {
                if (IsSet)
                {
                    Reset();
                }

                if (other.IsSet)
                {
                    new (Payload) T(std::move(*other));
                    IsSet = true;
                }
            }

            return *this;
        }

        /**
         * @brief Dereferences the stored value.
         * @return A reference to the stored value.
         * @throws Assertion failure if the `Option` is empty.
         */
        T& operator*() {
            assert(IsSet);
            return *GetPointer();
        }

        /**
         * @brief Dereferences the stored value (const version).
         * @return A const reference to the stored value.
         * @throws Assertion failure if the `Option` is empty.
         */
        const T& operator*() const
        {
            assert(IsSet);
            return *GetPointer();
        }

        /**
         * @brief Accesses the stored value's members.
         * @return A pointer to the stored value.
         * @throws Assertion failure if the `Option` is empty.
         */
        T* operator->()
        {
            return &**this;
        }

        /**
         * @brief Accesses the stored value's members (const version).
         * @return A const pointer to the stored value.
         * @throws Assertion failure if the `Option` is empty.
         */
        const T* operator->() const
        {
            return &**this;
        }

    private:
        /**
         * @brief Gets a pointer to the stored value.
         * @return A pointer to the stored value.
         */
        T* GetPointer() {
            return reinterpret_cast<T*>(Payload);
        }
        
        /**
         * @brief Gets a pointer to the stored value (const version).
         * @return A const pointer to the stored value.
         */
        const T* GetPointer() const {
            return reinterpret_cast<const T*>(Payload);
        }
    };
}