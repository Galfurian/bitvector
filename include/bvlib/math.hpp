/// @file math.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Mathematical operations among bitvectors.

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
inline bool add_bits(bool b1, bool b2, bool &carry)
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
inline bool sub_bits(bool b1, bool b2, bool &borrow)
{
    bool difference = borrow ? !(b1 ^ b2) : b1 ^ b2;
    borrow          = borrow ? !b1 || b2 : !b1 && b2;
    return difference;
}

/// @brief Returns the position of the most significant bit inside the given bitvector.
/// @param bitvector the input bitvector.
/// @return position of the most significant bit.
template <std::size_t N>
inline std::size_t most_significant_bit(const bvlib::BitVector<N> &bitvector)
{
    for (long i = N - 1; i >= 0; i--)
        if (bitvector[i])
            return i;
    return std::size_t(0);
}

// ============================================================================
// SHIFT
// ============================================================================

/// @brief Left-shifts the input bitvector by the given number of bits.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the shifted bitvector.
template <std::size_t N>
inline bvlib::BitVector<N> shift_left(bvlib::BitVector<N> bitvector, std::size_t shift)
{
    std::size_t it = 0;
    shift          = std::min(N, shift);
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
template <std::size_t N>
inline bvlib::BitVector<N> shift_right(bvlib::BitVector<N> bitvector, std::size_t shift)
{
    std::size_t it;
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
template <std::size_t N>
inline bvlib::BitVector<N> operator<<(const bvlib::BitVector<N> &bitvector, std::size_t shift)
{
    return bvlib::shift_left(bitvector, shift);
}

// ============================================================================
// OPERATOR(<<=)
// ============================================================================

/// @brief Left-shifts the input bitvector by the given number of bits, modifying it.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the input bitvector, shifted.
template <std::size_t N>
inline bvlib::BitVector<N> &operator<<=(bvlib::BitVector<N> &bitvector, std::size_t shift)
{
    std::size_t it = 0;
    if (shift > 0) {
        for (; it < (N - shift); ++it)
            bitvector.bits[it] = bitvector.bits[it + shift];
        for (; it < N; ++it)
            bitvector.bits[it] = false;
    }
    return bitvector;
}

// ============================================================================
// OPERATOR(>>)
// ============================================================================

/// @brief Right-shifts the input bitvector by the given number of bits.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the shifted bitvector.
template <std::size_t N>
inline bvlib::BitVector<N> operator>>(const bvlib::BitVector<N> &bitvector, std::size_t shift)
{
    return bvlib::shift_right(bitvector, shift);
}

// ============================================================================
// OPERATOR(>>)
// ============================================================================

/// @brief Right-shifts the input bitvector by the given number of bits, modifying it.
/// @param bitvector the bitvector.
/// @param shift the amount to shift.
/// @return the input bitvector, shifted.
template <std::size_t N>
inline bvlib::BitVector<N> &operator>>=(bvlib::BitVector<N> &bitvector, std::size_t shift)
{
    std::size_t it = 0;
    if (shift > 0) {
        for (it = N - 1; it >= shift; --it)
            bitvector.bits[it] = bitvector.bits[it - shift];
        for (it = 0; it < shift; ++it)
            bitvector.bits[it] = false;
    }
    return bitvector;
}

// ============================================================================
// BOOL(==)
// ============================================================================

/// @brief Checks equality between two bitvectors.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return if they are equal.
template <std::size_t N1, std::size_t N2>
inline bool operator==(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t min = std::min(N1, N2);
    std::size_t it;
    if constexpr (N1 > N2) {
        for (it = N1; it > min; --it)
            if (lhs[it - 1])
                return false;
    } else {
        for (it = N2; it > min; --it)
            if (rhs[it - 1])
                return false;
    }
    for (; it > 0; --it) {
        if (lhs[it - 1] != rhs[it - 1])
            return false;
    }
    return true;
}

/// @brief Checks equality between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if they are equal.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator==(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs == bvlib::BitVector<N>(rhs);
}

/// @brief Checks equality between a bitvector and an integer value.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if they are equal.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator==(T lhs, const bvlib::BitVector<N> &rhs)
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
template <std::size_t N1, std::size_t N2>
inline bool operator!=(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t min = std::min(N1, N2);
    std::size_t it;
    if constexpr (N1 > N2) {
        for (it = N1; it > min; --it)
            if (lhs[it - 1])
                return true;
    } else {
        for (it = N2; it > min; --it)
            if (rhs[it - 1])
                return true;
    }
    for (; it > 0; --it) {
        if (lhs[it - 1] != rhs[it - 1])
            return true;
    }
    return false;
}

/// @brief Checks inequality between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if they are equal.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator!=(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs != bvlib::BitVector<N>(rhs);
}

/// @brief Checks inequality between a bitvector and an integer value.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if they are equal.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator!=(T lhs, const bvlib::BitVector<N> &rhs)
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
template <std::size_t N1, std::size_t N2>
inline bool operator<(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t min = std::min(N1, N2);
    std::size_t it;
    if constexpr (N1 > N2) {
        for (it = N1; it > min; --it)
            if (lhs[it - 1])
                return false;
    } else {
        for (it = N2; it > min; --it)
            if (rhs[it - 1])
                return true;
    }
    for (; it > 0; --it) {
        const bool a = lhs[it - 1], b = rhs[it - 1];
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
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs < bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is smaller than the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is smaller than the second value.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<(T lhs, const bvlib::BitVector<N> &rhs)
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
template <std::size_t N1, std::size_t N2>
inline bool operator<=(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t min = std::min(N1, N2);
    std::size_t it;
    if constexpr (N1 > N2) {
        for (it = N1; it > min; --it)
            if (lhs[it - 1])
                return false;
    } else {
        for (it = N2; it > min; --it)
            if (rhs[it - 1])
                return true;
    }
    for (; it > 0; --it) {
        const bool a = lhs[it - 1], b = rhs[it - 1];
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
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<=(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs <= bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is smaller than or equal to the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is smaller than or equal to the second value.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<=(T lhs, const bvlib::BitVector<N> &rhs)
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
template <std::size_t N1, std::size_t N2>
inline bool operator>(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t min = std::min(N1, N2);
    std::size_t it;
    if constexpr (N1 > N2) {
        for (it = N1; it > min; --it)
            if (lhs[it - 1])
                return true;
    } else {
        for (it = N2; it > min; --it)
            if (rhs[it - 1])
                return false;
    }
    for (; it > 0; --it) {
        const bool a = lhs[it - 1], b = rhs[it - 1];
        if (a && !b)
            return true;
        if (!a && b)
            return false;
    }
    return false;
}

/// @brief Checks if the bitvector is greather than the integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if first value is greather than the second value.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs > bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is greather than the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is greather than the second value.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>(T lhs, const bvlib::BitVector<N> &rhs)
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
template <std::size_t N1, std::size_t N2>
inline bool operator>=(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t min = std::min(N1, N2);
    std::size_t it;
    if constexpr (N1 > N2) {
        for (it = N1; it > min; --it)
            if (lhs[it - 1])
                return true;
    } else {
        for (it = N2; it > min; --it)
            if (rhs[it - 1])
                return false;
    }
    for (; it > 0; --it) {
        const bool a = lhs[it - 1], b = rhs[it - 1];
        if (a && !b)
            return true;
        if (!a && b)
            return false;
    }
    return true;
}

/// @brief Checks if the bitvector is greather than or equal to the integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return if first value is greather than or equal to the second value.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>=(const bvlib::BitVector<N> &lhs, T rhs)
{
    return lhs >= bvlib::BitVector<N>(rhs);
}

/// @brief Checks if the integer value is greather than or equal to the bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return if first value is greather than or equal to the second value.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>=(T lhs, const bvlib::BitVector<N> &rhs)
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
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2)> sum(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t max = std::max(N1, N2);
    bvlib::BitVector<max> result;
    bool carry = false;
    for (std::size_t it = 0; it < max; ++it) {
        result[it] = bvlib::add_bits((it < N1) ? lhs[it] : false, (it < N2) ? rhs[it] : false, carry);
    }
    return result;
}

/// @brief Computes the sum between the first and second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the sum between the two values.
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2)> operator+(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::sum<N1, N2>(lhs, rhs);
}

/// @brief Computes the sum between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the sum between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N> operator+(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::sum<N, N>(lhs, BitVector<N>(rhs));
}

/// @brief Computes the sum between an integer value and a bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return the sum between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N> operator+(T lhs, BitVector<N> const &rhs)
{
    return bvlib::sum<N, N>(bvlib::BitVector<N>(lhs), rhs);
}

/// @brief Computes the sum between the first and second bitvector, saving the result inside the first.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the sum between the two values.
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<N1> &operator+=(bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    static_assert(N1 >= N2, "RHS has more bits than LHS");
    bool carry = false;
    for (std::size_t it = 0; it < N1; ++it) {
        lhs[it] = add_bits(lhs[it], (it < N2) ? rhs[it] : false, carry);
    }
    return lhs;
}

/// @brief Computes the sum between a bitvector and an integer value, saving the result inside the bitvector.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the sum between the two values.
template <std::size_t N>
inline bvlib::BitVector<N> &operator+=(bvlib::BitVector<N> &lhs, std::size_t rhs)
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
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2)> sub(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    constexpr std::size_t max = std::max(N1, N2);
    bvlib::BitVector<max> result;
    bool borrow = false;
    for (std::size_t it = 0; it < max; ++it)
        result[it] = bvlib::sub_bits((it < N1) ? lhs[it] : false, (it < N2) ? rhs[it] : false, borrow);
    return result;
}

/// @brief Computes the difference between the first and second bitvector.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the difference between the two values.
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2)> operator-(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::sub<N1, N2>(lhs, rhs);
}

/// @brief Computes the difference between a bitvector and an integer value.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N> operator-(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::sub<N, N>(lhs, bvlib::BitVector<N>(rhs));
}

/// @brief Computes the difference between an integer value and a bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector.
/// @return the difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N> operator-(T lhs, bvlib::BitVector<N> const &rhs)
{
    return bvlib::sub<N, N>(bvlib::BitVector<N>(lhs), rhs);
}

/// @brief Computes the difference between the first and second bitvector, saving the result inside the first.
/// @param lhs the first bitvector.
/// @param rhs the second bitvector.
/// @return the difference between the two values.
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2)> &operator-=(bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    static_assert(N1 >= N2, "RHS has more bits than LHS");
    bool borrow = false;
    for (std::size_t it = 0; it < N1; ++it) {
        lhs[it] = sub_bits(lhs[it], (it < N2) ? rhs[it] : false, borrow);
    }
    return lhs;
}

/// @brief Computes the difference between a bitvector and an integer value, saving the result inside the bitvector.
/// @param lhs the bitvector.
/// @param rhs the integer value.
/// @return the difference between the two values.
template <std::size_t N>
inline bvlib::BitVector<N> &operator-=(bvlib::BitVector<N> &lhs, std::size_t rhs)
{
    return (lhs -= bvlib::BitVector<N>(rhs));
}

// ============================================================================
// MUL
// ============================================================================

/// @brief Multiplies two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return a bitvector of size (std::max(N1, N2)*2), containing the multiplication result.
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2) * 2> mul(bvlib::BitVector<N1> const &lhs, bvlib::BitVector<N2> const &rhs)
{
    constexpr std::size_t max = std::max(N1, N2);
    std::size_t it = 0;
    bvlib::BitVector<max * 2> result;
    // Perform the multiplication.
    if (lhs.count() < rhs.count()) {
        bvlib::BitVector<max * 2> _rhs(rhs);
        for (; it < N1; ++it)
            if (lhs[it])
                result += bvlib::shift_left(_rhs, it);
    } else {
        bvlib::BitVector<max * 2> _lhs(lhs);
        for (; it < N2; ++it)
            if (rhs[it])
                result += bvlib::shift_left(_lhs, it);
    }
    return result;
}

/// @brief Multiplies two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return a bitvector of size (std::max(N1, N2)*2), containing the multiplication result.
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2) * 2> operator*(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
{
    return bvlib::mul<N1, N2>(lhs, rhs);
}

/// @brief Multiplies a bitvector and an integer value.
/// @param lhs the bitvector of size N.
/// @param rhs the integer value.
/// @return a bitvector of size (N*2), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N * 2> operator*(const bvlib::BitVector<N> &lhs, T rhs)
{
    return bvlib::mul<N, N>(lhs, bvlib::BitVector<N>(rhs));
}

/// @brief Multiplies an integer value and a  bitvector.
/// @param lhs the integer value.
/// @param rhs the bitvector of size N.
/// @return a bitvector of size (N*2), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N * 2> operator*(T lhs, bvlib::BitVector<N> const &rhs)
{
    return bvlib::mul<N, N>(bvlib::BitVector<N>(lhs), rhs);
}

// ============================================================================
// DIV
// ============================================================================

/// @brief Performs the division between two bitvectors.
/// @param lhs the first bitvector of size N1.
/// @param rhs the second bitvector of size N2.
/// @return two bitvectors of size std::max(N1, N2), the first contains the quotient
/// and the second contains the reminder.
/// @details Original version available in: "C++ Cookbook - By D. Ryan Stephens,
/// Ryan Stephens, Christopher Diggins, Jeff Cogswell, Jonathan Turkanis"
template <std::size_t N1, std::size_t N2>
inline std::pair<bvlib::BitVector<std::max(N1, N2)>, bvlib::BitVector<std::max(N1, N2)>> div(bvlib::BitVector<N1> const &lhs, bvlib::BitVector<N2> const &rhs)
{
    constexpr std::size_t max = std::max(N1, N2);
    bvlib::BitVector<max> qotient, remainder, support;
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
    std::size_t sig_lhs = most_significant_bit(lhs);
    std::size_t sig_rhs = most_significant_bit(rhs);
    // Align the y with the dividend.
    std::size_t n = (sig_lhs - sig_rhs);
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
/// @return two bitvectors of size std::max(N1, N2), the first contains the quotient
/// and the second contains the reminder.
/// @details Original version available in: "C++ Cookbook - By D. Ryan Stephens,
/// Ryan Stephens, Christopher Diggins, Jeff Cogswell, Jonathan Turkanis"
template <std::size_t N1, std::size_t N2>
inline bvlib::BitVector<std::max(N1, N2)> operator/(const bvlib::BitVector<N1> &lhs, const bvlib::BitVector<N2> &rhs)
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
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N> operator/(const bvlib::BitVector<N> &lhs, T rhs)
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
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bvlib::BitVector<N> operator/(T lhs, bvlib::BitVector<N> const &rhs)
{
    return bvlib::div<N, N>(bvlib::BitVector<N>(lhs), rhs).first;
}

} // namespace bvlib
