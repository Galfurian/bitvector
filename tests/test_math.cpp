#include "bvlib/math.hpp"

#include <cassert>
#include <iostream>

// ============================================================================
// SUM TESTS
// ============================================================================

/// @brief Test case 1: Sum of two BitVectors of the same size.
void test_sum_same_size()
{
    // BitVector<8> sum of "11001100" + "10101010"
    bvlib::BitVector<8> bv1("00001100");
    bvlib::BitVector<8> bv2("00001010");
    bvlib::BitVector<8> result = bv1 + bv2;
    assert(result.to_string() == "00010110" && "Sum of two BitVectors of same size should work correctly");
}

/// @brief Test case 2: Sum of two BitVectors where one is larger.
void test_sum_larger_and_smaller()
{
    // BitVector<8> + BitVector<16>
    bvlib::BitVector<8> bv8("11001100");
    bvlib::BitVector<16> bv16("0000000000001010");
    bvlib::BitVector<16> result = bv8 + bv16;
    assert(result.to_string() == "0000000011010110" && "Sum of larger and smaller BitVectors should work correctly");
}

/// @brief Test case 3: Sum of BitVector and integer.
void test_sum_bitvector_and_integer()
{
    bvlib::BitVector<8> bv("11001100");
    int value                  = 5;
    bvlib::BitVector<8> result = bv + value;
    assert(result.to_string() == "11010001" && "Sum of BitVector and integer should work correctly");
}

/// @brief Test case 4: Sum of integer and BitVector.
void test_sum_integer_and_bitvector()
{
    int value = 5;
    bvlib::BitVector<8> bv("11001100");
    bvlib::BitVector<8> result = value + bv;
    assert(result.to_string() == "11010001" && "Sum of integer and BitVector should work correctly");
}

/// @brief Test case 5: Sum of BitVector and 0.
void test_sum_bitvector_and_zero()
{
    bvlib::BitVector<8> bv("11001100");
    int value                  = 0;
    bvlib::BitVector<8> result = bv + value;
    assert(result.to_string() == "11001100" && "Sum of BitVector and 0 should return the BitVector unchanged");
}

/// @brief Test case 6: Sum of 0 and BitVector.
void test_sum_zero_and_bitvector()
{
    int value = 0;
    bvlib::BitVector<8> bv("11001100");
    bvlib::BitVector<8> result = value + bv;
    assert(result.to_string() == "11001100" && "Sum of 0 and BitVector should return the BitVector unchanged");
}

// Test case 7: Sum a BitVector from another of different sizes.
void test_sum_different_sizes()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<16> bv2("0000000000010101");
    auto result = bv1 + bv2;
    assert(
        result.to_string() == "0000000011100001" &&
        "Subtraction of BitVectors of different sizes should handle larger BitVectors correctly");
}

/// @brief Test case 1: BitVector += BitVector of the same size.
void test_sum_equal_same_size()
{
    bvlib::BitVector<8> bv1("00001100");
    bvlib::BitVector<8> bv2("10101010");
    bv1 += bv2;
    assert(bv1.to_string() == "10110110" && "BitVector += BitVector of the same size should work correctly");
}

/// @brief Test case 2: BitVector += BitVector of smaller size.
void test_sum_equal_smaller_size()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<4> bv2("1010");
    bv1 += bv2;
    assert(bv1.to_string() == "11010110" && "BitVector += smaller BitVector should work correctly");
}

/// @brief Test case 3: BitVector += integer.
void test_sum_equal_integer()
{
    bvlib::BitVector<8> bv("11001100");
    int value = 5;
    bv += value;
    assert(bv.to_string() == "11010001" && "BitVector += integer should work correctly");
}

/// @brief Test case 4: BitVector += 0.
void test_sum_equal_zero()
{
    bvlib::BitVector<8> bv("11001100");
    int value = 0;
    bv += value;
    assert(bv.to_string() == "11001100" && "BitVector += 0 should return the BitVector unchanged");
}

/// @brief Test case 1: Prefix increment.
void test_prefix_increment()
{
    bvlib::BitVector<8> bv("11001100");
    bvlib::BitVector<8> temp = ++bv;
    assert(temp.to_string() == "11001101" && "Prefix increment should increase the BitVector by 1");
    assert(bv.to_string() == "11001101" && "Prefix increment should increase the BitVector by 1");
}

/// @brief Test case 2: Postfix increment.
void test_postfix_increment()
{
    bvlib::BitVector<8> bv("11001100");
    bvlib::BitVector<8> temp = bv++;
    assert(temp.to_string() == "11001100" && "Postfix increment should return the original BitVector");
    assert(bv.to_string() == "11001101" && "Postfix increment should increase the BitVector by 1");
}

// ============================================================================
// SUBTRACTION TESTS
// ============================================================================

// Test case 1: Subtraction of two BitVectors using the - operator (same size).
void test_sub_same_size()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<8> bv2("10101010");
    auto result = bv1 - bv2;
    assert(result.to_string() == "00100010" && "Subtraction of two BitVectors of the same size should work correctly");
}

// Test case 2: Subtraction of a larger BitVector from a smaller BitVector.
void test_sub_larger_and_smaller()
{
    bvlib::BitVector<8> bv1("00001100");
    bvlib::BitVector<8> bv2("00010000");
    auto result = bv1 - bv2;
    assert(
        result.to_string() == "11111100" &&
        "Subtraction of a larger BitVector from a smaller BitVector should handle underflow correctly");
}

// Test case 3: Subtraction of a BitVector and an integer.
void test_sub_bitvector_and_integer()
{
    bvlib::BitVector<8> bv1("11001100");
    int value   = 5;
    auto result = bv1 - value;
    assert(result.to_string() == "11000111" && "Subtraction of a BitVector and an integer should work correctly");
}

// Test case 4: Subtraction of an integer and a BitVector.
void test_sub_integer_and_bitvector()
{
    int value = 204;
    bvlib::BitVector<8> bv1("00000101");
    auto result = value - bv1;
    assert(result.to_string() == "11000111" && "Subtraction of an integer and a BitVector should work correctly");
}

// Test case 5: Subtraction of BitVector and 0.
void test_sub_bitvector_and_zero()
{
    bvlib::BitVector<8> bv1("11001100");
    int value   = 0;
    auto result = bv1 - value;
    assert(
        result.to_string() == "11001100" && "Subtraction of a BitVector and 0 should return the BitVector unchanged");
}

// Test case 6: Subtraction of 0 and a BitVector.
void test_sub_zero_and_bitvector()
{
    int value = 0;
    bvlib::BitVector<8> bv1("00001100");
    auto result = value - bv1;
    assert(
        result.to_string() == "11110100" &&
        "Subtraction of 0 and a BitVector should return the negation of the BitVector");
}

// Test case 7: Subtracting a BitVector from another of different sizes.
void test_sub_different_sizes()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<16> bv2("0000000000010101");
    auto result = bv1 - bv2;
    assert(
        result.to_string() == "0000000010110111" &&
        "Subtraction of BitVectors of different sizes should handle larger BitVectors correctly");
}

/// @brief Tests the operator-= for subtracting BitVectors in place.
void test_sub_equal_same_size()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<8> bv2("10101010");
    bv1 -= bv2;
    assert(bv1.to_string() == "00100010" && "operator-= should subtract two BitVectors of the same size correctly");
}

// Test case 2: In-place subtraction of a larger BitVector from a smaller BitVector.
void test_sub_equal_smaller_size()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<4> bv2("1010");
    bv1 -= bv2;
    assert(bv1.to_string() == "11000010" && "operator-= should handle borrow correctly");
}

// Test case 3: In-place subtraction of a BitVector and an integer.
void test_sub_equal_integer()
{
    bvlib::BitVector<8> bv1("11001100");
    int value = 5;
    bv1 -= value;
    assert(
        bv1.to_string() == "11000111" &&
        "operator-= should handle subtraction of a BitVector and an integer correctly");
}

// Test case 4: In-place subtraction of 0 from BitVector (should be unchanged).
void test_sub_equal_zero()
{
    bvlib::BitVector<8> bv1("11001100");
    int value = 0;
    bv1 -= value;
    assert(bv1.to_string() == "11001100" && "operator-= with 0 should return the original BitVector unchanged");
}

/// @brief Test case 1: Prefix decrement.
void test_prefix_decrement()
{
    bvlib::BitVector<8> bv("11001100");
    bvlib::BitVector<8> temp = --bv;
    assert(temp.to_string() == "11001011" && "Prefix decrement should increase the BitVector by 1");
    assert(bv.to_string() == "11001011" && "Prefix decrement should increase the BitVector by 1");
}

/// @brief Test case 2: Postfix decrement.
void test_postfix_decrement()
{
    bvlib::BitVector<8> bv("11001100");
    bvlib::BitVector<8> temp = bv--;
    assert(temp.to_string() == "11001100" && "Postfix decrement should return the original BitVector");
    assert(bv.to_string() == "11001011" && "Postfix decrement should increase the BitVector by 1");
}

// ============================================================================
// MULTIPLICATION TESTS
// ============================================================================

// Test case 1: Multiply two BitVectors of the same size.
void test_operator_multiply_same_size()
{
    bvlib::BitVector<8> bv1("00000101"); // 5 in decimal
    bvlib::BitVector<8> bv2("00000010"); // 2 in decimal
    auto result = bv1 * bv2;
    assert(
        result.to_string() == "0000000000001010" &&
        "Multiplication of two BitVectors of the same size should work correctly");
}

// Test case 2: Multiply a BitVector by an integer.
void test_operator_multiply_bitvector_and_integer()
{
    bvlib::BitVector<8> bv("00000101"); // 5 in decimal
    int value   = 3;
    auto result = bv * value;
    assert(
        result.to_string() == "0000000000001111" &&
        "Multiplication of a BitVector and an integer should work correctly");
}

// Test case 3: Multiply an integer by a BitVector.
void test_operator_multiply_integer_and_bitvector()
{
    int value = 3;
    bvlib::BitVector<8> bv("00000101"); // 5 in decimal
    auto result = value * bv;
    assert(
        result.to_string() == "0000000000001111" &&
        "Multiplication of an integer and a BitVector should work correctly");
}

// Test case 4: Multiply two BitVectors where the first is smaller.
void test_operator_multiply_smaller_and_larger()
{
    bvlib::BitVector<4> bv1("1010");     // 10 in decimal
    bvlib::BitVector<8> bv2("00000011"); // 3 in decimal
    auto result = bv1 * bv2;
    assert(
        result.to_string() == "000000011110" &&
        "Multiplying a smaller BitVector with a larger BitVector should work correctly");
}

// Test case 5: Multiply two BitVectors where the second is smaller.
void test_operator_multiply_larger_and_smaller()
{
    bvlib::BitVector<8> bv1("11001010"); // 202 in decimal
    bvlib::BitVector<4> bv2("1010");     // 10 in decimal
    auto result = bv1 * bv2;
    assert(
        result.to_string() == "011111100100" &&
        "Multiplying a larger BitVector with a smaller BitVector should work correctly");
}

// Test case 6: Multiply a BitVector by 0.
void test_operator_multiply_bitvector_and_zero()
{
    bvlib::BitVector<8> bv("00000101"); // 5 in decimal
    int value   = 0;
    auto result = bv * value;
    assert(result.to_string() == "0000000000000000" && "Multiplying a BitVector by 0 should return all zeros");
}

// Test case 7: Multiply an integer by 0.
void test_operator_multiply_zero_and_bitvector()
{
    int value = 0;
    bvlib::BitVector<8> bv("00000101"); // 5 in decimal
    auto result = value * bv;
    assert(result.to_string() == "0000000000000000" && "Multiplying 0 by a BitVector should return all zeros");
}

// Test case 8: Multiply two BitVectors resulting in a larger product.
void test_operator_multiply_larger_product()
{
    bvlib::BitVector<4> bv1("1111"); // 15 in decimal
    bvlib::BitVector<4> bv2("1111"); // 15 in decimal
    auto result = bv1 * bv2;
    assert(result.to_string() == "11100001" && "Multiplying two BitVectors should result in a larger product");
}

// Test case 9: Multiply BitVector by a large integer (overflow scenario).
void test_operator_multiply_large_integer()
{
    bvlib::BitVector<4> bv("1111"); // 5 in decimal
    int large   = 15;
    auto result = bv * large;
    assert(result.to_string() == "11100001" && "Multiplying BitVector by a large integer should work correctly");
}

void test_operator_multiply_equal_same_size()
{
    bvlib::BitVector<8> bv1("00000101"); // 5 in decimal
    bvlib::BitVector<8> bv2("00000010"); // 2 in decimal
    bv1 *= bv2;
    assert(bv1.to_string() == "00001010" && "operator*= should multiply two BitVectors of the same size correctly");
}

void test_operator_multiply_equal_bitvector_and_integer()
{
    {
        bvlib::BitVector<8> bv("00000101"); // 5 in decimal
        int value = 3;
        bv *= value;
        assert(
            bv.to_string() == "00001111" &&
            "operator*= should handle multiplication of a BitVector and an integer correctly");
    }
    {
        int value = 3;
        bvlib::BitVector<8> bv("00000101"); // 5 in decimal
        value *= bv;
        assert(value == 15 && "operator*= should handle multiplication of a BitVector and an integer correctly");
    }
}

void test_operator_multiply_equal_zero()
{
    bvlib::BitVector<8> bv("00000101"); // 5 in decimal
    int value = 0;
    bv *= value;
    assert(bv.to_string() == "00000000" && "operator*= with 0 should return all zeros");
}

// ============================================================================
// DIVISION TESTS
// ============================================================================

// Test case 1: Divide two BitVectors of the same size.
void test_operator_divide_same_size()
{
    bvlib::BitVector<8> bv1("00001000"); // 8 in decimal
    bvlib::BitVector<8> bv2("00000010"); // 2 in decimal
    auto result = bv1 / bv2;
    assert(result.to_string() == "00000100" && "Division of two BitVectors of the same size should work correctly");
}

// Test case 2: Divide a BitVector by an integer.
void test_operator_divide_bitvector_and_integer()
{
    bvlib::BitVector<8> bv("00001000"); // 8 in decimal
    int value   = 2;
    auto result = bv / value;
    assert(result.to_string() == "00000100" && "Division of a BitVector by an integer should work correctly");
}

// Test case 3: Divide an integer by a BitVector.
void test_operator_divide_integer_and_bitvector()
{
    int value = 8;
    bvlib::BitVector<8> bv("00000010"); // 2 in decimal
    auto result = value / bv;
    assert(result.to_string() == "00000100" && "Division of an integer by a BitVector should work correctly");
}

// Test case 4: Divide two BitVectors where the first is smaller.
void test_operator_divide_smaller_and_larger()
{
    bvlib::BitVector<8> bv1("00000011");          // 3 in decimal
    bvlib::BitVector<16> bv2("0000000000001010"); // 10 in decimal
    auto result = bv1 / bv2;
    assert(
        result.to_string() == "00000000" &&
        "Dividing a smaller BitVector by a larger BitVector should give quotient 0");
}

// Test case 5: Divide two BitVectors where the second is smaller.
void test_operator_divide_larger_and_smaller()
{
    bvlib::BitVector<16> bv1("0000000000001010"); // 10 in decimal
    bvlib::BitVector<8> bv2("00000010");          // 2 in decimal
    auto result = bv1 / bv2;
    assert(
        result.to_string() == "0000000000000101" &&
        "Dividing a larger BitVector by a smaller BitVector should give the quotient correctly");
}

// Test case 6: Divide a BitVector by 1.
void test_operator_divide_bitvector_and_one()
{
    bvlib::BitVector<8> bv("00001000"); // 8 in decimal
    int value   = 1;
    auto result = bv / value;
    assert(result.to_string() == "00001000" && "Dividing a BitVector by 1 should return the BitVector unchanged");
}

// Test case 7: Divide an integer by 1.
void test_operator_divide_integer_and_one()
{
    int value = 8;
    bvlib::BitVector<8> bv("00000001"); // 1 in decimal
    auto result = value / bv;
    assert(result.to_string() == "00001000" && "Dividing an integer by 1 should return the integer as the quotient");
}

// Test case 8: Divide a BitVector by 0 (exception handling).
void test_operator_divide_bitvector_and_zero()
{
    try {
        bvlib::BitVector<8> bv("00001000"); // 8 in decimal
        int value   = 0;
        auto result = bv / value;
        assert(false && "Dividing by zero should throw an exception.");
    } catch (const std::domain_error &e) {
        assert(true && "Division by zero correctly throws an exception.");
    }
}

// Test case 9: Divide an integer by 0 (exception handling).
void test_operator_divide_integer_and_zero()
{
    try {
        int value = 8;
        bvlib::BitVector<8> bv("00000000"); // 0 in decimal
        auto result = value / bv;
        assert(false && "Dividing by zero should throw an exception.");
    } catch (const std::domain_error &e) {
        assert(true && "Division by zero correctly throws an exception.");
    }
}

// Test case 10: Divide two BitVectors resulting in a large quotient.
void test_operator_divide_larger_quotient()
{
    bvlib::BitVector<16> bv1("0000000000001010"); // 10 in decimal
    bvlib::BitVector<8> bv2("00000001");          // 1 in decimal
    auto result = bv1 / bv2;
    assert(
        result.to_string() == "0000000000001010" &&
        "Dividing a larger BitVector by 1 should give the original BitVector as quotient");
}

// Test case 11: Divide two BitVectors with identical values (quotient should be 1).
void test_operator_divide_identical_bitvectors()
{
    bvlib::BitVector<8> bv1("00001000"); // 8 in decimal
    bvlib::BitVector<8> bv2("00001000"); // 8 in decimal
    auto result = bv1 / bv2;
    assert(result.to_string() == "00000001" && "Dividing identical BitVectors should result in quotient 1");
}

// Test case 12: Divide BitVector by a value larger than the BitVector (quotient should be 0).
void test_operator_divide_bitvector_by_larger_value()
{
    bvlib::BitVector<8> bv("00000001"); // 1 in decimal
    int value   = 8;
    auto result = bv / value;
    assert(
        result.to_string() == "00000000" && "Dividing BitVector by a value larger than it should result in quotient 0");
}

/// @brief Tests for `operator/=` with two BitVectors.
void test_operator_divide_equal_bitvectors()
{
    bvlib::BitVector<8> bv1("00001100"); // 12 in decimal
    bvlib::BitVector<4> bv2("0010");     // 2 in decimal
    bv1 /= bv2;
    assert(bv1.to_string() == "00000110" && "operator/= with two BitVectors should divide correctly");
}

/// @brief Tests for `operator/=` with BitVector and an integer.
void test_operator_divide_equal_bitvector_and_integer()
{
    bvlib::BitVector<8> bv("00001100"); // 12 in decimal
    int value = 3;
    bv /= value;
    assert(bv.to_string() == "00000100" && "operator/= with BitVector and integer should divide correctly");
}

/// @brief Tests for `operator/=` with an integer and BitVector.
void test_operator_divide_equal_integer_and_bitvector()
{
    int value = 12;
    bvlib::BitVector<8> bv("00000011"); // 3 in decimal
    value /= bv;
    assert(value == 4 && "operator/= with integer and BitVector should divide correctly");
}

/// @brief Tests for `operator/=` with a BitVector and zero.
void test_operator_divide_equal_by_zero()
{
    bvlib::BitVector<8> bv("00001100"); // 12 in decimal
    int value = 0;
    try {
        bv /= value;
        assert(false && "operator/= by zero should throw an exception");
    } catch (const std::domain_error &e) {
        assert(std::string(e.what()) == "Division by zero is undefined." && "Exception message should match");
    }
}

/// @brief Tests for `operator/=` where the quotient is zero.
void test_operator_divide_equal_quotient_zero()
{
    bvlib::BitVector<8> bv1("00000000"); // 0 in decimal
    bvlib::BitVector<8> bv2("00000010"); // 2 in decimal
    bv1 /= bv2;
    assert(bv1.to_string() == "00000000" && "operator/= should return 0 when dividing by a non-zero BitVector");
}

/// @brief Tests for `operator/=` with integer and a BitVector where quotient is zero.
void test_operator_divide_equal_integer_and_bitvector_quotient_zero()
{
    int value = 0;
    bvlib::BitVector<8> bv("00000001"); // 1 in decimal
    value /= bv;
    assert(value == 0 && "operator/= should return 0 when dividing by a BitVector");
}

/// @brief Tests for `operator/=` with a BitVector divided by another larger BitVector.
void test_operator_divide_equal_larger_bitvector()
{
    bvlib::BitVector<8> bv1("00000001");          // 1 in decimal
    bvlib::BitVector<16> bv2("0000000100000000"); // Large BitVector with value 256
    bv1 /= bv2;
    assert(bv1.to_string() == "00000000" && "operator/= should return 0 when the divisor is larger than the dividend");
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
    std::cout << "Running all function tests." << std::endl;

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

    std::cout << "✅ All function tests passed!" << std::endl;
    return 0;
}
