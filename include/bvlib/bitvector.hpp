/// @file bitvector.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Base bitvector class.

#pragma once

#include <stdexcept>
#include <cstdint>
#include <climits>
#include <string>
#include <cmath>
#include <array>

namespace bvlib
{

namespace detail
{

/// @brief Counts the number of 1s in a 32-bit integer.
/// @param x The integer.
/// @return The number of set bits.
inline std::size_t popcount(std::uint32_t x)
{
    std::size_t count = 0;
    while (x) {
        x &= (x - 1); // Clears the lowest set bit
        count++;
    }
    return count;
}

/// @brief Counts the number of leading zeros in a 32-bit integer.
/// @param x The input number.
/// @return The number of leading zeros.
inline std::size_t count_leading_zeros(uint32_t x)
{
    // Special case: all bits are zero.
    if (x == 0) {
        return 32;
    }

    std::size_t count = 0;
    for (uint32_t mask = 1U << 31; (x & mask) == 0; mask >>= 1) {
        count++;
    }
    return count;
}

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
class BitReference {
private:
    /// @brief The block containing the bit being referenced.
    BlockType &_block;
    /// @brief The position of the bit within the block.
    std::size_t _pos;

public:
    /// @brief Constructs a BitReference for a specific block and bit position.
    /// @details
    /// This constructor initializes the reference with a block and a bit position.
    /// It allows modifying or accessing the bit at the specified position within the block.
    ///
    /// @param b The block that contains the bit.
    /// @param pos The position of the bit within the block.
    BitReference(BlockType &b, std::size_t pos)
        : _block(b), _pos(pos)
    {
        // Nothing to do
    }

    /// @brief Implicit conversion to bool to read the value of the bit.
    /// @details
    /// This operator allows using the `BitReference` in boolean contexts (e.g., in conditionals).
    /// It returns `true` if the bit at the specified position is set, `false` otherwise.
    ///
    /// @return `true` if the referenced bit is set, `false` otherwise.
    operator bool() const
    {
        return (_block & (BlockType(1) << _pos)) != 0;
    }

    /// @brief Assigns a value to the referenced bit.
    /// @details
    /// This operator allows modifying the bit at the specified position. If the value is `true`,
    /// the bit is set to 1. If the value is `false`, the bit is cleared (set to 0).
    ///
    /// @param value The value to assign to the referenced bit (`true` to set, `false` to clear).
    /// @return A reference to the current `BitReference` to allow chaining assignments.
    BitReference &operator=(bool value)
    {
        if (value) {
            _block |= (BlockType(1) << _pos); ///< Set the bit to 1.
        } else {
            _block &= ~(BlockType(1) << _pos); ///< Clear the bit (set to 0).
        }
        return *this;
    }
};

} // namespace detail

/// @brief Class which reproduces the behaviour of bit-vector.
/// @tparam N Lenght of the bit-vector
template <std::size_t N>
class BitVector {
public:
    using BlockType                           = std::uint32_t;
    static constexpr std::size_t BitsPerBlock = sizeof(BlockType) * CHAR_BIT;
    static constexpr std::size_t NumBlocks    = (N + BitsPerBlock - 1) / BitsPerBlock;

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
    /// @param other The other BitVector.
    template <std::size_t N2>
    explicit BitVector(const BitVector<N2> &other)
        : data{}
    {
        constexpr std::size_t minBlocks = std::min(NumBlocks, BitVector<N2>::NumBlocks);
        for (std::size_t i = 0; i < minBlocks; ++i) {
            data[i] = other.data[i];
        }

        // Ensure extra bits beyond N2 are cleared.
        if constexpr (N2 < N) {
            trim();
        }
    }

    virtual ~BitVector() = default;

    /// @brief Returns a bitvector of all ones.
    static inline BitVector<N> ones()
    {
        BitVector<N> result;
        result.data.fill(~BlockType(0)); // Set all bits to 1
        result.trim();
        return result;
    }

    /// @brief Returns a bitvector of all zeros.
    static inline BitVector<N> zeros()
    {
        return BitVector<N>();
    }

    /// @brief Sets all bits to 1.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &set()
    {
        // Set all bits in each block.
        data.fill(~BlockType(0));
        // Ensure extra bits beyond N are cleared.
        trim();
        return *this;
    }

    /// @brief Sets a bit at a given position.
    /// @param pos The position to set.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &set(std::size_t pos)
    {
        if (pos >= N) {
            throw std::out_of_range("Bit position out of range");
        }
        data[pos / BitsPerBlock] |= (BlockType(1) << (pos % BitsPerBlock));
        return *this;
    }

    /// @brief Sets every bit to false.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &reset()
    {
        // Set each block to all zeros.
        data.fill(0);
        return *this;
    }

    /// @brief Sets the given bit to false.
    /// @param pos The position to reset.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &reset(std::size_t pos)
    {
        if (pos >= N) {
            throw std::out_of_range("Bit position out of range");
        }
        // Ensure same indexing as `set(pos)`
        data[pos / BitsPerBlock] &= ~(BlockType(1) << (pos % BitsPerBlock));
        return *this;
    }

    /// @brief Sets or clears the sign bit (MSB).
    /// @param value `true` to set the sign bit (negative), `false` to clear it (positive).
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &set_sign(bool value)
    {
        // Set MSB to 1 (negative).
        if (value) {
            return set(N - 1);
        }
        // Clear MSB to 0 (positive).
        return reset(N - 1);
    }

    /// @brief Toggles a bit at a given position.
    /// @param pos The position to toggle.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &toggle(std::size_t pos)
    {
        if (pos >= N) {
            throw std::out_of_range("BitVector index out of range");
        }
        data[pos / BitsPerBlock] ^= (BlockType(1) << (pos % BitsPerBlock));
        return *this;
    }

    /// @brief Gets the value of a bit at a given position.
    /// @param pos The position to check.
    /// @return true if the bit is set, false otherwise.
    bool get(std::size_t pos) const
    {
        if (pos >= N) {
            throw std::out_of_range("BitVector index out of range");
        }
        return (data[pos / BitsPerBlock] & (BlockType(1) << (pos % BitsPerBlock))) != 0;
    }

    /// @brief Flips every bit.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &flip()
    {
        for (auto &block : data) {
            block = ~block;
        }
        trim();
        return *this;
    }

    /// @brief Flips a given bit.
    /// @param pos The position to flip.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &flip(std::size_t pos)
    {
        if (pos >= N) {
            throw std::out_of_range("BitVector index out of range");
        }
        data[pos / BitsPerBlock] ^= (BlockType(1) << (pos % BitsPerBlock));
        return *this;
    }

    /// @brief Trims extra bits beyond N (in the last block).
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &trim()
    {
        constexpr std::size_t extraBits = NumBlocks * BitsPerBlock - N;
        // Ensure no invalid shifts.
        if constexpr (extraBits > 0 && NumBlocks > 0) {
            constexpr BlockType mask = (BlockType(1) << (BitsPerBlock - extraBits)) - 1;
            data[NumBlocks - 1] &= mask;
        }
        return *this;
    }

    /// @brief Returns the size of the bit-vector.
    /// @return the size of the bitvector.
    inline constexpr std::size_t size() const noexcept
    {
        return N;
    }

    /// @brief Returns the number of bits which are set.
    /// @return the number of bits which are set.
    inline std::size_t count() const
    {
        std::size_t result = 0;
        for (const auto &block : data) {
            result += detail::popcount(block);
        }
        return result;
    }

    /// @brief Tests whether all the bits are on.
    inline bool all() const
    {
        for (std::size_t i = 0; i < NumBlocks - 1; ++i) {
            if (data[i] != ~BlockType(0))
                return false;
        }
        // Check the last block (handle excess bits)
        constexpr std::size_t extraBits = NumBlocks * BitsPerBlock - N;
        if (extraBits > 0) {
            BlockType mask = (BlockType(1) << (BitsPerBlock - extraBits)) - 1;
            return (data[NumBlocks - 1] & mask) == mask;
        }
        return data[NumBlocks - 1] == ~BlockType(0);
    }

    /// @brief Tests whether any of the bits are on.
    inline bool any() const
    {
        for (const auto &block : data) {
            if (block != 0)
                return true;
        }
        return false;
    }

    /// @brief Tests whether none of the bits are on.
    inline bool none() const
    {
        return !any();
    }

    /// @brief Returns the sign bit (MSB).
    /// @return `true` if the sign bit is 1 (negative in two's complement), `false` otherwise.
    inline bool sign() const
    {
        return get(N - 1); // Check the most significant bit (MSB)
    }

    /// @brief Performs two's complement (bitwise negation + manual addition).
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &two_complement()
    {
        flip(); // Invert all bits (bitwise negation)

        // Manually add 1 using bitwise logic.
        // Initial carry (since we're adding 1).
        bool carry = true;
        for (std::size_t i = 0; i < NumBlocks; ++i) {
            if (carry) {
                if (data[i] == ~BlockType(0)) {
                    // Overflow, propagate carry.
                    data[i] = 0;
                } else {
                    data[i] += 1;
                    // No more carry needed.
                    carry = false;
                }
            }
        }

        return *this;
    }

    /// @brief Swaps two bits at the given positions.
    /// @param lhs The position of the first bit to swap.
    /// @param rhs The position of the second bit to swap.
    /// @return A reference to the modified BitVector.
    inline BitVector<N> &swap(std::size_t lhs, std::size_t rhs)
    {
        if (lhs >= N || rhs >= N) {
            throw std::out_of_range("Bit position out of range");
        }
        bool bit_lhs = get(lhs);
        bool bit_rhs = get(rhs);
        // Swap the bits if they differ
        if (bit_lhs != bit_rhs) {
            if (bit_lhs) {
                set(rhs);   // Set the rhs bit to 1
                reset(lhs); // Reset the lhs bit to 0
            } else {
                set(lhs);   // Set the lhs bit to 1
                reset(rhs); // Reset the rhs bit to 0
            }
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
    BitVector<N> &swap_range(std::size_t start, std::size_t end)
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
    inline BitVector<N> &assign(const BitVector<N2> &rhs)
    {
        reset(); // Clear all bits before copying
        std::size_t min_size = std::min(N, N2);
        for (std::size_t i = 0; i < min_size; ++i) {
            if (rhs.get(i)) {
                set(i); // Set bit if rhs[i] is 1
            }
        }
        return *this;
    }

    /// @brief Copies rhs into this BitVector, iterating from left to right.
    /// @tparam N2 Size of the source BitVector.
    /// @param rhs The BitVector to copy from.
    /// @return A reference to the modified BitVector.
    template <std::size_t N2>
    inline BitVector<N> &rassign(const BitVector<N2> &rhs)
    {
        reset(); // Clear all bits before copying
        std::size_t min_size = std::min(N, N2);
        for (std::size_t i = 0; i < min_size; ++i) {
            if (rhs.get(N2 - i - 1)) {
                set(N - i - 1); // Set bit if rhs[N2 - i - 1] is 1
            }
        }
        return *this;
    }

    /// @brief Returns the bit at the given position.
    /// @param pos The bit position.
    /// @return True if the bit is set, false otherwise.
    inline bool at(std::size_t pos) const
    {
        if (pos >= N) {
            throw std::out_of_range("Accessing values outside bitvector");
        }
        return (data[pos / BitsPerBlock] & (BlockType(1) << (pos % BitsPerBlock))) != 0;
    }

    /// @brief Returns a modifiable reference-like proxy to a bit.
    /// @param pos The bit position.
    /// @return A BitReference object allowing modification of the bit.
    inline detail::BitReference<BlockType> at(std::size_t pos)
    {
        if (pos >= N) {
            throw std::out_of_range("Accessing values outside bitvector");
        }
        return detail::BitReference<BlockType>(data[pos / BitsPerBlock], pos % BitsPerBlock);
    }

    /// @brief Copies rhs into this BitVector.
    template <std::size_t N2>
    inline BitVector<N> &operator=(const BitVector<N2> &rhs)
    {
        // Clear all bits.
        reset();
        std::size_t min_size = std::min(N, N2);
        for (std::size_t i = 0; i < min_size; ++i) {
            if (rhs[i]) {
                set(i);
            }
        }
        return *this;
    }

    /// @brief Copies the string into this BitVector (e.g., "1010111").
    /// @param str The input string.
    inline BitVector<N> &operator=(const std::string &str)
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
    inline BitVector<N> &operator=(std::size_t rhs)
    {
        // Clear all bits.
        reset();
        for (std::size_t i = 0; i < N; ++i) {
            // Set bit if `rhs` has a 1 at position i.
            if (rhs & 1) {
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
    inline bool operator[](std::size_t pos) const
    {
        return at(pos);
    }

    /// @brief Returns a modifiable reference-like proxy to a bit using array indexing.
    /// @param pos The bit position.
    /// @return A BitReference object allowing modification of the bit.
    inline detail::BitReference<BlockType> operator[](std::size_t pos)
    {
        return at(pos);
    }

    /// @brief Converts the BitVector to a signed or unsigned integer.
    /// @tparam T The target integer type.
    /// @return The integer representation of the BitVector.
    template <typename T>
    constexpr inline T to_number() const
    {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        T result = 0;
        for (std::size_t i = 0; i < N; ++i) {
            if (get(i)) {
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
    std::string to_string() const
    {
        std::string str;
        for (std::size_t i = 0; i < N; ++i) {
            str.push_back(get(N - 1 - i) ? '1' : '0');
        }
        return str;
    }
};

} // namespace bvlib
