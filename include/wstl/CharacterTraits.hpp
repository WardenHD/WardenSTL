// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s character traits implementation,
// but it has been re-implemented with custom logic for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_CHARACTERTRAITS_HPP__
#define __WSTL_CHARACTERTRAITS_HPP__

#include <stddef.h>
#include <stdint.h>
#include "private/Platform.hpp"
#include "NullPointer.hpp"
#include "Algorithm.hpp"


namespace wstl {
    namespace __private {
        template<typename T> 
        class __CharacterTraitsTypes;

        template<> 
        class __CharacterTraitsTypes<char> {
        public:
            typedef char CharacterType;
            typedef int IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef char StateType;
        };

        template<> 
        class __CharacterTraitsTypes<signed char> {
        public:
            typedef signed char CharacterType;
            typedef int IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef signed char StateType;
        };

        template<> 
        class __CharacterTraitsTypes<unsigned char> {
        public:
            typedef unsigned char CharacterType;
            typedef int IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef unsigned char StateType;
        };

        template<> 
        class __CharacterTraitsTypes<wchar_t> {
        public:
            typedef wchar_t CharacterType;
            typedef uint_least16_t IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef char StateType;
        };

        #ifdef __WSTL_CXX20__
        template<>
        class __CharacterTraitsTypes<char8_t> {
        public:
            typedef char8_t CharacterType;
            typedef unsigned int IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef char StateType;
        };
        #endif

        #ifdef __WSTL_CXX11__
        template<>
        class __CharacterTraitsTypes<char16_t> {
        public:
            typedef char16_t CharacterType;
            typedef uint_least16_t IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef char StateType;
        };

        template<>
        class __CharacterTraitsTypes<char32_t> {
        public:
            typedef char32_t CharacterType;
            typedef uint_least32_t IntegerType;
            typedef long long OffsetType;
            typedef size_t PositionType;
            typedef char StateType;
        };
        #endif
    }

    /// @brief Class that stores character traits and provides static methods for character operations
    /// @tparam T Character type
    /// @ingroup string
    /// @see https://en.cppreference.com/w/cpp/string/char_traits.html
    template<typename T>
    class CharacterTraits : public __private::__CharacterTraitsTypes<T> {
    public:
        typedef typename __private::__CharacterTraitsTypes<T>::CharacterType CharacterType;
        typedef typename __private::__CharacterTraitsTypes<T>::IntegerType IntegerType;
        typedef typename __private::__CharacterTraitsTypes<T>::OffsetType OffsetType;
        typedef typename __private::__CharacterTraitsTypes<T>::PositionType PositionType;
        typedef typename __private::__CharacterTraitsTypes<T>::StateType StateType;

        /// @brief Compares two characters for equality
        /// @param a First character
        /// @param b Second character
        static __WSTL_CONSTEXPR__ bool Equal(CharacterType a, CharacterType b) {
            return a == b;
        }

        /// @brief Compares two characters for less-than relation
        /// @param a First character
        /// @param b Second character
        static __WSTL_CONSTEXPR__ bool LessThan(CharacterType a, CharacterType b) {
            return a < b;
        }

        /// @brief Gets the length of a C-style string
        /// @param string C-style string pointer
        static __WSTL_CONSTEXPR14__ size_t Length(const CharacterType* string) {
            size_t count = 0;

            if(string != 0) while(*string++) ++count;
            return count;
        }

        /// @brief Gets the length of a C-style string, up to a maximum length
        /// @param string C-style string pointer
        /// @param maxLength Maximum length to check
        static __WSTL_CONSTEXPR14__ size_t Length(const CharacterType* string, size_t maxLength) {
            size_t count = 0;

            if(string != 0) while(*string++ && count < maxLength) ++count;
            return count;
        }

        /// @brief Assigns a character to a reference
        /// @param r Reference to the character
        /// @param c Character to assign
        static __WSTL_CONSTEXPR14__ void Assign(CharacterType& r, CharacterType c) {
            r = c;
        }

        /// @brief Assigns a character to a range of characters
        /// @param pointer Pointer to the start of the range
        /// @param count Number of characters in the range
        /// @param c Character to assign
        /// @return Pointer to the start of the range
        static __WSTL_CONSTEXPR14__ CharacterType* Assign(CharacterType* pointer, size_t count, CharacterType c) {
            if(pointer != NullPointer) FillInRange(pointer, count, c);
            return pointer;
        }

        /// @brief Moves a range of characters from source to destination, handling overlapping ranges
        /// @param destination Pointer to the destination range
        /// @param source Pointer to the source range
        /// @param count Number of characters to move
        /// @return Pointer to the destination range
        static __WSTL_CONSTEXPR14__ CharacterType* Move(CharacterType* destination, const CharacterType* source, size_t count) {
            if((destination < source) || (destination > (source + count))) 
                CopyInRange(source, count, destination);
            else 
                CopyInRange(ReverseIterator<const CharacterType*>(source + count), count, 
                    ReverseIterator<CharacterType*>(destination + count));

            return destination;
        }

        /// @brief Copies a range of characters from source to destination
        /// @param destination Pointer to the destination range
        /// @param source Pointer to the source range
        /// @param count Number of characters to copy
        /// @return Pointer to the destination range
        static __WSTL_CONSTEXPR14__ CharacterType* Copy(CharacterType* destination, const CharacterType* source, size_t count) {
            CopyInRange(source, count, destination);
            return destination;
        }

        /// @brief Compares two C-style strings for equality
        /// @param string1 First C-style string pointer
        /// @param string2 Second C-style string pointer
        /// @return `0` if equal, negative if `string1 < string2`, positive if `string1 > string2`
        static __WSTL_CONSTEXPR14__ int Compare(const CharacterType* string1, const CharacterType* string2, size_t count) {
            for(size_t i = 0; i < count; ++i) {
                const CharacterType* c1 = *string1++;
                const CharacterType* c2 = *string2++;

                if(c1 < c2) return -1;
                else if(c1 > c2) return 1;
            }

            return 0;
        }

        /// @brief Finds the first occurrence of a character in a range
        /// @param pointer Pointer to the start of the range
        /// @param count Number of characters in the range
        /// @param c Character to find
        /// @return Pointer to the first occurrence of the character, or `0` if not found
        static __WSTL_CONSTEXPR14__ const CharacterType* Find(const CharacterType* pointer, size_t count, const CharacterType& c) {
            for(size_t i = 0; i < count; ++i, ++pointer) if(*pointer == c) return pointer;
            return 0;
        }

        /// @brief Converts an integer to a character
        /// @param c Integer to convert
        static __WSTL_CONSTEXPR__ CharacterType ToCharacterType(IntegerType c) {
            return static_cast<CharacterType>(c);
        }
        
        /// @brief Converts a character to an integer
        /// @param c Character to convert
        static __WSTL_CONSTEXPR__ CharacterType ToIntegerType(CharacterType c) {
            return static_cast<IntegerType>(c);
        }

        /// @brief Compares two integers for equality
        /// @param c1 First integer
        /// @param c2 Second integer
        static __WSTL_CONSTEXPR__ CharacterType EqualsIntegerType(IntegerType c1, IntegerType c2) {
            return c1 == c2;
        }
        
        /// @brief Returns the end-of-file (EOF) value
        static __WSTL_CONSTEXPR__ IntegerType EOF() {
            return -1;
        }

        /// @brief Checks whether a character is not EOF
        /// @param e Integer to check
        static __WSTL_CONSTEXPR__ IntegerType NotEOF(IntegerType e) {
            return (e == EOF()) ? EOF() - 1 : e;
        }
    };

    namespace string {
        /// @brief Gets the length of a C-style string
        /// @param string C-style string pointer
        /// @ingroup string
        template<typename T>
        __WSTL_CONSTEXPR14__ size_t Length(const T* string) {
            return CharacterTraits<T>::Length(string);
        }

        /// @brief Gets the length of a C-style string, up to a maximum length
        /// @param string C-style string pointer
        /// @param maxLength Maximum length to check
        /// @ingroup string
        template<typename T>
        __WSTL_CONSTEXPR14__ size_t Length(const T* string, size_t maxLength) {
            return CharacterTraits<T>::Length(string, maxLength);
        }

        /// @brief Compares two C-style strings for equality
        /// @param string1 First C-style string pointer
        /// @param string2 Second C-style string pointer
        /// @return `0` if equal, negative if `string1 < string2`, positive if `string1 > string2`
        /// @ingroup string
        template<typename T>
        __WSTL_CONSTEXPR14__ int Compare(const T* string1, const T* string2) {
            return CharacterTraits<T>::Compare(string1, string2);
        }

        /// @brief Compares two C-style strings for equality, up to a specified length
        /// @param string1 First C-style string pointer
        /// @param string2 Second C-style string pointer
        /// @param count Number of characters to compare
        /// @return `0` if equal, negative if `string1 < string2`, positive if `string1 > string2`
        /// @ingroup string
        template<typename T>
        __WSTL_CONSTEXPR14__ int CompareInRange(const T* string1, const T* string2, size_t count) {
            while (count > 0) {
                if(*string1 < *string2) return -1;
                if(*string1 > *string2) return 1;

                ++string1;
                ++string2;
                --count;
            }

            return 0;
        }

        /// @brief Copies a C-style string to a destination buffer
        /// @param destination Pointer to the destination buffer
        /// @param source Pointer to the source C-style string
        /// @return Pointer to the destination buffer
        /// @ingroup string
        template<typename T>
        __WSTL_CONSTEXPR14__ T* Copy(T* destination, const T* source) {
            T* result = destination;
            
            while(*source) *destination++ = *source++;
            *destination = 0;

            return result;
        }

        /// @brief Copies a `count` number of characters from a C-style string to a destination buffer
        /// @param destination Pointer to the destination buffer
        /// @param source Pointer to the source C-style string
        /// @param count Number of characters to copy
        /// @return Pointer to the destination buffer
        /// @ingroup string
        template<typename T>
        __WSTL_CONSTEXPR14__ T* CopyInRange(T* destination, const T* source, size_t count) {
            return CharacterTraits<T>::Copy(destination, source, count);
        }
    }
}

#endif