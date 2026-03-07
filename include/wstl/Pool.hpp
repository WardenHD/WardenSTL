// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_POOL_HPP__
#define __WSTL_POOL_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "PlacementNew.hpp"
#include "StandardExceptions.hpp"
#include "NullPointer.hpp"
#include "BoundedIterator.hpp"
#include "private/Error.hpp"
#include "private/Swap.hpp"
#include "Bitset.hpp"
#include <stddef.h>


/// @defgroup pool Pool
/// @brief Pool containers for storing objects
/// @ingroup containers

namespace wstl {
    // Basic intrusive pool

    /// @brief Basic intrusive pool class that stores objects and free list in-place
    /// @tparam Storage Storage type for the pool
    /// @ingroup pool
    template<typename Storage>
    class BasicIntrusivePool : public TypedContainerBase<Storage> {
    public:
        WSTL_STATIC_ASSERT(!IsVoid<Storage>::Value, "Storage must be non-void");

        typedef typename TypedContainerBase<Storage>::ValueType ValueType;
        typedef typename TypedContainerBase<Storage>::SizeType SizeType;
        typedef typename TypedContainerBase<Storage>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<Storage>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<Storage>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<Storage>::PointerType PointerType;
        typedef typename TypedContainerBase<Storage>::ConstPointerType ConstPointerType;

        typedef typename TypedContainerBase<Storage>::StorageType StorageType;

        WSTL_STATIC_ASSERT(sizeof(ValueType) >= sizeof(PointerType), "Type is too small to be used with intrusive pool");
        
    private:
        template<bool IsConst>
        class IntrusivePoolIterator {
        public:
            typedef typename Conditional<IsConst, const BasicIntrusivePool::ValueType, BasicIntrusivePool::ValueType>::Type ValueType;
            typedef ForwardIteratorTag IteratorCategory;
            typedef typename Conditional<IsConst, BasicIntrusivePool::ConstReferenceType, BasicIntrusivePool::ReferenceType>::Type ReferenceType;
            typedef typename Conditional<IsConst, BasicIntrusivePool::ConstPointerType, BasicIntrusivePool::PointerType>::Type PointerType;
            typedef ptrdiff_t DifferenceType;

            friend class BasicIntrusivePool;

            IntrusivePoolIterator(const IntrusivePoolIterator& other) : m_Current(other.m_Current), m_Pool(other.m_Pool) {}

            IntrusivePoolIterator& operator=(const IntrusivePoolIterator& other) {
                m_Current = other.m_Current;
                m_Pool = other.m_Pool;
                return *this;
            }

            ReferenceType operator*() const {
                return *m_Current;
            }

            PointerType operator->() const {
                return m_Current;
            }

            IntrusivePoolIterator& operator++() {
                ++m_Current;
                FindAllocated();
                return *this;
            }

            IntrusivePoolIterator& operator++(int) {
                IntrusivePoolIterator original(*this);
                ++(*this);
                return original;
            }

            friend bool operator==(const IntrusivePoolIterator& a, const IntrusivePoolIterator& b) {
                return a.m_Current == b.m_Current;
            }

            friend bool operator!=(const IntrusivePoolIterator& a, const IntrusivePoolIterator& b) {
                return !(a == b);
            }
 
        private:
            typedef typename Conditional<IsConst, const BasicIntrusivePool*, BasicIntrusivePool*>::Type PoolType;

            PointerType m_Current;
            PoolType m_Pool;

            IntrusivePoolIterator(PoolType pool, PointerType start) : m_Current(start), m_Pool(pool) {
                FindAllocated();
            }

            void FindAllocated() {
                while(m_Current < m_Pool->m_Storage.Data + m_Pool->Capacity() && m_Pool->IsFree(m_Current)) ++m_Current;
            }
        };

    public:
        typedef IntrusivePoolIterator<false> Iterator;
        typedef IntrusivePoolIterator<true> ConstIterator;

        /// @brief Default constructor
        BasicIntrusivePool() : TypedContainerBase<Storage>(), m_Next(this->m_Storage.Data) {
            Initialize<ValueType>();
        }

        /// @brief Constructor with storage, only for non-default-constructible storage
        /// @param storage Storage to use for the pool
        explicit BasicIntrusivePool(const Storage& storage) : TypedContainerBase<Storage>(storage), m_Next(this->m_Storage.Data) {
            Initialize<ValueType>();
        }

        /// @brief Destructor
        ~BasicIntrusivePool() {
            Initialize<ValueType>();
        }

        /// @brief Gets iterator to the beginning of the pool that iterates only allocated objects
        Iterator Begin() {
            return Iterator(this, this->m_Storage.Data);
        }

        /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
        ConstIterator Begin() const {
            return ConstIterator(this, this->m_Storage.Data);
        }

        /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
        ConstIterator ConstBegin() const {
            return ConstIterator(this, this->m_Storage.Data);
        }

        /// @brief Gets iterator to the end of the pool
        Iterator End() {
            return Iterator(this, this->m_Storage.Data + this->Capacity());
        }

        /// @brief Gets const iterator to the end of the pool
        ConstIterator End() const {
            return ConstIterator(this, this->m_Storage.Data + this->Capacity());
        }

        /// @brief Gets const iterator to the end of the pool
        ConstIterator ConstEnd() const {
            return ConstIterator(this, this->m_Storage.Data + this->Capacity());
        }

        /// @brief Allocates new object from the pool
        /// @return Pointer to the object
        PointerType Allocate() {
            __WSTL_ASSERT_RETURNVALUE__(m_Next != NullPointer, WSTL_MAKE_EXCEPTION(LengthError, "Intrusive pool is full"), NullPointer);

            PointerType result = m_Next;
            m_Next = *reinterpret_cast<PointerType*>(m_Next);
            ++this->m_CurrentSize;

            return result;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Allocates and constructs new object in the pool with given arguments
        /// @param ...args Arguments to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename... Args>
        PointerType Create(Args&&... args) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(Forward<Args>(args)...);
            return item;
        }
        #else
        /// @brief Allocates and constructs new object in the pool
        /// @return Pointer to the object
        PointerType Create() {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType();
            return item;
        }

        /// @brief Allocates and constructs new object in the pool
        /// @param arg Argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg>
        PointerType Create(const Arg& arg) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(arg);
            return item;
        }

        /// @brief Allocates and constructs new object in the pool with two arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(arg1, arg2);
            return item;
        }

        /// @brief Allocates and constructs new object in the pool with three arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @param arg3 Third argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2, typename Arg3>
        PointerType Create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(arg1, arg2, arg3);
            return item;
        }
        #endif

        /// @brief Releases object back to the pool
        /// @param value Pointer to the object
        void Release(ConstPointerType const value) {
            __WSTL_ASSERT_RETURN__(Contains(value), WSTL_MAKE_EXCEPTION(OutOfRange, "Pointer not in the intrusive pool range"));
            __WSTL_ASSERT_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "Intrusive pool is empty"));
          
            --this->m_CurrentSize;
            *reinterpret_cast<PointerType*>(const_cast<PointerType>(value)) = m_Next;
            m_Next = const_cast<PointerType>(value);
        }

        /// @brief Destroys object and releases it back to the pool
        /// @param value Pointer to the object
        void Destroy(ConstPointerType const value) {
            value->~ValueType();
            Release(value);
        }

        /// @brief Reinitializes the pool, destroying all objects
        void Clear() {
            Initialize<ValueType>();
        }

        /// @brief Checks whether the pool contains the given pointer inside
        bool Contains(ConstPointerType const value) const {
            return (value - this->m_Storage.Data) >= 0 && (value - this->m_Storage.Data) <= this->Capacity();
        }
    
    private:
        PointerType m_Next;

        /// @brief Deleted copy constructor
        BasicIntrusivePool(const BasicIntrusivePool&) __WSTL_DELETE__;

        /// @brief Deleted copy assignment operator
        BasicIntrusivePool& operator=(const BasicIntrusivePool&) __WSTL_DELETE__;

        /// @brief Initializes the pool: does not call destructors
        template<typename U>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            m_Next = this->m_Storage.Data;
            for(size_t i = 0; i < this->Capacity() - 1; ++i) *reinterpret_cast<PointerType*>(this->m_Storage.Data + i) = this->m_Storage.Data + i + 1;
            *reinterpret_cast<PointerType*>(this->m_Storage.Data + this->Capacity() - 1) = NullPointer; // last points to null

            this->m_CurrentSize = 0;
        }

        /// @brief Initializes the pool: calls destructors
        template<typename U>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            for(Iterator it = this->Begin(); it != this->End(); ++it) it->~ValueType();

            m_Next = this->m_Storage.Data;
            for (size_t i = 0; i < this->m_Capacity - 1; ++i) *reinterpret_cast<PointerType*>(this->m_Storage.Data + i) = this->m_Storage.Data + i + 1;
            *reinterpret_cast<PointerType*>(this->m_Storage.Data + this->Capacity() - 1) = NullPointer; // last points to null

            this->m_CurrentSize = 0;
        }

        /// @brief Checks whether the given pointer is in the free list
        bool IsFree(ConstPointerType value) const {
            ConstPointerType i = m_Next;
            while(i != NullPointer) {
                if(i == value) return true;
                i = *reinterpret_cast<ValueType* const*>(i);
            }

            return false;
        }
    };

    // Fixed-size intrusive pool

    /// @brief Fixed-size intrusive pool that stores objects and free list in-place
    /// @tparam T Object type to store
    /// @tparam N Maximum number of objects in the pool
    /// @ingroup pool
    template<typename T, size_t N>
    class IntrusivePool : public BasicIntrusivePool<FixedStorage<T, N> > {
    private:
        typedef BasicIntrusivePool<FixedStorage<T, N> > Base;

    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;

        typedef typename Base::StorageType StorageType;

        /// @brief The static size of the stack, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

        /// @brief Default constructor
        IntrusivePool() : Base() {}
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename IntrusivePool<T, N>::SizeType IntrusivePool<T, N>::StaticSize;

    // External intrusive pool

    namespace external {
        /// @brief Version of intrusive pool that uses external storage
        /// @tparam T Type of the elements
        /// @ingroup pool
        template<typename T>
        class IntrusivePool : public BasicIntrusivePool<ExternalStorage<T> > {
        private:
            typedef BasicIntrusivePool<ExternalStorage<T> > Base;
            
        public:
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::DifferenceType DifferenceType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;

            typedef typename Base::StorageType StorageType;

            /// @brief Constructor
            /// @param buffer Pointer to the external buffer
            /// @param capacity The capacity of the external buffer
            IntrusivePool(T* buffer, SizeType capacity) : Base(ExternalStorage(buffer, capacity)) {}
        };

        /// @brief Version of intrusive pool that uses fixed external storage with compile-time know capacity
        /// @tparam T Type of the elements
        /// @tparam N Capacity of the pool
        /// @ingroup pool
        template<typename T, size_t N>
        class FixedIntrusivePool : public BasicIntrusivePool<FixedExternalStorage<T, N> > {
        private:
            typedef BasicIntrusivePool<FixedExternalStorage<T, N> > Base;
            
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

            /// @brief Constructor
            /// @param buffer Pointer to the external buffer
            explicit FixedIntrusivePool(T* buffer) : Base(FixedExternalStorage<T, N>(buffer)) {}
        };

        template<typename T, size_t N>
        const __WSTL_CONSTEXPR__ typename FixedIntrusivePool<T, N>::SizeType FixedIntrusivePool<T, N>::StaticSize;

        // Template deduction guide

        #ifdef __WSTL_CXX17__
        template<typename T, size_t N>
        FixedIntrusivePool(T(&)[N]) -> FixedIntrusivePool<T, N>;
        #endif
    }

    // Indexed pool

    /// @brief Indexed pool that stores objects and free list using bitset
    /// @tparam T Object type to store
    /// @tparam N Maximum number of objects in the pool
    /// @ingroup pool
    template<typename T, size_t N>
    class IndexedPool : public TypedContainerBase<FixedStorage<T, N>> {
    private:
        typedef TypedContainerBase<FixedStorage<T, N>> Base; 

    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;

        /// @brief The static size of the stack, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

    private:
        template<bool IsConst>
        class IndexedPoolIterator {
        public:
            typedef typename Conditional<IsConst, const IndexedPool::ValueType, IndexedPool::ValueType>::Type ValueType;
            typedef ForwardIteratorTag IteratorCategory;
            typedef typename Conditional<IsConst, IndexedPool::ConstReferenceType, IndexedPool::ReferenceType>::Type ReferenceType;
            typedef typename Conditional<IsConst, IndexedPool::ConstPointerType, IndexedPool::PointerType>::Type PointerType;
            typedef ptrdiff_t DifferenceType;
            
            friend class IndexedPool;

            IndexedPoolIterator(const IndexedPoolIterator& other) : m_CurrentIndex(other.m_CurrentIndex), m_Pool(other.m_Pool) {}

            IndexedPoolIterator& operator=(const IndexedPoolIterator& other) {
                m_CurrentIndex = other.m_CurrentIndex;
                m_Pool = other.m_Pool;
                return *this;
            }

            ReferenceType operator*() const {
                return m_Pool->m_Storage.Data[m_CurrentIndex];
            }

            PointerType operator->() const {
                return (m_Pool->m_Storage.Data + m_CurrentIndex);
            }

            IndexedPoolIterator& operator++() {
                m_CurrentIndex = m_Pool->m_Indices.template FindNext<true>(m_CurrentIndex);
                return *this;
            }

            IndexedPoolIterator operator++(int) {
                IndexedPoolIterator original(*this);
                ++(*this);
                return original;
            }

            friend bool operator==(const IndexedPoolIterator& a, const IndexedPoolIterator& b) {
                return a.m_CurrentIndex == b.m_CurrentIndex;
            }

            friend bool operator!=(const IndexedPoolIterator& a, const IndexedPoolIterator& b) {
                return !(a == b);
            }
        
        private:
            typedef typename Conditional<IsConst, const IndexedPool*, IndexedPool*>::Type PoolType;

            SizeType m_CurrentIndex;
            PoolType m_Pool;

            IndexedPoolIterator(PoolType pool, SizeType start) : m_CurrentIndex(start), m_Pool(pool) {}
        };

    public:
        typedef IndexedPoolIterator<false> Iterator;
        typedef IndexedPoolIterator<true> ConstIterator;

        /// @brief Default constructor
        IndexedPool() : Base(), m_Indices() {}

        /// @brief Destructor
        ~IndexedPool() {
            Initialize<ValueType>();
        }

        /// @brief Gets iterator to the beginning of the pool that iterates only allocated objects
        Iterator Begin() {
            return Iterator(this, m_Indices.template FindFirst<true>());
        }
        
        /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
        ConstIterator Begin() const {
            return ConstIterator(this, m_Indices.template FindFirst<true>());
        }

        /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
        ConstIterator ConstBegin() const {
            return ConstIterator(this, m_Indices.template FindFirst<true>());
        }

        /// @brief Gets iterator to the end of the pool whose index is `NoPosition`
        Iterator End() {
            return Iterator(this, m_Indices.NoPosition);
        }

        /// @brief Gets const iterator to the end of the pool whose index is `NoPosition`
        ConstIterator End() const {
            return ConstIterator(this, m_Indices.NoPosition);
        }

        /// @brief Gets const iterator to the end of the pool whose index is `NoPosition`
        ConstIterator ConstEnd() const {
            return ConstIterator(this, m_Indices.NoPosition);
        }

        /// @brief Allocates new object from the pool
        /// @return Pointer to the object
        PointerType Allocate() {
            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Indexed pool is full"), NullPointer);

            PointerType result = (this->m_Storage.Data + this->m_CurrentSize);
            m_Indices.Set(this->m_CurrentSize);
            ++this->m_CurrentSize;

            return result;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Allocates and constructs new object in the pool with given arguments
        /// @param ...args Arguments to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename... Args>
        PointerType Create(Args&&... args) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(Forward<Args>(args)...);
            return item;
        }
        #else
        /// @brief Allocates and constructs new object in the pool
        /// @return Pointer to the object
        PointerType Create() {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType();
            return item;
        }

        /// @brief Allocates and constructs new object in the pool
        /// @param arg Argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg>
        PointerType Create(const Arg& arg) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(arg);
            return item;
        }

        /// @brief Allocates and constructs new object in the pool with two arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(arg1, arg2);
            return item;
        }

        /// @brief Allocates and constructs new object in the pool with three arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @param arg3 Third argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2, typename Arg3>
        PointerType Create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            PointerType item = Allocate();
            if(item) ::new(item) ValueType(arg1, arg2, arg3);
            return item;
        }
        #endif

        /// @brief Releases object back to the pool
        /// @param value Pointer to the object
        void Release(ConstPointerType const value) {
            __WSTL_ASSERT_RETURN__(Contains(value), WSTL_MAKE_EXCEPTION(OutOfRange, "Pointer not in the indexed pool range"));
            __WSTL_ASSERT_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "Indexed pool is empty"));
          
            --this->m_CurrentSize;
            m_Indices.Reset(value - this->m_Storage.Data);
        }

        /// @brief Destroys object and releases it back to the pool
        /// @param value Pointer to the object
        void Destroy(ConstPointerType const value) {
            value->~ValueType();
            Release(value);
        }

        /// @brief Reinitializes the pool, destroying all objects
        void Clear() {
            Initialize<ValueType>();
        }

        /// @brief Checks whether the pool contains the given pointer inside
        bool Contains(ConstPointerType const value) const {
            return (value - this->m_Storage.Data) >= 0 && (value - this->m_Storage.Data) <= this->Capacity();
        }

    private:
        Bitset<N> m_Indices;

        /// @brief Initializes the pool: does not call destructors
        template<typename U>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            m_Indices.Reset();
            this->m_CurrentSize = 0;
        }

        /// @brief Initializes the pool: calls destructors
        template<typename U>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            for(Iterator it = Begin(); it != End(); ++it) it->~ValueType();
            m_Indices.Reset();
            this->m_CurrentSize = 0;
        }
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename IndexedPool<T, N>::SizeType IndexedPool<T, N>::StaticSize;
}

#endif