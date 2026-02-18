// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_HASHERBASE_HPP__
#define __WSTL_HASHERBASE_HPP__

#include "private/Platform.hpp"
#include "Iterator.hpp"


namespace wstl {
    /// @brief Base class for hash/checksum algorithms
    /// @tparam Derived The derived class that implements the hashing logic
    /// @tparam THash The type of the hash value, typically an unsigned integer type
    /// @tparam TValue The type of the value being hashed, typically an unsigned integer type
    /// @ingroup hash
    template<typename Derived, typename THash, typename TValue>
    class HasherBase {
    public:
        typedef BackInsertIterator<Derived> InsertIterator;
        typedef THash HashType;
        typedef TValue ValueType;

        WSTL_STATIC_ASSERT(IsUnsigned<ValueType>::Value && IsUnsigned<HashType>::Value, "Unsigned types in derived class are not supported");
        
        /// @brief Pushes a value into the hasher
        /// @details This method must be implemented by the derived class
        /// @param value The value to be hashed
        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            static_cast<Derived*>(this)->PushBack(value);
        }

        /// @brief Appends a range to the hasher
        /// @param first The beginning of the range
        /// @param last The end of the range
        template<typename InputIterator>
        __WSTL_CONSTEXPR14__ void Append(InputIterator first, InputIterator last) {
            for(; first != last; ++first) PushBack(*first);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Appends a container to the hasher
        /// @param container The container whose elements will be hashed
        template<typename Container>
        __WSTL_CONSTEXPR14__ void Append(Container&& container) {
            Append(MakeMoveIterator(container.Begin()), MakeMoveIterator(container.End()));
        }
        #else
        /// @brief Appends a container to the hasher
        /// @param container The container whose elements will be hashed
        template<typename Container>
        void Append(const Container& container) {
            Append(container.Begin(), container.End());
        }
        #endif

        /// @brief Resets the hasher to its initial state
        /// @details This method must be implemented by the derived class
        __WSTL_CONSTEXPR14__ void Reset() {
            static_cast<Derived*>(this)->Reset();
        }

        /// @brief Finalizes and gets the final hash value
        /// @details The `Finalize` method must be implemented by the derived class
        /// @return The final hash value
        __WSTL_CONSTEXPR14__ HashType Value() {
            static_cast<Derived*>(this)->Finalize();
            return m_Hash;
        }

        /// @brief Conversion operator to get the final hash value
        /// @details The `Finalize` method must be implemented by the derived class
        /// @return The final hash value
        __WSTL_CONSTEXPR14__ operator HashType() {
            static_cast<Derived*>(this)->Finalize();
            return m_Hash;
        }
        
        /// @brief Returns a back insert iterator for the hasher
        __WSTL_CONSTEXPR14__ InsertIterator Inserter() {
            return InsertIterator(*static_cast<Derived*>(this));
        }

    protected:
        HashType m_Hash;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ HasherBase() {}
    };
}

#endif