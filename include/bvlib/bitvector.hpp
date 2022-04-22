/// @file bitvector.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Base bitvector class.

#pragma once

#include <stdexcept>
#include <string>
#include <cmath>

namespace bvlib
{

/// Type for sizes inside the bvlib namespace.
using size_type_t = long unsigned int;

/// @brief Class which reproduces the behaviour of bit-vector.
/// @tparam N Lenght of the bit-vector
template <size_type_t N>
class BitVector {
public:
    /// Internal vector of bits.
    bool bits[N];

    /// @brief Construct a new bitvector.
    explicit BitVector()
        : bits()
    {
        for (bvlib::size_type_t it = 0; it < N; ++it)
            bits[it] = false;
    }

    /// @brief Construct a new bitvector and intializes it with the given value.
    /// @param value the initial value.
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    explicit BitVector(T value)
        : bits()
    {
        for (bvlib::size_type_t it = 0; it < N; ++it) {
            this->at(it) = value % 2;
            value /= 2;
        }
    }

    /// @brief Construct a new bitvector based on the given string (e.g. "1010111").
    /// @param str the input string.
    explicit BitVector(const std::string &str)
        : bits()
    {
        for (std::string::size_type it = 0, len = str.length(); it < len; ++it)
            this->at(it) = (str[len - 1 - it] == '1');
    }

    /// @brief Copies the other bitvector into this one.
    /// @param other the other bitvector.
    template <bvlib::size_type_t N2>
    explicit BitVector(BitVector<N2> const &other)
        : bits()
    {
        bvlib::size_type_t min_size = std::min(N, N2);
        for (bvlib::size_type_t it = 0; it < min_size; ++it)
            this->at(it) = other[it];
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
        for (bvlib::size_type_t it = 0; it < N; ++it)
            bits[it] = false;
        return (*this);
    }

    /// @brief Sets the given bit to false.
    inline BitVector<N> &reset(bvlib::size_type_t position)
    {
        this->at(position) = false;
        return (*this);
    }

    /// @brief Flips every bit.
    inline BitVector<N> &flip()
    {
        for (bvlib::size_type_t it = 0; it < N; ++it)
            bits[it] = !bits[it];
        return (*this);
    }

    /// @brief Flips a given bit.
    inline BitVector<N> &flip(bvlib::size_type_t position)
    {
        this->at(position) = !this->at(position);
        return (*this);
    }

    /// @brief Returns the size of the bit-vector.
    inline auto size() const -> bvlib::size_type_t
    {
        return N;
    }

    /// @brief Returns the number of bits which are set.
    inline auto count() const -> bvlib::size_type_t
    {
        bvlib::size_type_t result = 0;
        for (bvlib::size_type_t it = 0; it < N; ++it)
            result += bits[it];
        return result;
    }

    /// @brief Tests whether all the bits are on.
    inline auto all() const -> bool
    {
        for (bvlib::size_type_t it = 0; it < N; ++it)
            if (!bits[it])
                return false;
        return true;
    }

    /// @brief Tests whether any of the bits are on.
    inline auto any() const -> bool
    {
        for (bvlib::size_type_t it = 0; it < N; ++it)
            if (bits[it])
                return true;
        return false;
    }

    /// @brief Tests whether none of the bits are on.
    inline auto none() const -> bool
    {
        for (bvlib::size_type_t it = 0; it < N; ++it)
            if (bits[it])
                return false;
        return true;
    }

    /// @brief Tests whether the sign bit is on.
    inline auto sign() const -> bool
    {
        return bits[0];
    }

    /// @brief Performs two-complement.
    inline BitVector<N> &two_complement()
    {
        return this->flip() += 1;
    }

    /// @brief Swaps the bits from 'start' to 'end'.
    inline BitVector<N> &swap(bvlib::size_type_t start = 0, bvlib::size_type_t end = N - 1)
    {
        do std::swap(bits[start], bits[end]);
        while (start++ < end--);
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template <bvlib::size_type_t N2>
    inline BitVector<N> &assign(BitVector<N2> const &rhs)
    {
        bvlib::size_type_t min_size = std::min(N, N2);
        this->reset();
        for (bvlib::size_type_t it = 0; it < min_size; ++it)
            this->at(it) = rhs[it];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector, iterating from left to right.
    template <bvlib::size_type_t N2>
    inline BitVector<N> &rassign(BitVector<N2> const &rhs)
    {
        bvlib::size_type_t min_size = std::min(N, N2);
        this->reset();
        for (bvlib::size_type_t it = 0; it < min_size; ++it)
            this->at(N - it - 1) = rhs[N2 - it - 1];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template <bvlib::size_type_t N2>
    inline BitVector<N> &operator=(const BitVector<N2> &rhs)
    {
        bvlib::size_type_t min_size = std::min(N, N2);
        this->reset();
        for (bvlib::size_type_t it = 0; it < min_size; ++it)
            this->at(it) = rhs[it];
        return (*this);
    }

    /// @brief Transforms rhs into a BitVector.
    inline BitVector<N> &operator=(bvlib::size_type_t rhs)
    {
        this->reset();
        for (bvlib::size_type_t it = 0; it < N; ++it) {
            this->at(it) = rhs % 2;
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
    inline bool &at(bvlib::size_type_t position)
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline const bool &at(bvlib::size_type_t position) const
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Returns a reference to the bit at the given position.
    inline bool &operator[](bvlib::size_type_t position)
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline const bool &operator[](bvlib::size_type_t position) const
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Transforms this BitVector to string.
    inline std::string to_string() const
    {
        std::string str;
        for (bvlib::size_type_t it = 0; it < N; ++it)
            str.push_back(bits[it] ? '1' : '0');
        return str;
    }

    /// @brief Transforms this BitVector to number.
    template <typename T = long>
    inline T to_number() const
    {
        T result = 0;
        for (long it = N - 1; it >= 0; --it)
            if (bits[it])
                result += std::pow(2, N - it - 1);
        return result;
    }
};

/// @brief Overrides the output stream operator.
template <long unsigned int N>
inline std::ostream &operator<<(std::ostream &lhs, bvlib::BitVector<N> const &rhs)
{
    lhs << rhs.to_number();
    return lhs;
}

} // namespace bvlib
