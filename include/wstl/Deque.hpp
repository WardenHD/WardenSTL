// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s deque implementation,
// but it has been re-implemented with custom logic for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_DEQUE_HPP__
#define __WSTL_DEQUE_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "Iterator.hpp"
#include "CircularIterator.hpp"
#include "InitializerList.hpp"
#include "StandardExceptions.hpp"
#include "PlacementNew.hpp"
#include "Algorithm.hpp"


/// @defgroup deque Deque
/// @ingroup containers
/// @brief A double-ended queue container

namespace wstl {
    // Basic deque

    /// @brief A double-ended queue that supports pushing and popping elements from both ends
    /// @tparam Storage The storage type used by the deque
    /// @ingroup deque
    /// @see https://en.cppreference.com/w/cpp/container/deque
    template<typename Storage>
    class BasicDeque : public TypedContainerBase<Storage> {
    private:
        typedef TypedContainerBase<Storage> Base;

    public:
        WSTL_STATIC_ASSERT(!IsVoid<Storage>::Value, "Storage must be non-void");

        typedef typename Storage::ValueType ValueType;
        typedef typename Storage::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;

        typedef typename Base::StorageType StorageType;

        /// @brief Circular const iterator type for the deque
        class ConstIterator : public wstl::Iterator<RandomAccessIteratorTag, const ValueType> {
        public:
            typedef typename wstl::Iterator<RandomAccessIteratorTag, const ValueType>::ReferenceType ReferenceType;  
            typedef typename wstl::Iterator<RandomAccessIteratorTag, const ValueType>::PointerType PointerType;
            typedef typename wstl::Iterator<RandomAccessIteratorTag, const ValueType>::DifferenceType DifferenceType;

            friend class BasicDeque;

            /// @brief Default constructor
            ConstIterator() : m_Deque(), m_CurrentIndex() {}

            /// @brief Copy constructor
            /// @param other Const iterator to copy from
            ConstIterator(const ConstIterator& other) : m_Deque(other.m_Deque), 
                m_CurrentIndex(other.m_CurrentIndex) {}

            /// @brief Copy constructor
            /// @param other Iterator to copy from
            ConstIterator(const typename BasicDeque::Iterator& other) : 
                m_Deque(other.m_Deque), m_CurrentIndex(other.m_CurrentIndex) {}

            /// @brief Copy assignment operator
            /// @param other Const iterator to assign from
            ConstIterator& operator=(const ConstIterator& other) {
                m_Deque = other.m_Deque;
                m_CurrentIndex = other.m_CurrentIndex;
                return *this;
            }

            /// @brief Copy assignment operator
            /// @param other Iterator to assign from
            ConstIterator& operator=(const typename BasicDeque::Iterator& other) {
                m_Deque = other.m_Deque;
                m_CurrentIndex = other.m_CurrentIndex;
                return *this;
            }

            /// @brief Const dereference operator
            ReferenceType operator*() const {
                return m_Deque->m_Storage.Data[m_Deque->PhysicalIndex(m_CurrentIndex)];
            }

            /// @brief Const arrow operator
            PointerType operator->() const {
                return m_Deque->m_Storage.Data + m_Deque->PhysicalIndex(m_CurrentIndex);
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            ConstIterator& operator++() {
                ++m_CurrentIndex;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            ConstIterator operator++(int) {
                ConstIterator original(*this);
                ++m_CurrentIndex;
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            ConstIterator& operator--() {
                --m_CurrentIndex;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            ConstIterator operator--(int) {
                ConstIterator original(*this);
                --m_CurrentIndex;
                return original;
            }

            /// @brief Addition operator - moves the iterator forward by a given offset
            /// @param offset The offset to add (negative for backward movement)
            /// @return Reference to the updated iterator
            ConstIterator& operator+=(DifferenceType offset) {
                m_CurrentIndex += offset;
                return *this;
            }

            /// @brief Subtraction operator - moves the iterator backwards by a given offset
            /// @param offset The offset to subtract (negative for forward movement)
            /// @return Reference to the updated iterator
            ConstIterator& operator-=(DifferenceType offset) {
                return operator+=(-offset);
            }

            /// @brief Access operator - allows access to the element at the given index
            /// @param i Index of the element to access
            /// @return Reference to the element at the given index
            ReferenceType operator[](SizeType i) const {
                return m_Deque->m_Storage.Data[m_Deque->PhysicalIndex(i + m_CurrentIndex)];
            }

            /// @brief Addition operator - allows adding an offset to the iterator
            /// @param x The iterator to add the offset to
            /// @param offset The offset to add (negative for backward movement)
            /// @return A new iterator advanced by `offset` positions
            friend ConstIterator operator+(const ConstIterator& x, DifferenceType offset) {
                ConstIterator result(x);
                result += offset;
                return result;
            }

            /// @brief Addition operator - allows adding an offset to the iterator
            /// @param offset The offset to add (negative for backward movement)
            /// @param x The iterator to add the offset to
            /// @return A new iterator advanced by `offset` positions
            friend ConstIterator operator+(DifferenceType offset, const ConstIterator& x) {
                ConstIterator result(x);
                result += offset;
                return result;
            }
            
            /// @brief Subtraction operator - allows subtracting an offset from the iterator
            /// @param x The iterator to subtract the offset from
            /// @param offset The offset to subtract (negative for forward movement)
            /// @return A new iterator moved back by `offset` positions
            friend ConstIterator operator-(const ConstIterator& x, DifferenceType offset) {
                ConstIterator result(x);
                result -= offset;
                return result;
            }

            /// @brief Subtraction operator - allows subtracting two iterators, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return The difference in positions between the two iterators
            friend DifferenceType operator-(const ConstIterator& a, const ConstIterator& b) {
                const DifferenceType indexA = a.m_CurrentIndex;
                const DifferenceType indexB = b.m_CurrentIndex;

                return indexA - indexB;
            }

            /// @brief Equality operator, checks if two iterators are equal
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if the iterators are equal, false otherwise
            friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
                return (a.m_Deque == b.m_Deque) && (a.m_CurrentIndex == b.m_CurrentIndex);
            }

            /// @brief Inequality operator, checks if two iterators are not equal
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if the iterators are not equal, false otherwise
            friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
                return !(a == b);
            }

            /// @brief Less-than operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is less than `b`, false otherwise
            friend bool operator<(const ConstIterator& a, const ConstIterator& b) {
                return a.m_CurrentIndex < b.m_CurrentIndex;
            }

            /// @brief Less-than-or-equal operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is less than or equal to `b`, false otherwise
            friend bool operator<=(const ConstIterator& a, const ConstIterator& b) {
                return !(b < a);
            }

            /// @brief Greater-than operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is greater than `b`, false otherwise
            friend bool operator>(const ConstIterator& a, const ConstIterator& b) {
                return b < a;
            }

            /// @brief Greater-than-or-equal operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is greater than or equal to `b`, false otherwise
            friend bool operator>=(const ConstIterator& a, const ConstIterator& b) {
                return !(a < b);
            }

        private:
            const BasicDeque* m_Deque;
            SizeType m_CurrentIndex;

            /// @brief Private constructor
            /// @param deque The deque to create the iterator for
            /// @param start Index of the starting element
            ConstIterator(const BasicDeque* deque, SizeType start) : m_Deque(deque), m_CurrentIndex(start) {}
        };

        /// @brief Circular iterator type for the deque
        class Iterator : public wstl::Iterator<RandomAccessIteratorTag, ValueType> {
        public:
            typedef typename wstl::Iterator<RandomAccessIteratorTag, ValueType>::ReferenceType ReferenceType;  
            typedef typename wstl::Iterator<RandomAccessIteratorTag, ValueType>::PointerType PointerType;
            typedef typename wstl::Iterator<RandomAccessIteratorTag, ValueType>::DifferenceType DifferenceType;

            friend class BasicDeque;
            friend class ConstIterator;

            /// @brief Default constructor
            Iterator() : m_Deque(), m_CurrentIndex() {}

            /// @brief Copy constructor
            /// @param other Iterator to copy from
            Iterator(const Iterator& other) : m_Deque(other.m_Deque), m_CurrentIndex(other.m_CurrentIndex) {}

            /// @brief Copy assignment operator
            /// @param other Iterator to assign from
            Iterator& operator=(const Iterator& other) {
                m_Deque = other.m_Deque;
                m_CurrentIndex = other.m_CurrentIndex;
                return *this;
            }

            /// @brief Dereference operator
            ReferenceType operator*() const {
                return m_Deque->m_Storage.Data[m_Deque->PhysicalIndex(m_CurrentIndex)];
            }

            /// @brief Arrow operator
            PointerType operator->() const {
                return m_Deque->m_Storage.Data + m_Deque->PhysicalIndex(m_CurrentIndex);
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            Iterator& operator++() {
                ++m_CurrentIndex;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            Iterator operator++(int) {
                Iterator original(*this);
                ++m_CurrentIndex;
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            Iterator& operator--() {
                --m_CurrentIndex;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            Iterator operator--(int) {
                Iterator original(*this);
                --m_CurrentIndex;
                return original;
            }

            /// @brief Addition operator - moves the iterator forward by a given offset
            /// @param offset The offset to add (negative for backward movement)
            /// @return Reference to the updated iterator
            Iterator& operator+=(DifferenceType offset) {
                m_CurrentIndex += offset;
                return *this;
            }

            /// @brief Subtraction operator - moves the iterator backwards by a given offset
            /// @param offset The offset to subtract (negative for forward movement)
            /// @return Reference to the updated iterator
            Iterator& operator-=(DifferenceType offset) {
                return operator+=(-offset);
            }

            /// @brief Access operator - allows access to the element at the given index
            /// @param i Index of the element to access
            /// @return Reference to the element at the given index
            ReferenceType operator[](SizeType i) const {
                return m_Deque->m_Storage.Data[m_Deque->PhysicalIndex(m_CurrentIndex + i)];
            }

            /// @brief Addition operator - allows adding an offset to the iterator
            /// @param x The iterator to add the offset to
            /// @param offset The offset to add (negative for backward movement)
            /// @return A new iterator advanced by `offset` positions
            friend Iterator operator+(const Iterator& x, DifferenceType offset) {
                Iterator result(x);
                result += offset;
                return result;
            }

            /// @brief Addition operator - allows adding an offset to the iterator
            /// @param offset The offset to add (negative for backward movement)
            /// @param x The iterator to add the offset to
            /// @return A new iterator advanced by `offset` positions
            friend Iterator operator+(DifferenceType offset, const Iterator& x) {
                Iterator result(x);
                result += offset;
                return result;
            }

            /// @brief Subtraction operator - allows subtracting an offset from the iterator
            /// @param x The iterator to subtract the offset from
            /// @param offset The offset to subtract (negative for forward movement)
            friend Iterator operator-(const Iterator& x, DifferenceType offset) {
                Iterator result(x);
                result -= offset;
                return result;
            }

            /// @brief Subtraction operator - allows subtracting two iterators, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return The difference in positions between the two iterators
            friend DifferenceType operator-(const Iterator& a, const Iterator& b) {
                const DifferenceType indexA = a.m_CurrentIndex;
                const DifferenceType indexB = b.m_CurrentIndex;
                
                return indexA - indexB;
            }

            /// @brief Equality operator, checks if two iterators are equal
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if the iterators are equal, false otherwise
            friend bool operator==(const Iterator& a, const Iterator& b) {
                return (a.m_Deque == b.m_Deque) && (a.m_CurrentIndex == b.m_CurrentIndex);
            }

            /// @brief Inequality operator, checks if two iterators are not equal
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if the iterators are not equal, false otherwise
            friend bool operator!=(const Iterator& a, const Iterator& b) {
                return !(a == b);
            }

            /// @brief Less-than operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is less than `b`, false otherwise
            friend bool operator<(const Iterator& a, const Iterator& b) {
                return a.m_CurrentIndex < b.m_CurrentIndex;
            }

            /// @brief Less-than-or-equal operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is less than or equal to `b`, false otherwise
            friend bool operator<=(const Iterator& a, const Iterator& b) {
                return !(b < a);
            }

            /// @brief Greater-than operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is greater than `b`, false otherwise
            friend bool operator>(const Iterator& a, const Iterator& b) {
                return b < a;
            }

            /// @brief Greater-than-or-equal operator, makes sense only when both iterators belong to the same deque
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return True if `a` is greater than or equal to `b`, false otherwise
            friend bool operator>=(const Iterator& a, const Iterator& b) {
                return !(a < b);
            }

        private:
            BasicDeque* m_Deque;
            SizeType m_CurrentIndex;

            /// @brief Private constructor
            /// @param deque The deque to create the iterator for
            /// @param start Index of the starting element
            Iterator(BasicDeque* deque, SizeType start) : m_Deque(deque), m_CurrentIndex(start) {}
        };

        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        /// @brief Default constructor, only for default-constructible storage
        BasicDeque() : Base(), m_StartIndex(0) {}

        /// @brief Constructor with custom storage, only for non-default-constructible storage
        /// @param storage Storage to use for the deque
        explicit BasicDeque(const StorageType& storage) : Base(storage), m_StartIndex(0) {}

        /// @brief Destructor
        ~BasicDeque() {
            Initialize<ValueType>();
        }

        /// @brief Copy constructor
        /// @param other The deque to copy from
        BasicDeque(const BasicDeque& other) : Base(), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            
            typename BasicDeque::ConstIterator it = other.Begin();
            for(; it != other.End(); ++it) CreateBack(*it);
        }

        /// @brief Copy constructor with custom storage, only for non-default-constructible storage
        /// @param other The deque to copy from
        /// @param storage Storage to use for the deque
        /// @throws `LengthError` if the copied deque's size exceeds the deque's capacity
        BasicDeque(const BasicDeque& other, const StorageType& storage) : Base(storage), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            
            typename BasicDeque::ConstIterator it = other.Begin();
            for(; it != other.End(); ++it) CreateBack(*it);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The deque to move from
        /// @throws `LengthError` if the moved deque's size exceeds the deque's capacity
        /// @since C++11
        BasicDeque(BasicDeque&& other) : Base(), m_StartIndex(0) {
            if(this != &other) {
                __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

                typename BasicDeque::Iterator it = other.Begin();
                for(; it != other.End(); ++it) CreateBack(Move(*it));
            }
        }

        /// @brief Move constructor with custom storage, only for non-default-constructible storage
        /// @param other The deque to move from
        /// @param storage Storage to use for the deque
        /// @throws `LengthError` if the moved deque's size exceeds the deque's capacity
        /// @since C++11
        BasicDeque(BasicDeque&& other, const StorageType& storage) : Base(storage), m_StartIndex(0) {
            if(this != &other) {
                __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

                typename BasicDeque::Iterator it = other.Begin();
                for(; it != other.End(); ++it) CreateBack(Move(*it));
            }
        }
        #endif

        /// @brief Constructor that initializes the deque with a range of elements
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @throws `LengthError` if the range size exceeds the deque's capacity
        template<typename InputIterator>
        BasicDeque(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            for(; first != last; ++first) CreateBack(*first);
        }

        /// @brief Constructor that initializes the deque with a range of elements and custom storage, 
        /// only for non-default-constructible storage
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @param storage Storage to use for the deque
        /// @throws `LengthError` if the range size exceeds the deque's capacity
        template<typename InputIterator>
        BasicDeque(InputIterator first, InputIterator last, const StorageType& storage, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(storage), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            for(; first != last; ++first) CreateBack(*first);
        }

        /// @brief Constructor that initializes the deque with a specific number of default-constructed elements
        /// @param count The number of elements to initialize the deque with
        /// @throws `LengthError` if count exceeds the deque's capacity
        explicit BasicDeque(SizeType count) : Base(), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            while(count--) CreateBack();
        }

        /// @brief Constructor that initializes the deque with a specific number of default-constructed elements and custom storage,
        /// only for non-default-constructible storage
        /// @param count The number of elements to initialize the deque with
        /// @param storage Storage to use for the deque
        /// @throws `LengthError` if count exceeds the deque's capacity
        explicit BasicDeque(SizeType count, const StorageType& storage) : Base(storage), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            while(count--) CreateBack();
        }

        /// @brief Constructor that initializes the deque with a specific number of elements
        /// @param count The number of elements to initialize the deque with
        /// @param value The value to initialize each element with
        /// @throws `LengthError` if count exceeds the deque's capacity
        BasicDeque(SizeType count, ConstReferenceType value) : Base(), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            while(count--) CreateBack(value);
        }

        /// @brief Constructor that initializes the deque with a specific number of elements and custom storage,
        /// only for non-default-constructible storage
        /// @param count The number of elements to initialize the deque with
        /// @param value The value to initialize each element with
        /// @param storage Storage to use for the deque
        /// @throws `LengthError` if count exceeds the deque's capacity
        BasicDeque(SizeType count, ConstReferenceType value, const StorageType& storage) : Base(storage), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            while(count--) CreateBack(value);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Constructor that initializes the deque with an initializer list
        /// @param list The initializer list to initialize the deque with
        /// @throws `LengthError` if list size exceeds the deque's capacity
        /// @since C++11
        BasicDeque(InitializerList<ValueType> list) : Base(), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(list.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            
            typename InitializerList<ValueType>::Iterator it = list.Begin();
            for(; it != list.End(); ++it) CreateBack(*it);
        }

        /// @brief Constructor that initializes the deque with an initializer list and custom storage,
        /// only for non-default-constructible storage
        /// @param list The initializer list to initialize the deque with
        /// @param storage Storage to use for the deque
        /// @throws `LengthError` if list size exceeds the deque's capacity
        /// @since C++11
        BasicDeque(InitializerList<ValueType> list, const StorageType& storage) : Base(storage), m_StartIndex(0) {
            __WSTL_ASSERT_RETURN__(list.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));
            
            typename InitializerList<ValueType>::Iterator it = list.Begin();
            for(; it != list.End(); ++it) CreateBack(*it);
        }
        #endif

        /// @brief Copy assignment operator
        /// @param other The deque to copy from
        /// @throws `LengthError` if the copied deque's size exceeds the deque's capacity
        BasicDeque& operator=(const BasicDeque& other) {
            if(this != &other) Assign(other.Begin(), other.End());
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The deque to move from
        /// @throws `LengthError` if the moved deque's size exceeds the deque's capacity
        /// @since C++11
        BasicDeque& operator=(BasicDeque&& other) {
            if(this != &other) {
                __WSTL_ASSERT_RETURNVALUE__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"), *this);
                Initialize<ValueType>();

                typename BasicDeque::Iterator it = other.Begin();
                for(; it != other.End(); ++it) CreateBack(Move(*it));
            }

            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator
        /// @param list The initializer list to assign to the deque
        /// @throws `LengthError` if list size exceeds the deque's capacity
        /// @since C++11
        BasicDeque& operator=(InitializerList<ValueType> list) {
            Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Assigns a range of elements to the deque
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @throws `LengthError` if the deque's capacity is exceeded
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Assign(InputIterator first, InputIterator last) {
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

            Initialize<ValueType>();
            for(; first != last; ++first) CreateBack(*first);
        }

        /// @brief Assigns a specific number of elements to the deque
        /// @param count The number of elements to assign
        /// @param value The value to assign to each element
        /// @throws `LengthError` if the deques's capacity is exceeded
        void Assign(SizeType count, ConstReferenceType value) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

            Initialize<ValueType>();
            while(count--) CreateBack(value);
        }
        
        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Assigns an initializer list to the deque
        /// @param list The initializer list to assign to the deque
        /// @throws `LengthError` if list size exceeds the deque's capacity
        /// @since C++11
        void Assign(InitializerList<ValueType> list) {
            __WSTL_ASSERT_RETURN__(list.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

            Initialize<ValueType>();
            for(typename InitializerList<ValueType>::Iterator it = list.Begin(); it != list.End(); ++it) CreateBack(*it);
        }
        #endif

        /// @brief Assigns a range of elements to the deque
        /// @param range The range to assign to the deque
        /// @throws `LengthError` if the deque's capacity is exceeded
        template<typename Range>
        inline void AssignRange(const Range& range) {
            Assign(wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Assings elements to the deque by moving them from range
        /// @param range The range to append the elements from
        /// @throws `LengthError` if the deque's capacity is exceeded
        /// @since C++11
        template<typename Range>
        inline void AssignRange(Range&& range) {
            Assign(MakeMoveIterator(wstl::Begin(range)), 
                MakeMoveIterator(wstl::End(range)));
        }
        #endif

        /// @brief Gets the element at the specified position in the deque
        /// @param position The position of the element to access
        /// @throws `OutOfRange` if the position is out of range
        ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position < this->m_CurrentSize, WSTL_MAKE_EXCEPTION(OutOfRange, "Deque index out of range"));
            return this->m_Storage.Data[PhysicalIndex(position)];
        }

        /// @brief Gets the element at the specified position in the deque
        /// @param position The position of the element to access
        /// @throws `OutOfRange` if the position is out of range
        ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position < this->m_CurrentSize, WSTL_MAKE_EXCEPTION(OutOfRange, "Deque index out of range"));
            return this->m_Storage.Data[PhysicalIndex(position)];
        }

        /// @brief Access operator
        /// @param index The index of the element to access
        ReferenceType operator[](SizeType index) {
            return this->m_Storage.Data[PhysicalIndex(index)];
        }

        /// @brief Const access operator
        /// @param index The index of the element to access
        ConstReferenceType operator[](SizeType index) const {
            return this->m_Storage.Data[PhysicalIndex(index)];
        }

        /// @brief Gets the first element in the deque
        ReferenceType Front() {
            return this->m_Storage.Data[this->m_StartIndex];
        }

        /// @brief Gets the first element in the deque
        ConstReferenceType Front() const {
            return this->m_Storage.Data[this->m_StartIndex];
        }

        /// @brief Gets the last element in the deque
        ReferenceType Back() {
            return this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize - 1)];
        }

        /// @brief Gets the last element in the deque
        ConstReferenceType Back() const {
            return this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize - 1)];
        }

        /// @brief Gets iterator to the beginning of the deque
        Iterator Begin() {
            return Iterator(this, 0);
        }

        /// @brief Gets const iterator to the beginning of the deque
        ConstIterator Begin() const {
            return ConstIterator(this, 0);
        }

        /// @brief Gets const iterator to the beginning of the deque
        ConstIterator ConstBegin() const {
            return ConstIterator(this, 0);
        }

        /// @brief Gets iterator to the end of the deque
        Iterator End() {
            return Iterator(this, this->m_CurrentSize);
        }

        /// @brief Gets const iterator to the end of the deque
        ConstIterator End() const {
            return ConstIterator(this, this->m_CurrentSize);
        }

        /// @brief Gets const iterator to the end of the deque
        ConstIterator ConstEnd() const {
            return ConstIterator(this, this->m_CurrentSize);
        }

        /// @brief Gets reverse iterator to the beginning of the deque
        ReverseIterator ReverseBegin() {
            return ReverseIterator(End());
        }

        /// @brief Gets const reverse iterator to the beginning of the deque
        ConstReverseIterator ReverseBegin() const {
            return ConstReverseIterator(End());
        }

        /// @brief Gets const reverse iterator to the beginning of the deque
        ConstReverseIterator ConstReverseBegin() const {
            return ConstReverseIterator(End());
        }

        /// @brief Gets reverse iterator to the end of the deque
        ReverseIterator ReverseEnd() {
            return ReverseIterator(Begin());
        }

        /// @brief Gets const reverse iterator to the end of the deque
        ConstReverseIterator ReverseEnd() const {
            return ConstReverseIterator(Begin());
        }

        /// @brief Gets const reverse iterator to the end of the deque
        ConstReverseIterator ConstReverseEnd() const {
            return ConstReverseIterator(Begin());
        }

        /// @brief Clears the deque, removing all elements
        void Clear() {
            Initialize<ValueType>();
        }

        /// @brief Inserts an element at specified position in the deque
        /// @param position The position to insert the element at
        /// @param value The value to insert
        /// @return Iterator to the newly inserted element
        /// @throws `LengthError` if the deque is full
        Iterator Insert(ConstIterator position, ConstReferenceType value) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);
            
            if(result == Begin()) CreateFront(value);
            else if(result == End()) CreateBack(value);
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());
                    *--result = value;
                }   
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    *result = value;
                }
            }

            return result;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts an element at specified position in the deque
        /// @param position The position to insert the element at
        /// @param value The value to insert (rvalue reference)
        /// @return Iterator to the newly inserted element
        /// @throws `LengthError` if the deque is full
        /// @since C++11
        Iterator Insert(ConstIterator position, ValueType&& value) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            if(result == Begin()) CreateFront(Move(value));
            else if(result == End()) CreateBack(Move(value));
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());
                    *--result = Move(value);
                }   
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    *result = Move(value);
                }
            }

            return result;
        }
        #endif

        /// @brief Inserts multiple elements at specified position in the deque
        /// @param position The position to insert the elements at
        /// @param count The number of elements to insert
        /// @param value The value to insert
        /// @return Iterator to the first inserted element
        /// @throws `LengthError` if the deque is full
        Iterator Insert(ConstIterator position, SizeType count, ConstReferenceType value) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(count <= this->Capacity() - this->m_CurrentSize, WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                for(SizeType i = 0; i < count; ++i) CreateFront(value);
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(SizeType i = 0; i < count; ++i) CreateBack(value);
                result = End() - count;
            }
            else {
                // Insert in the middle
                if (distanceFront <= distanceBack) {
                    SizeType createCopy = Min(count, distanceFront);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceFront - createCopy;

                    // Create new values
                    for(SizeType i = 0; i < createNew; ++i) CreateFront(value);

                    // Create copies of old values
                    Iterator from = Begin() + createCopy - 1 + createNew;
                    for(SizeType i = 0; i < createCopy; ++i, --from) CreateFront(*from);

                    // Move old values
                    from = result - copyOld;
                    Iterator to = Begin() + createCopy;
                    Move(from, from + copyOld, to);

                    // Fill the rest of the new values
                    to = result - createCopy;
                    FillInRange(to, copyNew, value);

                    result = Begin() + distanceFront;
                } 
                else {
                    SizeType createCopy = Min(count, distanceBack);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceBack - createCopy;

                    // Create new values
                    for(SizeType i = 0; i < createNew; ++i) CreateBack(value);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; ++i) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + count + copyOld);

                    // Fill the rest of the new values
                    FillInRange(result, copyNew, value);
                }
            }

            return result;
        }

        /// @brief Inserts a range of elements at specified position in the deque
        /// @param position The position to insert the elements at
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @return Iterator to the first inserted element
        /// @throws `LengthError` if the deque is full
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, Iterator>::Type
        Insert(ConstIterator position, InputIterator first, InputIterator last) {
            Iterator result = ToIterator(position);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(count <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                CreateFront(first, count);
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(; first != last; ++first) CreateBack(*first);
                result = End() - count;
            }
            else {
                // Insert in the middle
                if (distanceFront <= distanceBack) {
                    SizeType createCopy = Min(count, distanceFront);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceFront - createCopy;

                    // Create new values
                    CreateFront(first, createNew);

                    // Create copies of old values
                    CreateFront(Begin() + createNew, createCopy);

                    // Move old values
                    Iterator from = result - copyOld;
                    Iterator to = Begin() + createCopy;
                    Move(from, from + copyOld, to);
                    
                    // Copy new values
                    to = result - createCopy;
                    first += createNew;
                    CopyInRange(first, copyNew, to);

                    result = Begin() + distanceFront;
                } 
                else {
                    SizeType createCopy = Min(count, distanceBack);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceBack - createCopy;

                    // Create new values
                    InputIterator it = first + (count - createNew);
                    for(SizeType i = 0; i < createNew; ++i, ++it) CreateBack(*it);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; ++i) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + count + copyOld);

                    // Copy new values
                    CopyInRange(first, copyNew, result);
                }
            }

            return result;
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Inserts an initializer list at specified position in the deque
        /// @param position The position to insert the list at
        /// @param list The initializer list to insert
        /// @throws `LengthError` if the deque's capacity is exceeded
        /// @return Iterator to the first inserted element
        /// @since C++11
        Iterator Insert(ConstIterator position, InitializerList<ValueType> list) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(list.Size() <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"), result);

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                CreateFront(list.Begin(), list.Size());
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(typename InitializerList<ValueType>::Iterator it = list.Begin(); it != list.End(); ++it) CreateBack(*it);
                result = End() - list.Size();
            }
            else {
                // Insert in the middle
                if (distanceFront <= distanceBack) {
                    SizeType createCopy = Min(list.Size(), distanceFront);
                    SizeType createNew = (list.Size() > createCopy) ? list.Size() - createCopy : 0;
                    SizeType copyNew = (list.Size() > createNew) ? list.Size() - createNew : 0;
                    SizeType copyOld = distanceFront - createCopy;

                    // Create new values
                    CreateFront(list.Begin(), createNew);

                    // Create copies of old values
                    CreateFront(Begin() + createNew, createCopy);

                    // Move old values
                    Iterator from = result - copyOld;
                    Iterator to = Begin() + createCopy;
                    Move(from, from + copyOld, to);
                    
                    // Copy new values
                    to = result - createCopy;
                    CopyInRange(list.Begin() + createNew, copyNew, to);

                    result = Begin() + distanceFront;
                } 
                else {
                    SizeType createCopy = Min(list.Size(), distanceBack);
                    SizeType createNew = (list.Size() > createCopy) ? list.Size() - createCopy : 0;
                    SizeType copyNew = (list.Size() > createNew) ? list.Size() - createNew : 0;
                    SizeType copyOld = distanceBack - createCopy;

                    // Create new values
                    typename InitializerList<ValueType>::Iterator it = list.Begin() + (list.Size() - createNew);
                    for(SizeType i = 0; i < createNew; ++i, ++it) CreateBack(*it);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; ++i) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + list.Size() + copyOld);

                    // Copy new values
                    CopyInRange(list.Begin(), copyNew, result);
                }
            }

            return result;
        }
        #endif

        /// @brief Inserts a range of elements at the specified position in the deque
        /// @param position The position to insert the range at
        /// @param range The range to insert
        /// @return Iterator to the first inserted element
        /// @throws `LengthError` if the deque's capacity is exceeded
        template<typename Range>
        inline Iterator InsertRange(ConstIterator position, const Range& range) {
            return Insert(position, wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts elements to the deque by moving them from range
        /// @param position The position to insert the range at
        /// @param range The range to insert
        /// @return Iterator to the first inserted element
        /// @throws `LengthError` if the deque's capacity is exceeded
        /// @since C++11
        template<typename Range>
        inline Iterator InsertRange(ConstIterator position, Range&& range) {
            return Insert(position, MakeMoveIterator(wstl::Begin(range)), MakeMoveIterator(wstl::End(range)));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param ...args The arguments to forward to the constructor of the element
        /// @throws `LengthError` if the deque is full 
        /// @return Iterator to the newly emplaced element
        /// @since C++11
        template<typename... Args>
        Iterator Emplace(ConstIterator position, Args&&... args) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            void* pointer;
            
            if(result == Begin()) {
                // Set pointer to the end of the buffer
                this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
                ++this->m_CurrentSize;
                pointer = static_cast<void*>(&this->m_Storage.Data[this->m_StartIndex]);
            }
            else if(result == End()) {
                // Set pointer to the beginning of the buffer
                pointer = static_cast<void*>(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]);
                ++this->m_CurrentSize;
            }
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());

                    // Destroy the element at the result position to write a new one
                    (*--result).~T();
                    pointer = AddressOf(*result);
                }   
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    
                    // Destroy the element at the result position to write a new one
                    (*result).~T();
                    pointer = AddressOf(*result);
                }
            }

            ::new(pointer) ValueType(Forward<Args>(args)...);

            return result;
        }

        #else
        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @throws `LengthError` if the deque is full
        /// @return Iterator to the newly emplaced element
        Iterator Emplace(ConstIterator position) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            void* pointer;
            
            if(result == Begin()) {
                // Set pointer to the end of the buffer
                this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
                ++this->m_CurrentSize;
                pointer = static_cast<void*>(&this->m_Storage.Data[this->m_StartIndex]);
            }
            else if(result == End()) {
                // Set pointer to the beginning of the buffer
                pointer = static_cast<void*>(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]);
                ++this->m_CurrentSize;
            }
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());

                    // Destroy the element at the result position to write a new one
                    (*--result).~T();
                    pointer = AddressOf(*result);
                }
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    
                    // Destroy the element at the result position to write a new one
                    (*result).~T();
                    pointer = AddressOf(*result);
                }
            }

            ::new(pointer) ValueType();

            return result;
        }

        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg The argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg>
        Iterator Emplace(ConstIterator position, const Arg& arg) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            void* pointer;
            
            if(result == Begin()) {
                // Set pointer to the end of the buffer
                this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
                ++this->m_CurrentSize;
                pointer = static_cast<void*>(&this->m_Storage.Data[this->m_StartIndex]);
            }
            else if(result == End()) {
                // Set pointer to the beginning of the buffer
                pointer = static_cast<void*>(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]);
                ++this->m_CurrentSize;
            }
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());
                    
                    // Destroy the element at the result position to write a new one
                    (*--result).~T();
                    pointer = AddressOf(*result);
                }   
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    
                    // Destroy the element at the result position to write a new one
                    (*result).~T();
                    pointer = AddressOf(*result);
                }
            }

            ::new(pointer) ValueType(arg);

            return result;
        }

        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg1, typename Arg2>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            void* pointer;
            
            if(result == Begin()) {
                // Set pointer to the end of the buffer
                this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
                ++this->m_CurrentSize;
                pointer = static_cast<void*>(&this->m_Storage.Data[this->m_StartIndex]);
            }
            else if(result == End()) {
                // Set pointer to the beginning of the buffer
                pointer = static_cast<void*>(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]);
                ++this->m_CurrentSize;
            }
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());

                    // Destroy the element at the result position to write a new one
                    (*--result).~T();
                    pointer = AddressOf(*result);
                }   
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    
                    // Destroy the element at the result position to write a new one
                    (*result).~T();
                    pointer = AddressOf(*result);
                }
            }

            ::new(pointer) ValueType(arg1, arg2);

            return result;
        }

        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg1, typename Arg2, typename Arg3>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            Iterator result = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"), result);

            void* pointer;
            
            if(result == Begin()) {
                // Set pointer to the end of the buffer
                this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
                ++this->m_CurrentSize;
                pointer = static_cast<void*>(&this->m_Storage.Data[this->m_StartIndex]);
            }
            else if(result == End()) {
                // Set pointer to the beginning of the buffer
                pointer = static_cast<void*>(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]);
                ++this->m_CurrentSize;
            }
            else {
                // Shifting elements to make space
                if(Distance(Begin(), result) < Distance(result, End() - 1)) {
                    // Shift elements towards front
                    CreateFront(*Begin());
                    Move(Begin() + 1, result, Begin());

                    // Destroy the element at the result position to write a new one
                    (*--result).~T();
                    pointer = AddressOf(*result);
                }   
                else {
                    // Shift elements towards back
                    CreateBack(*(End() - 1));
                    MoveBackward(result, End() - 2, End() - 1);
                    
                    // Destroy the element at the result position to write a new one
                    (*result).~T();
                    pointer = AddressOf(*result);
                }
            }

            ::new(pointer) ValueType(arg1, arg2, arg3);

            return result;
        }
        #endif

        /// @brief Erases an element at specified position in the deque
        /// @param position The position of the element to erase
        /// @return Iterator to the element following the erased element
        /// @throws `OutOfRange` if the position is out of range
        Iterator Erase(ConstIterator position) {
            Iterator result = ToIterator(position);

            if(result == Begin()) {
                DestroyFront();
                result = Begin();
            }
            else if(result == End() - 1) {
                DestroyBack();
                result = End();
            }
            else {
                if(Distance(Begin(), result) <= Distance(result, End())) {
                    // Move front elements to the back to overwrite the position
                    MoveBackward(Begin(), result, result + 1);

                    // Destroy unneeded front element
                    DestroyFront();
                    ++result;
                }
                else {
                    // Move back elements to the front to overwrite the position
                    Move(result + 1, End(), result);

                    // Destroy unneeded back element
                    DestroyBack();
                }
            }

            return result;
        }

        /// @brief Erases a range of elements from the deque
        /// @param first Iterator to the first element in the range to erase
        /// @param last Iterator to the element following the last element in the range to erase
        /// @return Iterator to the first element following the erased range
        /// @throws `OutOfRange` if the range is out of bounds
        Iterator Erase(ConstIterator first, ConstIterator last) {
            Iterator result = ToIterator(first);
            SizeType count = Distance(first, last);

            if(result == Begin()) {
                for(SizeType i = 0; i < count; ++i) DestroyFront();
                result = Begin();
            }
            else if(result == End() - count) {
                for(SizeType i = 0; i < count; ++i) DestroyBack();
                result = End();
            }
            else {
                if(Distance(Begin(), result) <= DifferenceType(this->m_CurrentSize / 2)) {
                    // Move front elements to the back to overwrite the range
                    MoveBackward(Begin(), result, result + count);

                    // Destroy unneeded front elements
                    for(SizeType i = 0; i < count; ++i) DestroyFront();
                    result += count;
                }
                else {
                    // Move back elements to the front to overwrite the range
                    Move(result + count, End(), result);
                    
                    // Destroy unneeded back elements
                    for(SizeType i = 0; i < count; ++i) DestroyBack();
                }
            }

            return result;
        }

        /// @brief Pushes an element to the back of the deque
        /// @param value The value to push to the back
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PushBack(ConstReferenceType value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));
            CreateBack(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes an element to the back of the deque
        /// @param value The value to push to the back (rvalue reference)
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        void PushBack(ValueType&& value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));
            CreateBack(Forward<ValueType>(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param ...args The arguments to forward to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename... Args>
        void EmplaceBack(Args&&... args) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType(Forward<Args>(args)...);
            ++this->m_CurrentSize;
        }

        #else
        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void EmplaceBack() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType();
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param arg The argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg>
        void EmplaceBack(const Arg& arg) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType(arg);
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType(arg1, arg2);
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType(arg1, arg2, arg3);
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Appends a range of elements to the back of the deque
        /// @param range The range to append the elements from
        /// @throws `LengthError` if the deque is full
        template<typename Range>
        inline void AppendRange(const Range& range) {
            Insert(End(), wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Appends elements to the back of the deque by moving them from range
        /// @param range The range to append the elements from
        /// @throws `LengthError` if the deque is full
        /// @since C++11
        template<typename Range>
        inline void AppendRange(Range&& range) {
            Insert(End(), MakeMoveIterator(wstl::Begin(range)), 
                MakeMoveIterator(wstl::End(range)));
        }
        #endif

        /// @brief Pops the last element from the deque
        /// @throws `OutOfRange` if the deque is empty and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PopBack() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(OutOfRange, "Deque empty"));
            DestroyBack();
        }

        /// @brief Pushes an element to the front of the deque
        /// @param value The value to push to the front
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PushFront(ConstReferenceType value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));
            CreateFront(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes an element to the front of the deque
        /// @param value The value to push to the front (rvalue reference)
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        void PushFront(ValueType&& value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));
            CreateFront(Forward<ValueType>(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param ...args The arguments to forward to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename... Args>
        void EmplaceFront(Args&&... args) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct the front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType(Forward<Args>(args)...);
            ++this->m_CurrentSize;
        }

        #else
        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void EmplaceFront() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct the front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType();
            ++this->m_CurrentSize;
        }


        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param arg The argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg>
        void EmplaceFront(const Arg& arg) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct the front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType(arg);
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct the front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType(arg1, arg2);
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws `LengthError` if the deque is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Deque full"));

            // Construct the front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType(arg1, arg2, arg3);
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Pops the first element from the deque
        /// @throws `OutOfRange` if the deque is empty and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PopFront() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(OutOfRange, "Deque empty"));
            DestroyFront();
        }

        /// @brief Prepends a range of elements to the front of the deque
        /// @param range The range to prepend the elements from
        /// @throws `LengthError` if the deque is full
        template<typename Range>
        inline void PrependRange(const Range& range) {
            Insert(Begin(), wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Prepends a range of elements to the front of the deque, moving them
        /// @param range The range to prepend the elements from
        /// @throws `LengthError` if the deque is full
        /// @since C++11
        template<typename Range>
        inline void PrependRange(Range&& range) {
            Insert(Begin(), MakeMoveIterator(wstl::Begin(range)), 
                MakeMoveIterator(wstl::End(range)));
        }
        #endif

        /// @brief Resizes the deque to the specified size, filling new elements with a default value
        /// @param count The new size of the deque
        /// @throws `LengthError` if the deque's capacity is exceeded
        void Resize(SizeType count) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

            if(count < this->m_CurrentSize) {
                while(this->m_CurrentSize > count) DestroyBack();
            }
            else if(count > this->m_CurrentSize) {
                SizeType newCount = count - this->m_CurrentSize;
                for(SizeType i = 0; i < newCount; ++i) CreateBack();
            }
        }

        /// @brief Resizes the deque to the specified size, filling new elements with a specified value
        /// @param count The new size of the deque
        /// @param value The value to fill new elements with
        /// @throws `LengthError` if the deque's capacity is exceeded
        void Resize(SizeType count, ConstReferenceType value) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "Deque overflow"));

            if(count < this->m_CurrentSize) {
                while(this->m_CurrentSize > count) DestroyBack();
            }
            else if(count > this->m_CurrentSize) {
                SizeType newCount = count - this->m_CurrentSize;
                for(SizeType i = 0; i < newCount; ++i) CreateBack(value);
            }
        }

        /// @brief Swaps content of two deques
        /// @param other The deque to swap with
        void Swap(BasicDeque& other) __WSTL_NOEXCEPT_EXPR__(noexcept(wstl::Swap(this->m_Storage, other.m_Storage))) {
            wstl::Swap(this->m_Storage, other.m_Storage);
            wstl::Swap(m_StartIndex, other.m_StartIndex);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
        }
    
    private:
        SizeType m_StartIndex;

        /// @brief Converts a logical index to a physical index in the buffer
        /// @param index The logical index
        /// @return The physical index in the buffer
        SizeType PhysicalIndex(SizeType index) const {
            return (m_StartIndex + index) % this->Capacity();
        }

        Iterator ToIterator(ConstIterator iterator) {
            return Iterator(this, iterator.m_CurrentIndex);
        }

        /// @brief Initializes the deque, trivial version
        template<typename U>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
            this->m_StartIndex = 0;
        }

        /// @brief Initializes the deque, non-trivial version: calls destructors
        template<typename U>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->m_CurrentSize > 0) DestroyBack();
            this->m_StartIndex = 0;
        }

        /// @brief Creates an element at the front of the deque
        /// @param value The value to create the element with
        void CreateFront(ConstReferenceType value) {
            // Construct front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType(value);
            ++this->m_CurrentSize;
        }

        /// @brief Creates a range of elements at the front of the deque
        /// @param first Iterator to the first element in the range to create
        /// @param count The number of elements to create
        template<typename Iterator>
        void CreateFront(Iterator first, SizeType count) {
            if(count == 0) return;

            // Move start index from the end of the buffer by `count` positions
            this->m_StartIndex = PhysicalIndex(this->Capacity() - count);
            this->m_CurrentSize += count;

            for(SizeType i = 0; i < count; ++i, ++first)
                ::new(&this->m_Storage.Data[this->m_StartIndex + i]) ValueType(*first);
        }

        /// @brief Creates a default-constructed element at the back of the deque
        void CreateBack() {
            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType();
            ++this->m_CurrentSize;
        }
        
        /// @brief Creates an element at the back of the deque
        /// @param value The value to create the element with
        void CreateBack(ConstReferenceType value) {
            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType(value);
            ++this->m_CurrentSize;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Creates an element at the front of the deque
        /// @param value The value to create the element with (rvalue reference)
        /// @since C++11
        void CreateFront(ValueType&& value) {
            // Construct front element at the end of the buffer
            this->m_StartIndex = PhysicalIndex(this->Capacity() - 1);
            ::new(&this->m_Storage.Data[this->m_StartIndex]) ValueType(Move(value));
            ++this->m_CurrentSize;
        }

        /// @brief Creates an element at the back of the deque
        /// @param value The value to create the element with (rvalue reference)
        /// @since C++11
        void CreateBack(ValueType&& value) {
            // Construct back element at the beginning of the buffer
            ::new(&this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)]) ValueType(Move(value));
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Destroys an element at the back of the deque
        void DestroyBack() {
            --this->m_CurrentSize;
            this->m_Storage.Data[PhysicalIndex(this->m_CurrentSize)].~ValueType();
        }

        /// @brief Destroys an element at the front of the deque
        void DestroyFront() {
            this->m_Storage.Data[this->m_StartIndex].~ValueType();
            this->m_StartIndex = (this->m_StartIndex + 1) % this->Capacity();
            --this->m_CurrentSize;
        }
    };

    // Comparison operators

    template<typename Storage>
    inline bool operator==(const BasicDeque<Storage>& a, const BasicDeque<Storage>& b) {
        return (a.Size() == b.Size()) && Equal(a.Begin(), a.End(), b.Begin());
    }

    template<typename Storage>
    inline bool operator!=(const BasicDeque<Storage>& a, const BasicDeque<Storage>& b) {
        return !(a == b);
    }

    template<typename Storage>
    inline bool operator<(const BasicDeque<Storage>& a, const BasicDeque<Storage>& b) {
        return LexicographicalCompare(a.Begin(), a.End(), b.Begin(), b.End());
    }

    template<typename Storage>
    inline bool operator<=(const BasicDeque<Storage>& a, const BasicDeque<Storage>& b) {
        return !(b < a);
    }

    template<typename Storage>
    inline bool operator>(const BasicDeque<Storage>& a, const BasicDeque<Storage>& b) {
        return b < a;
    }

    template<typename Storage>
    inline bool operator>=(const BasicDeque<Storage>& a, const BasicDeque<Storage>& b) {
        return !(a < b);
    }

    // Deque

    /// @brief Version of deque with fixed storage, default option
    /// @tparam T Type of the elements
    /// @tparam N Capacity of the deque
    /// @ingroup deque
    template<typename T, size_t N>
    class Deque : public BasicDeque<FixedStorage<T, N> > {
    private:
        typedef BasicDeque<FixedStorage<T, N> > Base;

    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;

        typedef typename Base::StorageType StorageType;

        /// @brief The static size, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

        /// @brief Default constructor
        Deque() : Base() {}

        /// @brief Copy constructor
        /// @param other The deque to copy from
        Deque(const Deque& other) : Base(other) {}

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The deque to move from
        /// @since C++11
        Deque(Deque&& other) : Base(Move(other)) {}
        #endif

        /// @brief Constructor that initializes the deque with a range of elements
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        template<typename InputIterator>
        Deque(InputIterator first, InputIterator last) : Base(first, last) {}

        /// @brief Constructor that initializes the deque with a number of copies of a value
        /// @param count The number of elements to create
        explicit Deque(SizeType count) : Base(count) {}

        /// @brief Constructor that initializes the deque with a number of copies of a value
        /// @param count The number of elements to create
        /// @param value The value to fill the deque with
        Deque(SizeType count, ConstReferenceType value) : Base(count, value) {}

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Constructor that initializes the deque with an initializer list
        /// @param list The initializer list to initialize the deque with
        /// @since C++11
        Deque(InitializerList<ValueType> list) : Base(list) {}
        #endif

        /// @brief Copy assignment operator
        /// @param other The deque to copy from
        Deque& operator=(const Deque& other) {
            if(this != &other) this->Assign(other.Begin(), other.End());
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The deque to move from
        /// @since C++11
        Deque& operator=(Deque&& other) {
            if(this != &other) {
                this->Clear();

                typename Base::Iterator it = other.Begin();
                for(; it != other.End(); ++it) this->CreateBack(Move(*it));
            }

            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator that assigns from an initializer list
        /// @param list The initializer list to assign from
        /// @since C++11
        Deque& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename Deque<T, N>::SizeType Deque<T, N>::StaticSize;

    // Template deduction guides

    #ifdef __WSTL_CXX17__
    template<typename T, typename... U>
    Deque(T, U...) -> Deque<T, sizeof...(U) + 1>;
    #endif

    // Make deque

    #ifdef __WSTL_CXX11__
    /// @brief Makes a deque out of the given values, with specified type
    /// @tparam T Type of the elements
    /// @param ...values Values to make the deque with
    /// @return A deque containing the given values
    /// @ingroup deque
    /// @since C++11
    template<typename T, typename First, typename... Rest>
    constexpr auto MakeDeque(First&& first, Rest&&... rest) {
        return Deque<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }

    /// @brief Makes a deque out of the given values
    /// @param ...values Values to make the deque with
    /// @return A deque containing the given values
    /// @ingroup deque
    /// @since C++11
    template<typename First, typename... Rest>
    constexpr auto MakeDeque(First&& first, Rest&&... rest) {
        using T = CommonTypeType<First, Rest...>;
        return Deque<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }
    #endif

    // Deque external

    namespace external {
        /// @brief Version of deque that uses external storage
        /// @tparam T Type of the elements
        /// @ingroup deque
        template<typename T>
        class Deque : public BasicDeque<ExternalStorage<T> > {
        private:
            typedef BasicDeque<ExternalStorage<T> > Base;

        public:
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::DifferenceType DifferenceType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;

            typedef typename Base::StorageType StorageType;

            /// @brief Constructor that uses external buffer
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            Deque(T* buffer, SizeType capacity) : Base(ExternalStorage(buffer, capacity)) {}

            /// @brief Copy constructor that uses external buffer
            /// @param other The deque to copy from
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            Deque(const Deque& other, T* buffer, SizeType capacity) : Base(other, ExternalStorage(buffer, capacity)) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor that uses external buffer
            /// @param other The deque to move from
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            Deque(Deque&& other, T* buffer, SizeType capacity) : Base(Move(other), ExternalStorage(buffer, capacity)) {}
            #endif

            /// @brief Constructor that initializes the deque with a range of elements
            /// @param first Iterator to the first element in the range
            /// @param last Iterator to the element following the last element in the range
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            template<typename InputIterator>
            Deque(InputIterator first, InputIterator last, T* buffer, SizeType capacity) : Base(first, last, ExternalStorage(buffer, capacity)) {}

            /// @brief Constructor that initializes the deque with a number of copies of a value
            /// @param count The number of elements to create
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            explicit Deque(SizeType count, T* buffer, SizeType capacity) : Base(count, ExternalStorage(buffer, capacity)) {}

            /// @brief Constructor that initializes the deque with a number of copies of a value
            /// @param count The number of elements to create
            /// @param value The value to fill the deque with
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            Deque(SizeType count, ConstReferenceType value, T* buffer, SizeType capacity) : Base(count, value, ExternalStorage(buffer, capacity)) {}

            #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
            /// @brief Constructor that initializes the deque with an initializer list
            /// @param list The initializer list to initialize the deque with
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            /// @since C++11
            Deque(InitializerList<ValueType> list, T* buffer, SizeType capacity) : Base(list, ExternalStorage(buffer, capacity)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The deque to copy from
            Deque& operator=(const Deque& other) {
                if(this != &other) this->Assign(other.Begin(), other.End());
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The deque to move from
            /// @since C++11
            Deque& operator=(Deque&& other) {
                if(this != &other) {
                    this->Clear();

                    typename Base::Iterator it = other.Begin();
                    for(; it != other.End(); ++it) this->CreateBack(Move(*it));
                }

                return *this;
            }

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Assignment operator that assigns from an initializer list
            /// @param list The initializer list to assign from
            /// @since C++11
            Deque& operator=(InitializerList<ValueType> list) {
                this->Assign(list);
                return *this;
            }
            #endif
            #endif
        };

        /// @brief Version of deque that uses fixed external storage with compile-time known capacity
        /// @tparam T Type of the elements
        /// @tparam N Capacity of the deque
        /// @ingroup deque
        template<typename T, size_t N>
        class FixedDeque : public BasicDeque<FixedExternalStorage<T, N> > {
        private:
            typedef BasicDeque<FixedExternalStorage<T, N> > Base;

        public:
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::DifferenceType DifferenceType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;

            typedef typename Base::StorageType StorageType;

            /// @brief The static size, needed for metaprogramming
            static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

            /// @brief Constructor that uses external buffer
            /// @param buffer Pointer to the external buffer
            explicit FixedDeque(T* buffer) : Base(StorageType(buffer)) {}

            /// @brief Copy constructor that uses external buffer
            /// @param other The deque to copy from
            /// @param buffer Pointer to the external buffer
            FixedDeque(const FixedDeque& other, T* buffer) : Base(other, StorageType(buffer)) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor that uses external buffer
            /// @param other The deque to move from
            /// @param buffer Pointer to the external buffer
            FixedDeque(FixedDeque&& other, T* buffer) : Base(Move(other), StorageType(buffer)) {}
            #endif

            /// @brief Constructor that initializes the deque with a range of elements
            /// @param first Iterator to the first element in the range
            /// @param last Iterator to the element following the last element in the range
            /// @param buffer Pointer to the external buffer
            template<typename InputIterator>
            FixedDeque(InputIterator first, InputIterator last, T* buffer) : Base(first, last, StorageType(buffer)) {}

            /// @brief Constructor that initializes the deque with a number of copies of a value
            /// @param count The number of elements to create
            /// @param buffer Pointer to the external buffer
            explicit FixedDeque(SizeType count, T* buffer) : Base(count, StorageType(buffer)) {}

            /// @brief Constructor that initializes the deque with a number of copies of a value
            /// @param count The number of elements to create
            /// @param value The value to fill the deque with
            /// @param buffer Pointer to the external buffer
            FixedDeque(SizeType count, ConstReferenceType value, T* buffer) : Base(count, value, StorageType(buffer)) {}

            #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
            /// @brief Constructor that initializes the deque with an initializer list
            /// @param list The initializer list to initialize the deque with
            /// @param buffer Pointer to the external buffer
            /// @since C++11
            FixedDeque(InitializerList<ValueType> list, T* buffer) : Base(list, StorageType(buffer)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The deque to copy from
            FixedDeque& operator=(const FixedDeque& other) {
                if(this != &other) this->Assign(other.Begin(), other.End());
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The deque to move from
            /// @since C++11
            FixedDeque& operator=(FixedDeque&& other) {
                if(this != &other) {
                    this->Clear();

                    typename Base::Iterator it = other.Begin();
                    for(; it != other.End(); ++it) this->CreateBack(Move(*it));
                }

                return *this;
            }

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Assignment operator that assigns from an initializer list
            /// @param list The initializer list to assign from
            /// @since C++11
            FixedDeque& operator=(InitializerList<ValueType> list) {
                this->Assign(list);
                return *this;
            }
            #endif
            #endif
        };

        template<typename T, size_t N>
        const __WSTL_CONSTEXPR__ typename FixedDeque<T, N>::SizeType FixedDeque<T, N>::StaticSize;

        // Template deduction guides

        #ifdef __WSTL_CXX17__
        template<typename T, size_t N>
        FixedDeque(T(&)[N]) -> FixedDeque<T, N>;

        template<typename T, typename U, size_t N>
        FixedDeque(U, T(&)[N]) -> FixedDeque<T, N>;

        template<typename T, typename U1, typename U2, size_t N>
        FixedDeque(U1, U2, T(&)[N]) -> FixedDeque<T, N>;

        template<typename T, size_t N>
        FixedDeque(InitializerList<T>, T(&)[N]) -> FixedDeque<T, N>;
        #endif
    }
}

#endif