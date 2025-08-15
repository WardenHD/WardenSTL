// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s bit.h and binary.h,
// but it has been re-implemented with custom logic for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.


#ifndef __WSTL_BIT_HPP__
#define __WSTL_BIT_HPP__

#include "private/Platform.hpp"
#include "TypeTraits.hpp"
#include "Memory.hpp"
#include "Byte.hpp"
#include "Array.hpp"
#include "Limits.hpp"

#ifndef __WSTL_ENDIAN_NATIVE__
    #ifdef __BYTE_ORDER__
        #ifdef __ORDER_LITTLE_ENDIAN__
            #define __WSTL_LITTLE_ENDIAN__ __ORDER_LITTLE_ENDIAN__
            #define __WSTL_BIG_ENDIAN__ __ORDER_BIG_ENDIAN__
            #define __WSTL_NATIVE_ENDIAN__ __BYTE_ORDER__
        #elif defined(__LITTLE_ENDIAN__)
            #define __WSTL_LITTLE_ENDIAN__ __LITTLE_ENDIAN__
            #define __WSTL_BIG_ENDIAN__ __BIG_ENDIAN__
            #define __WSTL_NATIVE_ENDIAN__ __BYTE_ORDER__
        #endif
    #else
        #error Unable to determine native endianness. Define __WSTL_NATIVE_ENDIAN__ as 0 for 'little endian' or 1 for 'big endian'.
    #endif
#else
    #define __WSTL_LITTLE_ENDIAN__ 0
    #define __WSTL_BIG_ENDIAN__ 1
#endif


namespace wstl {
    /// @brief Enum class representing the endianness of the system
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/types/endian
    __WSTL_ENUM_CLASS__(Endian) {
        Little = __WSTL_LITTLE_ENDIAN__,
        Big = __WSTL_BIG_ENDIAN__,
        Native = __WSTL_NATIVE_ENDIAN__
    };

    // Bit cast

    /// @brief Reinterprets the bits of one type as another type
    /// @tparam To Type to reinterpret the bits as
    /// @tparam From Type to reinterpret the bits from
    /// @param source The source value to reinterpret
    /// @return The value of type `To` that has the same bit representation as `source`
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/bit_cast
    template<typename To, typename From>
    __WSTL_NODISCARD__
    typename EnableIf<!IsIntegral<To>::Value && !IsIntegral<From>::Value && sizeof(To) == sizeof(From) && IsTriviallyCopyable<From>::Value && IsTriviallyCopyable<To>::Value, To>::Type
    BitCast(const From& source) __WSTL_NOEXCEPT__ {
        To destination;
        memory::Copy(&source, sizeof(To), &destination);
        return destination;
    }

    /// @brief Reinterprets the bits of one type as another type
    /// @tparam To Type to reinterpret the bits as
    /// @tparam From Type to reinterpret the bits from
    /// @param source The source value to reinterpret
    /// @return The value of type `To` that has the same bit representation as `source`
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/bit_cast
    template<typename To, typename From>
    __WSTL_NODISCARD__
    typename EnableIf<IsIntegral<To>::Value && IsIntegral<From>::Value && sizeof(To) == sizeof(From) && IsTriviallyCopyable<From>::Value && IsTriviallyCopyable<To>::Value, To>::Type
    BitCast(const From& source) __WSTL_NOEXCEPT__ {
        return static_cast<To>(source);
    }

    // Byte swap

    // 1-byte

    /// @brief Swaps the byte order of an integral value - for 1-byte types, no-op
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to swap
    /// @return The value with its byte order swapped
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/byteswap
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, T>::Type ByteSwap(T value) {
        return value;
    }

    // 2-byte

    /// @brief Swaps the byte order of an integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to swap
    /// @return The value with its byte order swapped
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/byteswap
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, T>::Type ByteSwap(T value) {
        return (value << 8) | (value >> 8);
    }

    // 4-byte

    /// @brief Swaps the byte order of an integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to swap
    /// @return The value with its byte order swapped
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/byteswap
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, T>::Type ByteSwap(T value) {
        value = (value >> 24) | ((value >> 8) & 0x0000FF00UL) | ((value << 8) & 0x00FF0000) | (value << 24);
        return value;
    }

    // 8-byte

    /// @brief Swaps the byte order of an integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to swap
    /// @return The value with its byte order swapped
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/byteswap
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, T>::Type ByteSwap(T value) {
        value = ((value & 0xFF00FF00FF00FF00ULL) >> 8) | ((value & 0x00FF00FF00FF00FFULL) << 8);
        value = ((value & 0xFFFF0000FFFF0000ULL) >> 16) | ((value & 0x0000FFFF0000FFFFULL) << 16);
        return (value >> 32) | (value << 32);
    }

    // Has single bit 

    /// @brief Checks if a value has only a single bit set
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to check
    /// @return `true` if the value has only a single bit set, `false` otherwise
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/has_single_bit
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsUnsigned<T>::Value, bool>::Type HasSingleBit(T value) __WSTL_NOEXCEPT__ {
        return (value & (value - 1)) == 0;
    }

    // Count left zero

    // 1-byte

    /// @brief Counts the number of leading zero bits in an unsigned integral value - for 1-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading zeros in
    /// @return The number of leading zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, uint_least8_t>::Type CountLeftZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 8;

        int count = 0;

        if((value & 0xF0U) == 0) { 
            count += 4; 
            value <<= 4; 
        }

        if((value & 0xC0U) == 0) { 
            count += 2; 
            value <<= 2; 
        }

        if((value & 0x80U) == 0) count += 1;

        return count;
    }

    // 2-byte

    /// @brief Counts the number of leading zero bits in an unsigned integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading zeros in
    /// @return The number of leading zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, uint_least8_t>::Type CountLeftZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 16;

        int count = 0;

        if((value & 0xFF00U) == 0) {
            count += 8;
            value <<= 8;
        }

        if((value & 0xF000U) == 0) { 
            count += 4; 
            value <<= 4; 
        }

        if((value & 0xC000U) == 0) { 
            count += 2; 
            value <<= 2; 
        }

        if((value & 0x8000U) == 0) count += 1;

        return count;
    }

    // 4-byte

    /// @brief Counts the number of leading zero bits in an unsigned integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading zeros in
    /// @return The number of leading zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, uint_least8_t>::Type CountLeftZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 32;

        int count = 0;

        if((value & 0xFFFF0000UL) == 0) {
            count += 16;
            value <<= 16;
        }

        if((value & 0xFF000000UL) == 0) {
            count += 8;
            value <<= 8;
        }

        if((value & 0xF0000000UL) == 0) { 
            count += 4; 
            value <<= 4; 
        }

        if((value & 0xC0000000UL) == 0) { 
            count += 2; 
            value <<= 2; 
        }

        if((value & 0x80000000UL) == 0) count += 1;

        return count;
    }

    // 8-byte

    /// @brief Counts the number of leading zero bits in an unsigned integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading zeros in
    /// @return The number of leading zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, uint_least8_t>::Type CountLeftZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 64;

        int count = 0;

        if((value & 0xFFFFFFFF00000000ULL) == 0) {
            count += 32;
            value <<= 32;
        }

        if((value & 0xFFFF000000000000ULL) == 0) {
            count += 16;
            value <<= 16;
        }

        if((value & 0xFF00000000000000ULL) == 0) {
            count += 8;
            value <<= 8;
        }

        if((value & 0xF000000000000000ULL) == 0) { 
            count += 4; 
            value <<= 4; 
        }

        if((value & 0xC000000000000000ULL) == 0) { 
            count += 2; 
            value <<= 2; 
        }

        if((value & 0x8000000000000000ULL) == 0) count += 1;

        return count;
    }

    // For signed types

    /// @brief Counts the number of leading zero bits in a signed integral value
    /// @tparam T Type of the value, must be an integral type
    /// @param value The value to count leading zeros in
    /// @return The number of leading zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, uint_least8_t>::Type CountLeftZero(T value) __WSTL_NOEXCEPT__ {
        typedef typename MakeUnsigned<T>::Type U;  
        return CountLeftZero<U>(static_cast<U>(value));
    }


    // Count right zero

    // 1-byte

    /// @brief Counts the number of trailing zero bits in an unsigned integral value - for 1-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing zeros in
    /// @return The number of trailing zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, uint_least8_t>::Type CountRightZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 8;

        int count = 0;

        if((value & 0xFU) == 0) { 
            count += 4; 
            value >>= 4; 
        }

        if((value & 0x3U) == 0) { 
            count += 2; 
            value >>= 2; 
        }

        if((value & 0x1U) == 0) count += 1;

        return count;
    }

    // 2-byte

    /// @brief Counts the number of trailing zero bits in an unsigned integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing zeros in
    /// @return The number of trailing zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, uint_least8_t>::Type CountRightZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 16;

        int count = 0;

        if((value & 0xFFU) == 0) { 
            count += 8; 
            value >>= 8; 
        }

        if((value & 0xFU) == 0) { 
            count += 4; 
            value >>= 4; 
        }

        if((value & 0x3U) == 0) { 
            count += 2; 
            value >>= 2; 
        }

        if((value & 0x1U) == 0) count += 1;

        return count;
    }

    // 4-byte

    /// @brief Counts the number of trailing zero bits in an unsigned integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing zeros in
    /// @return The number of trailing zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, uint_least8_t>::Type CountRightZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 32;

        int count = 0;

        if((value & 0xFFFFUL) == 0) { 
            count += 16; 
            value >>= 16; 
        }

        if((value & 0xFFUL) == 0) { 
            count += 8; 
            value >>= 8; 
        }

        if((value & 0xFUL) == 0) { 
            count += 4; 
            value >>= 4; 
        }

        if((value & 0x3UL) == 0) { 
            count += 2; 
            value >>= 2; 
        }

        if((value & 0x1UL) == 0) count += 1;

        return count;
    }

    // 8-byte

    /// @brief Counts the number of trailing zero bits in an unsigned integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing zeros in
    /// @return The number of trailing zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, uint_least8_t>::Type CountRightZero(T value) __WSTL_NOEXCEPT__ {
        if(value == 0) return 64;

        int count = 0;

        if((value & 0xFFFFFFFFULL) == 0) { 
            count += 32; 
            value >>= 32; 
        }

        if((value & 0xFFFFULL) == 0) { 
            count += 16; 
            value >>= 16; 
        }

        if((value & 0xFFULL) == 0) { 
            count += 8; 
            value >>= 8; 
        }

        if((value & 0xFULL) == 0) { 
            count += 4; 
            value >>= 4; 
        }

        if((value & 0x3ULL) == 0) { 
            count += 2; 
            value >>= 2; 
        }

        if((value & 0x1ULL) == 0) count += 1;

        return count;
    }

    // For signed types

    /// @brief Counts the number of trailing zero bits in a signed integral value
    /// @tparam T Type of the value, must be an integral type
    /// @param value The value to count trailing zeros in
    /// @return The number of trailing zero bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_zero
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, uint_least8_t>::Type CountRightZero(T value) __WSTL_NOEXCEPT__ {
        return CountRightZero(static_cast<typename MakeUnsigned<T>::Type>(value));
    }

    
    // Count left one

    // 1-byte

    /// @brief Counts the number of leading one bits in an unsigned integral value - for 1-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading ones in
    /// @return The number of leading one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, uint_least8_t>::Type CountLeftOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFU) return 8;

        int count = 0;

        if((value & 0xF0U) == 0xF0U) {
            count += 4;
            value <<= 4;
        }

        if((value & 0xC0U) == 0xC0U) {
            count += 2;
            value <<= 2;
        }

        if((value & 0x80U) == 0x80U) count += 1;

        return count;
    }

    // 2-byte

    /// @brief Counts the number of leading one bits in an unsigned integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading ones in
    /// @return The number of leading one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, uint_least8_t>::Type CountLeftOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFFFU) return 16;

        int count = 0;

        if((value & 0xFF00U) == 0xFF00U) {
            count += 8;
            value <<= 8;
        }

        if((value & 0xF000U) == 0xF000U) {
            count += 4;
            value <<= 4;
        }

        if((value & 0xC000U) == 0xC000U) {
            count += 2;
            value <<= 2;
        }

        if((value & 0x8000U) == 0x8000U) count += 1;

        return count;
    }

    // 4-byte

    /// @brief Counts the number of leading one bits in an unsigned integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading ones in
    /// @return The number of leading one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, uint_least8_t>::Type CountLeftOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFFFFFFFUL) return 32;

        int count = 0;

        if((value & 0xFFFF0000UL) == 0xFFFF0000UL) {
            count += 16;
            value <<= 16;
        }

        if((value & 0xFF000000UL) == 0xFF000000UL) {
            count += 8;
            value <<= 8;
        }

        if((value & 0xF0000000UL) == 0xF0000000UL) {
            count += 4;
            value <<= 4;
        }

        if((value & 0xC0000000UL) == 0xC0000000UL) {
            count += 2;
            value <<= 2;
        }

        if((value & 0x80000000UL) == 0x80000000UL) count += 1;

        return count;
    }

    // 8-byte

    /// @brief Counts the number of leading one bits in an unsigned integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count leading ones in
    /// @return The number of leading one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, uint_least8_t>::Type CountLeftOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFFFFFFFFFFFFFFFULL) return 64;

        int count = 0;

        if((value & 0xFFFFFFFF00000000ULL)) {
            count += 32;
            value <<= 32;
        }

        if((value & 0xFFFF000000000000ULL) == 0xFFFF000000000000ULL) {
            count += 16;
            value <<= 16;
        }

        if((value & 0xFF00000000000000ULL) == 0xFF00000000000000ULL) {
            count += 8;
            value <<= 8;
        }

        if((value & 0xF000000000000000ULL) == 0xF000000000000000ULL) {
            count += 4;
            value <<= 4;
        }

        if((value & 0xC000000000000000ULL) == 0xC000000000000000ULL) {
            count += 2;
            value <<= 2;
        }

        if((value & 0x8000000000000000ULL) == 0x8000000000000000ULL) count += 1;

        return count;
    }

    // For signed types

    /// @brief Counts the number of leading one bits in a signed integral value
    /// @tparam T Type of the value, must be an integral type
    /// @param value The value to count leading ones in
    /// @return The number of leading one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countl_one
    template<typename T>
    inline __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, uint_least8_t>::Type CountLeftOne(T value) __WSTL_NOEXCEPT__ {
        return CountLeftOne(static_cast<typename MakeUnsigned<T>::Type>(value));
    }

    
    // Count right one

    // 1-byte

    /// @brief Counts the number of trailing one bits in an unsigned integral value - for 1-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing ones in
    /// @return The number of trailing one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, uint_least8_t>::Type CountRightOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFU) return 8;

        int count = 0;

        if((value & 0xFU) == 0xFU) {
            count += 4;
            value >>= 4;
        }

        if((value & 0x3U) == 0x3U) {
            count += 2;
            value >>= 2;
        }

        if((value & 0x1U) == 0x1U) count += 1;

        return count;
    }

    // 2-byte

    /// @brief Counts the number of trailing one bits in an unsigned integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing ones in
    /// @return The number of trailing one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, uint_least8_t>::Type CountRightOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFFFU) return 16;

        int count = 0;

        if((value & 0xFFU) == 0xFFU) {
            count += 8;
            value >>= 8;
        }

        if((value & 0xFU) == 0xFU) {
            count += 4;
            value >>= 4;
        }

        if((value & 0x3U) == 0x3U) {
            count += 2;
            value >>= 2;
        }

        if((value & 0x1U) == 0x1U) count += 1;

        return count;
    }

    // 4-byte

    /// @brief Counts the number of trailing one bits in an unsigned integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing ones in
    /// @return The number of trailing one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, uint_least8_t>::Type CountRightOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFFFFFFFUL) return 32;

        int count = 0;

        if((value & 0xFFFFUL) == 0xFFFFUL) {
            count += 16;
            value >>= 16;
        }

        if((value & 0xFFUL) == 0xFFUL) {
            count += 8;
            value >>= 8;
        }

        if((value & 0xFUL) == 0xFUL) {
            count += 4;
            value >>= 4;
        }

        if((value & 0x3UL) == 0x3UL) {
            count += 2;
            value >>= 2;
        }

        if((value & 0x1UL) == 0x1UL) count += 1;

        return count;
    }

    // 8-byte

    /// @brief Counts the number of trailing one bits in an unsigned integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count trailing ones in
    /// @return The number of trailing one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, uint_least8_t>::Type CountRightOne(T value) __WSTL_NOEXCEPT__ {
        if(value == 0xFFFFFFFFFFFFFFFFUL) return 32;

        int count = 0;

        if((value & 0xFFFFFFFFUL) == 0xFFFFFFFFUL) {
            count += 32;
            value >>= 32;
        }

        if((value & 0xFFFFUL) == 0xFFFFUL) {
            count += 16;
            value >>= 16;
        }

        if((value & 0xFFUL) == 0xFFUL) {
            count += 8;
            value >>= 8;
        }

        if((value & 0xFUL) == 0xFUL) {
            count += 4;
            value >>= 4;
        }

        if((value & 0x3UL) == 0x3UL) {
            count += 2;
            value >>= 2;
        }

        if((value & 0x1UL) == 0x1UL) count += 1;

        return count;
    }

    // For signed types

    /// @brief Counts the number of trailing one bits in a signed integral value
    /// @tparam T Type of the value, must be an integral type
    /// @param value The value to count trailing ones in
    /// @return The number of trailing one bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/countr_one
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, uint_least8_t>::Type CountRightOne(T value) __WSTL_NOEXCEPT__ {
        return CountRightOne(static_cast<typename MakeUnsigned<T>::Type>(value));
    }


    // Bit width

    /// @brief Finds the number of bits required to represent an unsigned integral value
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to analyze
    /// @return The number of bits required to represent the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/bit_width
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsUnsigned<T>::Value, uint_least8_t>::Type BitWidth(T value) __WSTL_NOEXCEPT__ {
        return NumericLimits<T>::Digits - CountLeftZero(value);
    }

    // Bit floor

    /// @brief Finds the largest power of two that is less than or equal to the given unsigned integral value
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to analyze
    /// @return The largest power of two that is less than or equal to the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/bit_floor
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsUnsigned<T>::Value, T>::Type BitFloor(T value) __WSTL_NOEXCEPT__ {
        return (value == T(0)) ? T(0) : T(1) << (BitWidth(value) - T(1));
    }

    // Bit ceil

    /// @brief Finds the smallest power of two that is greater than or equal to the given unsigned integral value
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to analyze
    /// @return The smallest power of two that is greater than or equal to the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/bit_ceil
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsUnsigned<T>::Value, T>::Type BitCeil(T value) __WSTL_NOEXCEPT__ {
        return (value == T(0)) ? T(1) : T(1) << BitWidth(T(value - T(1)));
    }

    // Rotate left

    /// @brief Rotates the bits of an unsigned integral value to the left by a specified number of positions
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to rotate
    /// @param shift The number of positions to rotate
    /// @return The value with its bits rotated to the left
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/rotl
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsUnsigned<T>::Value, T>::Type RotateLeft(T value, int shift) __WSTL_NOEXCEPT__ {
        const __WSTL_CONSTEXPR14__ int n = NumericLimits<T>::Digits;
        const int r = ((shift % n) + n) % n;
        return (value << r) | (value >> (n - r));
    }

    // Rotate right

    /// @brief Rotates the bits of an unsigned integral value to the right by a specified number of positions
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to rotate
    /// @param shift The number of positions to rotate
    /// @return The value with its bits rotated to the right
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/rotr
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsUnsigned<T>::Value, T>::Type RotateRight(T value, int shift) __WSTL_NOEXCEPT__ {
        const int __WSTL_CONSTEXPR14__ n = NumericLimits<T>::Digits;
        const int r = ((shift % n) + n) % n;
        return (value >> r) | (value << (n - r));
    }

    // Population count

    // 1-byte

    /// @brief Counts the number of set bits (population count) in an unsigned integral value - for 1-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count set bits in
    /// @return The number of set bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/popcount
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, uint_least8_t>::Type PopulationCount(T value) __WSTL_NOEXCEPT__ {
        value -= ((value >> 1) & 0x55);
        value = (value & 0x33) + ((value >> 2) & 0x33);
        value = (value + (value >> 4)) & 0x0F;
        return static_cast<uint_least8_t>(value); 
    }

    // 2-byte

    /// @brief Counts the number of set bits (population count) in an unsigned integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count set bits in
    /// @return The number of set bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/popcount
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, uint_least8_t>::Type PopulationCount(T value) __WSTL_NOEXCEPT__ {
        value -= ((value >> 1) & 0x5555U);
        value = (value & 0x3333U) + ((value >> 2) & 0x3333U);
        value = (value + (value >> 4)) & 0x0F0FU;
        value = (value + (value >> 8)) & 0x00FFU;
        return static_cast<uint_least8_t>(value);
    }

    // 4-byte

    /// @brief Counts the number of set bits (population count) in an unsigned integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count set bits in
    /// @return The number of set bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/popcount
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, uint_least8_t>::Type PopulationCount(T value) __WSTL_NOEXCEPT__ {
        value -= ((value >> 1) & 0x55555555U);
        value = (value & 0x33333333U) + ((value >> 2) & 0x33333333U);
        value = (value + (value >> 4)) & 0x0F0F0F0FU;
        return static_cast<uint_least8_t>((value * 0x01010101U) >> 24);
    }

    // 8-byte

    /// @brief Counts the number of set bits (population count) in an unsigned integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to count set bits in
    /// @return The number of set bits in the value
    /// @ingroup binary
    /// @see https://en.cppreference.com/w/cpp/numeric/popcount
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, uint_least8_t>::Type PopulationCount(T value) __WSTL_NOEXCEPT__ {
        value -= (value >> 1) & 0x5555555555555555ULL;
        value = (value & 0x3333333333333333ULL) + ((value >> 2) & 0x3333333333333333ULL);
        value = (value + (value >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
        value *= 0x0101010101010101ULL;
        return static_cast<uint_least8_t>(value >> 56);
    }

    // Parity

    // 1-byte

    /// @brief Computes the parity of an unsigned integral value - for 1-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to compute parity for
    /// @return The parity of the value (0 for even number of 1 bits, 1 for odd number of 1 bits)
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 1, uint_least8_t>::Type Parity(T value) __WSTL_NOEXCEPT__ {
        value ^= value >> 4U;
        value &= 0x0FU;
        return (0x6996U >> value) & 1U;
    }

    // 2-byte

    /// @brief Computes the parity of an unsigned integral value - for 2-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to compute parity for
    /// @return The parity of the value (0 for even number of 1 bits, 1 for odd number of 1 bits)
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 2, uint_least8_t>::Type Parity(T value) __WSTL_NOEXCEPT__ {
        value ^= value >> 8U;
        value ^= value >> 4U;
        value &= 0x0FU;
        return (0x6996U >> value) & 1U;
    }

    // 4-byte

    /// @brief Computes the parity of an unsigned integral value - for 4-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to compute parity for
    /// @return The parity of the value (0 for even number of 1 bits, 1 for odd number of 1 bits)
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 4, uint_least8_t>::Type Parity(T value) __WSTL_NOEXCEPT__ {
        value ^= value >> 16U;
        value ^= value >> 8U;
        value ^= value >> 4U;
        value &= 0x0FU;
        return (0x6996U >> value) & 1U;
    }

    // 8-byte

    /// @brief Computes the parity of an unsigned integral value - for 8-byte types
    /// @tparam T Type of the value, must be an unsigned integral type
    /// @param value The value to compute parity for
    /// @return The parity of the value (0 for even number of 1 bits, 1 for odd number of 1 bits)
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value && sizeof(T) == 8, uint_least8_t>::Type Parity(T value) __WSTL_NOEXCEPT__ {
        value ^= value >> 32U;
        value ^= value >> 16U;
        value ^= value >> 8U;
        value ^= value >> 4U;
        value &= 0x0FU;
        return (0x69966996UL >> value) & 1U;
    }

    // For signed types

    /// @brief Computes the parity of a signed integral value
    /// @tparam T Type of the value, must be an integral signed type
    /// @param value The value to compute parity for
    /// @return The parity of the value (0 for even number of 1 bits, 1 for odd number of 1 bits)
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, uint_least8_t>::Type Parity(T value) __WSTL_NOEXCEPT__ {
        return Parity(static_cast<typename MakeUnsigned<T>::Type>(value));
    }

    // Set flag

    /// @brief Sets a specific flag in an object
    /// @param object The object to modify
    /// @param flag The flag to set
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ inline void SetFlag(T& object, T flag) __WSTL_NOEXCEPT__ {
        object |= flag;
    }

    // Unset flag

    /// @brief Unsets a specific flag in an object
    /// @param object The object to modify
    /// @param flag The flag to unset
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ inline void UnsetFlag(T& object, T flag) __WSTL_NOEXCEPT__ {
        object &= ~flag;
    }

    // Test flag

    /// @brief Tests if a specific flag is set in an object
    /// @param object The object to check
    /// @param flag The flag to test
    /// @return True if the flag is set, false otherwise
    /// @ingroup binary
    template<typename T>
    __WSTL_CONSTEXPR14__ inline bool TestFlag(const T& object, T flag) __WSTL_NOEXCEPT__ {
        return (object & flag) != 0;
    }
}

#endif