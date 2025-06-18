// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s memory.h,
// which are licensed under the MIT License.
//
// It has been restructured and adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_MEMORY_HPP__
#define __WSTL_MEMORY_HPP__

#include "private/Platform.hpp"
#include "private/AddressOf.hpp"
#include "PlacementNew.hpp"
#include "Iterator.hpp"
#include "Utility.hpp"
#include "Algorithm.hpp"
#include "NullPointer.hpp"

#ifdef __WSTL_LIBC_WRAPPERS__
#include <string.h>
#endif


/// @defgroup memory Memory
/// @brief A set of memory management utilities 
/// @ingroup wstl

namespace wstl {
    // Pointer traits

    /// @brief Provides uniform interface for accessing properties of a pointer
    /// @tparam Pointer Type of the pointer
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/pointer_traits
    template<typename Pointer>
    struct PointerTraits {
        /// @brief The type of the pointer
        typedef Pointer PointerType;
        /// @brief The type of the element pointed to
        typedef typename Pointer::ElementType ElementType;
        /// @brief The type of the difference between two pointers
        typedef typename Conditional<!IsSame<typename Pointer::ElementType, NullPointerType>::Value, typename Pointer::ElementType, ptrdiff_t>::Type DifferenceType;

        /// @brief Rebinds the pointer to a different type
        /// @tparam U The new type to bind to
        template<typename U>
        struct Rebind { typedef typename Pointer::Rebind<U>::Other Other; };

        /// @brief Returns a pointer to the given reference
        static __WSTL_CONSTEXPR14__ PointerType PointerTo(ElementType& reference) {
            return Pointer::PointerTo(reference);
        }
    };

    /// @brief Provides uniform interface for accessing properties of a raw pointer
    /// @tparam T Type of the element pointed to
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/pointer_traits
    template<typename T>
    struct PointerTraits<T*> {
        /// @brief The type of the pointer
        typedef T* PointerType;
        /// @brief The type of the element pointed to
        typedef T ElementType;
        /// @brief The type of the difference between two pointers
        typedef ptrdiff_t DifferenceType;

        /// @brief Rebinds the pointer to a different type
        /// @tparam U The new type to bind to
        template<typename U>
        struct Rebind { typedef U* Other; };

        /// @brief Returns a pointer to the given reference
        static __WSTL_CONSTEXPR14__ PointerType PointerTo(ElementType& reference) __WSTL_NOEXCEPT__ {
            return AddressOf(reference);
        }
    };

    // Default delete

    /// @brief Default deleter for unique pointers
    /// @tparam T Type of the object being deleted
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/default_delete
    template<typename T>
    class DefaultDelete {
    public:
        /// @brief Constructor
        __WSTL_CONSTEXPR__ DefaultDelete() __WSTL_NOEXCEPT__ {}

        /// @brief Templated copy constructor
        /// @param other Another `DefaultDelete` object to copy from
        template<typename U>
        __WSTL_CONSTEXPR__ DefaultDelete(const DefaultDelete<U>&) __WSTL_NOEXCEPT__ {}

        /// @brief Calls `delete` on the pointer
        void operator()(T* pointer) const __WSTL_NOEXCEPT__ {
            delete pointer;
        }
    };

    /// @brief Default deleter for unique pointers to arrays
    /// @tparam T Type of the object being deleted
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/default_delete
    template<typename T>
    class DefaultDelete<T[]> {
    public:
        /// @brief Constructor
        __WSTL_CONSTEXPR__ DefaultDelete() __WSTL_NOEXCEPT__ {}

        /// @brief Templated copy constructor
        /// @param other Another `DefaultDelete` object to copy from
        template<typename U>
        __WSTL_CONSTEXPR__ DefaultDelete(const DefaultDelete<U[]>&) __WSTL_NOEXCEPT__ {}

        /// @brief Calls `delete[]` on the pointer
        template<typename U>
        void operator()(U* pointer) const {
            delete[] pointer;
        }
    };

    // Unique pointer

    template<typename T, typename Deleter = DefaultDelete<T>> 
    class UniquePointer;
    
    /// @brief A smart pointer that owns and manages another object through a pointer
    /// @details The object is destroyed and its memory deallocated when the `UniquePointer` goes out of scope
    /// @tparam Deleter The type of the deleter used to destroy the object
    /// @tparam T The type of the object being managed
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr
    template<typename T, typename Deleter>
    class UniquePointer {
    public:
        typedef Deleter DeleterType;
        typedef T ElementType;
        typedef T* PointerType;
        typedef T& ReferenceType;
        
        /// @brief Default constructor
        __WSTL_CONSTEXPR__ UniquePointer() : m_Pointer(NullPointer) __WSTL_NOEXCEPT__ {}

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        __WSTL_CONSTEXPR__ explicit UniquePointer(PointerType pointer) : m_Pointer(pointer) __WSTL_NOEXCEPT__ {}

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The object to move from
        UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                m_Pointer = other.Release();
                m_Deleter = Move(other.m_Deleter);
            }
        }
        #else
        /// @brief Copy constructor
        /// @param other The object to copy from
        UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                m_Pointer = other.Release();
                m_Deleter = other.m_Deleter;
            }
        }
        #endif

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, typename Conditional<IsReference<Deleter>::Value, 
            Deleter, typename AddLValueReference<const Deleter>::Type>::Type deleter) __WSTL_NOEXCEPT__
            : m_Pointer(pointer), m_Deleter(deleter) {}

        #ifdef __WSTL_CXX11__
        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, typename RemoveReference<Deleter>::Type&& deleter) __WSTL_NOEXCEPT__ 
            : m_Pointer(pointer), m_Deleter(Move(deleter)) {}
        
        /// @brief Templated move constructor
        /// @param other The object to move from
        template<typename U, typename E>
        UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ 
            : m_Pointer(other.Release()), m_Deleter(Forward<E>(other.m_Deleter)) {}
        #endif

        /// @brief Destructor
        ~UniquePointer() {
            if(m_Pointer != NullPointer) deleter(m_Pointer);
        }

        /// @brief Releases the ownership of the managed object
        /// @return The pointer to the object being managed
        PointerType Release() __WSTL_NOEXCEPT__ {
            PointerType value = m_Pointer;
            m_Pointer = NullPointer;    
            return value;
        }

        /// @brief Resets the managed object to a new pointer
        void Reset(PointerType pointer = PointerType()) __WSTL_NOEXCEPT__ {
            if(m_Pointer == NullPointer || pointer != m_Pointer) {
                PointerType value = m_Pointer;
                m_Pointer = pointer;

                if(value != NullPointer) m_Deleter(value);
            }
        }

        /// @brief Swaps the contents of two `UniquePointer` objects
        /// @param other The object to swap with
        void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Pointer, other.m_Pointer);
        }

        /// @brief Returns the managed pointer
        __WSTL_CONSTEXPR__ PointerType Get() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Returns reference to the deleter used to destroy the object
        Deleter& GetDeleter() __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        /// @brief Returns const reference to the deleter used to destroy the object
        const Deleter& GetDeleter() const __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        /// @brief Returns true if the pointer is not null
        explicit operator bool() const __WSTL_NOEXCEPT__ {
            return m_Pointer != NullPointer;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The object to move from
        UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = Move(other.m_Deleter);
            }

            return *this;
        }

        /// @brief Templated move assignment operator
        /// @param other The object to move from
        template<typename U, typename E>
        UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(NullPointer);
                m_Pointer = other.Release();
                m_Deleter = Forward<E>(other.m_Deleter);
            }
                
            return *this;
        }
        #else
        /// @brief Copy assignment operator
        /// @param other The object to copy from
        UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = other.m_Deleter;
            }

            return *this;
        }
        #endif

        /// @brief Assignment operator, assigns a null pointer
        UniquePointer& operator=(wstl::NullPointerType) __WSTL_NOEXCEPT__ {
            if(m_Pointer) Reset(__WSTL_NULLPTR__);
            return *this;
        }

        /// @brief Dereference operator
        /// @return A reference to the object pointed to by the pointer
        __WSTL_CONSTEXPR__ ReferenceType operator*() const __WSTL_NOEXCEPT__ {
            return *Get();
        }

        /// @brief Arrow operator
        /// @return A pointer to the object pointed to by the pointer
        __WSTL_CONSTEXPR__ PointerType operator->() const __WSTL_NOEXCEPT__ {
            return Get();
        }

        /// @brief Subscript operator
        /// @param index The index to access
        __WSTL_CONSTEXPR__ ReferenceType operator[](size_t index) const {
            return m_Pointer[index];
        }

    private:
        PointerType m_Pointer;
        Deleter m_Deleter;

        UniquePointer(const UniquePointer&) __WSTL_DELETE__;
        UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
    };

    /// @brief A specialization of `UniquePointer` for arrays
    /// @tparam T Type of the object being managed
    /// @tparam Deleter The type of the deleter used to destroy the object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr
    template<typename T, typename Deleter>
    class UniquePointer<T[], Deleter> {
    public:
        typedef T ElementType;
        typedef T* PointerType;
        typedef T& ReferenceType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR__ UniquePointer() : m_Pointer(__WSTL_NULLPTR__) __WSTL_NOEXCEPT__ {}

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        __WSTL_CONSTEXPR__ UniquePointer(PointerType pointer) : m_Pointer(pointer) __WSTL_NOEXCEPT__ {}

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The object to move from
        UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                m_Pointer = other.Release();
                m_Deleter = Move(other.m_Deleter);
            }
        }
        #else
        /// @brief Copy constructor
        /// @param other The object to copy from
        UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                m_Pointer = other.Release();
                m_Deleter = other.m_Deleter;
            }
        }
        #endif

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter to use to destroy the object
        UniquePointer(PointerType pointer, typename Conditional<IsReference<Deleter>::Value, 
            Deleter, typename AddLValueReference<const Deleter>::Type>::Type deleter) __WSTL_NOEXCEPT__
            : m_Pointer(pointer), m_Deleter(deleter) {}

        #ifdef __WSTL_CXX11__
        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter to use to destroy the object
        UniquePointer(PointerType pointer, typename RemoveReference<Deleter>::Type&& deleter) __WSTL_NOEXCEPT__ 
            : m_Pointer(pointer), m_Deleter(deleter) {}
        
        /// @brief Templated move constructor
        /// @param other The object to move from
        template<typename U, typename E>
        UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ 
            : m_Pointer(other.Release()), m_Deleter(Forward<E>(other.m_Deleter)) {}
        #endif
        
        /// @brief Destructor
        ~UniquePointer() {
            if(m_Pointer != __WSTL_NULLPTR__) deleter(m_Pointer);
        }

        /// @brief Releases the ownership of the managed object
        /// @return The pointer to the object being managed
        PointerType Release() __WSTL_NOEXCEPT__ {
            PointerType value = m_Pointer;
            m_Pointer = NullPointer;    
            return value;
        }

        /// @brief Resets the managed object to a new pointer
        /// @param pointer The new pointer to the object being managed
        template<typename U>
        void Reset(U pointer) __WSTL_NOEXCEPT__ {
            if(pointer != m_Pointer) {
                PointerType value = m_Pointer;
                m_Pointer = pointer;

                if(value != NullPointer) m_Deleter(value);
            }
        }

        /// @brief Resets the managed object to a null pointer
        void Reset(NullPointerType = NullPointer) __WSTL_NOEXCEPT__ {
            Reset(PointerType());
        }

        /// @brief Swaps the contents of two `UniquePointer` objects
        /// @param other The object to swap with
        void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Pointer, other.m_Pointer);
        }
    
        /// @brief Returns the managed pointer
        __WSTL_CONSTEXPR__ PointerType Get() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Returns reference to the deleter used to destroy the object
        Deleter& GetDeleter() __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        /// @brief Returns const reference to the deleter used to destroy the object
        const Deleter& GetDeleter() const __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        /// @brief Returns true if the pointer is not null
        explicit operator bool() const __WSTL_NOEXCEPT__ {
            return m_Pointer != NullPointer;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The object to move from
        UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = Move(other.m_Deleter);
            }

            return *this;
        }

        /// @brief Templated move assignment operator
        /// @param other The object to move from
        template<typename U, typename E>
        UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(NullPointer);
                m_Pointer = other.Release();
                m_Deleter = Forward<E>(other.m_Deleter);
            }
                
            return *this;
        }
        #else
        /// @brief Copy assignment operator
        /// @param other The object to copy from
        UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = other.m_Deleter;
            }

            return *this;
        }
        #endif

        /// @brief Assignment operator, assigns a null pointer
        UniquePointer& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            Reset(NullPointer);
            return *this;
        }

        /// @brief Dereference operator
        /// @return The object pointed to by the pointer
        __WSTL_CONSTEXPR__ PointerType operator*() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Arrow operator
        /// @return The object pointed to by the pointer
        __WSTL_CONSTEXPR__ PointerType operator->() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Subscript operator
        /// @param index The index to access
        __WSTL_CONSTEXPR__ ReferenceType operator[](size_t index) const {
            return m_Pointer[index];
        }

    private:
        PointerType m_Pointer;
        Deleter m_Deleter;

        UniquePointer(const UniquePointer&) __WSTL_DELETE__;
        UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
    };

    // Swap specialization

    /// @brief Swaps the contents of two `UniquePointer` objects
    /// @param a The first object to swap
    /// @param b The second object to swap
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr/swap2
    template<typename T, typename D>
    __WSTL_CONSTEXPR14__
    inline void Swap(UniquePointer<T, D>& a, UniquePointer<T, D>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Make unique

    namespace __private {
        template<typename T>
        struct __IsBoundedArray : FalseType {};

        template<typename T, size_t N>
        struct __IsBoundedArray<T[N]> : TrueType {};

        template<typename T>
        struct __IsUnboundedArray : FalseType {};

        template<typename T>
        struct __IsUnboundedArray<T[]> : TrueType {};
    }

    #ifdef __WSTL_CXX11__
    /// @brief Creates a `UniquePointer` to a new object of type `T`
    /// @tparam T The type of the object to create 
    /// @param ...args The arguments to pass to the constructor
    /// @return A `UniquePointer` to the newly created object
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename... Args>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<!IsArray<T>::Value, UniquePointer<T>> MakeUnique(Args&&... args) {
        return UniquePointer<T>(::new T(Forward<Args>(args)...));
    }

    /// @brief Creation of a `UniquePointer` for known bound array types is deleted\
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename... Args>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<__private::__IsBoundedArray<T>::Value> MakeUnique(Args&&...) = delete;
    #else
    /// @brief Creates a `UniquePointer` to a new object of type `T`
    /// @tparam T The type of the object to create
    /// @return A `UniquePointer` to the newly created object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T>
    inline typename EnableIf<!IsArray<T>::Value, UniquePointer<T>>::Type MakeUnique() {
        return UniquePointer<T>(::new T());
    }

    /// @brief Creates a `UniquePointer` to a new object of type `T`
    /// @tparam T The type of the object to create
    /// @param arg The argument to pass to the constructor
    /// @return A `UniquePointer` to the newly created object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename Arg>
    inline typename EnableIf<!IsArray<T>::Value, UniquePointer<T>>::Type MakeUnique(const Arg& arg) {
        return UniquePointer<T>(::new T(arg));
    }

    /// @brief Creates a `UniquePointer` to a new object of type `T`
    /// @tparam T The type of the object to create
    /// @param arg1 The first argument to pass to the constructor
    /// @param arg2 The second argument to pass to the constructor
    /// @return A `UniquePointer` to the newly created object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename Arg1, typename Arg2>
    inline typename EnableIf<!IsArray<T>::Value, UniquePointer<T>>::Type MakeUnique(const Arg1& arg1, const Arg2& arg2) {
        return UniquePointer<T>(::new T(arg1, arg2));
    }

    /// @brief Creates a `UniquePointer` to a new object of type `T`
    /// @tparam T The type of the object to create
    /// @param arg1 The first argument to pass to the constructor
    /// @param arg2 The second argument to pass to the constructor
    /// @param arg3 The third argument to pass to the constructor
    /// @return A `UniquePointer` to the newly created object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename Arg1, typename Arg2, typename Arg3>
    inline typename EnableIf<!IsArray<T>::Value, UniquePointer<T>>::Type MakeUnique(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
        return UniquePointer<T>(::new T(arg1, arg2, arg3));
    }
    #endif

    /// @brief Creates a `UniquePointer` to a dynamically allocated array of type `T`
    /// @tparam T The type of the object that the array will hold
    /// @param size The size of the array
    /// @return A `UniquePointer` to the newly created array
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T>
    __WSTL_CONSTEXPR14__
    inline typename EnableIf<__private::__IsUnboundedArray<T>::Value, UniquePointer<T>>::Type MakeUnique(size_t size) {
        return UniquePointer<T>(new typename RemoveAllExtents<T>::Type[size]());
    }
}

// Comparison operators for unique pointer

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator==(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return a.Get() == b.Get();
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator!=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return !(a == b);
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator<(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return reinterpret_cast<char*>(a.Get()) < reinterpret_cast<char*>(b.Get());
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator<=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return !(b < a);
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator>(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return b < a;
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator>=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return !(a < b);
}

namespace wstl {
    // Uninitialized copy

    /// @brief Copies elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        OutputIterator result = resultFirst;

        for(; first != last; ++first, ++result)
            ::new(static_cast<void*>(AddressOf(*result))) ValueType(*first);
        
        return resultFirst;
    }

    /// @brief Copies elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        return Copy(first, last, resultFirst);
    }

    // Uninitialized copy in range

    /// @brief Copies N elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param count Number of elements to copy
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    OutputIterator UninitializedCopyInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        return UninitializedCopy(first, first + count, resultFirst);
    }

    // Uninitialized fill

    /// @brief Fills uninitialized memory with a value, constructing the elements
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @param value The value to fill with
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill
    template<typename InputIterator, typename T>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<InputIterator>::ValueType>::Value, InputIterator>::Type 
    inline UninitializedFill(InputIterator first, InputIterator last, const T& value) {
        Fill(first, last, value);
        return last;
    }

    /// @brief Fills uninitialized memory with a value, constructing the elements
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @param value The value to fill with
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill
    template<typename OutputIterator, typename T>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedFill(OutputIterator first, OutputIterator last, const T& value) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; first != last; ++first) 
            ::new(static_cast<void*>(AddressOf(*first))) ValueType(value);
        
        return last;
    }

    // Uninitialized fill in range

    /// @brief Fills N elements of uninitialized memory with a value, constructing the elements
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to fill
    /// @param value The value to fill with
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill_n
    template<typename OutputIterator, typename Size, typename T>
    inline OutputIterator UninitializedFillInRange(OutputIterator first, Size count, const T& value) {
        return UninitializedFill(first, first + count, value);
    }

    // Unitialized move

    #ifdef __WSTL_CXX11__
    /// @brief Moves elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedMove(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        return Move(first, last, resultFirst);
    }

    /// @brief Moves elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedMove(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        OutputIterator result = resultFirst;

        for(; first != last; ++first, ++result) 
            ::new(static_cast<void*>(AddressOf(*result))) ValueType(Move(*first));

        return resultFirst;
    }  

    // Uninitialized move in range

    /// @brief Moves N elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param count Number of elements to move
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    inline OutputIterator UninitializedMoveInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        return UninitializedMove(first, first + count, resultFirst);
    }
    #endif

    // Uninitialized default construct

    /// @brief Default constructs elements in uninitialized memory,
    /// for trivially constructible types does nothing
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct
    template<typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
    UninitializedDefaultConstruct(OutputIterator, OutputIterator) {
        // Do nothing
    }
    
    /// @brief Default constructs elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct
    template<typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
    UninitializedDefaultConstruct(OutputIterator first, OutputIterator last) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;

        for(; first != last; ++first) ::new(static_cast<void*>(AddressOf(*first))) ValueType;
    }

    // Uninitialized default construct in range 

    /// @brief Default constructs N elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to default construct
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct_n
    template<typename OutputIterator, typename Size>
    OutputIterator UninitializedDefaultConstructInRange(OutputIterator first, Size count) {
        OutputIterator result = first + count;
        UninitializedDefaultConstruct(first, result);
        return result;
    }

    // Uninitialized value construct

    /// @brief Value constructs elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct
    template<typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
    UninitializedValueConstruct(OutputIterator first, OutputIterator last) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        Fill(first, last, ValueType());
    }

    /// @brief Value constructs elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct
    template<typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
    UninitializedValueConstruct(OutputIterator first, OutputIterator last) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; first != last; ++first) ::new(static_cast<void*>(AddressOf(*first))) ValueType();
    }

    // Uninitialized value construct in range

    /// @brief Value constructs N elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to value construct
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct_n
    template<typename OutputIterator, typename Size>
    OutputIterator UninitializedValueConstructInRange(OutputIterator first, Size count) {
        OutputIterator result = first + count;
        UninitializedValueConstruct(first, result);
        return result;
    }

    // Construct at

    #ifdef __WSTL_CXX11__
    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @param ...args The arguments to pass to the constructor
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T, typename... Args>
    T* ConstructAt(T* pointer, Args&&... args) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(Forward<Args>(args)...);
    }
    #else
    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T>
    T* ConstructAt(T* pointer) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T();
    }

    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @param args The argument to pass to the constructor
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T, typename Arg>
    T* ConstructAt(T* pointer, const Arg& arg) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(arg);
    }
    #endif
    
    // Destroy at

    /// @brief Destroys an object at the given address, for trivially destructible types does nothing
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_at
    template<typename T>
    typename EnableIf<IsTriviallyDestructible<T>::Value, void>::Type DestroyAt(T*) {}

    /// @brief Destroys an object at the given address
    /// @param pointer The address to destroy the object at
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_at
    template<typename T>
    typename EnableIf<!IsTriviallyDestructible<T>::Value, void>::Type DestroyAt(T* pointer) {
        pointer->~T();
    }

    // Destroy

    /// @brief Destroys a range of objects, for trivially destructible types does nothing
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy
    template<typename Iterator>
    typename EnableIf<IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, void>::Type 
    Destroy(Iterator, Iterator) {
        // Do nothing
    }

    /// @brief Destroys a range of objects
    /// @param first The beginning of the range
    /// @param last The end of the range
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy
    template<typename Iterator>
    typename EnableIf<!IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, void>::Type 
    Destroy(Iterator first, Iterator last) {
        for(; first != last; ++first) DestroyAt(AddressOf(*first));
    }

    // Destroy in range

    /// @brief Destroys N objects in a range, for trivially destructible types does nothing
    /// @returns The end of the range
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_n
    template<typename Iterator, typename Size>
    typename EnableIf<IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, Iterator>::Type 
    DestroyInRange(Iterator first, Size count) {
        // Do nothing
        return first + count;
    }

    /// @brief Destroys N objects in a range
    /// @param first The beginning of the range
    /// @param count The number of objects to destroy
    /// @returns The end of the range
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_n
    template<typename Iterator, typename Size>
    typename EnableIf<!IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, Iterator>::Type 
    DestroyInRange(Iterator first, Size count) {
        for(; count > 0; ++first, --count) DestroyAt(AddressOf(*first));
        return first;
    }

    // Uninitialized buffer

    /// @brief A class that provides an uninitialized buffer of a specified type, size, and alignment
    /// @tparam T The type of the objects in the buffer
    /// @tparam Length The number of objects in the buffer
    /// @tparam Alignment The alignment of the objects in the buffer
    /// @ingroup memory
    template<typename T, size_t Length, size_t Alignment = AlignmentOf<T>::Value>
    class UninitializedBuffer {
    public:
        typedef T ValueType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T* Iterator;
        typedef const T* ConstIterator;

        static const __WSTL_CONSTEXPR__ size_t ObjectSize = sizeof(T);
        static const __WSTL_CONSTEXPR__ size_t BufferLength = Length;
        static const __WSTL_CONSTEXPR__ size_t BufferAlignment = Alignment;

        /// @brief Conversion operator to reference type
        /// @return A reference to the first object in the buffer
        operator T&() {
            return m_Buffer.template GetReference<T>();
        }

        /// @brief Conversion operator to const reference type
        /// @return A const reference to the first object in the buffer
        operator const T&() const {
            return m_Buffer.template GetReference<const T>();
        }
        
        /// @brief Conversion operator to pointer type
        /// @return A pointer to the first object in the buffer
        operator T*() {
            return m_Buffer.template GetPointer<T>();
        }

        /// @brief Conversion operator to const pointer type
        /// @return A const pointer to the first object in the buffer
        operator const T*() const {
            return m_Buffer.template GetPointer<const T>();
        }

        /// @brief Returns a pointer to the underlying data
        T* Data() {
            return m_Buffer.template GetPointer<T>();
        }

        /// @brief Returns a const pointer to the underlying data
        const T* Data() const {
            return m_Buffer.template GetPointer<const T>();
        }

        /// @brief Returns a pointer to the beginning of the buffer
        T* Begin() {
            return m_Buffer.template GetPointer<T>();
        }

        /// @brief Returns a const pointer to the beginning of the buffer
        const T* Begin() const {
            return m_Buffer.template GetPointer<const T>();
        }

        /// @brief Returns a pointer to the end of the buffer
        T* End() {
            return Begin() + Length;
        }

        /// @brief Returns a const pointer to the end of the buffer
        const T* End() const {
            return Begin() + Length;
        }

    private:
        typename AlignedStorage<Length * sizeof(T), Alignment>::Type m_Buffer;
    };

    // Wrappers for libc functions (if there are user-defined ones)

    #ifdef __WSTL_LIBC_WRAPPERS__
    using ::memchr;
    using ::memcmp;
    using ::memcpy;
    using ::memmove;
    using ::memset;

    /// @brief Namespace for memory-related function wrappers
    namespace memory {
        // Copy - memcpy

        template<typename T, typename U>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<T*>::ValueType>::Value &&
            IsTriviallyCopyable<typename IteratorTraits<U*>::ValueType>::Value,
            T*
        >::Type 
        /// @brief Copies a range of memory from one location to another
        /// @param sourceFirst The beginning of the range to copy
        /// @param sourceLast The end of the range to copy
        /// @param destinationFirst The beginning of the range to copy to
        /// @return A pointer to the destination
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Copy(const T* sourceFirst, const T* sourceLast, U* destinationFirst) __WSTL_NOEXCEPT__ {
            return reinterpret_cast<T*>(memcpy(
                reinterpret_cast<void*>(destinationFirst),
                reinterpret_cast<const void*>(sourceFirst), 
                sizeof(typename IteratorTraits<U*>::ValueType) * static_cast<size_t>(sourceLast - sourceFirst)
            ));
        }

        template<typename T, typename U>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<T*>::ValueType>::Value && 
            IsTriviallyCopyable<typename IteratorTraits<U*>::ValueType>::Value,
            T*
        >::Type
        /// @brief Copies a range of memory from one location to another
        /// @param sourceFirst The beginning of the range to copy
        /// @param count The number of elements to copy
        /// @param destinationFirst The beginning of the range to copy to
        /// @return A pointer to the destination
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Copy(const T* sourceFirst, size_t count, U* destinationFirst) __WSTL_NOEXCEPT__ {
            return reinterpret_cast<T*>(memcpy(
                reinterpret_cast<void*>(destinationFirst),
                reinterpret_cast<const void*>(sourceFirst), 
                sizeof(typename IteratorTraits<U*>::ValueType) * count
            ));
        }

        // Set - memset

        template<typename Pointer, typename T>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, 
            Pointer
        >::Type
        /// @brief Sets a range of memory to a specified value
        /// @param first The beginning of the range to set
        /// @param last The end of the range to set
        /// @param value The value to set the range to
        /// @return A pointer to the beginning of the range
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Set(Pointer first, const Pointer last, T value) __WSTL_NOEXCEPT__ {
            return reinterpret_cast<Pointer>(memset(
                reinterpret_cast<void*>(first),
                static_cast<char>(value), 
                sizeof(typename IteratorTraits<Pointer>::ValueType) * static_cast<size_t>(last - first)
            ));
        }

        template<typename Pointer, typename T>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, 
            Pointer
        >::Type
        /// @brief Sets a range of memory to a specified value
        /// @param first The beginning of the range to set
        /// @param count The number of elements to set
        /// @param value The value to set the range to
        /// @return A pointer to the beginning of the range
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Set(Pointer first, size_t count, T value) __WSTL_NOEXCEPT__ {
            return reinterpret_cast<Pointer>(memset(
                reinterpret_cast<void*>(first),
                static_cast<char>(value), 
                sizeof(typename IteratorTraits<Pointer>::ValueType) * count
            ));
        }

        // Move - memmove

        template<typename T, typename U>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<T*>::ValueType>::Value &&
            IsTriviallyCopyable<typename IteratorTraits<U*>::ValueType>::Value, 
            T*
        >::Type
        /// @brief Moves a range of memory from one location to another, handling overlapping regions
        /// @param sourceFirst The beginning of the range to move
        /// @param sourceLast The end of the range to move
        /// @param destinationFirst The beginning of the destination range
        /// @return A pointer to the beginning of the destination range
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Move(const T* sourceFirst, const T* sourceLast, U* destinationFirst) __WSTL_NOEXCEPT__ {
            return reinterpret_cast<T*>(memmove(
                reinterpret_cast<void*>(destinationFirst),
                reinterpret_cast<const void*>(sourceFirst), 
                sizeof(typename IteratorTraits<U*>::ValueType) * static_cast<size_t>(sourceLast - sourceFirst)
            ));
        }

        template<typename T, typename U>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<T*>::ValueType>::Value &&
            IsTriviallyCopyable<typename IteratorTraits<U*>::ValueType>::Value, 
            T*
        >::Type
        /// @brief Moves a range of memory from one location to another, handling overlapping regions
        /// @param sourceFirst The beginning of the range to move
        /// @param count The number of elements to move
        /// @param destinationFirst The beginning of the destination range
        /// @return A pointer to the beginning of the destination range
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Move(const T* sourceFirst, size_t count, U* destinationFirst) __WSTL_NOEXCEPT__ {
            return reinterpret_cast<T*>(memmove(
                reinterpret_cast<void*>(destinationFirst),
                reinterpret_cast<const void*>(sourceFirst), 
                sizeof(typename IteratorTraits<U*>::ValueType) * count
            ));
        }

        // Compare - memcmp

        template<typename T, typename U>
        __WSTL_NODISCARD__
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<T*>::ValueType>::Value && 
            IsTriviallyCopyable<typename IteratorTraits<U*>::ValueType>::Value, 
            int
        >::Type
        /// @brief Compares two ranges of memory
        /// @param first1 The beginning of the first range
        /// @param last1 The end of the first range
        /// @param first2 The beginning of the second range
        /// @return Zero if equal, negative if `first1` < `first2`, positive if `first1` > `first2`
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Compare(const T* first1, const T* last1, const U* first2) __WSTL_NOEXCEPT__ {
            return memcmp(
                reinterpret_cast<const void*>(first1),
                reinterpret_cast<const void*>(first2), 
                sizeof(typename IteratorTraits<T*>::ValueType) * static_cast<size_t>(last1 - first1)
            );
        }

        template<typename T, typename U>
        __WSTL_NODISCARD__
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<T*>::ValueType>::Value &&
            IsTriviallyCopyable<typename IteratorTraits<U*>::ValueType>::Value, 
            int
        >::Type
        /// @brief Compares two ranges of memory
        /// @param first1 The beginning of the first range
        /// @param count The number of elements to compare
        /// @param first2 The beginning of the second range
        /// @return Zero if equal, negative if `first1` < `first2`, positive if `first1` > `first2`
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Compare(const T* first1, size_t count, const U* first2) __WSTL_NOEXCEPT__ {
            return memcmp(
                reinterpret_cast<const void*>(first1),
                reinterpret_cast<const void*>(first2), 
                sizeof(typename IteratorTraits<T*>::ValueType) * count
            );
        }

        // Locate - memchr

        template<typename Pointer, typename T>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value && 
            !IsConst<typename RemovePointer<Pointer>::Type>::Value, 
            char*
        >::Type
        /// @brief Finds the first occurrence of a byte value in a range of memory
        /// @param first The beginning of the range
        /// @param last The end of the range
        /// @param value The byte value to find
        /// @return A pointer to the first occurrence of `value` in the range, or `last` if not found
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Locate(Pointer first, Pointer last, T value) __WSTL_NOEXCEPT__ {
            void* result = memchr(
                reinterpret_cast<const void*>(first), 
                static_cast<char>(value), 
                sizeof(typename IteratorTraits<Pointer>::ValueType) * static_cast<size_t>(last - first)
            );
            
            return reinterpret_cast<char*>((result == 0) ? last : result);
        }

        template<typename Pointer, typename T>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value && 
            !IsConst<typename RemovePointer<Pointer>::Type>::Value, 
            char*
        >::Type
        /// @brief Finds the first occurrence of a byte value in a range of memory
        /// @param first The beginning of the range
        /// @param count The number of elements to search
        /// @param value The byte value to find
        /// @return A pointer to the first occurrence of `value` in the range, or `last` if not found
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Locate(Pointer first, size_t count, T value) __WSTL_NOEXCEPT__ {
            void* result = memchr(
                reinterpret_cast<const void*>(first), 
                static_cast<char>(value), 
                sizeof(typename IteratorTraits<Pointer>::ValueType) * count
            );
            
            return reinterpret_cast<char*>((result == 0) ? first + count : result);
        }

        template<typename Pointer, typename T>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value && 
            IsConst<typename RemovePointer<Pointer>::Type>::Value, 
            const char*
        >::Type
        /// @brief Finds the first occurrence of a byte value in a range of memory
        /// @param first The beginning of the range
        /// @param last The end of the range
        /// @param value The byte value to find
        /// @return A const pointer to the first occurrence of `value` in the range, or `last` if not found
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Locate(Pointer first, Pointer last, T value) __WSTL_NOEXCEPT__ {
            const void* result = memchr(
                reinterpret_cast<const void*>(first), 
                static_cast<char>(value), 
                sizeof(typename IteratorTraits<Pointer>::ValueType) * static_cast<size_t>(last - first)
            );
            
            return reinterpret_cast<const char*>((result == 0) ? last : result);
        }

        template<typename Pointer, typename T>
        inline typename EnableIf<
            IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value && 
            IsConst<typename RemovePointer<Pointer>::Type>::Value, 
            const char*
        >::Type
        /// @brief Finds the first occurrence of a byte value in a range of memory
        /// @param first The beginning of the range
        /// @param count The number of elements to search
        /// @param value The byte value to find
        /// @return A const pointer to the first occurrence of `value` in the range, or `last` if not found
        /// @note Requires `__WSTL_LIBC_WRAPPERS__` to be defined
        /// @ingroup memory
        Locate(Pointer first, size_t count, T value) __WSTL_NOEXCEPT__ {
            const void* result = memchr(
                reinterpret_cast<const void*>(first), 
                static_cast<char>(value), 
                sizeof(typename IteratorTraits<Pointer>::ValueType) * count
            );
            
            return reinterpret_cast<const char*>((result == 0) ? first + count : result);
        }
    }
    #endif
}

#endif