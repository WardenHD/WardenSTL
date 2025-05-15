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
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            LengthError(StringType message, StringType file, NumericType line) : Exception(message, file, line) {}
    };

    /// @brief Exception thrown when accessed element is out of range
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class OutOfRange : public Exception {
        public:
            /// @brief Constructor
            /// @param file File where the exception occurred
            /// @param line Line number where the exception occurred
            OutOfRange(StringType message, StringType file, NumericType line) : Exception(message, file, line) {}
    };
}

#endif