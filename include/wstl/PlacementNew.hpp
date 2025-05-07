#ifndef __WSTL_PLACEMENTNEW_HPP__
#define __WSTL_PLACEMENTNEW_HPP__

#include "private/Platform.hpp"
#include <stddef.h>


// Defines the placement new operators if they are not already defined

#ifndef __WSTL_CUSTOM_NEW__
inline void* operator new(size_t, void* p) __WSTL_NOEXCEPT__ { return p; }
inline void* operator new[](size_t, void* p) __WSTL_NOEXCEPT__ { return p; }

inline void operator delete(void*, void*) __WSTL_NOEXCEPT__ {}
inline void operator delete[](void*, void*) __WSTL_NOEXCEPT__ {}
#endif

#endif