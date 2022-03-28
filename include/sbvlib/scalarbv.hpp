/*! \file
 * # Definition of the classes for managing categorical data arrays.
 *
 * This header file defines classes allowing to store sampled input points, the corresponding outputs and derivatives.
 *
 * \copyright (c) 2020 SYDELITY b.v., all rights reserved.
 */

#pragma once

#include <cassert>
#include <iostream>

namespace sbvlib
{

#define bit_set(V, B)          ((V) | (1ULL << (B)))   ///< Sets the given bit.
#define bit_clear(V, B)        ((V) & ~(1ULL << (B)))  ///< Clears the given bit.
#define bit_flip(V, B)         ((V) ^ (1ULL << (B)))   ///< Flips the given bit.
#define bit_set_assign(V, B)   ((V) |= (1ULL << (B)))  ///< Sets the given bit, permanently.
#define bit_clear_assign(V, B) ((V) &= ~(1ULL << (B))) ///< Clears the given bit, permanently.
#define bit_flip_assign(V, B)  ((V) ^= (1ULL << (B)))  ///< Flips the given bit, permanently.
#define bit_check(V, B)        ((V) & (1ULL << (B)))   ///< Checks if the given bit is 1.

#define bit_set_from(V, B, S) ((S) ? bit_set_assign(V, B) : bit_clear_assign(V, B))

#define bitmask_set(V, M)          ((V) | (M))   ///< Sets the bits identified by the mask.
#define bitmask_clear(V, M)        ((V) & ~(M))  ///< Clears the bits identified by the mask.
#define bitmask_flip(V, M)         ((V) ^ (M))   ///< Flips the bits identified by the mask.
#define bitmask_set_assign(V, M)   ((V) |= (M))  ///< Sets the bits identified by the mask, permanently.
#define bitmask_clear_assign(V, M) ((V) &= ~(M)) ///< Clears the bits identified by the mask, permanently.
#define bitmask_flip_assign(V, M)  ((V) ^= (M))  ///< Flips the bits identified by the mask, permanently.
#define bitmask_check(V, M)        ((V) & (M))   ///< Checks if the bits identified by the mask are all 1.

class ScalarBitVector {
public:
    using size_type_t  = unsigned long long;
    using value_type_t = unsigned long long;

    value_type_t value = 0ULL;

    ScalarBitVector()
        : value(0)
    {
    }

    ScalarBitVector(value_type_t _value)
        : value(_value)
    {
    }

    const char *to_string() const
    {
        static char str[65] = { 0 };
        for (size_type_t i = 0; i < 64ULL; ++i)
            str[i] = bit_check(value, 64ULL - 1 - i) ? '1' : '0';
        return str;
    }

    value_type_t to_number() const
    {
        return value;
    }

    value_type_t to_ullong() const
    {
        return value;
    }

    /// @brief Flips every bit.
    inline auto &flip()
    {
        value = ~value;
        return (*this);
    }

    /// @brief Flips a given bit.
    inline ScalarBitVector &flip(size_type_t position)
    {
        assert(position < 64ULL);
        bit_flip_assign(value, position);
        return (*this);
    }

    /// @brief Flips a given bit.
    inline ScalarBitVector &set(size_type_t position)
    {
        assert(position < 64ULL);
        bit_set_assign(value, position);
        return (*this);
    }

    /// @brief Flips a given bit.
    inline ScalarBitVector &clear(size_type_t position)
    {
        assert(position < 64ULL);
        bit_clear_assign(value, position);
        return (*this);
    }

    /// @brief Returns the size in bit of the value.
    inline size_t size() const
    {
        return 64ULL;
    }

    /// @brief Tests whether all the bits are on.
    inline bool all() const
    {
        return ((value + 1) & value) == 0;
    }

    /// @brief Tests whether any of the bits are on.
    inline bool any() const
    {
        return value != 0;
    }

    /// @brief Tests whether none of the bits are on.
    inline bool none() const
    {
        return value == 0;
    }

    /// @brief Returns the number of bits which are set.
    inline size_type_t count() const
    {
        size_type_t result = 0;
        for (size_type_t i = 0; i < 64ULL; ++i)
            result += bit_check(value, i);
        return result;
    }

    inline bool operator[](size_type_t position) const
    {
        assert(position < 64ULL);
        return bit_check(value, position);
    }

    inline bool operator==(const ScalarBitVector &rhs) const
    {
        return value == rhs.value;
    }

    inline bool operator!=(const ScalarBitVector &rhs) const
    {
        return value != rhs.value;
    }

    inline bool operator<(const ScalarBitVector &rhs) const
    {
        return value < rhs.value;
    }

    inline bool operator>(const ScalarBitVector &rhs) const
    {
        return value > rhs.value;
    }

    inline bool operator<=(const ScalarBitVector &rhs) const
    {
        return value <= rhs.value;
    }

    inline bool operator>=(const ScalarBitVector &rhs) const
    {
        return value >= rhs.value;
    }

    inline ScalarBitVector operator+(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value += rhs.value;
        return result;
    }

    inline ScalarBitVector operator-(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value -= rhs.value;
        return result;
    }

    inline ScalarBitVector operator*(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value *= rhs.value;
        return result;
    }

    inline ScalarBitVector operator/(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value /= rhs.value;
        return result;
    }

    inline ScalarBitVector operator&(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value &= rhs.value;
        return result;
    }

    inline ScalarBitVector operator^(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value ^= rhs.value;
        return result;
    }

    inline ScalarBitVector operator|(const ScalarBitVector &rhs) const
    {
        ScalarBitVector result(value);
        result.value |= rhs.value;
        return result;
    }

    inline ScalarBitVector operator~() const
    {
        ScalarBitVector result(value);
        result.value = ~result.value;
        return result;
    }

    inline ScalarBitVector operator!() const
    {
        ScalarBitVector result(value);
        result.value = !result.value;
        return result;
    }

    inline ScalarBitVector operator-()
    {
        return ScalarBitVector(-value);
    }
};

/// @brief Overrides the output stream operator.
inline std::ostream &operator<<(std::ostream &lhs, ScalarBitVector const &rhs)
{
    lhs << rhs.to_number();
    return lhs;
}

} // namespace sbvlib
