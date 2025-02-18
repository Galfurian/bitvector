/// @file test_support.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests support functions.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include <cassert>
#include <iostream>

#include <bitset>

#include "bvlib/math.hpp"

// ============================================================================
// most_significant_bit() TESTS
// ============================================================================

/// @brief Tests the `most_significant_bit()` function to find the position of the most significant bit.
void test_most_significant_bit()
{
    bvlib::BitVector<32> bv1("00000000000000000000000000001010");
    assert(
        bvlib::detail::most_significant_bit(bv1) == 3 &&
        "most_significant_bit() should return correct position for bit 3");

    bvlib::BitVector<32> bv2("10000000000000000000000000000000");
    assert(
        bvlib::detail::most_significant_bit(bv2) == 31 &&
        "most_significant_bit() should return 31 for the most significant bit");

    bvlib::BitVector<32> bv3("00000000000000000000000000000000");
    assert(
        bvlib::detail::most_significant_bit(bv3) == 0 && "most_significant_bit() should return 0 if no bits are set");

    bvlib::BitVector<32> bv4("01000000000000000000000000000000");
    assert(
        bvlib::detail::most_significant_bit(bv4) == 30 &&
        "most_significant_bit() should return the correct position for bit 30");

    bvlib::BitVector<32> bv5("00000000000000010000000000000000");
    assert(
        bvlib::detail::most_significant_bit(bv5) == 16 &&
        "most_significant_bit() should return correct position for bit 17");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests." << std::endl;

    test_most_significant_bit();

    std::cout << "✅ All function tests passed!" << std::endl;
    return 0;
}
