// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_PLATFORM_HPP__
#define __WSTL_PLATFORM_HPP__


// Doxygen groups and namespaces

/// @defgroup wstl WardenSTL
/// @brief Main WardenSTL library group

/// @defgroup utilities Utilities
/// @brief A set of utility templates
/// @ingroup wstl

/// @defgroup containers Containers
/// @brief A set of container templates
/// @ingroup wstl

/// @defgroup maths Maths
/// @brief A set of mathematical templates
/// @ingroup wstl

/// @defgroup hash Hash
/// @brief A set of hash templates
/// @ingroup maths

/// @defgroup crc CRC
/// @brief A set of CRC calculation templates
/// @ingroup maths

/// @defgroup binary Binary
/// @brief A set of binary templates
/// @ingroup wstl

/// @defgroup string String
/// @brief A set of string templates
/// @ingroup containers

/// @brief Main namespace of WardenSTL library
/// @ingroup wstl
namespace wstl {
    /// @brief Namespace with private functions. DO NOT use it directly!
    /// @ingroup wstl
    namespace __private {
        /// @brief Namespace with private compile-time equivalents of functions functions. 
        /// DO NOT use it directly!
        /// @ingroup wstl
        namespace __compile {}
    }

    /// @brief Namespace that holds compile-time equivalents of functions
    /// @ingroup wstl
    namespace compile {}

    /// @brief Namespace that holds hash functions
    /// @ingroup hash
    namespace hash {}

    /// @brief Namespace that holds string-related functions and constants
    /// @ingroup string
    namespace string {}

    // /// @brief Namespace that holds variations of CRC functions
    // /// @ingroup crc
    // namespace crc {}
}

// C++ version defines

#if __cplusplus >= 201103L
    #define __WSTL_CXX11__
#endif

#if __cplusplus >= 201402L
    #define __WSTL_CXX14__
#endif

#if __cplusplus >= 201703L
    #define __WSTL_CXX17__
#endif

#if __cplusplus >= 202002L
    #define __WSTL_CXX20__
#endif

#if __cplusplus >= 202302L
    #define __WSTL_CXX23__
#endif  

// Compiler defines (ones that are supported)

#if defined(_MSC_VER)
#define __WSTL_MSVC__

#elif defined(__GNUC__) || defined(__GNUG__)
#define __WSTL_GCC__

#elif defined(__clang__)
#define __WSTL_CLANG__

#elif defined(__INTEL_COMPILER)
#define __WSTL_ICC__
#endif

// C++11 features

#if defined(__WSTL_CXX11__)
    #define __WSTL_CONSTEXPR__ constexpr
    #define __WSTL_CONSTEXPR11__ constexpr
    #define __WSTL_FINAL__ final
    #define __WSTL_DELETE__ = delete
    #define __WSTL_DEFAULT__ = default
    #define __WSTL_OVERRIDE__ override
    #define __WSTL_MOVE__(x) wstl::Move(x)
    #define __WSTL_ENUM_CLASS__(name) enum class name
    #define __WSTL_ENUM_CLASS_TYPE__(name, type) enum class name : type
    #define __WSTL_NORETURN__ [[noreturn]]

    #if defined(__WSTL_EXCEPTIONS__)
        #define __WSTL_NOEXCEPT__ noexcept
        #define __WSTL_NOEXCEPT_EXPR__(...) noexcept(__VA_ARGS__)
    #else
        #define __WSTL_NOEXCEPT__
        #define __WSTL_NOEXCEPT_EXPR__(...)
    #endif
#else
    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
        #define __WSTL_NORETURN__ __attribute__((noreturn))
    #elif defined(__WSTL_MSVC__)
        #define __WSTL_NORETURN__ __declspec(noreturn)
    #else
        #define __WSTL_NORETURN__
    #endif

    #if defined(__WSTL_EXCEPTIONS__)
        #define __WSTL_NOEXCEPT__ throw()
    #else
        #define __WSTL_NOEXCEPT__
    #endif

    #define __WSTL_CONSTEXPR__
    #define __WSTL_CONSTEXPR11__
    #define __WSTL_FINAL__
    #define __WSTL_DELETE__
    #define __WSTL_DEFAULT__
    #define __WSTL_OVERRIDE__
    #define __WSTL_NOEXCEPT_EXPR__(...)
    #define __WSTL_MOVE__(x) x
    #define __WSTL_ENUM_CLASS__(name) enum name
    #define __WSTL_ENUM_CLASS_TYPE__(name, type) enum name
#endif

// C++14 features

#if defined(__WSTL_CXX14__)
    #define __WSTL_CONSTEXPR14__ constexpr
    #define __WSTL_DEPRECATED__ [[deprecated]]
    #define __WSTL_DEPRECATED_REASON__(reason) [[deprecated(reason)]]
#else
    #define __WSTL_CONSTEXPR14__

    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
        #define __WSTL_DEPRECATED__ __attribute__((deprecated))
        #define __WSTL_DEPRECATED_REASON__(reason) __attribute__((deprecated(reason)))
    #elif defined(__WSTL_MSVC__)
        #define __WSTL_DEPRECATED__ __declspec(deprecated)
        #define __WSTL_DEPRECATED_REASON__(reason) __declspec(deprecated(reason))
    #else
        #define __WSTL_DEPRECATED__
        #define __WSTL_DEPRECATED_REASON__(reason)
    #endif
#endif

// C++17 features

#if defined(__WSTL_CXX17__)
    #define __WSTL_CONSTEXPR17__ constexpr
    #define __WSTL_IF_CONSTEXPR__ constexpr
    #define __WSTL_NODISCARD__ [[nodiscard]]
    #define __WSTL_FALLTHROUGH__ [[fallthrough]]
    #define __WSTL_MAYBE_UNUSED__ [[maybe_unused]]
    #define __WSTL_INLINE_VARIABLE__ inline
#else
    #define __WSTL_CONSTEXPR17__
    #define __WSTL_IF_CONSTEXPR__
    #define __WSTL_INLINE_VARIABLE__

    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
        #define __WSTL_MAYBE_UNUSED__ __attribute__((unused))
        #define __WSTL_NODISCARD__ __attribute__((warn_unused_result))
        #define __WSTL_FALLTHROUGH__ __attribute__((fallthrough))
    #elif defined(__WSTL_MSVC__)
        #define __WSTL_UNUSED__ __pragma(warning(suppress: 4100))
        #define __WSTL_NODISCARD__ __declspec(warn_unused_result)
        #define __WSTL_FALLTHROUGH__ /* fallthrough */
    #else
        #define __WSTL_UNUSED__
        #define __WSTL_NODISCARD__
        #define __WSTL_FALLTHROUGH__
    #endif
#endif

// C++20 features

#if defined(__WSTL_CXX20__)
    #define __WSTL_CONSTEXPR20__ constexpr
    #define __WSTL_LIKELY__ [[likely]]
    #define __WSTL_UNLIKELY__ [[unlikely]]
    #define __WSTL_CONSTEVAL__ consteval
    #define __WSTL_CONSTINIT__ constinit
    #define __WSTL_NO_UNIQUE_ADDRESS__ [[no_unique_address]]
    #define __WSTL_EXPLICIT_EXPR__(...) explicit(__VA_ARGS__)
#else
    #define __WSTL_CONSTEXPR20__
    #define __WSTL_LIKELY__
    #define __WSTL_UNLIKELY__
    #define __WSTL_CONSTEVAL__
    #define __WSTL_CONSTINIT__
    #define __WSTL_NO_UNIQUE_ADDRESS__
    #define __WSTL_EXPLICIT_EXPR__(...) explicit
#endif

#endif