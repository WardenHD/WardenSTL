// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_TUPLEPROPERTIES_HPP__
#define __WSTL_TUPLEPROPERTIES_HPP__

#include <stddef.h>
#include "../TypeTraits.hpp"
#include "Platform.hpp"


/// @brief Main namespace of WardenSTL library
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
}

#endif