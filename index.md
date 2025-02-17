---
layout: default
---

# BitVector

[![Ubuntu](https://github.com/Galfurian/bitvector/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/bitvector/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/bitvector/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/bitvector/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/bitvector/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/bitvector/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/bitvector/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/bitvector/actions/workflows/documentation.yml)

A highly optimized and flexible library for working with bit vectors in C++.
This library provides efficient operations for bit-level manipulations,
including bitwise operations, arithmetic operations, and more.

## Table of Contents

1. Features
2. Installation
    1. Clone the repository
    2. Include in your project
3. Usage
    1. Creating a BitVector
    2. Accessing and Modifying Bits
    3. Performing Operations
    4. Two's Complement
4. Error Handling
5. Contributing
6. License

## 1. Features

- **Efficient bit operations**: Perform bit-level manipulations like `AND`, `OR`, `XOR`, and shifts efficiently.
- **Arithmetic operations**: Supports operations like addition, subtraction, multiplication, and division.
- **Flexible bit sizes**: Work with bit vectors of arbitrary sizes and perform operations on them seamlessly.
- **In-place operations**: Modify bit vectors directly with functions like `+=`, `-=`, and `*=` to minimize memory usage.
- **Two's complement**: Easily compute the two's complement of bit vectors.
- **Error handling**: Includes error handling for common issues like division by zero or overflow during arithmetic operations.

## 2. Installation

To install the BitVector library, you can clone this repository or directly
include the header files in your project.

### 2.1. Clone the repository

```bash
git clone https://github.com/Galfurian/bitvector.git
```

### 2.2. Include in your project

Simply include the `bitvector.hpp` file in your project:

```cpp
#include "bvlib/bitvector.hpp"
```

## 3. Usage

The `BitVector` class allows you to create bit vectors of arbitrary sizes and
perform a wide range of operations on them.

### 3.1. Creating a BitVector

To create a `BitVector`:

```cpp
#include "bvlib/bitvector.hpp"

int main() {
    // Create a 32-bit BitVector initialized with all bits set.
    auto bv32 = bvlib::BitVector<32>::ones();
    // Create a 64-bit BitVector initialized at zero.
    auto bv64 = bvlib::BitVector<64>::zeros();
}
```

### 3.2. Accessing and Modifying Bits

Use `at()` to access a specific bit and `set()` or `reset()` to modify bits:

```cpp
bv.set(0);        // Set bit 0 to 1
bv.reset(1);      // Set bit 1 to 0
bool bit = bv[0]; // Access bit 0 using operator[]
bv[1] = true;     // Modify bit 1 using operator[]
```

### 3.3. Performing Operations

You can perform arithmetic and bitwise operations directly on `BitVector` objects.

#### Bitwise Operations

```cpp
bvlib::BitVector<8> bv1("11001100");
bvlib::BitVector<8> bv2("10101010");

auto result = bv1 & bv2;  // Bitwise AND
result = bv1 | bv2;       // Bitwise OR
result = bv1 ^ bv2;       // Bitwise XOR
result = ~bv1;            // Bitwise NOT
```

#### Arithmetic Operations

```cpp
bvlib::BitVector<8> bv1("11001100");
bvlib::BitVector<8> bv2("10101010");

auto sum = bv1 + bv2;      // Add two bit vectors
auto diff = bv1 - bv2;     // Subtract two bit vectors
auto product = bv1 * bv2;  // Multiply two bit vectors
auto quotient = bv1 / bv2; // Divide two bit vectors

bv1 += bv2;   // Add and assign
bv1 -= bv2;   // Subtract and assign
bv1 *= bv2;   // Multiply and assign
```

### 3.4. Two's Complement

You can compute the two's complement of a `BitVector`:

```cpp
bvlib::BitVector<8> bv("11110000");
bvlib::detail::two_complement(bv);
```

## 4. Error Handling

The library includes error handling for common scenarios, such as division by zero.

```cpp
try {
    auto quotient = bv1 / bv2;  // May throw if bv2 is zero
} catch (const std::domain_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## 5. Contributing

We welcome contributions to the BitVector library! To contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-name`).
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`).
5. Push to the branch (`git push origin feature-name`).
6. Open a pull request.

## 6. License

Copyright (c) 2024-2025 Enrico Fraccaroli <enry.frak@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
