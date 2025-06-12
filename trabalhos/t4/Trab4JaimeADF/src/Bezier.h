/**
 * @file Bezier.h
 * @brief Bezier curve evaluation class with binomial coefficient caching
 */

#pragma once

#include <iterator>
#include <unordered_map>
#include <cmath>
#include <cstdint>

/**
 * @class Bezier
 * @brief Provides Bezier curve evaluation functionality with optimized binomial coefficient computation
 */
class Bezier
{
private:
    struct BinomialKey
    {
        uint32_t N;
        uint32_t K;

        BinomialKey(uint32_t n, uint32_t k);

        bool operator==(const BinomialKey& other) const;
    };

    struct BinomialKeyHash
    {
        std::size_t operator()(const BinomialKey& key) const;
    };

public:
    Bezier();

    template<typename T>
    typename std::iterator_traits<T>::value_type Evaluate(T begin, T end, double t)
    {
        using ValueType = typename std::iterator_traits<T>::value_type;

        t = std::max(0.0, std::min(1.0, t));

        ValueType result = ValueType();
        T it = begin;

        uint32_t n = static_cast<uint32_t>(std::distance(begin, end)) - 1;

        for (uint32_t i = 0; i <= n; ++i)
        {
            double coefficient = BinomialCoefficient(n, i);
            double basis = coefficient * std::pow(t, i) * std::pow(1.0 - t, n - i);

            result += *it * basis;

            ++it;
        }

        return result;
    }

private:
    std::unordered_map<BinomialKey, uint64_t, BinomialKeyHash> m_BinomialCoefficients;

    uint64_t BinomialCoefficient(uint32_t n, uint32_t k);
};