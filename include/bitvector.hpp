/// @file bitvector.hpp
/// @brief Complete BitVector class.
/// @date 7/4/19
/// @author Enrico Fraccaroli
/// @copyright
/// Copyright (c) 2019 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///     The above copyright notice and this permission notice shall be included
///     in all copies or substantial portions of the Software.
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#pragma once

#include <cassert>
#include <cmath>

#ifndef MAX
#define MAX(A, B)(A > B ? A : B)
#endif
#ifndef MIN
#define MIN(A, B)(A < B ? A : B)
#endif

/// @brief Class which reproduces the behaviour of bit-vector.
/// @tparam N Lenght of the bit-vector
template<long unsigned int N>
class BitVector
{
private:
    /// Internal vector of bits.
    bool bits[N];

public:

    explicit BitVector() :
        bits()
    {
        for (long it = 0; it < N; ++it)
            bits[it] = false;
    }

    BitVector(long unsigned int value) :
        bits()
    {
        for (long unsigned int i = 0; i < N; ++i)
        {
            (*this)[i] = value % 2;
            value /= 2;
        }
    }

    BitVector(std::string str) :
        bits()
    {
        if (N > str.length())
            str.insert(0, std::string(N - str.length(), '0'));
        for (long it = N - 1; it >= 0; --it)
            bits[it] = (str[it] == '1');
    }

    template<long unsigned int N2>
    BitVector(BitVector<N2> const & other) :
        bits()
    {
        for (long it = 0; it < N; ++it)
            (*this)[it] = (it < N2) ? other[it] : false;
    }

    /// @brief Returns a bitvector of all ones.
    static inline BitVector<N> ones()
    {
        return BitVector<N>().flip();
    }

    /// @brief Returns a bitvector of all zeros.
    static inline BitVector<N> zeros()
    {
        return BitVector<N>();
    }

    /// @brief Sets every bit to false.
    inline BitVector<N> & reset()
    {
        for (long it = 0; it < N; ++it)
            bits[it] = false;
        return (*this);
    }

    /// @brief Sets the given bit to false.
    inline BitVector<N> & reset(size_t position)
    {
        assert(position < N);
        (*this)[position] = false;
        return (*this);
    }

    /// @brief Flips every bit.
    inline BitVector<N> & flip()
    {
        for (long it = 0; it < N; ++it)
            bits[it] = !bits[it];
        return (*this);
    }

    /// @brief Flips a given bit.
    inline BitVector<N> & flip(size_t position)
    {
        assert(position < N);
        (*this)[position] = !(*this)[position];
        return (*this);
    }

    /// @brief Returns the size of the bit-vector.
    inline size_t size() const
    {
        return N;
    }

    /// @brief Returns the number of bits which are set.
    inline size_t count() const
    {
        size_t result = 0;
        for (long it = 0; it < N; ++it)
            result += bits[it];
    }

    /// @brief Tests whether all the bits are on.
    inline bool all() const
    {
        for (long it = 0; it < N; ++it)
            if (!bits[it]) return false;
        return true;
    }

    /// @brief Tests whether any of the bits are on.
    inline bool any() const
    {
        for (long it = 0; it < N; ++it)
            if (bits[it]) return true;
        return false;
    }

    /// @brief Tests whether none of the bits are on.
    inline bool none() const
    {
        for (long it = 0; it < N; ++it)
            if (bits[it]) return false;
        return true;
    }

    /// @brief Tests whether the sign bit is on.
    inline bool sign() const
    {
        return bits[0];
    }

    /// @brief Performs two-complement.
    inline BitVector<N> & two_complement()
    {
        return ((*this) = flip() + 1);
    }

    /// @brief Swaps the bits from 'start' to 'end'.
    inline BitVector<N> & swap(size_t start = 0, size_t end = N - 1)
    {
        do std::swap(bits[start], bits[end]); while (start++ < end--);
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template<long unsigned int N2>
    inline BitVector<N> & assign(BitVector<N2> const & rhs)
    {
        constexpr long unsigned int min_size = MIN(N, N2);
        reset();
        for (long it = 0; it < min_size; ++it)
            (*this)[it] = rhs[it];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector, iterating from left to right.
    template<long unsigned int N2>
    inline BitVector<N> & rassign(BitVector<N2> const & rhs)
    {
        constexpr long unsigned int min_size = MIN(N, N2);
        reset();
        for (long it = 0; it < min_size; ++it)
            (*this)[N - it - 1] = rhs[N2 - it - 1];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template<long unsigned int N2>
    inline BitVector<N> & operator=(BitVector<N2> const & rhs)
    {
        constexpr long unsigned int min_size = MIN(N, N2);
        reset();
        for (long it = 0; it < min_size; ++it)
            (*this)[it] = rhs[it];
        return (*this);
    }

    /// @brief Transforms rhs into a BitVector.
    inline BitVector<N> & operator=(long unsigned int rhs)
    {
        reset();
        for (long unsigned int it = 0; it < N; ++it)
        {
            (*this)[it] = rhs % 2;
            rhs /= 2;
        }
    }

    /// @brief Transforms rhs into a BitVector.
    inline BitVector<N> & operator=(std::string str)
    {
        reset();
        if (N > str.length())
            str.insert(0, std::string(N - str.length(), '0'));
        for (long it = N - 1; it >= 0; --it)
            bits[it] = (str[it] == '1');
    }

    /// @brief Returns a reference to the bit at the given position.
    inline bool & at(size_t position)
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline bool at(size_t position) const
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Returns a reference to the bit at the given position.
    inline bool & operator[](size_t position)
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline bool operator[](size_t position) const
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Sums rhs to this BitVector, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    template<long unsigned int N2>
    inline BitVector<MAX(N, N2)> operator+(BitVector<N2> const & rhs) const
    {
        constexpr long unsigned int max_size = MAX(N, N2);
        auto add = [](bool b1, bool b2, bool & carry)
        {
            bool sum = (b1 ^ b2) ^carry;
            carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
            return sum;
        };
        BitVector<max_size> result;
        bool carry = false;
        for (long it = 0; it < max_size; ++it)
        {
            result[it] = add((it < N) ? (*this)[it] : false,
                             (it < N2) ? rhs[it] : false,
                             carry);
        }
        return result;
    }

    /// @brief Sums this BitVector and rhs.
    inline BitVector<N> operator+(long unsigned int rhs) const
    {
        return (*this) + BitVector<N>(rhs);
    }

    /// @brief Sums this BitVector and rhs and assign the result to this.
    inline BitVector<N> operator+=(BitVector<N> const & rhs)
    {
        return (*this) = (*this) + rhs;
    }

    /// @brief Sums this BitVector and rhs and assign the result to this.
    inline BitVector<N> operator+=(long unsigned int rhs)
    {
        return (*this) = (*this) + rhs;
    }

    /// @brief Substracts rhs to this BitVector, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    template<long unsigned int N2>
    inline BitVector<MAX(N, N2)> operator-(BitVector<N2> const & rhs) const
    {
        constexpr long unsigned int max_size = MAX(N, N2);
        auto sub = [](bool b1, bool b2, bool & borrow)
        {
            bool diff;
            if (borrow)
            {
                diff = !(b1 ^ b2);
                borrow = !b1 || (b1 && b2);
            }
            else
            {
                diff = b1 ^ b2;
                borrow = !b1 && b2;
            }
            return diff;
        };

        BitVector<max_size> result;
        bool borrow = false;
        for (long it = 0; it < max_size; ++it)
        {
            result[it] = sub((it < N) ? (*this)[it] : false,
                             (it < N2) ? rhs[it] : false,
                             borrow);
        }
        return result;
    }

    /// @brief Subtracts this BitVector and rhs.
    inline BitVector<N> operator-(long unsigned int rhs) const
    {
        return (*this) - BitVector<N>(rhs);
    }

    /// @brief Subtracts this BitVector and rhs and assign the result to this.
    inline BitVector<N> operator-=(BitVector<N> const & rhs)
    {
        return (*this) = (*this) - rhs;
    }

    /// @brief Subtracts this BitVector and rhs and assign the result to this.
    inline BitVector<N> operator-=(long unsigned int rhs)
    {
        return (*this) = (*this) - rhs;
    }

    /// @brief Multiplies rhs to this BitVector, the size of the output
    /// BitVector is the maximum length of this and the rhs BitVectors
    /// multiplied by 2.
    template<long unsigned int N2>
    inline BitVector<MAX(N, N2) * 2>
    operator*(BitVector<N2> const & rhs) const
    {
        constexpr long unsigned int max_size = MAX(N, N2);
        // Check the number of bits that are set.
        bool _count = (this->count() < rhs.count());
        // Resize the two BitVectors to match the maximum size.
        BitVector<max_size * 2> result,
            x(_count ? *this : rhs),
            y(_count ? rhs : *this);
        // Perform the multiplication.
        for (long it = 0; it < max_size; ++it)
            if (x[it])
                result += (y << it);
        return result;
    }

    /// @brief Multiplies this BitVector and rhs.
    inline BitVector<N * 2> operator*(long unsigned int rhs) const
    {
        return (*this) * BitVector<N>(rhs);
    }

    /// @brief Divides this BitVector for rhs, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    /// @details Original version available in:
    /// "C++ Cookbook - By D. Ryan Stephens, Ryan Stephens, Christopher Diggins,
    ///  Jeff Cogswell, Jonathan Turkanis"
    template<long unsigned int N2>
    inline BitVector<MAX(N, N2)> operator/(BitVector<N2> rhs) const
    {
        constexpr long unsigned int max_size = MAX(N, N2);

        auto get_most_significant_bit = [](BitVector<max_size> const & a)
        {
            for (long i = max_size - 1; i >= 0; i--)
                if (a[i]) return i;
            return 0L;
        };
        BitVector<max_size> qotient, remainder(to_string());
        BitVector<max_size> x = remainder, y(rhs.to_string());
        if (y.none())
            throw std::domain_error("division by zero undefined");
        if (x.none())
            return qotient;
        if (x == y)
        {
            qotient[0] = true;
            return qotient;
        }
        if (x < y)
            return qotient;
        // Count significant digits in y and dividend.
        size_t sig_x = get_most_significant_bit(x);
        size_t sig_y = get_most_significant_bit(y);
        // Align the y with the dividend.
        size_t n = (sig_x - sig_y);
        y <<= n;
        // Make sure the loop executes the right number of times.
        n += 1;
        // Long division algorithm, shift, and subtract.
        while (n--)
        {
            // Shift the quotient to the left.
            if (y <= remainder)
            {
                // Add a new digit to quotient.
                qotient[n] = true;
                remainder -= y;
            }
            // Shift the y to the right.
            y >>= 1;
        }
        return qotient;
    }

    /// @brief Divides this BitVector for rhs.
    inline BitVector<N> operator/(long unsigned int rhs) const
    {
        return (*this) / BitVector<N>(rhs);
    }

    /// @brief Right-shifts this BitVector by the given number of bits.
    inline BitVector<N> operator>>(size_t shift) const
    {
        BitVector<N> result = (*this);
        result >>= shift;
        return result;
    }

    /// @brief Right-shifts this BitVector by the given number of bits,
    /// modifying this BitVector.
    inline BitVector<N> & operator>>=(size_t shift)
    {
        if (shift > 0)
        {
            for (size_t it = N - 1; it >= shift; --it)
                bits[it] = bits[it - shift];
            for (size_t it = 0; it < shift; ++it)
                bits[it] = false;
        }
        return (*this);
    }

    /// @brief Left-shifts this BitVector by the given number of bits.
    inline BitVector<N> operator<<(size_t shift) const
    {
        BitVector<N> result = (*this);
        result <<= shift;
        return result;
    }

    /// @brief Left-shifts this BitVector by the given number of bits,
    /// modifying this BitVector.
    inline BitVector<N> & operator<<=(size_t shift)
    {
        if (shift > 0)
        {
            for (size_t it = 0; it < (N - shift); ++it)
                bits[it] = bits[it + shift];
            for (size_t it = (N - shift); it < N; ++it)
                bits[it] = false;
        }
        return (*this);
    }

    /// @brief Checks the equality between rhs and this.
    template<long unsigned int N2>
    inline bool operator==(BitVector<N2> const & rhs) const
    {
        for (int it = MAX(N, N2) - 1; it >= 0; --it)
        {
            bool a = (it < N) ? (*this)[it] : false;
            bool b = (it < N2) ? rhs[it] : false;
            if (a != b) return false;
        }
        return true;
    }

    /// @brief Checks the equality between rhs and this.
    inline bool operator==(long unsigned int rhs) const
    {
        return (*this) == BitVector<N>(rhs);
    }

    /// @brief Checks the inequality between rhs and this.
    template<long unsigned int N2>
    inline bool operator!=(BitVector<N2> const & rhs) const
    {
        for (int it = MAX(N, N2) - 1; it >= 0; --it)
        {
            bool a = (it < N) ? (*this)[it] : false;
            bool b = (it < N2) ? rhs[it] : false;
            if (a != b) return true;
        }
        return false;
    }

    /// @brief Checks the inequality between rhs and this.
    inline bool operator!=(long unsigned int rhs) const
    {
        return (*this) != BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is lesser-than rhs.
    template<long unsigned int N2>
    inline bool operator<(BitVector<N2> const & rhs) const
    {
        for (int it = MAX(N, N2) - 1; it >= 0; --it)
        {
            bool a = (it < N) ? (*this)[it] : false;
            bool b = (it < N2) ? rhs[it] : false;
            if (a && !b) return false;
            if (!a && b) return true;
        }
        return false;
    }

    /// @brief Checks if the value of this is lesser-than rhs.
    inline bool operator<(long unsigned int rhs) const
    {
        return (*this) < BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is greater-than rhs.
    template<long unsigned int N2>
    inline bool operator>(BitVector<N2> const & rhs) const
    {
        for (int it = MAX(N, N2) - 1; it >= 0; --it)
        {
            bool a = (it < N) ? (*this)[it] : false;
            bool b = (it < N2) ? rhs[it] : false;
            if (!a && b) return false;
            if (a && !b) return true;
        }
        return false;
    }

    /// @brief Checks if the value of this is greater-than rhs.
    inline bool operator>(long unsigned int rhs) const
    {
        return (*this) > BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is less-than-or-equal-to rhs.
    template<long unsigned int N2>
    inline bool operator<=(BitVector<N2> const & rhs)
    {
        for (int it = MAX(N, N2) - 1; it >= 0; --it)
        {
            bool a = (it < N) ? (*this)[it] : false;
            bool b = (it < N2) ? rhs[it] : false;
            if (a && !b) return false;
            if (!a && b) return true;
        }
        return true;
    }

    /// @brief Checks if the value of this is less-than-or-equal-to rhs.
    inline bool operator<=(long unsigned int rhs) const
    {
        return (*this) <= BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is greater-than-or-equal-to rhs.
    template<long unsigned int N2>
    inline bool operator>=(BitVector<N2> const & rhs)
    {
        for (int it = MAX(N, N2) - 1; it >= 0; --it)
        {
            bool a = (it < N) ? (*this)[it] : false;
            bool b = (it < N2) ? rhs[it] : false;
            if (!a && b) return false;
            if (a && !b) return true;
        }
        return true;
    }

    /// @brief Checks if the value of this is greater-than-or-equal-to rhs.
    inline bool operator>=(long unsigned int rhs) const
    {
        return (*this) <= BitVector<N>(rhs);
    }

    /// @brief Transforms this BitVector to string.
    inline std::string to_string() const
    {
        std::string str;
        for (long it = 0; it < N; ++it)
            str.push_back(bits[it] ? '1' : '0');
        return str;
    }

    /// @brief Transforms this BitVector to number.
    template<typename DataType = long>
    inline DataType to_number() const
    {
        DataType result = 0;
        for (long it = N - 1; it >= 0; --it)
            if (bits[it])
                result += std::pow(2, N - it - 1);
        return result;
    }

};

/// @brief Overrides the output stream operator.
template<long unsigned int N>
inline std::ostream & operator<<(std::ostream & lhs, BitVector<N> const & rhs)
{
    lhs << rhs.to_number();
    return lhs;
}

template<long unsigned int N>
inline BitVector<N> operator+(long unsigned int lhs,
                              BitVector<N> const & rhs)
{
    return BitVector<N>(lhs) + rhs;
}

template<long unsigned int N>
inline BitVector<N> operator-(long unsigned int lhs,
                              BitVector<N> const & rhs)
{
    return BitVector<N>(lhs) - rhs;
}

template<long unsigned int N>
inline BitVector<2 * N> operator*(long unsigned int lhs,
                                  BitVector<N> const & rhs)
{
    return BitVector<N>(lhs) * rhs;
}

template<long unsigned int N>
inline BitVector<N> operator/(long unsigned int lhs,
                              BitVector<N> const & rhs)
{
    return BitVector<N>(lhs) / rhs;
}