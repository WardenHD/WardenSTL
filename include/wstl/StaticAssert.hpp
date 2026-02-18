// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s static assert utilities,
// with modifications made for improved support under C++98.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_STATICASSERT_HPP__
#define __WSTL_STATICASSERT_HPP__

#include "private/Platform.hpp"


namespace wstl {
    #ifdef __WSTL_CXX11__
    #define WSTL_STATIC_ASSERT static_assert
    #else
    namespace __private {
        template<bool Condition>
        class __StaticAssert;

        template<>
        class __StaticAssert<true> {};    
    }
    
    #define WSTL_STATIC_ASSERT(condition, ...) \
        enum { __StaticAssertion = sizeof(__private::__StaticAssert<static_cast<bool>( (condition) )>) }
    #endif
}

#endif