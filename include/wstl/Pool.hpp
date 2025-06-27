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
#include <stddef.h>



namespace wstl {
    // Pool interface

    template<typename T, size_t SIZE, typename Derived>
    class PoolInterface : public TypedContainerBase<T> { 
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;
        
        PointerType Allocate() {
            return static_cast<Derived*>(this)->Allocate();
        }

        #ifdef __WSTL_CXX11__
        template<typename... Args>
        PointerType Create(Args&&... args) {
            return static_cast<Derived*>(this)->Create(Forward<Args>(args)...);
        }

        #else
        template<typename Arg>
        PointerType Create(const Arg& arg) {
            return static_cast<Derived*>(this)->Create(arg);
        }

        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            return static_cast<Derived*>(this)->Create(arg1, arg2);
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        PointerType Create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            return static_cast<Derived*>(this)->Create(arg1, arg2, arg3);
        }
        #endif

        void Release(ConstPointerType const value) {
            static_cast<Derived*>(this)->Release(value);
        }

        void Destroy(ConstPointerType const value) {
            static_cast<Derived*>(this)->Destroy(value);
        }

        void Clear() {
            static_cast<Derived*>(this)->Clear();
        }

        bool Contains(ConstPointerType const value) const {
            return static_cast<Derived*>(this)->Contains(value);
        }
    
    protected:
        PoolInterface() : TypedContainerBase<T>(SIZE) {}
        ~PoolInterface() __WSTL_DEFAULT__;
    };

    // Intrusive pool

    template<typename T, size_t SIZE>
    class IntrusivePool : public PoolInterface<T, SIZE, IntrusivePool<T, SIZE> > {
    public:
        StaticAssert(sizeof(T) >= sizeof(T*), "Type is too small to be used with intrusive pool");

        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::ValueType ValueType;
        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::SizeType SizeType;
        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::DifferenceType DifferenceType;
        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::ReferenceType ReferenceType;
        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::ConstReferenceType ConstReferenceType;
        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::PointerType PointerType;
        typedef typename PoolInterface<T, SIZE, IntrusivePool<T, SIZE> >::ConstPointerType ConstPointerType;
        
        template<bool IsConst>
        class IntrusivePoolIterator {
        public:
            typedef typename Conditional<IsConst, const T, T>::Type ValueType;
            typedef ForwardIteratorTag IteratorCategory;
            typedef typename Conditional<IsConst, const T&, T&>::Type ReferenceType;
            typedef typename Conditional<IsConst, const T*, T*>::Type PointerType;
            typedef ptrdiff_t DifferenceType;

            friend class IntrusivePool;

            IntrusivePoolIterator(const IntrusivePoolIterator& other) : m_Current(other.m_Current), m_Pool(other.m_Pool) {}

            #ifdef __WSTL_CXX11__
            IntrusivePoolIterator(IntrusivePoolIterator&& other) : m_Current(Move(other.m_Current)), m_Pool(Move(other.m_Pool)) {}
            #endif

            IntrusivePoolIterator& operator=(const IntrusivePoolIterator& other) {
                m_Current = other.m_Current;
                m_Pool = other.m_Pool;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            IntrusivePoolIterator& operator=(IntrusivePoolIterator&& other) __WSTL_NOEXCEPT__ {
                m_Current = Move(other.m_Current);
                m_Pool = Move(other.m_Pool);
                return *this;
            }
            #endif

            ReferenceType operator*() {
                return *m_Current;
            }

            const ReferenceType operator*() const {
                return *m_Current;
            }

            ValueType* operator->() {
                return m_Current;
            }

            const ValueType* operator->() const {
                return m_Current;
            }

            operator ValueType*() const {
                return m_Current;
            }

            __WSTL_CONSTEXPR14__ IntrusivePoolIterator& operator++() {
                ++m_Current;
                FindAllocated();
                return *this;
            }

            __WSTL_CONSTEXPR14__ IntrusivePoolIterator& operator++(int) {
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
            ValueType* m_Current;
            IntrusivePool& m_Pool;

            IntrusivePoolIterator(IntrusivePool& pool, ValueType* start) : m_Current(start), m_Pool(pool) {
                FindAllocated();
            }

            void FindAllocated() {
                while(m_Current < m_Pool.m_Buffer + m_Pool.m_Capacity && m_Pool.IsFree(m_Current)) ++m_Current;
            }
        };

        typedef IntrusivePoolIterator<false> Iterator;
        typedef IntrusivePoolIterator<true> ConstIterator;

        IntrusivePool() : PoolInterface<T, SIZE, IntrusivePool<T, SIZE>>(), m_Next(m_Buffer) {
            Initialize();
        }

        ~IntrusivePool() {
            for(Iterator it = Begin(); it != End(); ++it) it->~T();
        }

        Iterator Begin() {
            return Iterator(*this, m_Buffer);
        }

        ConstIterator Begin() const {
            return ConstIterator(*this, m_Buffer);
        }

        ConstIterator ConstBegin() const {
            return ConstIterator(*this, m_Buffer);
        }

        Iterator End() {
            return Iterator(*this, m_Buffer + this->m_Capacity);
        }

        ConstIterator End() const {
            return ConstIterator(*this, m_Buffer + this->m_Capacity);
        }

        ConstIterator ConstEnd() const {
            return ConstIterator(*this, m_Buffer + this->m_Capacity);
        }

        PointerType Allocate() {
            __WSTL_ASSERT_RETURNVALUE__(m_Next != NullPointer, WSTL_MAKE_EXCEPTION(LengthError, "Intrusive pool is full"), NullPointer);

            PointerType result = m_Next;
            m_Next = *reinterpret_cast<PointerType*>(m_Next);
            ++this->m_CurrentSize;

            return result;
        }

        #ifdef __WSTL_CXX11__
        template<typename... Args>
        PointerType Create(Args&&... args) {
            PointerType item = Allocate();
            if(item) ::new(item) T(Forward<Args>(args)...);
            return item;
        }
        #else
        PointerType Create() {
            PointerType item = Allocate();
            if(item) ::new(item) T();
            return item;
        }

        template<typename Arg>
        PointerType Create(const Arg& arg) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg);
            return item;
        }

        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg1, arg2);
            return item;
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        PointerType Create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg1, arg2, arg3);
            return item;
        }
        #endif

        void Release(ConstPointerType const value) {
            __WSTL_ASSERT_RETURN__(Contains(value), WSTL_MAKE_EXCEPTION(OutOfRange, "Pointer not in the intrusive pool range"));
            __WSTL_ASSERT_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "Intrusive pool is empty"));
          
            --this->m_CurrentSize;
            *reinterpret_cast<PointerType*>(const_cast<PointerType>(value)) = m_Next;
            m_Next = const_cast<PointerType>(value);
        }

        void Destroy(ConstPointerType const value) {
            value->~T();
            Release(value);
        }

        void Clear() {
            for(Iterator it = Begin(); it != End(); ++it) it->~T();
            this->m_CurrentSize = 0;
            Initialize();
        }

        bool Contains(ConstPointerType const value) const {
            return (value - m_Buffer) >= 0 && (value - m_Buffer) <= this->m_Capacity;
        }
    
    private:
        T m_Buffer[SIZE];
        PointerType m_Next;

        void Initialize() {
            m_Next = m_Buffer;

            for (size_t i = 0; i < this->m_Capacity - 1; ++i) *reinterpret_cast<PointerType*>(m_Buffer + i) = m_Buffer + i + 1;
            *reinterpret_cast<PointerType*>(m_Buffer + this->m_Capacity - 1) = NullPointer; // last points to null
        }

        bool IsFree(ConstPointerType value) const {
            ConstPointerType i = m_Next;
            while(i != NullPointer) {
                if(i == value) return true;
                i = *reinterpret_cast<T* const*>(i);
            }

            return false;
        }
    };

    // Indexed pool

    template<typename T, size_t SIZE>
    class IndexedPool : public PoolInterface<T, SIZE, IndexedPool<T, SIZE> > {
    public:
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::ValueType ValueType;
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::SizeType SizeType;
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::DifferenceType DifferenceType;
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::ReferenceType ReferenceType;
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::ConstReferenceType ConstReferenceType;
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::PointerType PointerType;
        typedef typename PoolInterface<T, SIZE, IndexedPool<T, SIZE> >::ConstPointerType ConstPointerType;

        /// TODO: Indexed pool implementation
    };
}

#endif