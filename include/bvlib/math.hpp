/// @file bitvector_math.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief
/// @copyright
/// Copyright (c) 2019 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///     The above copyright notice and this permission notice shall be included
///     in all copies or substantial portions of the Software.
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#pragma once

#include "bitvector.hpp"

#include <stdexcept>

namespace bvlib
{

// ============================================================================
// SUPPOR FUNCTIONS
// ============================================================================

/// @brief Adds two bits, and sets the carry.
/// @param b1 fist bit.
/// @param b2 second bit.
/// @param carry the carry from the operation.
/// @return the sum of the two bits.
inline constexpr bool add_bits(bool b1, bool b2, bool &carry)
{
    bool sum = (b1 ^ b2) ^ carry;
    carry    = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

/// @brief Computes the difference between two bits, and updates the borrow.
/// @param b1 fist bit.
/// @param b2 second bit.
/// @param borrow the borrow from the operation.
/// @return the difference between the two bits.
inline constexpr bool sub_bits(bool b1, bool b2, bool &borrow)
{
    bool difference = borrow ? !(b1 ^ b2) : b1 ^ b2;
    borrow          = borrow ? !b1 || b2 : !b1 && b2;
    return difference;
}

/// @brief Returns the position of the most significant bit inside the given bitvector.
/// @param bitvector the input bitvector.
/// @return position of the most significant bit.
template <bvlib::size_type_t N>
inline constexpr bvlib::size_type_t most_significant_bit(const bvlib::BitVector<N> &bitvector)
{
    for (long i = N - 1; i >= 0; i--)
        if (bitvector[i])
            return i;
    return 0;
}

// ============================================================================
// SHIFT
// ============================================================================

/// @brief Left-shifts the input bitvector by the given number of bits.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the shifted bitvector.
template <bvlib::size_type_t N>
inline constexpr auto shift_left(bvlib::BitVector<N> bitvector, bvlib::size_type_t shift)
{
    bvlib::size_type_t it = 0;
    if (shift > 0) {
        for (; it < (N - shift); ++it)
            bitvector.bits[it] = bitvector.bits[it + shift];
        for (; it < N; ++it)
            bitvector.bits[it] = false;
    }
    return bitvector;
}

/// @brief Right-shifts the input bitvector by the given number of bits.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the shifted bitvector.
template <bvlib::size_type_t N>
inline constexpr auto shift_right(bvlib::BitVector<N> bitvector, bvlib::size_type_t shift)
{
    bvlib::size_type_t it;
    if (shift > 0) {
        for (it = N - 1; it >= shift; --it)
            bitvector.bits[it] = bitvector.bits[it - shift];
        for (it = 0; it < shift; ++it)
            bitvector.bits[it] = false;
    }
    return bitvector;
}

// ============================================================================
// OPERATOR(<<)
// ============================================================================

/// @brief Left-shifts the input bitvector by the given number of bits.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the shifted bitvector.
template <bvlib::size_type_t N>
inline constexpr auto &operator<<(const bvlib::BitVector<N> &lhs, bvlib::size_type_t shift)
{
    return bvlib::shift_left(lhs, shift);
}

// ============================================================================
// OPERATOR(<<=)
// ============================================================================

/// @brief Left-shifts the input bitvector by the given number of bits, modifying it.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the input bitvector, shifted.
template <bvlib::size_type_t N>
inline constexpr auto &operator<<=(bvlib::BitVector<N> &lhs, bvlib::size_type_t shift)
{
    bvlib::size_type_t it = 0;
    if (shift > 0) {
        for (; it < (N - shift); ++it)
            lhs.bits[it] = lhs.bits[it + shift];
        for (; it < N; ++it)
            lhs.bits[it] = false;
    }
    return lhs;
}

// ============================================================================
// OPERATOR(>>)
// ============================================================================

/// @brief Right-shifts the input bitvector by the given number of bits.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the shifted bitvector.
template <bvlib::size_type_t N>
inline constexpr auto &operator>>(const bvlib::BitVector<N> &lhs, bvlib::size_type_t shift)
{
    return bvlib::shift_right(lhs, shift);
}

// ============================================================================
// OPERATOR(>>)
// ============================================================================

/// @brief Right-shifts the input bitvector by the given number of bits, modifying it.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the input bitvector, shifted.
template <bvlib::size_type_t N>
inline constexpr auto &operator>>=(bvlib::BitVector<N> &lhs, bvlib::size_type_t shift)
{
    bvlib::size_type_t it = 0;
    if (shift > 0) {
        for (it = N - 1; it >= shift; --it)
            lhs.bits[it] = lhs.bits[it - shift];
        for (it = 0; it < shift; ++it)
            lhs.bits[it] = false;
    }
    return lhs;
}

// ============================================================================
// BOOL(==)
// ============================================================================

/// @brief Checks equality between two bitvectors.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if they are equal.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator==(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    for (bvlib::size_type_t it = max_size; it > 0; --it) {
        if ((((it - 1) < N1) ? lhs[it - 1] : false) != (((it - 1) < N2) ? rhs[it - 1] : false))
            return false;
    }
    return true;
}

/// @brief Checks equality between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if they are equal.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator==(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs == bvlib::BitVector<N>(rhs);
}

/// @brief Checks equality between a bitvector and an integer value.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if they are equal.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator==(T lhs, const bvlib::BitVector<N> &rhs)
{
    return bvlib::BitVector<N>(lhs) == rhs;
}

// ============================================================================
// BOOL(!=)
// ============================================================================

/// @brief Checks inequality between two bitvectors.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if they are equal.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator!=(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    for (bvlib::size_type_t it = max_size; it > 0; --it) {
        if ((((it - 1) < N1) ? lhs[it - 1] : false) != (((it - 1) < N2) ? rhs[it - 1] : false))
            return true;
    }
    return false;
}

/// @brief Checks inequality between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if they are equal.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator!=(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs != bvlib::BitVector<N>(rhs);
}

/// @brief Checks inequality between a bitvector and an integer value.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if they are equal.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator!=(T lhs, const bvlib::BitVector<N> &rhs)
{
    return bvlib::BitVector<N>(lhs) != rhs;
}

// ============================================================================
// BOOL(<)
// ============================================================================

/// @brief Checks if the first bitvector is smaller than the second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if first value is smaller than the second value.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator<(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    for (bvlib::size_type_t it = max_size; it > 0; --it) {
        bool a = ((it - 1) < N1) ? lhs[it - 1] : false;
        bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
        if (a && !b)
            return false;
        if (!a && b)
            return true;
    }
    return false;
}

/// @brief Checks if the bitvector is smaller than the integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if first value is smaller than the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator<(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs < bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is smaller than the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is smaller than the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator<(T lhs, const bvlib::BitVector<N> &rhs)
{
    return bvlib::BitVector<N>(lhs) < rhs;
}

// ============================================================================
// BOOL(<=)
// ============================================================================

/// @brief Checks if the first bitvector is smaller than or equal to the second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if first value is smaller than or equal to the second value.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator<=(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    for (size_type_t it = max_size; it > 0; --it) {
        bool a = ((it - 1) < N1) ? lhs[it - 1] : false;
        bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
        if (a && !b)
            return false;
        if (!a && b)
            return true;
    }
    return true;
}

/// @brief Checks if the bitvector is smaller than or equal to the integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if first value is smaller than or equal to the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator<=(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs <= bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is smaller than or equal to the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is smaller than or equal to the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator<=(T lhs, const bvlib::BitVector<N> &rhs)
{
    return bvlib::BitVector<N>(lhs) <= rhs;
}

// ============================================================================
// BOOL(>)
// ============================================================================

/// @brief Checks if the first bitvector is greather than the second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if first value is greather than the second value.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator>(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    for (size_type_t it = max_size; it > 0; --it) {
        bool a = ((it - 1) < N1) ? lhs[it - 1] : false;
        bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
        if (!a && b)
            return false;
        if (a && !b)
            return true;
    }
    return false;
}

/// @brief Checks if the bitvector is greather than the integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if first value is greather than the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator>(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs > bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is greather than the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is greather than the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator>(T lhs, const bvlib::BitVector<N> &rhs)
{
    return bvlib::BitVector<N>(lhs) > rhs;
}

// ============================================================================
// BOOL(>=)
// ============================================================================

/// @brief Checks if the first bitvector is greather than or equal to the second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if first value is greather than or equal to the second value.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator>=(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    for (size_type_t it = max_size; it > 0; --it) {
        bool a = ((it - 1) < N1) ? lhs[it - 1] : false;
        bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
        if (!a && b)
            return false;
        if (a && !b)
            return true;
    }
    return true;
}

/// @brief Checks if the bitvector is greather than or equal to the integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if first value is greather than or equal to the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator>=(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs >= bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is greather than or equal to the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is greather than or equal to the second value.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator>=(T lhs, const bvlib::BitVector<N> &rhs)
{
    return bvlib::BitVector<N>(lhs) >= rhs;
}

// ============================================================================
// SUM
// ============================================================================

/// @brief Computes the sum between the first and second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the sum between the two values.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto sum(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    bvlib::BitVector<max_size> result;
    bool carry = false;
    for (size_type_t it = 0; it < max_size; ++it) {
        result[it] = bvlib::add_bits((it < N1) ? lhs[it] : false, (it < N2) ? rhs[it] : false, carry);
    }
    return result;
}

/// @brief Computes the sum between the first and second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the sum between the two values.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator+(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::sum<N1, N2>(lhs, rhs);
}

/// @brief Computes the sum between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the sum between the two values.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator+(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::sum<N, N>(lhs, BitVector<N>(rhs));
}

/// @brief Computes the sum between an integer value and a bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return the sum between the two values.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline BitVector<N> operator+(T lhs, BitVector<N> const &rhs)
{
    return bvlib::sum<N, N>(bvlib::BitVector<N>(lhs), rhs);
}

/// @brief Computes the sum between the first and second bitvector, saving the result inside the first.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the sum between the two values.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto &operator+=(bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    static_assert(N1 >= N2);
    bool carry = false;
    for (size_type_t it = 0; it < N1; ++it) {
        lhs[it] = add_bits(lhs[it], (it < N2) ? rhs[it] : false, carry);
    }
    return lhs;
}

/// @brief Computes the sum between a bitvector and an integer value, saving the result inside the bitvector.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the sum between the two values.
template <bvlib::size_type_t N>
inline constexpr auto &operator+=(bvlib::BitVector<N> &lhs, bvlib::size_type_t rhs)
{
    return (lhs += BitVector<N>(rhs));
}

// ============================================================================
// SUB
// ============================================================================

/// @brief Computes the difference between the first and second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the difference between the two values.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto sub(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    bvlib::BitVector<max_size> result;
    bool borrow = false;
    for (size_type_t it = 0; it < max_size; ++it)
        result[it] = bvlib::sub_bits((it < N1) ? lhs[it] : false, (it < N2) ? rhs[it] : false, borrow);
    return result;
}

/// @brief Computes the difference between the first and second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the difference between the two values.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator-(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::sub<N1, N2>(lhs, rhs);
}

/// @brief Computes the difference between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the difference between the two values.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator-(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::sub<N, N>(lhs, bvlib::BitVector<N>(rhs));
}

/// @brief Computes the difference between an integer value and a bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return the difference between the two values.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline bvlib::BitVector<N> operator-(T lhs, bvlib::BitVector<N> const &rhs)
{
    return bvlib::sub<N, N>(bvlib::BitVector<N>(lhs), rhs);
}

/// @brief Computes the difference between the first and second bitvector, saving the result inside the first.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the difference between the two values.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto &operator-=(bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    static_assert(N1 >= N2);
    bool borrow = false;
    for (size_type_t it = 0; it < N1; ++it) {
        lhs[it] = sub_bits(lhs[it], (it < N2) ? rhs[it] : false, borrow);
    }
    return lhs;
}

/// @brief Computes the difference between a bitvector and an integer value, saving the result inside the bitvector.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the difference between the two values.
template <bvlib::size_type_t N>
inline constexpr auto &operator-=(bvlib::BitVector<N> &lhs, bvlib::size_type_t rhs)
{
    return (lhs -= bvlib::BitVector<N>(rhs));
}

// ============================================================================
// MUL
// ============================================================================

/// @brief Multiplies two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return a bitvector of size (MAX(N1, N2)*2), containing the multiplication result.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto mul(bvlib::BitVector<N1> const &lhs, bvlib::BitVector<N2> const &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2), output_size = max_size * 2;
    bvlib::size_type_t it = 0;
    bvlib::BitVector<output_size> result;
    // Perform the multiplication.
    if (lhs.count() < rhs.count()) {
        for (; it < max_size; ++it)
            if (lhs[it])
                result += bvlib::shift_left(rhs, it);
    } else {
        for (; it < max_size; ++it)
            if (rhs[it])
                result += bvlib::shift_left(lhs, it);
    }
    return result;
}

/// @brief Multiplies two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return a bitvector of size (MAX(N1, N2)*2), containing the multiplication result.
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator*(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::mul<N1, N2>(lhs, rhs);
}

/// @brief Multiplies a bitvector and an integer value.
/// @param lhs the bitvector of size N.
/// @param rhs the integer value.
/// @return a bitvector of size (N*2), containing the multiplication result.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator*(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::mul<N, N>(lhs, bvlib::BitVector<N>(rhs));
}

/// @brief Multiplies an integer value and a  bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector of size N.
/// @return a bitvector of size (N*2), containing the multiplication result.
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator*(T lhs, bvlib::BitVector<N> const &rhs)
{
    return bvlib::mul<N, N>(bvlib::BitVector<N>(lhs), rhs);
}

// ============================================================================
// DIV
// ============================================================================

/// @brief Performs the division between two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return two bitvectors of size MAX(N1, N2), the first contains the quotient
/// and the second contains the reminder.
/// @details Original version available in: "C++ Cookbook - By D. Ryan Stephens,
/// Ryan Stephens, Christopher Diggins, Jeff Cogswell, Jonathan Turkanis"
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto div(bvlib::BitVector<N1> const &lhs, bvlib::BitVector<N2> const &rhs)
{
    constexpr bvlib::size_type_t max_size = std::max(N1, N2);
    bvlib::BitVector<max_size> qotient, remainder, support;
    if (rhs.none())
        throw std::domain_error("division by zero undefined");
    if (lhs.none())
        return std::make_pair(qotient, remainder);
    if (lhs == rhs) {
        qotient[0] = true;
        return std::make_pair(qotient, remainder);
    }
    if (lhs < rhs)
        return std::make_pair(qotient, remainder);
    // Initialize the remainder and the support vector.
    remainder = lhs;
    support   = rhs;
    // Count significant digits in lhs and rhs and dividend.
    auto sig_lhs = most_significant_bit(lhs);
    auto sig_rhs = most_significant_bit(rhs);
    // Align the y with the dividend.
    auto n = (sig_lhs - sig_rhs);
    support <<= n;
    // Make sure the loop executes the right number of times.
    n += 1;
    // Long division algorithm, shift, and subtract.
    while (n--) {
        // Shift the quotient to the left.
        if (support <= remainder) {
            // Add a new digit to quotient.
            qotient[n] = true;
            remainder -= support;
        }
        // Shift the y to the right.
        support >>= 1;
    }
    return std::make_pair(qotient, remainder);
}

/// @brief Performs the division between two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return two bitvectors of size MAX(N1, N2), the first contains the quotient
/// and the second contains the reminder.
/// @details Original version available in: "C++ Cookbook - By D. Ryan Stephens,
/// Ryan Stephens, Christopher Diggins, Jeff Cogswell, Jonathan Turkanis"
template <bvlib::size_type_t N1, bvlib::size_type_t N2>
inline constexpr auto operator/(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::div<N1, N2>(lhs, rhs).first;
}

/// @brief Performs the division between a bitvector and an integer value.
/// @param lhs the bitvector of size N.
/// @param rhs the integer value.
/// @return two bitvectors of size N, the first contains the quotient and the
/// second contains the reminder.
/// @details Original version available in: "C++ Cookbook - By D. Ryan Stephens,
/// Ryan Stephens, Christopher Diggins, Jeff Cogswell, Jonathan Turkanis"
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator/(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::div<N, N>(lhs, bvlib::BitVector<N>(rhs)).first;
}

/// @brief Performs the division between an integer value and a bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector of size N.
/// @return two bitvectors of size N, the first contains the quotient and the
/// second contains the reminder.
/// @details Original version available in: "C++ Cookbook - By D. Ryan Stephens,
/// Ryan Stephens, Christopher Diggins, Jeff Cogswell, Jonathan Turkanis"
template <bvlib::size_type_t N, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr auto operator/(T lhs, bvlib::BitVector<N> const &rhs)
{
    return bvlib::div<N, N>(bvlib::BitVector<N>(lhs), rhs).first;
}

} // namespace bvlib
