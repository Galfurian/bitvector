/// @file test_bitwise.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests bitwise operators.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include <bitset>
#include <cassert>
#include <iostream>

#include "bvlib/math.hpp"

/// @brief Tests for `&` (bitwise AND) operator for `BitVector`.
void test_bitwise_and()
{
    // ========================================================================
    // Test cases with N = 8 (BitVector<8>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector (should remain unchanged)
        bvlib::BitVector<8> bv8_0("11001100");
        bv8_0 = bv8_0 & bv8_0;
        assert(bv8_0.to_string() == "11001100" && "AND with same BitVector should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<8> bv8_1("11001100");
        bvlib::BitVector<8> bv8_ones("11111111");
        bv8_1 = bv8_1 & bv8_ones;
        assert(bv8_1.to_string() == "11001100" && "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<8> bv8_2("11001100");
        bvlib::BitVector<8> bv8_zeros("00000000");
        bv8_2 = bv8_2 & bv8_zeros;
        assert(bv8_2.to_string() == "00000000" && "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors
        bvlib::BitVector<8> bv8_3("11001100");
        bvlib::BitVector<8> bv8_4("10101010");
        bv8_3 = bv8_3 & bv8_4;
        assert(bv8_3.to_string() == "10001000" && "AND between two different BitVectors");
    }
    // ========================================================================
    // Test cases with N = 16 (BitVector<16>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector (should remain unchanged)
        bvlib::BitVector<16> bv16_0("1100110011001100");
        bv16_0 = bv16_0 & bv16_0;
        assert(bv16_0.to_string() == "1100110011001100" && "AND with same BitVector should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<16> bv16_1("1100110011001100");
        bvlib::BitVector<16> bv16_ones("1111111111111111");
        bv16_1 = bv16_1 & bv16_ones;
        assert(bv16_1.to_string() == "1100110011001100" && "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<16> bv16_2("1100110011001100");
        bvlib::BitVector<16> bv16_zeros("0000000000000000");
        bv16_2 = bv16_2 & bv16_zeros;
        assert(bv16_2.to_string() == "0000000000000000" && "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors
        bvlib::BitVector<16> bv16_3("1100110011001100");
        bvlib::BitVector<16> bv16_4("1010101010101010");
        bv16_3 = bv16_3 & bv16_4;
        assert(bv16_3.to_string() == "1000100010001000" && "AND between two different BitVectors");
    }
    // ========================================================================
    // Test cases with N = 32 (BitVector<32>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector (should remain unchanged)
        bvlib::BitVector<32> bv32_0("11001100110011001100110011001100");
        bv32_0 = bv32_0 & bv32_0;
        assert(
            bv32_0.to_string() == "11001100110011001100110011001100" &&
            "AND with same BitVector should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<32> bv32_1("11001100110011001100110011001100");
        bvlib::BitVector<32> bv32_ones("11111111111111111111111111111111");
        bv32_1 = bv32_1 & bv32_ones;
        assert(
            bv32_1.to_string() == "11001100110011001100110011001100" &&
            "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<32> bv32_2("11001100110011001100110011001100");
        bvlib::BitVector<32> bv32_zeros("00000000000000000000000000000000");
        bv32_2 = bv32_2 & bv32_zeros;
        assert(
            bv32_2.to_string() == "00000000000000000000000000000000" &&
            "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors
        bvlib::BitVector<32> bv32_3("11001100110011001100110011001100");
        bvlib::BitVector<32> bv32_4("10101010101010101010101010101010");
        bv32_3 = bv32_3 & bv32_4;
        assert(bv32_3.to_string() == "10001000100010001000100010001000" && "AND between two different BitVectors");
    }
    // ========================================================================
    // Test cases with unequal BitVector sizes
    // ========================================================================
    {
        // Test case 1: AND between BitVector<8> and BitVector<16> (smaller BitVector extended)
        bvlib::BitVector<8> bv8("11001100");
        bvlib::BitVector<16> bv16("1111000011110000");
        auto result1 = bv8 & bv16;
        assert(result1.to_string() == "0000000011000000" && "AND between BitVector<8> and BitVector<16>");

        // Test case 2: AND between BitVector<16> and BitVector<8> (smaller BitVector extended)
        bvlib::BitVector<16> bv16_2("1111000011110000");
        bvlib::BitVector<8> bv8_2("11001100");
        auto result2 = bv16_2 & bv8_2;
        assert(result2.to_string() == "0000000011000000" && "AND between BitVector<16> and BitVector<8>");
    }
    // ========================================================================
    // Test cases for BitVector and integer (BitVector<8> and integer)
    // ========================================================================
    {
        // Test case 1: AND between BitVector and integer
        bvlib::BitVector<8> bv8_3("11001100");
        int int_val  = 255; // 11111111 in binary
        auto result3 = bv8_3 & int_val;
        assert(result3.to_string() == "11001100" && "AND between BitVector and integer");

        // Test case 2: AND between integer and BitVector
        int int_val2 = 170; // 10101010 in binary
        auto result4 = int_val2 & bv8_3;
        assert(result4.to_string() == "10001000" && "AND between integer and BitVector");
    }
    // ========================================================================
    // Test cases for in-place AND (operator &=)
    // ========================================================================
    {
        // Test case 1: In-place AND between two BitVectors
        bvlib::BitVector<8> bv8_4("11001100");
        bvlib::BitVector<8> bv8_5("10101010");
        bv8_4 &= bv8_5;
        assert(bv8_4.to_string() == "10001000" && "In-place AND between two BitVectors");

        // Test case 2: In-place AND between BitVector and integer
        bvlib::BitVector<8> bv8_6("11001100");
        int int_val3 = 170; // 10101010 in binary
        bv8_6 &= int_val3;
        assert(bv8_6.to_string() == "10001000" && "In-place AND between BitVector and integer");
    }

    // ========================================================================
    // Test cases with N = 9 (BitVector<9>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector<9> (should remain unchanged)
        bvlib::BitVector<9> bv9_0("110011001");
        bv9_0 = bv9_0 & bv9_0;
        assert(bv9_0.to_string() == "110011001" && "AND with same BitVector<9> should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<9> bv9_1("110011001");
        bvlib::BitVector<9> bv9_ones("111111111");
        bv9_1 = bv9_1 & bv9_ones;
        assert(bv9_1.to_string() == "110011001" && "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<9> bv9_2("110011001");
        bvlib::BitVector<9> bv9_zeros("000000000");
        bv9_2 = bv9_2 & bv9_zeros;
        assert(bv9_2.to_string() == "000000000" && "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors<9>
        bvlib::BitVector<9> bv9_3("110011001");
        bvlib::BitVector<9> bv9_4("101010101");
        bv9_3 = bv9_3 & bv9_4;
        assert(bv9_3.to_string() == "100010001" && "AND between two different BitVectors<9>");
    }
    // ========================================================================
    // Test cases with N = 15 (BitVector<15>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector<15> (should remain unchanged)
        bvlib::BitVector<15> bv15_0("110011001100110");
        bv15_0 = bv15_0 & bv15_0;
        assert(bv15_0.to_string() == "110011001100110" && "AND with same BitVector<15> should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<15> bv15_1("110011001100110");
        bvlib::BitVector<15> bv15_ones("111111111111111");
        bv15_1 = bv15_1 & bv15_ones;
        assert(bv15_1.to_string() == "110011001100110" && "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<15> bv15_2("110011001100110");
        bvlib::BitVector<15> bv15_zeros("000000000000000");
        bv15_2 = bv15_2 & bv15_zeros;
        assert(bv15_2.to_string() == "000000000000000" && "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors<15>
        bvlib::BitVector<15> bv15_3("110011001100110");
        bvlib::BitVector<15> bv15_4("101010101010101");
        bv15_3 = bv15_3 & bv15_4;
        assert(bv15_3.to_string() == "100010001000100" && "AND between two different BitVectors<15>");
    }
    // ========================================================================
    // Test cases with N = 23 (BitVector<23>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector<23> (should remain unchanged)
        bvlib::BitVector<23> bv23_0("11001100110011001100100");
        bv23_0 = bv23_0 & bv23_0;
        assert(
            bv23_0.to_string() == "11001100110011001100100" && "AND with same BitVector<23> should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<23> bv23_1("11001100110011001100100");
        bvlib::BitVector<23> bv23_ones("11111111111111111111111");
        bv23_1 = bv23_1 & bv23_ones;
        assert(
            bv23_1.to_string() == "11001100110011001100100" && "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<23> bv23_2("11001100110011001100100");
        bvlib::BitVector<23> bv23_zeros("00000000000000000000000");
        bv23_2 = bv23_2 & bv23_zeros;
        assert(bv23_2.to_string() == "00000000000000000000000" && "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors<23>
        bvlib::BitVector<23> bv23_3("11001100110011001100100");
        bvlib::BitVector<23> bv23_4("10101010101010101010101");
        bv23_3 = bv23_3 & bv23_4;
        assert(bv23_3.to_string() == "10001000100010001000100" && "AND between two different BitVectors<23>");
    }
    // ========================================================================
    // Test cases with N = 30 (BitVector<30>)
    // ========================================================================
    {
        // Test case 1: AND with same BitVector<30> (should remain unchanged)
        bvlib::BitVector<30> bv30_0("110011001100110011001100110011");
        bv30_0 = bv30_0 & bv30_0;
        assert(
            bv30_0.to_string() == "110011001100110011001100110011" &&
            "AND with same BitVector<30> should remain unchanged");

        // Test case 2: AND with all ones (should be same as original)
        bvlib::BitVector<30> bv30_1("110011001100110011001100110011");
        bvlib::BitVector<30> bv30_ones("111111111111111111111111111111");
        bv30_1 = bv30_1 & bv30_ones;
        assert(
            bv30_1.to_string() == "110011001100110011001100110011" &&
            "AND with all ones should be the same as the original");

        // Test case 3: AND with all zeros (should be all zeros)
        bvlib::BitVector<30> bv30_2("110011001100110011001100110011");
        bvlib::BitVector<30> bv30_zeros("000000000000000000000000000000");
        bv30_2 = bv30_2 & bv30_zeros;
        assert(
            bv30_2.to_string() == "000000000000000000000000000000" && "AND with all zeros should result in all zeros");

        // Test case 4: AND between two different BitVectors<30>
        bvlib::BitVector<30> bv30_3("110011001100110011001100110011");
        bvlib::BitVector<30> bv30_4("101010101010101010101010101010");
        bv30_3 = bv30_3 & bv30_4;
        assert(bv30_3.to_string() == "100010001000100010001000100010" && "AND between two different BitVectors<30>");
    }
}

/// @brief Tests for `|` (bitwise OR) operator for `BitVector`.
void test_bitwise_or()
{
    // ========================================================================
    // Test cases with N = 8 (BitVector<8>)
    // ========================================================================
    {
        // Test case 1: OR with same BitVector<8> (should remain unchanged)
        bvlib::BitVector<8> bv8_0("11001100");
        bv8_0 = bv8_0 | bv8_0;
        assert(bv8_0.to_string() == "11001100" && "OR with same BitVector<8> should remain unchanged");

        // Test case 2: OR with all ones (should be the same as all ones)
        bvlib::BitVector<8> bv8_1("11001100");
        bvlib::BitVector<8> bv8_ones("11111111");
        bv8_1 = bv8_1 | bv8_ones;
        assert(bv8_1.to_string() == "11111111" && "OR with all ones should result in all ones");

        // Test case 3: OR with all zeros (should be same as original)
        bvlib::BitVector<8> bv8_2("11001100");
        bvlib::BitVector<8> bv8_zeros("00000000");
        bv8_2 = bv8_2 | bv8_zeros;
        assert(bv8_2.to_string() == "11001100" && "OR with all zeros should remain the same as the original");

        // Test case 4: OR between two different BitVectors
        bvlib::BitVector<8> bv8_3("11001100");
        bvlib::BitVector<8> bv8_4("10101010");
        bv8_3 = bv8_3 | bv8_4;
        assert(bv8_3.to_string() == "11101110" && "OR between two different BitVectors<8>");
    }
    // ========================================================================
    // Test cases with N = 16 (BitVector<16>)
    // ========================================================================
    {
        // Test case 1: OR with same BitVector<16> (should remain unchanged)
        bvlib::BitVector<16> bv16_0("1100110011001100");
        bv16_0 = bv16_0 | bv16_0;
        assert(bv16_0.to_string() == "1100110011001100" && "OR with same BitVector<16> should remain unchanged");

        // Test case 2: OR with all ones (should be the same as all ones)
        bvlib::BitVector<16> bv16_1("1100110011001100");
        bvlib::BitVector<16> bv16_ones("1111111111111111");
        bv16_1 = bv16_1 | bv16_ones;
        assert(bv16_1.to_string() == "1111111111111111" && "OR with all ones should result in all ones");

        // Test case 3: OR with all zeros (should be same as original)
        bvlib::BitVector<16> bv16_2("1100110011001100");
        bvlib::BitVector<16> bv16_zeros("0000000000000000");
        bv16_2 = bv16_2 | bv16_zeros;
        assert(bv16_2.to_string() == "1100110011001100" && "OR with all zeros should remain the same as the original");

        // Test case 4: OR between two different BitVectors
        bvlib::BitVector<16> bv16_3("1100110011001100");
        bvlib::BitVector<16> bv16_4("1010101010101010");
        bv16_3 = bv16_3 | bv16_4;
        assert(bv16_3.to_string() == "1110111011101110" && "OR between two different BitVectors<16>");
    }
    // ========================================================================
    // Test cases with N = 32 (BitVector<32>)
    // ========================================================================
    {
        // Test case 1: OR with same BitVector<32> (should remain unchanged)
        bvlib::BitVector<32> bv32_0("11001100110011001100110011001100");
        bv32_0 = bv32_0 | bv32_0;
        assert(
            bv32_0.to_string() == "11001100110011001100110011001100" &&
            "OR with same BitVector<32> should remain unchanged");

        // Test case 2: OR with all ones (should be the same as all ones)
        bvlib::BitVector<32> bv32_1("11001100110011001100110011001100");
        bvlib::BitVector<32> bv32_ones("11111111111111111111111111111111");
        bv32_1 = bv32_1 | bv32_ones;
        assert(
            bv32_1.to_string() == "11111111111111111111111111111111" && "OR with all ones should result in all ones");

        // Test case 3: OR with all zeros (should be same as original)
        bvlib::BitVector<32> bv32_2("11001100110011001100110011001100");
        bvlib::BitVector<32> bv32_zeros("00000000000000000000000000000000");
        bv32_2 = bv32_2 | bv32_zeros;
        assert(
            bv32_2.to_string() == "11001100110011001100110011001100" &&
            "OR with all zeros should remain the same as the original");

        // Test case 4: OR between two different BitVectors
        bvlib::BitVector<32> bv32_3("11001100110011001100110011001100");
        bvlib::BitVector<32> bv32_4("10101010101010101010101010101010");
        bv32_3 = bv32_3 | bv32_4;
        assert(bv32_3.to_string() == "11101110111011101110111011101110" && "OR between two different BitVectors<32>");
    }
    // ========================================================================
    // Test cases with N = 65 (BitVector<65>)
    // ========================================================================
    {
        // Test case 1: OR with same BitVector<65> (should remain unchanged)
        bvlib::BitVector<65> bv65_0("1100110011001100110011001100110011001100");
        bv65_0 = bv65_0 | bv65_0;
        assert(
            bv65_0.to_string() == "00000000000000000000000001100110011001100110011001100110011001100" &&
            "OR with same BitVector<65> should remain unchanged");

        // Test case 2: OR with all ones (should be the same as all ones)
        bvlib::BitVector<65> bv65_1("1100110011001100110011001100110011001100");
        bvlib::BitVector<65> bv65_ones("1111111111111111111111111111111111111111");
        bv65_1 = bv65_1 | bv65_ones;
        assert(
            bv65_1.to_string() == "00000000000000000000000001111111111111111111111111111111111111111" &&
            "OR with all ones should result in all ones");

        // Test case 3: OR with all zeros (should be same as original)
        bvlib::BitVector<65> bv65_2("1100110011001100110011001100110011001100");
        bvlib::BitVector<65> bv65_zeros("0000000000000000000000000000000000000000");
        bv65_2 = bv65_2 | bv65_zeros;
        assert(
            bv65_2.to_string() == "00000000000000000000000001100110011001100110011001100110011001100" &&
            "OR with all zeros should remain the same as the original");

        // Test case 4: OR between two different BitVectors
        bvlib::BitVector<65> bv65_3("1100110011001100110011001100110011001100");
        bvlib::BitVector<65> bv65_4("1010101010101010101010101010101010101010");
        bv65_3 = bv65_3 | bv65_4;
        assert(
            bv65_3.to_string() == "00000000000000000000000001110111011101110111011101110111011101110" &&
            "OR between two different BitVectors<65>");
    }
    // ========================================================================
    // Test cases with N = 7, 15, 23, 30, 126 (non-standard sizes)
    // ========================================================================
    {
        // Test case 1: OR between BitVector<7> and BitVector<8>
        bvlib::BitVector<7> bv7("1100110");
        bvlib::BitVector<8> bv8("11111111");
        auto result1 = bv7 | bv8;
        assert(result1.to_string() == "11111111" && "OR between BitVector<7> and BitVector<8>");

        // Test case 2: OR between BitVector<15> and BitVector<8>
        bvlib::BitVector<15> bv15("110011001100110");
        bvlib::BitVector<8> bv8_2("10101010");
        auto result2 = bv15 | bv8_2;
        assert(result2.to_string() == "110011011101110" && "OR between BitVector<15> and BitVector<8>");

        // Test case 3: OR between BitVector<23> and BitVector<15>
        bvlib::BitVector<23> bv23("11001100110011001100100");
        bvlib::BitVector<15> bv15_2("101010101010101");
        auto result3 = bv23 | bv15_2;
        assert(result3.to_string() == "11001100111011101110101" && "OR between BitVector<23> and BitVector<15>");

        // Test case 4: OR between BitVector<30> and BitVector<23>
        bvlib::BitVector<30> bv30("110011001100110011001100110011");
        bvlib::BitVector<23> bv23_2("10101010101010101010101");
        auto result4 = bv30 | bv23_2;
        assert(result4.to_string() == "110011011101110111011101110111" && "OR between BitVector<30> and BitVector<23>");

        // Test case 5: OR between BitVector<126> and BitVector<30>
        bvlib::BitVector<126> bv126("110011001100110011001100110011001100110011001100110011001100110011001100110011");
        bvlib::BitVector<30> bv30_2("10101010101010101010101");
        auto result5 = bv126 | bv30_2;
        assert(
            result5.to_string() == "00000000000000000000000000000000000000000000000011001100110011001100110011001100110"
                                   "0110011001100110011011101110111011101110111" &&
            "OR between BitVector<126> and BitVector<30>");
    }
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    test_bitwise_and();
    std::cout << "    ✅ All `&` tests completed!\n";

    test_bitwise_or();
    std::cout << "    ✅ All `|` tests completed!\n";

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
