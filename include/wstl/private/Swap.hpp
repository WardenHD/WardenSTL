// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s swap utilities,
// licensed under the MIT License, with no modifications made.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_SWAP_HPP__
#define __WSTL_SWAP_HPP__

#include "Platform.hpp"
#include <stddef.h>

#ifdef __WSTL_CXX11__
#include "../TypeTraits.hpp"
#include "../StaticAssert.hpp"
#endif


namespace wstl {
    #ifdef __WSTL_CXX11__
    // Move
    
    template<typename T>
    /// @brief Converts an object to rvalue reference
    /// @param t Object to convert
    /// @return Rvalue reference to the object
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/move
    constexpr typename wstl::RemoveReference<T>::Type&& Move(T&& t) __WSTL_NOEXCEPT__ {
        return static_cast<typename RemoveReference<T>::Type&&>(t);
    }

    // Move if noexcept

    #ifdef __WSTL_EXCEPTIONS__
    template<typename T>
    /// @brief Converts an object to rvalue reference if the move constructor is noexcept
    /// @param t Object to convert
    /// @return Rvalue reference to the object if 
    /// @ingroup utility
    /// @since C++11. 
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/utility/move_if_noexcept
    constexpr EnableIfType<IsNothrowMoveConstructible<T>::Value || !IsCopyConstructible<T>::Value, T&&> MoveIfNoexcept(T& t) __WSTL_NOEXCEPT__ {
        return Move(t);
    }

    template<typename T>
    /// @copydoc MoveIfNoexcept(T&)
    constexpr EnableIfType<!IsNothrowMoveConstructible<T>::Value || IsCopyConstructible<T>::Value, const T&> MoveIfNoexcept(T& t) __WSTL_NOEXCEPT__ {
        return t;
    }
    #endif
    
    // Forward

    template<typename T>
    /// @brief Forwards an object to another function or context
    /// @param t Object to forward
    /// @return Forwarded object
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/forward
    constexpr T&& Forward(typename wstl::RemoveReference<T>::Type& t) __WSTL_NOEXCEPT__ {
        return static_cast<T&&>(t);
    }

    template<typename T>
    /// @copydoc Forward(wstl::RemoveReference<T>::Type&)
    constexpr T&& Forward(typename wstl::RemoveReference<T>::Type&& t) __WSTL_NOEXCEPT__ {
        StaticAssert(!wstl::IsLValueReference<T>::Value, "Invalid conversion rvalue to lvalue");
        return static_cast<T&&>(t);
    }

    // Forward like

    // T is const & lvalue

    /// @brief Forwards an object with a value category and constness similar to `T`
    /// @tparam T Type to mimick
    /// @tparam U Type of the object to be forwarded
    /// @param u Object to forward
    /// @return Forwarded object
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/forward_like
    template<typename T, typename U>
    constexpr EnableIfType<IsConst<RemoveReferenceType<T>>::Value && IsLValueReference<T>::Value, const RemoveReferenceType<T>&> ForwardLike(U&& u) {
        return static_cast<const RemoveReferenceType<T>&>(u);
    }

    // T is const & rvalue

    /// @copydoc ForwardLike(U&&)
    template<typename T, typename U>
    constexpr EnableIfType<IsConst<RemoveReferenceType<T>>::Value && !IsLValueReference<T>::Value, const RemoveReferenceType<T>&&> ForwardLike(U&& u) {
        return static_cast<const RemoveReferenceType<T>&&>(u);
    }

    // T is not const & lvalue

    /// @copydoc ForwardLike(U&&)
    template<typename T, typename U>
    constexpr EnableIfType<IsConst<RemoveReferenceType<T>>::Value && IsLValueReference<T>::Value, RemoveReferenceType<T>&> ForwardLike(U&& u) {
        return static_cast<RemoveReferenceType<T>&>(u);
    }

    // T is not const & rvalue

    /// @copydoc ForwardLike(U&&)
    template<typename T, typename U>
    constexpr EnableIfType<!IsConst<RemoveReferenceType<T>>::Value && !IsLValueReference<T>::Value, RemoveReferenceType<T>&&> ForwardLike(U&& u) {
        return static_cast<RemoveReferenceType<T>&&>(u);
    }

    /// @brief Determines the type resulting from forwarding an object like `T` with a value of type `U`
    /// @tparam T Type to mimick
    /// @tparam U Type of the object to be forwarded
    /// @ingroup utility
    /// @since C++11
    template<typename T, typename U>
    using ForwardLikeType = decltype(ForwardLike<T>(DeclareValue<U&>()));
    #endif


    // Swap

    template<typename T>
    /// @brief Swaps the values of two objects
    /// @tparam T Type of the objects to swap
    /// @param a First object to swap
    /// @param b Second object to swap
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/swap
    __WSTL_CONSTEXPR14__ void Swap(T& a, T& b) {
        T temp = __WSTL_MOVE__(a);
        a = __WSTL_MOVE__(b);
        b = __WSTL_MOVE__(temp);
    }

    template<typename T, size_t N>
    /// @brief Swaps the values of two arrays
    /// @tparam T Type of the elements in the arrays
    /// @tparam N Size of the arrays
    /// @param a First array to swap
    /// @param b Second array to swap
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/swap
    __WSTL_CONSTEXPR14__ void Swap(T (&a)[N], T (&b)[N]) {
        for (size_t i = 0; i < N; ++i) Swap(a[i], b[i]);
    }
}

#endif