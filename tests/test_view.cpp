/// @file test_view.cpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Tests view functions.
/// @copyright Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>
/// Licensed under the MIT License. See LICENSE.md file root for details.

#include "support.hpp"

void test_view()
{
    bvlib::BitVector<16> bv("1100110011001100");

    auto view0 = bvlib::make_view<0, 8>(bv);
    auto view1 = bvlib::make_view<8, 16>(bv);

    // Test 1: View bit manipulation.
    view0[0] = true;
    view1[0] = true;
    test_unary([](auto &value) { return value[0]; }, bv, true);
    test_unary([](auto &value) { return value[8]; }, bv, true);

    view0[0] = false;
    view1[0] = false;
    test_unary([](auto &value) { return value[0]; }, bv, false);
    test_unary([](auto &value) { return value[8]; }, bv, false);

    // Test 2: View assignment from BitVector.
    view0 = bvlib::BitVector<4>("1010");
    test_unary([](auto &value) { return value[0]; }, bv, false);
    test_unary([](auto &value) { return value[1]; }, bv, true);
    test_unary([](auto &value) { return value[2]; }, bv, false);
    test_unary([](auto &value) { return value[3]; }, bv, true);

    // Test 3: View assignment from string.
    view0 = "0101";
    test_unary([](auto &value) { return value[0]; }, bv, true);
    test_unary([](auto &value) { return value[1]; }, bv, false);
    test_unary([](auto &value) { return value[2]; }, bv, true);
    test_unary([](auto &value) { return value[3]; }, bv, false);

    // Test 3: View assignment from another View (same size and range)
    view0 = view1;
    test_unary([](auto &value) { return value.to_string(); }, bv, "1100110011001100");
}

// ============================================================================
// RUN ALL TESTS
// ============================================================================

int main()
{
    std::cout << "Running all function tests.\n";

    test_view();

    std::cout << "✅ All function tests passed!\n";
    return 0;
}
