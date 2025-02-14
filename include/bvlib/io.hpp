/// @file io.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Input and output stream operators.

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "bvlib/bitvector.hpp"

/// @brief Overloads the stream insertion operator for BitVector to an output stream (std::ostream).
/// @param lhs The output stream to which the BitVector will be written.
/// @param rhs The BitVector to be written to the output stream.
/// @return The output stream after the BitVector has been written.
template <std::size_t N>
auto operator<<(std::ostream &lhs, const bvlib::BitVector<N> &rhs) -> std::ostream &
{
    lhs << rhs.template to_number<long>();
    return lhs;
}

/// @brief Overloads the stream insertion operator for BitVector to a string stream (std::stringstream).
/// @param lhs The string stream to which the BitVector will be written.
/// @param rhs The BitVector to be written to the string stream.
/// @return The string stream after the BitVector has been written.
template <std::size_t N>
auto operator<<(std::stringstream &lhs, const bvlib::BitVector<N> &rhs) -> std::stringstream &
{
    lhs << rhs.template to_number<long>();
    return lhs;
}

/// @brief Overloads the stream extraction operator to read a BitVector from an input file stream (std::ifstream).
/// @param lhs The input file stream from which the BitVector will be read.
/// @param rhs The BitVector to be read from the input file stream.
/// @return The input file stream after reading the BitVector.
template <std::size_t N>
auto operator>>(std::ifstream &lhs, bvlib::BitVector<N> &rhs) -> std::ifstream &
{
    long value = 0;
    lhs >> value;
    rhs = value;
    return lhs;
}

/// @brief Overloads the stream extraction operator to read a BitVector from a string stream (std::stringstream).
/// @param lhs The string stream from which the BitVector will be read.
/// @param rhs The BitVector to be read from the string stream.
/// @return The string stream after reading the BitVector.
template <std::size_t N>
auto operator>>(std::stringstream &lhs, bvlib::BitVector<N> &rhs) -> std::stringstream &
{
    long value = 0;
    lhs >> value;
    rhs = value;
    return lhs;
}
