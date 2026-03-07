// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_CONTAINER_HPP__
#define __WSTL_CONTAINER_HPP__

#include "private/Platform.hpp"
#include "TypeTraits.hpp"
#include "Memory.hpp"
#include <stddef.h>


namespace wstl {
    // Storage types

    /// @brief Storage type representing a fixed-size buffer
    /// @tparam T Type of the object to store
    /// @tparam N Number of objects in the buffer
    /// @ingroup containers
    template<typename T, size_t N>
    struct FixedStorage {
        typedef T ValueType;
        typedef size_t SizeType;

        ValueType Data[N];
        static const __WSTL_CONSTEXPR__ SizeType Capacity = N;
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename FixedStorage<T, N>::SizeType FixedStorage<T, N>::Capacity;

    /// @brief Storage type representing an external buffer
    /// @tparam T Type of the object to store, default is `void`
    /// @ingroup containers
    template<typename T = void>
    struct ExternalStorage {
        typedef T ValueType;
        typedef size_t SizeType;

        ValueType* Data;
        SizeType Capacity;

        /// @brief Constructor
        /// @param buffer Pointer to the external buffer
        /// @param capacity The capacity of the external buffer
        ExternalStorage(ValueType* buffer, SizeType capacity) : Data(buffer), Capacity(capacity) {}
    };

    /// @brief Storage type representing a fixed-size external buffer
    /// @tparam T Type of the object to store
    /// @tparam N Number of objects in the buffer
    /// @ingroup containers
    template<typename T, size_t N>
    struct FixedExternalStorage {
        typedef T ValueType;
        typedef size_t SizeType;
        
        ValueType* Data;
        static const __WSTL_CONSTEXPR__ SizeType Capacity = N;

        /// @brief Constructor
        /// @param buffer Pointer to the external buffer
        FixedExternalStorage(ValueType* buffer) : Data(buffer) {}
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename FixedExternalStorage<T, N>::SizeType FixedExternalStorage<T, N>::Capacity;

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T, size_t N>
    FixedExternalStorage(T (&)[N]) -> FixedExternalStorage<T, N>;
    #endif

    // Storage traits
    
    /// @brief Traits class for container storage types
    /// @tparam Storage The storage type to define traits for
    /// @ingroup containers
    template<typename Storage>
    struct StorageTraits {
        /// @brief The type of the storage
        typedef Storage StorageType;
        /// @brief The type of the elements stored in the storage
        typedef typename Storage::ValueType ValueType;
        /// @brief The type used for sizes and indices
        typedef typename Storage::SizeType SizeType;

        /// @brief Rebinds the storage type to a different value type
        /// @tparam U The new value type to rebind to
        template<typename U>
        struct Rebind { typedef typename Storage::Rebind<U>::Other Other; };

        /// @brief Indicates whether the storage type supports swapping itself without moving elements
        static const __WSTL_CONSTEXPR__ bool IsSwappable = false;
    };

    template<typename Storage>
    const __WSTL_CONSTEXPR__ bool StorageTraits<Storage>::IsSwappable;

    template<typename T, size_t N>
    struct StorageTraits<FixedStorage<T, N> > {
        /// @brief The type of the storage
        typedef FixedStorage<T, N> StorageType;
        /// @brief The type of the elements stored in the storage
        typedef T ValueType;
        /// @brief The type used for sizes and indices
        typedef size_t SizeType;

        /// @brief Rebinds the storage type to a different value type
        /// @tparam U The new value type to rebind to
        template<typename U>
        struct Rebind { typedef FixedStorage<U, N> Other; };

        /// @brief Indicates whether the storage type supports swapping itself without moving elements
        static const __WSTL_CONSTEXPR__ bool IsSwappable = false;
    };

    template<typename T>
    struct StorageTraits<ExternalStorage<T> > {
        /// @brief The type of the storage
        typedef ExternalStorage<T> StorageType;
        /// @brief The type of the elements stored in the storage
        typedef T ValueType;
        /// @brief The type used for sizes and indices
        typedef size_t SizeType;

        /// @brief Rebinds the storage type to a different value type
        /// @tparam U The new value type to rebind to
        template<typename U>
        struct Rebind { typedef ExternalStorage<U> Other; };

        /// @brief Indicates whether the storage type supports swapping itself without moving elements
        static const __WSTL_CONSTEXPR__ bool IsSwappable = true;
    };

    template<typename T, size_t N>
    struct StorageTraits<FixedExternalStorage<T, N> > {
        /// @brief The type of the storage
        typedef FixedExternalStorage<T, N> StorageType;
        /// @brief The type of the elements stored in the storage
        typedef T ValueType;
        /// @brief The type used for sizes and indices
        typedef size_t SizeType;

        /// @brief Rebinds the storage type to a different value type
        /// @tparam U The new value type to rebind to
        template<typename U>
        struct Rebind { typedef FixedExternalStorage<U, N> Other; };

        static const __WSTL_CONSTEXPR__ bool IsSwappable = true;
    };

    /// @brief Base class for all containers
    /// @tparam Storage The storage type used by the container (default is `void` for no storage)
    /// @details Storage is required to support the following types and members:
    /// - `ValueType` - type of the elements stored in the container
    /// - `SizeType` - type used for sizes and indices
    /// - `.Data` - member representing the underlying storage of the container
    /// - `.Capacity` - static constant representing the maximum number of elements the storage can hold
    ///
    /// @ingroup containers
    template<typename Storage = void, bool = IsDefaultConstructible<Storage>::Value>
    class ContainerBase;
    
    template<typename Storage>
    class ContainerBase<Storage, true> {
    public:
        typedef typename Storage::ValueType ValueType;
        typedef typename Storage::SizeType SizeType;

        typedef Storage StorageType;

        /// @brief Gets the current size of the container
        __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__ {
            return m_CurrentSize;
        }

        /// @brief Gets the capacity of the container
        __WSTL_CONSTEXPR__ SizeType Capacity() const __WSTL_NOEXCEPT__ {
            return m_Storage.Capacity;
        }

        /// @brief Gets the maximum size of the container
        __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__ {
            return m_Storage.Capacity;
        }

        /// @brief Checks if the container is empty
        __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__ {
            return Size() == 0;
        }

        /// @brief Checks if the container is full
        __WSTL_CONSTEXPR__ bool Full() const __WSTL_NOEXCEPT__ {
            return Size() == Capacity();
        }

        /// @brief Gets the available space in the container
        __WSTL_CONSTEXPR__ SizeType Available() const __WSTL_NOEXCEPT__ {
            return MaxSize() - Size();
        }

    protected:
        /// @brief Protected default constructor
        /// @details Only available if Storage is default-constructible
        ContainerBase() : m_Storage(), m_CurrentSize(0) {}

        /// @brief Protected destructor
        ~ContainerBase() {}

        Storage m_Storage;
        SizeType m_CurrentSize;
    };

    template<typename Storage>
    class ContainerBase<Storage, false> {
    public:
        typedef typename Storage::ValueType ValueType;
        typedef typename Storage::SizeType SizeType;

        typedef Storage StorageType;

        /// @brief Gets the current size of the container
        __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__ {
            return m_CurrentSize;
        }

        /// @brief Gets the capacity of the container
        __WSTL_CONSTEXPR__ SizeType Capacity() const __WSTL_NOEXCEPT__ {
            return m_Storage.Capacity;
        }

        /// @brief Gets the maximum size of the container
        __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__ {
            return m_Storage.Capacity;
        }

        /// @brief Checks if the container is empty
        __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__ {
            return Size() == 0;
        }

        /// @brief Checks if the container is full
        __WSTL_CONSTEXPR__ bool Full() const __WSTL_NOEXCEPT__ {
            return Size() == Capacity();
        }

        /// @brief Gets the available space in the container
        __WSTL_CONSTEXPR__ SizeType Available() const __WSTL_NOEXCEPT__ {
            return MaxSize() - Size();
        }

    protected:
        /// @brief Protected constructor with storage parameter
        /// @details Only available if Storage is not default-constructible
        /// @param storage The storage to use for the container
        ContainerBase(const Storage& storage) : m_Storage(storage), m_CurrentSize(0) {}

        /// @brief Protected destructor
        ~ContainerBase() {}

        Storage m_Storage;
        SizeType m_CurrentSize;
    };

    template<>
    class ContainerBase<void, false> {
    public:
        typedef size_t SizeType;

        __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__ {
            return m_CurrentSize;
        }

        /// @brief Gets the capacity of the container
        __WSTL_CONSTEXPR__ SizeType Capacity() const __WSTL_NOEXCEPT__ {
            return m_Capacity;
        }

        /// @brief Gets the maximum size of the container
        __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__ {
            return m_Capacity;
        }

        /// @brief Checks if the container is empty
        __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__ {
            return Size() == 0;
        }

        /// @brief Checks if the container is full
        __WSTL_CONSTEXPR__ bool Full() const __WSTL_NOEXCEPT__ {
            return Size() == Capacity();
        }

        /// @brief Gets the available space in the container
        __WSTL_CONSTEXPR__ SizeType Available() const __WSTL_NOEXCEPT__ {
            return MaxSize() - Size();
        }

    protected:
        /// @brief Protected constructor
        /// @param capacity The maximum number of elements container can hold
        ContainerBase(SizeType capacity) : m_Capacity(capacity), m_CurrentSize(0) {}

        /// @brief Protected destructor
        ~ContainerBase() {}

        SizeType m_CurrentSize;
        const SizeType m_Capacity;
    };

    /// @brief Base class for all typed containers
    /// @tparam Storage The storage type used by the contains
    /// @tparam T Type of the element to store in the container, must be provided manually if storage is `void`
    /// @details Storage is required to support the following types and members:
    /// - `ValueType` - type of the elements stored in the container, here it must not be `void`
    /// - `SizeType` - type used for sizes and indices
    /// - `.Data` - member representing the underlying storage of the container
    /// - `.Capacity` - static constant representing the maximum number of elements the storage can hold
    /// @ingroup containers
    template<typename Storage, typename T = void>
    class TypedContainerBase;
    
    template<>
    class TypedContainerBase<void, void> {
        // NO-OP
    };

    /// @brief Base class for all typed containers, version with storage
    /// @tparam Storage The storage type used by the contains
    /// @details Storage is required to support the following types and members:
    /// - `ValueType` - type of the elements stored in the container, here it must not be `void`
    /// - `SizeType` - type used for sizes and indices
    /// - `.Data` - member representing the underlying storage of the container
    /// - `.Capacity` - static constant representing the maximum number of elements the storage can hold
    /// @ingroup containers
    template<typename Storage>
    class TypedContainerBase<Storage, void> : public ContainerBase<Storage> {
    public:
        typedef typename ContainerBase<Storage>::ValueType ValueType;
        typedef typename ContainerBase<Storage>::SizeType SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef ValueType& ReferenceType;
        typedef const ValueType& ConstReferenceType;
        typedef ValueType* PointerType;
        typedef const ValueType* ConstPointerType;

        typedef typename ContainerBase<Storage>::StorageType StorageType;

        WSTL_STATIC_ASSERT(!IsVoid<ValueType>::Value, "Storage::ValueType must be non-void");

    protected:
        /// @brief Protected default constructor
        /// @details Only available if Storage is default-constructible
        TypedContainerBase() : ContainerBase<Storage>() {}

        /// @brief Protected constructor with storage parameter
        /// @details Only available if Storage is not default-constructible
        /// @param storage The storage to use for the container
        TypedContainerBase(const Storage& storage) : ContainerBase<Storage>(storage) {}
    };

    /// @brief Base class for all typed containers, version with storage and custom value type
    /// @tparam Storage The storage type used by the contains
    /// @tparam T Type of the element to store in the container, must be non-void
    /// @details Storage is required to support the following types and members:
    /// - `ValueType` - type of the elements stored in the container, here it must not be `void`
    /// - `SizeType` - type used for sizes and indices
    /// - `.Data` - member representing the underlying storage of the container
    /// - `.Capacity` - static constant representing the maximum number of elements the storage can hold
    /// @ingroup containers
    template<typename Storage, typename T>
    class TypedContainerBase : public ContainerBase<Storage> {
    public:
        typedef T ValueType;
        typedef typename ContainerBase<Storage>::SizeType SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef ValueType& ReferenceType;
        typedef const ValueType& ConstReferenceType;
        typedef ValueType* PointerType;
        typedef const ValueType* ConstPointerType;

        typedef typename ContainerBase<Storage>::StorageType StorageType;

        WSTL_STATIC_ASSERT(!IsVoid<ValueType>::Value, "T must be non-void");

    protected:
        /// @brief Protected default constructor
        /// @details Only available if Storage is default-constructible
        TypedContainerBase() : ContainerBase<Storage>() {}

        /// @brief Protected constructor with storage parameter
        /// @details Only available if Storage is not default-constructible
        /// @param storage The storage to use for the container
        TypedContainerBase(const Storage& storage) : ContainerBase<Storage>(storage) {}
    };

    /// @brief Base class for all typed containers, version without storage
    /// @tparam T Type of the element to store in the container, must be non-void
    /// @ingroup containers
    template<typename T>
    class TypedContainerBase<void, T> : public ContainerBase<> {
    public:
        typedef T ValueType;
        typedef typename ContainerBase<>::SizeType SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef ValueType& ReferenceType;
        typedef const ValueType& ConstReferenceType;
        typedef ValueType* PointerType;
        typedef const ValueType* ConstPointerType;

        WSTL_STATIC_ASSERT(!IsVoid<T>::Value, "T must be non-void");

    protected:
        /// @brief Protected constructor
        /// @param capacity The maximum number of elements container can hold
        TypedContainerBase(SizeType capacity) : ContainerBase<>(capacity) {}
    };
}

#endif