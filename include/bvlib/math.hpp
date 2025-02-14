/// @file math.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Mathematical operations among bitvectors.

#pragma once

#include "bitvector.hpp"

#include <iostream>
#include <stdexcept>

namespace bvlib
{

namespace detail
{

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
    // Standard bitwise subtraction with borrow.
    bool difference = b1 ^ b2 ^ borrow;
    // Borrow logic.
    borrow          = (!b1 && b2) || (borrow && (!b1 || b2));
    return difference;
}

/// @brief Adds two blocks and sets the carry, modifying the lhs block in place.
/// @param lhs_block The first block (to be modified).
/// @param rhs_block The second block.
/// @param carry The carry from the previous operation.
/// @return A reference to the first block.
template <typename BlockType>
inline BlockType &add_block_inplace(BlockType &lhs_block, BlockType rhs_block, bool &carry)
{
    // Add the rhs block and the carry to the lhs block directly.
    lhs_block += rhs_block + carry;
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
inline BlockType add_block(BlockType lhs_block, BlockType rhs_block, bool &carry)
{
    // Initialize the result.
    BlockType result = lhs_block;
    // Add the blocks.
    return add_block_inplace(result, rhs_block, carry);
}

/// @brief Subtracts two blocks and sets the borrow, modifying the lhs block in place.
/// @param lhs_block The first block (to be modified).
/// @param rhs_block The second block.
/// @param borrow The borrow from the previous operation.
/// @return A reference to the first block.
template <typename BlockType>
inline BlockType &subtract_block_inplace(BlockType &lhs_block, BlockType rhs_block, bool &borrow)
{
    // Subtract the rhs block and the borrow from the lhs block directly.
    lhs_block -= rhs_block + borrow;
    // Update the borrow: If the result of subtraction is negative, set borrow
    // to 1, else 0.
    borrow = (lhs_block > rhs_block);
    // Return A reference to the first block.
    return lhs_block;
}

/// @brief Subtracts two blocks of bits, and sets the borrow for the next block.
/// @param lhs_block The first block of bits (from lhs).
/// @param rhs_block The second block of bits (from rhs).
/// @param borrow The borrow from the previous operation.
/// @return The difference of the two blocks.
template <typename BlockType>
inline BlockType subtract_block(BlockType lhs_block, BlockType rhs_block, bool &borrow)
{
    // Initialize the result.
    BlockType result = lhs_block;
    // Subtract the blocks.
    return subtract_block_inplace(result, rhs_block, borrow);
}

/// @brief Returns the position of the most significant bit inside the given BitVector.
/// @param bitvector The input bitvector.
/// @return Position of the most significant bit (0 if empty).
template <std::size_t N>
inline std::size_t most_significant_bit(const BitVector<N> &bitvector)
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

/// @brief Shifts full blocks of a BitVector to the right or left.
/// @tparam direction The direction to shift: positive for right shift, negative for left shift.
/// @tparam BlockType The datatype of blocks.
/// @tparam NumBlocks The number of blocks.
/// @param data The data array that holds the blocks of the BitVector.
/// @param block_shift The number of blocks to shift.
template <int direction, typename BlockType, std::size_t NumBlocks>
inline void shift_full_blocks(std::array<BlockType, NumBlocks> &data, std::size_t block_shift)
{
    if (block_shift) {
        // Handle positive for right shift.
        if constexpr (direction > 0) {
            // Right shift full blocks
            for (std::size_t i = NumBlocks - 1; i >= block_shift; --i) {
                data[i] = data[i - block_shift];
            }
            std::fill(data.begin(), data.begin() + block_shift, 0);
        }
        // Handle negative for left shift.
        else if constexpr (direction < 0) {
            // Left shift full blocks
            for (std::size_t i = 0; i < NumBlocks - block_shift; ++i) {
                data[i] = data[i + block_shift];
            }
            std::fill(data.end() - block_shift, data.end(), 0);
        }
    }
}

/// @brief Shifts bits within blocks of a BitVector in either direction.
/// @tparam direction The direction to shift: positive for right shift, negative for left shift.
/// @tparam BlockType The datatype of blocks.
/// @tparam NumBlocks The number of blocks.
/// @param data The data array that holds the blocks of the BitVector.
/// @param bit_shift The number of bits to shift.
template <int direction, typename BlockType, std::size_t NumBlocks>
inline void shift_within_blocks(std::array<BlockType, NumBlocks> &data, std::size_t bit_shift)
{
    if (bit_shift) {
        // Handle positive for right shift.
        if constexpr (direction > 0) {
            // Right shift within blocks
            for (std::size_t i = 0; i < NumBlocks - 1; ++i) {
                // Wrap the bits from the next block.
                data[i] = (data[i] >> bit_shift) | (data[i + 1] << (BlockType(0) - bit_shift));
            }
            // Shift the last block.
            data[NumBlocks - 1] >>= bit_shift;
        }
        // Handle negative for left shift.
        else if constexpr (direction < 0) {
            // Left shift within blocks
            for (std::size_t i = NumBlocks - 1; i > 0; --i) {
                // Wrap the bits from the previous block.
                data[i] = (data[i] << bit_shift) | (data[i - 1] >> (BlockType(0) - bit_shift));
            }
            // Shift the first block.
            data[0] <<= bit_shift;
        }
    }
}

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
    // Shift full blocks to the left, passing -1 for left shift.
    bvlib::detail::shift_full_blocks<-1>(bitvector.data, shift / BitVector<N>::BitsPerBlock);
    // Shift bits within blocks, passing -1 for left shift.
    bvlib::detail::shift_within_blocks<-1>(bitvector.data, shift % BitVector<N>::BitsPerBlock);
    // Ensure extra bits beyond N are cleared.
    bitvector.trim();
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
    // Shift full blocks to the right, passing +1 for right shift.
    bvlib::detail::shift_full_blocks<+1>(bitvector.data, shift / BitVector<N>::BitsPerBlock);
    // Shift bits within blocks, passing +1 for right shift.
    bvlib::detail::shift_within_blocks<+1>(bitvector.data, shift % BitVector<N>::BitsPerBlock);
    return bitvector;
}

/// @brief Left-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline BitVector<N> shift_left(const BitVector<N> &bitvector, std::size_t shift)
{
    BitVector<N> result = bitvector;
    return shift_left(result, shift);
}

/// @brief Right-shifts the BitVector by the given number of bits.
/// @param bitvector The BitVector to shift.
/// @param shift The number of bits to shift.
/// @return Reference to the shifted BitVector.
template <std::size_t N>
inline BitVector<N> shift_right(const BitVector<N> &bitvector, std::size_t shift)
{
    BitVector<N> result = bitvector;
    return shift_right(result, shift);
}

/// @brief Computes the sum of two BitVectors.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum of the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> sum(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
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
        result.data[i] = bvlib::detail::add_block(lhs_block, rhs_block, carry);
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
inline BitVector<N1> &sum_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs)
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
        bvlib::detail::add_block_inplace(lhs.data[i], rhs_block, carry);
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
inline BitVector<std::max(N1, N2)> sub(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
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
        result.data[i] = bvlib::detail::subtract_block(lhs_block, rhs_block, borrow);
    }
    return result;
}

/// @brief Computes the difference of two BitVectors, modifying the first (lhs).
/// @param lhs The first BitVector (will be modified to contain the difference).
/// @param rhs The second BitVector.
/// @return A reference to lhs.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> &sub_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs)
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
        bvlib::detail::subtract_block_inplace(lhs.data[i], rhs_block, borrow);
    }
    return lhs;
}

/// @brief Multiplies two BitVectors.
/// @param lhs The first BitVector of size N1.
/// @param rhs The second BitVector of size N2.
/// @return A BitVector of size (N1 + N2), containing the multiplication result.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1 + N2> mul(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    // Prepare the result.
    BitVector<N1 + N2> result;
    // Make a copy of RHS.
    const BitVector<N1 + N2> temp(rhs);
    // Perform multiplication using bitwise shifts and additions.
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.get(i)) {
            // Shift the rhs by i bits and add.
            sum_inplace(result, shift_left(temp, i));
        }
    }
    return result;
}

/// @brief Multiplies two BitVectors, modifying the first (lhs) in place.
/// @param lhs The first BitVector (will be modified to contain the product).
/// @param rhs The second BitVector.
/// @return A reference to lhs.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> &mul_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    // Make a copy of the result vector.
    BitVector<N1> lhs_copy(lhs);
    // Reset the result vector.
    lhs.reset();
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs_copy.get(i)) {
            // Shift the rhs by i bits and add.
            sum_inplace(lhs, shift_left(rhs, i));
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
inline std::pair<BitVector<N1>, BitVector<N1>> div(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    BitVector<N1> quotient, remainder, divisor;

    if (rhs.none()) {
        throw std::domain_error("Division by zero is undefined.");
    }

    // Handle edge cases
    if (lhs.none())
        return {quotient, remainder}; // 0 / anything = 0 remainder 0
    if (lhs == rhs) {
        quotient[0] = true;
        return {quotient, remainder};
    } // lhs / lhs = 1
    if (lhs < rhs)
        return {quotient, remainder}; // lhs < rhs means quotient = 0, remainder = lhs

    // Initialize remainder and divisor.
    remainder = lhs;
    divisor   = rhs;

    // Align divisor to the most significant bit of the dividend
    std::size_t shift_amount = most_significant_bit(lhs) - most_significant_bit(rhs);
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
inline std::pair<BitVector<N1> &, BitVector<N1>> div_inplace(BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    BitVector<N1> remainder;
    BitVector<N1> divisor(rhs);
    BitVector<N1> quotient; // This will hold the quotient

    if (rhs.none()) {
        throw std::domain_error("Division by zero is undefined.");
    }

    if (lhs.none()) {
        return {quotient, remainder}; // 0 / anything = 0 remainder 0
    }

    if (lhs == rhs) {
        quotient[0] = true;
        lhs         = quotient;
        return {lhs, remainder};
    }

    if (lhs < rhs) {
        lhs = quotient; // lhs < rhs means quotient = 0, remainder = lhs
        return {lhs, remainder};
    }

    // Initialize remainder and divisor.
    remainder = lhs;

    // Align divisor to the most significant bit of the dividend
    std::size_t shift_amount = most_significant_bit(lhs) - most_significant_bit(rhs);
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
    return bvlib::detail::shift_left(result, shift);
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
    return bvlib::detail::shift_left(bitvector, shift);
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
    return bvlib::detail::shift_right(result, shift);
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
    return bvlib::detail::shift_right(bitvector, shift);
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
        auto lhs_block  = (idx < BitVector<N1>::NumBlocks) ? lhs.data[idx] : 0;
        auto rhs_block  = (idx < BitVector<N2>::NumBlocks) ? rhs.data[idx] : 0;

        if (lhs_block < rhs_block)
            return true;
        if (lhs_block > rhs_block)
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

/// @brief Computes the sum of two BitVectors using the + operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The sum of the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> operator+(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return bvlib::detail::sum(lhs, rhs);
}

/// @brief Computes the sum of a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum of the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator+(const BitVector<N> &lhs, T rhs)
{
    return bvlib::detail::sum(lhs, BitVector<N>(rhs));
}

/// @brief Computes the sum of an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The sum of the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator+(T lhs, const BitVector<N> &rhs)
{
    return bvlib::detail::sum(BitVector<N>(lhs), rhs);
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
        auto rhs_block = (i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0;
        auto sum       = lhs.data[i] + rhs_block + carry;
        // Detect overflow.
        carry          = (sum < lhs.data[i]);

        lhs.data[i] = sum;
    }

    return lhs;
}

/// @brief Computes the sum of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The sum stored in lhs.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> &operator+=(BitVector<N> &lhs, T rhs)
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

/// @brief Computes the difference of two BitVectors using the - operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The difference between the two values.
template <std::size_t N1, std::size_t N2>
inline BitVector<std::max(N1, N2)> operator-(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return bvlib::detail::sub(lhs, rhs);
}

/// @brief Computes the difference of a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator-(const BitVector<N> &lhs, T rhs)
{
    return bvlib::detail::sub(lhs, BitVector<N>(rhs));
}

/// @brief Computes the difference of an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The difference between the two values.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator-(T lhs, const BitVector<N> &rhs)
{
    return bvlib::detail::sub(BitVector<N>(lhs), rhs);
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
        auto rhs_block = (i < BitVector<N2>::NumBlocks) ? rhs.data[i] : 0;
        auto diff      = lhs.data[i] - rhs_block - borrow;
        borrow         = (lhs.data[i] < rhs_block + borrow); // Detect underflow

        lhs.data[i] = diff;
    }

    return lhs;
}

/// @brief Computes the difference of a BitVector and an integer, modifying the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The difference stored in lhs.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> &operator-=(BitVector<N> &lhs, T rhs)
{
    return (lhs -= BitVector<N>(rhs));
}

/// @brief Decrement the BitVector by 1 (prefix).
/// @param lhs The BitVector.
/// @return The incremented BitVector.
template <std::size_t N>
inline BitVector<N> &operator--(BitVector<N> &lhs)
{
    return lhs -= 1;
}

/// @brief Decrement the BitVector by 1 (postfix).
/// @param lhs The BitVector.
/// @return The original BitVector before incrementing.
template <std::size_t N>
inline BitVector<N> operator--(BitVector<N> &lhs, int)
{
    BitVector<N> temp = lhs;
    lhs -= 1;
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
inline BitVector<N1 + N2> operator*(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return bvlib::detail::mul(lhs, rhs);
}

/// @brief Multiplies a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return A BitVector of size (N + N), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N + N> operator*(const BitVector<N> &lhs, T rhs)
{
    return bvlib::detail::mul(lhs, BitVector<N>(rhs));
}

/// @brief Multiplies an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return A BitVector of size (N + N), containing the multiplication result.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N + N> operator*(T lhs, const BitVector<N> &rhs)
{
    return bvlib::detail::mul(BitVector<N>(lhs), rhs);
}

/// @brief Multiplies two BitVectors using the * operator, storing the result in the first BitVector.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The product stored in the first BitVector.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> &operator*=(BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return bvlib::detail::mul_inplace(lhs, rhs);
}

/// @brief Multiplies a BitVector and an integer, storing the result in the BitVector.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The product stored in the BitVector.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> &operator*=(BitVector<N> &lhs, T rhs)
{
    return bvlib::detail::mul_inplace(lhs, BitVector<N>(rhs));
}

/// @brief Multiplies an integer and a BitVector, storing the result in the BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The product stored in the BitVector.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline T &operator*=(T &lhs, const BitVector<N> &rhs)
{
    return (lhs = bvlib::detail::mul(rhs, BitVector<N>(lhs)).template to_number<T>());
}

// ============================================================================
// DIV
// ============================================================================

/// @brief Performs division using the / operator.
/// @param lhs The first BitVector.
/// @param rhs The second BitVector.
/// @return The quotient of the two BitVectors.
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> operator/(const BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return bvlib::detail::div(lhs, rhs).first;
}

/// @brief Performs division between a BitVector and an integer.
/// @param lhs The BitVector.
/// @param rhs The integer value.
/// @return The quotient of the division.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator/(const BitVector<N> &lhs, T rhs)
{
    return bvlib::detail::div(lhs, BitVector<N>(rhs)).first;
}

/// @brief Performs division between an integer and a BitVector.
/// @param lhs The integer value.
/// @param rhs The BitVector.
/// @return The quotient of the division.
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> operator/(T lhs, const BitVector<N> &rhs)
{
    return bvlib::detail::div(BitVector<N>(lhs), rhs).first;
}

/// @brief Performs division using the /= operator.
/// @param lhs The first BitVector (will be modified to contain the quotient).
/// @param rhs The second BitVector.
/// @return The modified lhs (quotient).
template <std::size_t N1, std::size_t N2>
inline BitVector<N1> &operator/=(BitVector<N1> &lhs, const BitVector<N2> &rhs)
{
    return bvlib::detail::div_inplace(lhs, rhs).first;
}

/// @brief Performs division between a BitVector and an integer using the /= operator.
/// @param lhs The BitVector (will be modified to contain the quotient).
/// @param rhs The integer value.
/// @return The modified lhs (quotient).
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline BitVector<N> &operator/=(BitVector<N> &lhs, T rhs)
{
    return bvlib::detail::div_inplace(lhs, BitVector<N>(rhs)).first;
}

/// @brief Performs division between an integer and a BitVector using the /= operator.
/// @param lhs The integer value (will be modified to contain the quotient).
/// @param rhs The BitVector.
/// @return The modified lhs (quotient).
template <std::size_t N, typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline T &operator/=(T &lhs, const BitVector<N> &rhs)
{
    return (lhs = bvlib::detail::div(BitVector<N>(lhs), rhs).first.template to_number<T>());
}

} // namespace bvlib
