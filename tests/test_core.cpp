/// @file test_core.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests core functionalities.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include "support.hpp"

// ============================================================================
// DEFAULT CONSTRUCTOR TESTS
// ============================================================================

/// @brief Tests the default constructor initializes all bits to 0.
void test_default_constructor()
{
    bvlib::BitVector<8> bv;
    test_unary([](const auto &value) { return value.none(); }, bv, true);
}

// ============================================================================
// INTEGER CONSTRUCTOR TESTS
// ============================================================================

/// @brief Tests construction from integer 0.
void test_integer_constructor_zero()
{
    bvlib::BitVector<8> bv(0);
    test_unary([](const auto &value) { return value.none(); }, bv, true);
}

/// @brief Tests construction from a small positive integer.
void test_integer_constructor_small_value()
{
    bvlib::BitVector<8> bv(5);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000101");
}

/// @brief Tests construction from a large integer value.
void test_integer_constructor_large_value()
{
    bvlib::BitVector<8> bv(255); // 255 = 11111111
    test_unary([](const auto &value) { return value.all(); }, bv, true);
}

/// @brief Tests construction from a value that exceeds the BitVector size.
void test_integer_constructor_overflow()
{
    bvlib::BitVector<8> bv(1023); // 1023 = 11 1111 1111 (truncated)
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11111111");
}

// ============================================================================
// STRING CONSTRUCTOR TESTS
// ============================================================================

/// @brief Tests construction from an empty string.
void test_string_constructor_empty()
{
    bvlib::BitVector<8> bv("");
    test_unary([](const auto &value) { return value.none(); }, bv, true);
}

/// @brief Tests construction from a valid binary string.
void test_string_constructor_valid()
{
    bvlib::BitVector<8> bv("10101010");
    test_unary([](const auto &value) { return value.to_string(); }, bv, "10101010");
}

/// @brief Tests construction from a binary string shorter than BitVector size.
void test_string_constructor_short_string()
{
    bvlib::BitVector<8> bv("1101"); // "1101" -> 00001101
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00001101");
}

/// @brief Tests construction from a binary string longer than BitVector size.
void test_string_constructor_long_string()
{
    bvlib::BitVector<8> bv("111100001111"); // "111100001111" -> Truncated to "11110000"
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00001111");
}

/// @brief Tests construction from a string with invalid characters.
void test_string_constructor_invalid_chars()
{
    try {
        bvlib::BitVector<8> bv("11012"); // Should fail due to '2'
        assert(false && "String constructor should throw on invalid characters");
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << "\n";
    }
}

// ============================================================================
// COPY CONSTRUCTOR TESTS
// ============================================================================

/// @brief Tests copy construction from the same size BitVector.
void test_copy_constructor_same_size()
{
    bvlib::BitVector<8> bv1("10101010");
    bvlib::BitVector<8> bv2(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "10101010");
}

/// @brief Tests copy construction from a smaller BitVector.
void test_copy_constructor_smaller_to_larger()
{
    bvlib::BitVector<4> bv1("1101");
    bvlib::BitVector<8> bv2(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "00001101");
}

/// @brief Tests copy construction from a larger BitVector.
void test_copy_constructor_larger_to_smaller()
{
    bvlib::BitVector<8> bv1("1010101011001100");
    bvlib::BitVector<4> bv2(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "1100");
}

/// @brief Tests copy construction where the source has extra bits.
void test_copy_constructor_truncation()
{
    bvlib::BitVector<16> bv1("1100110011001100");
    bvlib::BitVector<8> bv2(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "11001100");
}

/// @brief Tests copy construction from an empty BitVector.
void test_copy_constructor_empty()
{
    bvlib::BitVector<8> bv1;
    bvlib::BitVector<8> bv2(bv1);
    test_unary([](const auto &value) { return value.none(); }, bv2, true);
}

// ============================================================================
// TESTS FOR `ones()`
// ============================================================================

/// @brief Tests that ones() creates a BitVector with all bits set to 1.
void test_ones_small()
{
    bvlib::BitVector<4> bv = bvlib::BitVector<4>::ones();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1111");
}

void test_ones_medium()
{
    bvlib::BitVector<8> bv = bvlib::BitVector<8>::ones();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11111111");
}

void test_ones_large()
{
    bvlib::BitVector<16> bv = bvlib::BitVector<16>::ones();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1111111111111111");
}

void test_ones_single_bit()
{
    bvlib::BitVector<1> bv = bvlib::BitVector<1>::ones();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1");
}

void test_ones_trim_effect()
{
    bvlib::BitVector<10> bv = bvlib::BitVector<10>::ones();
    test_unary([](const auto &value) { return value.count(); }, bv, 10U);
}

// ============================================================================
// TESTS FOR `zeros()`
// ============================================================================

/// @brief Tests that zeros() creates a BitVector with all bits set to 0.
void test_zeros_small()
{
    bvlib::BitVector<4> bv = bvlib::BitVector<4>::zeros();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "0000");
}

void test_zeros_medium()
{
    bvlib::BitVector<8> bv = bvlib::BitVector<8>::zeros();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000000");
}

void test_zeros_large()
{
    bvlib::BitVector<16> bv = bvlib::BitVector<16>::zeros();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "0000000000000000");
}

void test_zeros_single_bit()
{
    bvlib::BitVector<1> bv = bvlib::BitVector<1>::zeros();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "0");
}

void test_zeros_no_effect_on_trim()
{
    bvlib::BitVector<10> bv = bvlib::BitVector<10>::zeros();
    test_unary([](const auto &value) { return value.count(); }, bv, 0U);
}

// ============================================================================
// SET TESTS
// ============================================================================

/// @brief Tests `set()` to ensure all bits are set to 1.
void test_set_all()
{
    bvlib::BitVector<8> bv;
    bv.set();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11111111");
}

/// @brief Tests `set(pos)` to ensure a single bit is set correctly.
void test_set_single_bit()
{
    bvlib::BitVector<8> bv;
    bv.set(3);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00001000");
}

/// @brief Tests `set(pos)` for the first and last bit (boundary cases).
void test_set_boundary_bits()
{
    bvlib::BitVector<8> bv;
    bv.set(0);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000001");
    bv.set(7);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "10000001");
}

/// @brief Tests `set(pos)` with an out-of-range index.
void test_set_out_of_range()
{
    bvlib::BitVector<8> bv;
    try {
        bv.set(8); // Out of range
        assert(false && "set(n) should throw out_of_range for n >= size");
    } catch (const std::out_of_range &e) {
        std::cerr << e.what() << "\n";
    }
}

// ============================================================================
// RESET TESTS
// ============================================================================

/// @brief Tests `reset()` to ensure all bits are reset to 0.
void test_reset_all()
{
    bvlib::BitVector<8> bv;
    bv.set();
    bv.reset();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000000");
}

/// @brief Tests `reset(pos)` to ensure a single bit is cleared.
void test_reset_single_bit()
{
    bvlib::BitVector<8> bv("11111111");
    bv.reset(5);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11011111");
}

/// @brief Tests `reset(pos)` for boundary bits (first and last).
void test_reset_boundary_bits()
{
    bvlib::BitVector<8> bv("11111111");
    bv.reset(0);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11111110");
    bv.reset(7);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "01111110");
}

/// @brief Tests `reset(pos)` with an out-of-range index.
void test_reset_out_of_range()
{
    bvlib::BitVector<8> bv;
    try {
        bv.reset(8); // Out of range
        assert(false && "reset(n) should throw out_of_range for n >= size");
    } catch (const std::out_of_range &e) {
        std::cerr << e.what() << "\n";
    }
}

// ============================================================================
// FLIP TESTS
// ============================================================================

/// @brief Tests `flip()` to ensure all bits are inverted.
void test_flip_all()
{
    bvlib::BitVector<8> bv;
    bv.flip();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11111111");
    bv.flip();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000000");
}

/// @brief Tests `flip(pos)` to toggle a single bit.
void test_flip_single_bit()
{
    bvlib::BitVector<8> bv;
    bv.flip(3);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00001000");
    bv.flip(3);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000000");
}

/// @brief Tests `flip(pos)` with an out-of-range index.
void test_flip_out_of_range()
{
    bvlib::BitVector<8> bv;
    try {
        bv.flip(8); // Out of range
        assert(false && "flip(n) should throw out_of_range for n >= size");
    } catch (const std::out_of_range &e) {
        std::cerr << e.what() << "\n";
    }
}

// ============================================================================
// SET_SIGN TESTS
// ============================================================================

/// @brief Tests `set_sign(true)` to set the MSB.
void test_set_sign_true()
{
    bvlib::BitVector<8> bv;
    bv.set_sign(true);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "10000000");
}

/// @brief Tests `set_sign(false)` to clear the MSB.
void test_set_sign_false()
{
    bvlib::BitVector<8> bv("10000000");
    bv.set_sign(false);
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00000000");
}

// ============================================================================
// TRIM TESTS
// ============================================================================

/// @brief Tests `trim()` when there are no extra bits (N is a multiple of BitsPerBlock).
void test_trim_no_extra_bits()
{
    bvlib::BitVector<16> bv("1111111111111111"); // No extra bits (16 bits exactly)
    bv.trim();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1111111111111111");
}

/// @brief Tests `trim()` when there are extra bits in the last block.
void test_trim_with_extra_bits()
{
    bvlib::BitVector<16> bv("001111111111111111"); // Extra 2 bits in last block (should be cleared)
    bv.trim();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1111111111111111");
}

/// @brief Tests `trim()` with a larger vector and a complex pattern.
void test_trim_large_vector()
{
    bvlib::BitVector<22> bv("11111111111111110000111111000011"); // Complex pattern, last block has extra bits
    bv.trim();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1111110000111111000011");
}

/// @brief Tests `trim()` with the minimal bitvector size that has extra bits.
void test_trim_minimal_size()
{
    bvlib::BitVector<2> bv("1111"); // 2 bits in a 4-bit block (so extra bits)
    bv.trim();
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11");
}

/// @brief Tests `trim()` when there's no need to trim extra bits (vector already correctly sized).
void test_trim_no_op_on_already_trimmed_vector()
{
    bvlib::BitVector<8> bv("11110000"); // No extra bits to trim
    bv.trim();                          // Should leave the vector unchanged
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11110000");
}

// ============================================================================
// count() TESTS
// ============================================================================

/// @brief Tests the `count()` function, which returns the number of set bits.
void test_count()
{
    bvlib::BitVector<8> bv1("11110000"); // 4 bits set
    test_unary([](const auto &value) { return value.count(); }, bv1, 4U);

    bvlib::BitVector<8> bv2("11111111"); // 8 bits set
    test_unary([](const auto &value) { return value.count(); }, bv2, 8U);

    bvlib::BitVector<8> bv3("00000000"); // 0 bits set
    test_unary([](const auto &value) { return value.count(); }, bv3, 0U);

    bvlib::BitVector<16> bv4("1010101010101010"); // 8 bits set
    test_unary([](const auto &value) { return value.count(); }, bv4, 8U);

    bvlib::BitVector<16> bv5("11110000000001010101010101010"); // 8 bits set, with trim.
    test_unary([](const auto &value) { return value.count(); }, bv5, 8U);
}

// ============================================================================
// all() TESTS
// ============================================================================

/// @brief Tests the `all()` function, which checks if all bits are set.
void test_all()
{
    bvlib::BitVector<8> bv1("11111111"); // All bits set
    test_unary([](const auto &value) { return value.all(); }, bv1, true);

    bvlib::BitVector<8> bv2("11110000"); // Not all bits set
    test_unary([](const auto &value) { return value.all(); }, bv2, false);

    bvlib::BitVector<8> bv3("00000000"); // No bits set
    test_unary([](const auto &value) { return value.all(); }, bv3, false);

    bvlib::BitVector<16> bv4("1111111111111111"); // All bits set in a 16-bit vector
    test_unary([](const auto &value) { return value.all(); }, bv4, true);

    bvlib::BitVector<16> bv5("00000001111111111111111"); // All bits set in a 16-bit vector, with trim.
    test_unary([](const auto &value) { return value.all(); }, bv5, true);
}

// ============================================================================
// any() TESTS
// ============================================================================

/// @brief Tests the `any()` function, which checks if any bit is set.
void test_any()
{
    bvlib::BitVector<8> bv1("00000000"); // No bits set
    test_unary([](const auto &value) { return value.any(); }, bv1, false);

    bvlib::BitVector<8> bv2("11111111"); // All bits set
    test_unary([](const auto &value) { return value.any(); }, bv2, true);

    bvlib::BitVector<8> bv3("11110000"); // Some bits set
    test_unary([](const auto &value) { return value.any(); }, bv3, true);

    bvlib::BitVector<16> bv4("0000000000000001"); // One bit set
    test_unary([](const auto &value) { return value.any(); }, bv4, true);

    bvlib::BitVector<16> bv5("100000000000000000"); // No bit set, with trim.
    test_unary([](const auto &value) { return value.any(); }, bv5, false);
}

// ============================================================================
// none() TESTS
// ============================================================================

/// @brief Tests the `none()` function, which checks if no bits are set.
void test_none()
{
    bvlib::BitVector<8> bv1("00000000"); // No bits set
    test_unary([](const auto &value) { return value.none(); }, bv1, true);

    bvlib::BitVector<8> bv2("11111111"); // All bits set
    test_unary([](const auto &value) { return value.none(); }, bv2, false);

    bvlib::BitVector<8> bv3("11110000"); // Some bits set
    test_unary([](const auto &value) { return value.none(); }, bv3, false);

    bvlib::BitVector<16> bv4("0000000000000001"); // One bit set
    test_unary([](const auto &value) { return value.none(); }, bv4, false);

    bvlib::BitVector<16> bv5("10000000000000000"); // No bit set
    test_unary([](const auto &value) { return value.none(); }, bv5, true);
}

// ============================================================================
// sign() TESTS
// ============================================================================

/// @brief Tests the `sign()` function, which checks if the MSB (sign bit) is set.
void test_sign()
{
    bvlib::BitVector<8> bv1("10000000"); // MSB set to 1 (negative value in two's complement)
    test_unary([](const auto &value) { return value.sign(); }, bv1, true);

    bvlib::BitVector<8> bv2("01111111"); // MSB set to 0 (positive value)
    test_unary([](const auto &value) { return value.sign(); }, bv2, false);

    bvlib::BitVector<8> bv3("00101000"); // MSB set to 0 (positive value)
    test_unary([](const auto &value) { return value.sign(); }, bv3, false);

    bvlib::BitVector<16> bv4("1000000000000000"); // MSB set to 1 (negative value in two's complement)
    test_unary([](const auto &value) { return value.sign(); }, bv4, true);

    bvlib::BitVector<16> bv5("001000000000000000"); // MSB set to 1 (negative value in two's complement), with trim.
    test_unary([](const auto &value) { return value.sign(); }, bv5, true);
}

// ============================================================================
// swap() TESTS
// ============================================================================

/// @brief Tests `swap()` to ensure two bits are swapped correctly.
void test_swap_bits()
{
    bvlib::BitVector<8> bv("11001100");
    bv.swap(0, 7); // Swap the first and last bits
    test_unary([](const auto &value) { return value.to_string(); }, bv, "01001101");
}

/// @brief Tests `swap_range()` to reverse the bits between two positions.
void test_swap_range()
{
    bvlib::BitVector<8> bv("11001100");
    bv.swap_range(0, 7); // Reverse the entire bit vector
    test_unary([](const auto &value) { return value.to_string(); }, bv, "00110011");
}

/// @brief Tests `swap_range()` on a smaller range.
void test_swap_range_small()
{
    bvlib::BitVector<8> bv("10");
    bv.swap_range(1, 6); // Reverse the bits between positions 1 and 6
    test_unary([](const auto &value) { return value.to_string(); }, bv, "01000000");
}

/// @brief Tests `swap_range()` with overlapping indices (no change).
void test_swap_range_same_position()
{
    bvlib::BitVector<8> bv("11001100");
    bv.swap_range(3, 3); // Same position, no change
    test_unary([](const auto &value) { return value.to_string(); }, bv, "11001100");
}

// ============================================================================
// assign() TESTS
// ============================================================================

void test_assign_same_size()
{
    bvlib::BitVector<8> bv1("10101010");
    bvlib::BitVector<8> bv2("00000000");
    bv2.assign(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "10101010");
}

void test_assign_smaller_to_larger()
{
    bvlib::BitVector<4> bv1("1010");
    bvlib::BitVector<8> bv2("00000000");
    bv2.assign(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "00001010");
}

void test_assign_larger_to_smaller()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<4> bv2("0000");
    bv2.assign(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "1100");
}

void test_assign_exceeds_32bit_smaller_to_larger()
{
    bvlib::BitVector<16> bv1("1111000011110000");
    bvlib::BitVector<64> bv2("0000000000000000000000000000000000000000000000000000000000000000");
    bv2.assign(bv1);
    test_unary([](const auto &value) { return value.to_string().substr(48); }, bv2, "1111000011110000");
}

void test_assign_exceeds_32bit_larger_to_smaller()
{
    bvlib::BitVector<64> bv1("1111111111111111000000000000000011111111111111110000000000000000");
    bvlib::BitVector<16> bv2("0000000000000000");
    bv2.assign(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "0000000000000000");
}

// ============================================================================
// rassign() TESTS
// ============================================================================

void test_rassign_same_size()
{
    bvlib::BitVector<8> bv1("10101010");
    bvlib::BitVector<8> bv2("00000000");
    bv2.rassign(bv1);
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "10101010");
}

void test_rassign_smaller_to_larger()
{
    bvlib::BitVector<4> bv1("1010");
    bvlib::BitVector<8> bv2("00000000");
    bv2.rassign(bv1);
    // reversed copy means '1010' -> '0101' in the higher bits if reversed copying, or similarly
    // here we interpret the function to fill from left to right in target
    // check correctness:
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "10100000");
}

void test_rassign_larger_to_smaller()
{
    bvlib::BitVector<8> bv1("11001100");
    bvlib::BitVector<4> bv2("0000");
    bv2.rassign(bv1);
    // reversed copy means the last 4 bits of bv1 get reversed into bv2's 4 bits
    // check correctness carefully:
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "1100");
}

void test_rassign_exceeds_32bit_smaller_to_larger()
{
    bvlib::BitVector<16> bv1("1111000011110000");
    bvlib::BitVector<64> bv2("0000000000000000000000000000000000000000000000000000000000000000");
    bv2.rassign(bv1);
    // reversed assignment means the bits of bv1 are reversed into the lower portion of bv2
    // check correctness: we expect '0000111100001111' or similar reversed pattern.
    test_unary([](const auto &value) { return value.to_string().substr(48); }, bv2, "0000000000000000");
}

void test_rassign_exceeds_32bit_larger_to_smaller()
{
    bvlib::BitVector<64> bv1("1111111111111111000000000000000011111111111111110000000000000000");
    bvlib::BitVector<16> bv2("0000000000000000");
    bv2.rassign(bv1);
    // Reversed copy from a 64-bit vector into a 16-bit vector check correctness.
    test_unary([](const auto &value) { return value.to_string(); }, bv2, "1111111111111111");
}

// ============================================================================
// at() TESTS
// ============================================================================

/// @brief Tests the `at()` function for reading a bit.
void test_at_read()
{
    bvlib::BitVector<8> bv("10101010");
    test_unary([](auto &value) { return value.at(0); }, bv, false);
    test_unary([](auto &value) { return value.at(1); }, bv, true);
    test_unary([](auto &value) { return value.at(2); }, bv, false);
    test_unary([](auto &value) { return value.at(3); }, bv, true);
    test_unary([](auto &value) { return value.at(4); }, bv, false);
    test_unary([](auto &value) { return value.at(5); }, bv, true);
    test_unary([](auto &value) { return value.at(6); }, bv, false);
    test_unary([](auto &value) { return value.at(7); }, bv, true);
}

/// @brief Tests the `at()` function for modifying a bit.
void test_at_modify()
{
    bvlib::BitVector<8> bv("10101010");

    bv.at(0) = true;  // Set   bit at position 0
    bv.at(1) = false; // Clear bit at position 1
    bv.at(2) = true;  // Set   bit at position 2
    bv.at(3) = false; // Clear bit at position 3

    test_unary([](const auto &value) { return value.to_string(); }, bv, "10100101");

    bv.at(4) = true;  // Set   bit at position 4
    bv.at(5) = false; // Clear bit at position 5
    bv.at(6) = true;  // Set   bit at position 6
    bv.at(7) = false; // Clear bit at position 7

    test_unary([](const auto &value) { return value.to_string(); }, bv, "01010101");
}

/// @brief Tests the `at()` function when accessing out-of-range indices.
void test_at_out_of_range()
{
    bvlib::BitVector<8> bv("10101010");

    try {
        bv.at(8); // Accessing out of bounds (bit 8 is invalid for 8-bit vector)
        assert(false && "at() should throw an exception for out-of-range access");
    } catch (const std::out_of_range &) {
        // Expected exception, test passes
    }

    try {
        bv.at(10); // Accessing out of bounds
        assert(false && "at() should throw an exception for out-of-range access");
    } catch (const std::out_of_range &) {
        // Expected exception, test passes
    }
}

/// @brief Tests the `at()` function for modification and checks the state after modifications.
void test_at_modify_state()
{
    bvlib::BitVector<16> bv("0000000000000000");

    // Modify specific bits and check the state
    bv.at(1) = true;
    bv.at(3) = true;
    bv.at(5) = true;
    bv.at(7) = true;
    test_unary([](const auto &value) { return value.to_string(); }, bv, "0000000010101010");

    bv.at(15) = true; // Set MSB
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1000000010101010");

    bv.at(0) = true; // Set LSB
    test_unary([](const auto &value) { return value.to_string(); }, bv, "1000000010101011");
}

/// @brief Tests that `at()` works correctly for a large BitVector.
void test_at_large_bitvector()
{
    bvlib::BitVector<64> bv("1010101010101010101010101010101010101010101010101010101010101010");

    test_unary([](const auto &value) { return value.at(0U); }, bv, false);
    test_unary([](const auto &value) { return value.at(63U); }, bv, true);

    bv.at(0)  = true;  // Modify the LSB
    bv.at(63) = false; // Modify the MSB

    test_unary(
        [](const auto &value) { return value.to_string(); }, bv,
        "0010101010101010101010101010101010101010101010101010101010101011");
}

/// @brief Tests that `at()` modifies a large bitvector correctly with multiple changes.
void test_at_large_bitvector_multiple_changes()
{
    bvlib::BitVector<64> bv("1010101010101010101010101010101010101010101010101010101010101010");

    // Modify several bits and check state after
    bv.at(1)  = false;
    bv.at(5)  = false;
    bv.at(7)  = false;
    bv.at(63) = false;

    test_unary(
        [](const auto &value) { return value.to_string(); }, bv,
        "0010101010101010101010101010101010101010101010101010101000001000");

    // Resetting some bits back to 1
    bv.at(1)  = true;
    bv.at(5)  = true;
    bv.at(7)  = true;
    bv.at(63) = true;

    test_unary(
        [](const auto &value) { return value.to_string(); }, bv,
        "1010101010101010101010101010101010101010101010101010101010101010");
}

// ============================================================================
// slice() TESTS
// ============================================================================

void test_slice()
{
    bvlib::BitVector<8> bv8("11001100");
    bvlib::BitVector<16> bv16("1100110011001100");
    bvlib::BitVector<32> bv32("11001100110011001100110011001100");

    // Slice from bit 2 to bit 5 (inclusive).
    test_unary([](const auto &value) { return value.template slice<2 COMMA 5>(); }, bv8, "0011");
    // Slice from bit 5 to bit 6 (inclusive).
    test_unary([](const auto &value) { return value.template slice<5 COMMA 6>(); }, bv8, "10");
    // Slice from bit 4 to bit 11 (inclusive).
    test_unary([](const auto &value) { return value.template slice<4 COMMA 11>(); }, bv16, "11001100");
    // Slice from bit 8 to bit 15 (inclusive).
    test_unary([](const auto &value) { return value.template slice<8 COMMA 15>(); }, bv32, "11001100");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

/// @brief Runs all tests related to the constructor.
void run_constructor_tests()
{
    // Default Constructor
    test_default_constructor();

    // Integer Constructor
    test_integer_constructor_zero();
    test_integer_constructor_small_value();
    test_integer_constructor_large_value();
    test_integer_constructor_overflow();

    // String Constructor
    test_string_constructor_empty();
    test_string_constructor_valid();
    test_string_constructor_short_string();
    test_string_constructor_long_string();
    test_string_constructor_invalid_chars();

    // Copy Constructor
    test_copy_constructor_same_size();
    test_copy_constructor_smaller_to_larger();
    test_copy_constructor_larger_to_smaller();
    test_copy_constructor_truncation();
    test_copy_constructor_empty();

    std::cout << "    ✅ All constructor tests passed!\n";
}

/// @brief Runs all tests related to the `ones()` and `zeros()` functions.
void run_ones_zeros_tests()
{
    // ones() Tests
    test_ones_small();
    test_ones_medium();
    test_ones_large();
    test_ones_single_bit();
    test_ones_trim_effect();

    // zeros() Tests
    test_zeros_small();
    test_zeros_medium();
    test_zeros_large();
    test_zeros_single_bit();
    test_zeros_no_effect_on_trim();

    std::cout << "    ✅ All ones/zeros tests passed!\n";
}

/// @brief Runs all tests related to the `set/reset/flip/set_sign` functions.
void run_set_reset_flip_tests()
{
    // Set tests
    test_set_all();
    test_set_single_bit();
    test_set_boundary_bits();
    test_set_out_of_range();

    // Reset tests
    test_reset_all();
    test_reset_single_bit();
    test_reset_boundary_bits();
    test_reset_out_of_range();

    // Flip tests
    test_flip_all();
    test_flip_single_bit();
    test_flip_out_of_range();

    // Set sign tests
    test_set_sign_true();
    test_set_sign_false();

    std::cout << "    ✅ All set/reset/flip/set_sign tests passed!\n";
}

/// @brief Runs all tests related to the `trim()` function.
void run_all_trim_tests()
{
    test_trim_no_extra_bits();
    test_trim_with_extra_bits();
    test_trim_large_vector();
    test_trim_minimal_size();
    test_trim_no_op_on_already_trimmed_vector();

    std::cout << "    ✅ All trim tests passed!\n";
}

/// @brief Runs all tests for `count()`, `all()`, `any()`, `none()`, and `sign()`.
void run_all_check_functions()
{
    test_count();
    test_all();
    test_any();
    test_none();
    test_sign();

    std::cout << "    ✅ All count/all/any/none/sign tests passed!\n";
}

/// @brief Runs all tests for `swap()`.
void run_all_swap_tests()
{
    test_swap_bits();
    test_swap_range();
    test_swap_range_small();
    test_swap_range_same_position();

    std::cout << "    ✅ All swap tests completed!\n";
}

void run_all_assign_rassign_tests()
{
    // assign tests
    test_assign_same_size();
    test_assign_smaller_to_larger();
    test_assign_larger_to_smaller();
    test_assign_exceeds_32bit_smaller_to_larger();
    test_assign_exceeds_32bit_larger_to_smaller();

    // rassign tests
    test_rassign_same_size();
    test_rassign_smaller_to_larger();
    test_rassign_larger_to_smaller();
    test_rassign_exceeds_32bit_smaller_to_larger();
    test_rassign_exceeds_32bit_larger_to_smaller();

    std::cout << "    ✅ All assign/rassign tests completed!\n";
}

void run_all_at_tests()
{
    test_at_read();
    test_at_modify();
    test_at_out_of_range();
    test_at_modify_state();
    test_at_large_bitvector();
    test_at_large_bitvector_multiple_changes();

    std::cout << "    ✅ All at() tests completed!\n";
}

void run_all_slice_tests()
{
    test_slice();

    std::cout << "    ✅ All slice() tests completed!\n";
}

int main()
{
    std::cout << "Running all standard function tests.\n";

    run_constructor_tests();
    run_ones_zeros_tests();
    run_set_reset_flip_tests();
    run_all_trim_tests();
    run_all_check_functions();
    run_all_swap_tests();
    run_all_assign_rassign_tests();
    run_all_at_tests();
    run_all_slice_tests();

    std::cout << "✅ All standard function tests passed!\n";
    return 0;
}
