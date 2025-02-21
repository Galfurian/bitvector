/// @file test_math.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests math operators.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include "bvlib/math.hpp"

#include <cassert>
#include <iostream>

#include "support.hpp"

// ============================================================================
// SUM TESTS
// ============================================================================

/// @brief Test case 1: Sum of two BitVectors of the same size.
void test_sum_same_size()
{
    // BitVector<8> sum of "11001100" + "10101010"
    bvlib::BitVector<8> value1("00001100");
    bvlib::BitVector<8> value2("00001010");
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "00010110");
}

/// @brief Test case 2: Sum of two BitVectors where one is larger.
void test_sum_larger_and_smaller()
{
    // BitVector<8> + BitVector<16>
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<16> value2("0000000000001010");
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "0000000011010110");
}

/// @brief Test case 3: Sum of BitVector and integer.
void test_sum_bitvector_and_integer()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "11010001");
}

/// @brief Test case 4: Sum of integer and BitVector.
void test_sum_integer_and_bitvector()
{
    int value1 = 5;
    bvlib::BitVector<8> value2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "11010001");
}

/// @brief Test case 5: Sum of BitVector and 0.
void test_sum_bitvector_and_zero()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 0;
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "11001100");
}

/// @brief Test case 6: Sum of 0 and BitVector.
void test_sum_zero_and_bitvector()
{
    int value1 = 0;
    bvlib::BitVector<8> value2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "11001100");
}

// Test case 7: Sum a BitVector from another of different sizes.
void test_sum_different_sizes()
{
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<16> value2("0000000000010101");
    test_binary([](auto &lhs, auto &rhs) { return lhs + rhs; }, value1, value2, "0000000011100001");
}

/// @brief Test case 1: BitVector += BitVector of the same size.
void test_sum_equal_same_size()
{
    bvlib::BitVector<8> value1("00001100");
    bvlib::BitVector<8> value2("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs += rhs; }, value1, value2, "10110110");
}

/// @brief Test case 2: BitVector += BitVector of smaller size.
void test_sum_equal_smaller_size()
{
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<4> value2("1010");
    test_binary([](auto &lhs, auto &rhs) { return lhs += rhs; }, value1, value2, "11010110");
}

/// @brief Test case 3: BitVector += integer.
void test_sum_equal_integer()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs += rhs; }, value1, value2, "11010001");
}

/// @brief Test case 4: BitVector += 0.
void test_sum_equal_zero()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 0;
    test_binary([](auto &lhs, auto &rhs) { return lhs += rhs; }, value1, value2, "11001100");
}

/// @brief Test case 1: Prefix increment.
void test_prefix_increment()
{
    bvlib::BitVector<8> value1, value2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs = ++rhs; }, value1, value2, "11001101");
    assert(value2.to_string() == "11001101" && "Prefix increment should increase the BitVector by 1");
}

/// @brief Test case 2: Postfix increment.
void test_postfix_increment()
{
    bvlib::BitVector<8> value1, value2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs = rhs++; }, value1, value2, "11001100");
    assert(value2.to_string() == "11001101" && "Postfix increment should increase the BitVector by 1");
}

// ============================================================================
// SUBTRACTION TESTS
// ============================================================================

// Test case 1: Subtraction of two BitVectors using the - operator (same size).
void test_sub_same_size()
{
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<8> value2("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "00100010");
}

// Test case 2: Subtraction of a larger BitVector from a smaller BitVector.
void test_sub_larger_and_smaller()
{
    bvlib::BitVector<8> value1("00001100");
    bvlib::BitVector<8> value2("00010000");
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "11111100");
}

// Test case 3: Subtraction of a BitVector and an integer.
void test_sub_bitvector_and_integer()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "11000111");
}

// Test case 4: Subtraction of an integer and a BitVector.
void test_sub_integer_and_bitvector()
{
    int value1 = 204;
    bvlib::BitVector<8> value2("00000101");
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "11000111");
}

// Test case 5: Subtraction of BitVector and 0.
void test_sub_bitvector_and_zero()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 0;
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "11001100");
}

// Test case 6: Subtraction of 0 and a BitVector.
void test_sub_zero_and_bitvector()
{
    int value1 = 0;
    bvlib::BitVector<8> value2("00001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "11110100");
}

// Test case 7: Subtracting a BitVector from another of different sizes.
void test_sub_different_sizes()
{
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<16> value2("0000000000010101");
    test_binary([](auto &lhs, auto &rhs) { return lhs - rhs; }, value1, value2, "0000000010110111");
}

/// @brief Tests the operator-= for subtracting BitVectors in place.
void test_sub_equal_same_size()
{
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<8> value2("10101010");
    test_binary([](auto &lhs, auto &rhs) { return lhs -= rhs; }, value1, value2, "00100010");
}

// Test case 2: In-place subtraction of a larger BitVector from a smaller BitVector.
void test_sub_equal_smaller_size()
{
    bvlib::BitVector<8> value1("11001100");
    bvlib::BitVector<4> value2("1010");
    test_binary([](auto &lhs, auto &rhs) { return lhs -= rhs; }, value1, value2, "11000010");
}

// Test case 3: In-place subtraction of a BitVector and an integer.
void test_sub_equal_integer()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 5;
    test_binary([](auto &lhs, auto &rhs) { return lhs -= rhs; }, value1, value2, "11000111");
}

// Test case 4: In-place subtraction of 0 from BitVector (should be unchanged).
void test_sub_equal_zero()
{
    bvlib::BitVector<8> value1("11001100");
    int value2 = 0;
    test_binary([](auto &lhs, auto &rhs) { return lhs -= rhs; }, value1, value2, "11001100");
}

/// @brief Test case 1: Prefix decrement.
void test_prefix_decrement()
{
    bvlib::BitVector<8> value1, value2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs = --rhs; }, value1, value2, "11001011");
    assert(value2.to_string() == "11001011" && "Prefix increment should increase the BitVector by 1");
}

/// @brief Test case 2: Postfix decrement.
void test_postfix_decrement()
{
    bvlib::BitVector<8> value1, value2("11001100");
    test_binary([](auto &lhs, auto &rhs) { return lhs = rhs--; }, value1, value2, "11001100");
    assert(value2.to_string() == "11001011" && "Prefix increment should increase the BitVector by 1");
}

// ============================================================================
// MULTIPLICATION TESTS
// ============================================================================

// Test case 1: Multiply two BitVectors of the same size.
void test_operator_multiply_same_size()
{
    bvlib::BitVector<8> value1("00000101"); // 5 in decimal
    bvlib::BitVector<8> value2("00000010"); // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "0000000000001010");
}

// Test case 2: Multiply a BitVector by an integer.
void test_operator_multiply_bitvector_and_integer()
{
    bvlib::BitVector<8> value1("00000101"); // 5 in decimal
    int value2 = 3;
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "0000000000001111");
}

// Test case 3: Multiply an integer by a BitVector.
void test_operator_multiply_integer_and_bitvector()
{
    int value1 = 3;
    bvlib::BitVector<8> value2("00000101"); // 5 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "0000000000001111");
}

// Test case 4: Multiply two BitVectors where the first is smaller.
void test_operator_multiply_smaller_and_larger()
{
    bvlib::BitVector<4> value1("1010");     // 10 in decimal
    bvlib::BitVector<8> value2("00000011"); // 3 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "000000011110");
}

// Test case 5: Multiply two BitVectors where the second is smaller.
void test_operator_multiply_larger_and_smaller()
{
    bvlib::BitVector<8> value1("11001010"); // 202 in decimal
    bvlib::BitVector<4> value2("1010");     // 10 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "011111100100");
}

// Test case 6: Multiply a BitVector by 0.
void test_operator_multiply_bitvector_and_zero()
{
    bvlib::BitVector<8> value1("00000101"); // 5 in decimal
    int value2 = 0;
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "0000000000000000");
}

// Test case 7: Multiply an integer by 0.
void test_operator_multiply_zero_and_bitvector()
{
    int value1 = 0;
    bvlib::BitVector<8> value2("00000101"); // 5 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "0000000000000000");
}

// Test case 8: Multiply two BitVectors resulting in a larger product.
void test_operator_multiply_larger_product()
{
    bvlib::BitVector<4> value1("1111"); // 15 in decimal
    bvlib::BitVector<4> value2("1111"); // 15 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "11100001");
}

// Test case 9: Multiply BitVector by a large integer (overflow scenario).
void test_operator_multiply_large_integer()
{
    bvlib::BitVector<4> value1("1111"); // 5 in decimal
    int value2 = 15;
    test_binary([](auto &lhs, auto &rhs) { return lhs * rhs; }, value1, value2, "11100001");
}

void test_operator_multiply_equal_same_size()
{
    bvlib::BitVector<8> value1("00000101"); // 5 in decimal
    bvlib::BitVector<8> value2("00000010"); // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs *= rhs; }, value1, value2, "00001010");
}

void test_operator_multiply_equal_bitvector_and_integer()
{
    {
        bvlib::BitVector<8> value1("00000101"); // 5 in decimal
        int value2 = 3;
        test_binary([](auto &lhs, auto &rhs) { return lhs *= rhs; }, value1, value2, "00001111");
    }
    {
        int value1 = 3;
        bvlib::BitVector<8> value2("00000101"); // 5 in decimal
        test_binary([](auto &lhs, auto &rhs) { return lhs *= rhs; }, value1, value2, 15);
    }
}

void test_operator_multiply_equal_zero()
{
    bvlib::BitVector<8> value1("00000101"); // 5 in decimal
    int value2 = 0;
    test_binary([](auto &lhs, auto &rhs) { return lhs *= rhs; }, value1, value2, "00000000");
}

// ============================================================================
// DIVISION TESTS
// ============================================================================

// Test case 1: Divide two BitVectors of the same size.
void test_operator_divide_same_size()
{
    bvlib::BitVector<8> value1("00001000"); // 8 in decimal
    bvlib::BitVector<8> value2("00000010"); // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00000100");
}

// Test case 2: Divide a BitVector by an integer.
void test_operator_divide_bitvector_and_integer()
{
    bvlib::BitVector<8> value1("00001000"); // 8 in decimal
    int value2 = 2;
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00000100");
}

// Test case 3: Divide an integer by a BitVector.
void test_operator_divide_integer_and_bitvector()
{
    int value1 = 8;
    bvlib::BitVector<8> value2("00000010"); // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00000100");
}

// Test case 4: Divide two BitVectors where the first is smaller.
void test_operator_divide_smaller_and_larger()
{
    bvlib::BitVector<8> value1("00000011");          // 3 in decimal
    bvlib::BitVector<16> value2("0000000000001010"); // 10 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00000000");
}

// Test case 5: Divide two BitVectors where the second is smaller.
void test_operator_divide_larger_and_smaller()
{
    bvlib::BitVector<16> value1("0000000000001010"); // 10 in decimal
    bvlib::BitVector<8> value2("00000010");          // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "0000000000000101");
}

// Test case 6: Divide a BitVector by 1.
void test_operator_divide_bitvector_and_one()
{
    bvlib::BitVector<8> value1("00001000"); // 8 in decimal
    int value2 = 1;
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00001000");
}

// Test case 7: Divide an integer by 1.
void test_operator_divide_integer_and_one()
{
    int value1 = 8;
    bvlib::BitVector<8> value2("00000001"); // 1 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00001000");
}

// Test case 8: Divide a BitVector by 0 (exception handling).
void test_operator_divide_bitvector_and_zero()
{
    try {
        bvlib::BitVector<8> value1("00001000"); // 8 in decimal
        int value2  = 0;
        auto result = value1 / value2;
        assert(false && "Dividing by zero should throw an exception.");
    } catch (const std::domain_error &) {
        assert(true && "Division by zero correctly throws an exception.");
    }
}

// Test case 9: Divide an integer by 0 (exception handling).
void test_operator_divide_integer_and_zero()
{
    try {
        int value1 = 8;
        bvlib::BitVector<8> value2("00000000"); // 0 in decimal
        auto result = value1 / value2;
        assert(false && "Dividing by zero should throw an exception.");
    } catch (const std::domain_error &) {
        assert(true && "Division by zero correctly throws an exception.");
    }
}

// Test case 10: Divide two BitVectors resulting in a large quotient.
void test_operator_divide_larger_quotient()
{
    bvlib::BitVector<16> value1("0000000000001010"); // 10 in decimal
    bvlib::BitVector<8> value2("00000001");          // 1 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "0000000000001010");
}

// Test case 11: Divide two BitVectors with identical values (quotient should be 1).
void test_operator_divide_identical_bitvectors()
{
    bvlib::BitVector<8> value1("00001000"); // 8 in decimal
    bvlib::BitVector<8> value2("00001000"); // 8 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00000001");
}

// Test case 12: Divide BitVector by a value larger than the BitVector (quotient should be 0).
void test_operator_divide_bitvector_by_larger_value()
{
    bvlib::BitVector<8> value1("00000001"); // 1 in decimal
    int value2 = 8;
    test_binary([](auto &lhs, auto &rhs) { return lhs / rhs; }, value1, value2, "00000000");
}

/// @brief Tests for `operator/=` with two BitVectors.
void test_operator_divide_equal_bitvectors()
{
    bvlib::BitVector<8> value1("00001100"); // 12 in decimal
    bvlib::BitVector<4> value2("0010");     // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs /= rhs; }, value1, value2, "00000110");
}

/// @brief Tests for `operator/=` with BitVector and an integer.
void test_operator_divide_equal_bitvector_and_integer()
{
    bvlib::BitVector<8> value1("00001100"); // 12 in decimal
    int value2 = 3;
    test_binary([](auto &lhs, auto &rhs) { return lhs /= rhs; }, value1, value2, "00000100");
}

/// @brief Tests for `operator/=` with an integer and BitVector.
void test_operator_divide_equal_integer_and_bitvector()
{
    int value1 = 12;
    bvlib::BitVector<8> value2("00000011"); // 3 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs /= rhs; }, value1, value2, 4);
}

/// @brief Tests for `operator/=` with a BitVector and zero.
void test_operator_divide_equal_by_zero()
{
    bvlib::BitVector<8> value1("00001100"); // 12 in decimal
    int value2 = 0;
    try {
        value1 /= value2;
        assert(false && "operator/= by zero should throw an exception");
    } catch (const std::domain_error &e) {
        assert(std::string(e.what()) == "Division by zero is undefined." && "Exception message should match");
    }
}

/// @brief Tests for `operator/=` where the quotient is zero.
void test_operator_divide_equal_quotient_zero()
{
    bvlib::BitVector<8> value1("00000000"); // 0 in decimal
    bvlib::BitVector<8> value2("00000010"); // 2 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs /= rhs; }, value1, value2, "00000000");
}

/// @brief Tests for `operator/=` with integer and a BitVector where quotient is zero.
void test_operator_divide_equal_integer_and_bitvector_quotient_zero()
{
    int value1 = 0;
    bvlib::BitVector<8> value2("00000001"); // 1 in decimal
    test_binary([](auto &lhs, auto &rhs) { return lhs /= rhs; }, value1, value2, 0);
}

/// @brief Tests for `operator/=` with a BitVector divided by another larger BitVector.
void test_operator_divide_equal_larger_bitvector()
{
    bvlib::BitVector<8> value1("00000001");          // 1 in decimal
    bvlib::BitVector<16> value2("0000000100000000"); // Large BitVector with value 256
    test_binary([](auto &lhs, auto &rhs) { return lhs /= rhs; }, value1, value2, "00000000");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

void run_sum_tests()
{
    test_sum_same_size();
    test_sum_larger_and_smaller();
    test_sum_bitvector_and_integer();
    test_sum_integer_and_bitvector();
    test_sum_bitvector_and_zero();
    test_sum_zero_and_bitvector();
    test_sum_different_sizes();

    std::cout << "    ✅ All `operator+` tests completed!\n";
}

void run_sum_equal_tests()
{
    test_sum_equal_same_size();
    test_sum_equal_smaller_size();
    test_sum_equal_integer();
    test_sum_equal_zero();

    std::cout << "    ✅ All `operator+=` tests completed!\n";
}

void run_increment_tests()
{
    test_prefix_increment();
    test_postfix_increment();

    std::cout << "    ✅ All `operator++` and `++operator` tests completed!\n";
}

void run_sub_tests()
{
    test_sub_same_size();
    test_sub_larger_and_smaller();
    test_sub_bitvector_and_integer();
    test_sub_integer_and_bitvector();
    test_sub_bitvector_and_zero();
    test_sub_zero_and_bitvector();
    test_sub_different_sizes();

    std::cout << "    ✅ All `operator-` tests completed!\n";
}

void run_sub_equal_tests()
{
    test_sub_equal_same_size();
    test_sub_equal_smaller_size();
    test_sub_equal_integer();
    test_sub_equal_zero();

    std::cout << "    ✅ All `operator-=` tests completed!\n";
}

void run_decrement_tests()
{
    test_prefix_decrement();
    test_postfix_decrement();

    std::cout << "    ✅ All `operator++` and `++operator` tests completed!\n";
}

void run_mul_tests()
{
    test_operator_multiply_same_size();
    test_operator_multiply_bitvector_and_integer();
    test_operator_multiply_integer_and_bitvector();
    test_operator_multiply_smaller_and_larger();
    test_operator_multiply_larger_and_smaller();
    test_operator_multiply_bitvector_and_zero();
    test_operator_multiply_zero_and_bitvector();
    test_operator_multiply_larger_product();
    test_operator_multiply_large_integer();

    std::cout << "    ✅ All `operator*` tests completed!\n";
}

void run_mul_equal_tests()
{
    test_operator_multiply_equal_same_size();
    test_operator_multiply_equal_bitvector_and_integer();
    test_operator_multiply_equal_zero();

    std::cout << "    ✅ All `operator*=` tests completed!\n";
}

void run_division_tests()
{
    test_operator_divide_same_size();
    test_operator_divide_bitvector_and_integer();
    test_operator_divide_integer_and_bitvector();
    test_operator_divide_smaller_and_larger();
    test_operator_divide_larger_and_smaller();
    test_operator_divide_bitvector_and_one();
    test_operator_divide_integer_and_one();
    test_operator_divide_bitvector_and_zero();
    test_operator_divide_integer_and_zero();
    test_operator_divide_larger_quotient();
    test_operator_divide_identical_bitvectors();
    test_operator_divide_bitvector_by_larger_value();

    std::cout << "    ✅ All `operator/` tests completed!\n";
}

void run_divide_equal_tests()
{
    test_operator_divide_equal_bitvectors();
    test_operator_divide_equal_bitvector_and_integer();
    test_operator_divide_equal_integer_and_bitvector();
    test_operator_divide_equal_by_zero();
    test_operator_divide_equal_quotient_zero();
    test_operator_divide_equal_integer_and_bitvector_quotient_zero();
    test_operator_divide_equal_larger_bitvector();

    std::cout << "    ✅ All `operator/=` tests completed!\n";
}
// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    run_sum_tests();
    run_sum_equal_tests();
    run_increment_tests();

    run_sub_tests();
    run_sub_equal_tests();
    run_decrement_tests();

    run_mul_tests();
    run_mul_equal_tests();

    run_division_tests();
    run_divide_equal_tests();

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
