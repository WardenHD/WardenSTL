// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_LIST_HPP__
#define __WSTL_LIST_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "NullPointer.hpp"
#include "TypeTraits.hpp"
#include "private/Error.hpp"


/// @defgroup list List
/// @ingroup containers
/// @brief A doubly linked list container

namespace wstl {
    // List node types

    /// @brief Node type of the list, does not store data
    /// @ingroup list
    struct ListNode {
        ListNode() : Previous(NullPointer), Next(NullPointer) {}

        ListNode* Previous;
        ListNode* Next;
    };

    /// @brief Node type of the list that stores data, represents how elements are stored
    /// @ingroup list
    template<typename T>
    struct ListDataNode : ListNode {
        T Data;
    };

    // Basic list

    /// @brief A doubly linked list that supports pushing and popping elements from both ends
    /// @details Elements in storage are stored as nodes, so storage value type is wrapped in `ListDataNode` type
    /// @tparam Storage The storage type used by the list
    /// @ingroup list
    /// @see https://en.cppreference.com/w/cpp/container/list
    template<typename Storage>
    class BasicList : public TypedContainerBase<
        typename StorageTraits<Storage>::Rebind<ListDataNode<typename Storage::ValueType> >::Other, 
        typename Storage::ValueType
    > {
    private:
        typedef TypedContainerBase<typename StorageTraits<Storage>::Rebind<
            ListDataNode<typename Storage::ValueType> >::Other, typename Storage::ValueType> Base;
        
    public:
        WSTL_STATIC_ASSERT(!IsVoid<Storage>::Value, "Storage must be non-void");

        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;

        typedef typename Base::StorageType StorageType;

        /// @brief Circular const iterator type for the list
        class ConstIterator : public wstl::Iterator<BidirectionalIteratorTag, const ValueType> {
        public:
            typedef typename wstl::Iterator<BidirectionalIteratorTag, const ValueType>::ReferenceType ReferenceType;  
            typedef typename wstl::Iterator<BidirectionalIteratorTag, const ValueType>::PointerType PointerType;
            typedef typename wstl::Iterator<BidirectionalIteratorTag, const ValueType>::DifferenceType DifferenceType;

            friend class BasicList;

            /// @brief Default constructor
            ConstIterator() : m_Current(NullPointer) {}

            /// @brief Copy constructor
            /// @param other Const iterator to copy from
            ConstIterator(const ConstIterator& other) : m_Current(other.m_Current) {}

            /// @brief Copy constructor
            /// @param other Iterator to copy from
            ConstIterator(const typename BasicList::Iterator& other) : m_Current(other.m_Current) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor
            /// @param other Const iterator to move from
            ConstIterator(ConstIterator&& other) : m_Current(Move(other.m_Current)) {}

            /// @brief Move constructor
            /// @param other Iterator to move from
            ConstIterator(typename BasicList::Iterator&& other) : m_Current(Move(other.m_Current)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other Const iterator to assign from
            ConstIterator& operator=(const ConstIterator& other) {
                m_Current = other.m_Current;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other Const iterator to move from
            ConstIterator& operator=(ConstIterator&& other) {
                m_Current = Move(other.m_Current);
                return *this;
            }
            #endif

            /// @brief Const dereference operator
            /// @details If iterator points to the end (i.e. sentinel), behavior is undefined
            /// @return Reference to the element pointed to by the iterator
            ReferenceType operator*() const {
                return DataCast(m_Current)->Data;
            }

            /// @brief Const address-of operator - provides access to the element's address
            /// @details If iterator points to the end (i.e. sentinel), behavior is undefined
            /// @return Pointer to the element pointed to by the iterator
            PointerType operator&() const {
                return &(DataCast(m_Current)->Data);
            }

            /// @brief Const arrow operator - provides access to element's member functions or properties
            /// @details If iterator points to the end (i.e. sentinel), behavior is undefined
            /// @return Pointer to the element pointed to by the iterator
            PointerType operator->() const {
                return &(DataCast(m_Current)->Data);
            }

            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            ConstIterator& operator++() {
                m_Current = m_Current->Next;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            ConstIterator operator++(int) {
                ConstIterator original(*this);
                m_Current = m_Current->Next;
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            ConstIterator& operator--() {
                m_Current = m_Current->Previous;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            ConstIterator operator--(int) {
                ConstIterator original(*this);
                m_Current = m_Current->Previous;
                return original;
            }

            friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
                return a.m_Current == b.m_Current;
            }

            friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
                return !(a == b);
            }
        
        private:
            const ListNode* m_Current;

            /// @brief Prvate constructor
            /// @param current The node the iterator should point to
            ConstIterator(const ListNode* current) : m_Current(current) {}
        };

        /// @brief Circular iterator type for the list
        class Iterator : public wstl::Iterator<BidirectionalIteratorTag, ValueType> {
        public:
            typedef typename wstl::Iterator<BidirectionalIteratorTag, ValueType>::ReferenceType ReferenceType;  
            typedef typename wstl::Iterator<BidirectionalIteratorTag, ValueType>::PointerType PointerType;
            typedef typename wstl::Iterator<BidirectionalIteratorTag, ValueType>::DifferenceType DifferenceType;

            friend class BasicList;
            friend class ConstIterator;

            /// @brief Default constructor
            Iterator() : m_Current(NullPointer) {}

            /// @brief Copy constructor
            /// @param other Iterator to copy from
            Iterator(const Iterator& other) : m_Current(other.m_Current) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor
            /// @param other Iterator to move from
            Iterator(Iterator&& other) : m_Current(Move(other.m_Current)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other Iterator to assign from
            Iterator& operator=(const Iterator& other) {
                m_Current = other.m_Current;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other Iterator to move from
            Iterator& operator=(Iterator&& other) {
                m_Current = Move(other.m_Current);
                return *this;
            }
            #endif

            /// @brief Dereference operator
            /// @details If iterator points to the end (i.e sentinel), behavior is undefined
            /// @return Reference to the element pointed to by the iterator
            ReferenceType operator*() const {
                return DataCast(m_Current)->Data;
            }

            /// @brief Address-of operator - provides access to the element's address
            /// @details If iterator points to the end (i.e sentinel), behavior is undefined
            /// @return Pointer to the element pointed to by the iterator
            PointerType operator&() const {
                return &(DataCast(m_Current)->Data);
            }

            /// @brief Arrow operator - provides access to element's member functions or properties
            /// @details If iterator points to the end (i.e sentinel), behavior is undefined
            /// @return Pointer to the element pointed to by the iterator
            PointerType operator->() const {
                return &(DataCast(m_Current)->Data);
            }
            
            /// @brief Pre-increment operator - moves the iterator forward by one element
            /// @return Reference to the updated iterator
            Iterator& operator++() {
                m_Current = m_Current->Next;
                return *this;
            }

            /// @brief Post-increment operator - moves the iterator forward by one element
            /// @return Reference to the iterator before incrementing
            Iterator operator++(int) {
                Iterator original(*this);
                m_Current = m_Current->Next;
                return original;
            }

            /// @brief Pre-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the updated iterator
            Iterator& operator--() {
                m_Current = m_Current->Previous;
                return *this;
            }

            /// @brief Post-decrement operator - moves the iterator backwards by one element
            /// @return Reference to the iterator before decrementing
            Iterator operator--(int) {
                Iterator original(*this);
                m_Current = m_Current->Previous;
                return original;
            }

            friend bool operator==(const Iterator& a, const Iterator& b) {
                return a.m_Current == b.m_Current;
            }

            friend bool operator!=(const Iterator& a, const Iterator& b) {
                return !(a == b);
            }
        
        private:
            ListNode* m_Current;

            /// @brief Private constructor
            /// @param current The node the iterator should point to
            Iterator(ListNode* current) : m_Current(current) {}
        };

        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        /// @brief Default constructor, for default-constructible storage
        BasicList() : Base(), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
        }

        /// @brief Constructor with custom storage, for non-default-constructible storage
        explicit BasicList(const StorageType& storage) : Base(storage), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
        }

        /// @brief Destructor
        ~BasicList() {
            Initialize<ValueType>();
        }

        /// @brief Constructor that creates a list with specified number of default-inserted elements, for default-constructible storage
        /// @param count Number of elements to create
        /// @throws `LengthError` if `count` exceeds the list's capacity
        explicit BasicList(SizeType count) : Base(), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            while(count--) CreateBack();
        }

        /// @brief Constructor that creates a list with specified number of default-inserted elements, for non-default-constructible storage
        /// @param count Number of elements to create
        /// @param storage Storage to use for the list
        /// @throws `LengthError` if `count` exceeds the list's capacity
        explicit BasicList(SizeType count, const StorageType& storage) : Base(storage), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            while(count--) CreateBack();
        }

        /// @brief Constructor that initializes a list with specified number of elements, for default-constructible storage
        /// @param count Number of elements to create
        /// @param value Value to copy for each element
        /// @throws `LengthError` if `count` exceeds the list's capacity
        BasicList(SizeType count, ConstReferenceType value) : Base(), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            while(count--) CreateBack(value);
        }

        /// @brief Constructor that initializes a list with specified number of elements, for non-default-constructible storage
        /// @param count Number of elements to create
        /// @param value Value to copy for each element
        /// @param storage Storage to use for the list
        /// @throws `LengthError` if `count` exceeds the list's capacity
        BasicList(SizeType count, ConstReferenceType value, const StorageType& storage) : Base(storage), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            while(count--) CreateBack(value);
        }

        /// @brief Constructor that initializes a list with a range of elements, for default-constructible storage
        /// @param first Input iterator to the beginning of the range
        /// @param last Input iterator to the end of the range
        /// @throws `LengthError` if the number of elements in the range exceeds the list's capacity
        template<typename InputIterator>
        BasicList(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
        
            for(; first != last; ++first) CreateBack(*first);
        }

        /// @brief Constructor that initializes a list with a range of elements, for non-default-constructible storage
        /// @param first Input iterator to the beginning of the range
        /// @param last Input iterator to the end of the range
        /// @param storage Storage to use for the list
        /// @throws `LengthError` if the number of elements in the range exceeds the list's capacity
        template<typename InputIterator>
        BasicList(InputIterator first, InputIterator last, const StorageType& storage, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(storage), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
        
            for(; first != last; ++first) CreateBack(*first);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Constructor that initializes a list with an initializer list, for default-constructible storage
        /// @param list Initializer list to initialize the list with
        /// @throws `LengthError` if the number of elements in the initializer list exceeds the list's capacity
        BasicList(InitializerList<ValueType> list) : Base(), m_HeadFree(nullptr) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(list.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
            typename InitializerList<ValueType>::Iterator it = list.Begin();
            for(; it != list.End(); ++it) CreateBack(*it);
        }

        /// @brief Constructor that initializes a list with an initializer list, for non-default-constructible storage
        /// @param list Initializer list to initialize the list with
        /// @param storage Storage to use for the list
        /// @throws `LengthError` if the number of elements in the initializer list exceeds the list's capacity
        BasicList(InitializerList<ValueType> list, const StorageType& storage) : Base(storage), m_HeadFree(nullptr) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(list.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
            typename InitializerList<ValueType>::Iterator it = list.Begin();
            for(; it != list.End(); ++it) CreateBack(*it);
        }
        #endif

        /// @brief Copy constructor, only for default-constructible storage
        /// @param other List to copy from
        /// @throws `LengthError` if the number of elements in `other` exceeds the list's capacity
        BasicList(const BasicList& other) : Base(), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
            typename BasicList::ConstIterator it = other.Begin();
            for(; it != other.End(); ++it) CreateBack(*it);
        }

        /// @brief Copy constructor, only for non-default-constructible storage
        /// @param other List to copy from
        /// @param storage Storage to use for the list
        /// @throws `LengthError` if the number of elements in `other` exceeds the list's capacity
        BasicList(const BasicList& other, const StorageType& storage) : Base(storage), m_HeadFree(NullPointer) {
            Initialize<ValueType>();
            __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
            typename BasicList::ConstIterator it = other.Begin();
            for(; it != other.End(); ++it) CreateBack(*it);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor, only for default-constructible storage
        /// @param other List to move from
        /// @throws `LengthError` if the number of elements in `other` exceeds the list's capacity
        /// @since C++11
        BasicList(BasicList&& other) : Base(), m_HeadFree(nullptr) {
            Initialize<ValueType>();

            if(this != &other) { 
                __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

                typename BasicList::Iterator it = other.Begin();
                for(; it != other.End(); ++it) CreateBack(Move(*it));
            }
        }

        /// @brief Move constructor, only for non-default-constructible storage
        /// @param other List to move from
        /// @param storage Storage to use for the list
        /// @throws `LengthError` if the number of elements in `other` exceeds the list's capacity
        /// @since C++11
        BasicList(BasicList&& other, const StorageType& storage) : Base(storage), m_HeadFree(nullptr) {
            Initialize<ValueType>();

            if(this != &other) {
                __WSTL_ASSERT_RETURN__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

                typename BasicList::Iterator it = other.Begin();
                for(; it != other.End(); ++it) CreateBack(Move(*it));
            }
        }
        #endif

        /// @brief Copy assignment operator
        /// @param other List to copy from
        BasicList& operator=(const BasicList& other) {
            if(this != &other) Assign(other.Begin(), other.End());
            return *this;
        }
        
        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other List to move from
        /// @since C++11
        BasicList& operator=(BasicList&& other) {
            if(this != &other) {
                __WSTL_ASSERT_RETURNVALUE__(other.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"), *this);
                Initialize<ValueType>();

                typename BasicList::Iterator it = other.Begin();
                for(; it != other.End(); ++it) CreateBack(Move(*it));
            }

            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator
        /// @param list The initializer list to assign to the list
        /// @throws `LengthError` if list size exceeds the list's capacity
        /// @since C++11
        BasicList& operator=(InitializerList<ValueType> list) {
            Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Assigns a specific number of elements to the list
        /// @param count The number of elements to assign
        /// @param value The value to assign to each element
        /// @throws `LengthError` if the list's capacity is exceeded
        void Assign(SizeType count, ConstReferenceType value) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            Initialize<ValueType>();
            while(count--) CreateBack(value);
        }

        /// @brief Assigns a range of elements to the list
        /// @param first Input iterator to the beginning of the range
        /// @param last Input iterator to the end of the range
        /// @throws `LengthError` if the number of elements in the range exceeds the list's capacity
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Assign(InputIterator first, InputIterator last) {
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
            Initialize<ValueType>();
            for(; first != last; ++first) CreateBack(*first);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Assigns an initializer list to the list
        /// @param list The initializer list to assign to the list
        /// @throws `LengthError` if the number of elements in the initializer list exceeds the list's capacity
        /// @since C++11
        void Assign(InitializerList<ValueType> list) {
            __WSTL_ASSERT_RETURN__(list.Size() <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            Initialize<ValueType>();
            for(typename InitializerList<ValueType>::Iterator it = list.Begin(); it != list.End(); ++it) CreateBack(*it);
        }
        #endif

        /// @brief Assigns a range of elements to the list
        /// @param range The range to assign to the list
        /// @throws `LengthError` if the number of elements in the range exceeds the list's capacity
        template<typename Range>
        inline void AssignRange(const Range& range) {
            Assign(wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Assigns a range of elements to the list using move semantics
        /// @param range The range to assign to the list
        /// @throws `LengthError` if the number of elements in the range exceeds the list's capacity
        template<typename Range>
        inline void AssignRange(Range&& range) {
            Assign(MakeMoveIterator(wstl::Begin(range)), 
                MakeMoveIterator(wstl::End(range)));
        }
        #endif

        /// @brief Gets the first element of the list
        ReferenceType Front() {
            return DataCast(HeadNode())->Data;
        }

        /// @brief Gets the first element of the list
        ConstReferenceType Front() const {
            return DataCast(HeadNode())->Data;
        }

        /// @brief Gets the last element of the list
        ReferenceType Back() {
            return DataCast(TailNode())->Data;
        }

        /// @brief Gets the last element of the list
        ConstReferenceType Back() const {
            return DataCast(TailNode())->Data;
        }

        /// @brief Gets an iterator to the beginning of the list
        Iterator Begin() {
            return Iterator(HeadNode());
        }

        /// @brief Gets a const iterator to the beginning of the list
        ConstIterator Begin() const {
            return ConstIterator(HeadNode());
        }

        /// @brief Gets a const iterator to the beginning of the list
        ConstIterator ConstBegin() {
            return ConstIterator(HeadNode());
        }

        /// @brief Gets an iterator to the end of the list
        Iterator End() {
            return Iterator(&m_Sentinel);
        }

        /// @brief Gets a const iterator to the end of the list
        ConstIterator End() const {
            return ConstIterator(&m_Sentinel);
        }

        /// @brief Gets a const iterator to the end of the list
        ConstIterator ConstEnd() {
            return ConstIterator(&m_Sentinel);
        }

        /// @brief Gets a reverse iterator to the beginning of the list
        ReverseIterator ReverseBegin() {
            return ReverseIterator(End());
        }

        /// @brief Gets a const reverse iterator to the beginning of the list
        ConstReverseIterator ReverseBegin() const {
            return ConstReverseIterator(End());
        }

        /// @brief Gets a const reverse iterator to the beginning of the list
        ConstReverseIterator ConstReverseBegin() const {
            return ConstReverseIterator(End());
        }

        /// @brief Gets a reverse iterator to the end of the list
        ReverseIterator ReverseEnd() {
            return ReverseIterator(Begin());
        }

        /// @brief Gets a const reverse iterator to the end of the list
        ConstReverseIterator ReverseEnd() const {
            return ConstReverseIterator(Begin());
        }

        /// @brief Gets a const reverse iterator to the end of the list
        ConstReverseIterator ConstReverseEnd() const {
            return ConstReverseIterator(Begin());
        }
        
        /// @brief Clears the list, removing all elements
        void Clear() {
            Initialize<ValueType>();
        }

        /// @brief Inserts an element at the specified position
        /// @param position The position to insert the element at
        /// @param value The value of the element to insert
        /// @return An iterator to the inserted element
        /// @throws `LengthError` if the list is full
        Iterator Insert(ConstIterator position, ConstReferenceType value) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType(value);
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts an element at the specified position using move semantics
        /// @param position The position to insert the element at
        /// @param value The value of the element to insert
        /// @return An iterator to the inserted element
        /// @throws `LengthError` if the list is full
        /// @since C++11
        Iterator Insert(ConstIterator position, ValueType&& value) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType(Move(value));
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }
        #endif

        /// @brief Inserts a specific number of elements at the specified position
        /// @param position The position to insert the elements at
        /// @param count The number of elements to insert
        /// @param value The value of the elements to insert
        /// @return An iterator to the first inserted element
        /// @throws `LengthError` if the list's capacity is exceeded
        Iterator Insert(ConstIterator position, SizeType count, ConstReferenceType value) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(count <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"), insert);

            ListNode* const result = m_HeadFree;
            ListDataNode<ValueType>* nextFree;

            this->m_CurrentSize += count;

            while(count--) {
                nextFree = DataCast(m_HeadFree->Next);

                ::new(&(m_HeadFree->Data)) ValueType(value);
                LinkNodeBefore(insert.m_Current, m_HeadFree);

                m_HeadFree = nextFree;
            }
            
            return Iterator(result);
        }

        /// @brief Inserts a range of elements at the specified position
        /// @param position The position to insert the elements at
        /// @param first Input iterator to the beginning of the range
        /// @param last Input iterator to the end of the range
        /// @return An iterator to the first inserted element
        /// @throws `LengthError` if the list's capacity is exceeded
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, Iterator>::Type
        Insert(ConstIterator position, InputIterator first, InputIterator last) {
            Iterator insert = ToIterator(position);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(count <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"), insert);

            ListNode* const result = m_HeadFree;
            ListDataNode<ValueType>* nextFree;

            this->m_CurrentSize += count;

            for(; first != last; ++first) {
                nextFree = DataCast(m_HeadFree->Next);

                ::new(&(m_HeadFree->Data)) ValueType(*first);
                LinkNodeBefore(insert.m_Current, m_HeadFree);

                m_HeadFree = nextFree;
            }
            
            return Iterator(result);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Inserts elements from an initializer list at the specified position
        /// @param position The position to insert the elements at
        /// @param list The initializer list to insert from
        /// @return An iterator to the first inserted element
        /// @throws `LengthError` if the list's capacity is exceeded
        /// @since C++11
        Iterator Insert(ConstIterator position, InitializerList<ValueType> list) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(list.Size() <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"), insert);

            ListNode* const result = m_HeadFree;
            ListDataNode<ValueType>* nextFree;

            this->m_CurrentSize += list.Size();

            for(typename InitializerList<ValueType>::Iterator it = list.Begin(); it != list.End(); ++it) {
                nextFree = DataCast(m_HeadFree->Next);

                ::new(&(m_HeadFree->Data)) ValueType(*it);
                LinkNodeBefore(insert.m_Current, m_HeadFree);

                m_HeadFree = nextFree;
            }
            
            return Iterator(result);
        }
        #endif

        /// @brief Inserts a range of elements at the specified position
        /// @param position The position to insert the elements at
        /// @param range The range to insert from
        /// @return An iterator to the first inserted element
        /// @throws `LengthError` if the list's capacity is exceeded
        template<typename Range>
        inline Iterator InsertRange(ConstIterator position, const Range& range) {
            return Insert(position, wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts a range of elements at the specified position using move semantics
        /// @param position The position to insert the elements at
        /// @param range The range to insert from
        /// @return An iterator to the first inserted element
        /// @throws `LengthError` if the list's capacity is exceeded
        template<typename Range>
        inline Iterator InsertRange(ConstIterator position, Range&& range) {
            return Insert(position, MakeMoveIterator(wstl::Begin(range)), MakeMoveIterator(wstl::End(range)));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the specified position, constructing it in place
        /// @param position The position to emplace the element at
        /// @param ...args Arguments to forward to the constructor of the element
        /// @return An iterator to the emplaced element
        /// @throws `LengthError` if the list is full
        /// @since C++11
        template<typename... Args>
        Iterator Emplace(ConstIterator position, Args&&... args) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType(Forward<Args>(args)...);
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }
        #else
        /// @brief Emplaces an element at the specified position, constructing it in place
        /// @param position The position to emplace the element at
        /// @return An iterator to the emplaced element
        /// @throws `LengthError` if the list is full
        Iterator Emplace(ConstIterator position) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType();
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }

        /// @brief Emplaces an element at the specified position, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg Argument to forward to the constructor of the element
        /// @return An iterator to the emplaced element
        /// @throws `LengthError` if the list is full
        template<typename Arg>
        Iterator Emplace(ConstIterator position, const Arg& arg) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType(arg);
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }

        /// @brief Emplaces an element at the specified position, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg1 First argument to forward to the constructor of the element
        /// @param arg2 Second argument to forward to the constructor of the element
        /// @return An iterator to the emplaced element
        /// @throws `LengthError` if the list is full
        template<typename Arg1, typename Arg2>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType(arg1, arg2);
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }

        /// @brief Emplaces an element at the specified position, constructing it in place
        /// @param position The position to emplace the element at
        /// @param arg1 First argument to forward to the constructor of the element
        /// @param arg2 Second argument to forward to the constructor of the element
        /// @param arg3 Third argument to forward to the constructor of the element
        /// @return An iterator to the emplaced element
        /// @throws `LengthError` if the list is full
        template<typename Arg1, typename Arg2, typename Arg3>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            Iterator insert = ToIterator(position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"), insert);

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);
            ListNode* const result = m_HeadFree;

            ::new(&(m_HeadFree->Data)) ValueType(arg1, arg2, arg3);
            LinkNodeBefore(insert.m_Current, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;

            return Iterator(result);
        }
        #endif

        /// @brief Erases the element at the specified position
        /// @param position The position of the element to erase
        /// @return An iterator to the element following the erased element
        Iterator Erase(ConstIterator position) {
            Iterator result = ToIterator(position);

            ++result;
            DestroyNode(DataCast(result.m_Current->Previous));
            return result;
        }

        /// @brief Erases a range of elements between the specified positions
        /// @param first The position of the first element to erase
        /// @param last The position following the last element to erase
        /// @return An iterator to the element following the last erased element
        Iterator Erase(ConstIterator first, ConstIterator last) {
            Iterator begin = ToIterator(first);
            Iterator end = ToIterator(last);

            while(begin != end) {
                ListDataNode<ValueType>* node = DataCast(begin.m_Current);
                ++begin;
                DestroyNode(node);
            }
            
            return end;
        }

        /// @brief Pushes an element to the back of the list
        /// @param value The value of the element to push
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PushBack(ConstReferenceType value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));
            CreateBack(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes an element to the back of the list using move semantics
        /// @param value The value of the element to push (rvalue reference)
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PushBack(ValueType&& value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));
            CreateBack(Move(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the back of the list, constructing it in place
        /// @param ...args Arguments to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename... Args>
        void EmplaceBack(Args&&... args) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(Forward<Args>(args)...);
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        #else
        /// @brief Emplaces an element at the back of the list, constructing it in place
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void EmplaceBack() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType();
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the back of the list, constructing it in place
        /// @param arg Argument to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg>
        void EmplaceBack(const Arg& arg) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(arg);
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the back of the list, constructing it in place
        /// @param arg1 First argument to forward to the constructor of the element
        /// @param arg2 Second argument to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(arg1, arg2);
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the back of the list, constructing it in place
        /// @param arg1 First argument to forward to the constructor of the element
        /// @param arg2 Second argument to forward to the constructor of the element
        /// @param arg3 Third argument to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(arg1, arg2, arg3);
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Appends a range of elements to the back of the list
        /// @param range The range to append
        /// @throws `LengthError` if list's capacity is exceeded
        template<typename Range>
        inline void AppendRange(const Range& range) {
            Insert(End(), wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Appends a range of elements to the back of the list using move semantics
        /// @param range The range to append (rvalue reference)
        /// @throws `LengthError` if list's capacity is exceeded
        template<typename Range>
        inline void AppendRange(Range&& range) {
            Insert(End(), MakeMoveIterator(wstl::Begin(range)), 
                MakeMoveIterator(wstl::End(range)));
        }
        #endif

        /// @brief Pops an element from the back of the list
        /// @throws `LengthError` if the list is empty and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PopBack() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "List empty"));
            DestroyBack();
        }

        /// @brief Pushes an element to the front of the list
        /// @param value The value of the element to push
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PushFront(ConstReferenceType value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));
            CreateFront(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes an element to the front of the list using move semantics
        /// @param value The value of the element to push (rvalue reference)
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PushFront(ValueType&& value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));
            CreateFront(Move(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Emplaces an element at the front of the list, constructing it in place
        /// @param ...args Arguments to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename... Args>
        void EmplaceFront(Args&&... args) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(Forward<Args>(args)...);
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        #else
        /// @brief Emplaces an element at the front of the list, constructing it in place
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void EmplaceFront() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType();
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the front of the list, constructing it in place
        /// @param arg Argument to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg>
        void EmplaceFront(const Arg& arg) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(arg);
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        
        /// @brief Emplaces an element at the front of the list, constructing it in place
        /// @param arg1 First argument to forward to the constructor of the element
        /// @param arg2 Second argument to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(arg1, arg2);
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Emplaces an element at the front of the list, constructing it in place
        /// @param arg1 First argument to forward to the constructor of the element
        /// @param arg2 Second argument to forward to the constructor of the element
        /// @param arg3 Third argument to forward to the constructor of the element
        /// @throws `LengthError` if the list is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "List full"));

            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(arg1, arg2, arg3);
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Pops an element from the front of the list
        /// @throws `LengthError` if the list is empty and `__WSTL_ASSERT_PUSHPOP__` is defined
        void PopFront() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "List empty"));
            DestroyFront();
        }

        /// @brief Prepends a range of elements to the front of the list
        /// @param range The range to prepend
        /// @throws `LengthError` if list's capacity is exceeded
        template<typename Range>
        inline void PrependRange(const Range& range) {
            Insert(Begin(), wstl::Begin(range), wstl::End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Prepends a range of elements to the front of the list using move semantics
        /// @param range The range to prepend (rvalue reference)
        /// @throws `LengthError` if list's capacity is exceeded
        template<typename Range>
        inline void PrependRange(Range&& range) {
            Insert(Begin(), MakeMoveIterator(wstl::Begin(range)), 
                MakeMoveIterator(wstl::End(range)));
        }
        #endif

        /// @brief Resizes the list to the specified size, filling new elements with a default value
        /// @param count The new size of the list
        /// @throws `LengthError` if the new size exceeds the list's capacity
        void Resize(SizeType count) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            if(count < this->m_CurrentSize) {
                while(this->m_CurrentSize > count) DestroyBack();
            }
            else if(count > this->m_CurrentSize) {
                SizeType newCount = count - this->m_CurrentSize;
                for(SizeType i = 0; i < newCount; ++i) CreateBack();
            }
        }

        /// @brief Resizes the list to the specified size, filling new elements with a specified value
        /// @param count The new size of the list
        /// @param value The value to fill new elements with
        /// @throws `LengthError` if the new size exceeds the list's capacity
        void Resize(SizeType count, ConstReferenceType value) {
            __WSTL_ASSERT_RETURN__(count <= this->Capacity(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

            if(count < this->m_CurrentSize) {
                while(this->m_CurrentSize > count) DestroyBack();
            }
            else if(count > this->m_CurrentSize) {
                SizeType newCount = count - this->m_CurrentSize;
                for(SizeType i = 0; i < newCount; ++i) CreateBack(value);
            }
        }

        /// @brief Swaps content of two lists
        /// @param other The list to swap with
        void Swap(BasicList& other) __WSTL_NOEXCEPT_EXPR__(noexcept(__Swap<StorageType>(other))) {
            __Swap<StorageType>(other);
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted using a comparator
        /// @param other The list to merge
        /// @param compare Binary comparator to use for merging
        template<typename Compare>
        void Merge(BasicList& other, Compare compare) {
            if(this != &other && !other.Empty()) {
                Iterator thisBegin = Begin();
                Iterator thisEnd = End();
                Iterator otherBegin = other.Begin();
                Iterator otherEnd = other.End();

                while((thisBegin != thisEnd) && (otherBegin != otherEnd)) {
                    while((otherBegin != otherEnd) && compare(*otherBegin, *thisBegin)) {
                        Insert(thisBegin, *otherBegin);
                        ++otherBegin;
                    }

                    ++thisBegin;
                }

                // Append leftover
                if((thisBegin == thisEnd) && (otherBegin != otherEnd)) 
                    Insert(thisEnd, otherBegin, otherEnd);

                other.Clear();
            }
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted
        /// @param other The list to merge
        inline void Merge(BasicList& other) {
            Merge(other, Less<ValueType>());
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted using a comparator
        /// @param other The list to merge
        /// @param compare Binary comparator to use for merging
        template<typename UStorage, typename Compare>
        void Merge(BasicList<UStorage>& other, Compare compare) {
            WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");

            if(!other.Empty()) {
                Iterator thisBegin = Begin();
                Iterator thisEnd = End();
                typename BasicList<UStorage>::Iterator otherBegin = other.Begin();
                typename BasicList<UStorage>::Iterator otherEnd = other.End();

                while((thisBegin != thisEnd) && (otherBegin != otherEnd)) {
                    while((otherBegin != otherEnd) && compare(*otherBegin, *thisBegin)) {
                        Insert(thisBegin, *otherBegin);
                        ++otherBegin;
                    }

                    ++thisBegin;
                }

                // Append leftover
                if((thisBegin == thisEnd) && (otherBegin != otherEnd)) 
                    Insert(thisEnd, otherBegin, otherEnd);

                other.Clear();
            }
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted
        /// @param other The list to merge
        template<typename UStorage>
        inline void Merge(BasicList<UStorage>& other) {
            Merge(other, Less<ValueType>());
        }

        #ifdef __WSTL_CXX11__
        /// @brief Merges another sorted list into this list, providing that it's sorted using a comparator
        /// @param other The list to merge (rvalue reference)
        /// @param compare Binary comparator to use for merging
        template<typename Compare>
        void Merge(BasicList&& other, Compare compare) {
            if(this != &other && !other.Empty()) {
                Iterator thisBegin = Begin();
                Iterator thisEnd = End();
                Iterator otherBegin = other.Begin();
                Iterator otherEnd = other.End();

                while((thisBegin != thisEnd) && (otherBegin != otherEnd)) {
                    while((otherBegin != otherEnd) && compare(*otherBegin, *thisBegin)) {
                        Insert(thisBegin, Move(*otherBegin));
                        ++otherBegin;
                    }

                    ++thisBegin;
                }

                // Append leftover
                if((thisBegin == thisEnd) && (otherBegin != otherEnd))
                    while(otherBegin != otherEnd) {
                        Insert(thisEnd, Move(*otherBegin));
                        ++otherBegin;
                    }

                other.Clear();
            }
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted
        /// @param other The list to merge (rvalue reference)
        inline void Merge(BasicList&& other) {
            Merge(Move(other), Less<ValueType>());
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted using a comparator
        /// @param other The list to merge (rvalue reference)
        /// @param compare Binary comparator to use for merging
        template<typename UStorage, typename Compare>
        void Merge(BasicList<UStorage>&& other, Compare compare) {
            WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");

            if(!other.Empty()) {
                Iterator thisBegin = Begin();
                Iterator thisEnd = End();
                typename BasicList<UStorage>::Iterator otherBegin = other.Begin();
                typename BasicList<UStorage>::Iterator otherEnd = other.End();

                while((thisBegin != thisEnd) && (otherBegin != otherEnd)) {
                    while((otherBegin != otherEnd) && compare(*otherBegin, *thisBegin)) {
                        Insert(thisBegin, Move(*otherBegin));
                        ++otherBegin;
                    }

                    ++thisBegin;
                }

                // Append leftover
                if((thisBegin == thisEnd) && (otherBegin != otherEnd))
                    while(otherBegin != otherEnd) {
                        Insert(thisEnd, Move(*otherBegin));
                        ++otherBegin;
                    }

                other.Clear();
            }
        }

        /// @brief Merges another sorted list into this list, providing that it's sorted
        /// @param other The list to merge (rvalue reference)
        template<typename UStorage>
        inline void Merge(BasicList<UStorage>&& other) {
            Merge(Move(other), Less<ValueType>());
        }
        #endif

        /// @brief Splices another list into this list at the specified position by transferring elements
        /// @param position The position to splice at
        /// @param other The list to splice from
        void Splice(ConstIterator position, BasicList& other) {
            if(this != &other) {
                Insert(position, other.Begin(), other.End());
                other.Clear();
            }
        }

        /// @brief Splices another list into this list at the specified position by transferring elements
        /// @param position The position to splice at
        /// @param other The list to splice from
        template<typename UStorage>
        void Splice(ConstIterator position, BasicList<UStorage>& other) {
            WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");
            Insert(position, other.Begin(), other.End());
            other.Clear();
        }

        #ifdef __WSTL_CXX11__
        /// @brief Splices another list into this list at the specified position by transferring elements using move semantics
        /// @param position The position to splice at
        /// @param other The list to splice from (rvalue reference)
        void Splice(ConstIterator position, BasicList&& other) {
            if(this != &other) {
                __WSTL_ASSERT_RETURN__(other.Size() <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));

                this->m_CurrentSize += other.Size();

                Iterator it = other.Begin();
                for(; it != other.End(); ++it) {
                    ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

                    ::new(&(m_HeadFree->Data)) ValueType(Move(*it));
                    LinkNodeBefore(ToIterator(position).m_Current, m_HeadFree);

                    m_HeadFree = nextFree;
                }

                other.Clear();
            }
        }

        /// @brief Splices another list into this list at the specified position by transferring elements using move semantics
        /// @param position The position to splice at
        /// @param other The list to splice from (rvalue reference)
        template<typename UStorage>
        void Splice(ConstIterator position, BasicList<UStorage>&& other) {
            WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");
            
            __WSTL_ASSERT_RETURN__(other.Size() <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
            this->m_CurrentSize += other.Size();

            typename BasicList<UStorage>::Iterator it = other.Begin();
            for(; it != other.End(); ++it) {
                ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

                ::new(&(m_HeadFree->Data)) ValueType(Move(*it));
                LinkNodeBefore(ToIterator(position).m_Current, m_HeadFree);

                m_HeadFree = nextFree;
            }

            other.Clear();
        }
        #endif

        /// @brief Splices an element from another list into this list at the specified position by transferring it
        /// @param position The position to splice at
        /// @param other The list to splice from
        /// @param iterator The position of the element in the other list to splice
        void Splice(ConstIterator position, BasicList& other, ConstIterator iterator) {
            if(this == &other) MoveNode(ToIterator(position).m_Current, ToIterator(iterator).m_Current);
            else {
                Insert(position, *iterator);
                other.Erase(iterator);
            }
        }

        /// @brief Splices an element from another list into this list at the specified position by transferring it
        /// @param position The position to splice at
        /// @param other The list to splice from
        /// @param iterator The position of the element in the other list to splice
        template<typename UStorage>
        void Splice(ConstIterator position, BasicList<UStorage>& other, typename BasicList<UStorage>::ConstIterator iterator) {
            WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");
            Insert(position, *iterator);
            other.Erase(iterator);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Splices an element from another list into this list at the specified position by transferring it using move semantics
        /// @param position The position to splice at
        /// @param other The list to splice from (rvalue reference)
        /// @param iterator The position of the element in the other list to splice
        void Splice(ConstIterator position, BasicList&& other, ConstIterator iterator) {
            if(this == &other) MoveNode(ToIterator(position).m_Current, ToIterator(iterator).m_Current);
            else {
                Insert(position, Move(*iterator));
                other.Erase(iterator);
            }
        }

        /// @brief Splices an element from another list into this list at the specified position by transferring it using move semantics
        /// @param position The position to splice at
        /// @param other The list to splice from (rvalue reference)
        /// @param iterator The position of the element in the other list to splice
        template<typename UStorage>
        void Splice(ConstIterator position, BasicList<UStorage>&& other, typename BasicList<UStorage>::ConstIterator iterator) {
            WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");
            Insert(position, Move(*iterator));
            other.Erase(iterator);
        }
        #endif

        /// @brief Splices a range of elements from another list into this list at the specified position by transferring them
        /// @param position The position to splice at
        /// @param other The list to splice from
        /// @param first The position of the first element in the other list to splice
        /// @param last The position following the last element in the other list to splice
        void Splice(ConstIterator position, BasicList& other, ConstIterator first, ConstIterator last) {
            if(this == &other) 
                MoveNodeRange(ToIterator(position).m_Current, ToIterator(first).m_Current, 
                    ToIterator(last).m_Current);
            else {
                Insert(position, first, last);
                other.Erase(first, last);
            }
        }

        /// @brief Splices a range of elements from another list into this list at the specified position by transferring them
        /// @param position The position to splice at
        /// @param other The list to splice from
        /// @param first The position of the first element in the other list to splice
        /// @param last The position following the last element in the other list to splice
        template<typename UStorage>
        void Splice(ConstIterator position, BasicList<UStorage>& other, typename BasicList<UStorage>::ConstIterator first, 
            typename BasicList<UStorage>::ConstIterator last) {
                WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");
                Insert(position, first, last);
                other.Erase(first, last);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Splices a range of elements from another list into this list at the specified position by transferring them using move semantics
        /// @param position The position to splice at
        /// @param other The list to splice from (rvalue reference)
        /// @param first The position of the first element in the other list to splice
        /// @param last The position following the last element in the other list to splice
        void Splice(ConstIterator position, BasicList&& other, ConstIterator first, ConstIterator last) {
            if(this == &other) 
                MoveNodeRange(ToIterator(position).m_Current, ToIterator(first).m_Current, 
                    ToIterator(last).m_Current);
            else {
                SizeType count = Distance(first, last);
                __WSTL_ASSERT_RETURN__(count <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
                this->m_CurrentSize += count;

                Iterator it = other.ToIterator(first);
                for(; it != last; ++it) {
                    ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

                    ::new(&(m_HeadFree->Data)) ValueType(Move(*it));
                    LinkNodeBefore(ToIterator(position).m_Current, m_HeadFree);

                    m_HeadFree = nextFree;
                }

                other.Erase(first, last);
            }
        }

        /// @brief Splices a range of elements from another list into this list at the specified position by transferring them using move semantics
        /// @param position The position to splice at
        /// @param other The list to splice from (rvalue reference)
        /// @param first The position of the first element in the other list to splice
        /// @param last The position following the last element in the other list to splice
        template<typename UStorage>
        void Splice(ConstIterator position, BasicList<UStorage>&& other, typename BasicList<UStorage>::ConstIterator first,
            typename BasicList<UStorage>::ConstIterator last) {
                WSTL_STATIC_ASSERT((IsAssignable<typename UStorage::ValueType, ValueType>::Value), "Can't assign values from another list");

                SizeType count = Distance(first, last);
                __WSTL_ASSERT_RETURN__(count <= this->Available(), WSTL_MAKE_EXCEPTION(LengthError, "List overflow"));
            
                this->m_CurrentSize += count;

                typename BasicList<UStorage>::Iterator it = other.ToIterator(first);
                for(; it != last; ++it) {
                    ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

                    ::new(&(m_HeadFree->Data)) ValueType(Move(*it));
                    LinkNodeBefore(ToIterator(position).m_Current, m_HeadFree);

                    m_HeadFree = nextFree;
                }

                other.Erase(first, last);
        }
        #endif

        /// @brief Removes all occurrences of the specified value from the list
        /// @param value The value to remove
        /// @return The number of elements removed
        SizeType Remove(ConstReferenceType value) {
            SizeType count = 0;
            Iterator it = Begin();

            while(it != End()) {
                if(*it == value) {
                    ++count;
                    it = Erase(it);
                }
                else ++it;
            }

            return count;
        }

        /// @brief Removes all elements satisfying the specified predicate from the list
        /// @param predicate Unary predicate to determine which elements to remove
        /// @return The number of elements removed
        template<typename UnaryPredicate>
        SizeType RemoveIf(UnaryPredicate predicate) {
            SizeType count = 0;
            Iterator it = Begin();

            while(it != End()) {
                if(predicate(*it)) {
                    ++count;
                    it = Erase(it);
                }
                else ++it;
            }

            return count;
        }

        /// @brief Reverses the order of elements in the list
        void Reverse() {
            if(this->Size() < 2) return;

            for(ListNode* i = HeadNode(); i != &m_Sentinel; i = i->Previous) 
                wstl::Swap(i->Previous, i->Next);

            wstl::Swap(m_Sentinel.Previous, m_Sentinel.Next);
        }

        /// @brief Removes consecutive duplicate elements from the list
        /// @return The number of elements removed
        SizeType Unique() {
            if(this->Empty()) return 0;
            SizeType count = 0;

            Iterator it = ++Begin();
            Iterator previous = Begin();

            while(it != End()) {
                if(*it == *previous) {
                    ++count;
                    it = Erase(it);
                }
                else {
                    previous = it;
                    ++it;
                }
            }

            return count;
        }

        /// @brief Removes consecutive duplicate elements from the list based on a provided binary predicate
        /// @param predicate Binary predicate to determine if two elements are considered duplicates
        /// @return The number of elements removed
        template<typename BinaryPredicate>
        SizeType Unique(BinaryPredicate predicate) {
            if(this->Empty()) return 0;
            SizeType count = 0;

            Iterator it = ++Begin();
            Iterator previous = Begin();

            while(it != End()) {
                if(predicate(*it, *previous)) {
                    ++count;
                    it = Erase(it);
                }
                else {
                    previous = it;
                    ++it;
                }
            }

            return count;
        }

        /*
        * Portions of this implementation are based on Simon Tatham's linked list mergesort.
        * Modified and adapted for WardenSTL.
        * 
        * Copyright 2001 Simon Tatham.
        * 
        * Permission is hereby granted, free of charge, to any person
        * obtaining a copy of this software and associated documentation
        * files (the "Software"), to deal in the Software without
        * restriction, including without limitation the rights to use,
        * copy, modify, merge, publish, distribute, sublicense, and/or
        * sell copies of the Software, and to permit persons to whom the
        * Software is furnished to do so, subject to the following
        * conditions:
        * 
        * The above copyright notice and this permission notice shall be
        * included in all copies or substantial portions of the Software.
        * 
        * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
        * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
        * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
        * NONINFRINGEMENT.  IN NO EVENT SHALL SIMON TATHAM BE LIABLE FOR
        * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
        * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
        * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
        * SOFTWARE.
        */

        /// @brief Sorts the list using the provided comparison function
        /// @details Uses a stable bottom-up merge sort algorithm
        /// @param compare Binary comparator function that returns a boolean value
        template<typename Compare>
        void Sort(Compare compare) {
            if(this->Size() < 2) return;

            Iterator left;
            Iterator node;
            Iterator right;
            Iterator tail;
            SizeType leftSize;
            SizeType rightSize;

            for(SizeType i = 1; i < this->Size(); i *= 2) {
                left = Begin();
                tail = End();
                
                while(left != End()) {
                    right = left;
                    leftSize = 0;

                    // Step 'i' places along from left
                    for(SizeType j = 0; j < i && right != End(); ++j) {
                        ++leftSize;
                        ++right;
                    }

                    // If right hasn't fallen off end, we have two lists to merge
                    rightSize = i;

                    // Now we have two lists. Merge them
                    while(leftSize > 0 || (rightSize > 0 && right != End())) {
                        // Decide which node to merge, left or right
                        if(leftSize == 0) {
                            // Left is empty, use node from right
                            node = right++;
                            --rightSize;
                        }
                        else if(rightSize == 0 || right == End() || !compare(*right, *left)) {
                            // Right is empty or left is smaller or equal, use node from left
                            node = left++;
                            --leftSize;
                        }
                        else {
                            // Right is lower, use node from right
                            node = right++;
                            --rightSize;
                        }

                        // Merge the node to the tail
                        LinkNodes(tail.m_Current, node.m_Current);
                        tail = node;
                    }

                    // Link new tail to the sentinel node
                    LinkNodes(tail.m_Current, &m_Sentinel);

                    // Now left has stepped `i' places along, and right has too 
                    left = right;
                }
            }
        }

        /// @brief Sorts the list in ascending order
        inline void Sort() {
            Sort(Less<ValueType>());
        }
 
    private:
        ListDataNode<ValueType>* m_HeadFree;
        ListNode m_Sentinel;
        
        /// @brief Links two nodes together
        /// @param left The left node to link
        /// @param right The right node to link
        static void LinkNodes(ListNode* left, ListNode* right) {
            left->Next = right;
            right->Previous = left;
        }

        /// @brief Unlinks a node from the list
        /// @param node The node to unlink
        static void UnlinkNode(ListNode* node) {
            LinkNodes(node->Previous, node->Next);
        }

        /// @brief Links a node before a specified position
        /// @param position The position to link before
        /// @param node The node to link
        static void LinkNodeBefore(ListNode* position, ListNode* node) {
            LinkNodes(position->Previous, node);
            LinkNodes(node, position);
        }

        /// @brief Moves a node to a new position in the list
        /// @param position The position to move the node before
        /// @param node The node to move
        static void MoveNode(ListNode* position, ListNode* node) {
            if(position == node) return;
            UnlinkNode(node); // disconnect the node
            LinkNodeBefore(position, node); // insert
        }

        /// @brief Moves a range of nodes to a new position in the list
        /// @param position The position to move the nodes before
        /// @param first The first node in the range to move
        /// @param last The position following the last node in the range to move
        static void MoveNodeRange(ListNode* position, ListNode* first, ListNode* last) {
            if(position == first || position == last) return;
            
            ListNode* const lastElement = last->Previous;

            // Disconnect the range
            LinkNodes(first->Previous, last); 

            // Link the range before position
            LinkNodes(position->Previous, first);
            LinkNodes(lastElement, position);
        }

        /// @brief Casts a list node to a data node
        /// @param node The node to cast
        static ListDataNode<ValueType>* DataCast(ListNode* node) {
            return static_cast<ListDataNode<ValueType>*>(node);
        }

        /// @brief Casts a list node to a const data node
        /// @param node The node to cast
        static const ListDataNode<ValueType>* DataCast(const ListNode* node) {
            return static_cast<const ListDataNode<ValueType>*>(node);
        }

        /// @brief Gets a pointer to the head node of the list
        ListNode* HeadNode() {
            return m_Sentinel.Next;
        }

        /// @brief Gets a const pointer to the head node of the list
        const ListNode* HeadNode() const {
            return m_Sentinel.Next;
        }

        /// @brief Gets a pointer to the tail node of the list
        ListNode* TailNode() {
            return m_Sentinel.Previous;
        }

        /// @brief Gets a const pointer to the tail node of the list
        const ListNode* TailNode() const {
            return m_Sentinel.Previous;
        }

        /// @brief Converts a const iterator to a non-const iterator
        /// @param iterator The const iterator to convert
        Iterator ToIterator(ConstIterator iterator) {
            return Iterator(const_cast<ListNode*>(iterator.m_Current));
        }

        /// @brief Initializes the list, setting up the free list and sentinel node, trivial version: does not call destructors
        template<typename U>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            if(this->m_CurrentSize > 0) {
                // Make allocated elements a part of the free list
                if(m_HeadFree != NullPointer) TailNode()->Next = m_HeadFree;
                m_HeadFree = DataCast(HeadNode());
            }
            else if(m_HeadFree == NullPointer) {
                // Rebuild the free list
                for(size_t i = 0; i < this->Capacity() - 1; ++i)
                    this->m_Storage.Data[i].Next = &(this->m_Storage.Data[i + 1]);

                this->m_Storage.Data[this->Capacity() - 1].Next = NullPointer;
                m_HeadFree = &(this->m_Storage.Data[0]);
            }

            this->m_CurrentSize = 0;
            LinkNodes(&m_Sentinel, &m_Sentinel);
        }

        /// @brief Initializes the list, setting up the free list and sentinel node, non-trivial version: calls destructors
        template<typename U>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            if(this->m_CurrentSize > 0) {
                // Call destructors on allocated nodes 
                for(ListDataNode<ValueType>* i = DataCast(HeadNode()); i != &m_Sentinel; i = i->Next) i->Data.~ValueType();

                // Connect allocated to free list
                if(m_HeadFree != NullPointer) TailNode()->Next = m_HeadFree;
                m_HeadFree = DataCast(HeadNode());
            }
            else if(m_HeadFree == NullPointer) {
                // Rebuild the free list
                for(size_t i = 0; i < this->Capacity() - 1; ++i)
                    this->m_Storage.Data[i].Next = &(this->m_Storage.Data[i + 1]);

                this->m_Storage.Data[this->Capacity() - 1].Next = NullPointer;
                m_HeadFree = &(this->m_Storage.Data[0]);
            }

            this->m_CurrentSize = 0;
            LinkNodes(&m_Sentinel, &m_Sentinel);
        }

        /// @brief Creates a new default-constructed element at the back of the list
        void CreateBack() {
            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType();
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        
        /// @brief Creates a new element at the back of the list
        /// @param value The value to initialize the new element with
        void CreateBack(ConstReferenceType value) {
            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(value);
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Creates a new element at the front of the list
        /// @param value The value to initialize the new element with
        void CreateFront(ConstReferenceType value) {
            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(value);
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Creates a new element at the back of the list using move semantics
        /// @param value The value to initialize the new element with (rvalue reference)
        void CreateBack(ValueType&& value) {
            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(Move(value));
            LinkNodeBefore(&m_Sentinel, m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }

        /// @brief Creates a new element at the front of the list using move semantics
        /// @param value The value to initialize the new element with (rvalue reference)
        void CreateFront(ValueType&& value) {
            ListDataNode<ValueType>* const nextFree = DataCast(m_HeadFree->Next);

            ::new(&(m_HeadFree->Data)) ValueType(Move(value));
            LinkNodeBefore(HeadNode(), m_HeadFree);

            m_HeadFree = nextFree;
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Destroys a node, calling its destructor and returning it to the free list
        /// @param node The node to destroy
        void DestroyNode(ListDataNode<ValueType>* node) {
            node->Data.~ValueType();
            UnlinkNode(node);

            node->Next = m_HeadFree;
            m_HeadFree = node;
            --this->m_CurrentSize;
        }

        /// @brief Destroys the last node of the list
        void DestroyBack() {
            DestroyNode(DataCast(TailNode()));
        }

        /// @brief Destroys the first node of the list
        void DestroyFront() {
            DestroyNode(DataCast(HeadNode()));
        }

        /// @brief Swaps the contents of two lists, version for swappable storage types: justs swap the storage and metadata
        /// @param other The list to swap with
        template<typename U>
        typename EnableIf<StorageTraits<U>::IsSwappable, void>::Type 
        __Swap(BasicList& other) __WSTL_NOEXCEPT_EXPR__(noexcept(wstl::Swap(this->m_Storage, other.m_Storage))) {
            wstl::Swap(this->m_Storage, other.m_Storage);
            wstl::Swap(m_Sentinel, other.m_Sentinel);
            wstl::Swap(m_HeadFree, other.m_HeadFree);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);

            // Relink heads and tails to sentinels because their addresses haven't changed
            HeadNode()->Previous = &m_Sentinel;
            TailNode()->Next = &m_Sentinel;
            other.HeadNode()->Previous = &other.m_Sentinel;
            other.TailNode()->Next = &other.m_Sentinel;
        }
        
        /// @brief Swaps the contents of two lists, version for non-swappable storage types: swaps elements one by one
        /// @param other The list to swap with
        template<typename U>
        typename EnableIf<!StorageTraits<U>::IsSwappable, void>::Type 
        __Swap(BasicList& other) {
            const SizeType thisSize = this->Size();
            const SizeType otherSize = other.Size();
            SizeType minCount = Min(thisSize, otherSize);

            for(SizeType i = 0; i < minCount; ++i) {
                ValueType temp = other.Front();
                other.DestroyFront();
                other.CreateBack(Front());
                DestroyFront();
                CreateBack(temp);
            }

            if(thisSize > otherSize) {
                for(SizeType i = otherSize; i < thisSize; ++i) {
                    other.CreateBack(Front());
                    DestroyFront();
                }
            }
            else if(thisSize < otherSize) {
                for(SizeType i = thisSize; i < otherSize; ++i) {
                    CreateBack(other.Front());
                    other.DestroyFront();
                }
            }
        }
    };

    // Comparison operators

    template<typename Storage>
    inline bool operator==(const BasicList<Storage>& a, const BasicList<Storage>& b) {
        return (a.Size() == b.Size()) && Equal(a.Begin(), a.End(), b.Begin());
    }

    template<typename Storage>
    inline bool operator!=(const BasicList<Storage>& a, const BasicList<Storage>& b) {
        return !(a == b);
    }

    template<typename Storage>
    inline bool operator<(const BasicList<Storage>& a, const BasicList<Storage>& b) {
        return LexicographicalCompare(a.Begin(), a.End(), b.Begin(), b.End());
    }

    template<typename Storage>
    inline bool operator<=(const BasicList<Storage>& a, const BasicList<Storage>& b) {
        return !(b < a);
    }

    template<typename Storage>
    inline bool operator>(const BasicList<Storage>& a, const BasicList<Storage>& b) {
        return b < a;
    }

    template<typename Storage>
    inline bool operator>=(const BasicList<Storage>& a, const BasicList<Storage>& b) {
        return !(a < b);
    }

    // List

    /// @brief Version of the list with fixed storage, default option
    /// @tparam T Type of the elements
    /// @tparam N Capacity of the list
    /// @ingroup list
    template<typename T, size_t N>
    class List : public BasicList<FixedStorage<T, N> > {
    private:
        typedef BasicList<FixedStorage<T, N> > Base;

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
        List() : Base() {}
        
        /// @brief Copy constructor
        /// @param other The list to copy from
        List(const List& other) : Base(other) {}

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The list to move from
        /// @since C++11
        List(List&& other) : Base(Move(other)) {}
        #endif
        
        /// @brief Constructor that initializes the list with a range of elements
        /// @param first The position of the first element in the range
        /// @param last The position following the last element in the range
        template<typename InputIterator>
        List(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(first, last) {}

        /// @brief Constructor that initializes the list with a specified number of copies of a value
        /// @param count The number of copies to create
        explicit List(SizeType count) : Base(count) {}

        /// @brief Constructor that initializes the list with a specified number of copies of a value
        /// @param count The number of copies to create
        /// @param value The value to copy into the new elements
        List(SizeType count, ConstReferenceType value) : Base(count, value) {}

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Constructor that initializes the list with an initializer list
        /// @param list The initializer list to initialize from
        /// @since C++11
        List(InitializerList<ValueType> list) : Base(list) {}
        #endif

        /// @brief Copy assignment operator
        /// @param other The list to copy from
        List& operator=(const List& other) {
            if(this != &other) this->Assign(other.Begin(), other.End());
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The list to move from
        /// @since C++11
        List& operator=(List&& other) {
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
        List& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename List<T, N>::SizeType List<T, N>::StaticSize;

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T, typename... U>
    List(T, U...) -> List<T, sizeof...(U) + 1>;
    #endif

    // Make list

    #ifdef __WSTL_CXX11__
    /// @brief Makes a list out of the given values, with specified type
    /// @tparam T Type of the elements
    /// @param ...values Values to create the list with
    /// @return A list containing the given values
    /// @ingroup list
    /// @since C++11
    template<typename T, typename First, typename... Rest>
    constexpr auto MakeList(First&& first, Rest&&... rest) {
        return List<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }

    /// @brief Makes a list out of the given values
    /// @param ...values Values to create the list with
    /// @return A list containing the given values
    /// @ingroup list
    /// @since C++11
    template<typename First, typename... Rest>
    constexpr auto MakeList(First&& first, Rest&&... rest) {
        using T = CommonTypeType<First, Rest...>;
        return List<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }
    #endif

    // List external

    namespace external {
        /// @brief Version of the list that uses external storage
        /// @details Elements in storage are stored as nodes, so value type is wrapped in `ListDataNode` type
        /// @tparam T Type of the elements
        /// @ingroup list
        template<typename T>
        class List : public BasicList<ExternalStorage<T> > {
        private:
            typedef BasicList<ExternalStorage<T> > Base;

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
            List(ListDataNode<T>* buffer, SizeType capacity) : Base(StorageType(buffer, capacity)) {}

            /// @brief Copy constructor that uses external buffer
            /// @param other The list to copy from
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            List(const List& other, ListDataNode<T>* buffer, SizeType capacity) : Base(other, StorageType(buffer, capacity)) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor that uses external buffer
            /// @param other The list to move from
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            /// @since C++11
            List(List&& other, ListDataNode<T>* buffer, SizeType capacity) : Base(Move(other), StorageType(buffer, capacity)) {}
            #endif

            /// @brief Constructor that initializes the list with a range of elements
            /// @param first The position of the first element in the range
            /// @param last The position following the last element in the range
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            template<typename InputIterator>
            List(InputIterator first, InputIterator last, ListDataNode<T>* buffer, SizeType capacity, 
                typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(first, last, StorageType(buffer, capacity)) {}

            /// @brief Constructor that initializes the list with a specified number of copies of a value
            /// @param count The number of copies to create
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            explicit List(SizeType count, ListDataNode<T>* buffer, SizeType capacity) : Base(count, StorageType(buffer, capacity)) {}
                
            /// @brief Constructor that initializes the list with a specified number of copies of a value
            /// @param count The number of copies to create
            /// @param value The value to copy into the new elements
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            List(SizeType count, ConstReferenceType value, ListDataNode<T>* buffer, SizeType capacity) : Base(count, value, StorageType(buffer, capacity)) {}

            #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
            /// @brief Constructor that initializes the list with an initializer list
            /// @param list The initializer list to initialize from
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            /// @since C++11
            List(InitializerList<ValueType> list, ListDataNode<T>* buffer, SizeType capacity) : Base(list, StorageType(buffer, capacity)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The list to copy from
            List& operator=(const List& other) {
                if(this != &other) this->Assign(other.Begin(), other.End());
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The list to move from
            /// @since C++11
            List& operator=(List&& other) {
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
            List& operator=(InitializerList<ValueType> list) {
                this->Assign(list);
                return *this;
            }
            #endif
            #endif
        };

        /// @brief Version of the list that uses fixed external storage with compile-time known capacity
        /// @details Elements in storage are stored as nodes, so value type is wrapped in `ListDataNode` type
        /// @tparam T Type of the elements
        /// @tparam N Capacity of the list
        /// @ingroup list
        template<typename T, size_t N>
        class FixedList : public BasicList<FixedExternalStorage<T, N> > {
        private:
            typedef BasicList<FixedExternalStorage<T, N> > Base;

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

            /// @brief Constructor that uses fixed external buffer
            /// @param buffer Pointer to the external buffer
            FixedList(ListDataNode<T>* buffer) : Base(StorageType(buffer)) {}

            /// @brief Copy constructor that uses fixed external buffer
            /// @param other The list to copy from
            /// @param buffer Pointer to the external buffer
            FixedList(const FixedList& other, ListDataNode<T>* buffer) : Base(other, StorageType(buffer)) {}

            #ifdef __WSTL_CXX11__
            /// @brief Move constructor that uses fixed external buffer
            /// @param other The list to move from
            /// @param buffer Pointer to the external buffer
            /// @since C++11
            FixedList(FixedList&& other, ListDataNode<T>* buffer) : Base(Move(other), StorageType(buffer)) {}
            #endif

            /// @brief Constructor that initializes the list with a range of elements
            /// @param first The position of the first element in the range
            /// @param last The position following the last element in the range
            /// @param buffer Pointer to the external buffer
            template<typename InputIterator>
            FixedList(InputIterator first, InputIterator last, ListDataNode<T>* buffer, 
                typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(first, last, StorageType(buffer)) {}

            /// @brief Constructor that initializes the list with a specified number of copies of a value
            /// @param count The number of copies to create
            /// @param buffer Pointer to the external buffer
            explicit FixedList(SizeType count, ListDataNode<T>* buffer) : Base(count, StorageType(buffer)) {}
            
            /// @brief Constructor that initializes the list with a specified number of copies of a value
            /// @param count The number of copies to create
            /// @param value The value to copy into the new elements
            /// @param buffer Pointer to the external buffer
            FixedList(SizeType count, ConstReferenceType value, ListDataNode<T>* buffer) : Base(count, value, StorageType(buffer)) {}

            #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
            /// @brief Constructor that initializes the list with an initializer list
            /// @param list The initializer list to initialize from
            /// @param buffer Pointer to the external buffer
            /// @since C++11
            FixedList(InitializerList<ValueType> list, ListDataNode<T>* buffer) : Base(list, StorageType(buffer)) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The list to copy from
            FixedList& operator=(const FixedList& other) {
                if(this != &other) this->Assign(other.Begin(), other.End());
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The list to move from
            /// @since C++11
            FixedList& operator=(FixedList&& other) {
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
            FixedList& operator=(InitializerList<ValueType> list) {
                this->Assign(list);
                return *this;
            }
            #endif
            #endif
        };

        template<typename T, size_t N>
        const __WSTL_CONSTEXPR__ typename FixedList<T, N>::SizeType FixedList<T, N>::StaticSize;

        // Template deduction guides

        #ifdef __WSTL_CXX17__
        template<typename T, size_t N>
        FixedList(T(&)[N]) -> FixedList<T, N>;

        template<typename T, typename U, size_t N>
        FixedList(U, T(&)[N]) -> FixedList<T, N>;

        template<typename T, typename U1, typename U2, size_t N>
        FixedList(U1, U2, T(&)[N]) -> FixedList<T, N>;

        template<typename T, size_t N>
        FixedList(InitializerList<T>, T(&)[N]) -> FixedList<T, N>;
        #endif
    }
}

#endif