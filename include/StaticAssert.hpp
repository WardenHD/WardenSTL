#ifndef __WSTL_STATICASSERT_HPP__
#define __WSTL_STATICASSERT_HPP__

#include "private/Platform.hpp"


namespace wstl {
    #ifdef __WSTL_CXX11__
    #define StaticAssert static_assert
    #else
    namespace __private {
        template<bool Condition>
        class __StaticAssert;

        template<>
        class __StaticAssert<true> {};    
    }
    
    #define StaticAssert(condition, ...) \
        enum { __StaticAssertion = sizeof(__private::__StaticAssert<static_cast<bool>( (condition) )>) }
    #endif
}

#endif