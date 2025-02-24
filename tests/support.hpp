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
template <typename Function, typename T1, typename T2, typename ResultType>
void __test_binary(
    Function function,
    T1 &lhs,
    T2 &rhs,
    const ResultType &expected_result,
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
template <typename Function, typename T, typename ResultType>
void __test_unary(
    Function function,
    T &bv,
    const ResultType &expected_result,
    const std::string filename,
    int line,
    const std::string function_str)
{
    // Perform the unary function.
    auto result = function(bv);
    // Check result and print error if it does not match the expected value
    if (result != expected_result) {
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

// Generic helper function for unary functions.
template <typename ValueType, typename T, typename ResultType>
void __test_value(
    ValueType value,
    T &bv,
    const ResultType &expected_result,
    const std::string filename,
    int line,
    const std::string function_str)
{
    // Check result and print error if it does not match the expected value.
    if (value != expected_result) {
        std::cout << "❌ Test failed.\n";
        std::cout << "File     : " << filename << ":" << line << "\n";
        std::cout << "Function : " << function_str << "\n";
        std::cout << "Inputs : \n";
        std::cout << "    bv   : " << std::setw(64) << std::right << bv << "\n";
        std::cout << "Obtained : " << std::setw(64) << std::right << value << "\n";
        std::cout << "Expected : " << std::setw(64) << std::right << expected_result << "\n\n";
        std::exit(1);
    }
}

#define COMMA ,

// Helper macro to capture the test details, including the file, line, and test expression.
#define test_binary(function, lhs, rhs, expected_result)                                                               \
    do {                                                                                                               \
        __test_binary(function, lhs, rhs, expected_result, __FILE__, __LINE__, #function);                             \
    } while (0)

// Helper macro to capture the test details, including the file, line, and test expression.
#define test_unary(function, bv, expected_result)                                                                      \
    do {                                                                                                               \
        __test_unary(function, bv, expected_result, __FILE__, __LINE__, #function);                                    \
    } while (0)
