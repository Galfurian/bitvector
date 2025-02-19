/// @file support.hpp
/// @brief
/// @copyright Copyright (c) 2025

#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>

#include "bvlib/bitvector.hpp"
#include "bvlib/io.hpp"
#include "bvlib/math.hpp"

// Generic helper function for binary functions.
template <typename Function, typename T1, typename T2>
void __test_binary(
    Function function,
    const T1 &lhs,
    const T2 &rhs,
    const std::string expected_result,
    const std::string filename,
    int line,
    const std::string function_str)
{
    // Perform the binary function.
    auto result = function(lhs, rhs);
    // Check result and print error if it does not match the expected value.
    if (result.to_string() != expected_result) {
        std::cout << "❌ Test failed.\n";
        std::cout << "File     : " << filename << ":" << line << "\n";
        std::cout << "Function : " << function_str << "\n";
        std::cout << "Inputs : \n";
        std::cout << "    lhs  : " << std::setw(64) << std::right << lhs << "\n";
        std::cout << "    rhs  : " << std::setw(64) << std::right << rhs << "\n";
        std::cout << "Obtained : " << std::setw(64) << std::right << result << "\n";
        std::cout << "Expected : " << std::setw(64) << std::right << expected_result << "\n\n";
        std::exit(1);
    }
}

// Generic helper function for binary functions.
template <typename Function, typename T1, typename T2>
void __test_binary_assign(
    Function function,
    T1 &lhs,
    const T2 &rhs,
    const std::string expected_result,
    const std::string filename,
    int line,
    const std::string function_str)
{
    // Copy the input.
    auto copy = lhs;
    // Perform the binary function.
    function(lhs, rhs);
    // Check result and print error if it does not match the expected value.
    if (lhs.to_string() != expected_result) {
        std::cout << "❌ Test failed.\n";
        std::cout << "File     : " << filename << ":" << line << "\n";
        std::cout << "Function : " << function_str << "\n";
        std::cout << "Inputs : \n";
        std::cout << "    lhs  : " << std::setw(64) << std::right << copy << "\n";
        std::cout << "    rhs  : " << std::setw(64) << std::right << rhs << "\n";
        std::cout << "Obtained : " << std::setw(64) << std::right << lhs << "\n";
        std::cout << "Expected : " << std::setw(64) << std::right << expected_result << "\n\n";
        std::exit(1);
    }
}

// Generic helper function for binary functions.
template <typename Function, typename T1, typename T2>
void __test_binary_bool(
    Function function,
    const T1 &lhs,
    const T2 &rhs,
    bool expected_result,
    const std::string filename,
    int line,
    const std::string function_str)
{
    // Perform the binary function.
    auto result = function(lhs, rhs);
    // Check result and print error if it does not match the expected value.
    if (result != expected_result) {
        std::cout << "❌ Test failed.\n";
        std::cout << "File     : " << filename << ":" << line << "\n";
        std::cout << "Function : " << function_str << "\n";
        std::cout << "Inputs : \n";
        std::cout << "    lhs  : " << std::setw(64) << std::right << lhs << "\n";
        std::cout << "    rhs  : " << std::setw(64) << std::right << rhs << "\n";
        std::cout << "Obtained : " << std::setw(64) << std::right << result << "\n";
        std::cout << "Expected : " << std::setw(64) << std::right << expected_result << "\n\n";
        std::exit(1);
    }
}

// Generic helper function for unary functions.
template <typename Function, typename T>
void __test_unary(
    Function function,
    const T &bv,
    const std::string expected_result,
    const std::string filename,
    int line,
    const std::string function_str)
{
    // Perform the unary function.
    auto result = function(bv);
    // Check result and print error if it does not match the expected value
    if (result.to_string() != expected_result) {
        std::cout << "❌ Test failed.\n";
        std::cout << "File     : " << filename << ":" << line << "\n";
        std::cout << "Function : " << function_str << "\n";
        std::cout << "Inputs : \n";
        std::cout << "    bv   : " << std::setw(64) << std::right << bv << "\n";
        std::cout << "Obtained : " << std::setw(64) << std::right << result << "\n";
        std::cout << "Expected : " << std::setw(64) << std::right << expected_result << "\n\n";
        std::exit(1);
    }
}

// Helper macro to capture the test details, including the file, line, and test expression.
#define test_binary(function, lhs, rhs, expected_result)                                                               \
    do {                                                                                                               \
        __test_binary(function, lhs, rhs, expected_result, __FILE__, __LINE__, #function);                             \
    } while (0)

// Helper macro to capture the test details, including the file, line, and test expression.
#define test_binary_assign(function, lhs, rhs, expected_result)                                                        \
    do {                                                                                                               \
        __test_binary_assign(function, lhs, rhs, expected_result, __FILE__, __LINE__, #function);                      \
    } while (0)

// Helper macro to capture the test details, including the file, line, and test expression.
#define test_binary_bool(function, lhs, rhs, expected_result)                                                          \
    do {                                                                                                               \
        __test_binary_bool(function, lhs, rhs, expected_result, __FILE__, __LINE__, #function);                     \
    } while (0)

// Helper macro to capture the test details, including the file, line, and test expression.
#define test_unary(function, bv, expected_result)                                                                      \
    do {                                                                                                               \
        __test_unary(function, bv, expected_result, __FILE__, __LINE__, #function);                                    \
    } while (0)