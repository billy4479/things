#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using std::size_t;

// Platform detection adapted from:
// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/PlatformDetection.h
// See https://github.com/TheCherno/Hazel/blob/master/LICENSE

#define NO_UNKNOWN 1

#if NO_UNKNOWN
    #define NO_UNKNOWN_PLATFORM 1
    #define NO_UNKNOWN_COMPILER 1
#else
    // Edit these separately if needed
    #define NO_UNKNOWN_PLATFORM 0
    #define NO_UNKNOWN_COMPILER 0
#endif

#ifdef _WIN32
    /* Windows x64/x86 */
    #ifdef _WIN64
        /* Windows x64  */
        #define PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
     * so we must check all of them (in this order)
     * to ensure that we're running on MAC
     * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define PLATFORM_MACOS
    #else
        #error "Unknown Apple platform!"
    #endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
    #error "Android is not supported!"
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(SWIG)
    #define PLATFORM_SWIG
#else
    /* Unknown compiler/platform */
    #if NO_UNKNOWN_PLATFORM
        #error "Unknown platform!"
    #else
        #define PLATFORM_UNKNOWN
    #endif
#endif

// End of platform detection

#if defined(__clang__)
    #define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GCC
#elif defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(SWIG)
#else
    #if NO_UNKNOWN_COMPILER
        #error "Unsupported compiler"
    #else
        #define COMPILER_UNKNOWN
    #endif
#endif

#ifndef NDEBUG
    #define DEBUG 1
#endif

#define STRINGIFY(x) #x

#define BIT(x) (1 << x)

#define NON_COPYABLE(className)                                                \
    className(const className &) = delete;                                     \
    className &operator=(const className &) = delete;

#define NON_MOVABLE(className)                                                 \
    className(className &&) = delete;                                          \
    className &operator=(className &&) = delete;

#define NON_COPYABLE_NON_MOVABLE(className)                                    \
    NON_COPYABLE(className)                                                    \
    NON_MOVABLE(className)

#define DEFAULT_COPY(className)                                                \
    className(const className &) = default;                                    \
    className &operator=(const className &) = default;

#define DEFAULT_MOVE(className)                                                \
    className(className &&) = default;                                         \
    className &operator=(className &&) = default;

#define DEFAULT_COPY_AND_MOVE(className)                                       \
    DEFAULT_COPY(className)                                                    \
    DEFAULT_MOVE(className)

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#ifdef PLATFORM_WINDOWS
    #define FUNC_SIG __FUNCSIG__
#else
    #define FUNC_SIG __PRETTY_FUNCTION__
#endif

[[noreturn]] inline void VERIFY_NOT_REACHED() {

    #ifndef COMPILER_MSVC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstring-conversion"
    #endif
    assert(false || "Unreachable");
#ifndef COMPILER_MSVC
    #pragma GCC diagnostic pop
    #endif
    abort();
}