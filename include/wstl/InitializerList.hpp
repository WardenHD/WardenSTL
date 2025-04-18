// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_INITIALIZERLIST_HPP__
#define __WSTL_INITIALIZERLIST_HPP__

#include "private/Platform.hpp"
#include "NullPointer.hpp"
#include <stddef.h>


#if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
namespace std {
    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
    template<typename T>
    class initializer_list {
    public:
        /// @brief Type of elements in the array
        typedef T ValueType;
        /// @brief Type of references to elements in the array
        typedef const T& ReferenceType;
        /// @brief Type of constant references to elements in the array
        typedef const T& ConstReferenceType;
        typedef size_t SizeType;

        /// @brief Type of the iterator
        typedef const T* Iterator;
        /// @brief Type of the constant iterator
        typedef const T* ConstIterator;

        /// @brief Returns the number of the elements in the list
        constexpr SizeType Size() const noexcept {
            return m_Length;
        }

        /// @brief Returns an iterator to the beginning of the list
        constexpr ConstIterator Begin() const noexcept {
            return m_First;
        }

        /// @brief Returns an iterator to the end of the list
        constexpr ConstIterator End() const noexcept {
            return m_First + m_Length;
        }

        /// @brief Default constructor
        constexpr initializer_list() noexcept : m_First(nullptr), m_Length(0) {}

    private:
        constexpr initializer_list(ConstIterator first, SizeType length) noexcept : 
            m_First(first), m_Length(length) {};

        ConstIterator m_First;
        SizeType m_Length;
    };

    #elif defined(__WSTL_MSVC__)
    template<typename T>
    class initializer_list {
    public:
        /// @brief Type of elements in the array
        typedef T ValueType;
        /// @brief Type of references to elements in the array
        typedef const T& ReferenceType;
        /// @brief Type of constant references to elements in the array
        typedef const T& ConstReferenceType;
        /// @brief Type of size of the array
        typedef size_t SizeType;

        /// @brief Type of the iterator
        typedef const T* Iterator;
        /// @brief Type of the constant iterator
        typedef const T* ConstIterator;

        /// @brief Returns the number of the elements in the list
        constexpr SizeType Size() const noexcept {
            return static_cast<SizeType>(m_Last - m_First);
        }

        /// @brief Returns an iterator to the beginning of the list
        constexpr ConstIterator Begin() const noexcept {
            return m_First;
        }

        /// @brief Returns an iterator to the end of the list
        constexpr ConstIterator End() const noexcept {
            return m_Last;
        }

        /// @brief Default constructor
        constexpr initializer_list() noexcept : m_First(nullptr), m_Last(nullptr) {}

    private:
        constexpr initializer_list(ConstIterator first, ConstIterator last) noexcept : 
            m_First(first), m_Last(last) {};

        ConstIterator m_First;
        ConstIterator m_Last;
    };
    
    #else
    #error No definition of initializer_list is currently available for your compiler
    #endif
}

namespace wstl {
    /// @brief A lightweight wrapper for an array of constant elements.
    /// This class provides a simple way to access a read-only array of elements,
    /// typically used for initializing containers or handling brace-enclosed lists.
    /// @tparam T Type of elements in the array
    /// @ingroup container
    /// @note Can be disabled by defining `__WSTL_NO_INITIALIZERLIST__`
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/initializer_list
    template<typename T>
    using InitializerList = std::initializer_list<T>;

    // Begin and End specializations

    /// @brief Returns a const pointer to the beginning of the initializer list
    /// @param x The initializer list whose beginning is returned
    /// @return A const pointer to the beginning of the initializer list
    /// @ingroup container
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/initializer_list/begin2
    template<typename T>
    constexpr const T* Begin(InitializerList<T> x) noexcept {
        return x.Begin();
    }

    /// @brief Returns a const pointer to the end of the initializer list
    /// @param x The initializer list whose end is returned
    /// @return A const pointer to the end of the initializer list
    /// @ingroup container
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/initializer_list/end2
    template<typename T>
    constexpr const T* End(InitializerList<T> x) noexcept {
        return x.End();
    }

    /// @brief Checks if an initializer list is empty
    /// @param x The initializer list to check
    /// @return True if the initializer list is empty, false otherwise
    /// @ingroup container
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/iterator/empty
    template<typename T>
    constexpr bool Empty(InitializerList<T> x) noexcept {
        return x.Size() == 0;
    }

    /// @brief Retrieves a const pointer to the underlying data of an initializer list
    /// @param x The initializer list whose underlying data is retrieved
    /// @return Const pointer to the underlying data storage of the initializer list
    /// @ingroup container
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/iterator/data
    template<typename T>
    constexpr const T* Data(InitializerList<T> x) noexcept {
        return x.Begin();
    }
}
#endif

#endif