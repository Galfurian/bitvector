/// @file test_comparison.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests comparison operators.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include <cassert>
#include <iostream>

#include "support.hpp"

// ============================================================================
// BOOL (==)
// ============================================================================

/// @brief Tests equality between two BitVectors of the same size.
void test_operator_equal_same_size()
{
    // Test case 1: Same BitVector, equal
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_1, bv8_2, true);

    // Test case 2: Same BitVector, different value
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_3, bv8_4, false);
}
/// @brief Tests equality between a BitVector and an integer value.
void test_operator_equal_bitvector_integer()
{
    // Test case 1: BitVector equals integer
    bvlib::BitVector<8> bv8_1("11001100");
    int value = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_1, value, true);

    // Test case 2: BitVector does not equal integer
    bvlib::BitVector<8> bv8_2("11001100");
    int value2 = 123;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_2, value2, false);
}

/// @brief Tests equality between an integer value and a BitVector.
void test_operator_equal_integer_bitvector()
{
    // Test case 1: Integer equals BitVector
    int value = 204;
    bvlib::BitVector<8> bv8_1("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, value, bv8_1, true);

    // Test case 2: Integer does not equal BitVector
    int value2 = 123;
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, value2, bv8_2, false);
}

/// @brief Tests equality between BitVectors of different sizes.
void test_operator_equal_different_sizes()
{
    // Test case 1: BitVectors with different sizes, should be false
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_1, bv16_1, false);

    // Test case 2: BitVector with extra leading zeros, should be equal
    bvlib::BitVector<8> bv8_2("11001100");
    bvlib::BitVector<16> bv16_2("0000000011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_2, bv16_2, true);
}

/// @brief Tests equality with a BitVector and an integer value.
void test_operator_equal_bitvector_integer_extended()
{
    // Test case 1: BitVector and integer value with all bits set
    bvlib::BitVector<16> bv16_1("1111111111111111");
    int value = 65535;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv16_1, value, true);

    // Test case 2: BitVector and integer value with different bits
    bvlib::BitVector<16> bv16_2("1111000011110000");
    int value2 = 61680;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv16_2, value2, true);

    // Test case 3: BitVector and integer mismatch
    bvlib::BitVector<8> bv8_1("11001100");
    int value3 = 123;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_1, value3, false);
}

/// @brief Tests equality with a BitVector and a truncated integer.
void test_operator_equal_truncated_integer()
{
    // Test case 1: Truncated BitVector and integer
    bvlib::BitVector<4> bv4_1("1100");
    int value = 12;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv4_1, value, true);

    // Test case 2: Truncated BitVector and integer mismatch
    bvlib::BitVector<4> bv4_2("1000");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv4_2, value2, false);
}

/// @brief Tests equality between BitVectors of equal size but different data.
void test_operator_equal_bitvectors_different_data()
{
    // Test case 1: Equal size but different data
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_1, bv8_2, false);

    // Test case 2: Equal size but same data
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs == rhs; }, bv8_3, bv8_4, true);
}

// ============================================================================
// BOOL (!=)
// ============================================================================

/// @brief Tests inequality between two BitVectors of the same size.
void test_operator_not_equal_same_size()
{
    // Test case 1: Same BitVector, equal
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_1, bv8_2, false);

    // Test case 2: Same BitVector, different value
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_3, bv8_4, true);
}

/// @brief Tests inequality between a BitVector and an integer value.
void test_operator_not_equal_bitvector_integer()
{
    // Test case 1: BitVector equals integer
    bvlib::BitVector<8> bv8_1("11001100");
    int value = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_1, value, false);

    // Test case 2: BitVector does not equal integer
    bvlib::BitVector<8> bv8_2("11001100");
    int value2 = 123;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_2, value2, true);
}

/// @brief Tests inequality between an integer value and a BitVector.
void test_operator_not_equal_integer_bitvector()
{
    // Test case 1: Integer equals BitVector
    int value = 204;
    bvlib::BitVector<8> bv8_1("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, value, bv8_1, false);

    // Test case 2: Integer does not equal BitVector
    int value2 = 123;
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, value2, bv8_2, true);
}

/// @brief Tests inequality between BitVectors of different sizes.
void test_operator_not_equal_different_sizes()
{
    // Test case 1: BitVectors with different sizes, should be true
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_1, bv16_1, true);

    // Test case 2: BitVector with extra leading zeros, should be false
    bvlib::BitVector<8> bv8_2("11001100");
    bvlib::BitVector<16> bv16_2("0000000011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_2, bv16_2, false);
}

/// @brief Tests inequality with a BitVector and an integer value.
void test_operator_not_equal_bitvector_integer_extended()
{
    // Test case 1: BitVector and integer value with all bits set
    bvlib::BitVector<16> bv16_1("1111111111111111");
    int value = 65535;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv16_1, value, false);

    // Test case 2: BitVector and integer value with different bits
    bvlib::BitVector<16> bv16_2("1111000011110000");
    int value2 = 61680;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv16_2, value2, false);

    // Test case 3: BitVector and integer mismatch
    bvlib::BitVector<8> bv8_1("11001100");
    int value3 = 123;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_1, value3, true);
}

/// @brief Tests inequality with a BitVector and a truncated integer.
void test_operator_not_equal_truncated_integer()
{
    // Test case 1: Truncated BitVector and integer
    bvlib::BitVector<4> bv4_1("1100");
    int value = 12;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv4_1, value, false);

    // Test case 2: Truncated BitVector and integer mismatch
    bvlib::BitVector<4> bv4_2("1000");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv4_2, value2, true);
}

/// @brief Tests inequality between BitVectors of equal size but different data.
void test_operator_not_equal_bitvectors_different_data()
{
    // Test case 1: Equal size but different data
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_1, bv8_2, true);

    // Test case 2: Equal size but same data
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs != rhs; }, bv8_3, bv8_4, false);
}

// ============================================================================
// BOOL (<)
// ============================================================================

/// @brief Tests the operator< for BitVectors of the same size.
void test_operator_less_than_same_size()
{
    // Test case 1: lhs < rhs (lhs is less than rhs)
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_1, bv8_2, true);

    // Test case 2: lhs > rhs (lhs is greater than rhs)
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_3, bv8_4, false);

    // Test case 3: lhs == rhs (lhs is equal to rhs)
    bvlib::BitVector<8> bv8_5("11001100");
    bvlib::BitVector<8> bv8_6("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_5, bv8_6, false);
}

/// @brief Tests the operator< for a BitVector and an integer value.
void test_operator_less_than_bitvector_integer()
{
    // Test case 1: BitVector < integer (lhs < rhs)
    bvlib::BitVector<8> bv8_1("11001100");
    int value = 205;
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_1, value, true);

    // Test case 2: BitVector > integer (lhs > rhs)
    bvlib::BitVector<8> bv8_2("11110000");
    int value2 = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_2, value2, false);

    // Test case 3: BitVector == integer (lhs == rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    int value3 = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_3, value3, false);
}

/// @brief Tests the operator< for an integer value and a BitVector.
void test_operator_less_than_integer_bitvector()
{
    // Test case 1: integer < BitVector (lhs < rhs)
    int value = 203;
    bvlib::BitVector<8> bv8_1("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, value, bv8_1, true);

    // Test case 2: integer > BitVector (lhs > rhs)
    int value2 = 205;                      // 11100100
    bvlib::BitVector<8> bv8_2("11001100"); // 11001100
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, value2, bv8_2, false);

    // Test case 3: integer == BitVector (lhs == rhs)
    int value3 = 204;
    bvlib::BitVector<8> bv8_3("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, value3, bv8_3, false);
}

/// @brief Tests the operator< for BitVectors of different sizes.
void test_operator_less_than_different_sizes()
{
    // Test case 1: BitVector<8> < BitVector<16> (lhs < rhs)
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_1, bv16_1, true);

    // Test case 2: BitVector<16> < BitVector<8> (lhs > rhs)
    bvlib::BitVector<16> bv16_2("1100110011001100");
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv16_2, bv8_2, false);

    // Test case 3: BitVector<8> == BitVector<16> (lhs == rhs if the same value is extended)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<16> bv16_3("0000000011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_3, bv16_3, false);
}

/// @brief Tests the operator< for larger BitVectors.
void test_operator_less_than_large_bitvectors()
{
    // Test case 1: Shifted comparison of larger BitVectors
    bvlib::BitVector<64> bv64_1("1100110011001100110011001100110011001100110011001100110011001100");
    bvlib::BitVector<64> bv64_2("1111000011110000111100001111000011110000111100001111000011110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv64_1, bv64_2, true);

    // Test case 2: Comparison with BitVector containing all 1s
    bvlib::BitVector<64> bv64_3("1111111111111111111111111111111111111111111111111111111111111111");
    bvlib::BitVector<64> bv64_4("0000000000000000000000000000000000000000000000000000000000000000");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv64_3, bv64_4, false);
}

/// @brief Tests the operator< for unequal BitVectors of the same size but different values.
void test_operator_less_than_different_data()
{
    // Test case 1: Unequal BitVectors of the same size (lhs < rhs)
    bvlib::BitVector<8> bv8_1("10101010");
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_1, bv8_2, true);

    // Test case 2: Unequal BitVectors of the same size (lhs > rhs)
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_3, bv8_4, false);

    // Test case 3: Unequal BitVectors of the same size (lhs == rhs)
    bvlib::BitVector<8> bv8_5("11110000");
    bvlib::BitVector<8> bv8_6("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv8_5, bv8_6, false);
}

/// @brief Tests the operator< for truncated BitVectors and integers.
void test_operator_less_than_truncated_integer()
{
    // Test case 1: Truncated BitVector < integer
    bvlib::BitVector<4> bv4_1("11100");
    int value = 12;
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv4_1, value, false);

    // Test case 2: Truncated BitVector < integer
    bvlib::BitVector<4> bv4_2("10000");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs < rhs; }, bv4_2, value2, true);
}

// ============================================================================
// BOOL (>)
// ============================================================================

/// @brief Tests the operator> for BitVectors of the same size.
void test_operator_greater_than_same_size()
{
    // Test case 1: lhs > rhs (lhs is greater than rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_1, bv8_2, true);

    // Test case 2: lhs < rhs (lhs is smaller than rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_3, bv8_4, false);

    // Test case 3: lhs == rhs (lhs is equal to rhs)
    bvlib::BitVector<8> bv8_5("11001100");
    bvlib::BitVector<8> bv8_6("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_5, bv8_6, false);
}

/// @brief Tests the operator> for a BitVector and an integer value.
void test_operator_greater_than_bitvector_integer()
{
    // Test case 1: BitVector > integer (lhs > rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    int value = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_1, value, true);

    // Test case 2: BitVector < integer (lhs < rhs)
    bvlib::BitVector<8> bv8_2("11001100");
    int value2 = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_2, value2, false);

    // Test case 3: BitVector == integer (lhs == rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    int value3 = 204;
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_3, value3, false);
}

/// @brief Tests the operator> for an integer value and a BitVector.
void test_operator_greater_than_integer_bitvector()
{
    // Test case 1: integer > BitVector (lhs > rhs)
    int value = 250;
    bvlib::BitVector<8> bv8_1("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, value, bv8_1, true);

    // Test case 2: integer < BitVector (lhs < rhs)
    int value2 = 100;
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, value2, bv8_2, false);

    // Test case 3: integer == BitVector (lhs == rhs)
    int value3 = 204;
    bvlib::BitVector<8> bv8_3("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, value3, bv8_3, false);
}

/// @brief Tests the operator> for BitVectors of different sizes.
void test_operator_greater_than_different_sizes()
{
    // Test case 1: BitVector<8> > BitVector<16> (lhs > rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_1, bv16_1, false);

    // Test case 2: BitVector<16> > BitVector<8> (lhs > rhs)
    bvlib::BitVector<16> bv16_2("1100110011001100");
    bvlib::BitVector<8> bv8_2("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv16_2, bv8_2, true);

    // Test case 3: BitVector<8> == BitVector<16> (lhs == rhs if the same value is extended)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<16> bv16_3("0000000011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_3, bv16_3, false);
}

/// @brief Tests the operator> for larger BitVectors.
void test_operator_greater_than_large_bitvectors()
{
    // Test case 1: Shifted comparison of larger BitVectors
    bvlib::BitVector<64> bv64_1("1111000011110000111100001111000011110000111100001111000011110000");
    bvlib::BitVector<64> bv64_2("1100110011001100110011001100110011001100110011001100110011001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv64_1, bv64_2, true);

    // Test case 2: Comparison with BitVector containing all 0s
    bvlib::BitVector<64> bv64_3("0000000000000000000000000000000000000000000000000000000000000000");
    bvlib::BitVector<64> bv64_4("1111111111111111111111111111111111111111111111111111111111111111");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv64_3, bv64_4, false);
}

/// @brief Tests the operator> for unequal BitVectors of the same size but different values.
void test_operator_greater_than_different_data()
{
    // Test case 1: Unequal BitVectors of the same size (lhs > rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    bvlib::BitVector<8> bv8_2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_1, bv8_2, true);

    // Test case 2: Unequal BitVectors of the same size (lhs < rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_3, bv8_4, false);

    // Test case 3: Unequal BitVectors of the same size (lhs == rhs)
    bvlib::BitVector<8> bv8_5("11110000");
    bvlib::BitVector<8> bv8_6("11110000");
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv8_5, bv8_6, false);
}

/// @brief Tests the operator> for truncated BitVectors and integers.
void test_operator_greater_than_truncated_integer()
{
    // Test case 1: Truncated BitVector > integer
    bvlib::BitVector<4> bv4_1("1100");
    int value = 12;
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv4_1, value, false);

    // Test case 2: Truncated BitVector > integer
    bvlib::BitVector<4> bv4_2("1000");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs > rhs; }, bv4_2, value2, true);
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

void run_operator_equal_all_tests()
{
    test_operator_equal_same_size();
    test_operator_equal_bitvector_integer();
    test_operator_equal_integer_bitvector();
    test_operator_equal_different_sizes();
    test_operator_equal_bitvector_integer_extended();
    test_operator_equal_truncated_integer();
    test_operator_equal_bitvectors_different_data();

    std::cout << "    ✅ All `operator==` tests completed!\n";
}

void run_operator_not_equal_all_tests()
{
    test_operator_not_equal_same_size();
    test_operator_not_equal_bitvector_integer();
    test_operator_not_equal_integer_bitvector();
    test_operator_not_equal_different_sizes();
    test_operator_not_equal_bitvector_integer_extended();
    test_operator_not_equal_truncated_integer();
    test_operator_not_equal_bitvectors_different_data();

    std::cout << "    ✅ All `operator!=` tests completed!\n";
}

void run_operator_less_than_all_tests()
{
    test_operator_less_than_same_size();
    test_operator_less_than_bitvector_integer();
    test_operator_less_than_integer_bitvector();
    test_operator_less_than_different_sizes();
    test_operator_less_than_large_bitvectors();
    test_operator_less_than_different_data();
    test_operator_less_than_truncated_integer();

    std::cout << "    ✅ All `operator<` tests completed!\n";
}

void run_operator_greater_than_all_tests()
{
    test_operator_greater_than_same_size();
    test_operator_greater_than_bitvector_integer();
    test_operator_greater_than_integer_bitvector();
    test_operator_greater_than_different_sizes();
    test_operator_greater_than_large_bitvectors();
    test_operator_greater_than_different_data();
    test_operator_greater_than_truncated_integer();

    std::cout << "    ✅ All `operator>` tests completed!\n";
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    run_operator_equal_all_tests();
    run_operator_not_equal_all_tests();
    run_operator_less_than_all_tests();
    run_operator_greater_than_all_tests();

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
