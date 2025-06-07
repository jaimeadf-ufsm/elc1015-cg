#include "Bezier.h"

Bezier::BinomialKey::BinomialKey(uint32_t n, uint32_t k) : N(n), K(k)
{
}

bool Bezier::BinomialKey::operator==(const BinomialKey& other) const
{
    return N == other.N && K == other.K;
}

std::size_t Bezier::BinomialKeyHash::operator()(const BinomialKey& key) const
{
    return std::hash<uint32_t>()(key.N) ^ std::hash<uint32_t>()(key.K);
}

Bezier::Bezier() : m_BinomialCoefficients()
{
}

uint64_t Bezier::BinomialCoefficient(uint32_t n, uint32_t k)
{
    if (k > n)
    {
        return 0;
    }

    BinomialKey key(n, k);

    auto it = m_BinomialCoefficients.find(key);

    if (it != m_BinomialCoefficients.end())
    {
        return it->second;
    }

    if (k == 0 || k == n)
    {
        return 1;
    }

    uint64_t result = BinomialCoefficient(n - 1, k - 1) + BinomialCoefficient(n - 1, k);

    m_BinomialCoefficients[key] = result;

    return result;
}