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

        /// @brief Constructor
        /// @param message The exception message
        /// @param file File (usually __FILE__)
        /// @param line Line number (usually __LINE__)
        __WSTL_CONSTEXPR__ Exception(StringType message, StringType file, NumericType line)
            : m_Message(message), m_Line(line), m_Filename(file) {}

        /// @brief Getter for the exception message
        __WSTL_CONSTEXPR__ StringType What() const {
            return m_Message;
        }

        /// @brief Gets the file for the exception
        __WSTL_CONSTEXPR__ StringType Filename() const {
            return m_Filename;
        }

        /// @brief Gets the line number for the exception
        __WSTL_CONSTEXPR__ NumericType Line() const {
            return m_Line;
        }

    private:
        StringType m_Message;
        NumericType m_Line;
        StringType m_Filename;
    };
}

#endif