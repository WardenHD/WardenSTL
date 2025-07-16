// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_STANDARDEXCEPTIONS_HPP__
#define __WSTL_STANDARDEXCEPTIONS_HPP__

#include "private/Platform.hpp"
#include "Exception.hpp"


/// @defgroup standard_exceptions Standard Exceptions
/// @brief A set of standard exceptions that can be thrown by the library
/// @ingroup exceptions

namespace wstl {
    /// @brief Exception thrown when exceeding the length of a container
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/length_error
    class LengthError : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            __WSTL_CONSTEXPR__ LengthError(StringType file, NumericType line) : Exception("Length error", file, line) {}

            /// @brief Constructor with message
            /// @param message Message describing the exception
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            __WSTL_CONSTEXPR__ LengthError(StringType message, StringType file, NumericType line) : Exception(message, file, line) {}
            #else
            /// @brief Constructor
            __WSTL_CONSTEXPR__ LengthError() : Exception("Length error") {}

            /// @brief Constructor with message
            /// @param message Message describing the exception
            __WSTL_CONSTEXPR__ LengthError(StringType message) : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "LengthError";
            }
    };

    /// @brief Exception thrown when accessed element is out of range
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class OutOfRange : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            __WSTL_CONSTEXPR__ OutOfRange(StringType file, NumericType line) : Exception("Out of range", file, line) {}

            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            __WSTL_CONSTEXPR__ OutOfRange(StringType message, StringType file, NumericType line) : Exception(message, file, line) {}
            #else
            /// @brief Constructor
            __WSTL_CONSTEXPR__ OutOfRange() : Exception("Out of range") {}

            /// @brief Constructor with a message
            /// @param message Message describing the exception
            __WSTL_CONSTEXPR__ OutOfRange(StringType message) : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "OutOfRange";
            }
    };

    /// @brief Exception thrown when a bad cast is attempted
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class BadCast : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            __WSTL_CONSTEXPR__ BadCast(StringType file, NumericType line) : Exception("Bad cast", file, line) {}

            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            __WSTL_CONSTEXPR__ OutOfRange(StringType message, StringType file, NumericType line) : Exception(message, file, line) {}
            #else
            /// @brief Constructor
            __WSTL_CONSTEXPR__ BadCast() : Exception("Bad cast") {}

            /// @brief Constructor with a message
            /// @param message Message describing the exception
            __WSTL_CONSTEXPR__ BadCast(StringType message) : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "BadCast";
            }
    };
}

#endif