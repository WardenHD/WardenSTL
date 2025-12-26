// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s limits utilities,
// and some concepts and functions have been adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_LIMITS_HPP__
#define __WSTL_LIMITS_HPP__

#include "private/Platform.hpp"
#include "TypeTraits.hpp"

#ifndef __USE_C99_MATH
#define __USE_C99_MATH
#endif

#include <limits.h>
#include <float.h>
#include <stdint.h>


#ifdef __WSTL_MATH_SUPPORT__
#include <math.h>
#endif

#define __WSTL_LOG10_2__(x) (((x) * 301) / 1000)

// These macros may not be defiend 

#if !defined(LLONG_MIN) && defined(__WSTL_CXX11__)
    #define LLONG_MAX  __LONG_LONG_MAX__
    #define LLONG_MIN  (-__LONG_LONG_MAX__ - 1LL)
    #define ULLONG_MAX (__LONG_LONG_MAX__ * 2ULL + 1ULL)
#endif

#ifndef WCHAR_MIN
    #define WCHAR_MIN __WCHAR_MIN__
    #define WCHAR_MAX __WCHAR_MAX__
#endif

#if !defined(LDBL_MIN) && defined(DBL_MIN)
    #define LDBL_MIN DBL_MIN
    #define LDBL_MAX DBL_MAX
    #define LDBL_EPSILON DBL_EPSILON
    #define LDBL_MANT_DIG DBL_MANT_DIG
    #define LDBL_DIG DBL_DIG
    #define LDBL_MIN_EXP DBL_MIN_EXP
    #define LDBL_MIN_10_EXP DBL_MIN_10_EXP
    #define LDBL_MAX_EXP DBL_MAX_EXP
    #define LDBL_MAX_10_EXP DBL_MAX_10_EXP
#endif

#ifndef HUGE_VAL
    #define HUGE_VALF FLT_MAX
    #define HUGE_VAL DBL_MAX
    #define HUGE_VALL LDBL_MAX
#endif

#ifdef NAN
    #define __WSTL_NANF__ NAN
    #define __WSTL_NAN__ static_cast<double>(NAN)
    #define __WSTL_NANL__ static_cast<long double>(NAN)
    #define __WSTL_HAS_NAN__ true
#elif defined(__WSTL_MATH_SUPPORT__) && defined(__WSTL_NAN_SUPPORT__)
    #define __WSTL_NANF__ nanf("")
    #define __WSTL_NAN__ nan("")
    #define __WSTL_NANL__ nanl("")
    #define __WSTL_HAS_NAN__ true
#else
    #define __WSTL_NANF__ HUGE_VALF
    #define __WSTL_NAN__ HUGE_VAL
    #define __WSTL_NANL__ HUGE_VALL
    #define __WSTL_HAS_NAN__ false
#endif


// Defines introduced

/// @def __WSTL_NAN_SUPPORT__
/// @brief If defined, the library will use math.h NaN functions where necessary
/// @ingroup math
#ifdef __DOXYGEN__
    #define __WSTL_NAN_SUPPORT__ 
#endif

namespace wstl {
    /// @brief Indicates floating-point rounding modes
    /// @ingroup utilities
    /// @see https://en.cppreference.com/w/cpp/types/numeric_limits/float_round_style
    enum FloatRoundStyle {
        ROUND_INTERMEDIATE = -1,
        ROUND_TOWARD_ZERO = 0,
        ROUND_TO_NEAREST = 1,
        ROUND_TOWARD_INFINITY = 2,
        ROUND_TOWARD_NEGATIVE_INFINITY = 3
    };

    /// @brief Indicates floating-point denormalization modes
    /// @ingroup utilities
    /// @see https://en.cppreference.com/w/cpp/types/numeric_limits/float_denorm_style
    enum FloatDenormalizationStyle {
        DENORMALIZATION_INDETERMINATE = -1,
        DENORMALIZATION_ABSENT = 0,
        DENORMALIZATION_PRESENT = 1
    };

    namespace __private {
        template <typename T = void>
        class __IntegralLimitsCommon {
        public:
            static const __WSTL_CONSTEXPR__ bool IsSpecialized = true;
            static const __WSTL_CONSTEXPR__ bool IsExact = true;
            static const __WSTL_CONSTEXPR__ bool IsInteger = true;
            static const __WSTL_CONSTEXPR__ int MaxDigits10 = 0;
            static const __WSTL_CONSTEXPR__ int Radix = 2;
            static const __WSTL_CONSTEXPR__ int MinExponent = 0;
            static const __WSTL_CONSTEXPR__ int MaxExponent = 0;
            static const __WSTL_CONSTEXPR__ int MinExponent10 = 0;
            static const __WSTL_CONSTEXPR__ int MaxExponent10 = 0;
            static const __WSTL_CONSTEXPR__ bool HasInfinity = false;
            static const __WSTL_CONSTEXPR__ bool HasQuietNaN = false;
            static const __WSTL_CONSTEXPR__ bool HasSignalingNaN = false;
            static const __WSTL_CONSTEXPR__ bool HasDenormalizationLoss = false;
            static const __WSTL_CONSTEXPR__ bool IsIEC559 = false;
            static const __WSTL_CONSTEXPR__ bool IsBounded = true;
            static const __WSTL_CONSTEXPR__ bool Traps = false;
            static const __WSTL_CONSTEXPR__ bool TinynessBefore = false;
            static const __WSTL_CONSTEXPR__ FloatDenormalizationStyle HasDenormalization = DENORMALIZATION_ABSENT;
            static const __WSTL_CONSTEXPR__ FloatRoundStyle RoundStyle = ROUND_TOWARD_ZERO;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::IsSpecialized;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::IsExact;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::IsInteger;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __IntegralLimitsCommon<T>::MaxDigits10;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __IntegralLimitsCommon<T>::Radix;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __IntegralLimitsCommon<T>::MinExponent;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __IntegralLimitsCommon<T>::MaxExponent;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __IntegralLimitsCommon<T>::MinExponent10;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __IntegralLimitsCommon<T>::MaxExponent10;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::HasInfinity;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::HasQuietNaN;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::HasSignalingNaN;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::HasDenormalizationLoss;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::IsIEC559;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::IsBounded;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::Traps;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __IntegralLimitsCommon<T>::TinynessBefore;

        template<typename T>
        const __WSTL_CONSTEXPR__ FloatDenormalizationStyle __IntegralLimitsCommon<T>::HasDenormalization;

        template<typename T>
        const __WSTL_CONSTEXPR__ FloatRoundStyle __IntegralLimitsCommon<T>::RoundStyle;

        // Floating point limits common

        template<typename T = void>
        class __FloatingPointLimitsCommon {
        public:
            static const __WSTL_CONSTEXPR__ bool IsSpecialized = true;
            static const __WSTL_CONSTEXPR__ bool IsExact = false;
            static const __WSTL_CONSTEXPR__ bool IsInteger = false;
            static const __WSTL_CONSTEXPR__ bool IsSigned = true;
            static const __WSTL_CONSTEXPR__ int Radix = 2;
            static const __WSTL_CONSTEXPR__ bool HasInfinity = true;
            static const __WSTL_CONSTEXPR__ bool HasQuietNaN = __WSTL_HAS_NAN__;
            static const __WSTL_CONSTEXPR__ bool HasSignalingNaN = __WSTL_HAS_NAN__;
            static const __WSTL_CONSTEXPR__ bool HasDenormalizationLoss = false;
            static const __WSTL_CONSTEXPR__ bool IsIEC559 = true;
            static const __WSTL_CONSTEXPR__ bool IsBounded = true;
            static const __WSTL_CONSTEXPR__ bool IsModulo = false;
            static const __WSTL_CONSTEXPR__ bool Traps = false;
            static const __WSTL_CONSTEXPR__ bool TinynessBefore = false;
            static const __WSTL_CONSTEXPR__ FloatDenormalizationStyle HasDenormalization = DENORMALIZATION_INDETERMINATE;
            static const __WSTL_CONSTEXPR__ FloatRoundStyle RoundStyle = ROUND_INTERMEDIATE;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsSpecialized;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsExact;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsInteger;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsSigned;

        template<typename T>
        const __WSTL_CONSTEXPR__ int __FloatingPointLimitsCommon<T>::Radix;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::HasInfinity;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::HasQuietNaN;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::HasSignalingNaN;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::HasDenormalizationLoss;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsIEC559;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsBounded;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::IsModulo;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::Traps;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FloatingPointLimitsCommon<T>::TinynessBefore;

        template<typename T>
        const __WSTL_CONSTEXPR__ FloatDenormalizationStyle __FloatingPointLimitsCommon<T>::HasDenormalization;
        
        template<typename T>
        const __WSTL_CONSTEXPR__ FloatRoundStyle __FloatingPointLimitsCommon<T>::RoundStyle;
    }
    
    /// @brief Provides an interface to query properties of all fundamental numeric types
    /// @tparam T Fundamental numeric type to query
    /// @ingroup utilities
    /// @see https://en.cppreference.com/w/cpp/types/numeric_limits
    template<typename T>
    class NumericLimits;

    template<typename T>
    class NumericLimits<const T> : public NumericLimits<T> {};

    template<typename T>
    class NumericLimits<volatile T> : public NumericLimits<T> {};

    template<typename T>
    class NumericLimits<const volatile T> : public NumericLimits<T> {};

    // boolean

    template<>
    class NumericLimits<bool> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = 1;
        static const __WSTL_CONSTEXPR__ int Digits10 = 0;
        static const __WSTL_CONSTEXPR__ bool IsSigned = false;
        static const __WSTL_CONSTEXPR__ bool IsModulo = false;
        
        static __WSTL_CONSTEXPR__ bool Min() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool Max() __WSTL_NOEXCEPT__ { return true; }
        static __WSTL_CONSTEXPR__ bool Lowest() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool Epsilon() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool RoundError() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool Infinity() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool QuietNaN() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool SignalingNaN() __WSTL_NOEXCEPT__ { return false; }
        static __WSTL_CONSTEXPR__ bool DenormalizedMin() __WSTL_NOEXCEPT__ { return false; }
    };

    // char

    template<>
    class NumericLimits<char> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(char)) - (IsSigned<char>::Value ? 1 : 0);
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = IsSigned<char>::Value;
        static const __WSTL_CONSTEXPR__ bool IsModulo = IsUnsigned<char>::Value;

        static __WSTL_CONSTEXPR__ char Min() __WSTL_NOEXCEPT__ { return char(CHAR_MIN); }
        static __WSTL_CONSTEXPR__ char Max() __WSTL_NOEXCEPT__ { return char(CHAR_MAX); }
        static __WSTL_CONSTEXPR__ char Lowest() __WSTL_NOEXCEPT__ { return char(CHAR_MIN); }
        static __WSTL_CONSTEXPR__ char Epsilon() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ char RoundError() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ char Infinity() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ char QuietNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ char SignalingNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ char DenormalizedMin() __WSTL_NOEXCEPT__ { return 0; }
    };

    // signed char

    template<>
    class NumericLimits<signed char> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(signed char)) - 1;
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = true;
        static const __WSTL_CONSTEXPR__ bool IsModulo = false;

        static __WSTL_CONSTEXPR__ signed char Min() __WSTL_NOEXCEPT__ { return SCHAR_MIN; }
        static __WSTL_CONSTEXPR__ signed char Max() __WSTL_NOEXCEPT__ { return SCHAR_MAX; }
        static __WSTL_CONSTEXPR__ signed char Lowest() __WSTL_NOEXCEPT__ { return SCHAR_MIN; }
        static __WSTL_CONSTEXPR__ signed char Epsilon() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ signed char RoundError() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ signed char Infinity() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ signed char QuietNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ signed char SignalingNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ signed char DenormalizedMin() __WSTL_NOEXCEPT__ { return 0; }
    };

    // unsigned char

    template<>
    class NumericLimits<unsigned char> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(unsigned char));
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = false;
        static const __WSTL_CONSTEXPR__ bool IsModulo = true;

        static __WSTL_CONSTEXPR__ unsigned char Min() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char Max() __WSTL_NOEXCEPT__ { return UCHAR_MAX; }
        static __WSTL_CONSTEXPR__ unsigned char Lowest() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char Epsilon() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char RoundError() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char Infinity() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char QuietNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char SignalingNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned char DenormalizedMin() __WSTL_NOEXCEPT__ { return 0U; }
    };

    // char8_t 
    
    #ifdef __WSTL_CXX20__
    template<>
    class NumericLimits<char8_t> : public __private::__IntegralLimitsCommon<> {
    public:
        static constexpr int Digits = (CHAR_BIT * sizeof(char8_t)) - (IsSigned<char8_t>::Value ? 1 : 0);
        static constexpr int Digits10 = __WSTL_LOG10_2__(Digits);
        static constexpr bool IsSigned = IsSigned<char8_t>::Value;
        static constexpr bool IsModulo = false;

        static constexpr char8_t Min() __WSTL_NOEXCEPT__ { return char8_t(CHAR_MIN); }
        static constexpr char8_t Max() __WSTL_NOEXCEPT__ { return char8_t(CHAR_MAX); }
        static constexpr char8_t Lowest() __WSTL_NOEXCEPT__ { return char8_t(CHAR_MIN); }
        static constexpr char8_t Epsilon() __WSTL_NOEXCEPT__ { return 0; }
        static constexpr char8_t RoundError() __WSTL_NOEXCEPT__ { return 0; }
        static constexpr char8_t Infinity() __WSTL_NOEXCEPT__ { return 0; }
        static constexpr char8_t QuietNaN() __WSTL_NOEXCEPT__ { return 0; }
        static constexpr char8_t SignalingNaN() __WSTL_NOEXCEPT__ { return 0; }
        static constexpr char8_t DenormalizedMin() __WSTL_NOEXCEPT__ { return 0; }
    };
    #endif

    // char16_t

    #ifdef __WSTL_CXX11__
    template<>
    class NumericLimits<char16_t> : public __private::__IntegralLimitsCommon<> {
    public:
        static constexpr int Digits = (CHAR_BIT * sizeof(char16_t));
        static constexpr int Digits10 = __WSTL_LOG10_2__(Digits);
        static constexpr bool IsSigned = false;
        static constexpr bool IsModulo = true;

        static constexpr char16_t Min() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t Max() __WSTL_NOEXCEPT__ { return UINT_LEAST16_MAX; }
        static constexpr char16_t Lowest() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t Epsilon() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t RoundError() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t Infinity() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t QuietNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t SignalingNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char16_t DenormalizedMin() __WSTL_NOEXCEPT__ { return 0U; }
    };

    // char32_t

    template<>
    class NumericLimits<char32_t> : public __private::__IntegralLimitsCommon<> {
    public:
        static constexpr int Digits = (CHAR_BIT * sizeof(char32_t));
        static constexpr int Digits10 = __WSTL_LOG10_2__(Digits);
        static constexpr bool IsSigned = false;
        static constexpr bool IsModulo = true;

        static constexpr char32_t Min() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t Max() __WSTL_NOEXCEPT__ { return UINT_LEAST32_MAX; }
        static constexpr char32_t Lowest() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t Epsilon() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t RoundError() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t Infinity() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t QuietNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t SignalingNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static constexpr char32_t DenormalizedMin() __WSTL_NOEXCEPT__ { return 0U; }
    };
    #endif

    // wchar_t

    template<>
    class NumericLimits<wchar_t> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(wchar_t)) - (IsSigned<wchar_t>::Value ? 1 : 0);
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = IsSigned<wchar_t>::Value;
        static const __WSTL_CONSTEXPR__ bool IsModulo = IsUnsigned<wchar_t>::Value;

        static __WSTL_CONSTEXPR__ wchar_t Min() __WSTL_NOEXCEPT__ { return WCHAR_MIN; }
        static __WSTL_CONSTEXPR__ wchar_t Max() __WSTL_NOEXCEPT__ { return WCHAR_MAX; }
        static __WSTL_CONSTEXPR__ wchar_t Lowest() __WSTL_NOEXCEPT__ { return WCHAR_MIN; }
        static __WSTL_CONSTEXPR__ wchar_t Epsilon() __WSTL_NOEXCEPT__ { return wchar_t(0); }
        static __WSTL_CONSTEXPR__ wchar_t RoundError() __WSTL_NOEXCEPT__ { return wchar_t(0); }
        static __WSTL_CONSTEXPR__ wchar_t Infinity() __WSTL_NOEXCEPT__ { return wchar_t(0); }
        static __WSTL_CONSTEXPR__ wchar_t QuietNaN() __WSTL_NOEXCEPT__ { return wchar_t(0); }
        static __WSTL_CONSTEXPR__ wchar_t SignalingNaN() __WSTL_NOEXCEPT__ { return wchar_t(0); }
        static __WSTL_CONSTEXPR__ wchar_t DenormalizedMin() __WSTL_NOEXCEPT__ { return wchar_t(0); }
    };

    // short

    template<>
    class NumericLimits<short> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(short)) - 1;
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = true;
        static const __WSTL_CONSTEXPR__ bool IsModulo = false;

        static __WSTL_CONSTEXPR__ short Min() __WSTL_NOEXCEPT__ { return SHRT_MIN; }
        static __WSTL_CONSTEXPR__ short Max() __WSTL_NOEXCEPT__ { return SHRT_MAX; }
        static __WSTL_CONSTEXPR__ short Lowest() __WSTL_NOEXCEPT__ { return SHRT_MIN; }
        static __WSTL_CONSTEXPR__ short Epsilon() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ short RoundError() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ short Infinity() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ short QuietNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ short SignalingNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ short DenormalizedMin() __WSTL_NOEXCEPT__ { return 0; }
    };

    // unsigned short

    template<>
    class NumericLimits<unsigned short> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(unsigned short));
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = false;
        static const __WSTL_CONSTEXPR__ bool IsModulo = true;

        static __WSTL_CONSTEXPR__ unsigned short Min() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short Max() __WSTL_NOEXCEPT__ { return USHRT_MAX; }
        static __WSTL_CONSTEXPR__ unsigned short Lowest() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short Epsilon() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short RoundError() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short Infinity() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short QuietNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short SignalingNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned short DenormalizedMin() __WSTL_NOEXCEPT__ { return 0U; }
    };

    // int

    template<>
    class NumericLimits<int> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(int)) - 1;
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = true;
        static const __WSTL_CONSTEXPR__ bool IsModulo = false;

        static __WSTL_CONSTEXPR__ int Min() __WSTL_NOEXCEPT__ { return INT_MIN; }
        static __WSTL_CONSTEXPR__ int Max() __WSTL_NOEXCEPT__ { return INT_MAX; }
        static __WSTL_CONSTEXPR__ int Lowest() __WSTL_NOEXCEPT__ { return INT_MIN; }
        static __WSTL_CONSTEXPR__ int Epsilon() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ int RoundError() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ int Infinity() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ int QuietNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ int SignalingNaN() __WSTL_NOEXCEPT__ { return 0; }
        static __WSTL_CONSTEXPR__ int DenormalizedMin() __WSTL_NOEXCEPT__ { return 0; }
    };

    // unsigned int

    template<>
    class NumericLimits<unsigned int> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(unsigned int));
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = false;
        static const __WSTL_CONSTEXPR__ bool IsModulo = true;

        static __WSTL_CONSTEXPR__ unsigned int Min() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int Max() __WSTL_NOEXCEPT__ { return UINT_MAX; }
        static __WSTL_CONSTEXPR__ unsigned int Lowest() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int Epsilon() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int RoundError() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int Infinity() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int QuietNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int SignalingNaN() __WSTL_NOEXCEPT__ { return 0U; }
        static __WSTL_CONSTEXPR__ unsigned int DenormalizedMin() __WSTL_NOEXCEPT__ { return 0U; }
    };

    // long

    template<>
    class NumericLimits<long> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(long)) - 1;
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = true;
        static const __WSTL_CONSTEXPR__ bool IsModulo = false;

        static __WSTL_CONSTEXPR__ long Min() __WSTL_NOEXCEPT__ { return LONG_MIN; }
        static __WSTL_CONSTEXPR__ long Max() __WSTL_NOEXCEPT__ { return LONG_MAX; }
        static __WSTL_CONSTEXPR__ long Lowest() __WSTL_NOEXCEPT__ { return LONG_MIN; }
        static __WSTL_CONSTEXPR__ long Epsilon() __WSTL_NOEXCEPT__ { return 0L; }
        static __WSTL_CONSTEXPR__ long RoundError() __WSTL_NOEXCEPT__ { return 0L; }
        static __WSTL_CONSTEXPR__ long Infinity() __WSTL_NOEXCEPT__ { return 0L; }
        static __WSTL_CONSTEXPR__ long QuietNaN() __WSTL_NOEXCEPT__ { return 0L; }
        static __WSTL_CONSTEXPR__ long SignalingNaN() __WSTL_NOEXCEPT__ { return 0L; }
        static __WSTL_CONSTEXPR__ long DenormalizedMin() __WSTL_NOEXCEPT__ { return 0L; }
    };

    // unsigned long

    template<>
    class NumericLimits<unsigned long> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(unsigned long));
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = false;
        static const __WSTL_CONSTEXPR__ bool IsModulo = true;

        static __WSTL_CONSTEXPR__ unsigned long Min() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long Max() __WSTL_NOEXCEPT__ { return ULONG_MAX; }
        static __WSTL_CONSTEXPR__ unsigned long Lowest() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long Epsilon() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long RoundError() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long Infinity() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long QuietNaN() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long SignalingNaN() __WSTL_NOEXCEPT__ { return 0UL; }
        static __WSTL_CONSTEXPR__ unsigned long DenormalizedMin() __WSTL_NOEXCEPT__ { return 0UL; }
    };

    // long long

    #ifdef __WSTL_CXX11__
    template<>
    class NumericLimits<long long> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(long long)) - 1;
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = true;
        static const __WSTL_CONSTEXPR__ bool IsModulo = false;
        
        static __WSTL_CONSTEXPR__ long long Min() __WSTL_NOEXCEPT__ { return LLONG_MIN; }
        static __WSTL_CONSTEXPR__ long long Max() __WSTL_NOEXCEPT__ { return LLONG_MAX; }
        static __WSTL_CONSTEXPR__ long long Lowest() __WSTL_NOEXCEPT__ { return LLONG_MIN; }
        static __WSTL_CONSTEXPR__ long long Epsilon() __WSTL_NOEXCEPT__ { return 0LL; }
        static __WSTL_CONSTEXPR__ long long RoundError() __WSTL_NOEXCEPT__ { return 0LL; }
        static __WSTL_CONSTEXPR__ long long Infinity() __WSTL_NOEXCEPT__ { return 0LL; }
        static __WSTL_CONSTEXPR__ long long QuietNaN() __WSTL_NOEXCEPT__ { return 0LL; }
        static __WSTL_CONSTEXPR__ long long SignalingNaN() __WSTL_NOEXCEPT__ { return 0LL; }
        static __WSTL_CONSTEXPR__ long long DenormalizedMin() __WSTL_NOEXCEPT__ { return 0LL; }
    };

    // unsigned long long

    template<>
    class NumericLimits<unsigned long long> : public __private::__IntegralLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = (CHAR_BIT * sizeof(unsigned long long));
        static const __WSTL_CONSTEXPR__ int Digits10 = __WSTL_LOG10_2__(Digits);
        static const __WSTL_CONSTEXPR__ bool IsSigned = false;
        static const __WSTL_CONSTEXPR__ bool IsModulo = true;

        static __WSTL_CONSTEXPR__ unsigned long long Min() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long Max() __WSTL_NOEXCEPT__ { return ULLONG_MAX; }
        static __WSTL_CONSTEXPR__ unsigned long long Lowest() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long Epsilon() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long RoundError() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long Infinity() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long QuietNaN() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long SignalingNaN() __WSTL_NOEXCEPT__ { return 0ULL; }
        static __WSTL_CONSTEXPR__ unsigned long long DenormalizedMin() __WSTL_NOEXCEPT__ { return 0ULL; }
    };
    #endif

    // float

    template<>
    class NumericLimits<float> : public __private::__FloatingPointLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = FLT_MANT_DIG;
        static const __WSTL_CONSTEXPR__ int Digits10 = FLT_DIG;
        static const __WSTL_CONSTEXPR__ int MaxDigits10 = __WSTL_LOG10_2__(Digits) + 2;

        static const __WSTL_CONSTEXPR__ int MinExponent = FLT_MIN_EXP;
        static const __WSTL_CONSTEXPR__ int MinExponent10 = FLT_MIN_10_EXP;
        static const __WSTL_CONSTEXPR__ int MaxExponent = FLT_MAX_EXP;
        static const __WSTL_CONSTEXPR__ int MaxExponent10 = FLT_MAX_10_EXP;

        static __WSTL_CONSTEXPR__ float Min() __WSTL_NOEXCEPT__ { return FLT_MIN; }
        static __WSTL_CONSTEXPR__ float Max() __WSTL_NOEXCEPT__ { return FLT_MAX; }
        static __WSTL_CONSTEXPR__ float Lowest() __WSTL_NOEXCEPT__ { return -FLT_MAX; }
        static __WSTL_CONSTEXPR__ float Epsilon() __WSTL_NOEXCEPT__ { return FLT_EPSILON; }
        static __WSTL_CONSTEXPR__ float RoundError() __WSTL_NOEXCEPT__ { return 0.5F; }
        static __WSTL_CONSTEXPR__ float Infinity() __WSTL_NOEXCEPT__ { return HUGE_VALF; }
        static __WSTL_CONSTEXPR__ float QuietNaN() __WSTL_NOEXCEPT__ { return __WSTL_NANF__; }
        static __WSTL_CONSTEXPR__ float SignalingNaN() __WSTL_NOEXCEPT__ { return __WSTL_NANF__; }
        static __WSTL_CONSTEXPR__ float DenormalizedMin() __WSTL_NOEXCEPT__ { return FLT_MIN; }
    };

    // double

    template<>
    class NumericLimits<double> : public __private::__FloatingPointLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = DBL_MANT_DIG;
        static const __WSTL_CONSTEXPR__ int Digits10 = DBL_DIG;
        static const __WSTL_CONSTEXPR__ int MaxDigits10 = __WSTL_LOG10_2__(Digits) + 2;

        static const __WSTL_CONSTEXPR__ int MinExponent = DBL_MIN_EXP;
        static const __WSTL_CONSTEXPR__ int MinExponent10 = DBL_MIN_10_EXP;
        static const __WSTL_CONSTEXPR__ int MaxExponent = DBL_MAX_EXP;
        static const __WSTL_CONSTEXPR__ int MaxExponent10 = DBL_MAX_10_EXP;

        static __WSTL_CONSTEXPR__ double Min() __WSTL_NOEXCEPT__ { return DBL_MIN; }
        static __WSTL_CONSTEXPR__ double Max() __WSTL_NOEXCEPT__ { return DBL_MAX; }
        static __WSTL_CONSTEXPR__ double Lowest() __WSTL_NOEXCEPT__ { return -DBL_MAX; }
        static __WSTL_CONSTEXPR__ double Epsilon() __WSTL_NOEXCEPT__ { return DBL_EPSILON; }
        static __WSTL_CONSTEXPR__ double RoundError() __WSTL_NOEXCEPT__ { return 0.5; }
        static __WSTL_CONSTEXPR__ double Infinity() __WSTL_NOEXCEPT__ { return HUGE_VAL; }
        static __WSTL_CONSTEXPR__ double QuietNaN() __WSTL_NOEXCEPT__ { return __WSTL_NAN__; }
        static __WSTL_CONSTEXPR__ double SignalingNaN() __WSTL_NOEXCEPT__ { return __WSTL_NAN__; }
        static __WSTL_CONSTEXPR__ double DenormalizedMin() __WSTL_NOEXCEPT__ { return DBL_MIN; }
    };

    // long double

    template<>
    class NumericLimits<long double> : public __private::__FloatingPointLimitsCommon<> {
    public:
        static const __WSTL_CONSTEXPR__ int Digits = LDBL_MANT_DIG;
        static const __WSTL_CONSTEXPR__ int Digits10 = LDBL_DIG;
        static const __WSTL_CONSTEXPR__ int MaxDigits10 = __WSTL_LOG10_2__(Digits) + 2;
        
        static const __WSTL_CONSTEXPR__ int MinExponent = LDBL_MIN_EXP;
        static const __WSTL_CONSTEXPR__ int MinExponent10 = LDBL_MIN_10_EXP;
        static const __WSTL_CONSTEXPR__ int MaxExponent = LDBL_MAX_EXP;
        static const __WSTL_CONSTEXPR__ int MaxExponent10 = LDBL_MAX_10_EXP;

        static __WSTL_CONSTEXPR__ long double Min() __WSTL_NOEXCEPT__ { return LDBL_MIN; }
        static __WSTL_CONSTEXPR__ long double Max() __WSTL_NOEXCEPT__ { return LDBL_MAX; }
        static __WSTL_CONSTEXPR__ long double Lowest() __WSTL_NOEXCEPT__ { return -LDBL_MAX; }
        static __WSTL_CONSTEXPR__ long double Epsilon() __WSTL_NOEXCEPT__ { return LDBL_EPSILON; }
        static __WSTL_CONSTEXPR__ long double RoundError() __WSTL_NOEXCEPT__ { return 0.5L; }
        static __WSTL_CONSTEXPR__ long double Infinity() __WSTL_NOEXCEPT__ { return HUGE_VALL; }
        static __WSTL_CONSTEXPR__ long double QuietNaN() __WSTL_NOEXCEPT__ { return __WSTL_NANL__; }
        static __WSTL_CONSTEXPR__ long double SignalingNaN() __WSTL_NOEXCEPT__ { return __WSTL_NANL__; }
        static __WSTL_CONSTEXPR__ long double DenormalizedMin() __WSTL_NOEXCEPT__ { return LDBL_MIN; }
    };
}

#endif