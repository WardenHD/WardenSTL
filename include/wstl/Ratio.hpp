// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s ratio utilities,
// with modifications made for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_RATIO_HPP__
#define __WSTL_RATIO_HPP__

#include "private/Platform.hpp"
#include "Numeric.hpp"
#include <stdint.h>


/// @defgroup ratio Ratio
/// @brief A set of templates for working with ratios
/// @ingroup maths

namespace wstl {
    /// @brief A class that represents a rational fraction of two numbers
    /// @tparam NUMERATOR Numerator of the ratio
    /// @tparam DENOMINATOR Denominator of the ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio
    template<intmax_t NUMERATOR, intmax_t DENOMINATOR = 1>
    struct Ratio {
        static const __WSTL_CONSTEXPR__ intmax_t Numerator = NUMERATOR;
        static const __WSTL_CONSTEXPR__ intmax_t Denominator = DENOMINATOR;

        typedef Ratio<NUMERATOR, DENOMINATOR> Type;
    };

    template<intmax_t NUMERATOR, intmax_t DENOMINATOR>
    const __WSTL_CONSTEXPR__ intmax_t Ratio<NUMERATOR, DENOMINATOR>::Numerator;

    template<intmax_t NUMERATOR, intmax_t DENOMINATOR>
    const __WSTL_CONSTEXPR__ intmax_t Ratio<NUMERATOR, DENOMINATOR>::Denominator;
    
    #if INT_MAX > INT32_MAX 
    typedef Ratio<1, 1000000000000000000000000> Yocto;
    typedef Ratio<1, 1000000000000000000000> Zepto;
    typedef Ratio<1, 1000000000000000000> Atto;
    typedef Ratio<1, 1000000000000000> Femto;
    typedef Ratio<1, 1000000000000> Pico;
    #endif

    #if (INT_MAX >= INT32_MAX)
    typedef Ratio<1, 1000000000> Nano;
    typedef Ratio<1, 1000000> Micro;
    #endif

    #if (INT_MAX >= INT16_MAX)
    typedef Ratio<1, 1000> Milli;
    typedef Ratio<1, 100> Centi;
    typedef Ratio<1, 10> Deci;
    typedef Ratio<10, 1> Deca;
    typedef Ratio<100, 1> Hecto;
    typedef Ratio<1000, 1> Kilo;
    #endif

    #if (INT_MAX >= INT32_MAX)
    typedef Ratio<1000000, 1> Mega;
    typedef Ratio<1000000000, 1> Giga;
    #endif

    #if INT_MAX > INT32_MAX
    typedef Ratio<1000000000000, 1> Tera;
    typedef Ratio<1000000000000000, 1> Peta
    typedef Ratio<1000000000000000000, 1> Exa;
    typedef Ratio<1000000000000000000000, 1> Zetta;
    typedef Ratio<1000000000000000000000000, 1> Yotta;
    #endif

    /// @brief An approximation of PI to 6 digits
    typedef Ratio<355, 113> RatioPi;

    /// @brief An approximation of root of 2
    typedef Ratio<239, 169> RatioRoot2;

    /// @brief An approximation of e
    typedef Ratio<326, 120> RatioE;

    namespace __private {
        template<typename R>
        class __RatioSimplify {
        private:
            static const __WSTL_CONSTEXPR__ intmax_t GCD = compile::GCD<intmax_t, R::Numerator, R::Denominator>::Value;
        
        public:
            typedef Ratio<R::Numerator / GCD, R::Denominator / GCD> Type;
        };

        template<typename R1, typename R2>
        class __RatioAdd {
        private:
            static const __WSTL_CONSTEXPR__ intmax_t LCM = compile::LCM<intmax_t, R1::Denominator, R2::Denominator>::Value;

        public:
            typedef typename __RatioSimplify<Ratio<R1::Numerator * LCM / R1::Denominator + R2::Numerator * LCM / R2::Denominator, LCM>>::Type Type;
        };

        template<typename R1, typename R2>
        struct __RatioSubtract {
            typedef typename __RatioAdd<R1, Ratio<-R2::Numerator, R2::Denominator>>::Type Type;
        };

        template<typename R1, typename R2>
        class __RatioMultiply {
        private:
            static const __WSTL_CONSTEXPR__ intmax_t GCD1 = compile::GCD<intmax_t, R1::Numerator, R2::Denominator>::Value;
            static const __WSTL_CONSTEXPR__ intmax_t GCD2 = compile::GCD<intmax_t, R1::Denominator, R2::Numerator>::Value;

        public:
            typedef Ratio<(R1::Numerator / GCD1) * (R2::Numerator / GCD2), (R1::Denominator / GCD2) * (R2::Denominator / GCD1)> Type;
        };

        template<typename R1, typename R2>
        struct __RatioDivide {
            typedef typename __RatioMultiply<R1, Ratio<R2::Denominator, R2::Numerator>>::Type Type;
        };
    }

    #ifdef __WSTL_CXX11__
    /// @brief Adds two ratios together
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_add
    template<typename R1, typename R2>
    using RatioAdd = typename __private::__RatioAdd<R1, R2>::Type;

    /// @brief Subtracts two ratios
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_subtract
    template<typename R1, typename R2>
    using RatioSubtract = typename __private::__RatioSubtract<R1, R2>::Type;

    /// @brief Multiplies two ratios
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_multiply
    template<typename R1, typename R2>
    using RatioMultiply = typename __private::__RatioMultiply<R1, R2>::Type;

    /// @brief Divides two ratios
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_divide
    template<typename R1, typename R2>
    using RatioDivide = typename __private::__RatioDivide<R1, R2>::Type;

    #else
    /// @brief Adds two ratios together
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_add
    template<typename R1, typename R2>
    struct RatioAdd : __private::__RatioAdd<R1, R2> {};

    /// @brief Subtracts two ratios
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_subtract
    template<typename R1, typename R2>
    struct RatioSubtract : __private::__RatioSubtract<R1, R2> {};

    /// @brief Multiplies two ratios
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_multiply
    template<typename R1, typename R2>
    struct RatioMultiply : __private::__RatioMultiply<R1, R2> {};

    /// @brief Divides two ratios
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_divide
    template<typename R1, typename R2>
    struct RatioDivide : __private::__RatioDivide<R1, R2> {};
    #endif

    /// @brief Checks whether two ratios are equal
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_equal
    template<typename R1, typename R2>
    struct RatioEqual : BoolConstant<(R1::Numerator == R2::Numerator && R1::Denominator == R2::Denominator)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc RatioEqual
    /// @since C++17
    template<typename R1, typename R2>
    inline constexpr bool RatioEqualVariable = RatioEqual<R1, R2>::Value;
    #endif

    /// @brief Checks whether two ratios are not equal
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_not_equal
    template<typename R1, typename R2>
    struct RatioNotEqual : BoolConstant<(R1::Numerator != R2::Numerator || R1::Denominator != R2::Denominator)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc RatioNotEqual
    /// @since C++17
    template<typename R1, typename R2>
    inline constexpr bool RatioNotEqualVariable = RatioNotEqual<R1, R2>::Value;
    #endif

    /// @brief Checks whether first ratio is less than second
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_less
    template<typename R1, typename R2>
    struct RatioLess : BoolConstant<(R1::Numerator * R2::Denominator < R1::Denominator * R2::Numerator)> {};
    
    #ifdef __WSTL_CXX17__
    /// @copydoc RatioLess
    /// @since C++17
    template<typename R1, typename R2>
    inline constexpr bool RatioLessVariable = RatioLess<R1, R2>::Value;
    #endif

    /// @brief Checks whether first ratio is less than or equal to second
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_less_equal
    template<typename R1, typename R2>
    struct RatioLessEqual : BoolConstant<(R1::Numerator * R2::Denominator <= R1::Denominator * R2::Numerator)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc RatioLessEqual
    /// @since C++17
    template<typename R1, typename R2>
    inline constexpr bool RatioLessEqualVariable = RatioLessEqual<R1, R2>::Value;
    #endif

    /// @brief Checks whether first ratio is greater than second
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_greater
    template<typename R1, typename R2>
    struct RatioGreater : BoolConstant<(R1::Numerator * R2::Denominator > R1::Denominator * R2::Numerator)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc RatioGreater
    /// @since C++17
    template<typename R1, typename R2>
    inline constexpr bool RatioGreaterVariable = RatioGreater<R1, R2>::Value;
    #endif

    /// @brief Checks whether first ratio is greater than or equal to second
    /// @tparam R1 First ratio
    /// @tparam R2 Second ratio
    /// @ingroup ratio
    /// @see https://en.cppreference.com/w/cpp/numeric/ratio/ratio_greater_equal
    template<typename R1, typename R2>
    struct RatioGreaterEqual : BoolConstant<(R1::Numerator * R2::Denominator >= R1::Denominator * R2::Numerator)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc RatioGreaterEqual
    /// @since C++17
    template<typename R1, typename R2>
    inline constexpr bool RatioGreaterEqualVariable = RatioGreaterEqual<R1, R2>::Value;
    #endif
}

#endif