/// @file test_support.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests support functions.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include "support.hpp"

// ============================================================================
// most_significant_bit() TESTS
// ============================================================================

/// @brief Tests the `most_significant_bit()` function to find the position of the most significant bit.
void test_most_significant_bit()
{
    bvlib::BitVector<32> bv1("00000000000000000000000000001010");
    test_unary([](auto &value) { return bvlib::detail::most_significant_bit(value); }, bv1, 3U);
    bvlib::BitVector<32> bv2("10000000000000000000000000000000");
    test_unary([](auto &value) { return bvlib::detail::most_significant_bit(value); }, bv2, 31U);
    bvlib::BitVector<32> bv3("00000000000000000000000000000000");
    test_unary([](auto &value) { return bvlib::detail::most_significant_bit(value); }, bv3, 0U);
    bvlib::BitVector<32> bv4("01000000000000000000000000000000");
    test_unary([](auto &value) { return bvlib::detail::most_significant_bit(value); }, bv4, 30U);
    bvlib::BitVector<32> bv5("00000000000000010000000000000000");
    test_unary([](auto &value) { return bvlib::detail::most_significant_bit(value); }, bv5, 16U);

    std::cout << "    ✅ All `most_significant_bit` tests completed!\n";
}

// ============================================================================
// count_leading_zeros() TESTS
// ============================================================================

/// @brief Tests the `count_leading_zeros()` function.
void test_count_leading_zeros()
{
    std::uint32_t val1 = 0b00000000000000000000000011001100;
    test_unary([](auto &value) { return bvlib::detail::count_leading_zeros(value); }, val1, 24U);
    std::uint32_t val2 = 0b00000000000000000000000000001100;
    test_unary([](auto &value) { return bvlib::detail::count_leading_zeros(value); }, val2, 28U);
    std::uint32_t val3 = 0b00000000000000000011001100110011;
    test_unary([](auto &value) { return bvlib::detail::count_leading_zeros(value); }, val3, 18U);
    std::uint32_t val4 = 0b11001100110011001100110011001100;
    test_unary([](auto &value) { return bvlib::detail::count_leading_zeros(value); }, val4, 0U);

    std::cout << "    ✅ All `count_leading_zeros` tests completed!\n";
}

// ============================================================================
// count_trailing_zeros() TESTS
// ============================================================================

/// @brief Tests the `count_trailing_zeros()` function.
void test_count_trailing_zeros()
{
    unsigned val1 = 0b00000000000000000000000011001100;
    test_unary([](auto &value) { return bvlib::detail::count_trailing_zeros(value); }, val1, 2U);
    unsigned val2 = 0b00000000000000000000000011000000;
    test_unary([](auto &value) { return bvlib::detail::count_trailing_zeros(value); }, val2, 6U);
    unsigned val3 = 0b00000000000000001100110011001100;
    test_unary([](auto &value) { return bvlib::detail::count_trailing_zeros(value); }, val3, 2U);
    unsigned val4 = 0b11001100110011001100110011001100;
    test_unary([](auto &value) { return bvlib::detail::count_trailing_zeros(value); }, val4, 2U);

    std::cout << "    ✅ All `count_trailing_zeros` tests completed!\n";
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    test_most_significant_bit();
    test_count_leading_zeros();
    test_count_trailing_zeros();

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
