// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s nullptr.h
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been modified by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_NULLPOINTER_HPP__
#define __WSTL_NULLPOINTER_HPP__

#include "private/Platform.hpp"
#ifdef __WSTL_CXX11__
#include <stddef.h>
#endif


namespace wstl {
    #ifndef __WSTL_CXX11__
    class nullptr_t {
    public:
        template<class T>
        inline operator T*() const { return 0; }

        template<class T, class U>
        inline operator T U::*() const { return 0; }

        inline bool operator==(nullptr_t) const { return true; }
        inline bool operator!=(nullptr_t) const { return false; }

    private:
        void operator&() const __WSTL_DELETE__;
    };

    #define __WSTL_NULLPTR__ nullptr_t();
    #else
    using nullptr_t = decltype(nullptr);
    #define __WSTL_NULLPTR__ nullptr;
    #endif
    
    static const nullptr_t NullPointer = __WSTL_NULLPTR__;
    /// @brief Type alias of `nullptr_t` for consistent naming
    typedef nullptr_t NullPointerType;
}

#endif