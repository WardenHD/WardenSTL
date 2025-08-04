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
    class LengthError __WSTL_FINAL__ : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            /// @param message Message describing the exception, default is `Length error`
            __WSTL_CONSTEXPR__ LengthError(StringType file, NumericType line, StringType message = "Length error") : Exception(file, line, message) {}
            #else
            /// @brief Constructor with message
            /// @param message Message describing the exception, default is `Length error`
            __WSTL_CONSTEXPR__ LengthError(StringType message = "Length error") : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "LengthError";
            }
    };

    /// @brief Exception thrown when accessed element is out of range
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class OutOfRange __WSTL_FINAL__ : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            /// @param message Message describing the exception, default is `Out of range`
            __WSTL_CONSTEXPR__ OutOfRange(StringType file, NumericType line, StringType message = "Out of range") : Exception(file, line, message) {}
            #else
            /// @brief Constructor
            /// @param message Message describing the exception, default is `Out of range`
            __WSTL_CONSTEXPR__ OutOfRange(StringType message = "Out of range") : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "OutOfRange";
            }
    };

    /// @brief Exception thrown when a bad cast is attempted
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class BadCast __WSTL_FINAL__ : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            /// @param message Message describing the exception, default is `Bad cast`
            __WSTL_CONSTEXPR__ BadCast(StringType file, NumericType line, StringType message = "Bad cast") : Exception(file, line, message) {}
            #else
            /// @brief Constructor with a message
            /// @param message Message describing the exception, default is `Bad cast`
            __WSTL_CONSTEXPR__ BadCast(StringType message = "Bad cast") : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "BadCast";
            }
    };

    /// @brief Exception thrown when a logic error occurs
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/logic_error
    class LogicError __WSTL_FINAL__ : public Exception {
        public:
            #ifdef __WSTL_EXCEPTION_LOCATION__
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            /// @param message Message describing the exception, default is `Logic error`
            __WSTL_CONSTEXPR__ LogicError(StringType file, NumericType line, StringType message = "Logic error") : Exception(file, line, message) {}
            #else
            /// @brief Constructor
            /// @param message Message describing the exception, default is `Logic error`
            __WSTL_CONSTEXPR__ LogicError(StringType message = "Logic error") : Exception(message) {}
            #endif

            /// @copydoc Exception::Name()
            __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
                return "LogicError";
            }
    };
}

#endif