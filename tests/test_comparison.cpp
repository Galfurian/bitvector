#include "bvlib/bitvector.hpp"
#include "bvlib/math.hpp"
#include "bvlib/io.hpp"

#include <iomanip>
#include <vector>
#include <random>

template <std::size_t N>
struct test_bv_pair_t {
    bvlib::BitVector<N> bitvector;
    std::size_t original;
    test_bv_pair_t(std::size_t _original = 0)
        : bitvector(_original),
          original(_original)
    {
    }
};

template <std::size_t N1, std::size_t N2, std::size_t QT>
int test_operators()
{
    test_bv_pair_t<N1> inputs1[QT];
    test_bv_pair_t<N2> inputs2[QT];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(1UL, static_cast<std::size_t>(std::pow(N2 / 2, 2) - 1));

    for (std::size_t i = 0; i < QT; ++i) {
        inputs1[i] = test_bv_pair_t<N1>(distr(gen));
        inputs2[i] = test_bv_pair_t<N2>(distr(gen));
    }

    for (std::size_t i = 0; i < QT; ++i) {
        for (std::size_t j = 0; j < QT; ++j) {
            auto bv_result = inputs1[i].bitvector < inputs2[j].bitvector;
            auto nm_result = inputs1[i].original < inputs2[j].original;
            if (bv_result != nm_result) {
                std::cerr << inputs1[i].bitvector << " < " << inputs2[j].bitvector << " = " << bv_result << " (" << N1 << "&" << N2 << " bits)!= \n";
                std::cerr << inputs1[i].original << " < " << inputs2[j].original << " = " << nm_result << "\n\n";
                return 1;
            }
        }
    }
    for (std::size_t i = 0; i < QT; ++i) {
        for (std::size_t j = 0; j < QT; ++j) {
            auto bv_result = inputs1[i].bitvector <= inputs2[j].bitvector;
            auto nm_result = inputs1[i].original <= inputs2[j].original;
            if (bv_result != nm_result) {
                std::cerr << inputs1[i].bitvector << " <= " << inputs2[j].bitvector << " = " << bv_result << " (" << N1 << "&" << N2 << " bits)!= \n";
                std::cerr << inputs1[i].original << " <= " << inputs2[j].original << " = " << nm_result << "\n\n";
                return 1;
            }
        }
    }
    for (std::size_t i = 0; i < QT; ++i) {
        for (std::size_t j = 0; j < QT; ++j) {
            auto bv_result = inputs1[i].bitvector > inputs2[j].bitvector;
            auto nm_result = inputs1[i].original > inputs2[j].original;
            if (bv_result != nm_result) {
                std::cerr << inputs1[i].bitvector << " > " << inputs2[j].bitvector << " = " << bv_result << " (" << N1 << "&" << N2 << " bits)!= \n";
                std::cerr << inputs1[i].original << " > " << inputs2[j].original << " = " << nm_result << "\n\n";
                return 1;
            }
        }
    }
    for (std::size_t i = 0; i < QT; ++i) {
        for (std::size_t j = 0; j < QT; ++j) {
            auto bv_result = inputs1[i].bitvector >= inputs2[j].bitvector;
            auto nm_result = inputs1[i].original >= inputs2[j].original;
            if (bv_result != nm_result) {
                std::cerr << inputs1[i].bitvector << " >= " << inputs2[j].bitvector << " = " << bv_result << " (" << N1 << "&" << N2 << " bits)!= \n";
                std::cerr << inputs1[i].original << " >= " << inputs2[j].original << " = " << nm_result << "\n\n";
                return 1;
            }
        }
    }
    for (std::size_t i = 0; i < QT; ++i) {
        for (std::size_t j = 0; j < QT; ++j) {
            auto bv_result = inputs1[i].bitvector == inputs2[j].bitvector;
            auto nm_result = inputs1[i].original == inputs2[j].original;
            if (bv_result != nm_result) {
                std::cerr << inputs1[i].bitvector << " == " << inputs2[j].bitvector << " = " << bv_result << " (" << N1 << "&" << N2 << " bits)!= \n";
                std::cerr << inputs1[i].original << " == " << inputs2[j].original << " = " << nm_result << "\n\n";
                return 1;
            }
        }
    }
    for (std::size_t i = 0; i < QT; ++i) {
        for (std::size_t j = 0; j < QT; ++j) {
            auto bv_result = inputs1[i].bitvector != inputs2[j].bitvector;
            auto nm_result = inputs1[i].original != inputs2[j].original;
            if (bv_result != nm_result) {
                std::cerr << inputs1[i].bitvector << " != " << inputs2[j].bitvector << " = " << bv_result << " (" << N1 << "&" << N2 << " bits)!= \n";
                std::cerr << inputs1[i].original << " != " << inputs2[j].original << " = " << nm_result << "\n\n";
                return 1;
            }
        }
    }
    return 0;
}

int main(int, char *[])
{
    if (test_operators<8, 4, 256>())
        return 1;
    if (test_operators<16, 8, 256>())
        return 1;
    if (test_operators<32, 16, 256>())
        return 1;
    if (test_operators<64, 32, 256>())
        return 1;
    if (test_operators<128, 64, 256>())
        return 1;
    if (test_operators<256, 128, 256>())
        return 1;
    return 0;
}