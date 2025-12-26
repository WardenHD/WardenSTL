// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_ALLOCATOR_HPP__
#define __WSTL_ALLOCATOR_HPP__

#include "private/Platform.hpp"
#include "Exception.hpp"
#include <stddef.h>


/// @defgroup allocator Allocator
/// @brief A set of simple memory allocation utilities and a common interface
/// @ingroup memory

namespace wstl {
    // Bad allocation exception

    /// @brief Exception thrown when memory allocation fails
    /// @ingroup allocator
    class BadAllocation : public Exception {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message The exception message, default is `Bad allocation`
        __WSTL_CONSTEXPR__ BadAllocation(StringType file, NumericType line, StringType message = "Bad allocation") : Exception(file, line, message) {}
        #else
        /// @brief Constructor
        /// @param message The exception message, default is `Bad allocation`
        __WSTL_CONSTEXPR__ BadAllocation(StringType message = "Bad allocation") : Exception(message) {}
        #endif

        /// @copydoc Exception::Name()
        virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "BadAllocation";
        }
    };

    /// @brief A common interface for all memory allocators
    /// @ingroup allocator
    class Allocator {
    public:
        /// @brief Allocates a block of memory of the specified size
        /// @param size The size of the memory to allocate
        /// @return A pointer to the allocated memory or null pointer if unsuccessful
        /// @throws `BadAllocation` if the allocation fails
        virtual void* Allocate(size_t size) = 0;

        /// @brief Frees a block of memory at the specified address
        /// @param address The address to free
        virtual void Free(void* address) = 0;
    };
}

#endif