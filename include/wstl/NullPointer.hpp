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
    class NullPointerType {
    public:
        template<class T>
        inline operator T*() const { return 0; }

        template<class T, class U>
        inline operator T U::*() const { return 0; }

        inline bool operator==(NullPointerType) const { return true; }
        inline bool operator!=(NullPointerType) const { return false; }

    private:
        void operator&() const __WSTL_DELETE__;
    };

    #define __WSTL_NULLPTR__ NullPointerType()
    #else
    using NullPointerType = decltype(nullptr);
    #define __WSTL_NULLPTR__ nullptr
    #endif
    
    static const __WSTL_CONSTEXPR__ NullPointerType NullPointer = __WSTL_NULLPTR__;
    
    /// @brief Type alias for STL naming support
    typedef NullPointerType nullptr_t;
}

#endif