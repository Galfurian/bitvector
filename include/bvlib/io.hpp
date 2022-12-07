/// @file io.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Input and output stream operators.

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

template <std::size_t N>
std::ostream &operator<<(std::ostream &lhs, const bvlib::BitVector<N> &rhs)
{
    lhs << rhs.template to_number<long>();
    return lhs;
}

template <std::size_t N>
std::stringstream &operator<<(std::stringstream &lhs, const bvlib::BitVector<N> &rhs)
{
    lhs << rhs.template to_number<long>();
    return lhs;
}

template <std::size_t N>
std::ifstream &operator>>(std::ifstream &lhs, bvlib::BitVector<N> &rhs)
{
    long value;
    lhs >> value;
    rhs = value;
    return lhs;
}

template <std::size_t N>
std::stringstream &operator>>(std::stringstream &lhs, bvlib::BitVector<N> &rhs)
{
    long value;
    lhs >> value;
    rhs = value;
    return lhs;
}
