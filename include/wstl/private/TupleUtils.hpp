// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_TUPLEUTILS_HPP__
#define __WSTL_TUPLEUTILS_HPP__

#include <stddef.h>
#include "../TypeTraits.hpp"
#include "Platform.hpp"


namespace wstl {
    // Tuple element

    /// @brief Obtains element types from tuple-like type
    /// @tparam T Tuple-like type
    /// @tparam Index Index to obtain
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple_element
    template<size_t Index, typename T>
    struct TupleElement;

    template<size_t Index, typename T>
    struct TupleElement<Index, const T> {
        typedef typename AddConst<typename TupleElement<Index, T>::Type>::Type Type;
    };

    template<size_t Index, typename T>
    struct TupleElement<Index, volatile T> {
        typedef typename AddVolatile<typename TupleElement<Index, T>::Type>::Type Type;
    };

    template<size_t Index, typename T>
    struct TupleElement<Index, const volatile T> {
        typedef typename AddCV<typename TupleElement<Index, T>::Type>::Type Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc TupleElement
    /// @since C++11
    template<size_t Index, typename T>
    using TupleElementType = typename TupleElement<Index, T>::Type;
    #endif

    // Tuple size

    /// @brief Obtains the number of elements in tuple-like type
    /// @tparam T Tuple-like type
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple_size
    template<typename T>
    struct TupleSize;

    template<typename T>
    struct TupleSize<const T> : IntegralConstant<size_t, TupleSize<T>::Value> {};

    template<typename T>
    struct TupleSize<volatile T> : IntegralConstant<size_t, TupleSize<T>::Value> {};

    template<typename T>
    struct TupleSize<const volatile T> : IntegralConstant<size_t, TupleSize<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc TupleSize
    /// @since C++17
    template<typename T>
    inline constexpr size_t TupleSizeVariable = TupleSize<T>::Value;
    #endif

    // Integer sequence

    #ifdef __WSTL_CXX11__
    /// @brief Creates compile-time sequence of integers
    /// @tparam T Integer type
    /// @tparam ...Integers Sequence of integers
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/integer_sequence
    template<typename T, T... Integers>
    struct IntegerSequence {
        WSTL_STATIC_ASSERT(IsIntegral<T>::Value, "Integral types only!");

        typedef T ValueType;

        static constexpr size_t Size() __WSTL_NOEXCEPT__ {
            return sizeof...(Integers);
        }
    };

    /// @brief Helper alias for IntegerSequence with `size_t` as type
    /// @tparam ...Indices Sequence of indices
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/integer_sequence
    template<size_t... Indices>
    using IndexSequence = IntegerSequence<size_t, Indices...>;

    namespace __private {
        template<size_t N, size_t... Indices>
        struct __MakeIndexSequence {
            typedef typename __MakeIndexSequence<N - 1, N - 1, Indices...>::Type Type;
        };

        template<size_t... Indices>
        struct __MakeIndexSequence<0, Indices...> {
            typedef IndexSequence<Indices...> Type;
        };

        template<typename T, size_t... Indices>
        static constexpr IntegerSequence<T, static_cast<T>(Indices)...> __Generate(IndexSequence<Indices...>);
    }

    /// @brief Makes compile-time sequence of indices (from 0 to N - 1)
    /// @tparam N Number of indices to generate
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/integer_sequence
    template<size_t N>
    using MakeIndexSequence = typename __private::__MakeIndexSequence<N>::Type;
    
    /// @brief Makes compile-time sequence of integers (from 0 to N - 1)
    /// @tparam T Integer type
    /// @tparam N Number of integers to generate
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/integer_sequence
    template<typename T, T N>
    using MakeIntegerSequence = decltype(__private::__Generate<T>(MakeIndexSequence<N>()));

    /// @brief Makes compile-time sequence of integers (from 0 to N - 1) of the length of T
    /// @tparam T... Parameter pack to use
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/integer_sequence
    template<typename... T>
    using IndexSequenceFor = MakeIndexSequence<sizeof...(T)>;
    #endif
}

#endif