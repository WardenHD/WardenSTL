// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_JENKINS_HPP__
#define __WSTL_JENKINS_HPP__

#include "../private/Platform.hpp"
#include "../HasherBase.hpp"
#include <stdint.h>


namespace wstl {
    namespace hash {
        /// @brief Jenkins hash function implementation
        /// @ingroup hash
        class Jenkins : public HasherBase<Jenkins, uint32_t, uint8_t> {
        private:
            typedef HasherBase<Jenkins, uint32_t, uint8_t> Base;

        public:
            typedef typename Base::HashType HashType;
            typedef typename Base::ValueType ValueType;

            /// @brief Default constructor
            __WSTL_CONSTEXPR14__ Jenkins() : m_IsFinalized(false) {
                m_Hash = 0;
            }

            /// @brief Constructor that initializes the hasher with a range of values
            /// @param first The beginning of the range
            /// @param last The end of the range
            template<typename Iterator>
            __WSTL_CONSTEXPR14__ Jenkins(Iterator first, Iterator last) : m_IsFinalized(false) {
                StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
                m_Hash = 0;
                this->Append(first, last);
            }

            /// @brief Resets the hasher to its initial state
            __WSTL_CONSTEXPR14__ void Reset() {
                m_Hash = 0;
                m_IsFinalized = false;
            }

            /// @brief Pushes a value into the hasher
            /// @param value The value to be hashed
            /// @throws `LogicError` if the hasher is already finalized
            __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
                __WSTL_ASSERT_RETURN__(!m_IsFinalized, WSTL_MAKE_EXCEPTION(LogicError, "Cannot add value to finalized Jenkins hash"));

                m_Hash += value;
                m_Hash += (m_Hash << 10);
                m_Hash ^= (m_Hash >> 6);
            }

        private:
            bool m_IsFinalized;

            friend Base;

            /// @brief Finalizes the hash value
            __WSTL_CONSTEXPR14__ void Finalize() {
                if(!m_IsFinalized) {
                    m_Hash += (m_Hash << 3);
                    m_Hash ^= (m_Hash >> 11);
                    m_Hash += (m_Hash << 15);
                }

                m_IsFinalized = true;
            }
        };
    }
}

#endif