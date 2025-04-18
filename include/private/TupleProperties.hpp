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
    class TupleElement;

    template<size_t Index, typename T>
    class TupleElement<Index, const T> {
    public:
        typedef typename AddConst<typename TupleElement<Index, T>::Type>::Type Type;
    };

    template<size_t Index, typename T>
    class TupleElement<Index, volatile T> {
    public:
        typedef typename AddVolatile<typename TupleElement<Index, T>::Type>::Type Type;
    };

    template<size_t Index, typename T>
    class TupleElement<Index, const volatile T> {
    public:
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
    class TupleSize;

    template<typename T>
    class TupleSize<const T> : public IntegralConstant<size_t, TupleSize<T>::Value> {};

    template<typename T>
    class TupleSize<volatile T> : public IntegralConstant<size_t, TupleSize<T>::Value> {};

    template<typename T>
    class TupleSize<const volatile T> : public IntegralConstant<size_t, TupleSize<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc TupleSize
    /// @since C++17
    template<typename T>
    inline constexpr size_t TupleSizeVariable = TupleSize<T>::Value;
    #endif
}

#endif