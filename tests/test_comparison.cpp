#include "bvlib/math.hpp"

#include <cassert>
#include <iostream>

// ============================================================================
// BOOL (==)
// ============================================================================

/// @brief Tests equality between two BitVectors of the same size.
void test_operator_equal_same_size()
{
    // Test case 1: Same BitVector, equal
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("11001100");
    assert(bv8_1 == bv8_2 && "operator== should return true for equal BitVectors of same size");

    // Test case 2: Same BitVector, different value
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("10101010");
    assert(!(bv8_3 == bv8_4) && "operator== should return false for unequal BitVectors of same size");
}

/// @brief Tests equality between a BitVector and an integer value.
void test_operator_equal_bitvector_integer()
{
    // Test case 1: BitVector equals integer
    bvlib::BitVector<8> bv8_1("11001100");
    int value = 204;
    assert(bv8_1 == value && "operator== should return true when BitVector equals integer");

    // Test case 2: BitVector does not equal integer
    bvlib::BitVector<8> bv8_2("11001100");
    int value2 = 123;
    assert(!(bv8_2 == value2) && "operator== should return false when BitVector does not equal integer");
}

/// @brief Tests equality between an integer value and a BitVector.
void test_operator_equal_integer_bitvector()
{
    // Test case 1: Integer equals BitVector
    int value = 204;
    bvlib::BitVector<8> bv8_1("11001100");
    assert(value == bv8_1 && "operator== should return true when integer equals BitVector");

    // Test case 2: Integer does not equal BitVector
    int value2 = 123;
    bvlib::BitVector<8> bv8_2("11001100");
    assert(!(value2 == bv8_2) && "operator== should return false when integer does not equal BitVector");
}

/// @brief Tests equality between BitVectors of different sizes.
void test_operator_equal_different_sizes()
{
    // Test case 1: BitVectors with different sizes, should be false
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    assert(!(bv8_1 == bv16_1) && "operator== should return false for BitVectors with different sizes");

    // Test case 2: BitVector with extra leading zeros, should be equal
    bvlib::BitVector<8> bv8_2("11001100");
    bvlib::BitVector<16> bv16_2("0000000011001100");
    assert(bv8_2 == bv16_2 && "operator== should return true for BitVectors with equal bits and different sizes");
}

/// @brief Tests equality with a BitVector and an integer value.
void test_operator_equal_bitvector_integer_extended()
{
    // Test case 1: BitVector and integer value with all bits set
    bvlib::BitVector<16> bv16_1("1111111111111111");
    int value = 65535;
    assert(bv16_1 == value && "operator== should return true when BitVector and integer match");

    // Test case 2: BitVector and integer value with different bits
    bvlib::BitVector<16> bv16_2("1111000011110000");
    int value2 = 61680;
    assert(bv16_2 == value2 && "operator== should return true when BitVector and integer match");

    // Test case 3: BitVector and integer mismatch
    bvlib::BitVector<8> bv8_1("11001100");
    int value3 = 123;
    assert(!(bv8_1 == value3) && "operator== should return false when BitVector and integer do not match");
}

/// @brief Tests equality with a BitVector and a truncated integer.
void test_operator_equal_truncated_integer()
{
    // Test case 1: Truncated BitVector and integer
    bvlib::BitVector<4> bv4_1("1100");
    int value = 12;
    assert(bv4_1 == value && "operator== should return true when truncated BitVector and integer match");

    // Test case 2: Truncated BitVector and integer mismatch
    bvlib::BitVector<4> bv4_2("1000");
    int value2 = 5;
    assert(!(bv4_2 == value2) && "operator== should return false when truncated BitVector and integer do not match");
}

/// @brief Tests equality between BitVectors of equal size but different data.
void test_operator_equal_bitvectors_different_data()
{
    // Test case 1: Equal size but different data
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("10101010");
    assert(!(bv8_1 == bv8_2) && "operator== should return false for BitVectors with the same size but different data");

    // Test case 2: Equal size but same data
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11110000");
    assert(bv8_3 == bv8_4 && "operator== should return true for BitVectors with the same size and same data");
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
    assert(!(bv8_1 != bv8_2) && "operator!= should return false for equal BitVectors of same size");

    // Test case 2: Same BitVector, different value
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("10101010");
    assert(bv8_3 != bv8_4 && "operator!= should return true for unequal BitVectors of same size");
}

/// @brief Tests inequality between a BitVector and an integer value.
void test_operator_not_equal_bitvector_integer()
{
    // Test case 1: BitVector equals integer
    bvlib::BitVector<8> bv8_1("11001100");
    int value = 204;
    assert(!(bv8_1 != value) && "operator!= should return false when BitVector equals integer");

    // Test case 2: BitVector does not equal integer
    bvlib::BitVector<8> bv8_2("11001100");
    int value2 = 123;
    assert(bv8_2 != value2 && "operator!= should return true when BitVector does not equal integer");
}

/// @brief Tests inequality between an integer value and a BitVector.
void test_operator_not_equal_integer_bitvector()
{
    // Test case 1: Integer equals BitVector
    int value = 204;
    bvlib::BitVector<8> bv8_1("11001100");
    assert(!(value != bv8_1) && "operator!= should return false when integer equals BitVector");

    // Test case 2: Integer does not equal BitVector
    int value2 = 123;
    bvlib::BitVector<8> bv8_2("11001100");
    assert(value2 != bv8_2 && "operator!= should return true when integer does not equal BitVector");
}

/// @brief Tests inequality between BitVectors of different sizes.
void test_operator_not_equal_different_sizes()
{
    // Test case 1: BitVectors with different sizes, should be true
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    assert(bv8_1 != bv16_1 && "operator!= should return true for BitVectors with different sizes");

    // Test case 2: BitVector with extra leading zeros, should be false
    bvlib::BitVector<8> bv8_2("11001100");
    bvlib::BitVector<16> bv16_2("0000000011001100");
    assert(!(bv8_2 != bv16_2) && "operator!= should return false for BitVectors with equal bits and different sizes");
}

/// @brief Tests inequality with a BitVector and an integer value.
void test_operator_not_equal_bitvector_integer_extended()
{
    // Test case 1: BitVector and integer value with all bits set
    bvlib::BitVector<16> bv16_1("1111111111111111");
    int value = 65535;
    assert(!(bv16_1 != value) && "operator!= should return false when BitVector and integer match");

    // Test case 2: BitVector and integer value with different bits
    bvlib::BitVector<16> bv16_2("1111000011110000");
    int value2 = 61680;
    assert(!(bv16_2 != value2) && "operator!= should return false when BitVector and integer match");

    // Test case 3: BitVector and integer mismatch
    bvlib::BitVector<8> bv8_1("11001100");
    int value3 = 123;
    assert(bv8_1 != value3 && "operator!= should return true when BitVector and integer do not match");
}

/// @brief Tests inequality with a BitVector and a truncated integer.
void test_operator_not_equal_truncated_integer()
{
    // Test case 1: Truncated BitVector and integer
    bvlib::BitVector<4> bv4_1("1100");
    int value = 12;
    assert(!(bv4_1 != value) && "operator!= should return false when truncated BitVector and integer match");

    // Test case 2: Truncated BitVector and integer mismatch
    bvlib::BitVector<4> bv4_2("1000");
    int value2 = 5;
    assert(bv4_2 != value2 && "operator!= should return true when truncated BitVector and integer do not match");
}

/// @brief Tests inequality between BitVectors of equal size but different data.
void test_operator_not_equal_bitvectors_different_data()
{
    // Test case 1: Equal size but different data
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<8> bv8_2("10101010");
    assert(bv8_1 != bv8_2 && "operator!= should return true for BitVectors with the same size but different data");

    // Test case 2: Equal size but same data
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11110000");
    assert(!(bv8_3 != bv8_4) && "operator!= should return false for BitVectors with the same size and same data");
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
    assert(bv8_1 < bv8_2 && "operator< should return true when lhs is less than rhs");

    // Test case 2: lhs > rhs (lhs is greater than rhs)
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11001100");
    assert(!(bv8_3 < bv8_4) && "operator< should return false when lhs is greater than rhs");

    // Test case 3: lhs == rhs (lhs is equal to rhs)
    bvlib::BitVector<8> bv8_5("11001100");
    bvlib::BitVector<8> bv8_6("11001100");
    assert(!(bv8_5 < bv8_6) && "operator< should return false when lhs is equal to rhs");
}

/// @brief Tests the operator< for a BitVector and an integer value.
void test_operator_less_than_bitvector_integer()
{
    // Test case 1: BitVector < integer (lhs < rhs)
    bvlib::BitVector<8> bv8_1("11001100");
    int value = 205;
    assert(bv8_1 < value && "operator< should return true when BitVector is less than integer");

    // Test case 2: BitVector > integer (lhs > rhs)
    bvlib::BitVector<8> bv8_2("11110000");
    int value2 = 204;
    assert(!(bv8_2 < value2) && "operator< should return false when BitVector is greater than integer");

    // Test case 3: BitVector == integer (lhs == rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    int value3 = 204;
    assert(!(bv8_3 < value3) && "operator< should return false when BitVector equals integer");
}

/// @brief Tests the operator< for an integer value and a BitVector.
void test_operator_less_than_integer_bitvector()
{
    // Test case 1: integer < BitVector (lhs < rhs)
    int value = 203;
    bvlib::BitVector<8> bv8_1("11001100");
    assert(value < bv8_1 && "operator< should return true when integer is less than BitVector");

    // Test case 2: integer > BitVector (lhs > rhs)
    int value2 = 205;                      // 11100100
    bvlib::BitVector<8> bv8_2("11001100"); // 11001100
    assert(!(value2 < bv8_2) && "operator< should return false when integer is greater than BitVector");

    // Test case 3: integer == BitVector (lhs == rhs)
    int value3 = 204;
    bvlib::BitVector<8> bv8_3("11001100");
    assert(!(value3 < bv8_3) && "operator< should return false when integer equals BitVector");
}

/// @brief Tests the operator< for BitVectors of different sizes.
void test_operator_less_than_different_sizes()
{
    // Test case 1: BitVector<8> < BitVector<16> (lhs < rhs)
    bvlib::BitVector<8> bv8_1("11001100");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    assert(bv8_1 < bv16_1 && "operator< should return true when lhs is a smaller BitVector than rhs");

    // Test case 2: BitVector<16> < BitVector<8> (lhs > rhs)
    bvlib::BitVector<16> bv16_2("1100110011001100");
    bvlib::BitVector<8> bv8_2("11001100");
    assert(!(bv16_2 < bv8_2) && "operator< should return false when lhs is larger than rhs");

    // Test case 3: BitVector<8> == BitVector<16> (lhs == rhs if the same value is extended)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<16> bv16_3("0000000011001100");
    assert(!(bv8_3 < bv16_3) && "operator< should return false for BitVectors with the same bits but different sizes");
}

/// @brief Tests the operator< for larger BitVectors.
void test_operator_less_than_large_bitvectors()
{
    // Test case 1: Shifted comparison of larger BitVectors
    bvlib::BitVector<64> bv64_1("1100110011001100110011001100110011001100110011001100110011001100");
    bvlib::BitVector<64> bv64_2("1111000011110000111100001111000011110000111100001111000011110000");
    assert(
        bv64_1 < bv64_2 && "operator< should return true when the first BitVector is smaller in value than the second");

    // Test case 2: Comparison with BitVector containing all 1s
    bvlib::BitVector<64> bv64_3("1111111111111111111111111111111111111111111111111111111111111111");
    bvlib::BitVector<64> bv64_4("0000000000000000000000000000000000000000000000000000000000000000");
    assert(!(bv64_3 < bv64_4) && "operator< should return false when BitVector is all 1s and the other is all 0s");
}

/// @brief Tests the operator< for unequal BitVectors of the same size but different values.
void test_operator_less_than_different_data()
{
    // Test case 1: Unequal BitVectors of the same size (lhs < rhs)
    bvlib::BitVector<8> bv8_1("10101010");
    bvlib::BitVector<8> bv8_2("11001100");
    assert(bv8_1 < bv8_2 && "operator< should return true for BitVectors of the same size but different values");

    // Test case 2: Unequal BitVectors of the same size (lhs > rhs)
    bvlib::BitVector<8> bv8_3("11110000");
    bvlib::BitVector<8> bv8_4("11001100");
    assert(!(bv8_3 < bv8_4) && "operator< should return false for BitVectors of the same size but lhs > rhs");

    // Test case 3: Unequal BitVectors of the same size (lhs == rhs)
    bvlib::BitVector<8> bv8_5("11110000");
    bvlib::BitVector<8> bv8_6("11110000");
    assert(!(bv8_5 < bv8_6) && "operator< should return false for BitVectors with the same size and same value");
}

/// @brief Tests the operator< for truncated BitVectors and integers.
void test_operator_less_than_truncated_integer()
{
    // Test case 1: Truncated BitVector < integer
    bvlib::BitVector<4> bv4_1("11100");
    int value = 12;
    assert(!(bv4_1 < value) && "operator< should return false when truncated BitVector equals integer");

    // Test case 2: Truncated BitVector < integer
    bvlib::BitVector<4> bv4_2("10000");
    int value2 = 5;
    assert(bv4_2 < value2 && "operator< should return true when truncated BitVector is less than integer");
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
    assert(bv8_1 > bv8_2 && "operator> should return true when lhs is greater than rhs");

    // Test case 2: lhs < rhs (lhs is smaller than rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("11110000");
    assert(!(bv8_3 > bv8_4) && "operator> should return false when lhs is smaller than rhs");

    // Test case 3: lhs == rhs (lhs is equal to rhs)
    bvlib::BitVector<8> bv8_5("11001100");
    bvlib::BitVector<8> bv8_6("11001100");
    assert(!(bv8_5 > bv8_6) && "operator> should return false when lhs is equal to rhs");
}

/// @brief Tests the operator> for a BitVector and an integer value.
void test_operator_greater_than_bitvector_integer()
{
    // Test case 1: BitVector > integer (lhs > rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    int value = 204;
    assert(bv8_1 > value && "operator> should return true when BitVector is greater than integer");

    // Test case 2: BitVector < integer (lhs < rhs)
    bvlib::BitVector<8> bv8_2("11001100");
    int value2 = 204;
    assert(!(bv8_2 > value2) && "operator> should return false when BitVector is less than integer");

    // Test case 3: BitVector == integer (lhs == rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    int value3 = 204;
    assert(!(bv8_3 > value3) && "operator> should return false when BitVector equals integer");
}

/// @brief Tests the operator> for an integer value and a BitVector.
void test_operator_greater_than_integer_bitvector()
{
    // Test case 1: integer > BitVector (lhs > rhs)
    int value = 250;
    bvlib::BitVector<8> bv8_1("11110000");
    assert(value > bv8_1 && "operator> should return true when integer is greater than BitVector");

    // Test case 2: integer < BitVector (lhs < rhs)
    int value2 = 100;
    bvlib::BitVector<8> bv8_2("11001100");
    assert(!(value2 > bv8_2) && "operator> should return false when integer is less than BitVector");

    // Test case 3: integer == BitVector (lhs == rhs)
    int value3 = 204;
    bvlib::BitVector<8> bv8_3("11001100");
    assert(!(value3 > bv8_3) && "operator> should return false when integer equals BitVector");
}

/// @brief Tests the operator> for BitVectors of different sizes.
void test_operator_greater_than_different_sizes()
{
    // Test case 1: BitVector<8> > BitVector<16> (lhs > rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    bvlib::BitVector<16> bv16_1("1100110011001100");
    assert(!(bv8_1 > bv16_1) && "operator> should return false when lhs is a smaller BitVector than rhs");

    // Test case 2: BitVector<16> > BitVector<8> (lhs > rhs)
    bvlib::BitVector<16> bv16_2("1100110011001100");
    bvlib::BitVector<8> bv8_2("11110000");
    assert(bv16_2 > bv8_2 && "operator> should return true when lhs is larger than rhs");

    // Test case 3: BitVector<8> == BitVector<16> (lhs == rhs if the same value is extended)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<16> bv16_3("0000000011001100");
    assert(!(bv8_3 > bv16_3) && "operator> should return false for BitVectors with the same bits but different sizes");
}

/// @brief Tests the operator> for larger BitVectors.
void test_operator_greater_than_large_bitvectors()
{
    // Test case 1: Shifted comparison of larger BitVectors
    bvlib::BitVector<64> bv64_1("1111000011110000111100001111000011110000111100001111000011110000");
    bvlib::BitVector<64> bv64_2("1100110011001100110011001100110011001100110011001100110011001100");
    assert(
        bv64_1 > bv64_2 && "operator> should return true when the first BitVector is greater in value than the second");

    // Test case 2: Comparison with BitVector containing all 0s
    bvlib::BitVector<64> bv64_3("0000000000000000000000000000000000000000000000000000000000000000");
    bvlib::BitVector<64> bv64_4("1111111111111111111111111111111111111111111111111111111111111111");
    assert(!(bv64_3 > bv64_4) && "operator> should return false when BitVector is all 0s and the other is all 1s");
}

/// @brief Tests the operator> for unequal BitVectors of the same size but different values.
void test_operator_greater_than_different_data()
{
    // Test case 1: Unequal BitVectors of the same size (lhs > rhs)
    bvlib::BitVector<8> bv8_1("11110000");
    bvlib::BitVector<8> bv8_2("11001100");
    assert(bv8_1 > bv8_2 && "operator> should return true for BitVectors of the same size but different values");

    // Test case 2: Unequal BitVectors of the same size (lhs < rhs)
    bvlib::BitVector<8> bv8_3("11001100");
    bvlib::BitVector<8> bv8_4("11110000");
    assert(!(bv8_3 > bv8_4) && "operator> should return false for BitVectors of the same size but lhs < rhs");

    // Test case 3: Unequal BitVectors of the same size (lhs == rhs)
    bvlib::BitVector<8> bv8_5("11110000");
    bvlib::BitVector<8> bv8_6("11110000");
    assert(!(bv8_5 > bv8_6) && "operator> should return false for BitVectors with the same size and same value");
}

/// @brief Tests the operator> for truncated BitVectors and integers.
void test_operator_greater_than_truncated_integer()
{
    // Test case 1: Truncated BitVector > integer
    bvlib::BitVector<4> bv4_1("1100");
    int value = 12;
    assert(!(bv4_1 > value) && "operator> should return false when truncated BitVector equals integer");

    // Test case 2: Truncated BitVector > integer
    bvlib::BitVector<4> bv4_2("1000");
    int value2 = 5;
    assert(bv4_2 > value2 && "operator> should return true when truncated BitVector is greater than integer");
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
    std::cout << "Running all function tests." << std::endl;

    run_operator_equal_all_tests();
    run_operator_not_equal_all_tests();
    run_operator_less_than_all_tests();
    run_operator_greater_than_all_tests();

    std::cout << "✅ All function tests passed!" << std::endl;
    return 0;
}
