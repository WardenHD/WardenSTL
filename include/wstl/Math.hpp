// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s math utilities,
// and several custom functions have been added for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_MATH_HPP__
#define __WSTL_MATH_HPP__

#include "private/Platform.hpp"
#include "TypeTraits.hpp"
#include "Limits.hpp"
#include "Exception.hpp"

#include <stdint.h>
#include <float.h>

#ifdef __WSTL_MATH_SUPPORT__
#include <math.h>
#include <stdlib.h>
#endif


// Defines introduced

/// @def __WSTL_MATH_SUPPORT__
/// @brief If defined, the library will use math.h and stdlib.h functions where necessary
/// @ingroup math
#ifdef __DOXYGEN__
    #define __WSTL_MATH_SUPPORT__ 
#endif

namespace wstl {
    // Absolute

    /// @brief Computes an absolute value of integral type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/abs
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsSigned<T>::Value, T>::Type Absolute(T value) {
        return (value < T(0)) ? -value : value;
    }

    /// @brief Computes an absolute value of integral type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/abs
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsUnsigned<T>::Value, T>::Type Absolute(T value) {
        return value;
    }

    // Absolute unsigned

    /// @brief Computes an absolute value of integral type and returns the result as unsigned type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value as unsigned type
    /// @ingroup maths
    #ifdef __WSTL_CXX11__
    template<typename T, typename Return = MakeUnsignedType<T>>
    #else
    template<typename T, typename Return>
    #endif
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsSigned<T>::Value, Return>::Type AbsoluteUnsigned(T value) __WSTL_NOEXCEPT__ {
        return (value == NumericLimits<T>::Min()) ? (NumericLimits<Return>::Max() / 2U) + 1U : 
            (value < T(0)) ? Return(-value) : Return(value);
    }

    /// @brief Computes an absolute value of integral type and returns the result as unsigned type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value as unsigned type
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsSigned<T>::Value, T>::Type AbsoluteUnsigned(T value) __WSTL_NOEXCEPT__ {
        return Absolute(value);
    }

    // Compile-time equivalent

    namespace compile {
        /// @brief Computes an absolute value of integral type at compile time
        /// @tparam T Type of the input value
        /// @tparam Result Type of the output value
        /// @tparam N Value to compute the absolute value of
        /// @ingroup maths
        template<typename T, T N, typename Result = T, bool = IsSigned<T>::Value>
        class Absolute;

        template<typename T, T N, typename Result>
        class Absolute<T, N, Result, false> {
        public:
            static const __WSTL_CONSTEXPR__ Result Value = N;
        };

        template<typename T, T N, typename Result>
        class Absolute<T, N, Result, true> {    
        private:
            static const __WSTL_CONSTEXPR__ Result AbsoluteValue = (N < T(0) && (N >= NumericLimits<Result>::Min() || Result(-N) <= NumericLimits<Result>::Max())) 
                ? Result(-N) : Result(N);
            static const __WSTL_CONSTEXPR__ bool Fallback = (AbsoluteValue != (N < 0 ? -N : N));

            StaticAssert(!Fallback, "Cannot represent the absolute value in the provided result type");

        public:
            static const __WSTL_CONSTEXPR__ Result Value = AbsoluteValue;
        };

        #ifdef __WSTL_CXX17__
        /// @copydoc Absolute
        /// @since C++17
        template<typename T, T N, typename Result = T>
        inline constexpr Result AbsoluteVariable = Absolute<T, N, Result>::Value;
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Computes an absolute value of integral type at compile time and holds the result as unsigned type
        /// @tparam T Type of the input value
        /// @tparam N Value to compute the absolute value of
        /// @tparam Result Type of the output value (default `unsigned T`)
        /// @ingroup maths
        template<typename T, T N, typename Result = typename MakeUnsigned<T>::Type>
        using AbsoluteUnsigned = Absolute<T, N, Result>;
        #else
        /// @brief Computes an absolute value of integral type at compile time and holds the result as unsigned type
        /// @tparam T Type of the input value
        /// @tparam N Value to compute the absolute value of
        /// @tparam Result Type of the output value (default `unsigned T`)
        /// @ingroup maths
        template<typename T, T N, typename Result = typename MakeUnsigned<T>::Type>
        class AbsoluteUnsigned : public Absolute<T, N, Result> {};
        #endif
        
        #ifdef __WSTL_CXX17__
        /// @copydoc AbsoluteUnsigned
        /// @since C++17
        template<typename T, T N, typename Result = typename MakeUnsigned<T>::Type>
        inline constexpr Result AbsoluteUnsignedVariable = AbsoluteUnsigned<T, N, Result>::Value;
        #endif
    }
    
    // Divide

    /// @brief Result type of `Divide` function
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/div
    template<typename T>
    struct DivisionType {
        T Quotient;
        T Remainder;
    };

    /// @brief Computes quotient and remainder of integral division
    /// @param x Divident
    /// @param y Divisor
    /// @return A `DivisionType` object that holds quotient and remainder
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/div
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsIntegral<T>::Value, DivisionType<T> >::Type Divide(T x, T y) {
        return { x / y, x % y };
    }

    // Power

    /// @brief Raises a number of integral type to the given power
    /// @tparam N Number to raise
    /// @tparam POWER Power to raise to
    /// @tparam UT Unsigned type to use for storing the result (default is `uint64_t`)
    /// @note Only positive exponents are supported
    /// @ingroup maths
    template<size_t N, size_t POWER, typename UT = uint64_t>
    struct Power {
        static const __WSTL_CONSTEXPR__ UT Value = N * Power<N, POWER - 1, UT>::Value;
    };

    /// @brief Raises a number of integral type to the given power - specialization for exponent 0
    /// @tparam N Number to raise
    /// @tparam UT Unsigned type to use for storing the result (default is `uint64_t`)
    /// @note Only positive exponents are supported
    /// @ingroup maths
    template<size_t N, typename UT>
    struct Power<N, 0, UT> {
        static const __WSTL_CONSTEXPR__ UT Value = 1;
    };

    template<size_t N, typename UT>
    const __WSTL_CONSTEXPR__ UT Power<N, 0, UT>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc Power
    /// @ingroup maths
    template<size_t N, size_t POWER, typename UT = uint64_t>
    inline constexpr UT PowerVariable = Power<N, POWER, UT>::Value;
    #endif

    // Nth root

    /// @brief Computes nth root of a number of integral type
    /// @tparam N Number to compute the root of
    /// @tparam ROOT Root to compute
    /// @ingroup maths
    template<size_t N, size_t ROOT, size_t I = 1, typename Enable = typename EnableIf<(ROOT >= 2)>::Type>
    class NthRoot {
    private:
        typedef typename Conditional<(Power<I, ROOT>::Value > N), IntegralConstant<intmax_t, I - 1>, 
        NthRoot<N, ROOT, I + 1, Enable>>::Type Type;

    public:
        #ifdef __WSTL_CXX11__
        static constexpr size_t Value = Type::Value;
        #else
        enum {
            Value = Type::Value;
        }
        #endif
    };

    #ifdef __WSTL_CXX11__
    template<size_t N, size_t ROOT, size_t I, typename Enable>
    constexpr size_t NthRoot<N, ROOT, I, Enable>::Value;
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc NthRoot
    /// @ingroup maths
    template<size_t N, size_t ROOT, size_t I = 1>
    inline constexpr size_t NthRootVariable = NthRoot<N, ROOT, I>::Value;
    #endif

    // Square root

    /// @brief Computes square root of a number of integral type
    /// @tparam N Number to compute the square root of
    /// @ingroup maths
    template<size_t N, size_t I = 1>
    struct SquareRoot : NthRoot<N, 2, I> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc SquareRoot
    /// @ingroup maths
    template<size_t N, size_t I = 1>
    inline constexpr size_t SquareRootVariable = SquareRoot<N, I>::Value;
    #endif

    // Logarithm

    /// @brief Computes logarithm of a number of integral type
    /// @tparam N Number to compute the logarithm of
    /// @tparam BASE Base of the logarithm
    /// @ingroup maths
    template<size_t N, size_t BASE>
    struct Logarithm {
        #ifdef __WSTL_CXX11__
        static constexpr size_t Value = (N >= BASE) ? 1 + Logarithm<N / BASE, BASE>::Value : 0;
        #else
        enum {
            Value = (N >= BASE) ? 1 + Logarithm<N / BASE, BASE>::Value : 0;
        }
        #endif
    };

    /// @brief Computes logarithm of a number of integral type - specialization for N = 1
    /// @tparam BASE Base of the logarithm
    /// @ingroup maths
    template<size_t BASE>
    struct Logarithm<1, BASE> {
        #ifdef __WSTL_CXX11__
        static constexpr size_t Value = 0;
        #else
        enum {
            Value = 0;
        }
        #endif
    };

    /// @brief Computes logarithm of a number of integral type - specialization for N = 0
    /// @tparam BASE Base of the logarithm
    /// @ingroup maths
    template<size_t BASE>
    struct Logarithm<0, BASE> {
        #ifdef __WSTL_CXX11__
        static constexpr size_t Value = 0;
        #else
        enum {
            Value = 0;
        }
        #endif
    };

    #ifdef __WSTL_CXX11__
    template<size_t N, size_t BASE>
    constexpr size_t Logarithm<N, BASE>::Value;
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc Logarithm
    /// @ingroup maths
    template<size_t N, size_t BASE>
    inline constexpr size_t LogarithmVariable = Logarithm<N, BASE>::Value;
    #endif

    /// @brief Computes logarithm of a number of integral type with base 2
    /// @tparam N Number to compute the logarithm of
    /// @ingroup maths
    template<size_t N>
    struct Logarithm2 : Logarithm<N, 2> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Logarithm2
    /// @ingroup maths
    template<size_t N>
    inline constexpr size_t Logarithm2Variable = Logarithm2<N>::Value;
    #endif

    /// @brief Computes logarithm of a number of integral type with base 10
    /// @tparam N Number to compute the logarithm of
    /// @ingroup maths
    template<size_t N>
    struct Logarithm10 : Logarithm<N, 10> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Logarithm10
    /// @ingroup maths
    template<size_t N>
    inline constexpr size_t Logarithm10Variable = Logarithm10<N>::Value;
    #endif

    // Factorial

    /// @brief Computes factorial of a number of integral type
    /// @tparam N Number to compute the factorial of
    /// @ingroup maths
    template<size_t N>
    struct Factorial {
        static const __WSTL_CONSTEXPR__ size_t Value = N * Factorial<N - 1>::Value;
    };

    /// @brief Computes factorial of a number of integral type - specialization for N = 0
    /// @ingroup maths
    template<>
    struct Factorial<0> {
        static const __WSTL_CONSTEXPR__ size_t Value = 1;
    };

    template<size_t N>
    const __WSTL_CONSTEXPR__ size_t Factorial<N>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc Factorial
    /// @ingroup maths
    template<size_t N>
    inline constexpr size_t FactorialVariable = Factorial<N>::Value;
    #endif

    // Fibonacci

    /// @brief Computes Fibonacci sequence for nth number
    /// @tparam N Nth number in the Fibonacci sequence
    /// @ingroup maths
    template<size_t N>
    struct Fibonacci {
        static const __WSTL_CONSTEXPR__ size_t Value = Fibonacci<N - 1>::Value + Fibonacci<N - 2>::Value;
    };

    /// @brief Computes Fibonacci sequence for nth number - specialization for N = 1
    /// @ingroup maths
    template<>
    struct Fibonacci<1> {
        static const __WSTL_CONSTEXPR__ size_t Value = 1;
    };

    /// @brief Computes Fibonacci sequence for nth number - specialization for N = 0
    /// @ingroup maths
    template<>
    struct Fibonacci<0> {
        static const __WSTL_CONSTEXPR__ size_t Value = 0;
    };

    template<size_t N>
    const __WSTL_CONSTEXPR__ size_t Fibonacci<N>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc Fibonacci
    /// @ingroup maths
    template<size_t N>
    inline constexpr size_t FibonacciVariable = Fibonacci<N>::Value;
    #endif

    // Permutations

    /// @brief Computes permutations of N taken R at a time
    /// @tparam N Total number of elements
    /// @tparam R Number of elements selected
    /// @ingroup maths
    template<size_t N, size_t R>
    struct Permutations {
        static const __WSTL_CONSTEXPR__ size_t Value = N * Permutations<N - 1, R - 1>::Value;
    };

    template<size_t N, size_t R>
    const __WSTL_CONSTEXPR__ size_t Permutations<N, R>::Value;

    /// @brief Computes permutations of N taken R at a time - specialization for R = 0
    /// @tparam N Total number of elements
    /// @ingroup maths
    template<size_t N>
    struct Permutations<N, 0> {
        static const __WSTL_CONSTEXPR__ size_t Value = 1;
    };

    template<size_t N>
    const __WSTL_CONSTEXPR__ size_t Permutations<N, 0>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc Permutations
    /// @ingroup maths
    template<size_t N, size_t R>
    inline constexpr size_t PermutationsVariable = Permutations<N, R>::Value;
    #endif

    // Combinations

    /// @brief Computes combinations of N taken R at a time
    /// @ingroup maths
    template<size_t N, size_t R>
    struct Combinations {
        static const __WSTL_CONSTEXPR__ size_t Value = Permutations<N, R>::Value / Factorial<R>::Value;
    };

    template<size_t N, size_t R>
    const __WSTL_CONSTEXPR__ size_t Combinations<N, R>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc Combinations
    /// @ingroup maths
    template<size_t N, size_t R>
    inline constexpr size_t CombinationsVariable = Combinations<N, R>::Value;
    #endif

    // Is NaN

    #if defined(__WSTL_CXX11__) && defined(__WSTL_MATH_SUPPORT__)
    /// @brief Checks if a number is Not a Number
    /// @param value Value to check
    /// @return `true` if the value is NaN, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsNaN(T value) {
        return fpclassify(value) == FP_NAN;
    }
    #else
    /// @brief Checks if a number is Not a Number
    /// @param value Value to check
    /// @return `true` if the value is NaN, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsNaN(T value) {
        return value != value;
    }
    #endif

    /// @brief Checks if a number is Not a Number
    /// @param value Value to check
    /// @return `true` if the value is NaN, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsFloatingPoint<T>::Value, bool>::Type IsNaN(T value) {
        return false;
    }

    // Is infinity

    #if defined(__WSTL_CXX11__) && defined(__WSTL_MATH_SUPPORT__)
    /// @brief Checks whether a floating point value is infinity
    /// @param value Value to check
    /// @return `true` if the value is infinity, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsInfinity(T value) {
        return fpclassify(value) == FP_INFINITE;
    }
    #else
    /// @brief Checks whether a floating point value is infinity
    /// @param value Value to check
    /// @return `true` if the value is infinity, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsInfinity(T value) {
        return (value == NumericLimits<T>::Infinity()) || (value == -NumericLimits<T>::Infinity());
    }
    #endif

    /// @brief Checks whether a floating point value is infinity
    /// @param value Value to check
    /// @return `true` if the value is infinity, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsFloatingPoint<T>::Value, bool>::Type IsInfinity(T value) {
        return false;
    }

    // Is zero

    #if defined(__WSTL_CXX11__) && defined(__WSTL_MATH_SUPPORT__)
    /// @brief Checks whether a floating point value is zero
    /// @param value Value to check
    /// @return `true` if the value is zero, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsZero(T value) {
        return fpclassify(value) == FP_ZERO;
    }
    #else
    /// @brief Checks whether a floating point value is zero
    /// @param value Value to check
    /// @return `true` if the value is zero, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsZero(T value) {
        return value == T(0);
    }
    #endif

    /// @brief Checks whether a floating point value is zero
    /// @param value Value to check
    /// @return `true` if the value is zero, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsFloatingPoint<T>::Value, bool>::Type IsZero(T value) {
        return value == T(0);
    }

    // Is exactly equal

    /// @brief Checks whether two values are exactly equal
    /// @param x First value
    /// @param y Second value
    /// @return `true` if the values are exactly equal, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline bool IsExactlyEqual(T x, T y) {
        return x == y;
    }

    // Math constants

    namespace __private {
        template<typename T = void>
        struct __MathConstants {
            static const __WSTL_CONSTEXPR__ double PI = 3.14159265358979;
            static const __WSTL_CONSTEXPR__ double PI_RECIRPOCAL = 0.31830988618379;
            static const __WSTL_CONSTEXPR__ double PI_SQUARED = 9.86960440108936;
            static const __WSTL_CONSTEXPR__ double E = 2.71828182845905;
            static const __WSTL_CONSTEXPR__ double E_RECIPROCAL = 0.36787944117144;
            static const __WSTL_CONSTEXPR__ double E_SQUARED = 7.38905609893065;
            static const __WSTL_CONSTEXPR__ double ROOT2 = 1.41421356237310;
            static const __WSTL_CONSTEXPR__ double ROOT2_RECIPROCAL = 0.70710678118655;
            static const __WSTL_CONSTEXPR__ double EULER = 0.57721566490153;
            static const __WSTL_CONSTEXPR__ double GOLDEN_RATIO = 1.61803398874989;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::PI;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::PI_RECIRPOCAL;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::PI_SQUARED;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::E;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::E_RECIPROCAL;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::E_SQUARED;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::ROOT2;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::ROOT2_RECIPROCAL;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::EULER;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::GOLDEN_RATIO;
    }

    /// @brief Class that defines various mathematical constants
    /// @ingroup maths
    struct MathConstants : __private::__MathConstants<> {};
}   

#endif