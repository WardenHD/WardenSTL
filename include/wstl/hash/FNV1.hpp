// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_FNV1_HPP__
#define __WSTL_FNV1_HPP__

#include "../private/Platform.hpp"
#include "../HasherBase.hpp"
#include <stdint.h>


namespace wstl {
    // FNV constants

    namespace __private {
        template<typename T>
        struct __FNVConstants;

        template<>
        struct __FNVConstants<uint32_t> {
            static const __WSTL_INLINE_VARIABLE__ __WSTL_CONSTEXPR__ uint32_t OFFSET_BASIS = 0x811C9DC5UL;
            static const __WSTL_INLINE_VARIABLE__ __WSTL_CONSTEXPR__ uint32_t PRIME = 0x01000193UL;
        };

        template<>
        struct __FNVConstants<uint64_t> {
            static const __WSTL_INLINE_VARIABLE__ __WSTL_CONSTEXPR__ uint64_t OFFSET_BASIS = 0xCBF29CE484222325ULL;
            static const __WSTL_INLINE_VARIABLE__ __WSTL_CONSTEXPR__ uint64_t PRIME = 0x00000100000001B3ULL;
        };
    }

    namespace hash {
        // FNV1

        /// @brief FNV-1 hash function implementation
        /// @tparam T The type of the hash value (either uint32_t or uint64_t)
        /// @ingroup hash
        template<typename T>
        class FNV1 : public HasherBase<FNV1<T>, T, uint8_t> {
        private:
            typedef __private::__FNVConstants<T> Constants;
            typedef HasherBase<FNV1<T>, T, uint8_t> Base;

        public:
            typedef typename Base::HashType HashType;
            typedef typename Base::ValueType ValueType;

            StaticAssert((IsSame<T, uint32_t>::Value || IsSame<T, uint64_t>::Value), "Only 32 and 64-bit types are supported");

            /// @brief Default constructor
            __WSTL_CONSTEXPR14__ FNV1() {
                Reset();
            }

            /// @brief Constructor that initializes the hasher with a range of values
            /// @param first The beginning of the range
            /// @param last The end of the range
            template<typename Iterator>
            __WSTL_CONSTEXPR14__ FNV1(Iterator first, Iterator last) {
                StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
                Reset();
                this->Append(first, last);
            }

            /// @brief Resets the hasher to its initial state
            __WSTL_CONSTEXPR14__ void Reset() {
                this->m_Hash = Constants::OFFSET_BASIS;
            }

            /// @brief Pushes a value into the hasher
            /// @param value The value to be hashed
            __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
                this->m_Hash *= Constants::PRIME;
                this->m_Hash ^= value;
            }

        private:
            friend Base;

            /// @brief Finalizes the hash computation, does nothing
            __WSTL_CONSTEXPR14__ void Finalize() {}
        };

        /// @brief Type alias for 32-bit FNV-1 hash
        /// @ingroup hash
        typedef FNV1<uint32_t> FNV1_32;

        /// @brief Type alias for 64-bit FNV-1 hash
        /// @ingroup hash
        typedef FNV1<uint64_t> FNV1_64;

        // FNV1a

        /// @brief FNV-1a hash function implementation
        /// @tparam T The type of the hash value (uint32_t or uint64_t)
        /// @ingroup hash
        template<typename T>
        class FNV1a : public HasherBase<FNV1a<T>, T, uint8_t> {
        private:
            typedef __private::__FNVConstants<T> Constants;
            typedef HasherBase<FNV1a<T>, T, uint8_t> Base;

        public:
            typedef typename Base::HashType HashType;
            typedef typename Base::ValueType ValueType;

            StaticAssert((IsSame<T, uint32_t>::Value || IsSame<T, uint64_t>::Value), "Only 32 and 64-bit types are supported");

            /// @brief Default constructor
            __WSTL_CONSTEXPR14__ FNV1a() {
                Reset();
            }

            /// @brief Constructor that initializes the hasher with a range of values
            /// @param first The beginning of the range
            /// @param last The end of the range
            template<typename Iterator>
            __WSTL_CONSTEXPR14__ FNV1a(Iterator first, Iterator last) {
                StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
                Reset();
                this->Append(first, last);
            }
            
            /// @brief Resets the hasher to its initial state
            __WSTL_CONSTEXPR14__ void Reset() {
                this->m_Hash = Constants::OFFSET_BASIS;
            }

            /// @brief Pushes a value into the hasher
            /// @param value The value to be hashed
            __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
                this->m_Hash ^= value;
                this->m_Hash *= Constants::PRIME;
            }

        private:
            friend Base;

            /// @brief Finalizes the hash computation, does nothing
            __WSTL_CONSTEXPR14__ void Finalize() {}
        };

        /// @brief Type alias for 32-bit FNV-1a hash
        /// @ingroup hash
        typedef FNV1a<uint32_t> FNV1a_32;

        /// @brief Type alias for 64-bit FNV-1a hash
        /// @ingroup hash
        typedef FNV1a<uint64_t> FNV1a_64;
    }
}

#endif