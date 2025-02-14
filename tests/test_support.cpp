#include <cassert>
#include <iostream>

#include "bvlib/math.hpp"

// ============================================================================
// add_bits() TESTS
// ============================================================================

/// @brief Tests all combinations of `add_bits` with both carry = true and carry = false.
void test_add_bits()
{
    bool carry;
    bool result;

    // ========================================================================
    // Test cases with carry = false
    // ========================================================================

    // Test case 1: 0 + 0 with no carry
    carry  = false;
    result = bvlib::detail::add_bits(false, false, carry);
    assert(result == false && "add_bits(0, 0) should return false");
    assert(carry == false && "carry should remain false");

    // Test case 2: 1 + 0 with no carry
    carry  = false;
    result = bvlib::detail::add_bits(true, false, carry);
    assert(result == true && "add_bits(1, 0) should return true");
    assert(carry == false && "carry should remain false");

    // Test case 3: 0 + 1 with no carry
    carry  = false;
    result = bvlib::detail::add_bits(false, true, carry);
    assert(result == true && "add_bits(0, 1) should return true");
    assert(carry == false && "carry should remain false");

    // Test case 4: 1 + 1 with no carry
    carry  = false;
    result = bvlib::detail::add_bits(true, true, carry);
    assert(result == false && "add_bits(1, 1) should return false");
    assert(carry == true && "carry should be true");

    // ========================================================================
    // Test cases with carry = true
    // ========================================================================

    // Test case 1: 0 + 0 with carry
    carry  = true;
    result = bvlib::detail::add_bits(false, false, carry);
    assert(result == true && "add_bits(0, 0, carry) should return true");
    assert(carry == false && "carry should be false after operation");

    // Test case 2: 1 + 0 with carry
    carry  = true;
    result = bvlib::detail::add_bits(true, false, carry);
    assert(result == false && "add_bits(1, 0, carry) should return false");
    assert(carry == true && "carry should remain true");

    // Test case 3: 0 + 1 with carry
    carry  = true;
    result = bvlib::detail::add_bits(false, true, carry);
    assert(result == false && "add_bits(0, 1, carry) should return false");
    assert(carry == true && "carry should remain true");

    // Test case 4: 1 + 1 with carry
    carry  = true;
    result = bvlib::detail::add_bits(true, true, carry);
    assert(result == true && "add_bits(1, 1, carry) should return true");
    assert(carry == true && "carry should remain true");
}

// ============================================================================
// sub_bits() TESTS
// ============================================================================

/// @brief Tests the `sub_bits()` function for subtracting bits and updating the borrow.
void test_sub_bits()
{
    bool borrow;
    bool result;

    // ========================================================================
    // Test cases with borrow = false
    // ========================================================================

    // Test case 1: 0 - 0 with no borrow
    borrow = false;
    result = bvlib::detail::sub_bits(false, false, borrow);
    assert(result == false && "sub_bits(0, 0) should return false");
    assert(borrow == false && "borrow should remain false");

    // Test case 2: 1 - 0 with no borrow
    borrow = false;
    result = bvlib::detail::sub_bits(true, false, borrow);
    assert(result == true && "sub_bits(1, 0) should return true");
    assert(borrow == false && "borrow should remain false");

    // Test case 3: 0 - 1 with no borrow
    borrow = false;
    result = bvlib::detail::sub_bits(false, true, borrow);
    assert(result == true && "sub_bits(0, 1) should return true");
    assert(borrow == true && "borrow should be true");

    // Test case 4: 1 - 1 with no borrow
    borrow = false;
    result = bvlib::detail::sub_bits(true, true, borrow);
    assert(result == false && "sub_bits(1, 1) should return false");
    assert(borrow == false && "borrow should remain false");

    // ========================================================================
    // Test cases with borrow = true
    // ========================================================================

    // Test case 1: 0 - 0 with borrow
    borrow = true;
    result = bvlib::detail::sub_bits(false, false, borrow);
    assert(result == true && "sub_bits(0, 0, borrow) should return true");
    assert(borrow == true && "borrow should be true after operation");

    // Test case 2: 1 - 0 with borrow
    borrow = true;
    result = bvlib::detail::sub_bits(true, false, borrow);
    assert(result == false && "sub_bits(1, 0, borrow) should return false");
    assert(borrow == false && "borrow should remain false");

    // Test case 3: 0 - 1 with borrow
    borrow = true;
    result = bvlib::detail::sub_bits(false, true, borrow);
    assert(result == false && "sub_bits(0, 1, borrow) should return false");
    assert(borrow == true && "borrow should remain true");

    // Test case 4: 1 - 1 with borrow
    borrow = true;
    result = bvlib::detail::sub_bits(true, true, borrow);
    assert(result == true && "sub_bits(1, 1, borrow) should return true");
    assert(borrow == true && "borrow should remain true");
}

// ============================================================================
// most_significant_bit() TESTS
// ============================================================================

/// @brief Tests the `most_significant_bit()` function to find the position of the most significant bit.
void test_most_significant_bit()
{
    bvlib::BitVector<32> bv1("00000000000000000000000000001010");
    assert(bvlib::detail::most_significant_bit(bv1) == 3 && "most_significant_bit() should return correct position for bit 3");

    bvlib::BitVector<32> bv2("10000000000000000000000000000000");
    assert(bvlib::detail::most_significant_bit(bv2) == 31 && "most_significant_bit() should return 31 for the most significant bit");

    bvlib::BitVector<32> bv3("00000000000000000000000000000000");
    assert(bvlib::detail::most_significant_bit(bv3) == 0 && "most_significant_bit() should return 0 if no bits are set");

    bvlib::BitVector<32> bv4("01000000000000000000000000000000");
    assert(bvlib::detail::most_significant_bit(bv4) == 30 && "most_significant_bit() should return the correct position for bit 30");

    bvlib::BitVector<32> bv5("00000000000000010000000000000000");
    assert(bvlib::detail::most_significant_bit(bv5) == 16 && "most_significant_bit() should return correct position for bit 17");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests." << std::endl;

    test_add_bits();
    test_sub_bits();
    test_most_significant_bit();

    std::cout << "✅ All function tests passed!" << std::endl;
    return 0;
}
