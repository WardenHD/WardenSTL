// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_BOUNDEDITERATOR_HPP__
#define __WSTL_BOUNDEDITERATOR_HPP__

#include "private/Platform.hpp"
#include "Iterator.hpp"


namespace wstl {
    // Forward bounded iterator

    /// @brief A forward bounded iterator that iterates over a range of elements
    /// @tparam T Type of the underlying iterator
    /// @ingroup iterator
    template<typename T>
    class ForwardBoundedIterator : public Iterator<ForwardIteratorTag, T> {
    public:
        typedef typename IteratorTraits<T>::ValueType ValueType;
        typedef ForwardIteratorTag IteratorCategory;
        typedef typename IteratorTraits<T>::ReferenceType ReferenceType;
        typedef typename IteratorTraits<T>::PointerType PointerType;
        typedef typename MakeUnsigned<typename IteratorTraits<T>::DifferenceType>::Type DifferenceType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator() : m_Current(), m_End() {}

        /// @brief Parameterized constructor
        /// @param begin Iterator to beginning of the range
        /// @param end Iterator to end of the range
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(T begin, T end) : m_Current(begin), m_End(end) {}

        /// @brief Copy constructor
        /// @param other Another `ForwardBoundedIterator` to copy from
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(const ForwardBoundedIterator& other) : m_Current(other.m_Current), m_End(other.m_End) {}

        /// @brief Templated copy constructor - copies from forward bounded iterator of potentially different type
        /// @param other Another `ForwardBoundedIterator` to copy from
        template<typename U>
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(const ForwardBoundedIterator<U>& other) : m_Current(other.m_Current), m_End(other.m_End) {}

        /// @brief Copy assignment operator
        /// @param other Another `ForwardBoundedIterator` to copy from
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator=(const ForwardBoundedIterator& other) {
            if(this != &other) {
                m_Current = other.m_Current;
                m_End = other.m_End;
            }

            return *this;
        }

        /// @brief Templated copy assignment operator
        /// @param other Another `ForwardBoundedIterator` to copy from
        template<typename U>
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator=(const ForwardBoundedIterator<U>& other) {
            m_Current = other.m_Current;
            m_End = other.m_End;
            return *this;
        }

        /// @brief Dereference operator
        /// @return Reference to the current element
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator*() const { 
            return *m_Current; 
        }

        /// @brief Arrow operator - provides access to element's member functions or properties
        /// @return Pointer to the current element
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ PointerType operator->() const {
            return m_Current;
        }

        /// @brief Pre-increment operator - moves the iterator forward by one element
        /// @return Reference to the updated iterator
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator++() {
            if(m_Current != m_End - 1) ++m_Current;
            return *this;
        }

        /// @brief Post-increment operator - moves the iterator forward by one element
        /// @return Reference to the iterator before incrementing
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator operator++(int) {
            ForwardBoundedIterator original(*this);
            ++(*this);
            return original;
        }

    private:
        T m_Current;
        T m_End;
    };

    // Comparison operators

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator==(const ForwardBoundedIterator<U>& a, const ForwardBoundedIterator<U>& b) {
        return U(a) == U(b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator==(const ForwardBoundedIterator<U>& a, U b) {
        return U(a) == b;
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator==(U a, const ForwardBoundedIterator<U>& b) {
        return a == U(b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator!=(const ForwardBoundedIterator<U>& a, const ForwardBoundedIterator<U>& b) {
        return !(a == b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator!=(const ForwardBoundedIterator<U>& a, U b) {
        return !(a == b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator!=(U a, const ForwardBoundedIterator<U>& b) {
        return !(a == b);
    }
}

#endif