/// @file bitvector.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Base bitvector class.

#pragma once

#include <stdexcept>
#include <string>
#include <cmath>

namespace bvlib
{

/// @brief Class which reproduces the behaviour of bit-vector.
/// @tparam N Lenght of the bit-vector
template <std::size_t N>
class BitVector {
public:
    /// Internal vector of bits.
    bool bits[N];

    /// @brief Construct a new bitvector.
    explicit BitVector()
        : bits()
    {
        for (std::size_t it = 0; it < N; ++it)
            bits[it] = false;
    }

    /// @brief Construct a new bitvector and intializes it with the given value.
    /// @param value the initial value.
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
    explicit BitVector(T value)
        : bits()
    {
        for (std::size_t it = 0; it < N; ++it) {
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
    template <std::size_t N2>
    explicit BitVector(BitVector<N2> const &other)
        : bits()
    {
        std::size_t min_size = std::min(N, N2);
        for (std::size_t it = 0; it < min_size; ++it)
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
        for (std::size_t it = 0; it < N; ++it)
            bits[it] = false;
        return (*this);
    }

    /// @brief Sets the given bit to false.
    inline BitVector<N> &reset(std::size_t position)
    {
        this->at(position) = false;
        return (*this);
    }

    /// @brief Flips every bit.
    inline BitVector<N> &flip()
    {
        for (std::size_t it = 0; it < N; ++it)
            bits[it] = !bits[it];
        return (*this);
    }

    /// @brief Flips a given bit.
    inline BitVector<N> &flip(std::size_t position)
    {
        this->at(position) = !this->at(position);
        return (*this);
    }

    /// @brief Returns the size of the bit-vector.
    inline auto size() const -> std::size_t
    {
        return N;
    }

    /// @brief Returns the number of bits which are set.
    inline auto count() const -> std::size_t
    {
        std::size_t result = 0;
        for (std::size_t it = 0; it < N; ++it)
            result += bits[it];
        return result;
    }

    /// @brief Tests whether all the bits are on.
    inline auto all() const -> bool
    {
        for (std::size_t it = 0; it < N; ++it)
            if (!bits[it])
                return false;
        return true;
    }

    /// @brief Tests whether any of the bits are on.
    inline auto any() const -> bool
    {
        for (std::size_t it = 0; it < N; ++it)
            if (bits[it])
                return true;
        return false;
    }

    /// @brief Tests whether none of the bits are on.
    inline auto none() const -> bool
    {
        for (std::size_t it = 0; it < N; ++it)
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
    inline BitVector<N> &swap(std::size_t start = 0, std::size_t end = N - 1)
    {
        do std::swap(bits[start], bits[end]);
        while (start++ < end--);
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template <std::size_t N2>
    inline BitVector<N> &assign(BitVector<N2> const &rhs)
    {
        std::size_t min_size = std::min(N, N2);
        this->reset();
        for (std::size_t it = 0; it < min_size; ++it)
            this->at(it) = rhs[it];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector, iterating from left to right.
    template <std::size_t N2>
    inline BitVector<N> &rassign(BitVector<N2> const &rhs)
    {
        std::size_t min_size = std::min(N, N2);
        this->reset();
        for (std::size_t it = 0; it < min_size; ++it)
            this->at(N - it - 1) = rhs[N2 - it - 1];
        return (*this);
    }

    /// @brief Copies rhs into this BitVector.
    template <std::size_t N2>
    inline BitVector<N> &operator=(const BitVector<N2> &rhs)
    {
        std::size_t min_size = std::min(N, N2);
        this->reset();
        for (std::size_t it = 0; it < min_size; ++it)
            this->at(it) = rhs[it];
        return (*this);
    }

    /// @brief Transforms rhs into a BitVector.
    inline BitVector<N> &operator=(std::size_t rhs)
    {
        this->reset();
        for (std::size_t it = 0; it < N; ++it) {
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
    inline bool &at(std::size_t position)
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline const bool &at(std::size_t position) const
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Returns a reference to the bit at the given position.
    inline bool &operator[](std::size_t position)
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Returns the bit at the given position.
    inline const bool &operator[](std::size_t position) const
    {
        if (position >= N)
            throw std::out_of_range("accessing values outside bitvector");
        return bits[N - position - 1];
    }

    /// @brief Transforms this BitVector to string.
    inline std::string to_string() const
    {
        std::string str;
        for (std::size_t it = 0; it < N; ++it)
            str.push_back(bits[it] ? '1' : '0');
        return str;
    }

    /// @brief Transforms this BitVector to number.
    template <typename T>
    constexpr inline T to_number() const
    {
        T result = 0;
        for (std::size_t it = 0; it < N; ++it) {
            if (bits[it]) {
                result += static_cast<T>(std::pow(2, N - it - 1));
            }
        }
        return result;
    }
};

} // namespace bvlib
