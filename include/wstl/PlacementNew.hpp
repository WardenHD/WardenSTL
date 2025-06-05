// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_PLACEMENTNEW_HPP__
#define __WSTL_PLACEMENTNEW_HPP__

#include "private/Platform.hpp"
#include <stddef.h>


inline void* operator new(size_t, void* p) __WSTL_NOEXCEPT__ { return p; }
inline void* operator new[](size_t, void* p) __WSTL_NOEXCEPT__ { return p; }

inline void operator delete(void*, void*) __WSTL_NOEXCEPT__ {}
inline void operator delete[](void*, void*) __WSTL_NOEXCEPT__ {}

#endif