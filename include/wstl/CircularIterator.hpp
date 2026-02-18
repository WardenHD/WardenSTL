// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s circular_iterator.h
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been modified by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_CIRCULARITERATOR_HPP__
#define __WSTL_CIRCULARITERATOR_HPP__

#include "private/Platform.hpp"
#include "Numeric.hpp"
#include "Iterator.hpp"


namespace wstl {
    namespace __private {
        template<typename T>
        class __CircularIteratorCommon : public Iterator<typename IteratorTraits<T>::IteratorCategory,
        typename IteratorTraits<T>::ValueType> {
        public:
            typedef typename IteratorTraits<T>::ValueType ValueType;
            typedef typename IteratorTraits<T>::IteratorCategory IteratorCategory;
            typedef typename IteratorTraits<T>::ReferenceType ReferenceType;
            typedef typename IteratorTraits<T>::PointerType PointerType;
            typedef typename IteratorTraits<T>::DifferenceType DifferenceType;

            __WSTL_CONSTEXPR14__ __CircularIteratorCommon() : m_Begin(T()), m_Current(T()),
                m_End(T()) {}

            __WSTL_CONSTEXPR14__ __CircularIteratorCommon(T begin, T end, T start) :
                m_Begin(begin), m_End(end), m_Current(start) {}

            __WSTL_CONSTEXPR14__ __CircularIteratorCommon(const __CircularIteratorCommon& other) :
                m_Begin(other.Begin()), m_End(other.End()), m_Current(other.Current()) {}

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIteratorCommon(const __CircularIteratorCommon<U>& other) :
                m_Begin(other.Begin()), m_End(other.End()), m_Current(other.Current()) {}

            __WSTL_CONSTEXPR14__ __CircularIteratorCommon& operator=(const __CircularIteratorCommon& other) {
                m_Begin = other.m_Begin;
                m_End = other.m_End;
                m_Current = other.m_Current;
                return *this;
            }

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIteratorCommon& operator=(const __CircularIteratorCommon<U>& other) {
                m_Begin = other.m_Begin;
                m_End = other.m_End;
                m_Current = other.m_Current;
                return *this;
            }

            /// @brief Returns the iterator pointing to the beginning
            __WSTL_CONSTEXPR14__ T Begin() const {
                return m_Begin;
            }

            /// @brief Returns the iterator pointing to the end
            __WSTL_CONSTEXPR14__ T End() const {
                return m_End;
            }

            /// @brief Returns the distance between the beginning and end iterators
            __WSTL_CONSTEXPR14__ size_t Size() const {
                return Distance(m_Begin, m_End);
            }

            /// @brief Checks whether the current iterator reached the end
            __WSTL_CONSTEXPR14__ bool Empty() const {
                return m_Begin == m_End;
            }

            /// @brief Deference operator
            __WSTL_CONSTEXPR14__ ReferenceType operator*() const {
                return *m_Current;
            }

            /// @brief Arrow operator
            __WSTL_CONSTEXPR14__ T operator->() const {
                return m_Current;
            }

            /// @brief Conversion operator to the underlying type
            __WSTL_CONSTEXPR14__ operator T() const {
                return m_Current;
            }

            /// @brief Returns the current iterator
            __WSTL_CONSTEXPR14__ T Current() const {
                return m_Current;
            }

        protected:
            T m_Begin;
            T m_End;
            T m_Current;
        };

        template<typename T, typename Category = typename IteratorTraits<T>::IteratorCategory>
        class __CircularIterator {
            WSTL_STATIC_ASSERT((IsSame<Category, InputIteratorTag>::Value), "InputIteratorCategory is not supported by CircularIterator");
            WSTL_STATIC_ASSERT((IsSame<Category, OutputIteratorTag>::Value), "OutputIteratorCategory is not supported by CircularIterator");
        };

        // Specialization for ForwardIterator

        template<typename T>
        class __CircularIterator<T, ForwardIteratorTag> : public __CircularIteratorCommon<T> {
        private:
            typedef __CircularIteratorCommon<T> CommonType;

        public: 
            typedef typename CommonType::ValueType ValueType;
            typedef typename CommonType::IteratorCategory IteratorCategory;
            typedef typename CommonType::ReferenceType ReferenceType;
            typedef typename CommonType::PointerType PointerType;
            typedef typename CommonType::DifferenceType DifferenceType;

            using CommonType::operator=;
            
            __WSTL_CONSTEXPR14__ __CircularIterator() : CommonType() {}

            __WSTL_CONSTEXPR14__ __CircularIterator(T begin, T end) : CommonType(begin, end, begin) {}

            __WSTL_CONSTEXPR14__ __CircularIterator(T begin, T end, T start) : 
                CommonType(begin, end, start) {}

            __WSTL_CONSTEXPR14__ __CircularIterator(const __CircularIterator& other) :
                CommonType(other) {}

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIterator(const __CircularIterator<U>& other) :
                CommonType(other) {}

            __WSTL_CONSTEXPR14__ __CircularIterator operator=(const __CircularIterator& other) {
                CommonType::operator=(other);
                return *this;
            }

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIterator operator=(const __CircularIterator<U>& other) {
                CommonType::operator=(other);
                return *this;
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator++() {
                if(++this->m_Current == this->m_End) this->m_Current = this->m_Begin;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            __WSTL_CONSTEXPR14__ __CircularIterator operator++(int) {
                __CircularIterator original(*this);
                ++(*this);
                return original;
            }
        };

        // Specialization for BidirectionalIterator

        template<typename T>
        class __CircularIterator<T, BidirectionalIteratorTag> : public __CircularIteratorCommon<T> {
        private:
            typedef __CircularIteratorCommon<T> CommonType;

        public: 
            typedef typename CommonType::ValueType ValueType;
            typedef typename CommonType::IteratorCategory IteratorCategory;
            typedef typename CommonType::ReferenceType ReferenceType;
            typedef typename CommonType::PointerType PointerType;
            typedef typename CommonType::DifferenceType DifferenceType;

            using CommonType::operator=;

            __WSTL_CONSTEXPR14__ __CircularIterator() : CommonType() {}

            __WSTL_CONSTEXPR14__ __CircularIterator(T begin, T end) : CommonType(begin, end, begin) {}

            __WSTL_CONSTEXPR14__ __CircularIterator(T begin, T end, T start) : 
                CommonType(begin, end, start) {}

            __WSTL_CONSTEXPR14__ __CircularIterator(const __CircularIterator& other) :
                CommonType(other) {}

            __WSTL_CONSTEXPR14__ __CircularIterator operator=(const __CircularIterator& other) {
                CommonType::operator=(other);
                return *this;
            }

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIterator operator=(const __CircularIterator<U>& other) {
                CommonType::operator=(other);
                return *this;
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator++() {
                if(++this->m_Current == this->m_End) this->m_Current = this->m_Begin;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            __WSTL_CONSTEXPR14__ __CircularIterator operator++(int) {
                __CircularIterator original(*this);
                ++(*this);
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator--() {
                if(this->m_Current == this->m_Begin) {
                    ReverseIterator<T> reverse(this->m_End);
                    ++reverse;
                    this->m_Current = reverse.Base();
                }
                else --this->m_Current;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            __WSTL_CONSTEXPR14__ __CircularIterator operator--(int) {
                __CircularIterator original(*this);
                --(*this);
                return original;
            }
        };

        // Specialization for RandomAccessIterator

        template<typename T>
        class __CircularIterator<T, RandomAccessIteratorTag> : public __CircularIteratorCommon<T> {
        private:
            typedef __CircularIteratorCommon<T> CommonType;

        public: 
            typedef typename CommonType::ValueType ValueType;
            typedef typename CommonType::IteratorCategory IteratorCategory;
            typedef typename CommonType::ReferenceType ReferenceType;
            typedef typename CommonType::PointerType PointerType;
            typedef typename CommonType::DifferenceType DifferenceType;
            
            using CommonType::operator=;
            
            __WSTL_CONSTEXPR14__ __CircularIterator() : CommonType() {}

            __WSTL_CONSTEXPR14__ __CircularIterator(T begin, T end) : CommonType(begin, end, begin) {}

            __WSTL_CONSTEXPR14__ __CircularIterator(T begin, T end, T start) : 
                CommonType(begin, end, start) {}

            __WSTL_CONSTEXPR14__ __CircularIterator(const __CircularIterator& other) :
                CommonType(other) {}

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIterator(const __CircularIterator<U>& other) :
                CommonType(other) {}
            
            __WSTL_CONSTEXPR14__ __CircularIterator operator=(const __CircularIterator& other) {
                CommonType::operator=(other);
                return *this;
            }

            template<typename U>
            __WSTL_CONSTEXPR14__ __CircularIterator operator=(const __CircularIterator<U>& other) {
                CommonType::operator=(other);
                return *this;
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator++() {
                if(++this->m_Current == this->m_End) this->m_Current = this->m_Begin;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            __WSTL_CONSTEXPR14__ __CircularIterator operator++(int) {
                __CircularIterator original(*this);
                ++(*this);
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator--() {
                if(this->m_Current == this->m_Begin) {
                    ReverseIterator<T> reverse(this->m_End);
                    ++reverse;
                    this->m_Current = reverse.Base();
                }
                else --this->m_Current;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            __WSTL_CONSTEXPR14__ __CircularIterator operator--(int) {
                __CircularIterator original(*this);
                --(*this);
                return original;
            }

            /// @brief Addition operator - moves the iterator forward by a given offset
            /// @param offset The offset to add (negative for backward movement)
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator+=(DifferenceType offset) {
                const DifferenceType length = DifferenceType(this->Size());
                DifferenceType position = Distance(this->m_Begin, this->m_Current);

                DifferenceType newPosition = (position + offset) % length;
                if (newPosition < 0) newPosition += length;

                this->m_Current = this->m_Begin + newPosition;
                return *this;
            }

            /// @brief Subtraction operator - moves the iterator backwards by a given offset
            /// @param offset The offset to subtract (negative for forward movement)
            /// @return Reference to the updated iterator
            __WSTL_CONSTEXPR14__ __CircularIterator& operator-=(DifferenceType offset) {
                return operator+=(-offset);
            }
        };
    }

    /// @brief A wrapper class for iterators that allows circular iteration
    /// @tparam T Type of the underlying iterator 
    /// @note Input and output iterators are not supported
    /// @ingroup iterator
    template<typename T>
    class CircularIterator __WSTL_FINAL__ : public __private::__CircularIterator<T, 
    typename IteratorTraits<T>::IteratorCategory> {
    private:
        typedef typename __private::__CircularIterator<T, typename IteratorTraits<T>::IteratorCategory> InternalType;

    public: 
        typedef typename InternalType::ValueType ValueType;
        typedef typename InternalType::IteratorCategory IteratorCategory;
        typedef typename InternalType::ReferenceType ReferenceType;
        typedef typename InternalType::PointerType PointerType;
        typedef typename InternalType::DifferenceType DifferenceType;

        using InternalType::operator=;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ CircularIterator() : InternalType() {}

        /// @brief Parameterized constructor
        /// @param begin Iterator pointing to the beginning
        /// @param end Iterator pointing to the end
        __WSTL_CONSTEXPR14__ CircularIterator(T begin, T end) : InternalType(begin, end, begin) {}

        /// @brief Parameterized constructor with a starting point
        /// @param begin Iterator pointing to the beginning
        /// @param end Iterator pointing to the end
        /// @param start Iterator pointing to the starting point
        __WSTL_CONSTEXPR14__ CircularIterator(T begin, T end, T start) : 
            InternalType(begin, end, start) {}

        /// @brief Copy constructor
        /// @param other Circular iterator to copy from
        __WSTL_CONSTEXPR14__ CircularIterator(const CircularIterator& other) :
            InternalType(other) {}
        
        /// @brief Templated copy constructor
        /// @param other Circular iterator to copy from
        template<typename U>
        __WSTL_CONSTEXPR14__ CircularIterator(const CircularIterator<U>& other) :
            InternalType(other) {}

        /// @brief Assignment operator
        /// @param other Circular iterator to assign from
        __WSTL_CONSTEXPR14__ CircularIterator& operator=(const CircularIterator& other) {
            InternalType::operator=(other);
            return *this;
        }

        /// @brief Templated assignment operator
        /// @param other Circular iterator to assign from
        template<typename U>
        __WSTL_CONSTEXPR14__ CircularIterator& operator=(const CircularIterator<U>& other) {
            InternalType::operator=(other);
            return *this;
        }
    };

    // Addition operator

    /// @brief Adds an offset to a circular iterator and returns a new circular iterator
    /// @param x Circular iterator to add offset to
    /// @param offset The offset to add (negative for backward movement)    
    /// @return A new circular iterator advanced by `offset` positions
    /// @ingroup iterator
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    CircularIterator<T> operator+(const CircularIterator<T>& x, typename IteratorTraits<T>::DifferenceType offset) {
        CircularIterator<T> result(x);
        result += offset;
        return result;
    }

    /// @brief Adds an offset to a circular iterator and returns a new circular iterator
    /// @param offset The offset to add (negative for backward movement)  
    /// @param x Circular iterator to add offset to  
    /// @return A new circular iterator advanced by `offset` positions
    /// @ingroup iterator
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    CircularIterator<T> operator+(typename IteratorTraits<T>::DifferenceType offset, const CircularIterator<T>& x) {
        CircularIterator<T> result(x);
        result += offset;
        return result;
    }

    // Offset operator

    /// @brief Subtracts an offset from a circular iterator and returns a new circular iterator
    /// @param x Circular iterator to subtract offset from
    /// @param offset The offset to subtract (negative for forward movement)
    /// @return A new circular iterator moved by `offset` positions
    /// @ingroup iterator
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    CircularIterator<T> operator-(const CircularIterator<T>& x, typename IteratorTraits<T>::DifferenceType offset) {
        CircularIterator<T> result(x);
        result -= offset;
        return result;
    }

    // Difference operator

    /// @brief Calculates the difference between two circular iterators, works only when the iterators belong to the same range
    /// @param a First circular iterator
    /// @param b Second circular iterator
    /// @return The difference between the two iterators, or undefined value if the iterators do not belong to the same range
    /// @ingroup iterator
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    typename IteratorTraits<T>::DifferenceType operator-(CircularIterator<T>& a, CircularIterator<T>& b) {
        return T(a) - T(b);
    }

    // Comparison operators

    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator==(const CircularIterator<T>& a, const CircularIterator<T>& b) {
        return T(a) == T(b);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator==(const CircularIterator<T>& a, T b) {
        return T(a) == b;
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator==(T a, const CircularIterator<T>& b) {
        return a == T(b);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator!=(const CircularIterator<T>& a, const CircularIterator<T>& b) {
        return !(a == b);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator!=(const CircularIterator<T>& a, T b) {
        return !(a == b);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator!=(T a, const CircularIterator<T>& b) {
        return !(a == b);
    }

    /// @brief Checks if one iterator is less than another, only makes sense when iterators belong to the same range
    /// @param a Iterator to check
    /// @param b Iterator to check against
    /// @return True if the iterators are equal, false otherwise
    /// @ingroup iterator
    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator<(const CircularIterator<T>& a, const CircularIterator<T>& b) {
        return T(a) < T(b);
    }

    /// @brief Checks if one iterator is less than or equal to another, only makes sense when iterators belong to the same range
    /// @param a Iterator to check
    /// @param b Iterator to check against
    /// @return True if the first iterator is less than or equal to the second, false otherwise
    /// @ingroup iterator
    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator<=(const CircularIterator<T>& a, const CircularIterator<T>& b) {
        return !(b < a);
    }

    /// @brief Checks if one iterator is greater than another, only makes sense when iterators belong to the same range
    /// @param a Iterator to check
    /// @param b Iterator to check against
    /// @return True if the first iterator is greater than the second, false otherwise
    /// @ingroup iterator
    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator>(const CircularIterator<T>& a, const CircularIterator<T>& b) {
        return b < a;
    }

    /// @brief Checks if one iterator is greater than or equal to another, only makes sense when iterators belong to the same range
    /// @param a Iterator to check
    /// @param b Iterator to check against
    /// @return True if the first iterator is greater than or equal to the second, false otherwise
    /// @ingroup iterator
    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool operator>=(const CircularIterator<T>& a, const CircularIterator<T>& b) {
        return !(a < b);
    }
}

#endif