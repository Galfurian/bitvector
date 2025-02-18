/// @file math.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Mathematical operations among bitvectors.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#pragma once

#include "bitvector.hpp"

#include <stdexcept>

namespace bvlib
{

namespace detail
{

/// @brief Counts the number of leading zeros in a 32-bit integer.
/// @param x The input number.
/// @return The number of leading zeros.
template <typename T>
inline auto count_leading_zeros(T x) -> std::size_t
{
    // Special case: all bits are zero.
    if (x == 0) {
        // Return the bit-width of T (based on CHAR_BIT).
        return sizeof(T) * CHAR_BIT;
    }
    std::size_t count = 0;
    // Mask to check the most significant bit.
    for (unsigned long mask = static_cast<unsigned long>(1) << (sizeof(T) * CHAR_BIT - 1); (x & mask) == 0;
         mask >>= 1) {
        count++;
    }
    return count;
}

/// @brief Returns the position of the most significant bit inside the given BitVector.
/// @param bitvector The input bitvector.
/// @return Position of the most significant bit (0 if empty).
template <std::size_t N>
inline auto most_significant_bit(const BitVector<N> &bitvector) -> std::size_t
{
    // Iterate through the blocks starting from the most significant block.
    for (std::size_t blockIdx = BitVector<N>::NumBlocks; blockIdx > 0; --blockIdx) {
        // Get the current block.
        const auto &block = bitvector.data[blockIdx - 1];
        // Check if the current block has any bits set.
        if (block) {
            // Calculate the position of the most significant bit.
            return ((blockIdx - 1) * BitVector<N>::BitsPerBlock) +
                   (BitVector<N>::BitsPerBlock - 1 - detail::count_leading_zeros(block));
        }
    }
    // Return 0 if no bits are set (i.e., the BitVector is empty or all bits are 0).
    return 0;
}

/// @brief Adds two blocks and sets the carry, modifying the lhs block in place.
/// @param lhs_block The first block (to be modified).
/// @param rhs_block The second block.
/// @param carry The carry from the previous operation.
/// @return A reference to the first block.
template <typename BlockType>
inline auto add_block_inplace(BlockType &lhs_block, BlockType rhs_block, bool &carry) -> BlockType &
{
    // Add the rhs block and the carry to the lhs block directly.
    lhs_block += static_cast<BlockType>(rhs_block + carry);
    // Update the carry: If the result of addition overflows, set carry to 1, else 0.
    carry = lhs_block < rhs_block;
    // Return A reference to the first block.
    return lhs_block;
}

/// @brief Adds two blocks of bits, and sets the carry for the next block.
/// @param lhs_block The first block of bits (from lhs).
/// @param rhs_block The second block of bits (from rhs).
/// @param carry The carry from the previous operation.
/// @return The sum of the two blocks.
template <typename BlockType>
inline auto add_block(BlockType lhs_block, BlockType rhs_block, bool &carry) -> BlockType
{
    // Add the rhs block and the carry to the lhs block directly.
    lhs_block += static_cast<BlockType>(rhs_block + carry);
    // Update the carry: If the result of addition overflows, set carry to 1, else 0.
    carry = lhs_block < rhs_block;
    // Return A reference to the first block.
    return lhs_block;
}

/// @brief Subtracts two blocks and sets the borrow, modifying the lhs block in place.
/// @param lhs_block The first block (to be modified).
/// @param rhs_block The second block.
/// @param borrow The borrow from the previous operation.
/// @return A reference to the first block.
template <typename BlockType>
inline auto subtract_block_inplace(BlockType &lhs_block, BlockType rhs_block, bool &borrow) -> BlockType &
{
    // Precompute the borrow: If lhs_block is smaller than rhs_block + borrow,
    // borrow will be set to true.
    bool new_borrow = (lhs_block < (rhs_block + borrow));
    // Perform the subtraction with the borrow.
    lhs_block -= static_cast<BlockType>(rhs_block + borrow);
    // Update the borrow value.
    borrow = new_borrow;
    // Return a reference to the modified lhs_block.
    return lhs_block;
}

/// @brief Subtracts two blocks of bits, and sets the borrow for the next block.
/// @param lhs_block The first block of bits (from lhs).
/// @param rhs_block The second block of bits (from rhs).
/// @param borrow The borrow from the previous operation.
/// @return The difference of the two blocks.
template <typename BlockType>
inline auto subtract_block(BlockType lhs_block, BlockType rhs_block, bool &borrow) -> BlockType
{
    // Perform the subtraction with the borrow from the previous block.
    auto result = static_cast<BlockType>(lhs_block - rhs_block - borrow);
    // Set the borrow: If the result of subtraction is negative, set borrow to 1, else 0.
    borrow      = (lhs_block < (rhs_block + borrow));
    // Return the result of the subtraction.
    return result;
}

/// @brief Rotates the bits of the BitVector to the right by a given number of positions, modifying the current BitVector.
/// @param n The number of positions to rotate to the right.
/// @return A reference to the modified BitVector.
template <std::size_t N>
inline BitVector<N> &rotate_right_inplace(BitVector<N> &bv, std::size_t n)
{
    // Normalize the number of positions to prevent unnecessary rotations.
    n %= N;
    // No need to rotate if n is zero
    if (n == 0) {
        return bv;
    }
    // Perform the right rotation using a temporary variable for the bitwise
    // operation.
    for (std::size_t i = 0; i < N; ++i) {
        std::size_t new_pos = (i + N - n) % N;
        if (bv.at(i)) {
            bv.set(new_pos);
        } else {
            bv.reset(new_pos);
        }
    }
    return bv;
}

/// @brief Rotates the bits of the BitVector to the right by a given number of positions.
/// @param bv The BitVector to rotate.
/// @param n The number of positions to rotate to the right.
/// @return A new BitVector with bits rotated to the right.
template <std::size_t N>
inline BitVector<N> rotate_right(const BitVector<N> &bv, std::size_t n)
{
    // Create a copy to avoid modifying the original BitVector.
    BitVector<N> result = bv;
    return detail::rotate_right_inplace(result, n);
}

/// @brief Shifts full blocks of a BitVector to the right or left.
/// @tparam direction The direction to shift: positive for right shift, negative for left shift.
/// @param bv The BitVector to shift.
/// @param block_shift The number of blocks to shift.
template <int direction, std::size_t N>
inline void shift_full_blocks(BitVector<N> &bv, std::size_t block_shift)
{
    constexpr auto NumBlocks = BitVector<N>::NumBlocks;

    if (block_shift) {
        // Handle left shift (negative direction).
        if constexpr (direction > 0) {
            // Left shift full blocks
            for (std::size_t i = 0; i < NumBlocks - block_shift; ++i) {
                bv.data[i] = bv.data[i + block_shift];
            }
            // Zero out the remaining blocks that have moved.
            std::fill(bv.data.end() - block_shift, bv.data.end(), 0);
        }
        // Handle right shift (positive direction).
        else if constexpr (direction < 0) {
            // Right shift full blocks
            for (std::size_t i = NumBlocks - 1; i >= block_shift; --i) {
                bv.data[i] = bv.data[i - block_shift];
            }
            // Zero out the remaining blocks.
            std::fill(bv.data.begin(), bv.data.begin() + block_shift, 0);
        }
    }
}

/// @brief Shifts bits within blocks of a BitVector in either direction.
/// @tparam direction The direction to shift: positive for right shift, negative for left shift.
/// @param bv The BitVector to shift.
/// @param bit_shift The number of bits to shift.
template <int direction, std::size_t N>
inline void shift_within_blocks(BitVector<N> &bv, std::size_t bit_shift)
{
    using BlockType             = typename BitVector<N>::BlockType;
    constexpr auto NumBlocks    = BitVector<N>::NumBlocks;
    constexpr auto BitsPerBlock = static_cast<BlockType>(BitVector<N>::BitsPerBlock);
    if (bit_shift) {
        // Precompute the wrap bits once.
        const auto wrap_bits = BitsPerBlock - bit_shift;
        if constexpr (direction < 0) {
            // Left shift within blocks.
            for (std::size_t i = NumBlocks - 1; i > 0; --i) {
                // Shift current block left, and wrap bits from the previous block.
                bv.data[i] = static_cast<BlockType>((bv.data[i] << bit_shift) | (bv.data[i - 1] >> wrap_bits));
            }
            // Handle the first block (no previous block to wrap bits from).
            bv.data[0] = static_cast<BlockType>(bv.data[0] << bit_shift);
        } else if constexpr (direction > 0) {
            // Right shift within blocks.
            for (std::size_t i = 0; i < NumBlocks - 1; ++i) {
                // Shift current block right, and wrap bits from the next block.
                bv.data[i] = static_cast<BlockType>((bv.data[i] >> bit_shift) | (bv.data[i + 1] << wrap_bits));
            }
            // Handle the last block (no next block to wrap bits from).
            bv.data[NumBlocks - 1] = static_cast<BlockType>(bv.data[NumBlocks - 1] >> bit_shift);
        }
    }
}

/// @brief Left-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline auto shift_left_inplace(BitVector<N> &bitvector, std::size_t shift) -> BitVector<N> &
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
    // Shift full blocks to the left, passing -1 for left shift.
    detail::shift_full_blocks<-1>(bitvector, shift / BitVector<N>::BitsPerBlock);
    // Shift bits within blocks, passing -1 for left shift.
    detail::shift_within_blocks<-1>(bitvector, shift % BitVector<N>::BitsPerBlock);
    // Ensure extra bits beyond N are cleared.
    bitvector.trim();
    return bitvector;
}

/// @brief Right-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline auto shift_right_inplace(BitVector<N> &bitvector, std::size_t shift) -> BitVector<N> &
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
    // Shift full blocks to the right, passing +1 for right shift.
    detail::shift_full_blocks<+1>(bitvector, shift / BitVector<N>::BitsPerBlock);
    // Shift bits within blocks, passing +1 for right shift.
    detail::shift_within_blocks<+1>(bitvector, shift % BitVector<N>::BitsPerBlock);
    return bitvector;
}

/// @brief Left-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline auto shift_left(const BitVector<N> &bitvector, std::size_t shift) -> BitVector<N>
{
    BitVector<N> result = bitvector;
    return detail::shift_left_inplace(result, shift);
}

/// @brief Right-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline auto shift_right(const BitVector<N> &bitvector, std::size_t shift) -> BitVector<N>
{
    BitVector<N> result = bitvector;
    return detail::shift_right_inplace(result, shift);
}

/// @brief Computes the sum of two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum of the two values.
template <std::size_t N1, std::size_t N2>
inline auto sum(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<std::max(N1, N2)>
{
    using ResultType                = BitVector<std::max(N1, N2)>;
    using BlockType                 = typename ResultType::BlockType;
    constexpr std::size_t NumBlocks = ResultType::NumBlocks;
    // Prepare the result.
    ResultType result;
    // Keep track of the carry.
    bool carry = false;
    // Iterate block-wise for efficient addition.
    for (std::size_t i = 0; i < NumBlocks; ++i) {
        auto lhs_block = static_cast<BlockType>((i < BitVector<N1>::NumBlocks) ? lhs.data[i] : 0);
        auto rhs_block = static_cast<BlockType>((i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0);
        // Use add_block to add the blocks and handle the carry in place.
        result.data[i] = detail::add_block(lhs_block, rhs_block, carry);
        // If there's an overflow, throw an exception.
        if (carry && (i == NumBlocks - 1)) {
            throw std::overflow_error("Overflow occurred while adding BitVectors");
        }
    }
    return result;
}

/// @brief Computes the sum of two BitVectors, modifying the first (lhs).
/// @param lhs The first BitVector (will be modified to contain the sum).
/// @param rhs The second BitVector.
/// @return A reference to lhs.
template <std::size_t N1, std::size_t N2>
inline auto sum_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    using ResultType                = BitVector<N1>;
    using BlockType                 = typename ResultType::BlockType;
    constexpr std::size_t NumBlocks = ResultType::NumBlocks;
    // Keep track of the carry.
    bool carry                      = false;
    // Iterate block-wise for efficient addition.
    for (std::size_t i = 0; i < NumBlocks; ++i) {
        auto rhs_block = static_cast<BlockType>((i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0);
        // Use add_block to add the blocks and handle the carry in place.
        detail::add_block_inplace(lhs.data[i], rhs_block, carry);
        // If there's an overflow, throw an exception.
        if (carry && (i == NumBlocks - 1)) {
            throw std::overflow_error("Overflow occurred while adding BitVectors");
        }
    }
    return lhs;
}

/// @brief Computes the difference of two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference between the two values.
template <std::size_t N1, std::size_t N2>
inline auto sub(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<std::max(N1, N2)>
{
    using ResultType                = BitVector<std::max(N1, N2)>;
    using BlockType                 = typename ResultType::BlockType;
    constexpr std::size_t NumBlocks = ResultType::NumBlocks;
    // Prepare the result.
    ResultType result;
    // Keep track of the borrow.
    bool borrow = false;
    // Iterate block-wise for efficient subtraction.
    for (std::size_t i = 0; i < NumBlocks; ++i) {
        auto lhs_block = static_cast<BlockType>((i < BitVector<N1>::NumBlocks) ? lhs.data[i] : 0);
        auto rhs_block = static_cast<BlockType>((i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0);
        // Use subtract_block to subtract the blocks and handle the borrow in
        // place.
        result.data[i] = detail::subtract_block(lhs_block, rhs_block, borrow);
    }
    return result;
}

/// @brief Computes the difference of two BitVectors, modifying the first (lhs).
/// @param lhs The first BitVector (will be modified to contain the difference).
/// @param rhs The second BitVector.
/// @return A reference to lhs.
template <std::size_t N1, std::size_t N2>
inline auto sub_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    using ResultType                = BitVector<N1>;
    using BlockType                 = typename ResultType::BlockType;
    constexpr std::size_t NumBlocks = ResultType::NumBlocks;
    // Keep track of the borrow.
    bool borrow                     = false;
    // Iterate block-wise for efficient subtraction.
    for (std::size_t i = 0; i < NumBlocks; ++i) {
        auto rhs_block = static_cast<BlockType>((i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0);
        // Use subtract_block_inplace to subtract the blocks and handle the
        // borrow in place.
        detail::subtract_block_inplace(lhs.data[i], rhs_block, borrow);
    }
    return lhs;
}

/// @brief Multiplies two BitVectors.
/// @param lhs The first BitVector of size N1.
/// @param rhs The second BitVector of size N2.
/// @return A BitVector of size (N1 + N2), containing the multiplication result.
template <std::size_t N1, std::size_t N2>
inline auto mul(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1 + N2>
{
    // Prepare the result.
    BitVector<N1 + N2> result;
    // Make a copy of RHS.
    const BitVector<N1 + N2> temp(rhs);
    // Perform multiplication using bitwise shifts and additions.
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.at(i)) {
            // Shift the rhs by i bits and add.
            detail::sum_inplace(result, shift_left(temp, i));
        }
    }
    return result;
}

/// @brief Multiplies two BitVectors, modifying the first (lhs) in place.
/// @param lhs The first BitVector (will be modified to contain the product).
/// @param rhs The second BitVector.
/// @return A reference to lhs.
template <std::size_t N1, std::size_t N2>
inline auto mul_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    // Make a copy of the result vector.
    BitVector<N1> lhs_copy(lhs);
    // Reset the result vector.
    lhs.reset();
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs_copy.at(i)) {
            // Shift the rhs by i bits and add.
            detail::sum_inplace(lhs, shift_left(rhs, i));
        }
    }
    return lhs;
}

/// @brief Performs the division of two BitVectors.
/// @param lhs The first BitVector of size N1.
/// @param rhs The second BitVector of size N2.
/// @return A pair of BitVectors of size N1:
///         - first: quotient (size of lhs)
///         - second: remainder (size of lhs)
template <std::size_t N1, std::size_t N2>
inline auto div(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> std::pair<BitVector<N1>, BitVector<N1>>
{
    BitVector<N1> quotient;
    BitVector<N1> remainder;
    BitVector<N1> divisor;

    if (rhs.none()) {
        throw std::domain_error("Division by zero is undefined.");
    }

    // Handle edge cases
    if (lhs.none()) {
        return {quotient, remainder}; // 0 / anything = 0 remainder 0
    }
    if (lhs == rhs) {
        quotient = 1;
        return {quotient, remainder};
    } // lhs / lhs = 1
    if (lhs < rhs) {
        return {quotient, remainder}; // lhs < rhs means quotient = 0, remainder = lhs
    }

    // Initialize remainder and divisor.
    remainder = lhs;
    divisor   = rhs;

    // Align divisor to the most significant bit of the dividend
    std::size_t shift_amount = detail::most_significant_bit(lhs) - detail::most_significant_bit(rhs);
    divisor <<= shift_amount;

    // Perform long division.
    for (std::size_t i = 0; i <= shift_amount; ++i) {
        if (remainder >= divisor) {
            quotient.set(shift_amount - i); // Set corresponding quotient bit
            remainder -= divisor;           // Subtract divisor from remainder
        }
        divisor >>= 1; // Shift divisor for the next step
    }

    return {quotient, remainder};
}

/// @brief Performs the division of two BitVectors, modifying the first (lhs) in place.
/// @param lhs The first BitVector of size N1 (will be modified to contain the quotient).
/// @param rhs The second BitVector of size N2.
/// @return The modified lhs (quotient), and the remainder stored in a separate BitVector.
template <std::size_t N1, std::size_t N2>
inline auto div_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> std::pair<BitVector<N1> &, BitVector<N1>>
{
    BitVector<N1> remainder;
    BitVector<N1> divisor(rhs);
    BitVector<N1> quotient; // This will hold the quotient

    if (rhs.none()) {
        throw std::domain_error("Division by zero is undefined.");
    }

    if (lhs.none()) {
        return {lhs = 0, remainder}; // 0 / anything = 0 remainder 0
    }

    if (lhs == rhs) {
        quotient = 1;
        lhs      = quotient;
        return {lhs, remainder};
    }

    if (lhs < rhs) {
        lhs = quotient; // lhs < rhs means quotient = 0, remainder = lhs
        return {lhs, remainder};
    }

    // Initialize remainder and divisor.
    remainder = lhs;

    // Align divisor to the most significant bit of the dividend
    std::size_t shift_amount = detail::most_significant_bit(lhs) - detail::most_significant_bit(rhs);
    divisor <<= shift_amount;

    // Perform long division.
    for (std::size_t i = 0; i <= shift_amount; ++i) {
        if (remainder >= divisor) {
            quotient.set(shift_amount - i); // Set corresponding quotient bit
            remainder -= divisor;           // Subtract divisor from remainder
        }
        divisor >>= 1; // Shift divisor for the next step
    }

    lhs = quotient;          // Assign the quotient back to lhs
    return {lhs, remainder}; // Return lhs (quotient) and remainder
}

/// @brief Performs two's complement (bitwise negation + manual addition of 1).
/// @param bitvector The BitVector to work with.
/// @return A reference to the modified BitVector.
template <std::size_t N>
inline auto two_complement(BitVector<N> &bitvector) -> BitVector<N> &
{
    // Invert all bits (bitwise negation).
    bitvector.flip();
    // Add 1 using sum_inplace, which will handle the carry propagation. We
    // create a temporary BitVector representing the number 1 to add to the
    // inverted bits.
    BitVector<N> one;
    // Set the least significant bit to 1, effectively adding 1.
    one[0] = true;
    // Use sum_inplace to add 1 to the BitVector.
    detail::sum_inplace(bitvector, one);
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
inline auto operator<<(const BitVector<N> &bitvector, std::size_t shift) -> BitVector<N>
{
    return detail::shift_left(bitvector, shift);
}

// ============================================================================
// OPERATOR (<<=)
// ============================================================================

/// @brief Left-shifts the BitVector by the given number of bits, modifying it.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the modified BitVector.
template <std::size_t N>
inline auto operator<<=(BitVector<N> &bitvector, std::size_t shift) -> BitVector<N> &
{
    return detail::shift_left_inplace(bitvector, shift);
}

// ============================================================================
// OPERATOR (>>
// ============================================================================

/// @brief Right-shifts the input BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return The shifted BitVector.
template <std::size_t N>
inline auto operator>>(const BitVector<N> &bitvector, std::size_t shift) -> BitVector<N>
{
    return detail::shift_right(bitvector, shift);
}

// ============================================================================
// OPERATOR (>>=)
// ============================================================================

/// @brief Right-shifts the BitVector by the given number of bits, modifying it.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the modified BitVector.
template <std::size_t N>
inline auto operator>>=(BitVector<N> &bitvector, std::size_t shift) -> BitVector<N> &
{
    return detail::shift_right_inplace(bitvector, shift);
}

// ============================================================================
// BOOL (==)
// ============================================================================

/// @brief Checks equality between two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return True if they are equal, false otherwise.
template <std::size_t N1, std::size_t N2>
inline auto operator==(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> bool
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
inline auto operator==(const BitVector<N> &lhs, T rhs) -> bool
{
    return lhs == BitVector<N>(rhs);
}

/// @brief Checks equality between an integer value and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if they are equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator==(T lhs, const BitVector<N> &rhs) -> bool
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
inline auto operator!=(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> bool
{
    return !(lhs == rhs);
}

/// @brief Checks inequality between a BitVector and an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if they are not equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator!=(const BitVector<N> &lhs, T rhs) -> bool
{
    return !(lhs == rhs);
}

/// @brief Checks inequality between an integer value and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if they are not equal, false otherwise.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator!=(T lhs, const BitVector<N> &rhs) -> bool
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
inline auto operator<(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> bool
{
    // constexpr std::size_t minBlocks = std::min(BitVector<N1>::NumBlocks, BitVector<N2>::NumBlocks);
    constexpr std::size_t maxBlocks = std::max(BitVector<N1>::NumBlocks, BitVector<N2>::NumBlocks);

    // Compare from most significant block to least significant
    for (std::size_t i = maxBlocks; i > 0; --i) {
        std::size_t idx = i - 1;
        auto lhs_block  = (idx < BitVector<N1>::NumBlocks) ? lhs.data[idx] : 0;
        auto rhs_block  = (idx < BitVector<N2>::NumBlocks) ? rhs.data[idx] : 0;

        if (lhs_block < rhs_block) {
            return true;
        }
        if (lhs_block > rhs_block) {
            return false;
        }
    }

    return false;
}

/// @brief Checks if the BitVector is smaller than an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is smaller than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator<(const BitVector<N> &lhs, T rhs) -> bool
{
    return lhs < BitVector<N>(rhs);
}

/// @brief Checks if an integer value is smaller than the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is smaller than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator<(T lhs, const BitVector<N> &rhs) -> bool
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
inline auto operator<=(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> bool
{
    return (lhs < rhs) || (lhs == rhs);
}

/// @brief Checks if the BitVector is smaller than or equal to an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is smaller than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator<=(const BitVector<N> &lhs, T rhs) -> bool
{
    return lhs <= BitVector<N>(rhs);
}

/// @brief Checks if an integer value is smaller than or equal to the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is smaller than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator<=(T lhs, const BitVector<N> &rhs) -> bool
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
inline auto operator>(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> bool
{
    return rhs < lhs; // Leverage the already optimized operator<
}

/// @brief Checks if the BitVector is greater than an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is greater than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator>(const BitVector<N> &lhs, T rhs) -> bool
{
    return lhs > BitVector<N>(rhs);
}

/// @brief Checks if an integer value is greater than the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is greater than the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator>(T lhs, const BitVector<N> &rhs) -> bool
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
inline auto operator>=(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> bool
{
    return (lhs > rhs) || (lhs == rhs);
}

/// @brief Checks if the BitVector is greater than or equal to an integer value.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return True if the first value is greater than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator>=(const BitVector<N> &lhs, T rhs) -> bool
{
    return lhs >= BitVector<N>(rhs);
}

/// @brief Checks if an integer value is greater than or equal to the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return True if the first value is greater than or equal to the second.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator>=(T lhs, const BitVector<N> &rhs) -> bool
{
    return BitVector<N>(lhs) >= rhs;
}

// ============================================================================
// SUM
// ============================================================================

/// @brief Computes the sum of two BitVectors using the + operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum of the two values.
template <std::size_t N1, std::size_t N2>
inline auto operator+(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<std::max(N1, N2)>
{
    return detail::sum(lhs, rhs);
}

/// @brief Computes the sum of a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum of the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator+(const BitVector<N> &lhs, T rhs) -> BitVector<N>
{
    return detail::sum(lhs, BitVector<N>(rhs));
}

/// @brief Computes the sum of an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The sum of the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator+(T lhs, const BitVector<N> &rhs) -> BitVector<N>
{
    return detail::sum(BitVector<N>(lhs), rhs);
}

/// @brief Computes the sum of two BitVectors and stores the result in the first BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum stored in lhs.
template <std::size_t N1, std::size_t N2>
inline auto operator+=(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    return detail::sum_inplace(lhs, rhs);
}

/// @brief Computes the sum of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum stored in lhs.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator+=(BitVector<N> &lhs, T rhs) -> BitVector<N> &
{
    return detail::sum_inplace(lhs, BitVector<N>(rhs));
}

/// @brief Computes the sum of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum stored in lhs.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator+=(T &lhs, const BitVector<N> &rhs) -> T &
{
    return (lhs = detail::sum(BitVector<N>(lhs), rhs).template to_number<T>());
}

/// @brief Increments the BitVector by 1 (prefix).
/// @param lhs The BitVector.
/// @return The incremented BitVector.
template <std::size_t N>
inline auto operator++(BitVector<N> &lhs) -> BitVector<N> &
{
    return detail::sum_inplace(lhs, BitVector<N>(1));
}

/// @brief Increments the BitVector by 1 (postfix).
/// @param lhs The BitVector.
/// @return The original BitVector before incrementing.
template <std::size_t N>
inline auto operator++(BitVector<N> &lhs, int) -> BitVector<N>
{
    BitVector<N> temp(lhs);
    detail::sum_inplace(lhs, BitVector<N>(1));
    return temp;
}

// ============================================================================
// SUB
// ============================================================================

/// @brief Computes the difference of two BitVectors using the - operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference between the two values.
template <std::size_t N1, std::size_t N2>
inline auto operator-(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<std::max(N1, N2)>
{
    return detail::sub(lhs, rhs);
}

/// @brief Computes the difference of a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator-(const BitVector<N> &lhs, T rhs) -> BitVector<N>
{
    return detail::sub(lhs, BitVector<N>(rhs));
}

/// @brief Computes the difference of an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator-(T lhs, const BitVector<N> &rhs) -> BitVector<N>
{
    return detail::sub(BitVector<N>(lhs), rhs);
}

/// @brief Computes the difference of two BitVectors and stores the result in the first BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference stored in lhs.
template <std::size_t N1, std::size_t N2>
inline auto operator-=(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    return detail::sub_inplace(lhs, rhs);
}

/// @brief Computes the difference of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The difference stored in lhs.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator-=(BitVector<N> &lhs, T rhs) -> BitVector<N> &
{
    return detail::sub_inplace(lhs, BitVector<N>(rhs));
}

/// @brief Computes the difference of an integer and a BitVector, modifying the integer.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The difference stored in lhs.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator-=(T &lhs, const BitVector<N> &rhs) -> T &
{
    return (lhs = detail::sub(BitVector<N>(lhs), rhs).template to_number<T>());
}

/// @brief Decrement the BitVector by 1 (prefix).
/// @param lhs The BitVector.
/// @return The incremented BitVector.
template <std::size_t N>
inline auto operator--(BitVector<N> &lhs) -> BitVector<N> &
{
    return detail::sub_inplace(lhs, BitVector<N>(1));
}

/// @brief Decrement the BitVector by 1 (postfix).
/// @param lhs The BitVector.
/// @return The original BitVector before incrementing.
template <std::size_t N>
inline auto operator--(BitVector<N> &lhs, int) -> BitVector<N>
{
    BitVector<N> temp(lhs);
    detail::sub_inplace(lhs, BitVector<N>(1));
    return temp;
}

// ============================================================================
// MUL
// ============================================================================

/// @brief Multiplies two BitVectors using the * operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The product of the two BitVectors.
template <std::size_t N1, std::size_t N2>
inline auto operator*(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1 + N2>
{
    return detail::mul(lhs, rhs);
}

/// @brief Multiplies a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return A BitVector of size (N + N), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator*(const BitVector<N> &lhs, T rhs) -> BitVector<N + N>
{
    return detail::mul(lhs, BitVector<N>(rhs));
}

/// @brief Multiplies an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return A BitVector of size (N + N), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator*(T lhs, const BitVector<N> &rhs) -> BitVector<N + N>
{
    return detail::mul(BitVector<N>(lhs), rhs);
}

/// @brief Multiplies two BitVectors using the * operator, storing the result in the first BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The product stored in the first BitVector.
template <std::size_t N1, std::size_t N2>
inline auto operator*=(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    return detail::mul_inplace(lhs, rhs);
}

/// @brief Multiplies a BitVector and an integer, storing the result in the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The product stored in the BitVector.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator*=(BitVector<N> &lhs, T rhs) -> BitVector<N> &
{
    return detail::mul_inplace(lhs, BitVector<N>(rhs));
}

/// @brief Multiplies an integer and a BitVector, storing the result in the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The product stored in the BitVector.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator*=(T &lhs, const BitVector<N> &rhs) -> T &
{
    return (lhs = detail::mul(BitVector<N>(lhs), rhs).template to_number<T>());
}

// ============================================================================
// DIV
// ============================================================================

/// @brief Performs division using the / operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The quotient of the two BitVectors.
template <std::size_t N1, std::size_t N2>
inline auto operator/(const BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1>
{
    return detail::div(lhs, rhs).first;
}

/// @brief Performs division between a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The quotient of the division.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator/(const BitVector<N> &lhs, T rhs) -> BitVector<N>
{
    return detail::div(lhs, BitVector<N>(rhs)).first;
}

/// @brief Performs division between an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The quotient of the division.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator/(T lhs, const BitVector<N> &rhs) -> BitVector<N>
{
    return detail::div(BitVector<N>(lhs), rhs).first;
}

/// @brief Performs division using the /= operator.
/// @param lhs The first BitVector (will be modified to contain the quotient).
/// @param rhs The second BitVector.
/// @return The modified lhs (quotient).
template <std::size_t N1, std::size_t N2>
inline auto operator/=(BitVector<N1> &lhs, const BitVector<N2> &rhs) -> BitVector<N1> &
{
    return detail::div_inplace(lhs, rhs).first;
}

/// @brief Performs division between a BitVector and an integer using the /= operator.
/// @param lhs The BitVector (will be modified to contain the quotient).
/// @param rhs The integer value.
/// @return The modified lhs (quotient).
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator/=(BitVector<N> &lhs, T rhs) -> BitVector<N> &
{
    return detail::div_inplace(lhs, BitVector<N>(rhs)).first;
}

/// @brief Performs division between an integer and a BitVector using the /= operator.
/// @param lhs The integer value (will be modified to contain the quotient).
/// @param rhs The BitVector.
/// @return The modified lhs (quotient).
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline auto operator/=(T &lhs, const BitVector<N> &rhs) -> T &
{
    return (lhs = detail::div(BitVector<N>(lhs), rhs).first.template to_number<T>());
}

} // namespace bvlib
