#ifndef __WSTL_FNV1_HPP__
#define __WSTL_FNV1_HPP__

#include "../Platform.hpp"
#include "../../HasherBase.hpp"
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

    // FNV1

    template<typename T>
    class FNV1 : public HasherBase<FNV1<T>, T, uint8_t> {
    private:
        typedef __private::__FNVConstants<T> Constants;
        typedef HasherBase<FNV1<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        StaticAssert((IsSame<T, uint32_t>::Value || IsSame<T, uint64_t>::Value), "Only 32 and 64-bit types are supported");

        __WSTL_CONSTEXPR14__ FNV1() : Base() {}

        template<typename Iterator>
        __WSTL_CONSTEXPR14__ FNV1(Iterator first, Iterator last) : Base(first, last) {}

        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = Constants::OFFSET_BASIS;
        }

        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash *= Constants::PRIME;
            this->m_Hash ^= value;
        }

    private:
        friend Base;

        __WSTL_CONSTEXPR14__ void Finalize() {}
    };

    typedef FNV1<uint32_t> FNV1_32;
    typedef FNV1<uint64_t> FNV1_64;

    // FNV1a

    template<typename T>
    class FNV1a : public HasherBase<FNV1a<T>, T, uint8_t> {
    private:
        typedef __private::__FNVConstants<T> Constants;
        typedef HasherBase<FNV1a<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        StaticAssert((IsSame<T, uint32_t>::Value || IsSame<T, uint64_t>::Value), "Only 32 and 64-bit types are supported");

        __WSTL_CONSTEXPR14__ FNV1a() : Base() {}

        template<typename Iterator>
        __WSTL_CONSTEXPR14__ FNV1a(Iterator first, Iterator last) : Base(first, last) {}

        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = Constants::OFFSET_BASIS;
        }

        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash ^= value;
            this->m_Hash *= Constants::PRIME;
        }

    private:
        friend Base;

        __WSTL_CONSTEXPR14__ void Finalize() {}
    };

    typedef FNV1a<uint32_t> FNV1a_32;
    typedef FNV1a<uint64_t> FNV1a_64;
}

#endif