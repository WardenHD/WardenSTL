#ifndef __WSTL_CHARACTERTRAITS_HPP__
#define __WSTL_CHARACTERTRAITS_HPP__

#include <stddef.h>
#include <stdint.h>
#include "private/Platform.hpp"
#include "NullPointer.hpp"
#include "Algorithm.hpp"

namespace wstl {
    template<typename T> 
    class CharacterTraitsTypes;

    template<> 
    class CharacterTraitsTypes<char> {
    public:
        typedef char CharacterType;
        typedef int IntegerType;
        typedef long long OffsetType;
        typedef size_t PositionType;
        typedef char StateType;
    };

    template<> 
    class CharacterTraitsTypes<signed char> {
    public:
        typedef signed char CharacterType;
        typedef int IntegerType;
        typedef long long OffsetType;
        typedef size_t PositionType;
        typedef signed char StateType;
    };

    template<> 
    class CharacterTraitsTypes<unsigned char> {
    public:
        typedef unsigned char CharacterType;
        typedef int IntegerType;
        typedef long long OffsetType;
        typedef size_t PositionType;
        typedef unsigned char StateType;
    };

    template<> 
    class CharacterTraitsTypes<wchar_t> {
    public:
        typedef wchar_t CharacterType;
        typedef uint_least16_t IntegerType;
        typedef long long OffsetType;
        typedef size_t PositionType;
        typedef char StateType;
    };

    #ifdef __WSTL_CXX20__
    template<>
    class CharacterTraitsTypes<char8_t> {
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
    class CharacterTraitsTypes<char16_t> {
    public:
        typedef char16_t CharacterType;
        typedef uint_least16_t IntegerType;
        typedef long long OffsetType;
        typedef size_t PositionType;
        typedef char StateType;
    };

    template<>
    class CharacterTraitsTypes<char32_t> {
    public:
        typedef char32_t CharacterType;
        typedef uint_least32_t IntegerType;
        typedef long long OffsetType;
        typedef size_t PositionType;
        typedef char StateType;
    };
    #endif

    template<typename T>
    class CharacterTraits : public CharacterTraitsTypes<T> {
    public:
        typedef typename CharacterTraitsTypes<T>::CharacterType CharacterType;
        typedef typename CharacterTraitsTypes<T>::IntegerType IntegerType;
        typedef typename CharacterTraitsTypes<T>::OffsetType OffsetType;
        typedef typename CharacterTraitsTypes<T>::PositionType PositionType;
        typedef typename CharacterTraitsTypes<T>::StateType StateType;

        static __WSTL_CONSTEXPR__ bool Equal(CharacterType a, CharacterType b) {
            return a == b;
        }

        static __WSTL_CONSTEXPR__ bool LessThan(CharacterType a, CharacterType b) {
            return a < b;
        }

        static __WSTL_CONSTEXPR14__ size_t Length(const CharacterType* string) {
            size_t count = 0;

            if(string != 0) while(*++string) count++;
            return count;
        }

        static __WSTL_CONSTEXPR14__ size_t Length(const CharacterType* string, size_t maxLength) {
            size_t count = 0;

            if(string != 0) while(*++string && count < maxLength) count++;
            return count;
        }

        static __WSTL_CONSTEXPR14__ CharacterType* Assign(CharacterType& r, CharacterType c) {
            r = c;
        }

        static __WSTL_CONSTEXPR14__ CharacterType* Assign(CharacterType* pointer, size_t count, CharacterType c) {
            if(pointer != NullPointer) FillInRange(pointer, count, c);
            return pointer;
        }

        static __WSTL_CONSTEXPR14__ CharacterType* Move(CharacterType* destination, const CharacterType* source, size_t count) {
            if((destination < source) || (destination > (source + count))) 
                CopyInRange(source, count, destination);
            else 
                CopyInRange(ReverseIterator<const CharacterType*>(source), count, 
                    ReverseIterator<CharacterType*>(destination));

            return destination;
        }

        static __WSTL_CONSTEXPR14__ CharacterType* Copy(CharacterType* destination, const CharacterType* source, size_t count) {
            CopyInRange(source, count, destination);
            return destination;
        }

        static __WSTL_CONSTEXPR14__ int Compare(const CharacterType* string1, const CharacterType* string2, size_t count) {
            for(size_t i = 0; i < count; ++i) {
                const CharacterType* c1 = *string1++;
                const CharacterType* c2 = *string2++;

                if(c1 < c2) return -1;
                else if(c1 > c2) return 1;
            }

            return 0;
        }

        static __WSTL_CONSTEXPR14__ const CharacterType* Find(const CharacterType* pointer, size_t count, const CharacterType& c) {
            for(size_t i = 0; i < count; i++, pointer++) if(*pointer == c) return pointer;
            return 0;
        }

        static __WSTL_CONSTEXPR__ CharacterType ToCharacterType(IntegerType c) {
            return static_cast<CharacterType>(c);
        }

        static __WSTL_CONSTEXPR__ CharacterType ToIntegerType(CharacterType c) {
            return static_cast<IntegerType>(c);
        }

        static __WSTL_CONSTEXPR__ CharacterType EqualsIntegerType(IntegerType c1, IntegerType c2) {
            return c1 == c2;
        }

        static __WSTL_CONSTEXPR__ IntegerType EOF() {
            return -1;
        }

        static __WSTL_CONSTEXPR__ IntegerType NotEOF(IntegerType e) {
            return (e == EOF()) ? EOF() - 1 : e;
        }
    };

    namespace string {
        template<typename T>
        __WSTL_CONSTEXPR14__ size_t Length(const T* t) {
            return CharacterTraits<T>::Length(t);
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ size_t Length(const T* t, size_t maxLength) {
            return CharacterTraits<T>::Length(t, maxLength);
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ int Compare(const T* a, const T* b) {
            return CharacterTraits<T>::Compare(a, b);
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ int CompareInRange(const T* a, const T* b, size_t count) {
            while (count > 0) {
                if(*a < *b) return -1;
                if(*a > *b) return 1;

                a++;
                b++;
                count--;
            }

            return 0;
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ T* Copy(T* destination, const T* source) {
            T* result = destination;
            
            while(*source) *destination++ = *source++;
            *destination = 0;

            return result;
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ T* CopyInRange(T* destination, const T* source, size_t count) {
            return CharacterTraits<T>::Copy(destination, source, count);
        }
    }
}

#endif