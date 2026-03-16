// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_DIAGNOSTIC_HPP__
#define __WSTL_DIAGNOSTIC_HPP__

#include "private/Platform.hpp"


#ifdef __WSTL_CLANG__
    #define WSTL_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
    #define WSTL_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
    #define WSTL_DIAGNOSTIC_IGNORE(w) _Pragma(WSTL_STRINGIFY(clang diagnostic ignored w))
#elif defined(__WSTL_GCC__)
    #define WSTL_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
    #define WSTL_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
    #define WSTL_DIAGNOSTIC_IGNORE(w) _Pragma(WSTL_STRINGIFY(GCC diagnostic ignored w))
#elif defined(__WSTL_MSVC__) || defined(__WSTL_ICC__)
    #define WSTL_DIAGNOSTIC_PUSH _Pragma(warning(push))
    #define WSTL_DIAGNOSTIC_POP _Pragma(warning(pop))
    #define WSTL_DIAGNOSTIC_IGNORE(w) _Pragma(warning(disable: w))
#else
    #define WSTL_DIAGNOSTIC_PUSH
    #define WSTL_DIAGNOSTIC_POP
    #define WSTL_DIAGNOSTIC_IGNORE(w)
#endif

#endif