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
#include <iostream>
#include <cassert>
#include <cmath>

#ifndef MAX
#define MAX(A, B) (A > B ? A : B)
#endif
#ifndef MIN
#define MIN(A, B) (A < B ? A : B)
#endif

/// @brief Class which reproduces the behaviour of bit-vector.
/// @tparam N Lenght of the bit-vector
template <long unsigned int N>
class BitVector {
private:
    /// Internal vector of bits.
    bool bits[N];

    using size_type = long unsigned int;

public:
    explicit BitVector()
        : bits()
    {
        for (size_type it = 0; it < N; ++it)
            bits[it] = false;
    }

    explicit BitVector(size_type value)
        : bits()
    {
        for (size_type i = 0; i < N; ++i) {
            (*this)[i] = value % 2;
            value /= 2;
        }
    }

    explicit BitVector(const std::string &str)
        : bits()
    {
        for (std::string::size_type it = 0, len = str.length(); it < len; ++it)
            (*this)[it] = (str[len - 1 - it] == '1');
    }

    BitVector(BitVector<N> const &other)
        : bits()
    {
        for (size_type it = 0; it < N; ++it)
            (*this)[it] = other[it];
    }

    template <size_type N2, size_type MIN_SIZE = MIN(N, N2)>
    explicit BitVector(BitVector<N2> const &other)
        : bits()
    {
        for (size_type it = 0; it < MIN_SIZE; ++it)
            (*this)[it] = other[it];
    }

    virtual ~BitVector() = default;

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
    inline BitVector<N> &reset()
    {
        for (size_type it = 0; it < N; ++it)
            bits[it] = false;
        return (*this);
    }

    /// @brief Sets the given bit to false.
    inline BitVector<N> &reset(size_type position)
    {
        assert(position < N);
        (*this)[position] = false;
        return (*this);
    }

    /// @brief Flips every bit.
    inline BitVector<N> &flip()
    {
        for (size_type it = 0; it < N; ++it)
            bits[it] = !bits[it];
        return (*this);
    }

    /// @brief Flips a given bit.
    inline BitVector<N> &flip(size_type position)
    {
        assert(position < N);
        (*this)[position] = !(*this)[position];
        return (*this);
    }

    /// @brief Returns the size of the bit-vector.
    inline size_type size() const
    {
        return N;
    }

    /// @brief Returns the number of bits which are set.
    inline size_type count() const
    {
        size_type result = 0;
        for (size_type it = 0; it < N; ++it)
            result += bits[it];
        return result;
    }

    /// @brief Tests whether all the bits are on.
    inline bool all() const
    {
        for (size_type it = 0; it < N; ++it)
            if (!bits[it])
                return false;
        return true;
    }

    /// @brief Tests whether any of the bits are on.
    inline bool any() const
    {
        for (size_type it = 0; it < N; ++it)
            if (bits[it])
                return true;
        return false;
    }

    /// @brief Tests whether none of the bits are on.
    inline bool none() const
    {
        for (size_type it = 0; it < N; ++it)
            if (bits[it])
                return false;
        return true;
    }

    /// @brief Tests whether the sign bit is on.
    inline bool sign() const
    {
        return bits[0];
    }

    /// @brief Performs two-complement.
    inline BitVector<N> &two_complement()
    {
        return flip() += 1;
    }

    /// @brief Swaps the bits from 'start' to 'end'.
    inline BitVector<N> &swap(size_type start = 0, size_type end = N - 1)
    {
        do std::swap(bits[start], bits[end]);
        while (start++ < end--);
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template <size_type N2, size_type MIN_SIZE = MIN(N, N2)>
    inline BitVector<N> &assign(BitVector<N2> const &rhs)
    {
        this->reset();
        for (size_type it = 0; it < MIN_SIZE; ++it)
            (*this)[it] = rhs[it];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector, iterating from left to right.
    template <size_type N2, size_type MIN_SIZE = MIN(N, N2)>
    inline BitVector<N> &rassign(BitVector<N2> const &rhs)
    {
        this->reset();
        for (size_type it = 0; it < MIN_SIZE; ++it)
            (*this)[N - it - 1] = rhs[N2 - it - 1];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template <size_type N2, size_type MIN_SIZE = MIN(N, N2)>
    inline BitVector<N> &operator=(const BitVector<N2> &rhs)
    {
        this->reset();
        for (size_type it = 0; it < MIN_SIZE; ++it)
            (*this)[it] = rhs[it];
        return (*this);
    }

    /// @brief Transforms rhs into a BitVector.
    inline BitVector<N> &operator=(size_type rhs)
    {
        this->reset();
        for (size_type it = 0; it < N; ++it) {
            (*this)[it] = rhs % 2;
            rhs /= 2;
        }
        return *this;
    }

    /// @brief Transforms rhs into a BitVector.
    inline BitVector<N> &operator=(const std::string &str)
    {
        this->reset();
        for (std::string::size_type it = 0, len = str.length(); it < len; ++it)
            bits[N - 1 - it] = (str[len - 1 - it] == '1');
        return *this;
    }

    /// @brief Returns a reference to the bit at the given position.
    inline bool &at(size_type position)
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline bool at(size_type position) const
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Returns a reference to the bit at the given position.
    inline bool &operator[](size_type position)
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline bool operator[](size_type position) const
    {
        assert(position < N);
        return bits[N - position - 1];
    }

    /// @brief Sums rhs to this BitVector, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE> &sum(BitVector<N2> const &rhs, BitVector<MAX_SIZE> &result) const
    {
        bool carry = false;
        for (size_type it = 0; it < MAX_SIZE; ++it) {
            result[it] = this->add_bits((it < N) ? (*this)[it] : false,
                                        (it < N2) ? rhs[it] : false,
                                        carry);
        }
        return result;
    }

    /// @brief Substracts rhs to this BitVector, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE> &sub(BitVector<N2> const &rhs, BitVector<MAX_SIZE> &result) const
    {
        bool borrow = false;
        for (size_type it = 0; it < MAX_SIZE; ++it) {
            result[it] = this->sub_bits((it < N) ? (*this)[it] : false,
                                        (it < N2) ? rhs[it] : false,
                                        borrow);
        }
        return result;
    }

    /// @brief Multiplies rhs to this BitVector, the size of the output
    ///         BitVector is the maximum length of this and the rhs BitVectors
    ///         multiplied by 2.
    /// @tparam N2 The dimension of the other BitVector.
    /// @param rhs     Right-hand side element.
    /// @param result  Where the result will be accumulated.
    /// @param support A support BitVector.
    /// @return A reference to `result`.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE * 2> &mul(BitVector<N2> const &rhs,
                                        BitVector<MAX_SIZE * 2> &result,
                                        BitVector<MAX_SIZE * 2> &support) const
    {
        // Check the number of bits that are set.
        auto this_count = this->count(), rhs_count = rhs.count();
        // Perform the multiplication.
        if (this_count < rhs_count) {
            for (size_type it = 0; it < MAX_SIZE; ++it)
                if (this->operator[](it))
                    result += rhs.shift_left(it, support);
        } else {
            for (size_type it = 0; it < MAX_SIZE; ++it)
                if (rhs.operator[](it))
                    result += this->shift_left(it, support);
        }
        return result;
    }

    /// @brief Divides this BitVector for rhs, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    /// @details Original version available in:
    /// "C++ Cookbook - By D. Ryan Stephens, Ryan Stephens, Christopher Diggins,
    ///  Jeff Cogswell, Jonathan Turkanis"
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE> &div(const BitVector<N2> &rhs,
                                    BitVector<MAX_SIZE> &qotient,
                                    BitVector<MAX_SIZE> &remainder,
                                    BitVector<MAX_SIZE> &support) const
    {
        if (rhs.none())
            throw std::domain_error("division by zero undefined");
        if (this->none())
            return qotient;
        if ((*this) == rhs) {
            qotient[0] = true;
            return qotient;
        }
        if ((*this) < rhs)
            return qotient;
        // Initialize the remainder and the support vector.
        remainder = (*this);
        support   = rhs;
        // Count significant digits in lhs and rhs and dividend.
        size_type sig_lhs = most_significant_bit(*this);
        size_type sig_rhs = most_significant_bit(rhs);
        // Align the y with the dividend.
        size_type n = (sig_lhs - sig_rhs);
        support <<= n;
        // Make sure the loop executes the right number of times.
        n += 1;
        // Long division algorithm, shift, and subtract.
        while (n--) {
            // Shift the quotient to the left.
            if (support <= remainder) {
                // Add a new digit to quotient.
                qotient[n] = true;
                remainder -= support;
            }
            // Shift the y to the right.
            support >>= 1;
        }
        return qotient;
    }

    /// @brief Left-shifts this BitVector by the given number of bits. It uses
    ///         the `support` bitvector to store the result.
    template <size_type N2>
    inline BitVector<N2> &shift_right(size_type shift, BitVector<N2> &support) const
    {
        return ((support = (*this)) >>= shift);
    }

    /// @brief Left-shifts this BitVector by the given number of bits. It uses
    //    ///         the `support` bitvector to store the result.
    template <size_type N2>
    inline BitVector<N2> &shift_left(size_type shift, BitVector<N2> &support) const
    {
        return ((support = (*this)) <<= shift);
    }

    /// @brief Sums rhs to this BitVector, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE> operator+(BitVector<N2> const &rhs) const
    {
        BitVector<MAX_SIZE> result;
        return this->sum<N2, MAX_SIZE>(rhs, result);
    }

    /// @brief Sums this BitVector and rhs.
    inline BitVector<N> operator+(size_type rhs) const
    {
        return (*this) + BitVector<N>(rhs);
    }

    /// @brief Sums this BitVector and rhs and assign the result to this.
    inline BitVector<N> &operator+=(BitVector<N> const &rhs)
    {
        return this->sum(rhs, *this);
    }

    /// @brief Sums this BitVector and rhs and assign the result to this.
    inline BitVector<N> &operator+=(size_type rhs)
    {
        return (*this) += BitVector<N>(rhs);
    }

    /// @brief Substracts rhs to this BitVector, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE> operator-(BitVector<N2> const &rhs) const
    {
        BitVector<MAX_SIZE> result;
        return this->sub<N2, MAX_SIZE>(rhs, result);
    }

    /// @brief Subtracts this BitVector and rhs.
    inline BitVector<N> operator-(size_type rhs) const
    {
        return (*this) - BitVector<N>(rhs);
    }

    /// @brief Subtracts this BitVector and rhs and assign the result to this.
    inline BitVector<N> &operator-=(BitVector<N> const &rhs)
    {
        return this->sub(rhs, *this);
    }

    /// @brief Subtracts this BitVector and rhs and assign the result to this.
    inline BitVector<N> &operator-=(size_type rhs)
    {
        return (*this) = (*this) - rhs;
    }

    /// @brief Multiplies rhs to this BitVector, the size of the output
    /// BitVector is the maximum length of this and the rhs BitVectors
    /// multiplied by 2.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE * 2> operator*(BitVector<N2> const &rhs) const
    {
        BitVector<MAX_SIZE * 2> result, support;
        return this->mul(rhs, result, support);
    }

    /// @brief Multiplies this BitVector and rhs.
    inline BitVector<N * 2> operator*(size_type rhs) const
    {
        return (*this) * BitVector<N>(rhs);
    }

    /// @brief Divides this BitVector for rhs, the size of the output BitVector
    /// is the maximum length of this and the rhs BitVectors.
    /// @details Original version available in:
    /// "C++ Cookbook - By D. Ryan Stephens, Ryan Stephens, Christopher Diggins,
    ///  Jeff Cogswell, Jonathan Turkanis"
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline BitVector<MAX_SIZE> operator/(BitVector<N2> rhs) const
    {
        BitVector<MAX_SIZE> qotient, remainder, support;
        return this->div(rhs, qotient, remainder, support);
    }

    /// @brief Divides this BitVector for rhs.
    inline BitVector<N> operator/(size_type rhs) const
    {
        return (*this) / BitVector<N>(rhs);
    }

    /// @brief Right-shifts this BitVector by the given number of bits.
    inline BitVector<N> operator>>(size_type shift) const
    {
        BitVector<N> result = (*this);
        result >>= shift;
        return result;
    }

    /// @brief Right-shifts this BitVector by the given number of bits,
    /// modifying this BitVector.
    inline BitVector<N> &operator>>=(size_type shift)
    {
        if (shift > 0) {
            for (size_type it = N - 1; it >= shift; --it)
                bits[it] = bits[it - shift];
            for (size_type it = 0; it < shift; ++it)
                bits[it] = false;
        }
        return (*this);
    }

    /// @brief Left-shifts this BitVector by the given number of bits.
    inline BitVector<N> operator<<(size_type shift) const
    {
        BitVector<N> result = (*this);
        result <<= shift;
        return result;
    }

    /// @brief Left-shifts this BitVector by the given number of bits,
    /// modifying this BitVector.
    inline BitVector<N> &operator<<=(size_type shift)
    {
        if (shift > 0) {
            for (size_type it = 0; it < (N - shift); ++it)
                bits[it] = bits[it + shift];
            for (size_type it = (N - shift); it < N; ++it)
                bits[it] = false;
        }
        return (*this);
    }

    /// @brief Checks the equality between rhs and this.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline bool operator==(BitVector<N2> const &rhs) const
    {
        for (size_type it = MAX(N, N2); it > 0; --it) {
            bool a = ((it - 1) < N) ? (*this)[it - 1] : false;
            bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
            if (a != b)
                return false;
        }
        return true;
    }

    /// @brief Checks the equality between rhs and this.
    inline bool operator==(size_type rhs) const
    {
        return (*this) == BitVector<N>(rhs);
    }

    /// @brief Checks the inequality between rhs and this.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline bool operator!=(BitVector<N2> const &rhs) const
    {
        for (size_type it = MAX_SIZE; it > 0; --it) {
            bool a = ((it - 1) < N) ? (*this)[it - 1] : false;
            bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
            if (a != b)
                return true;
        }
        return false;
    }

    /// @brief Checks the inequality between rhs and this.
    inline bool operator!=(size_type rhs) const
    {
        return (*this) != BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is lesser-than rhs.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline bool operator<(BitVector<N2> const &rhs) const
    {
        for (size_type it = MAX_SIZE; it > 0; --it) {
            bool a = ((it - 1) < N) ? (*this)[it - 1] : false;
            bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
            if (a && !b)
                return false;
            if (!a && b)
                return true;
        }
        return false;
    }

    /// @brief Checks if the value of this is lesser-than rhs.
    inline bool operator<(size_type rhs) const
    {
        return (*this) < BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is greater-than rhs.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline bool operator>(BitVector<N2> const &rhs) const
    {
        for (size_type it = MAX_SIZE; it > 0; --it) {
            bool a = ((it - 1) < N) ? (*this)[it - 1] : false;
            bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
            if (!a && b)
                return false;
            if (a && !b)
                return true;
        }
        return false;
    }

    /// @brief Checks if the value of this is greater-than rhs.
    inline bool operator>(size_type rhs) const
    {
        return (*this) > BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is less-than-or-equal-to rhs.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline bool operator<=(BitVector<N2> const &rhs)
    {
        for (size_type it = MAX_SIZE; it > 0; --it) {
            bool a = ((it - 1) < N) ? (*this)[it - 1] : false;
            bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
            if (a && !b)
                return false;
            if (!a && b)
                return true;
        }
        return true;
    }

    /// @brief Checks if the value of this is less-than-or-equal-to rhs.
    inline bool operator<=(size_type rhs) const
    {
        return (*this) <= BitVector<N>(rhs);
    }

    /// @brief Checks if the value of this is greater-than-or-equal-to rhs.
    template <size_type N2, size_type MAX_SIZE = MAX(N, N2)>
    inline bool operator>=(BitVector<N2> const &rhs)
    {
        for (size_type it = MAX_SIZE; it > 0; --it) {
            bool a = ((it - 1) < N) ? (*this)[it - 1] : false;
            bool b = ((it - 1) < N2) ? rhs[it - 1] : false;
            if (!a && b)
                return false;
            if (a && !b)
                return true;
        }
        return true;
    }

    /// @brief Checks if the value of this is greater-than-or-equal-to rhs.
    inline bool operator>=(size_type rhs) const
    {
        return (*this) <= BitVector<N>(rhs);
    }

    /// @brief Transforms this BitVector to string.
    inline std::string to_string() const
    {
        std::string str;
        for (size_type it = 0; it < N; ++it)
            str.push_back(bits[it] ? '1' : '0');
        return str;
    }

    /// @brief Transforms this BitVector to number.
    template <typename DataType = long>
    inline DataType to_number() const
    {
        DataType result = 0;
        for (long it = N - 1; it >= 0; --it)
            if (bits[it])
                result += std::pow(2, N - it - 1);
        return result;
    }

private:
    static inline bool add_bits(bool b1, bool b2, bool &carry)
    {
        bool sum = (b1 ^ b2) ^ carry;
        carry    = (b1 && b2) || (b1 && carry) || (b2 && carry);
        return sum;
    }

    static inline bool sub_bits(bool b1, bool b2, bool &borrow)
    {
        bool diff;
        if (borrow) {
            diff   = !(b1 ^ b2);
            borrow = !b1 || b2;
        } else {
            diff   = b1 ^ b2;
            borrow = !b1 && b2;
        }
        return diff;
    };

    template <size_type DIM>
    static inline size_type most_significant_bit(BitVector<DIM> const &a)
    {
        for (long i = DIM - 1; i >= 0; i--)
            if (a[i])
                return i;
        return 0L;
    }
};

/// @brief Overrides the output stream operator.
template <long unsigned int N>
inline std::ostream &operator<<(std::ostream &lhs, BitVector<N> const &rhs)
{
    lhs << rhs.to_number();
    return lhs;
}

template <long unsigned int N>
inline BitVector<N> operator+(long unsigned int lhs,
                              BitVector<N> const &rhs)
{
    return BitVector<N>(lhs) + rhs;
}

template <long unsigned int N>
inline BitVector<N> operator-(long unsigned int lhs,
                              BitVector<N> const &rhs)
{
    return BitVector<N>(lhs) - rhs;
}

template <long unsigned int N>
inline BitVector<2 * N> operator*(long unsigned int lhs,
                                  BitVector<N> const &rhs)
{
    return BitVector<N>(lhs) * rhs;
}

template <long unsigned int N>
inline BitVector<N> operator/(long unsigned int lhs,
                              BitVector<N> const &rhs)
{
    return BitVector<N>(lhs) / rhs;
}