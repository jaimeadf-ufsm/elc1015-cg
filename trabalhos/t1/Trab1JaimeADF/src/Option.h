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
        bool IsSet;
        alignas(T) unsigned char Payload[sizeof(T)];

    public:
        Option() : IsSet(false)
        {
        }

        Option(const T& value) : IsSet(true)
        {
            new (Payload) T(value);
        }

        Option(T&& value) : IsSet(true)
        {
            new (Payload) T(std::move(value));
        }

        Option(const Option& other) : IsSet(other.IsSet)
        {
            if (IsSet)
            {
                new (Payload) T(*other);
            }
        }

        Option(Option&& other) noexcept : IsSet(other.IsSet) {
            if (IsSet)
            {
                new (Payload) T(std::move(*other));
            }
        }

        ~Option()
        {
            Reset();
        }

        void Reset() {
            if (IsSet) 
            {
                GetPointer()->~T();
                IsSet = false;
            }
        }

        operator bool() const
        {
            return IsSet;
        }

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

        T& operator*() {
            assert(IsSet);
            return *GetPointer();
        }

        const T& operator*() const
        {
            assert(IsSet);
            return *GetPointer();
        }

        T* operator->()
        {
            return &**this;
        }

        const T* operator->() const
        {
            return &**this;
        }

    private:
            T* GetPointer() {
                return reinterpret_cast<T*>(Payload);
            }
            
            const T* GetPointer() const {
                return reinterpret_cast<const T*>(Payload);
            }
    };
}