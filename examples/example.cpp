#include "bvlib/bitvector.hpp"
#include "bvlib/math.hpp"
#include "bvlib/io.hpp"

#include <iomanip>
#include <vector>
#include <random>

int main(int, char *[])
{
    bvlib::BitVector<8> a(8);
    std::cout << a << "\n";
    a = bvlib::shift_left(a, 9);
    std::cout << a << "\n";
    return 0;
}