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
#include "Tuple.hpp"


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
    __WSTL_CONSTEXPR14__ T Exchange(T& object, U&& newValue) __WSTL_NOEXCEPT_EXPR__(IsNothrowMoveConstructible<T>::Value && IsNothrowAssignable<T&, U>::Value) {
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

    // Piecewise construct

    #ifdef __WSTL_CXX11__
    /// @brief Tag type that is used to disambiguate between different functions that take two tuple arguments
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/piecewise_construct
    /// @since C++11
    struct PiecewiseConstructType {
        explicit PiecewiseConstructType() = default;
    };

    /// @brief Tag object used to disambiguate between different functions that take two tuple arguments
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/piecewise_construct
    /// @since C++11
    constexpr PiecewiseConstructType PiecewiseConstruct{};
    #endif

    // Pair

    /// @brief Stores two objects as a single unit
    /// @tparam T1 Type of the first object
    /// @tparam T2 Type of the second object
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair
    template<typename T1, typename T2>
    struct Pair {
        /// @brief Type alias for the first object
        typedef T1 FirstType;
        /// @brief Type alias for the second object
        typedef T2 SecondType;

        FirstType First;
        SecondType Second;

        /// @brief Default constructor - initializes both objects to their default-constructed values 
        __WSTL_CONSTEXPR__ Pair() : First(), Second() {}

        /// @brief Parameterized constructor
        /// @param first Value of the first object
        /// @param second Value of the second object
        __WSTL_CONSTEXPR14__ Pair(const T1& first, const T2& second) : First(first), Second(second) {}

        /// @brief Copy constructor - copies from pair of the same types
        /// @param other Pair to copy from
        __WSTL_CONSTEXPR14__ Pair(const Pair& other) : First(other.First), Second(other.Second) {}

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
        __WSTL_CONSTEXPR14__ Pair(Pair&& other) : First(Move(other.First)), 
            Second(Move(other.Second)) {}

        /// @brief Templated move constructor - moves from pair of potentially different types
        /// @param other Pair to move from
        /// @since C++11
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair(Pair<U1, U2>&& other) : First(Forward<U1>(other.First)), 
            Second(Forward<U2>(other.Second)) {}

        /// @brief Piecewise constructor - constructs members from tuples of arguments
        /// @param firstArgs Tuple of arguments for the first member
        /// @param secondArgs Tuple of arguments for the second member
        /// @since C++11
        template<typename... Args1, typename... Args2>
        __WSTL_CONSTEXPR14__ Pair(PiecewiseConstructType, Tuple<Args1...> firstArgs, Tuple<Args2...> secondArgs) : 
            Pair(firstArgs, secondArgs, IndexSequenceFor<Args1...>{}, IndexSequenceFor<Args2...>{}) {}
        #endif

        /// @brief Swaps content of two pairs
        /// @param other Pair to swap contents with
        __WSTL_CONSTEXPR14__ void Swap(Pair& other) __WSTL_NOEXCEPT_EXPR__(noexcept(wstl::Swap(First, other.First)) && noexcept(wstl::Swap(Second, other.Second))) {
            wstl::Swap(First, other.First);
            wstl::Swap(Second, other.Second);
        }
        
        /// @brief Copy assignment operator - with the same types
        /// @param other Pair to assign from
        __WSTL_CONSTEXPR14__ Pair& operator=(const Pair& other) {
            if(this != &other) {
                First = other.First;
                Second = other.Second;
            }
            
            return *this;
        }   

        /// @brief Copy assignment operator - with potentially different types
        /// @tparam U1 Type of the first object
        /// @tparam U2 Type of the second object
        /// @param other Pair to assign from
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair& operator=(const Pair<U1, U2>& other) {
            First = other.First;
            Second = other.Second;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator - assigns using move semantics with the same types
        /// @param other Pair to move from
        /// @since C++11
        __WSTL_CONSTEXPR14__ Pair& operator=(const Pair&& other) __WSTL_NOEXCEPT_EXPR__(IsNothrowMoveAssignable<T1>::Value && IsNothrowMoveAssignable<T2>::Value) {
            if(this != &other) {
                First = Move(other.First);
                Second = Move(other.Second);
            }

            return *this;
        }

        /// @brief Templated move assigment operator - assigns using move semantics
        /// with potentially different types  
        /// @tparam U1 Type of the first object
        /// @tparam U2 Type of the second object
        /// @param other Pair to move from
        /// @since C++11
        template<typename U1, typename U2>
        __WSTL_CONSTEXPR14__ Pair& operator=(const Pair<U1, U2>&& other) {
            First = Move(other.First);
            Second = Move(other.Second);
            return *this;
        }
        #endif
    
    private:
        template<typename... Args1, size_t... Indices1, typename... Args2, size_t... Indices2>
        __WSTL_CONSTEXPR14__ Pair(Tuple<Args1...>& firstArgs, Tuple<Args2...>& secondArgs, 
            IndexSequence<Indices1...>, IndexSequence<Indices2...>) : First(Forward<Args1>(Get<Indices1>(firstArgs))...), Second(Forward<Args2>(Get<Indices2>(secondArgs))...) {}
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T1, typename T2>
    Pair(T1, T2) -> Pair<T1, T2>;
    #endif

    // Tuple size specialization

    template<typename T1, typename T2>
    struct TupleSize<Pair<T1, T2> > : IntegralConstant<size_t, 2> {};

    // Tuple element specialization

    template<size_t Index, typename T1, typename T2>
    struct TupleElement<Index, Pair<T1, T2> > {
        WSTL_STATIC_ASSERT((Index < 2), "wstl::Pair has only 2 elements!");
    };

    template<typename T1, typename T2>
    struct TupleElement<0, Pair<T1, T2> > { typedef T1 Type; };

    template<typename T1, typename T2>
    struct TupleElement<1, Pair<T1, T2> > { typedef T2 Type; };

    // Get (for Pair)

    /// @brief Gets an element from pair
    /// @tparam Index Index of element
    /// @param pair Pair from which to get the element
    /// @return The element at the specified index
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/pair/get
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline typename EnableIf<(Index == 0), typename TupleElement<Index, Pair<T1, T2> >::Type&>::Type Get(Pair<T1, T2>& pair) __WSTL_NOEXCEPT__ {
        return pair.First;
    }

    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline typename EnableIf<(Index == 1), typename TupleElement<Index, Pair<T1, T2> >::Type&>::Type Get(Pair<T1, T2>& pair) __WSTL_NOEXCEPT__ {
        return pair.Second;
    }

    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline typename EnableIf<(Index == 0), const typename TupleElement<Index, Pair<T1, T2> >::Type&>::Type Get(const Pair<T1, T2>& pair) __WSTL_NOEXCEPT__ {
        return pair.First;
    }

    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__ 
    inline typename EnableIf<(Index == 1), const typename TupleElement<Index, Pair<T1, T2> >::Type&>::Type Get(const Pair<T1, T2>& pair) __WSTL_NOEXCEPT__ {
        return pair.Second;
    }

    #ifdef __WSTL_CXX11__
    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 0), TupleElementType<Index, Pair<T1, T2>>&&> Get(Pair<T1, T2>&& pair) __WSTL_NOEXCEPT__ {
        return Move(pair.First);
    }

    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 1), TupleElementType<Index, Pair<T1, T2>>&&> Get(Pair<T1, T2>&& pair) __WSTL_NOEXCEPT__ {
        return Move(pair.Second);
    }

    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 0), const TupleElementType<Index, Pair<T1, T2>>&&> Get(const Pair<T1, T2>&& pair) __WSTL_NOEXCEPT__ {
        return Move(pair.First);
    }

    /// @copydoc Get(Pair<T1, T2>&)
    template<size_t Index, typename T1, typename T2>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 1), const TupleElementType<Index, Pair<T1, T2>>&&> Get(const Pair<T1, T2>&& pair) __WSTL_NOEXCEPT__ {
        return Move(pair.Second);
    }
    #endif

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

    // In place

    /// @brief Tag type that is used for in-place construction
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/in_place
    struct InPlaceType {
        explicit __WSTL_CONSTEXPR__ InPlaceType() {};
    };

    #ifdef __WSTL_CXX14__
    /// @brief Tag object that is used for in-place construction
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/in_place
    __WSTL_INLINE_VARIABLE__  constexpr InPlaceType InPlace {};
    #endif

    /// @brief Tag type that is used for in-place construction for specified type
    /// @tparam T Type to construct
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/in_place
    template<typename T>
    struct InPlaceForTypeType {
        explicit __WSTL_CONSTEXPR__ InPlaceForTypeType() {};
    };

    #ifdef __WSTL_CXX14__
    /// @brief Tag object that is used for in-place construction for specified type
    /// @tparam T Type to construct
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/in_place
    template<typename T>
    __WSTL_INLINE_VARIABLE__ constexpr InPlaceForTypeType<T> InPlaceForType {};
    #endif

    /// @brief Tag type that is used for in-place construction for specified index
    /// @tparam Index Index to construct
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/in_place
    template<size_t Index>
    struct InPlaceForIndexType {
        explicit __WSTL_CONSTEXPR__ InPlaceForIndexType() {};
    };

    #ifdef __WSTL_CXX14__
    /// @brief Tag object that is used for in-place construction for specified index
    /// @tparam Index Index to construct
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/in_place
    template<size_t Index>
    __WSTL_INLINE_VARIABLE__ constexpr InPlaceForIndexType<Index> InPlaceForIndex {};
    #endif
}

#endif