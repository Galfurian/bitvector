#include <iostream>
#include <iomanip>
#include <vector>

#include "sbvlib/scalarbv.hpp"

using namespace sbvlib;

#define TEST_BINARY_OP(op)                                                      \
    std::cout << "== Testing " #op " " << std::string(25, '=') << "\n";         \
    for (size_t it = 0; it < inputs.size(); ++it) {                             \
        for (size_t it2 = 0; it2 < inputs.size(); ++it2) {                      \
            auto &a = inputs[it], &b = inputs[it2];                             \
            std::cout                                                           \
                << std::setw(12) << a.bitVector << " " #op " "                  \
                << std::setw(12) << b.bitVector << " = "                        \
                << std::setw(12) << std::right << (a.bitVector op b.bitVector)  \
                << " == "                                                       \
                << std::setw(12) << std::left << (a.original op b.original)     \
                << ((a.bitVector op b.bitVector) == (a.original op b.original)) \
                << "\n";                                                        \
            if ((a.bitVector op b.bitVector) != (a.original op b.original)) {   \
                return 1;                                                       \
            }                                                                   \
        }                                                                       \
    }

#define TEST_UNARY_OP(op)                                                          \
    std::cout << "== Testing " #op " " << std::string(25, '=') << "\n";            \
    for (const auto &input : inputs) {                                             \
        std::cout << " " #op << std::setw(12) << input.bitVector << " = "          \
                  << std::setw(12) << std::right << (op input.bitVector) << " == " \
                  << std::setw(12) << std::left << (op input.original)             \
                  << ((op input.bitVector) == (op input.original))                 \
                  << "\n";                                                         \
    }

struct test_sbv_pair_t {
    ScalarBitVector bitVector;
    ScalarBitVector::value_type_t original;
    test_sbv_pair_t(unsigned long _original)
        : bitVector(_original),
          original(_original)
    {
    }
};

int main(int argc, char *argv[])
{
    std::vector<test_sbv_pair_t> inputs;
    inputs.emplace_back(test_sbv_pair_t(6));
    inputs.emplace_back(test_sbv_pair_t(4));
    inputs.emplace_back(test_sbv_pair_t(2));
    inputs.emplace_back(test_sbv_pair_t(25));
    inputs.emplace_back(test_sbv_pair_t(321));
    inputs.emplace_back(test_sbv_pair_t(76));
    inputs.emplace_back(test_sbv_pair_t(1023));
    inputs.emplace_back(test_sbv_pair_t(65));

    std::cout << "== INPUTS " << std::string(25, '=') << "\n";
    for (size_t it = 0; it < inputs.size(); ++it) {
        std::cout << "op" << it
                  << " [" << inputs[it].bitVector.to_string() << "] "
                  << inputs[it].bitVector.to_number()
                  << " (" << inputs[it].original << ")\n";
    }
    std::cout << std::string(35, '=') << "\n";
    TEST_BINARY_OP(+)
    TEST_BINARY_OP(-)
    TEST_BINARY_OP(*)
    TEST_BINARY_OP(/)
    std::cout << std::string(35, '=') << "\n";
    TEST_BINARY_OP(<)
    TEST_BINARY_OP(<=)
    TEST_BINARY_OP(>)
    TEST_BINARY_OP(>=)
    TEST_BINARY_OP(==)
    TEST_BINARY_OP(!=)
    TEST_BINARY_OP(|)
    TEST_UNARY_OP(!)
    TEST_UNARY_OP(~)
    std::cout << std::string(35, '=') << "\n";
    return 0;
}