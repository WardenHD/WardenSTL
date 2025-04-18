// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s utility.h
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been modified by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_UTILITY_HPP__
#define __WSTL_UTILITY_HPP__

#include <stddef.h>
#include "TypeTraits.hpp"
#include "StaticAssert.hpp"
#include "private/Platform.hpp"
#include "private/Swap.hpp"
#include "private/TupleProperties.hpp"


/// @defgroup utility Utility
/// @brief A set of utility templates
/// @ingroup utilities

/// @brief Main namespace of WardenSTL library
namespace wstl {
    // As const

    template<typename T>
    /// @brief Gets const reference to the object
    /// @param t Object to get const reference to
    /// @return Const reference to the object
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/as_const
    __WSTL_CONSTEXPR__ 
    inline typename AddConst<T>::Type& AsConst(T& t) __WSTL_NOEXCEPT__ {
        return t;
    }

    #ifdef __WSTL_CXX11__
    /// @brief Const rvalue reference is deleted to disallow rvalue arguments
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/as_const
    template<typename T>
    void AsConst(const T&&) = delete;
    #endif

    // Exchange

    #ifdef __WSTL_CXX11__
    /// @brief Replaces the value of object, returning its old value
    /// @param object Object whose value to replace
    /// @param newValue Value to assign
    /// @return The old value of object
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/exchange
    template<typename T, typename U = T>
    __WSTL_CONSTEXPR14__ T Exchange(T& object, U&& newValue) {
        T old = Move(object);
        object = Forward<U>(newValue);
        return old;
    }
    #else
    /// @brief Replaces the value of object, returning its old value
    /// @param object Object whose value to replace
    /// @param newValue Value to assign
    /// @return The old value of object
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/exchange
    template<typename T>
    T Exchange(T& object, const T& newValue) {
        T old = object;
        object = newValue;
        return old;
    }

    /// @copydoc Exchange(T& object, const T&)
    template<typename T, typename U>
    T Exchange(T& object, const U& newValue) {
        T old = object;
        object = newValue;
        return old;
    }
    #endif

    // Pair

    /// @brief Stores two objects as a single unit
    /// @tparam T1 Type of the first object
    /// @tparam T2 Type of the second object
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair
    template<typename T1, typename T2>
    class Pair {
    public:
        /// @brief Type alias for the first object
        typedef T1 FirstType;
        /// @brief Type alias for the second object
        typedef T2 SecondType;

        FirstType First;
        SecondType Second;

        /// @brief Default constructor - initializes both objects to their default-constructed values    
        __WSTL_CONSTEXPR__ Pair() : First(FirstType()), Second(SecondType()) {}

        /// @brief Parameterized constructor
        /// @param first Value of the first object
        /// @param second Value of the second object
        __WSTL_CONSTEXPR14__ Pair(const T1& first, const T2& second) : First(first), Second(second) {}

        /// @brief Copy constructor - copies from pair of the same types
        /// @param other Pair to copy from
        __WSTL_CONSTEXPR14__ Pair(const Pair<T1, T2>& other) : First(other.First), Second(other.Second) {}

        /// @brief Templated copy constructor - copies from pair of potentially different types
        /// @param other Pair to copy from
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair(const Pair<U1, U2>& other) : First(other.First), Second(other.Second) {}

        #ifdef __WSTL_CXX11__
        /// @brief Templated parameterized constructor - with forwarding for both members
        /// @param first Value for the first object
        /// @param second Value for the second object
        /// @since C++11
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair(U1&& first, U2&& second) : First(Forward<U1>(first)), 
            Second(Forward<U2>(second)) {}

        /// @brief Move constructor - moves from pair of the same types
        /// @param other Pair to move from
        /// @since C++11
        __WSTL_CONSTEXPR14__ Pair(Pair<T1, T2>&& other) : First(Move(other.First)), 
            Second(Move(other.Second)) {}

        /// @brief Templated move constructor - moves from pair of potentially different types
        /// @param other Pair to move from
        /// @since C++11
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair(Pair<U1, U2>&& other) : First(Move(other.First)), 
            Second(Move(other.Second)) {}
        #endif

        /// @brief Swaps content of two pairs
        /// @param other Pair to swap contents with
        __WSTL_CONSTEXPR14__ void Swap(Pair<T1, T2>& other) {
            wstl::Swap(First, other.First);
            wstl::Swap(Second, other.Second);
        }
        
        /// @brief Copy assignment operator - with the same types
        /// @param other Pair to assign from
        __WSTL_CONSTEXPR14__ Pair<T1, T2>& operator=(const Pair<T1, T2>& other) {
            First = other.First;
            Second = other.Second;
            return *this;
        }   

        /// @brief Copy assignment operator - with potentially different types
        /// @tparam U1 Type of the first object
        /// @tparam U2 Type of the second object
        /// @param other Pair to assign from
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair<U1, U2>& operator=(const Pair<U1, U2>& other) {
            First = other.First;
            Second = other.Second;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator - assigns using move semantics with the same types
        /// @param other Pair to move from
        /// @since C++11
        __WSTL_CONSTEXPR14__ Pair<T1, T2>& operator=(const Pair<T1, T2>&& other) {
            First = Forward(other.First);
            Second = Forward(other.Second);
            return *this;
        }

        /// @brief Templated move assigment operator - assigns using move semantics
        /// with potentially different types  
        /// @tparam U1 Type of the first object
        /// @tparam U2 Type of the second object
        /// @param other Pair to move from
        /// @since C++11
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair<U1, U2>& operator=(const Pair<U1, U2>&& other) {
            First = Forward(other.First);
            Second = Forward(other.Second);
            return *this;
        }
        #endif
    };

    // Tuple size specialization

    template<typename T1, typename T2>
    class TupleSize<Pair<T1, T2> > : public IntegralConstant<size_t, 2> {};

    // Tuple element specialization

    template<size_t Index, typename T1, typename T2>
    class TupleElement<Index, Pair<T1, T2> > {
    public:
        StaticAssert(Index < 2, "wstl::Pair has only 2 elements!");
    };

    template<typename T1, typename T2>
    class TupleElement<0, Pair<T1, T2> > {
    public:
        typedef T1 Type;
    };

    template<typename T1, typename T2>
    class TupleElement<1, Pair<T1, T2> > {
    public:
        typedef T2 Type;
    };

    // Get (for Pair)

    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    /// @brief Gets an element from pair
    /// @tparam Index Index of element
    /// @param p Pair from which to get the element
    /// @return The element at the specified index
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair/get
    inline typename TupleElement<Index, Pair<T1, T2> >::Type& Get(Pair<T1, T2>& p) __WSTL_NOEXCEPT__ {
        if(Index == 0) return p.First;
        else return p.Second;
    }

    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    /// @copydoc Get(Pair<T1, T2>&)
    inline const typename TupleElement<Index, Pair<T1, T2> >::Type& Get(const Pair<T1, T2>& p) __WSTL_NOEXCEPT__ {
        if(Index == 0) return p.First;
        else return p.Second;
    }

    #ifdef __WSTL_CXX11__
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    /// @copydoc Get(Pair<T1, T2>&)
    inline typename TupleElement<Index, Pair<T1, T2>>::Type&& Get(Pair<T1, T2>&& p) __WSTL_NOEXCEPT__ {
        if(Index == 0) return Move(p.First);
        else return Move(p.Second);
    }

    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    /// @copydoc Get(Pair<T1, T2>&)
    inline const typename TupleElement<Index, Pair<T1, T2>>::Type&& Get(const Pair<T1, T2>&& p) __WSTL_NOEXCEPT__ {
        if(Index == 0) return Move(p.Second);
        else return Move(p.Second);
    }
    #endif

    // Swap specialization

    template<typename T1, typename T2>
    /// @brief Swaps content of two pairs
    /// @param a First pair
    /// @param b Second pair
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair/swap2
    __WSTL_CONSTEXPR14__
    inline void Swap(Pair<T1, T2>& a, Pair<T1, T2>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Comparison operators for Pair

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline bool operator==(const Pair<T1, T2>& a, const Pair<T1, T2>& b) {
        return (a.First == b.First) && !(a.Second < b.Second) && !(a.Second > b.Second);
    }

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline bool operator!=(const Pair<T1, T2>& a, const Pair<T1, T2>& b) {
        return !(a == b);
    }

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline bool operator<(const Pair<T1, T2>& a, const Pair<T1, T2>& b) {
        return (a.First < b.First) || (!(b.First < a.First) && (a.Second < b.Second));
    }

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline bool operator>(const Pair<T1, T2>& a, const Pair<T1, T2>& b) {
        return (b < a);
    }

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline bool operator<=(const Pair<T1, T2>& a, const Pair<T1, T2>& b) {
        return !(b < a);
    }

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline bool operator>=(const Pair<T1, T2>& a, const Pair<T1, T2>& b) {
        return !(a < b);
    }

    // Make pair

    #ifdef __WSTL_CXX11__
    /// @brief Makes a pair out of elements
    /// @param a First element
    /// @param b Second element
    /// @return Pair of those two elements
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair/make_pair
    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline Pair<T1, T2> MakePair(T1&& a, T2&& b) {
        return Pair<T1, T2>(Forward<T1>(a), Forward<T2>(b));
    }
    #else
    /// @brief Makes a pair out of elements
    /// @param a First element
    /// @param b Second element
    /// @return Pair of those two elements
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair/make_pair
    template<typename T1, typename T2>
    inline Pair<T1, T2> MakePair(T1 a, T2 b) {
        return Pair<T1, T2>(a, b);
    }
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
    class IntegerSequence {
    public:
        StaticAssert(IsIntegral<T>::Value, "Integral types only!");

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
        class __MakeIndexSequence {
        public:
            typedef typename __MakeIndexSequence<N - 1, N - 1, Indices...>::Type Type;
        };

        template<size_t... Indices>
        class __MakeIndexSequence<0, Indices...> {
        public:
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