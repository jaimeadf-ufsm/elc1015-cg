#pragma once

#include <utility>
#include <cassert>

namespace yap
{
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
                reinterpret_cast<T*>(Payload)->~T();
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
            return *reinterpret_cast<T*>(Payload);
        }

        const T& operator*() const
        {
            assert(IsSet);
            return *reinterpret_cast<const T*>(Payload);
        }

        T* operator->()
        {
            return &**this;
        }

        const T* operator->() const
        {
            return &**this;
        }
    };
}