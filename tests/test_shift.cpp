/// @file test_shift.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests shift operators.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include "support.hpp"

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
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 0); }, bv8_0, "11001100");

    // Test case 2: Shift by 1
    bvlib::BitVector<8> bv8_1("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 1); }, bv8_1, "10011000");

    // Test case 3: Shift by 7 (full shift)
    bvlib::BitVector<8> bv8_2("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 7); }, bv8_2, "00000000");

    // Test case 4: Shift by 8 (overflow, reset to all zeros)
    bvlib::BitVector<8> bv8_3("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 8); }, bv8_3, "00000000");

    // ========================================================================
    // Test cases with N = 16 (BitVector<16>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<16> bv16_0("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 0); }, bv16_0, "1100110011001100");

    // Test case 2: Shift by 1
    bvlib::BitVector<16> bv16_1("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 1); }, bv16_1, "1001100110011000");

    // Test case 3: Shift by 15
    bvlib::BitVector<16> bv16_2("1100110011001101");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 15); }, bv16_2, "1000000000000000");

    // Test case 4: Shift by 16 (overflow, reset to all zeros)
    bvlib::BitVector<16> bv16_3("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 16); }, bv16_3, "0000000000000000");

    // ========================================================================
    // Test cases with N = 32 (BitVector<32>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<32> bv32_0("11001100110011001100110011001100");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_left(value, 0); }, bv32_0, "11001100110011001100110011001100");

    // Test case 2: Shift by 1
    bvlib::BitVector<32> bv32_1("11001100110011001100110011001100");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_left(value, 1); }, bv32_1, "10011001100110011001100110011000");

    // Test case 3: Shift by 31 (maximum left shift)
    bvlib::BitVector<32> bv32_2("11001100110011001100110011001101");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_left(value, 31); }, bv32_2, "10000000000000000000000000000000");

    // Test case 4: Shift by 32 (overflow, reset to all zeros)
    bvlib::BitVector<32> bv32_3("11001100110011001100110011001100");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_left(value, 32); }, bv32_3, "00000000000000000000000000000000");

    // ========================================================================
    // Test cases for shift larger than bit size (overflow)
    // ========================================================================

    // Test case 1: Shift by more than bit vector size, it should be reset to 0
    bvlib::BitVector<8> bv8_large_shift("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 10); }, bv8_large_shift, "00000000");

    // Test case 2: Shift by N + 1, reset to 0
    bvlib::BitVector<16> bv16_large_shift("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_left(value, 17); }, bv16_large_shift, "0000000000000000");
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
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 0); }, bv8_0, "11001100");

    // Test case 2: Shift by 1
    bvlib::BitVector<8> bv8_1("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 1); }, bv8_1, "01100110");

    // Test case 3: Shift by 7 (full shift)
    bvlib::BitVector<8> bv8_2("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 7); }, bv8_2, "00000001");

    // Test case 4: Shift by 8 (overflow, reset to all zeros)
    bvlib::BitVector<8> bv8_3("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 8); }, bv8_3, "00000000");

    // ========================================================================
    // Test cases with N = 16 (BitVector<16>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<16> bv16_0("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 0); }, bv16_0, "1100110011001100");

    // Test case 2: Shift by 1
    bvlib::BitVector<16> bv16_1("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 1); }, bv16_1, "0110011001100110");

    // Test case 3: Shift by 15
    bvlib::BitVector<16> bv16_2("1100110011001101");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 15); }, bv16_2, "0000000000000001");

    // Test case 4: Shift by 16 (overflow, reset to all zeros)
    bvlib::BitVector<16> bv16_3("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 16); }, bv16_3, "0000000000000000");

    // ========================================================================
    // Test cases with N = 32 (BitVector<32>)
    // ========================================================================

    // Test case 1: Shift by 0
    bvlib::BitVector<32> bv32_0("11001100110011001100110011001100");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_right(value, 0); }, bv32_0, "11001100110011001100110011001100");

    // Test case 2: Shift by 1
    bvlib::BitVector<32> bv32_1("11001100110011001100110011001100");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_right(value, 1); }, bv32_1, "01100110011001100110011001100110");

    // Test case 3: Shift by 31 (maximum right shift)
    bvlib::BitVector<32> bv32_2("11001100110011001100110011001101");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_right(value, 31); }, bv32_2, "00000000000000000000000000000001");

    // Test case 4: Shift by 32 (overflow, reset to all zeros)
    bvlib::BitVector<32> bv32_3("11001100110011001100110011001100");
    test_unary(
        [](auto &value) { return bvlib::detail::shift_right(value, 32); }, bv32_3, "00000000000000000000000000000000");

    // ========================================================================
    // Test cases for shift larger than bit size (overflow)
    // ========================================================================

    // Test case 1: Shift by more than bit vector size, it should be reset to 0
    bvlib::BitVector<8> bv8_large_shift("11001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 10); }, bv8_large_shift, "00000000");

    // Test case 2: Shift by N + 1, reset to 0
    bvlib::BitVector<16> bv16_large_shift("1100110011001100");
    test_unary([](auto &value) { return bvlib::detail::shift_right(value, 17); }, bv16_large_shift, "0000000000000000");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    test_shift_left();
    std::cout << "    ✅ All `shift_left` tests completed!\n";

    test_shift_right();
    std::cout << "    ✅ All `shift_right` tests completed!\n";

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
