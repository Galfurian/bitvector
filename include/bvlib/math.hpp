/// @file math.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Mathematical operations among bitvectors.

#pragma once

#include "bitvector.hpp"

#include <stdexcept>

namespace bvlib
{

namespace detail
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
    // Standard bitwise sum with carry.
    bool sum = b1 ^ b2 ^ carry;
    // Carry logic.
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

/// @brief Computes the difference between two bits, and updates the borrow.
/// @param b1 fist bit.
/// @param b2 second bit.
/// @param borrow the borrow from the operation.
/// @return the difference between the two bits.
inline bool sub_bits(bool b1, bool b2, bool &borrow)
{
    // Standard bitwise subtraction with borrow.
    bool difference = b1 ^ b2 ^ borrow;
    // Borrow logic.
    borrow = (!b1 && b2) || (borrow && (!b1 || b2));
    return difference;
}

/// @brief Returns the position of the most significant bit inside the given BitVector.
/// @param bitvector The input bitvector.
/// @return Position of the most significant bit (0 if empty).
template <std::size_t N>
inline std::size_t most_significant_bit(const BitVector<N> &bitvector)
{
    // Start from the most significant block and iterate down
    for (std::size_t blockIdx = BitVector<N>::NumBlocks; blockIdx > 0; --blockIdx) {
        auto block = bitvector.data[blockIdx - 1];
        if (block) {
            return ((blockIdx - 1) * BitVector<N>::BitsPerBlock) + (31 - detail::count_leading_zeros(block));
        }
    }
    return 0; // No bits are set
}

// ============================================================================
// SHIFT
// ============================================================================

/// @brief Left-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline BitVector<N> &shift_left(BitVector<N> &bitvector, std::size_t shift)
{
    // No change when shifting by 0.
    if (shift == 0) {
        return bitvector;
    }
    // Reset if shift exceeds or equals bitvector size.
    if (shift >= N) {
        bitvector.reset();
        return bitvector;
    }

    std::size_t blockShift = shift / BitVector<N>::BitsPerBlock;
    std::size_t bitShift   = shift % BitVector<N>::BitsPerBlock;
    auto &data             = bitvector.data;

    // Shift full blocks
    if (blockShift > 0) {
        for (std::size_t i = BitVector<N>::NumBlocks - 1; i >= blockShift; --i) {
            data[i] = data[i - blockShift];
        }
        std::fill(data.begin(), data.begin() + blockShift, 0);
    }

    // Shift bits within blocks
    if (bitShift > 0) {
        for (std::size_t i = BitVector<N>::NumBlocks - 1; i > 0; --i) {
            data[i] = (data[i] << bitShift) | (data[i - 1] >> (BitVector<N>::BitsPerBlock - bitShift));
        }
        data[0] <<= bitShift;
    }

    bitvector.trim(); // Ensure extra bits beyond N are cleared
    return bitvector;
}

/// @brief Right-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline BitVector<N> &shift_right(BitVector<N> &bitvector, std::size_t shift)
{
    // No change when shifting by 0.
    if (shift == 0) {
        return bitvector;
    }
    // Reset if shift exceeds or equals bitvector size.
    if (shift >= N) {
        bitvector.reset();
        return bitvector;
    }

    std::size_t blockShift = shift / BitVector<N>::BitsPerBlock;
    std::size_t bitShift   = shift % BitVector<N>::BitsPerBlock;
    auto &data             = bitvector.data;

    // Shift full blocks
    if (blockShift > 0) {
        for (std::size_t i = 0; i < BitVector<N>::NumBlocks - blockShift; ++i) {
            data[i] = data[i + blockShift];
        }
        std::fill(data.end() - blockShift, data.end(), 0);
    }

    // Shift bits within blocks
    if (bitShift > 0) {
        for (std::size_t i = 0; i < BitVector<N>::NumBlocks - 1; ++i) {
            data[i] = (data[i] >> bitShift) | (data[i + 1] << (BitVector<N>::BitsPerBlock - bitShift));
        }
        data[BitVector<N>::NumBlocks - 1] >>= bitShift;
    }

    return bitvector;
}

} // namespace detail

// ============================================================================
// OPERATOR (<<)
// ============================================================================

/// @brief Left-shifts the input BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return The shifted BitVector.
template <std::size_t N>
inline BitVector<N> operator<<(const BitVector<N> &bitvector, std::size_t shift)
{
    BitVector<N> result = bitvector;
    return shift_left(result, shift);
}

// ============================================================================
// OPERATOR (<<=)
// ============================================================================

/// @brief Left-shifts the BitVector by the given number of bits, modifying it.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the modified BitVector.
template <std::size_t N>
inline BitVector<N> &operator<<=(BitVector<N> &bitvector, std::size_t shift)
{
    return shift_left(bitvector, shift);
}

// ============================================================================
// OPERATOR (>>
// ============================================================================

/// @brief Right-shifts the input BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return The shifted BitVector.
template <std::size_t N>
inline BitVector<N> operator>>(const BitVector<N> &bitvector, std::size_t shift)
{
    BitVector<N> result = bitvector;
    return shift_right(result, shift);
}

// ============================================================================
// OPERATOR (>>=)
// ============================================================================

/// @brief Right-shifts the BitVector by the given number of bits, modifying it.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the modified BitVector.
template <std::size_t N>
inline BitVector<N> &operator>>=(BitVector<N> &bitvector, std::size_t shift)
{
    return shift_right(bitvector, shift);
}

// ============================================================================
// BOOL (==)
// ============================================================================

/// @brief Checks equality between two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if they are equal, false otherwise.
template <std::size_t N1, std::size_t N2>
inline bool operator==(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    constexpr std::size_t minBlocks = std::min(BitVector<N1>::NumBlocks, BitVector<N2>::NumBlocks);
    constexpr std::size_t maxBlocks = std::max(BitVector<N1>::NumBlocks, BitVector<N2>::NumBlocks);

    // Compare common blocks
    for (std::size_t i = 0; i < minBlocks; ++i) {
        if (lhs.data[i] != rhs.data[i]) {
            return false;
        }
    }

    // Ensure leading bits are zero in the larger BitVector
    if constexpr (N1 > N2) {
        for (std::size_t i = minBlocks; i < maxBlocks; ++i) {
            if (lhs.data[i] != 0) {
                return false;
            }
        }
    } else if constexpr (N2 > N1) {
        for (std::size_t i = minBlocks; i < maxBlocks; ++i) {
            if (rhs.data[i] != 0) {
                return false;
            }
        }
    }

    return true;
}

/// @brief Checks equality between a BitVector and an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if they are equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator==(const BitVector<N> &lhs, T rhs)
{
    return lhs == BitVector<N>(rhs);
}

/// @brief Checks equality between an integer value and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if they are equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator==(T lhs, const BitVector<N> &rhs)
{
    return BitVector<N>(lhs) == rhs;
}

// ============================================================================
// BOOL (!=)
// ============================================================================

/// @brief Checks inequality between two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if they are not equal, false otherwise.
template <std::size_t N1, std::size_t N2>
inline bool operator!=(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return !(lhs == rhs);
}

/// @brief Checks inequality between a BitVector and an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if they are not equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator!=(const BitVector<N> &lhs, T rhs)
{
    return !(lhs == rhs);
}

/// @brief Checks inequality between an integer value and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if they are not equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator!=(T lhs, const BitVector<N> &rhs)
{
    return !(lhs == rhs);
}

// ============================================================================
// BOOL(<)
// ============================================================================

/// @brief Checks if the first BitVector is smaller than the second BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if the first value is smaller than the second.
template <std::size_t N1, std::size_t N2>
inline bool operator<(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    // constexpr std::size_t minBlocks = std::min(BitVector<N1>::NumBlocks, BitVector<N2>::NumBlocks);
    constexpr std::size_t maxBlocks = std::max(BitVector<N1>::NumBlocks, BitVector<N2>::NumBlocks);

    // Compare from most significant block to least significant
    for (std::size_t i = maxBlocks; i > 0; --i) {
        std::size_t idx = i - 1;
        auto lhsBlock   = (idx < BitVector<N1>::NumBlocks) ? lhs.data[idx] : 0;
        auto rhsBlock   = (idx < BitVector<N2>::NumBlocks) ? rhs.data[idx] : 0;

        if (lhsBlock < rhsBlock)
            return true;
        if (lhsBlock > rhsBlock)
            return false;
    }

    return false;
}

/// @brief Checks if the BitVector is smaller than an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is smaller than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<(const BitVector<N> &lhs, T rhs)
{
    return lhs < BitVector<N>(rhs);
}

/// @brief Checks if an integer value is smaller than the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is smaller than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<(T lhs, const BitVector<N> &rhs)
{
    return BitVector<N>(lhs) < rhs;
}

// ============================================================================
// BOOL(<=)
// ============================================================================

/// @brief Checks if the first BitVector is smaller than or equal to the second BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if the first value is smaller than or equal to the second.
template <std::size_t N1, std::size_t N2>
inline bool operator<=(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

/// @brief Checks if the BitVector is smaller than or equal to an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is smaller than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<=(const BitVector<N> &lhs, T rhs)
{
    return lhs <= BitVector<N>(rhs);
}

/// @brief Checks if an integer value is smaller than or equal to the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is smaller than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator<=(T lhs, const BitVector<N> &rhs)
{
    return BitVector<N>(lhs) <= rhs;
}

// ============================================================================
// BOOL (>)
// ============================================================================

/// @brief Checks if the first BitVector is greater than the second BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if the first value is greater than the second.
template <std::size_t N1, std::size_t N2>
inline bool operator>(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return rhs < lhs; // Leverage the already optimized operator<
}

/// @brief Checks if the BitVector is greater than an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is greater than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>(const BitVector<N> &lhs, T rhs)
{
    return lhs > BitVector<N>(rhs);
}

/// @brief Checks if an integer value is greater than the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is greater than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>(T lhs, const BitVector<N> &rhs)
{
    return BitVector<N>(lhs) > rhs;
}

// ============================================================================
// BOOL (>=)
// ============================================================================

/// @brief Checks if the first BitVector is greater than or equal to the second BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if the first value is greater than or equal to the second.
template <std::size_t N1, std::size_t N2>
inline bool operator>=(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}

/// @brief Checks if the BitVector is greater than or equal to an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is greater than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>=(const BitVector<N> &lhs, T rhs)
{
    return lhs >= BitVector<N>(rhs);
}

/// @brief Checks if an integer value is greater than or equal to the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is greater than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline bool operator>=(T lhs, const BitVector<N> &rhs)
{
    return BitVector<N>(lhs) >= rhs;
}

// ============================================================================
// SUM
// ============================================================================

/// @brief Computes the sum of two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum of the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> sum(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    constexpr std::size_t maxN = std::max(N1, N2);
    BitVector<maxN> result;
    bool carry = false;

    // Iterate block-wise for efficient addition.
    for (std::size_t i = 0; i < BitVector<maxN>::NumBlocks; ++i) {
        auto lhsBlock = (i < BitVector<N1>::NumBlocks) ? lhs.data[i] : 0;
        auto rhsBlock = (i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0;

        auto sum = lhsBlock + rhsBlock + carry;
        carry    = (sum < lhsBlock); // Detect overflow

        result.data[i] = sum;
    }

    return result;
}

/// @brief Computes the sum of two BitVectors using the + operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum of the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> operator+(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return sum(lhs, rhs);
}

/// @brief Computes the sum of a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum of the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator+(const BitVector<N> &lhs, T rhs)
{
    return sum(lhs, BitVector<N>(rhs));
}

/// @brief Computes the sum of an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The sum of the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator+(T lhs, const BitVector<N> &rhs)
{
    return sum(BitVector<N>(lhs), rhs);
}

/// @brief Computes the sum of two BitVectors and stores the result in the first BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum stored in lhs.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> &operator+=(BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    static_assert(N1 >= N2, "LHS must be at least as large as RHS");
    bool carry = false;

    // Iterate block-wise for efficient addition.
    for (std::size_t i = 0; i < BitVector<N1>::NumBlocks; ++i) {
        auto rhsBlock = (i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0;
        auto sum      = lhs.data[i] + rhsBlock + carry;
        carry         = (sum < lhs.data[i]); // Detect overflow

        lhs.data[i] = sum;
    }

    return lhs;
}

/// @brief Computes the sum of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum stored in lhs.
template <std::size_t N>
inline BitVector<N> &operator+=(BitVector<N> &lhs, std::size_t rhs)
{
    return (lhs += BitVector<N>(rhs));
}

/// @brief Increments the BitVector by 1 (prefix).
/// @param lhs The BitVector.
/// @return The incremented BitVector.
template <std::size_t N>
inline BitVector<N> &operator++(BitVector<N> &lhs)
{
    return lhs += 1;
}

/// @brief Increments the BitVector by 1 (postfix).
/// @param lhs The BitVector.
/// @return The original BitVector before incrementing.
template <std::size_t N>
inline BitVector<N> operator++(BitVector<N> &lhs, int)
{
    BitVector<N> temp = lhs;
    lhs += 1;
    return temp;
}

// ============================================================================
// SUB
// ============================================================================

/// @brief Computes the difference of two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference between the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> sub(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    constexpr std::size_t maxN = std::max(N1, N2);
    BitVector<maxN> result;
    bool borrow = false;

    // Iterate block-wise for efficient subtraction.
    for (std::size_t i = 0; i < BitVector<maxN>::NumBlocks; ++i) {
        auto lhsBlock = (i < BitVector<N1>::NumBlocks) ? lhs.data[i] : 0;
        auto rhsBlock = (i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0;

        auto diff = lhsBlock - rhsBlock - borrow;
        borrow    = (lhsBlock < rhsBlock + borrow); // Detect underflow

        result.data[i] = diff;
    }

    return result;
}

/// @brief Computes the difference of two BitVectors using the - operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference between the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> operator-(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return sub(lhs, rhs);
}

/// @brief Computes the difference of a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator-(const BitVector<N> &lhs, T rhs)
{
    return sub(lhs, BitVector<N>(rhs));
}

/// @brief Computes the difference of an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator-(T lhs, const BitVector<N> &rhs)
{
    return sub(BitVector<N>(lhs), rhs);
}

/// @brief Computes the difference of two BitVectors and stores the result in the first BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference stored in lhs.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> &operator-=(BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    static_assert(N1 >= N2, "LHS must be at least as large as RHS");
    bool borrow = false;

    // Iterate block-wise for efficient subtraction.
    for (std::size_t i = 0; i < BitVector<N1>::NumBlocks; ++i) {
        auto rhsBlock = (i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0;
        auto diff     = lhs.data[i] - rhsBlock - borrow;
        borrow        = (lhs.data[i] < rhsBlock + borrow); // Detect underflow

        lhs.data[i] = diff;
    }

    return lhs;
}

/// @brief Computes the difference of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The difference stored in lhs.
template <std::size_t N>
inline BitVector<N> &operator-=(BitVector<N> &lhs, std::size_t rhs)
{
    return (lhs -= BitVector<N>(rhs));
}

// ============================================================================
// MUL
// ============================================================================

/// @brief Multiplies two BitVectors.
/// @param lhs The first BitVector of size N1.
/// @param rhs The second BitVector of size N2.
/// @return A BitVector of size (std::max(N1, N2) * 2), containing the multiplication result.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2) * 2> mul(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    constexpr std::size_t maxN = std::max(N1, N2);
    BitVector<maxN * 2> result;

    // Convert lhs and rhs to BitVector<maxN>
    BitVector<maxN> lhsExt(lhs);
    BitVector<maxN> rhsExt(rhs);

    // Determine which operand has fewer 1s (Hamming weight).
    const BitVector<maxN> &small = (lhsExt.count() < rhsExt.count()) ? lhsExt : rhsExt;
    const BitVector<maxN> &large = (lhsExt.count() < rhsExt.count()) ? rhsExt : lhsExt;

    BitVector<maxN * 2> tempLarge(large);

    // Perform multiplication using bitwise shifts and additions.
    for (std::size_t i = 0; i < small.size(); ++i) {
        if (small.get(i)) {
            result += shift_left(tempLarge, i);
        }
    }

    return result;
}

/// @brief Multiplies two BitVectors using the * operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The product of the two BitVectors.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2) * 2> operator*(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return mul(lhs, rhs);
}

/// @brief Multiplies a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return A BitVector of size (N * 2), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N * 2> operator*(const BitVector<N> &lhs, T rhs)
{
    return mul(lhs, BitVector<N>(rhs));
}

/// @brief Multiplies an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return A BitVector of size (N * 2), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N * 2> operator*(T lhs, const BitVector<N> &rhs)
{
    return mul(BitVector<N>(lhs), rhs);
}

// ============================================================================
// DIV
// ============================================================================

/// @brief Performs the division of two BitVectors.
/// @param lhs The first BitVector of size N1.
/// @param rhs The second BitVector of size N2.
/// @return A pair of BitVectors of size std::max(N1, N2):
///         - first: quotient
///         - second: remainder
template <std::size_t N1, std::size_t N2>
inline std::pair<BitVector<std::max(N1, N2)>, BitVector<std::max(N1, N2)>>
div(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    constexpr std::size_t maxN = std::max(N1, N2);
    BitVector<maxN> quotient, remainder, divisor;

    if (rhs.none()) {
        throw std::domain_error("Division by zero is undefined.");
    }
    if (lhs.none()) {
        return { quotient, remainder }; // 0 / anything = 0 remainder 0
    }
    if (lhs == rhs) {
        quotient[0] = true; // lhs / lhs = 1
        return { quotient, remainder };
    }
    if (lhs < rhs) {
        return { quotient, remainder }; // lhs < rhs means quotient = 0, remainder = lhs
    }

    // Initialize remainder and divisor.
    remainder = lhs;
    divisor   = rhs;

    // Compute most significant bit positions.
    std::size_t sig_lhs      = most_significant_bit(lhs);
    std::size_t sig_rhs      = most_significant_bit(rhs);
    std::size_t shift_amount = sig_lhs - sig_rhs;

    // Align divisor with dividend.
    divisor <<= shift_amount;

    // Perform long division.
    while (shift_amount-- + 1) {
        if (divisor <= remainder) {
            quotient.set(shift_amount);
            remainder -= divisor;
        }
        divisor >>= 1;
    }

    return { quotient, remainder };
}

/// @brief Performs division using the / operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The quotient of the two BitVectors.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> operator/(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return div(lhs, rhs).first;
}

/// @brief Performs division between a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The quotient of the division.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator/(const BitVector<N> &lhs, T rhs)
{
    return div(lhs, BitVector<N>(rhs)).first;
}

/// @brief Performs division between an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The quotient of the division.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator/(T lhs, const BitVector<N> &rhs)
{
    return div(BitVector<N>(lhs), rhs).first;
}

} // namespace bvlib
