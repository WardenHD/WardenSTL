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
    // Pool interface

    /// @brief Interface for pool containers
    /// @tparam T Object type to store
    /// @tparam Derived Derived class type
    /// @ingroup pool
    template<typename T, typename Derived>
    class PoolInterface : public TypedContainerBase<T> { 
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;
        
        /// @brief Allocates new object from the pool
        /// @return Pointer to the object
        PointerType Allocate() {
            return static_cast<Derived*>(this)->Allocate();
        }

        #ifdef __WSTL_CXX11__
        /// @brief Allocates and constructs new object in the pool with given arguments
        /// @param ...args Arguments to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename... Args>
        PointerType Create(Args&&... args) {
            return static_cast<Derived*>(this)->Create(Forward<Args>(args)...);
        }

        #else
        /// @brief Allocates and constructs new object in the pool
        /// @return Pointer to the object
        PointerType Create() {
            return static_cast<Derived*>(this)->Create();
        }

        /// @brief Allocates and constructs new object in the pool
        /// @param arg Argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg>
        PointerType Create(const Arg& arg) {
            return static_cast<Derived*>(this)->Create(arg);
        }

        /// @brief Allocates and constructs new object in the pool with two arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            return static_cast<Derived*>(this)->Create(arg1, arg2);
        }

        /// @brief Allocates and constructs new object in the pool with three arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @param arg3 Third argument to forward to the constructor of the object
        template<typename Arg1, typename Arg2, typename Arg3>
        PointerType Create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            return static_cast<Derived*>(this)->Create(arg1, arg2, arg3);
        }
        #endif

        /// @brief Releases object back to the pool
        /// @param value Pointer to the object
        void Release(ConstPointerType const value) {
            static_cast<Derived*>(this)->Release(value);
        }

        /// @brief Destroys object and releases it back to the pool
        /// @param value Pointer to the object
        void Destroy(ConstPointerType const value) {
            static_cast<Derived*>(this)->Destroy(value);
        }

        /// @brief Clears the pool, destroying all objects
        void Clear() {
            static_cast<Derived*>(this)->Clear();
        }

        /// @brief Checks whether the pool contains the given pointer inside
        bool Contains(ConstPointerType const value) const {
            return static_cast<Derived*>(this)->Contains(value);
        }
    
    protected:
        PoolInterface(SizeType capacity) : TypedContainerBase<T>(capacity) {}
        ~PoolInterface() __WSTL_DEFAULT__;
    };

    // Intrusive pool

    /// @brief Intrusive pool that stores objects and free list in-place
    /// @tparam T Object type to store
    /// @tparam N Maximum number of objects in the pool
    /// @ingroup pool
    template<typename T, size_t N>
    class IntrusivePool : public PoolInterface<T, IntrusivePool<T, N> > {
    private:
        typedef PoolInterface<T, IntrusivePool<T, N> > Base;
    public:
        StaticAssert(sizeof(T) >= sizeof(T*), "Type is too small to be used with intrusive pool");

        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;
        
    private:
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
            IntrusivePool* m_Pool;

            IntrusivePoolIterator(IntrusivePool* pool, ValueType* start) : m_Current(start), m_Pool(pool) {
                FindAllocated();
            }

            void FindAllocated() {
                while(m_Current < m_Pool->m_Buffer + m_Pool->m_Capacity && m_Pool->IsFree(m_Current)) ++m_Current;
            }
        };

    public:
        typedef IntrusivePoolIterator<false> Iterator;
        typedef IntrusivePoolIterator<true> ConstIterator;

        /// @brief Default constructor
        IntrusivePool() : Base(N), m_Buffer(), m_Next(m_Buffer) {
            Initialize();
        }

        /// @brief Destructor
        ~IntrusivePool() {
            for(Iterator it = Begin(); it != End(); ++it) it->~T();
        }

        /// @brief Gets iterator to the beginning of the pool that iterates only allocated objects
        Iterator Begin() {
            return Iterator(this, m_Buffer);
        }

        /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
        ConstIterator Begin() const {
            return ConstIterator(this, m_Buffer);
        }

        /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
        ConstIterator ConstBegin() const {
            return ConstIterator(this, m_Buffer);
        }

        /// @brief Gets iterator to the end of the pool
        Iterator End() {
            return Iterator(this, m_Buffer + this->m_Capacity);
        }

        /// @brief Gets const iterator to the end of the pool
        ConstIterator End() const {
            return ConstIterator(this, m_Buffer + this->m_Capacity);
        }

        /// @brief Gets const iterator to the end of the pool
        ConstIterator ConstEnd() const {
            return ConstIterator(this, m_Buffer + this->m_Capacity);
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
            if(item) ::new(item) T(Forward<Args>(args)...);
            return item;
        }
        #else
        /// @brief Allocates and constructs new object in the pool
        /// @return Pointer to the object
        PointerType Create() {
            PointerType item = Allocate();
            if(item) ::new(item) T();
            return item;
        }

        /// @brief Allocates and constructs new object in the pool
        /// @param arg Argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg>
        PointerType Create(const Arg& arg) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg);
            return item;
        }

        /// @brief Allocates and constructs new object in the pool with two arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg1, arg2);
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
            if(item) ::new(item) T(arg1, arg2, arg3);
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
            value->~T();
            Release(value);
        }

        /// @brief Clears the pool, destroying all objects
        void Clear() {
            for(Iterator it = Begin(); it != End(); ++it) it->~T();
            this->m_CurrentSize = 0;
            Initialize();
        }

        /// @brief Checks whether the pool contains the given pointer inside
        bool Contains(ConstPointerType const value) const {
            return (value - m_Buffer) >= 0 && (value - m_Buffer) <= this->m_Capacity;
        }
    
    private:
        T m_Buffer[N];
        PointerType m_Next;

        // Disable copy constructor and assignment operator
        
        IntrusivePool(const IntrusivePool&) __WSTL_DELETE__;
        IntrusivePool& operator=(const IntrusivePool&) __WSTL_DELETE__;

        /// @brief Initializes the free list
        void Initialize() {
            m_Next = m_Buffer;

            for (size_t i = 0; i < this->m_Capacity - 1; ++i) *reinterpret_cast<PointerType*>(m_Buffer + i) = m_Buffer + i + 1;
            *reinterpret_cast<PointerType*>(m_Buffer + this->m_Capacity - 1) = NullPointer; // last points to null
        }

        /// @brief Checks whether the given pointer is in the free list
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

    /// @brief Indexed pool that stores objects and free list using bitset
    /// @tparam T Object type to store
    /// @tparam N Maximum number of objects in the pool
    /// @ingroup pool
    template<typename T, size_t N>
    class IndexedPool : public PoolInterface<T, IndexedPool<T, N> > {
    private:
        typedef PoolInterface<T, IndexedPool<T, N> > Base; 

    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::DifferenceType DifferenceType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;

    private:
        template<bool IsConst>
        class IndexedPoolIterator {
        public:
            typedef typename Conditional<IsConst, const T, T>::Type ValueType;
            typedef ForwardIteratorTag IteratorCategory;
            typedef typename Conditional<IsConst, const T&, T&>::Type ReferenceType;
            typedef typename Conditional<IsConst, const T*, T*>::Type PointerType;
            typedef ptrdiff_t DifferenceType;
            
            friend class IndexedPool;

            IndexedPoolIterator(const IndexedPoolIterator& other) : m_CurrentIndex(other.m_CurrentIndex), m_Pool(other.m_Pool) {}

            #ifdef __WSTL_CXX11__
            IndexedPoolIterator(IndexedPoolIterator&& other) : m_CurrentIndex(Move(other.m_CurrentIndex)), m_Pool(Move(other.m_Pool)) {}
            #endif

            IndexedPoolIterator& operator=(const IndexedPoolIterator& other) {
                m_CurrentIndex = other.m_CurrentIndex;
                m_Pool = other.m_Pool;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            IndexedPoolIterator& operator=(IndexedPoolIterator&& other) __WSTL_NOEXCEPT__ {
                m_CurrentIndex = Move(other.m_CurrentIndex);
                m_Pool = Move(other.m_Pool);
                return *this;
            }
            #endif

            ReferenceType operator*() {
                return m_Pool->m_Buffer[m_CurrentIndex];
            }

            ConstReferenceType operator*() const {
                return m_Pool->m_Buffer[m_CurrentIndex];
            }

            ValueType* operator->() {
                return (m_Pool->m_Buffer + m_CurrentIndex);
            }

            const ValueType* operator->() const {
                return (m_Pool->m_Buffer + m_CurrentIndex);
            }

            operator ValueType*() const {
                return (m_Pool->m_Buffer + m_CurrentIndex);
            }

            __WSTL_CONSTEXPR14__ IndexedPoolIterator& operator++() {
                m_CurrentIndex = m_Pool->m_Indices.template FindNext<true>(m_CurrentIndex);
                return *this;
            }

            __WSTL_CONSTEXPR14__ IndexedPoolIterator& operator++(int) {
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
            SizeType m_CurrentIndex;
            IndexedPool* m_Pool;

            IndexedPoolIterator(IndexedPool* pool, SizeType start) : m_CurrentIndex(start), m_Pool(pool) {}
        };

    public:
        typedef IndexedPoolIterator<false> Iterator;
        typedef IndexedPoolIterator<true> ConstIterator;

        /// @brief Default constructor
        IndexedPool() : Base(N), m_Indices(), m_Buffer() {}

        /// @brief Destructor
        ~IndexedPool() {
            for(Iterator it = Begin(); it != End(); ++it) it->~T();
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

            PointerType result = (m_Buffer + this->m_CurrentSize);
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
            if(item) ::new(item) T(Forward<Args>(args)...);
            return item;
        }
        #else
        /// @brief Allocates and constructs new object in the pool
        /// @return Pointer to the object
        PointerType Create() {
            PointerType item = Allocate();
            if(item) ::new(item) T();
            return item;
        }

        /// @brief Allocates and constructs new object in the pool
        /// @param arg Argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg>
        PointerType Create(const Arg& arg) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg);
            return item;
        }

        /// @brief Allocates and constructs new object in the pool with two arguments
        /// @param arg1 First argument to forward to the constructor of the object
        /// @param arg2 Second argument to forward to the constructor of the object
        /// @return Pointer to the object
        template<typename Arg1, typename Arg2>
        PointerType Create(const Arg1& arg1, const Arg2& arg2) {
            PointerType item = Allocate();
            if(item) ::new(item) T(arg1, arg2);
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
            if(item) ::new(item) T(arg1, arg2, arg3);
            return item;
        }
        #endif

        /// @brief Releases object back to the pool
        /// @param value Pointer to the object
        void Release(ConstPointerType const value) {
            __WSTL_ASSERT_RETURN__(Contains(value), WSTL_MAKE_EXCEPTION(OutOfRange, "Pointer not in the indexed pool range"));
            __WSTL_ASSERT_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "Indexed pool is empty"));
          
            --this->m_CurrentSize;
            m_Indices.Reset(value - m_Buffer);
        }

        /// @brief Destroys object and releases it back to the pool
        /// @param value Pointer to the object
        void Destroy(ConstPointerType const value) {
            value->~T();
            Release(value);
        }

        /// @brief Clears the pool, destroying all objects
        void Clear() {
            for(Iterator it = Begin(); it != End(); ++it) it->~T();
            this->m_CurrentSize = 0;
            m_Indices.Reset();
        }

        /// @brief Checks whether the pool contains the given pointer inside
        bool Contains(ConstPointerType const value) const {
            return (value - m_Buffer) >= 0 && (value - m_Buffer) <= this->m_Capacity;
        }

    private:
        Bitset<N> m_Indices;
        T m_Buffer[N];

        IndexedPool(const IndexedPool&) __WSTL_DELETE__;
        IndexedPool& operator=(const IndexedPool&) __WSTL_DELETE__;
    };

    // Pool external

    namespace external {
        // Intrusive pool

        /// @brief Intrusive pool that uses externally defined storage
        /// @tparam T Object type to store
        /// @ingroup pool
        template<typename T>
        class IntrusivePool : public PoolInterface<T, IntrusivePool<T> > {
        private:
            typedef PoolInterface<T, IntrusivePool<T> > Base;
        public:
            StaticAssert(sizeof(T) >= sizeof(T*), "Type is too small to be used with intrusive pool");

            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::DifferenceType DifferenceType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;
            
        private:
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
                IntrusivePool* m_Pool;

                IntrusivePoolIterator(IntrusivePool* pool, ValueType* start) : m_Current(start), m_Pool(pool) {
                    FindAllocated();
                }

                void FindAllocated() {
                    while(m_Current < m_Pool->m_Buffer + m_Pool->m_Capacity && m_Pool->IsFree(m_Current)) ++m_Current;
                }
            };

        public:
            typedef IntrusivePoolIterator<false> Iterator;
            typedef IntrusivePoolIterator<true> ConstIterator;

            /// @brief Default constructor
            IntrusivePool(PointerType buffer, SizeType size) : Base(size), m_Buffer(buffer), m_Next(m_Buffer) {
                Initialize();
            }

            /// @brief Destructor
            ~IntrusivePool() {
                for(Iterator it = Begin(); it != End(); ++it) it->~T();
            }

            /// @brief Gets iterator to the beginning of the pool that iterates only allocated objects
            Iterator Begin() {
                return Iterator(this, m_Buffer);
            }

            /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
            ConstIterator Begin() const {
                return ConstIterator(this, m_Buffer);
            }

            /// @brief Gets const iterator to the beginning of the pool that iterates only allocated objects
            ConstIterator ConstBegin() const {
                return ConstIterator(this, m_Buffer);
            }

            /// @brief Gets iterator to the end of the pool
            Iterator End() {
                return Iterator(this, m_Buffer + this->m_Capacity);
            }

            /// @brief Gets const iterator to the end of the pool
            ConstIterator End() const {
                return ConstIterator(this, m_Buffer + this->m_Capacity);
            }

            /// @brief Gets const iterator to the end of the pool
            ConstIterator ConstEnd() const {
                return ConstIterator(this, m_Buffer + this->m_Capacity);
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
                if(item) ::new(item) T(Forward<Args>(args)...);
                return item;
            }
            #else
            /// @brief Allocates and constructs new object in the pool
            /// @return Pointer to the object
            PointerType Create() {
                PointerType item = Allocate();
                if(item) ::new(item) T();
                return item;
            }

            /// @brief Allocates and constructs new object in the pool
            /// @param arg Argument to forward to the constructor of the object
            /// @return Pointer to the object
            template<typename Arg>
            PointerType Create(const Arg& arg) {
                PointerType item = Allocate();
                if(item) ::new(item) T(arg);
                return item;
            }

            /// @brief Allocates and constructs new object in the pool with two arguments
            /// @param arg1 First argument to forward to the constructor of the object
            /// @param arg2 Second argument to forward to the constructor of the object
            /// @return Pointer to the object
            template<typename Arg1, typename Arg2>
            PointerType Create(const Arg1& arg1, const Arg2& arg2) {
                PointerType item = Allocate();
                if(item) ::new(item) T(arg1, arg2);
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
                if(item) ::new(item) T(arg1, arg2, arg3);
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
                value->~T();
                Release(value);
            }

            /// @brief Clears the pool, destroying all objects
            void Clear() {
                for(Iterator it = Begin(); it != End(); ++it) it->~T();
                this->m_CurrentSize = 0;
                Initialize();
            }

            /// @brief Checks whether the pool contains the given pointer inside
            bool Contains(ConstPointerType const value) const {
                return (value - m_Buffer) >= 0 && (value - m_Buffer) <= this->m_Capacity;
            }
        
        private:
            PointerType m_Buffer;
            PointerType m_Next;
            
            /// @brief Deleted copy constructor
            IntrusivePool(const IntrusivePool&) __WSTL_DELETE__;
            /// @brief Deleted copy assignment operator
            IntrusivePool& operator=(const IntrusivePool&) __WSTL_DELETE__;

            /// @brief Initializes the free list
            void Initialize() {
                m_Next = m_Buffer;

                for (size_t i = 0; i < this->m_Capacity - 1; ++i) *reinterpret_cast<PointerType*>(m_Buffer + i) = m_Buffer + i + 1;
                *reinterpret_cast<PointerType*>(m_Buffer + this->m_Capacity - 1) = NullPointer; // last points to null
            }

            /// @brief Checks whether the given pointer is in the free list
            bool IsFree(ConstPointerType value) const {
                ConstPointerType i = m_Next;
                while(i != NullPointer) {
                    if(i == value) return true;
                    i = *reinterpret_cast<T* const*>(i);
                }

                return false;
            }
        };
    }
}

#endif