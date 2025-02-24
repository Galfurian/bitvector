/// @file bitvector.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Base bitvector class.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#pragma once

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>

enum : unsigned char {
    BVLIB_MAJOR_VERSION = 1, ///< Major version of the library.
    BVLIB_MINOR_VERSION = 5, ///< Minor version of the library.
    BVLIB_MICRO_VERSION = 0  ///< Micro version of the library.
};

/// @brief Contains the BitVector class.
namespace bvlib
{

/// @brief Contains support classes and functions.
namespace detail
{

/// @brief Counts the number of 1s in a 32-bit integer.
/// @param x The integer.
/// @return The number of set bits.
template <typename T>
inline auto popcount(T x) -> std::size_t
{
    std::size_t count = 0;
    while (x) {
        // Clears the lowest set bit.
        x = static_cast<T>(x & (x - 1));
        count++;
    }
    return count;
}

/// @brief Calculates the smallest appropriate unsigned data type for BlockType
/// based on the number of bits in the BitVector.
/// @param N The size of the BitVector in bits.
template <std::size_t N>
struct BlockTypeHelper {
    /// Determine the smallest unsigned integer type that can store N bits.
    using type = std::conditional_t<
        (N <= 8),
        std::uint8_t, // 8-bit
        std::conditional_t<
            (N <= 16),
            std::uint16_t, // 16-bit
            std::conditional_t<
                (N <= 32),
                std::uint32_t, // 32-bit
                std::uint64_t  // 64-bit
                >>>;
};

/// @brief A reference-like proxy to a modifiable bit.
/// @details
/// This class provides a reference-like interface to access and modify individual bits
/// within a block of bits (e.g., `BlockType`). It allows getting and setting bit values
/// at a specified position in the block. The `operator bool()` allows reading the bit
/// value, and the `operator=` allows modifying the bit.
///
/// @tparam BlockType The datatype used to store blocks of bits (e.g.,///
/// `uint32_t`, `uint64_t`).
template <typename BlockType>
class BitReference
{
private:
    /// @brief The block containing the bit being referenced.
    std::reference_wrapper<BlockType> block;
    /// @brief The position of the bit within the block.
    std::size_t position;

public:
    /// @brief Constructs a BitReference for a specific block and bit position.
    /// @details
    /// This constructor initializes the reference with a block and a bit position.
    /// It allows modifying or accessing the bit at the specified position within the block.
    ///
    /// @param _block The block that contains the bit.
    /// @param _position The position of the bit within the block.
    BitReference(BlockType &_block, std::size_t _position)
        : block(_block)
        , position(_position)
    {
        // Nothing to do
    }

    /// @brief Implicit conversion to bool to read the value of the bit.
    /// @details
    /// This operator allows using the `BitReference` in boolean contexts (e.g., in conditionals).
    /// It returns `true` if the bit at the specified position is set, `false` otherwise.
    ///
    /// @return `true` if the referenced bit is set, `false` otherwise.
    operator bool() const { return (block & (BlockType(1) << position)) != 0; }

    /// @brief Assigns a value to the referenced bit.
    /// @details
    /// This operator allows modifying the bit at the specified position. If the value is `true`,
    /// the bit is set to 1. If the value is `false`, the bit is cleared (set to 0).
    ///
    /// @param value The value to assign to the referenced bit (`true` to set, `false` to clear).
    /// @return A reference to the current `BitReference` to allow chaining assignments.
    auto operator=(bool value) -> BitReference &
    {
        if (value) {
            // Set the bit.
            block |= static_cast<BlockType>(BlockType(1) << position);
        } else {
            // Clear the bit.
            block &= static_cast<BlockType>(~(BlockType(1) << position));
        }
        return *this;
    }
};

} // namespace detail

/// @brief Class which reproduces the behaviour of bit-vector.
/// @tparam N Lenght of the bit-vector
template <std::size_t N>
class BitVector
{
    static_assert(N > 0, "BitVector length must be greater than 0");

public:
    /// @brief Defines the block type used for storing bits.
    /// @details The block type is defined as a 64-bit unsigned integer, which
    /// will store multiple bits per block for efficient operations.
    using BlockType = typename detail::BlockTypeHelper<N>::type;

    /// @brief Number of stored bits.
    static constexpr std::size_t NumBits = N;

    /// @brief Number of bits per block.
    /// @details This is calculated as the size of the block type in bits, using
    /// `sizeof(BlockType) * CHAR_BIT` to support different sizes of
    /// `BlockType`.
    static constexpr std::size_t BitsPerBlock = sizeof(BlockType) * CHAR_BIT;

    /// @brief Number of blocks required to store N bits.
    /// @details This is calculated by dividing the total number of bits (N) by
    /// the number of bits per block (`BitsPerBlock`). The result is rounded up
    /// to ensure that all bits fit into the required number of blocks.
    static constexpr std::size_t NumBlocks = (N + BitsPerBlock - 1) / BitsPerBlock;

    /// @brief The array holding the actual data of the BitVector.
    /// @details This array contains the blocks that hold the bits of the
    /// BitVector. It is sized according to `NumBlocks`, and each block is of
    /// type `BlockType` (e.g., `std::uint64_t`), allowing efficient bitwise
    /// operations.
    std::array<BlockType, NumBlocks> data = {};

    /// @brief Constructs a new BitVector (all bits set to 0).
    explicit BitVector()
        : data()
    {
        reset();
    }

    /// @brief Constructs a BitVector and initializes it with a given integral value.
    /// @param value The initial value.
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    explicit BitVector(T value)
    {
        reset();
        for (std::size_t i = 0; i < N; ++i) {
            if (value & 1) {
                set(i);
            }
            value >>= 1;
        }
    }

    /// @brief Constructs a BitVector based on a string representation (e.g., "1010111").
    /// @param str The input string.
    explicit BitVector(const std::string &str)
    {
        // Clear all bits.
        reset();
        std::size_t len = str.length();
        for (std::size_t i = 0; i < len && i < N; ++i) {
            // Set the bit.
            if (str[len - 1 - i] == '1') {
                set(i);
            } else if (str[len - 1 - i] != '0') {
                throw std::invalid_argument("BitVector string must contain only '0' and '1'");
            }
        }
    }

    /// @brief Copies another BitVector into this one, resizing if necessary.
    /// @tparam N2 The size of the `other` BitVector.
    /// @param other The other BitVector.
    template <std::size_t N2>
    explicit BitVector(const BitVector<N2> &other)
        : data{}
    {
        // Determine the number of full blocks to copy (using the smaller of the two block counts).
        constexpr std::size_t minBlocks = std::min(NumBlocks, BitVector<N2>::NumBlocks);

        // Copy full blocks, but ensure the bitwise copy works even with different BlockTypes.
        for (std::size_t i = 0; i < minBlocks; ++i) {
            // Extract bits from the other vector's block and place them in the corresponding block of this vector
            data[i] = static_cast<BlockType>(other.data[i]);
        }

        // Handle extra bits if N2 < N, we need to clear any bits beyond the valid part in this vector
        if constexpr (N2 < N) {
            // Ensure extra bits are cleared, as they don't exist in the source.
            trim();
        }

        // Handle the case where the source vector has more blocks but fewer bits
        if constexpr (N2 > N) {
            // For the case where source has more blocks but this vector is smaller, clear excess bits
            for (std::size_t i = N; i < NumBlocks; ++i) {
                data[i] = 0;
            }
        }
    }

    /// @brief Copy constructor.
    /// @param other The other instance to copy.
    BitVector(const BitVector &other) = default;

    /// @brief Copy assignment operator.
    /// @param other The other instance to copy.
    /// @return A reference to this instance.
    auto operator=(const BitVector &other) -> BitVector & = default;

    /// @brief Move constructor.
    /// @param other The other instance to move.
    BitVector(BitVector &&other) noexcept = default;

    /// @brief Move assignment operator.
    /// @param other The other instance to move.
    /// @return A reference to this instance.
    auto operator=(BitVector &&other) noexcept -> BitVector & = default;

    /// @brief Virtual destructor.
    virtual ~BitVector() = default;

    /// @brief Returns a bitvector of all ones.
    /// @return A bitvector of all 1s.
    static auto ones() -> BitVector<N>
    {
        BitVector<N> result;
        // Set all bits to 1.
        result.data.fill(static_cast<BlockType>(~0U));
        result.trim();
        return result;
    }

    /// @brief Returns a bitvector of all zeros.
    /// @return A bitvector of all 0s.
    static auto zeros() -> BitVector<N> { return BitVector<N>(); }

    /// @brief Sets all bits to 1.
    /// @return A reference to the modified BitVector.
    auto set() -> BitVector<N> &
    {
        // Set all bits in each block.
        data.fill(static_cast<BlockType>(~0U));
        // Ensure extra bits beyond N are cleared.
        trim();
        return *this;
    }

    /// @brief Sets a bit at a given position.
    /// @param pos The position to set.
    /// @return A reference to the modified BitVector.
    auto set(std::size_t pos) -> BitVector<N> &
    {
        if (pos >= N) {
            throw std::out_of_range("Bit position out of range.");
        }
        // Get the block index and bit position for the given position.
        std::size_t block = get_block_index(pos);
        std::size_t bit   = get_bit_position(pos);

        // Set the bit at the calculated position.
        data[block] |= static_cast<BlockType>(BlockType(1) << bit);

        return *this;
    }

    /// @brief Sets every bit to false.+

    /// @return A reference to the modified BitVector.
    auto reset() -> BitVector<N> &
    {
        // Set each block to all zeros.
        data.fill(0);
        return *this;
    }

    /// @brief Sets the given bit to false.
    /// @param pos The position to reset.
    /// @return A reference to the modified BitVector.
    auto reset(std::size_t pos) -> BitVector<N> &
    {
        if (pos >= N) {
            throw std::out_of_range("Bit position out of range");
        }
        // Get the block index and bit position for the given position.
        std::size_t block = get_block_index(pos);
        std::size_t bit   = get_bit_position(pos);

        // Reset the bit at the calculated position.
        data[block] = static_cast<BlockType>(data[block] & ~(1U << bit));
        return *this;
    }

    /// @brief Sets or clears the sign bit (MSB).
    /// @param value `true` to set the sign bit (negative), `false` to clear it (positive).
    /// @return A reference to the modified BitVector.
    auto set_sign(bool value) -> BitVector<N> &
    {
        // Set MSB to 1 (negative).
        if (value) {
            return set(N - 1);
        }
        // Clear MSB to 0 (positive).
        return reset(N - 1);
    }

    /// @brief Flips every bit.
    /// @return A reference to the modified BitVector.
    auto flip() -> BitVector<N> &
    {
        for (auto &block : data) {
            block = static_cast<BlockType>(~block);
        }
        trim();
        return *this;
    }

    /// @brief Flips a given bit.
    /// @param pos The position to flip.
    /// @return A reference to the modified BitVector.
    auto flip(std::size_t pos) -> BitVector<N> &
    {
        if (pos >= N) {
            throw std::out_of_range("BitVector index out of range");
        }
        // Get the block index and bit position for the given position.
        std::size_t block = get_block_index(pos);
        std::size_t bit   = get_bit_position(pos);

        // Flip the bit at the calculated position.
        data[block] = static_cast<BlockType>(data[block] ^ (1 << bit));
        return *this;
    }

    /// @brief Trims extra bits beyond N (in the last block).
    /// @return A reference to the modified BitVector.
    auto trim() -> BitVector<N> &
    {
        constexpr std::size_t extra_bits = (NumBlocks * BitsPerBlock) - N;
        // Ensure no invalid shifts.
        if constexpr (extra_bits > 0 && NumBlocks > 0) {
            data[NumBlocks - 1] &= ((BlockType(1) << (BitsPerBlock - extra_bits)) - 1);
        }
        return *this;
    }

    /// @brief Returns the size of the bit-vector.
    /// @return the size of the bitvector.
    constexpr auto size() const noexcept -> std::size_t { return N; }

    /// @brief Returns the number of bits which are set.
    /// @return the number of bits which are set.
    auto count() const -> std::size_t
    {
        std::size_t result = 0;
        for (const auto &block : data) {
            result += detail::popcount(block);
        }
        return result;
    }

    /// @brief Tests whether all the bits in the BitVector are set to 1.
    /// @return `true` if all bits are 1, otherwise `false`.
    auto all() const -> bool
    {
        // Check all blocks except the last one.
        for (std::size_t i = 0; i < NumBlocks - 1; ++i) {
            if (data[i] != static_cast<BlockType>(~0U)) {
                return false;
            }
        }
        // For the last block, handle excess bits.
        if (data[NumBlocks - 1] == static_cast<BlockType>(~0U)) {
            return true;
        }
        // For the last block, handle excess bits.
        auto extra_bits = static_cast<BlockType>((NumBlocks * BitsPerBlock) - N);
        // Create a mask for the last block's valid bits and check.
        auto mask       = static_cast<BlockType>(1 << (BitsPerBlock - extra_bits)) - 1;
        return (data[NumBlocks - 1] & mask) == mask;
    }

    /// @brief Tests whether any bit in the BitVector is set to 1.
    /// @return `true` if at least one bit is set to 1, otherwise `false`.
    auto any() const -> bool
    {
        for (const auto &block : data) {
            if (block != 0) {
                return true;
            }
        }
        return false;
    }

    /// @brief Tests whether all the bits in the BitVector are set to 0.
    /// @return `true` if all bits are set to 0, otherwise `false`.
    auto none() const -> bool { return !any(); }

    /// @brief Returns the sign bit (MSB).
    /// @return `true` if the sign bit is 1 (negative in two's complement), `false` otherwise.
    auto sign() const -> bool
    {
        // Check the most significant bit (MSB).
        return at(N - 1);
    }

    /// @brief Swaps two bits at the given positions.
    /// @param lhs The position of the first bit to swap.
    /// @param rhs The position of the second bit to swap.
    /// @return A reference to the modified BitVector.
    auto swap(std::size_t lhs, std::size_t rhs) noexcept -> BitVector<N> &
    {
        try {
            if ((lhs >= N) || (rhs >= N)) {
                throw std::out_of_range("BitVector index out of range");
            }
            bool bit_lhs = at(lhs);
            bool bit_rhs = at(rhs);
            // Swap the bits if they differ.
            if (bit_lhs != bit_rhs) {
                if (bit_lhs) {
                    set(rhs);   // Set the rhs bit to 1.
                    reset(lhs); // Reset the lhs bit to 0.
                } else {
                    set(lhs);   // Set the lhs bit to 1.
                    reset(rhs); // Reset the rhs bit to 0.
                }
            }
        } catch (const std::out_of_range &e) {
            (void)e;
        }
        return *this;
    }

    /// @brief Reverses the bits in the specified range by swapping bits.
    /// @details This function reverses the order of the bits between the
    /// `start` and `end` positions, inclusive, by iteratively swapping the
    /// bits. It modifies the BitVector in place.
    /// @param start The position of the first bit in the range to reverse.
    /// @param end The position of the last bit in the range to reverse.
    /// @return A reference to the modified BitVector.
    auto swap_range(std::size_t start, std::size_t end) -> BitVector<N> &
    {
        while (start < end) {
            // Swap the bits at the start and end positions.
            swap(start, end);
            ++start;
            --end;
        }
        return *this;
    }

    /// @brief Copies rhs into this BitVector.
    /// @tparam N2 Size of the source BitVector.
    /// @param rhs The BitVector to copy from.
    /// @return A reference to the modified BitVector.
    template <std::size_t N2>
    auto assign(const BitVector<N2> &rhs) -> BitVector<N> &
    {
        reset(); // Clear all bits before copying
        std::size_t min_size = std::min(N, N2);
        for (std::size_t i = 0; i < min_size; ++i) {
            if (rhs.at(i)) {
                set(i); // Set bit if rhs[i] is 1
            }
        }
        return *this;
    }

    /// @brief Copies rhs into this BitVector, iterating from left to right.
    /// @tparam N2 The size of the `rhs` BitVector.
    /// @param rhs The BitVector to copy from.
    /// @return A reference to the modified BitVector.
    template <std::size_t N2>
    auto rassign(const BitVector<N2> &rhs) -> BitVector<N> &
    {
        reset(); // Clear all bits before copying
        std::size_t min_size = std::min(N, N2);
        for (std::size_t i = 0; i < min_size; ++i) {
            if (rhs.at(N2 - i - 1)) {
                set(N - i - 1); // Set bit if rhs[N2 - i - 1] is 1
            }
        }
        return *this;
    }

    /// @brief Returns the bit at the given position.
    /// @param pos The bit position.
    /// @return True if the bit is set, false otherwise.
    auto at(std::size_t pos) const -> bool
    {
        if (pos >= N) {
            throw std::out_of_range("Accessing values outside bitvector");
        }
        // Get the block index and bit position for the given position.
        std::size_t block = get_block_index(pos);
        std::size_t bit   = get_bit_position(pos);

        // Return the bit at the calculated position.
        return (data[block] & (BlockType(1) << bit)) != 0;
    }

    /// @brief Returns a modifiable reference-like proxy to a bit.
    /// @param pos The bit position.
    /// @return A BitReference object allowing modification of the bit.
    auto at(std::size_t pos) -> detail::BitReference<BlockType>
    {
        if (pos >= N) {
            throw std::out_of_range("Accessing values outside bitvector");
        }
        // Get the block index and bit position for the given position.
        std::size_t block = get_block_index(pos);
        std::size_t bit   = get_bit_position(pos);

        // Return the bit reference at the calculated position.
        return detail::BitReference<BlockType>(data[block], bit);
    }

    /// @brief Slices the BitVector from the specified start to end bit position.
    ///
    /// This function returns a new BitVector that contains the bits between the
    /// start and end indices (inclusive). It is a non-destructive operation
    /// that preserves the original BitVector while returning a subset of its bits.
    ///
    /// @tparam Start The index of the first bit to include in the slice (inclusive).
    /// @tparam End The index of the last bit to include in the slice (inclusive).
    /// @return A new BitVector containing the sliced bits from the original BitVector.
    template <std::size_t Start, std::size_t End>
    auto slice() const -> BitVector<End - Start + 1>
    {
        static_assert(Start < End, "Start index must be less than End index.");
        static_assert(End <= N, "End index must be within the size of the BitVector.");

        BitVector<End - Start + 1> result;
        for (std::size_t i = 0; i < End - Start + 1; ++i) {
            result[i] = this->at(Start + i);
        }
        return result;
    }

    /// @brief Copies the contents of the provided BitVector into this BitVector.
    /// @tparam N2 The size of the `rhs` BitVector.
    /// @param rhs The BitVector to copy from.
    /// @return A reference to this BitVector (the destination).
    template <std::size_t N2>
    auto operator=(const BitVector<N2> &rhs) -> BitVector<N> &
    {
        // Clear all bits.
        reset();
        for (std::size_t i = 0; i < std::min(N, N2); ++i) {
            if (rhs[i]) {
                set(i);
            }
        }
        return *this;
    }

    /// @brief Copies the string into this BitVector (e.g., "1010111").
    /// @param str The input string.
    /// @return A reference to this BitVector (the destination).
    auto operator=(const std::string &str) -> BitVector<N> &
    {
        // Clear all bits.
        reset();
        std::size_t len = str.length();
        for (std::size_t i = 0; i < len && i < N; ++i) {
            // Set the bit.
            if (str[len - 1 - i] == '1') {
                set(i);
            } else if (str[len - 1 - i] != '0') {
                throw std::invalid_argument("BitVector string must contain only '0' and '1'");
            }
        }
        return *this;
    }

    /// @brief Transforms an integer `rhs` into a BitVector.
    /// @param rhs The integer to assign.
    /// @return A reference to the modified BitVector.
    auto operator=(std::size_t rhs) -> BitVector<N> &
    {
        // Clear all bits.
        reset();
        for (std::size_t i = 0; i < N; ++i) {
            // Set bit if `rhs` has a 1 at position i.
            if ((rhs & 1U) != 0U) {
                set(i);
            }
            // Move to the next bit.
            rhs >>= 1;
        }
        return *this;
    }

    /// @brief Returns the bit at the given position using array indexing.
    /// @param pos The bit position.
    /// @return True if the bit is set, false otherwise.
    auto operator[](std::size_t pos) const -> bool { return at(pos); }

    /// @brief Returns a modifiable reference-like proxy to a bit using array indexing.
    /// @param pos The bit position.
    /// @return A BitReference object allowing modification of the bit.
    auto operator[](std::size_t pos) -> detail::BitReference<BlockType> { return at(pos); }

    /// @brief Returns the bit at the given position using array indexing.
    /// @param pos The bit position.
    /// @return True if the bit is set, false otherwise.
    auto operator()(std::size_t pos) const -> bool { return at(pos); }

    /// @brief Returns a modifiable reference-like proxy to a bit using array indexing.
    /// @param pos The bit position.
    /// @return A BitReference object allowing modification of the bit.
    auto operator()(std::size_t pos) -> detail::BitReference<BlockType> { return at(pos); }

    /// @brief Converts the BitVector to a signed or unsigned integer.
    /// @tparam T The target integer type.
    /// @return The integer representation of the BitVector.
    template <typename T = std::size_t>
    constexpr auto to_number() const -> T
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        T result = 0;
        for (std::size_t i = 0; i < N; ++i) {
            if (at(i)) {
                result |= (T(1) << i);
            }
        }
        // If the sign bit (MSB) is set and T is signed, apply two’s complement
        // conversion.
        if constexpr (std::is_signed<T>::value) {
            if (sign()) {
                result -= (T(1) << N);
            }
        }
        return result;
    }

    /// @brief Converts the BitVector to a string.
    /// @param split if true, it will add spaces between blocks based on BitsPerBlock.
    /// @return The binary string representing the bitvector.
    auto to_string(bool split = false) const -> std::string
    {
        std::string str;
        for (std::size_t i = 0; i < N; ++i) {
            if (split && (i > 0) && ((i % BitsPerBlock) == 0)) {
                str.push_back(' ');
            }
            str.push_back(at(N - 1 - i) ? '1' : '0');
        }
        return str;
    }

    /// @brief Gets the block index for a given bit position.
    /// @param pos The bit position in the BitVector.
    /// @return The index of the block containing the bit.
    auto get_block_index(std::size_t pos) const -> std::size_t { return pos / BitsPerBlock; }

    /// @brief Gets the bit position within a block for a given bit position.
    /// @param pos The bit position in the BitVector.
    /// @return The position of the bit within its block.
    auto get_bit_position(std::size_t pos) const -> std::size_t { return pos % BitsPerBlock; }
};

} // namespace bvlib
