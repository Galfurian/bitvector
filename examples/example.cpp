/// @file example.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief A simple example.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include "bvlib/bitvector.hpp"
#include "bvlib/io.hpp"
#include "bvlib/math.hpp"

#include <iomanip>
#include <random>
#include <vector>

int main(int, char *[])
{
    bvlib::BitVector<6> a(0);
    for (std::size_t i = 0; i <= 0xF; ++i) {
        std::cout << a.to_string() << " : ";
        std::cout << std::setw(8) << std::right << a.to_number<int>() << " : ";
        std::cout << std::setw(8) << std::right << bvlib::detail::most_significant_bit(a) << "\n";
        a++;
    }
    return 0;
}