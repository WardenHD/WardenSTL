// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_EXCEPTION_HPP__
#define __WSTL_EXCEPTION_HPP__

#include "private/Platform.hpp"


/// @defgroup exception Exception
/// @brief A base class for all WSTL exceptions
/// @ingroup utilities

namespace wstl {
    /// @brief A base class for all exceptions
    /// @ingroup exception
    /// @see https://en.cppreference.com/w/cpp/error/exception
    class Exception {
    public:
        typedef const char* StringType;
        typedef int NumericType;

        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File (usually __FILE__)
        /// @param line Line number (usually __LINE__)
        /// @param message The exception message
        /// @note This is only available if __WSTL_EXCEPTION_LOCATION__ is defined
        __WSTL_CONSTEXPR__ Exception(StringType file, NumericType line, StringType message)
            : m_Message(message), m_Line(line), m_Filename(file) {}

        #else 
        /// @brief Constructor
        /// @param message The exception message
        __WSTL_CONSTEXPR__ Exception(StringType message) : m_Message(message) {}
        #endif

        /// @brief Virtual destructor
        virtual ~Exception() __WSTL_NOEXCEPT__ {}

        /// @brief Returns the name of the exception
        virtual StringType Name() const __WSTL_NOEXCEPT__ = 0;

        /// @brief Gets the exception message
        __WSTL_CONSTEXPR__ StringType What() const {
            return m_Message;
        }

        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Gets the file for the exception
        /// @note This is only available if __WSTL_EXCEPTION_LOCATION__ is defined
        __WSTL_CONSTEXPR__ StringType Filename() const {
            return m_Filename;
        }

        /// @brief Gets the line number for the exception
        /// @note This is only available if __WSTL_EXCEPTION_LOCATION__ is defined
        __WSTL_CONSTEXPR__ NumericType Line() const {
            return m_Line;
        }
        #endif

    private:
        StringType m_Message;

        #ifdef __WSTL_EXCEPTION_LOCATION__
        NumericType m_Line;
        StringType m_Filename;
        #endif
    };


    #ifdef __WSTL_EXCEPTION_LOCATION__
    #define __WSTL_MAKE_EXCEPTION_1(exception) exception(__FILE__, __LINE__)
    #define __WSTL_MAKE_EXCEPTION_2(exception, message) exception(__FILE__, __LINE__, message)
    #else
    #define __WSTL_MAKE_EXCEPTION_1(exception) exception()
    #define __WSTL_MAKE_EXCEPTION_2(exception, message) exception(message)
    #endif

    #define __WSTL_EXPAND__(x) x

    #define __WSTL_COUNT_ARGS__(...) __WSTL_EXPAND__(__WSTL_COUNT_ARGS_IMPL__(__VA_ARGS__, 3,2,1,0))
    #define __WSTL_COUNT_ARGS_IMPL__(_1,_2,_3,N,...) N
    
    #define __WSTL_MAKE_EXCEPTION_IMPL__(count, ...) __WSTL_MAKE_EXCEPTION_##count(__VA_ARGS__)
    #define __WSTL_MAKE_EXCEPTION_IMPL2__(count, ...) __WSTL_MAKE_EXCEPTION_IMPL__(count, __VA_ARGS__)

    /// @brief Macro for making exception constructor with arguments
    /// @param exception Exception class name
    /// @param message (optional) Exception message
    /// @ingroup exception
    #define WSTL_MAKE_EXCEPTION(...) __WSTL_MAKE_EXCEPTION_IMPL2__(__WSTL_COUNT_ARGS__(__VA_ARGS__), __VA_ARGS__) 
}

#endif