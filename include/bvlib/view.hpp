/// @file view.hpp
/// @brief Contains a class to create views of bitvectors.
/// @copyright Copyright (c) 2025

#pragma once

#include "bvlib/bitvector.hpp"

namespace bvlib
{

/// @brief A view class for a portion of a BitVector, specified by a bit range [Start, End).
/// @tparam Start The starting bit index of the view (inclusive).
/// @tparam End The ending bit index of the view (exclusive).
/// @tparam N The total number of bits in the original BitVector.
template <std::size_t Start, std::size_t End, std::size_t N>
class View
{
private:
    static_assert(Start <= End, "Start must be less than or equal to End.");
    static_assert(End <= N, "End must be less than N.");

    /// @brief A reference to the original BitVector.
    std::reference_wrapper<bvlib::BitVector<N>> data;

public:
    /// @brief Defines the block type used for storing bits.
    using BlockType = typename bvlib::BitVector<N>::BlockType;

    /// @brief Number of blocks required to store N bits.
    static constexpr std::size_t NumBits = End - Start;

    /// @brief Constructs a View from a BitVector, given a bit range [Start, End).
    /// @details The constructor calculates which blocks the view covers and stores the relevant data.
    /// @param bit_vector The BitVector from which the view is created.
    View(bvlib::BitVector<N> &bit_vector)
        : data(bit_vector)
    {
        // Nothing to do.
    }

    /// @brief Accessor to get the value of the bit at position pos in the view.
    /// @param pos The position of the bit in the view's range.
    /// @return The value of the bit at the given position (true if set, false if not).
    auto at(std::size_t pos) const -> bool
    {
        if ((Start + pos) >= End) {
            throw std::out_of_range(
                "Accessing bit (" + std::to_string(Start + pos) + ") outside view (" + std::to_string(End) + ").");
        }
        return data.get().at(Start + pos);
    }

    /// @brief Accessor to get a reference to the bit at position pos in the view.
    /// @param pos The position of the bit in the view's range.
    /// @return A reference to the bit at the given position (true if set, false if not).
    auto at(std::size_t pos) -> detail::BitReference<BlockType>
    {
        if ((Start + pos) >= End) {
            throw std::out_of_range(
                "Accessing bit (" + std::to_string(Start + pos) + ") outside view (" + std::to_string(End) + ").");
        }
        return data.get().at(Start + pos);
    }

    /// @brief Accessor to get the value of the bit at position pos in the view.
    /// @param pos The position of the bit in the view's range.
    /// @return The value of the bit at the given position (true if set, false if not).
    auto operator[](std::size_t pos) const -> bool { return this->at(pos); }

    /// @brief Accessor to get a reference to the bit at position pos in the view.
    /// @param pos The position of the bit in the view's range.
    /// @return A reference to the bit at the given position (true if set, false if not).
    auto operator[](std::size_t pos) -> detail::BitReference<BlockType> { return this->at(pos); }

    /// @brief Accessor to get the value of the bit at position pos in the view.
    /// @param pos The position of the bit in the view's range.
    /// @return The value of the bit at the given position (true if set, false if not).
    auto operator()(std::size_t pos) const -> bool { return this->at(pos); }

    /// @brief Accessor to get a reference to the bit at position pos in the view.
    /// @param pos The position of the bit in the view's range.
    /// @return A reference to the bit at the given position (true if set, false if not).
    auto operator()(std::size_t pos) -> detail::BitReference<BlockType> { return this->at(pos); }

    /// @brief Assignment operator from BitVector to View.
    /// @param bit_vector The BitVector to assign from.
    /// @return A reference to the current View.
    template <std::size_t N2>
    auto operator=(const BitVector<N2> &bit_vector) -> View &
    {
        // Clear all bits.
        for (std::size_t i = 0; i < std::min(NumBits, N2); ++i) {
            at(i) = bit_vector[i];
        }
        return *this;
    }

    /// @brief Assigns the bits from one view to another.
    /// @tparam S2 The starting bit index of the source view (inclusive).
    /// @tparam E2 The ending bit index of the source view (exclusive).
    /// @tparam N2 The total number of bits in the source view.
    /// @param view The source view whose bits will be copied.
    /// @return The current view after the assignment.
    template <std::size_t S2, std::size_t E2, std::size_t N2>
    auto operator=(const View<S2, E2, N2> &view) -> View &
    {
        // Clear all bits.
        for (std::size_t i = 0; i < std::min(NumBits, View<S2, E2, N2>::NumBits); ++i) {
            at(i) = view[i];
        }
        return *this;
    }

    /// @brief Assigns a string to the view, setting bits according to the string's values.
    /// @param str The string to assign to the view.
    /// @return A reference to the current view after the assignment.
    auto operator=(const std::string &str) -> View &
    {
        // Clear all bits.
        for (std::size_t i = 0; i < std::min(NumBits, str.length()); ++i) {
            // Set the bit value, from least-significant to most-significant.
            at(i) = (str[str.length() - i - 1] == '1');
        }
        return *this;
    }

    /// @brief Converts the bit range in the View to a string.
    /// @param split if true, it will add spaces between blocks based on BitsPerBlock.
    /// @return A string representation of the bits in the range [Start, End).
    auto to_string(bool split = false) const -> std::string
    {
        std::string str;
        for (std::size_t pos = 0; pos < NumBits; ++pos) {
            if (split && (pos > 0) && ((pos % bvlib::BitVector<N>::BitsPerBlock) == 0)) {
                str.push_back(' ');
            }
            str.push_back(this->at(NumBits - pos - 1) ? '1' : '0');
        }
        return str;
    }
};

/// @brief Creates a view for a portion of a BitVector, specified by the range [Start, End).
/// @tparam Start The starting bit index (inclusive) of the view.
/// @tparam End The ending bit index (exclusive) of the view.
/// @tparam N The total number of bits in the original BitVector.
/// @param bit_vector The BitVector from which the view is created.
/// @return A view that references the specified range of bits in the BitVector.
template <std::size_t Start, std::size_t End, std::size_t N>
auto make_view(bvlib::BitVector<N> &bit_vector) -> bvlib::View<Start, End, N>
{
    return bvlib::View<Start, End, N>(bit_vector);
}

} // namespace bvlib
