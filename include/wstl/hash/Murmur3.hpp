// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_MURMUR3_HPP__
#define __WSTL_MURMUR3_HPP__

#include "../private/Platform.hpp"
#include "../private/Error.hpp"
#include "../Bit.hpp"
#include "../HasherBase.hpp"
#include "../StandardExceptions.hpp"
#include <stdint.h>


namespace wstl {
    namespace hash {
        /// @brief Murmur3 hash function implementation
        /// @tparam T The type of the value being hashed (either uint32_t or uint64_t)
        /// @ingroup hash
        template<typename T>
        class Murmur3 : public HasherBase<Murmur3<T>, T, uint8_t> {
        private:
            typedef HasherBase<Murmur3<T>, T, uint8_t> Base;

        public:
            typedef typename Base::HashType HashType;
            typedef typename Base::ValueType ValueType;

            StaticAssert((IsSame<T, uint32_t>::Value || IsSame<T, uint64_t>::Value), "Only 32 and 64-bit types are supported");

            /// @brief Default constructor
            /// @param seed Initial seed value for the hash, defaults to 0
            __WSTL_CONSTEXPR14__ Murmur3(HashType seed = 0) : m_Seed(seed) {
                Reset();
            }
            
            /// @brief Constructor that initializes the hasher with a range of values
            /// @param first The beginning of the range
            /// @param last The end of the range
            /// @param seed Initial seed value for the hash, defaults to 0
            template<typename Iterator>
            __WSTL_CONSTEXPR14__ Murmur3(Iterator first, Iterator last, HashType seed = 0) : m_Seed(seed) {
                StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
                Reset();
                this->Append(first, last);
            }

            /// @brief Resets the hasher to its initial state
            __WSTL_CONSTEXPR14__ void Reset() {
                this->m_Hash = m_Seed;
                m_Block = 0;
                m_BlockSize = 0;
                m_CharCount = 0;
                m_IsFinalized = false;
            }

            /// @brief Pushes a value into the hasher
            /// @param value The value to be hashed
            /// @throws `LogicError` if the hasher is already finalized
            __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
                __WSTL_ASSERT_RETURN__(!m_IsFinalized, WSTL_MAKE_EXCEPTION(LogicError, "Cannot add value to finalized Murmur3 hash"));

                m_Block |= value << (m_BlockSize * 8);
                
                if(++m_BlockSize == 4) {
                    m_Block *= CONSTANT1;
                    m_Block = RotateLeft(m_Block, ROTATE1);
                    m_Block *= CONSTANT2;

                    this->m_Hash ^= m_Block;
                    this->m_Hash = RotateLeft(this->m_Hash, ROTATE2);
                    this->m_Hash = (this->m_Hash * MULTIPLY) + ADD;

                    m_Block = 0;
                    m_BlockSize = 0;
                }

                ++m_CharCount;
            }

        private:
            HashType m_Seed;
            HashType m_Block;       
            uint8_t m_BlockSize;
            HashType m_CharCount;
            bool m_IsFinalized;

            friend Base;

            /// @brief Finalizes the hash value
            __WSTL_CONSTEXPR14__ void Finalize() {
                if(!m_IsFinalized) {
                    if(m_BlockSize > 0) {
                        m_Block *= CONSTANT1;
                        m_Block = RotateLeft(m_Block, ROTATE1);
                        m_Block *= CONSTANT2;
                        this->m_Hash ^= m_Block;
                    }
                
                    this->m_Hash ^= m_CharCount;
                    this->m_Hash ^= (this->m_Hash >> 16U);
                    this->m_Hash *= 0x85EBCA6BUL;
                    this->m_Hash ^= (this->m_Hash >> 13U);
                    this->m_Hash *= 0xC2B2AE35UL;
                    this->m_Hash ^= (this->m_Hash >> 16U);
                    m_IsFinalized = true;
                }
            }
            
            static const __WSTL_CONSTEXPR__ __WSTL_INLINE_VARIABLE__ HashType CONSTANT1 = 0xCC9E2D51UL;
            static const __WSTL_CONSTEXPR__ __WSTL_INLINE_VARIABLE__ HashType CONSTANT2 = 0x1B873593UL;
            static const __WSTL_CONSTEXPR__ __WSTL_INLINE_VARIABLE__ HashType MULTIPLY = 5U;
            static const __WSTL_CONSTEXPR__ __WSTL_INLINE_VARIABLE__ HashType ADD = 0xE6546B64UL;
            static const __WSTL_CONSTEXPR__ __WSTL_INLINE_VARIABLE__ HashType ROTATE1 = 15UL;
            static const __WSTL_CONSTEXPR__ __WSTL_INLINE_VARIABLE__ HashType ROTATE2 = 13UL;
        };

        /// @brief Type alias for 32-bit Murmur3 hash
        /// @ingroup hash
        typedef Murmur3<uint32_t> Murmur3_32;

        /// @brief Type alias for 64-bit Murmur3 hash
        /// @ingroup hash
        typedef Murmur3<uint64_t> Murmur3_64;
    }
}

#endif