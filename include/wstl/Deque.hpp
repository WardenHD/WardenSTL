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
/// @brief A double-ended queue with size defined at compile-time

namespace wstl {
    /// @brief A double-ended queue that supports pushing and popping elements from both ends
    /// @tparam T Type of the element to store in the deque
    /// @tparam SIZE The maximum number of elements the deque can hold
    /// @ingroup deque
    /// @see https://en.cppreference.com/w/cpp/container/deque
    template<typename T, const size_t SIZE>
    class Deque : public TypedContainerBase<T> {
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;

        /// @brief Circular const iterator type for the deque
        class ConstIterator : public wstl::Iterator<RandomAccessIteratorTag, typename IteratorTraits<const T*>::ValueType> {
        public: 
            friend class Deque;

            /// @brief Default constructor
            ConstIterator() : m_Iterator() {}

            /// @brief Copy constructor
            /// @param other Const iterator to copy from
            ConstIterator(const ConstIterator& other) :
                m_Iterator(other.m_Iterator) {}

            /// @brief Copy constructor
            /// @param other Iterator to copy from
            ConstIterator(const typename Deque::Iterator& other) :
                m_Iterator(other.m_Iterator) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor
            /// @param other Const iterator to move from
            ConstIterator(ConstIterator&& other) : m_Iterator(Move(other.m_Iterator)) {}

            /// @brief Move constructor
            /// @param other Iterator to move from
            ConstIterator(typename Deque::Iterator&& other) : m_Iterator(Move(other.m_Iterator)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other Const iterator to assign from
            ConstIterator& operator=(const ConstIterator& other) {
                m_Iterator = other.m_Iterator;
                return *this;
            }

            /// @brief Copy assignment operator
            /// @param other Iterator to assign from
            ConstIterator& operator=(const typename Deque::Iterator& other) {
                m_Iterator = other.m_Iterator;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other Const iterator to move from
            ConstIterator& operator=(ConstIterator&& other) {
                m_Iterator = Move(other.m_Iterator);
                return *this;
            }

            /// @brief Move assignment operator
            /// @param other Iterator to move from
            ConstIterator& operator=(typename Deque::Iterator&& other) {
                m_Iterator = Move(other.m_Iterator);
                return *this;
            }
            #endif

            /// @brief Const deference operator
            ReferenceType operator*() const {
                return *m_Iterator;
            }

            /// @brief Const arrow operator
            PointerType operator->() const {
                return m_Iterator.operator->();
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            ConstIterator& operator++() {
                ++m_Iterator;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            ConstIterator operator++(int) {
                ConstIterator original(*this);
                ++(*this);
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            ConstIterator& operator--() {
                --m_Iterator;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            ConstIterator operator--(int) {
                ConstIterator original(*this);
                --(*this);
                return original;
            }

            /// @brief Addition operator - moves the iterator forward by a given offset
            /// @param offset The offset to add (negative for backward movement)
            /// @return Reference to the updated iterator
            ConstIterator& operator+=(DifferenceType offset) {
                m_Iterator += offset;
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
            ReferenceType operator[](SizeType i) {
                return *(m_Iterator + i);
            }

            /// @brief Const access operator - allows access to the element at the given index
            /// @param i Index of the element to access
            /// @return Const reference to the element at the given index
            const ReferenceType operator[](SizeType i) const {
                return *(m_Iterator + i);
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

            /// @brief Subtraction operator - allows subtracting two iterators
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return The difference in positions between the two iterators
            friend DifferenceType operator-(const ConstIterator& a, const ConstIterator& b) {
                return a.m_Iterator - b.m_Iterator;
            }

            friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
                return a.m_Iterator == b.m_Iterator;
            }

            friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
                return !(a == b);
            }

            friend bool operator<(const ConstIterator& a, const ConstIterator& b) {
                return a.m_Iterator < b.m_Iterator;
            }

            friend bool operator<=(const ConstIterator& a, const ConstIterator& b) {
                return !(b < a);
            }
            
            friend bool operator>(const ConstIterator& a, const ConstIterator& b) {
                return b < a;
            }

            friend bool operator>=(const ConstIterator& a, const ConstIterator& b) {
                return !(a < b);
            }

        private:
            CircularIterator<const T*> m_Iterator;

            /// @brief Private constructor
            /// @param deque The deque to create the iterator for
            ConstIterator(const Deque& deque) : m_Iterator(wstl::Begin(deque.m_Buffer), wstl::End(deque.m_Buffer)) {}

            /// @brief Private constructor
            /// @param deque The deque to create the iterator for
            /// @param start Pointer to the starting element
            ConstIterator(const Deque& deque, const T* start) : m_Iterator(wstl::Begin(deque.m_Buffer), wstl::End(deque.m_Buffer), start) {}
        };

        /// @brief Circular iterator type for the deque
        class Iterator : public wstl::Iterator<RandomAccessIteratorTag, typename IteratorTraits<T*>::ValueType> {
        public: 
            friend class Deque;
            friend class ConstIterator;

            /// @brief Default constructor
            Iterator() : m_Iterator() {}

            /// @brief Copy constructor
            /// @param other Iterator to copy from
            Iterator(const Iterator& other) :
                m_Iterator(other.m_Iterator) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor
            /// @param other Iterator to move from
            Iterator(Iterator&& other) : m_Iterator(Move(other.m_Iterator)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other Iterator to assign from
            Iterator& operator=(const Iterator& other) {
                m_Iterator = other.m_Iterator;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other Iterator to move from
            Iterator& operator=(Iterator&& other) {
                m_Iterator = Move(other.m_Iterator);
                return *this;
            }
            #endif

            /// @brief Const deference operator
            ReferenceType operator*() const {
                return *m_Iterator;
            }

            /// @brief Const arrow operator
            PointerType operator->() const {
                return m_Iterator.operator->();
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            Iterator& operator++() {
                ++m_Iterator;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            Iterator operator++(int) {
                Iterator original(*this);
                ++(*this);
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            Iterator& operator--() {
                --m_Iterator;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            Iterator operator--(int) {
                Iterator original(*this);
                --(*this);
                return original;
            }

            /// @brief Addition operator - moves the iterator forward by a given offset
            /// @param offset The offset to add (negative for backward movement)
            /// @return Reference to the updated iterator
            Iterator& operator+=(DifferenceType offset) {
                m_Iterator += offset;
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
            ReferenceType operator[](SizeType i) {
                return m_Iterator[i];
            }

            /// @brief Const access operator - allows access to the element at the given index
            /// @param i Index of the element to access
            /// @return Const reference to the element at the given index
            const ReferenceType operator[](SizeType i) const {
                return m_Iterator[i];
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

            /// @brief Subtraction operator - allows subtracting two iterators
            /// @param a The first iterator
            /// @param b The second iterator
            /// @return The difference in positions between the two iterators
            friend DifferenceType operator-(const Iterator& a, const Iterator& b) {
                return a.m_Iterator - b.m_Iterator;
            }

            friend bool operator==(const Iterator& a, const Iterator& b) {
                return a.m_Iterator == b.m_Iterator;
            }

            friend bool operator!=(const Iterator& a, const Iterator& b) {
                return !(a == b);
            }

            friend bool operator<(const Iterator& a, const Iterator& b) {
                return a.m_Iterator < b.m_Iterator;
            }

            friend bool operator<=(const Iterator& a, const Iterator& b) {
                return !(b < a);
            }
            
            friend bool operator>(const Iterator& a, const Iterator& b) {
                return b < a;
            }

            friend bool operator>=(const Iterator& a, const Iterator& b) {
                return !(a < b);
            }

        private:
            CircularIterator<T*> m_Iterator;

            /// @brief Private constructor
            /// @param deque The deque to create the iterator for
            Iterator(Deque& deque) : m_Iterator(wstl::Begin(deque.m_Buffer), wstl::End(deque.m_Buffer)) {}

            /// @brief Private constructor
            /// @param deque The deque to create the iterator for
            /// @param start Pointer to the starting element
            Iterator(Deque& deque, T* start) : m_Iterator(wstl::Begin(deque.m_Buffer), wstl::End(deque.m_Buffer), start) {}
        };

        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        /// @brief Default constructor
        Deque() : TypedContainerBase<T>(SIZE), m_StartIndex(0) {
            Initialize();
        }

        /// @brief Destructor
        ~Deque() {
            Initialize();
        }

        /// @brief Copy constructor
        /// @param other The deque to copy from
        Deque(const Deque& other) : TypedContainerBase<T>(SIZE), m_StartIndex(0) {
            if(this != &other) Assign(other.Begin(), other.End());
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The deque to move from
        Deque(Deque&& other) : TypedContainerBase<T>(SIZE), m_StartIndex(0) {
            if(this != &other) {
                Initialize();

                typename Deque::Iterator it = other.Begin();
                for(; it != other.End(); it++) CreateBack(Move(*it));
            }
        }
        #endif

        /// @brief Constructor that initializes the deque with a range of elements
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        template<typename InputIterator>
        Deque(InputIterator first, InputIterator last) : TypedContainerBase<T>(SIZE), m_StartIndex(0) {
            Assign(first, last);
        }

        /// @brief Constructor that initializes the deque with a specific number of elements
        /// @param count The number of elements to initialize the deque with
        /// @param value The value to initialize each element with (default is ValueType())
        explicit Deque(SizeType count, ConstReferenceType value = ValueType()) : TypedContainerBase<T>(SIZE), m_StartIndex(0) {
            Assign(count, value);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Constructor that initializes the deque with an initializer list
        /// @param list The initializer list to initialize the deque with
        Deque(InitializerList<T> list) : TypedContainerBase<T>(SIZE), m_StartIndex(0) {
            Assign(list);
        }
        #endif

        /// @brief Copy assignment operator
        /// @param other The deque to copy from
        Deque& operator=(const Deque& other) {
            if(this != &other) Assign(other.Begin(), other.End());
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The deque to move from
        Deque& operator=(Deque&& other) {
            if(this != &other) {
                Clear();

                typename Deque::Iterator it = other.Begin();
                for(; it != other.End(); it++) CreateBack(Move(*it));
            }

            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator
        /// @param list The initializer list to assign to the deque
        Deque& operator=(InitializerList<T> list) {
            Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Assigns a range of elements to the deque
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Assign(InputIterator first, InputIterator last) {
            Initialize();
            for(; first != last; ++first) PushBack(__WSTL_MOVE__(*first));
        }

        /// @brief Assigns a specific number of elements to the deque
        /// @param count The number of elements to assign
        /// @param value The value to assign to each element
        /// @throws LengthError if the deque is full
        void Assign(SizeType count, ConstReferenceType value) {
            __WSTL_ASSERT_RETURN__(count <= this->m_Capacity, LengthError("Deque is full", __FILE__, __LINE__));
            Initialize();
            for(; count > 0; count--) PushBack(value);
        }
        
        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Assigns an initializer list to the deque
        /// @param list The initializer list to assign to the deque
        void Assign(InitializerList<T> list) {
            __WSTL_ASSERT_RETURN__(list.Size() <= this->m_Capacity, LengthError("Deque is full", __FILE__, __LINE__));
            Initialize();
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); it++) PushBack(*it);
        }
        #endif

        /// @brief Gets the element at the specified position in the deque
        /// @param position The position of the element to access
        /// @throws OutOfRange if the position is out of range
        ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position < this->m_CurrentSize, OutOfRange("Deque index out of range", __FILE__, __LINE__));
            return m_Buffer[(this->m_StartIndex + position) % this->m_Capacity];
        }

        /// @brief Gets the element at the specified position in the deque
        /// @param position The position of the element to access
        /// @throws OutOfRange if the position is out of range
        ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position < this->m_CurrentSize, OutOfRange("Deque index out of range", __FILE__, __LINE__));
            return m_Buffer[(this->m_StartIndex + position) % this->m_Capacity];
        }

        /// @brief Access operator
        /// @param index The index of the element to access
        ReferenceType operator[](SizeType index) {
            return m_Buffer[(this->m_StartIndex + index) % this->m_Capacity];
        }

        /// @brief Const access operator
        /// @param index The index of the element to access
        ConstReferenceType operator[](SizeType index) const {
            return m_Buffer[(this->m_StartIndex + index) % this->m_Capacity];
        }

        /// @brief Gets the first element in the deque
        ReferenceType Front() {
            return m_Buffer[this->m_StartIndex];
        }

        /// @brief Gets the first element in the deque
        ConstReferenceType Front() const {
            return m_Buffer[this->m_StartIndex];
        }

        /// @brief Gets the last element in the deque
        ReferenceType Back() {
            return m_Buffer[(this->m_StartIndex + this->m_CurrentSize - 1) % this->m_Capacity];
        }

        /// @brief Gets the last element in the deque
        ConstReferenceType Back() const {
            return m_Buffer[(this->m_StartIndex + this->m_CurrentSize - 1) % this->m_Capacity];
        }

        /// @brief Gets iterator to the beginning of the deque
        Iterator Begin() {
            return Iterator(*this, wstl::Begin(m_Buffer) + this->m_StartIndex);
        }

        /// @brief Gets const iterator to the beginning of the deque
        ConstIterator Begin() const {
            return ConstIterator(*this, wstl::ConstBegin(m_Buffer) + this->m_StartIndex);
        }

        /// @brief Gets const iterator to the beginning of the deque
        ConstIterator ConstBegin() const {
            return ConstIterator(*this, wstl::ConstBegin(m_Buffer) + this->m_StartIndex);
        }

        /// @brief Gets iterator to the end of the deque
        Iterator End() {
            return Iterator(*this, wstl::Begin(m_Buffer) + ((this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity));
        }

        /// @brief Gets const iterator to the end of the deque
        ConstIterator End() const {
            return ConstIterator(*this, wstl::ConstBegin(m_Buffer) + ((this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity));
        }

        /// @brief Gets const iterator to the end of the deque
        ConstIterator ConstEnd() const {
            return ConstIterator(*this, wstl::ConstBegin(m_Buffer) + ((this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity));
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
            Initialize();
        }

        /// @brief Inserts an element at specified position in the deque
        /// @param position The position to insert the element at
        /// @param value The value to insert
        /// @return Iterator to the newly inserted element
        /// @throws LengthError if the deque is full
        Iterator Insert(ConstIterator position, ConstReferenceType value) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);
            
            if(pos == Begin()) CreateFront(value);
            else if(pos == End()) CreateBack(value);
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());
                    *--pos = value;
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    *pos = value;
                }
            }

            return pos;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts an element at specified position in the deque
        /// @param position The position to insert the element at
        /// @param value The value to insert (rvalue reference)
        /// @return Iterator to the newly inserted element
        /// @throws LengthError if the deque is full
        Iterator Insert(ConstIterator position, T&& value) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            if(pos == Begin()) CreateFront(Move(value));
            else if(pos == End()) CreateBack(Move(value));
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());
                    pos--;
                    *pos = Move(value);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    *pos = Move(value);
                }
            }

            return pos;
        }
        #endif

        /// @brief Inserts multiple elements at specified position in the deque
        /// @param position The position to insert the elements at
        /// @param count The number of elements to insert
        /// @param value The value to insert
        /// @return Iterator to the first inserted element
        /// @throws LengthError if the deque is full
        Iterator Insert(ConstIterator position, SizeType count, ConstReferenceType value) {
            Iterator result = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(
                count <= this->m_Capacity - this->m_CurrentSize,
                LengthError("Deque is full", __FILE__, __LINE__),
                result
            );

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
                    for(SizeType i = 0; i < createNew; i++) CreateFront(value);

                    // Create copies of old values
                    Iterator from = Begin() + createCopy - 1 + createNew;
                    for(SizeType i = 0; i < createCopy; i++, from--) CreateFront(*from);

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
                    for(SizeType i = 0; i < createNew; i++) CreateBack(value);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; i++) CreateBack(*(result + copyOld + i));

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
        /// @throws LengthError if the deque is full
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, Iterator>::Type
        Insert(ConstIterator position, InputIterator first, InputIterator last) {
            Iterator result = Begin() + Distance(ConstBegin(), position);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(
                count <= this->m_Capacity - this->m_CurrentSize,
                LengthError("Deque is full", __FILE__, __LINE__),
                result
            );

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                CreateFront(first, count);
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(; first != last; first++) CreateBack(__WSTL_MOVE__(*first));
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
                    for(SizeType i = 0; i < createNew; i++, it++) CreateBack(*it);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; i++) CreateBack(*(result + copyOld + i));

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
        /// @throws LengthError if the deque is full
        /// @return Iterator to the first inserted element
        Iterator Insert(ConstIterator position, InitializerList<T> list) {
            Iterator result = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(
                list.Size() <= this->m_Capacity - this->m_CurrentSize,
                LengthError("Deque is full", __FILE__, __LINE__),
                result
            );

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                CreateFront(list.Begin(), list.Size());
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); it++) CreateBack(*it);
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
                    typename InitializerList<T>::Iterator it = list.Begin() + (list.Size() - createNew);
                    for(SizeType i = 0; i < createNew; i++, it++) CreateBack(*it);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; i++) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + list.Size() + copyOld);

                    // Copy new values
                    CopyInRange(list.Begin(), copyNew, result);
                }
            }

            return result;
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param ...args The arguments to forward to the constructor of the element
        /// @throws LengthError if the deque is full 
        /// @return Iterator to the newly emplaced element
        template<typename... Args>
        Iterator Emplace(ConstIterator position, Args&&... args) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(Forward<Args>(args)...);

            return pos;
        }

        #else
        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @throws LengthError if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg>
        Iterator Emplace(ConstIterator position) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T();

            return pos;
        }

        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg The argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg>
        Iterator Emplace(ConstIterator position, const Arg& arg) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(arg);

            return pos;
        }

        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg1, typename Arg2>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(arg1, arg2);

            return pos;
        }

        /// @brief Emplaces an element at specified position in the deque, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full
        /// @return Iterator to the newly emplaced element
        template<typename Arg1, typename Arg2, typename Arg3>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(arg1, arg2, arg3);

            return pos;
        }
        #endif

        /// @brief Erases an element at specified position in the deque
        /// @param position The position of the element to erase
        /// @return Iterator to the element following the erased element
        /// @throws OutOfRange if the position is out of range
        Iterator Erase(Iterator position) {
            __WSTL_ASSERT_RETURNVALUE__(
                Distance(Begin(), position) <= DifferenceType(this->m_CurrentSize), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                position
            );

            if(position == Begin()) {
                DestroyFront();
                position = Begin();
            }
            else if(position == End() - 1) {
                DestroyBack();
                position = End();
            }
            else {
                if(Distance(Begin(), position) <= Distance(position, End())) {
                    MoveBackward(Begin(), position, position + 1);
                    DestroyFront();
                    position++;
                }
                else {
                    Move(position + 1, End(), position);
                    DestroyBack();
                }
            }

            return position;
        }

        /// @brief Erases an element at specified position in the deque
        /// @param position The position of the element to erase
        /// @return Iterator to the element following the erased element
        /// @throws OutOfRange if the position is out of range
        Iterator Erase(ConstIterator position) {
            Iterator result = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(
                Distance(Begin(), result) <= DifferenceType(this->m_CurrentSize), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                result
            );

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
                    MoveBackward(Begin(), result, result + 1);
                    DestroyFront();
                    result++;
                }
                else {
                    Move(result + 1, End(), result);
                    DestroyBack();
                }
            }

            return result;
        }

        /// @brief Erases a range of elements from the deque
        /// @param first Iterator to the first element in the range to erase
        /// @param last Iterator to the element following the last element in the range to erase
        /// @return Iterator to the first element following the erased range
        /// @throws OutOfRange if the range is out of bounds
        Iterator Erase(Iterator first, Iterator last) {
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(
                (Distance(Begin(), first) <= DifferenceType(this->m_CurrentSize)) && (Distance(Begin(), last) <= DifferenceType(this->m_CurrentSize)), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                first
            );

            if(first == Begin()) {
                for(SizeType i = 0; i < count; i++) DestroyFront();
                first = Begin();
            }
            else if(first == End() - count) {
                for(SizeType i = 0; i < count; i++) DestroyBack();
                first = End();
            }
            else {
                if(Distance(Begin(), first) < DifferenceType(this->m_CurrentSize / 2)) {
                    MoveBackward(Begin(), first, first + count);

                    for(SizeType i = 0; i < count; i++) DestroyFront();
                    first += count;
                }
                else {
                    Move(first + count, End(), first);

                    for(SizeType i = 0; i < count; i++) DestroyBack();
                }
            }

            return first;
        }

        /// @brief Erases a range of elements from the deque
        /// @param first Iterator to the first element in the range to erase
        /// @param last Iterator to the element following the last element in the range to erase
        /// @return Iterator to the first element following the erased range
        /// @throws OutOfRange if the range is out of bounds
        Iterator Erase(ConstIterator first, ConstIterator last) {
            Iterator result = Begin() + Distance(ConstBegin(), first);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(
                (Distance(ConstBegin(), first) <= DifferenceType(this->m_CurrentSize)) && (Distance(ConstBegin(), last) <= DifferenceType(this->m_CurrentSize)), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                result
            );

            if(result == Begin()) {
                for(SizeType i = 0; i < count; i++) DestroyFront();
                result = Begin();
            }
            else if(result == End() - count) {
                for(SizeType i = 0; i < count; i++) DestroyBack();
                result = End();
            }
            else {
                if(Distance(Begin(), result) <= DifferenceType(this->m_CurrentSize / 2)) {
                    MoveBackward(Begin(), result, result + count);

                    for(SizeType i = 0; i < count; i++) DestroyFront();
                    result += count;
                }
                else {
                    Move(result + count, End(), result);
                    
                    for(SizeType i = 0; i < count; i++) DestroyBack();
                }
            }

            return result;
        }

        /// @brief Pushes an element to the back of the deque
        /// @param value The value to push to the back
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void PushBack(ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateBack(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes an element to the back of the deque
        /// @param value The value to push to the back
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void PushBack(T&& value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateBack(Forward<T>(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param ...args The arguments to forward to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename... Args>
        void EmplaceBack(Args&&... args) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(Forward<Args>(args)...);
            this->m_CurrentSize++;
        }

        #else
        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void EmplaceBack() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T();
            this->m_CurrentSize++;
        }

        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param arg The argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg>
        void EmplaceBack(const Arg& arg) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(arg);
            this->m_CurrentSize++;
        }

        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(arg1, arg2);
            this->m_CurrentSize++;
        }

        /// @brief Emplaces an element at the back of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(arg1, arg2, arg3);
            this->m_CurrentSize++;
        }
        #endif

        /// @brief Appends a range of elements to the back of the deque
        /// @param container The container to append the elements from
        /// @throws LengthError if the deque is full
        template<typename Container>
        inline void AppendRange(const Container& container) {
            Insert(End(), wstl::Begin(container), wstl::End(container));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Appends a range of elements to the back of the deque, moving them
        /// @param container The container to append the elements from
        /// @throws LengthError if the deque is full
        template<typename Container>
        inline void AppendRange(Container&& container) {
            Insert(End(), MakeMoveIterator(wstl::Begin(container)), 
                MakeMoveIterator(wstl::End(container)));
        }
        #endif

        /// @brief Pops the last element from the deque
        /// @throws OutOfRange if the deque is empty and `__WSTL_CHECK_PUSHPOP__` is defined
        void PopBack() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Empty(), OutOfRange("Deque is empty", __FILE__, __LINE__));
            #endif
            DestroyBack();
        }

        /// @brief Pushes an element to the front of the deque
        /// @param value The value to push to the front
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void PushFront(ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateFront(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes an element to the front of the deque
        /// @param value The value to push to the front
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void PushFront(T&& value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateFront(Forward<T>(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param ...args The arguments to forward to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename... Args>
        void EmplaceFront(Args&&... args) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(Forward<Args>(args)...);
            this->m_CurrentSize++;
        }

        #else
        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void EmplaceFront() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T();
            this->m_CurrentSize++;
        }


        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param arg The argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg>
        void EmplaceFront(const Arg& arg) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(arg);
            this->m_CurrentSize++;
        }

        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(arg1, arg2);
            this->m_CurrentSize++;
        }

        /// @brief Emplaces an element at the front of the deque, constructing it in place
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws LengthError if the deque is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(arg1, arg2, arg3);
            this->m_CurrentSize++;
        }
        #endif

        /// @brief Pops the first element from the deque
        /// @throws OutOfRange if the deque is empty and `__WSTL_CHECK_PUSHPOP__` is defined
        void PopFront() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Empty(), OutOfRange("Deque is empty", __FILE__, __LINE__));
            #endif
            DestroyFront();
        }

        /// @brief Prepends a range of elements to the front of the deque
        /// @param container The container to prepend the elements from
        /// @throws LengthError if the deque is full
        template<typename Container>
        inline void PrependRange(const Container& container) {
            Insert(Begin(), wstl::Begin(container), wstl::End(container));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Prepends a range of elements to the front of the deque, moving them
        /// @param container The container to prepend the elements from
        /// @throws LengthError if the deque is full
        template<typename Container>
        inline void PrependRange(Container&& container) {
            Insert(Begin(), MakeMoveIterator(wstl::Begin(container)), 
                MakeMoveIterator(wstl::End(container)));
        }
        #endif

        /// @brief Resizes the deque to the specified size, filling new elements with a default value
        /// @param count The new size of the deque
        /// @param value The value to fill new elements with, defaults to default constructed value of T
        /// @throws LengthError if the deque is full
        void Resize(SizeType count, ConstReferenceType value = ValueType()) {
            __WSTL_ASSERT_RETURN__(count <= this->m_Capacity, LengthError("Deque is full", __FILE__, __LINE__));

            if(count < this->m_CurrentSize) 
                while(this->m_CurrentSize > count) DestroyBack();
            else if(count > this->m_CurrentSize) {
                SizeType newCount = count - this->m_CurrentSize;
                for(SizeType i = 0; i < newCount; i++) CreateBack(value);
            }
        }

        /// @brief Swaps content of two deques, for trivial types
        /// @param other The deque to swap with
        template<typename U = T>
        typename EnableIf<IsTriviallyCopyable<U>::Value, void>::Type Swap(Deque& other) {
            wstl::Swap(m_Buffer, other.m_Buffer);
            wstl::Swap(m_StartIndex, other.m_StartIndex);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
        }
        
        /// @brief Swaps content of two deques, for non-trivial types
        /// @param other The deque to swap with
        template<typename U = T>
        typename EnableIf<!IsTriviallyCopyable<U>::Value, void>::Type Swap(Deque& other) {
            SizeType minCount = Min(this->m_CurrentSize, other.m_CurrentSize);

            for(SizeType i = 0; i < minCount; i++) {
                T temp = other.Front();
                other.PopFront();
                other.PushBack(Front());
                PopFront();
                PushBack(temp);
            }

            if(this->m_CurrentSize > other.m_CurrentSize) {
                for(SizeType i = other.m_CurrentSize; i < this->m_CurrentSize; i++) {
                    other.PushBack(Front());
                    PopFront();
                }
            }
            else if(this->m_CurrentSize < other.m_CurrentSize) {
                for(SizeType i = this->m_CurrentSize; i < other.m_CurrentSize; i++) {
                    PushBack(other.Front());
                    other.PopFront();
                }
            }
        }
    
    private:
        static const __WSTL_CONSTEXPR__ SizeType m_BufferCapacity = SIZE + 1;

        T m_Buffer[m_BufferCapacity];
        SizeType m_StartIndex;

        /// @brief Initializes the deque
        template<typename U = T>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
            this->m_StartIndex = 0;
        }

        /// @brief Initializes the deque
        template<typename U = T>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->m_CurrentSize > 0) DestroyBack();
            this->m_StartIndex = 0;
        }

        /// @brief Creates an element at the front of the deque
        /// @param value The value to create the element with
        void CreateFront(ConstReferenceType value) {
            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(value);
            this->m_CurrentSize++;
        }

        /// @brief Creates a range of elements at the front of the deque
        /// @param first Iterator to the first element in the range to create
        /// @param count The number of elements to create
        template<typename Iterator>
        void CreateFront(Iterator first, SizeType count) {
            if(count == 0) return;

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - count) % this->m_BufferCapacity;
            this->m_CurrentSize += count;

            for(SizeType i = 0; i < count; i++, first++)
                ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex + i])) T(__WSTL_MOVE__(*first));
        }
        
        /// @brief Creates an element at the back of the deque
        /// @param value The value to create the element with
        void CreateBack(ConstReferenceType value) {
            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(value);
            this->m_CurrentSize++;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Creates an element at the front of the deque
        /// @param value The value to create the element with
        void CreateFront(T&& value) {
            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(Move(value));
            this->m_CurrentSize++;
        }

        /// @brief Creates an element at the back of the deque
        /// @param value The value to create the element with
        void CreateBack(T&& value) {
            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(Move(value));
            this->m_CurrentSize++;
        }
        #endif

        /// @brief Destroys an element at the back of the deque
        void DestroyBack() {
            this->m_CurrentSize--;
            m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity].~T();
        }

        /// @brief Destroys an element at the front of the deque
        void DestroyFront() {
            m_Buffer[this->m_StartIndex].~T();
            this->m_StartIndex = (this->m_StartIndex + 1) % this->m_BufferCapacity;
            this->m_CurrentSize--;
        }
    };

    template<typename T, const size_t SIZE>
    const __WSTL_CONSTEXPR__ typename Deque<T, SIZE>::SizeType Deque<T, SIZE>::m_BufferCapacity;

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename... T>
    Deque(T...) -> Deque<CommonTypeType<T...>, sizeof...(T)>;
    #endif

    // Swap specialization

    /// @brief Swaps content of two deques
    /// @param a First deque
    /// @param b Second deque
    /// @ingroup deque
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/swap2
    template<typename T, const size_t SIZE>
    __WSTL_CONSTEXPR14__ 
    inline void Swap(Deque<T, SIZE>& a, Deque<T, SIZE>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Make deque

    #ifdef __WSTL_CXX11__
    /// @brief Makes a deque out of the given values
    /// @param ...values Values to make the deque with
    /// @return A deque containing the given values
    /// @ingroup deque
    template<typename T, typename... Values>
    constexpr auto MakeDeque(Values&&... values) {
        return { Forward<T>(values)... };
    }
    #endif

    // Comparison operators

    template<typename T, const size_t SIZE>
    inline bool operator==(const Deque<T, SIZE>& a, const Deque<T, SIZE>& b) {
        return (a.Size() == b.Size()) && Equal(a.Begin(), a.End(), b.Begin());
    }

    template<typename T, const size_t SIZE>
    inline bool operator!=(const Deque<T, SIZE>& a, const Deque<T, SIZE>& b) {
        return !(a == b);
    }

    template<typename T, const size_t SIZE>
    inline bool operator<(const Deque<T, SIZE>& a, const Deque<T, SIZE>& b) {
        return LexicographicalCompare(a.Begin(), a.End(), b.Begin(), b.End());
    }

    template<typename T, const size_t SIZE>
    inline bool operator<=(const Deque<T, SIZE>& a, const Deque<T, SIZE>& b) {
        return !(b < a);
    }

    template<typename T, const size_t SIZE>
    inline bool operator>(const Deque<T, SIZE>& a, const Deque<T, SIZE>& b) {
        return b < a;
    }

    template<typename T, const size_t SIZE>
    inline bool operator>=(const Deque<T, SIZE>& a, const Deque<T, SIZE>& b) {
        return !(a < b);
    }
}

#endif