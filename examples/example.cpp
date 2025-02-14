#include "bvlib/bitvector.hpp"
#include "bvlib/math.hpp"
#include "bvlib/io.hpp"

#include <iomanip>
#include <vector>
#include <random>

int main(int, char *[])
{
    bvlib::BitVector<6> a(0);
    for (std::size_t i = 0; i <= 0xF; ++i) {
        std::cout << a.to_string() << " : ";
        std::cout << std::setw(8) << std::right << a.to_number<int>() << " : ";
        std::cout << std::setw(8) << std::right << bvlib::detail::most_significant_bit(a) << "\n";
        a++;
    }
    return 0;
}