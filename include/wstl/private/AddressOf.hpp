// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s address_of implementation,
// licensed under the MIT License, with minor modifications made for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.


#ifndef __WSTL_ADDRESSOF_HPP__
#define __WSTL_ADDRESSOF_HPP__

#include "Platform.hpp"
#include "../TypeTraits.hpp"


namespace wstl {
    // Address of

    /// @brief Returns the address of the given object
    /// @param object The object to get the address of
    /// @return A pointer to the object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/address_of
    template<typename T>
    __WSTL_CONSTEXPR__ typename EnableIf<!IsSame<T, NullPointerType>::Value, T>::Type* AddressOf(T& object) __WSTL_NOEXCEPT__ {
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(object)));
    }

    /// @brief Returns the address of the given object, const rvalues are deleted
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/address_of
    template<typename T>
    __WSTL_CONSTEXPR__ const T* AddressOf(const T&&) __WSTL_DELETE__;

    // To address

    /// @brief Converts a pointer to an object to a pointer to its address
    /// @param pointer The pointer to convert
    /// @return A pointer to the address of the object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/to_address
    template<typename T>
    __WSTL_CONSTEXPR__ T* ToAddress(T* pointer) {
        WSTL_STATIC_ASSERT(!IsFunction<T>::Value, "Function pointers are not allowed");
        return pointer;
    }

    /// @brief Converts an iterator to a pointer to its address
    /// @param iterator The iterator to convert
    /// @return A pointer to the address of the object pointed to by the iterator
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/to_address
    template<typename Iterator>
    __WSTL_CONSTEXPR__ typename Iterator::PointerType ToAddress(const Iterator& iterator) {
        return ToAddress(iterator.operator->());
    }
}

#endif