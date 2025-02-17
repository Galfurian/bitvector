/// @file test_shift.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests shift operators.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include <cassert>
#include <iostream>

#include "bvlib/math.hpp"

// ============================================================================
// shift_left() TESTS
// ============================================================================

/// @brief Tests for `shift_left` function.
void test_shift_left()
{
    // ========================================================================
    // Test cases with N = 8 (BitVector<8>)
    // ========================================================================

    // Test case 1: Shift by 0, no change
    bvlib::BitVector<8> bv8_0("11001100");
    bv8_0 = bvlib::detail::shift_left(bv8_0, 0);
    assert(bv8_0.to_string() == "11001100" && "Shift by 0 should not change the BitVector");

    // Test case 2: Shift by 1
    bvlib::BitVector<8> bv8_1("11001100");
    bv8_1 = bvlib::detail::shift_left(bv8_1, 1);
    assert(bv8_1.to_string() == "10011000" && "Shift left by 1 should shift bits left and add 0");

    // Test case 3: Shift by 7 (full shift)
    bvlib::BitVector<8> bv8_2("11001100");
    bv8_2 = bvlib::detail::shift_left(bv8_2, 7);
    assert(bv8_2.to_string() == "00000000" && "Shift left by 7 should move leftmost bit to the MSB");

    // Test case 4: Shift by 8 (overflow, reset to all zeros)
    bvlib::BitVector<8> bv8_3("11001100");
    bv8_3 = bvlib::detail::shift_left(bv8_3, 8);
    assert(bv8_3.to_string() == "00000000" && "Shift left by 8 should reset all bits to 0");

    // ========================================================================
    // Test cases with N = 16 (BitVector<16>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<16> bv16_0("1100110011001100");
    bv16_0 = bvlib::detail::shift_left(bv16_0, 0);
    assert(bv16_0.to_string() == "1100110011001100" && "Shift by 0 should not change the BitVector");

    // Test case 2: Shift by 1
    bvlib::BitVector<16> bv16_1("1100110011001100");
    bv16_1 = bvlib::detail::shift_left(bv16_1, 1);
    assert(bv16_1.to_string() == "1001100110011000" && "Shift left by 1 should shift bits left and add 0");

    // Test case 3: Shift by 15
    bvlib::BitVector<16> bv16_2("1100110011001101");
    bv16_2 = bvlib::detail::shift_left(bv16_2, 15);
    assert(bv16_2.to_string() == "1000000000000000" && "Shift left by 15 should move leftmost bit to the MSB");

    // Test case 4: Shift by 16 (overflow, reset to all zeros)
    bvlib::BitVector<16> bv16_3("1100110011001100");
    bv16_3 = bvlib::detail::shift_left(bv16_3, 16);
    assert(bv16_3.to_string() == "0000000000000000" && "Shift left by 16 should reset all bits to 0");

    // ========================================================================
    // Test cases with N = 32 (BitVector<32>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<32> bv32_0("11001100110011001100110011001100");
    bv32_0 = bvlib::detail::shift_left(bv32_0, 0);
    assert(bv32_0.to_string() == "11001100110011001100110011001100" && "Shift by 0 should not change the BitVector");

    // Test case 2: Shift by 1
    bvlib::BitVector<32> bv32_1("11001100110011001100110011001100");
    bv32_1 = bvlib::detail::shift_left(bv32_1, 1);
    assert(
        bv32_1.to_string() == "10011001100110011001100110011000" && "Shift left by 1 should shift bits left and add 0");

    // Test case 3: Shift by 31 (maximum left shift)
    bvlib::BitVector<32> bv32_2("11001100110011001100110011001101");
    bv32_2 = bvlib::detail::shift_left(bv32_2, 31);
    assert(
        bv32_2.to_string() == "10000000000000000000000000000000" &&
        "Shift left by 31 should move leftmost bit to the MSB");

    // Test case 4: Shift by 32 (overflow, reset to all zeros)
    bvlib::BitVector<32> bv32_3("11001100110011001100110011001100");
    bv32_3 = bvlib::detail::shift_left(bv32_3, 32);
    assert(bv32_3.to_string() == "00000000000000000000000000000000" && "Shift left by 32 should reset all bits to 0");

    // ========================================================================
    // Test cases for shift larger than bit size (overflow)
    // ========================================================================

    // Test case 1: Shift by more than bit vector size, it should be reset to 0
    bvlib::BitVector<8> bv8_large_shift("11001100");
    bv8_large_shift = bvlib::detail::shift_left(bv8_large_shift, 10);
    assert(bv8_large_shift.to_string() == "00000000" && "Shift left by a large number should reset all bits to 0");

    // Test case 2: Shift by N + 1, reset to 0
    bvlib::BitVector<16> bv16_large_shift("1100110011001100");
    bv16_large_shift = bvlib::detail::shift_left(bv16_large_shift, 17);
    assert(bv16_large_shift.to_string() == "0000000000000000" && "Shift left by N + 1 should reset all bits to 0");
}

// ============================================================================
// shift_right() TESTS
// ============================================================================

/// @brief Tests for `shift_right` function.
void test_shift_right()
{
    // ========================================================================
    // Test cases with N = 8 (BitVector<8>)
    // ========================================================================

    // Test case 1: Shift by 0, no change
    bvlib::BitVector<8> bv8_0("11001100");
    bv8_0 = bvlib::detail::shift_right(bv8_0, 0);
    assert(bv8_0.to_string() == "11001100" && "Shift by 0 should not change the BitVector");

    // Test case 2: Shift by 1
    bvlib::BitVector<8> bv8_1("11001100");
    bv8_1 = bvlib::detail::shift_right(bv8_1, 1);
    assert(bv8_1.to_string() == "01100110" && "Shift right by 1 should shift bits right and add 0");

    // Test case 3: Shift by 7 (full shift)
    bvlib::BitVector<8> bv8_2("11001100");
    bv8_2 = bvlib::detail::shift_right(bv8_2, 7);
    assert(bv8_2.to_string() == "00000001" && "Shift right by 7 should move rightmost bit to the LSB");

    // Test case 4: Shift by 8 (overflow, reset to all zeros)
    bvlib::BitVector<8> bv8_3("11001100");
    bv8_3 = bvlib::detail::shift_right(bv8_3, 8);
    assert(bv8_3.to_string() == "00000000" && "Shift right by 8 should reset all bits to 0");

    // ========================================================================
    // Test cases with N = 16 (BitVector<16>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<16> bv16_0("1100110011001100");
    bv16_0 = bvlib::detail::shift_right(bv16_0, 0);
    assert(bv16_0.to_string() == "1100110011001100" && "Shift by 0 should not change the BitVector");

    // Test case 2: Shift by 1
    bvlib::BitVector<16> bv16_1("1100110011001100");
    bv16_1 = bvlib::detail::shift_right(bv16_1, 1);
    assert(bv16_1.to_string() == "0110011001100110" && "Shift right by 1 should shift bits right and add 0");

    // Test case 3: Shift by 15
    bvlib::BitVector<16> bv16_2("1100110011001101");
    bv16_2 = bvlib::detail::shift_right(bv16_2, 15);
    assert(bv16_2.to_string() == "0000000000000001" && "Shift right by 15 should move rightmost bit to the LSB");

    // Test case 4: Shift by 16 (overflow, reset to all zeros)
    bvlib::BitVector<16> bv16_3("1100110011001100");
    bv16_3 = bvlib::detail::shift_right(bv16_3, 16);
    assert(bv16_3.to_string() == "0000000000000000" && "Shift right by 16 should reset all bits to 0");

    // ========================================================================
    // Test cases with N = 32 (BitVector<32>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<32> bv32_0("11001100110011001100110011001100");
    bv32_0 = bvlib::detail::shift_right(bv32_0, 0);
    assert(bv32_0.to_string() == "11001100110011001100110011001100" && "Shift by 0 should not change the BitVector");

    // Test case 2: Shift by 1
    bvlib::BitVector<32> bv32_1("11001100110011001100110011001100");
    bv32_1 = bvlib::detail::shift_right(bv32_1, 1);
    assert(
        bv32_1.to_string() == "01100110011001100110011001100110" &&
        "Shift right by 1 should shift bits right and add 0");

    // Test case 3: Shift by 31 (maximum right shift)
    bvlib::BitVector<32> bv32_2("11001100110011001100110011001101");
    bv32_2 = bvlib::detail::shift_right(bv32_2, 31);
    assert(
        bv32_2.to_string() == "00000000000000000000000000000001" &&
        "Shift right by 31 should move rightmost bit to the LSB");

    // Test case 4: Shift by 32 (overflow, reset to all zeros)
    bvlib::BitVector<32> bv32_3("11001100110011001100110011001100");
    bv32_3 = bvlib::detail::shift_right(bv32_3, 32);
    assert(bv32_3.to_string() == "00000000000000000000000000000000" && "Shift right by 32 should reset all bits to 0");

    // ========================================================================
    // Test cases for shift larger than bit size (overflow)
    // ========================================================================

    // Test case 1: Shift by more than bit vector size, it should be reset to 0
    bvlib::BitVector<8> bv8_large_shift("11001100");
    bv8_large_shift = bvlib::detail::shift_right(bv8_large_shift, 10);
    assert(bv8_large_shift.to_string() == "00000000" && "Shift right by a large number should reset all bits to 0");

    // Test case 2: Shift by N + 1, reset to 0
    bvlib::BitVector<16> bv16_large_shift("1100110011001100");
    bv16_large_shift = bvlib::detail::shift_right(bv16_large_shift, 17);
    assert(bv16_large_shift.to_string() == "0000000000000000" && "Shift right by N + 1 should reset all bits to 0");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests." << std::endl;

    test_shift_left();
    std::cout << "    ✅ All `shift_left` tests completed!\n";

    test_shift_right();
    std::cout << "    ✅ All `shift_right` tests completed!\n";

    std::cout << "✅ All function tests passed!" << std::endl;
    return 0;
}
