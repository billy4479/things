# C++ Template

This is a simple C++ template repository, using CMake as build system.

This repo expects you to use a C++20 capable compiler and `CMake >= 3.18`.

It features support for multiple compiler warnings and sanitizers
and object files caching through `ccache` (if available).

Build with:

```bash
cmake -S . -B build
cmake --build build
```
