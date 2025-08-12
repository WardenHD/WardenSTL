// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_PEARSON_HPP__
#define __WSTL_PEARSON_HPP__

#include "../private/Platform.hpp"
#include "../HasherBase.hpp"
#include <stdint.h>


namespace wstl {
    namespace hash {
        /// @brief Pearson hash function implementation
        /// @ingroup hash
        class Pearson : public HasherBase<Pearson, uint8_t, uint8_t> {
        private:
            typedef HasherBase<Pearson, uint8_t, uint8_t> Base;

        public:
            typedef typename Base::HashType HashType;
            typedef typename Base::ValueType ValueType;

            /// @brief Default constructor
            __WSTL_CONSTEXPR14__ Pearson() {
                Reset();
            }

            /// @brief Constructor that initializes the hasher with a range of values
            /// @param first The beginning of the range
            /// @param last The end of the range
            template<typename Iterator>
            __WSTL_CONSTEXPR14__ Pearson(Iterator first, Iterator last) {
                StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
                Reset();
                this->Append(first, last);
            }

            /// @brief Resets the hasher to its initial state
            __WSTL_CONSTEXPR14__ void Reset() {
                this->m_Hash = 0;
            }

            /// @brief Pushes a value into the hasher
            /// @param value The value to be hashed
            __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
                this->m_Hash = PERMUTATION_TABLE[this->m_Hash ^ value];
            }

        private:
            static const __WSTL_INLINE_VARIABLE__ __WSTL_CONSTEXPR__ uint8_t PERMUTATION_TABLE[256] = {
                53, 11, 205, 219, 3, 76, 158, 70, 175, 217, 222, 197, 215, 111, 245, 201, 122, 110, 200, 
                88, 104, 33, 38, 230, 7, 127, 119, 116, 0, 162, 106, 109, 153, 142, 83, 164, 96, 128, 194, 
                90, 28, 234, 211, 163, 55, 220, 54, 32, 177, 50, 178, 150, 27, 233, 49, 79, 185, 59, 15, 
                186, 121, 97, 14, 147, 31, 145, 58, 154, 37, 192, 72, 61, 67, 89, 231, 140, 218, 26, 187, 
                176, 174, 255, 183, 213, 66, 12, 25, 168, 165, 210, 159, 251, 47, 102, 16, 188, 71, 207, 63, 
                129, 98, 6, 77, 117, 212, 151, 118, 1, 138, 101, 216, 238, 155, 94, 248, 113, 44, 60, 227, 
                91, 225, 5, 253, 190, 237, 87, 240, 246, 198, 131, 120, 51, 56, 170, 156, 228, 181, 236, 52, 
                241, 224, 157, 64, 45, 149, 169, 100, 23, 29, 182, 223, 208, 65, 195, 203, 82, 10, 161, 92, 
                229, 226, 4, 189, 42, 252, 115, 191, 135, 19, 68, 30, 57, 139, 206, 214, 114, 193, 8, 107, 74, 
                250, 235, 78, 247, 9, 22, 69, 249, 124, 144, 125, 171, 21, 84, 93, 75, 99, 18, 146, 13, 34, 
                73, 242, 254, 17, 24, 184, 20, 179, 173, 95, 243, 209, 133, 41, 148, 36, 108, 239, 196, 40, 
                81, 2, 86, 160, 62, 46, 167, 137, 221, 199, 48, 123, 103, 244, 112, 166, 143, 132, 204, 202, 
                130, 232, 105, 80, 43, 180, 39, 126, 136, 152, 141, 134, 172, 85, 35
            };

            friend Base;

            /// @brief Finalizes the hash value, does nothing
            __WSTL_CONSTEXPR14__ void Finalize() {}
        };
    }
}

#endif