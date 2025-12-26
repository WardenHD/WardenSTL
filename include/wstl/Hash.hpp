// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s hash.h
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been modified and improved by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_HASH_HPP__
#define __WSTL_HASH_HPP__

#include "private/Platform.hpp"
#include "Math.hpp"

#include "hash/FNV1.hpp"
#include "hash/Jenkins.hpp"
#include "hash/Pearson.hpp"
#include "hash/Murmur3.hpp"


namespace wstl {
    namespace __private {
        template<typename T>
        __WSTL_CONSTEXPR14__ typename EnableIf<sizeof(T) == sizeof(uint16_t), size_t>::Type __GenericHash(const uint8_t* first, const uint8_t* last) {
            uint32_t hash = hash::FNV1a_32(first, last);
            return static_cast<size_t>(hash ^ (hash >> 16));
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ typename EnableIf<sizeof(T) == sizeof(uint32_t), size_t>::Type __GenericHash(const uint8_t* first, const uint8_t* last) {
            return hash::FNV1a_32(first, last);
        }
        
        template<typename T>
        __WSTL_CONSTEXPR14__ typename EnableIf<sizeof(T) == sizeof(uint64_t), size_t>::Type __GenericHash(const uint8_t* first, const uint8_t* last) {
            return hash::FNV1a_64(first, last);
        }

        template<typename T, bool IsEnum = false>
        struct __HashBase {
        private:
            __HashBase();
            __HashBase(const __HashBase&);
            __HashBase& operator=(const __HashBase&);

            #ifdef __WSTL_CXX11__
            __HashBase(__HashBase&&);
            __HashBase& operator=(__HashBase&&);
            #endif
        };
    }

    /// @brief Generic hash function for various types
    /// @tparam T The type to be hashed
    /// @ingroup hash
    /// @see https://en.cppreference.com/w/cpp/utility/hash.html
    template<typename T>
    struct Hash;

    template<typename T>
    struct Hash : __private::__HashBase<T, IsEnum<T>::Value> {};

    // Specializations of hash for 1-byte trivial types

    #define __WSTL_HASH_TRIVIAL_1BYTE__(T) \
    template<> \
    struct Hash<T> { \
        StaticAssert(sizeof(size_t) >= sizeof(T), "size_t is smaller than hashed type"); \
        \
        __WSTL_CONSTEXPR14__ size_t operator()(T value) const { \
            return static_cast<size_t>(value); \
        } \
    };

    __WSTL_HASH_TRIVIAL_1BYTE__(bool)
    __WSTL_HASH_TRIVIAL_1BYTE__(char)
    __WSTL_HASH_TRIVIAL_1BYTE__(unsigned char)
    __WSTL_HASH_TRIVIAL_1BYTE__(signed char)

    // Specializations of hash for larger trivial types

    #define __WSTL_HASH_TRIVIAL_LARGER__(T) \
    template<> \
    struct Hash<T> { \
        __WSTL_CONSTEXPR14__ size_t operator()(T value) const { \
            if __WSTL_IF_CONSTEXPR__(sizeof(size_t) >= sizeof(wchar_t)) return static_cast<size_t>(value); \
            else { \
                uint8_t* p = reinterpret_cast<uint8_t*>(&value); \
                return __private::__GenericHash<size_t>(p, p + sizeof(value)); \
            } \
        } \
    };

    __WSTL_HASH_TRIVIAL_LARGER__(wchar_t)
    __WSTL_HASH_TRIVIAL_LARGER__(short)
    __WSTL_HASH_TRIVIAL_LARGER__(unsigned short)
    __WSTL_HASH_TRIVIAL_LARGER__(int)
    __WSTL_HASH_TRIVIAL_LARGER__(unsigned int)
    __WSTL_HASH_TRIVIAL_LARGER__(long)
    __WSTL_HASH_TRIVIAL_LARGER__(unsigned long)
    __WSTL_HASH_TRIVIAL_LARGER__(long long)
    __WSTL_HASH_TRIVIAL_LARGER__(unsigned long long)

    // Specializations of hash for floating-point types

    template<>
    struct Hash<float> {
        size_t operator()(float value) const {
            if __WSTL_IF_CONSTEXPR__(sizeof(size_t) == sizeof(float)) {
                union {
                    size_t Result;
                    float Value;
                } u;

                if(IsZero(value)) value = 0.0F;
                u.Value = value;

                return u.Result;
            }
            else {
                uint8_t* p = reinterpret_cast<uint8_t*>(&value);
                return __private::__GenericHash<size_t>(p, p + sizeof(value));
            }
        }
    };

    template<>
    struct Hash<double> {
        size_t operator()(double value) const {
            if __WSTL_IF_CONSTEXPR__(sizeof(size_t) == sizeof(double)) {
                union {
                    size_t Result;
                    double Value;
                } u;

                if(IsZero(value)) value = 0.0;
                u.Value = value;

                return u.Result;
            }
            else {
                uint8_t* p = reinterpret_cast<uint8_t*>(&value);
                return __private::__GenericHash<size_t>(p, p + sizeof(value));
            }
        }
    };

    template<>
    struct Hash<long double> {
        size_t operator()(float value) const {
            if __WSTL_IF_CONSTEXPR__(sizeof(size_t) == sizeof(long double)) {
                union {
                    size_t Result;
                    long double Value;
                } u;

                if(IsZero(value)) value = 0.0L;
                u.Value = value;

                return u.Result;
            }
            else {
                uint8_t* p = reinterpret_cast<uint8_t*>(&value);
                return __private::__GenericHash<size_t>(p, p + sizeof(value));
            }
        }
    };

    // Specialization for pointers

    template<typename T>
    struct Hash<T*> {
        size_t operator()(T* value) const {
            if __WSTL_IF_CONSTEXPR__(sizeof(size_t) == sizeof(T*)) {
                union {
                    size_t Result;
                    const T* Value;
                } u;

                u.Value = value;

                return u.Result;
            }
            else {
                uint8_t* p = reinterpret_cast<uint8_t*>(&value);
                return __private::__GenericHash<size_t>(p, p + sizeof(value));
            }
        }
    };

    // Specialization for enums

    namespace __private {
        template<typename T>
        struct __HashBase<T, true> {
            __WSTL_CONSTEXPR14__ size_t operator()(T value) const {
                if __WSTL_IF_CONSTEXPR__(sizeof(size_t) >= sizeof(T)) return static_cast<size_t>(value);
                else return Hash<unsigned long long>()(static_cast<unsigned long long>(value));
            }
        };
    }
}

#endif