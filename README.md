# C++ Record Accessor

A header-only C++ library that provides reflection-like capabilities for accessing and modifying class members by name at runtime using Boost.Describe.

## Features

- Get member values by name using `getMemberValueByName`
- Set member values by name using `setMemberValueByName`
- Type-safe operations with `std::any`
- Debug logging support

## Requirements

- C++17 or later
- CMake 3.25 or later
- Conan 2.0 or later
- GTest 1.8.1 (for testing, and C++11 compatible)

## Building

1. Install dependencies using Conan:
```bash
conan install .
```

2. Configure and build with CMake:
```bash
cmake --preset=conan-debug
cmake --build --preset=conan-debug
```

## Testing

To run the tests:
```bash
cd build/debug
ctest --output-on-failure
```