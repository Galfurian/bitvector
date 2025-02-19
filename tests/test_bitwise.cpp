/// @file test_bitwise.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests bitwise operators.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include <bitset>
#include <cassert>
#include <iostream>

#include "support.hpp"

void test_bitwise_and()
{
    // Test 1: BitVector<8> AND BitVector<8>
    bvlib::BitVector<8> bv8a("11001100");
    bvlib::BitVector<8> bv8b("00101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv8a, bv8b, "00001000");

    // Test 2: BitVector<8> AND BitVector<16>
    bvlib::BitVector<8> bv8c("11001100");
    bvlib::BitVector<16> bv16("1010101010101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv8c, bv16, "0000000010001000");

    // Test 3: BitVector<16> AND BitVector<16>
    bvlib::BitVector<16> bv16a("1100110011001100");
    bvlib::BitVector<16> bv16b("1010101010101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv16a, bv16b, "1000100010001000");

    // Test 4: BitVector<16> AND BitVector<8>
    bvlib::BitVector<16> bv16c("1100110011001100");
    bvlib::BitVector<8> bv8d("10101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv16c, bv8d, "0000000010001000");

    // Test 5: BitVector<32> AND BitVector<32>
    bvlib::BitVector<32> bv32a("11001100110011001100110011001100");
    bvlib::BitVector<32> bv32b("10101010101010101010101010101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv32a, bv32b, "10001000100010001000100010001000");

    // Test 6: BitVector<32> AND BitVector<16>
    bvlib::BitVector<32> bv32c("11001100110011001100110011001100");
    bvlib::BitVector<16> bv16d("1010101010101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv32c, bv16d, "00000000000000001000100010001000");

    // Test 7: BitVector<32> AND BitVector<8>
    bvlib::BitVector<32> bv32e("11001100110011001100110011001100");
    bvlib::BitVector<8> bv8f("10101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv32e, bv8f, "00000000000000000000000010001000");

    // Test 8: BitVector<7> AND BitVector<8>
    bvlib::BitVector<7> bv7a("1100110");
    bvlib::BitVector<8> bv8g("00101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv7a, bv8g, "00100010");

    // Test 9: BitVector<7> AND int
    bvlib::BitVector<7> bv7b("1100110");
    int value1 = 101;
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, bv7a, value1, "1100100");

    // Test 10: int AND BitVector<7>
    int value2 = 42;
    bvlib::BitVector<7> bv7c("1100110");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs & rhs; }, value2, bv7c, "0100010");

    std::cout << "    ✅ All `&` tests completed!\n";
}

void test_bitwise_or()
{
    // Test 1: BitVector<8> OR BitVector<8>
    bvlib::BitVector<8> bv8a("11001100");
    bvlib::BitVector<8> bv8b("00101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv8a, bv8b, "11101110");

    // Test 2: BitVector<8> OR BitVector<16>
    bvlib::BitVector<8> bv8c("11001100");
    bvlib::BitVector<16> bv16("1010101010101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv8c, bv16, "1010101011101110");

    // Test 3: BitVector<16> OR BitVector<16>
    bvlib::BitVector<16> bv16a("1100110011001100");
    bvlib::BitVector<16> bv16b("1010101010101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv16a, bv16b, "1110111011101110");

    // Test 4: BitVector<16> OR BitVector<8>
    bvlib::BitVector<16> bv16c("1100110011001100");
    bvlib::BitVector<8> bv8d("10101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv16c, bv8d, "1100110011101110");

    // Test 5: BitVector<32> OR BitVector<32>
    bvlib::BitVector<32> bv32a("11001100110011001100110011001100");
    bvlib::BitVector<32> bv32b("10101010101010101010101010101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv32a, bv32b, "11101110111011101110111011101110");

    // Test 6: BitVector<32> OR BitVector<16>
    bvlib::BitVector<32> bv32c("11001100110011001100110011001100");
    bvlib::BitVector<16> bv16d("1010101010101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv32c, bv16d, "11001100110011001110111011101110");

    // Test 7: BitVector<32> OR BitVector<8>
    bvlib::BitVector<32> bv32e("11001100110011001100110011001100");
    bvlib::BitVector<8> bv8f("10101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv32e, bv8f, "11001100110011001100110011101110");

    // Test 8: BitVector<7> OR BitVector<8>
    bvlib::BitVector<7> bv7a("1100110");
    bvlib::BitVector<8> bv8g("00101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs | rhs; }, bv7a, bv8g, "01101110");

    std::cout << "    ✅ All `|` tests completed!\n";
}

void test_bitwise_xor()
{
    // Test 1: BitVector<8> XOR BitVector<8>
    bvlib::BitVector<8> bv8a("11001100");
    bvlib::BitVector<8> bv8b("00101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv8a, bv8b, "11100110");

    // Test 2: BitVector<8> XOR BitVector<16>
    bvlib::BitVector<8> bv8c("11001100");
    bvlib::BitVector<16> bv16("1010101010101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv8c, bv16, "1010101001100110");

    // Test 3: BitVector<16> XOR BitVector<16>
    bvlib::BitVector<16> bv16a("1100110011001100");
    bvlib::BitVector<16> bv16b("1010101010101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv16a, bv16b, "0110011001100110");

    // Test 4: BitVector<16> XOR BitVector<8>
    bvlib::BitVector<16> bv16c("1100110011001100");
    bvlib::BitVector<8> bv8d("10101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv16c, bv8d, "1100110001100110");

    // Test 5: BitVector<32> XOR BitVector<32>
    bvlib::BitVector<32> bv32a("11001100110011001100110011001100");
    bvlib::BitVector<32> bv32b("10101010101010101010101010101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv32a, bv32b, "01100110011001100110011001100110");

    // Test 6: BitVector<32> XOR BitVector<16>
    bvlib::BitVector<32> bv32c("11001100110011001100110011001100");
    bvlib::BitVector<16> bv16d("1010101010101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv32c, bv16d, "11001100110011000110011001100110");

    // Test 7: BitVector<32> XOR BitVector<8>
    bvlib::BitVector<32> bv32e("11001100110011001100110011001100");
    bvlib::BitVector<8> bv8f("10101010");
    test_binary(
        [](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv32e, bv8f, "11001100110011001100110001100110");

    // Test 8: BitVector<7> XOR BitVector<8>
    bvlib::BitVector<7> bv7a("1100110");
    bvlib::BitVector<8> bv8g("00101010");
    test_binary([](const auto &lhs, const auto &rhs) { return lhs ^ rhs; }, bv7a, bv8g, "01001100");

    std::cout << "    ✅ All `^` tests completed!\n";
}

void test_bitwise_not()
{
    // Test 1: BitVector<8> NOT
    bvlib::BitVector<8> bv8("11001100");
    test_unary([](const auto &opr) { return ~opr; }, bv8, "00110011");

    // Test 2: BitVector<16> NOT
    bvlib::BitVector<16> bv16("1100110011001100");
    test_unary([](const auto &opr) { return ~opr; }, bv16, "0011001100110011");

    // Test 3: BitVector<32> NOT
    bvlib::BitVector<32> bv32("11001100110011001100110011001100");
    test_unary([](const auto &opr) { return ~opr; }, bv32, "00110011001100110011001100110011");

    // Test 4: BitVector<7> NOT
    bvlib::BitVector<7> bv7("1100110");
    test_unary([](const auto &opr) { return ~opr; }, bv7, "0011001");

    // Test 5: BitVector<8> NOT (edge case: all ones)
    bvlib::BitVector<8> bv8_edge("11111111");
    test_unary([](const auto &opr) { return ~opr; }, bv8_edge, "00000000");

    // Test 6: BitVector<8> NOT (edge case: all zeros)
    bvlib::BitVector<8> bv8_zeros("00000000");
    test_unary([](const auto &opr) { return ~opr; }, bv8_zeros, "11111111");

    // Test 7: BitVector<8> NOT (alternating bits)
    bvlib::BitVector<8> bv8_alt("01010101");
    test_unary([](const auto &opr) { return ~opr; }, bv8_alt, "10101010");

    // Test 8: BitVector<8> NOT (edge case: single bit)
    bvlib::BitVector<8> bv8_single("00000001");
    test_unary([](const auto &opr) { return ~opr; }, bv8_single, "11111110");

    // Test 9: BitVector<8> NOT (edge case: another single bit)
    bvlib::BitVector<8> bv8_single2("10000000");
    test_unary([](const auto &opr) { return ~opr; }, bv8_single2, "01111111");

    std::cout << "    ✅ All `~` tests completed!\n";
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    test_bitwise_and();
    test_bitwise_or();
    test_bitwise_xor();
    test_bitwise_not();

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
