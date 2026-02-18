// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s bitset.h,
// but it has been re-implemented with custom logic for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_BITSET_HPP__
#define __WSTL_BITSET_HPP__

#include "private/Platform.hpp"
#include "Limits.hpp"
#include "Span.hpp"
#include "BasicString.hpp"
#include "String.hpp"

#include <stddef.h>


/// @defgroup bitset Bitset
/// @ingroup containers
/// @brief A fixed-size sequence of bits

// Defines introduced

/// @def __WSTL_BITSET_USE_CTZ___
/// @brief Define to enable use of `CountRightZero` function in bitset find functions
/// @details This define best works with `__WSTL_BIT_USE_BUILTIN__` defined. 
/// These two defines improve the performance of find functions at the cost of constexpr-ness.
/// @ingroup bitset
#ifdef __DOXYGEN__
    #define __WSTL_BITSET_USE_CTZ___ 
#endif

namespace wstl {
    // Bitset internal classes

    namespace __private {
        template<size_t N, typename T>
        class __BitsetCommon {
        public:
            typedef size_t SizeType;
            typedef T ElementType;
            typedef T* PointerType;
            typedef const T* ConstPointerType;

            /// @brief Special constant indicating no position with value `SizeType(-1)`
            static const __WSTL_CONSTEXPR__ SizeType NoPosition = NumericLimits<SizeType>::Max();

        protected:
            static const __WSTL_CONSTEXPR__ SizeType BitsPerElement = sizeof(ElementType) * 8;
            static const __WSTL_CONSTEXPR__ ElementType AllSet = NumericLimits<ElementType>::Max();
            static const __WSTL_CONSTEXPR__ ElementType AllClear = ElementType(0);
            static const __WSTL_CONSTEXPR__ SizeType NumberOfElements = (N % BitsPerElement == 0) ? N / BitsPerElement : N / BitsPerElement + 1; 
            static const __WSTL_CONSTEXPR__ SizeType TopBits = N % BitsPerElement;
            static const __WSTL_CONSTEXPR__ ElementType TopMask = ElementType((TopBits == 0) ? AllSet : ~(AllSet << TopBits));
            static const __WSTL_CONSTEXPR__ SizeType ActualSize = NumberOfElements * BitsPerElement;
        
        public:
            typedef Span<T, NumberOfElements> SpanType;
            typedef Span<const T, NumberOfElements> ConstSpanType;    
        };

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::NoPosition;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::BitsPerElement;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::ElementType __BitsetCommon<N, T>::AllSet;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::ElementType __BitsetCommon<N, T>::AllClear;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::NumberOfElements;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::TopBits;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::ElementType __BitsetCommon<N, T>::TopMask;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ typename __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::ActualSize;

        template<size_t N, typename T, size_t Elements>
        class __Bitset;

        template<size_t N, typename T>
        class __Bitset<N, T, 1> : public __BitsetCommon<N, T> {
        private:
            typedef __BitsetCommon<N, T> Base;

        public:
            typedef typename Base::SizeType SizeType;
            typedef typename Base::ElementType ElementType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;
            typedef typename Base::SpanType SpanType;
            typedef typename Base::ConstSpanType ConstSpanType;

            using Base::NoPosition;

        protected:
            using Base::BitsPerElement;
            using Base::AllSet;
            using Base::AllClear;
            using Base::NumberOfElements;
            using Base::TopBits;
            using Base::TopMask;
            using Base::ActualSize;

            static __WSTL_CONSTEXPR14__ bool Test(ConstPointerType bits, SizeType position) {
                return (*bits & (ElementType(1) << position)) != 0;
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ bool Test(ConstPointerType bits) {
                return (*bits & (ElementType(1) << Position)) != 0U;
            }

            static __WSTL_CONSTEXPR14__ bool All(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                return (*bits & TopMask) == AllSet;
            }

            static __WSTL_CONSTEXPR14__ bool Any(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                return *bits != AllClear;
            }

            static __WSTL_CONSTEXPR14__ bool None(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                return *bits == AllClear;
            }

            static __WSTL_CONSTEXPR14__ SizeType Count(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                return PopulationCount(*bits);
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) __WSTL_NOEXCEPT__ {
                *bits = (AllSet & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, SizeType position) {
                *bits |= (ElementType(1) << position);
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) {
                *bits |= (ElementType(1) << Position);
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, SizeType position, bool value) {
                if(value) *bits |= (ElementType(1) << position);
                *bits &= ~(ElementType(1) << position);
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, bool value) {
                if(value) *bits |= (ElementType(1) << Position);
                *bits &= ~(ElementType(1) << Position);
            }

            template<size_t Position, bool Value>
            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) {
                if(Value) *bits |= (ElementType(1) << Position);
                *bits &= ~(ElementType(1) << Position);
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits) __WSTL_NOEXCEPT__ {
                *bits = AllClear;
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits, SizeType position) {
                *bits &= ~(ElementType(1) << position);
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits) {
                *bits &= ~(ElementType(1) << Position);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits) __WSTL_NOEXCEPT__ {
                *bits = (~*bits & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits, SizeType position) {
                *bits ^= (ElementType(1) << position);
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits) {
                *bits ^= (ElementType(1) << Position);
            }

            static __WSTL_CONSTEXPR14__ bool OperatorEquality(ConstPointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                return *bits1 == *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorAND(PointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                *bits1 &= *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorOR(PointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                *bits1 |= *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorXOR(PointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                *bits1 ^= *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftLeft(PointerType bits, SizeType shift) __WSTL_NOEXCEPT__ {
                if(shift >= N) Reset(bits);
                *bits = (*bits << shift) & TopMask;
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftRight(PointerType bits, SizeType shift) __WSTL_NOEXCEPT__ {
                if(shift >= N) Reset(bits);
                *bits = (*bits >> shift) & TopMask;
            }

            static __WSTL_CONSTEXPR14__ void Initialize(PointerType bits, unsigned long long value) __WSTL_NOEXCEPT__ {
                *bits = static_cast<ElementType>(value);
            }

            template<typename TChar>
            static __WSTL_CONSTEXPR14__ void FromString(PointerType bits, const TChar* string, size_t count, TChar one) __WSTL_NOEXCEPT__ {
                Reset(bits);

                if(string != NullPointer) {
                    const SizeType start = count - Min(N, count);
                    ElementType value = 0;
                    
                    for(SizeType i = start; i < count; ++i) {
                        value <<= 1;
                        if(string[i] == one) value |= 1;
                    }
                        
                    *bits = value;
                }
            }

            static __WSTL_CONSTEXPR14__ void OperatorAssignment(PointerType bits, ConstPointerType bitsOther) __WSTL_NOEXCEPT__ {
                *bits = *bitsOther;
            }

            template<typename TString>
            static __WSTL_CONSTEXPR14__ TString ToString(ConstPointerType bits, typename TString::ValueType zero, typename TString::ValueType one) {
                TString result;

                result.Resize(N);

                __WSTL_ASSERT_RETURNVALUE__(result.Size() == N, WSTL_MAKE_EXCEPTION(LengthError, "Bitset string size is too small"), result);

                for(SizeType i = N; i > 0; --i) result[N - i] = Test(bits, i - 1) ? one : zero;

                return result;
            }

            template<typename U>
            static __WSTL_CONSTEXPR14__ U Value(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                return U(*bits);
            }

            static __WSTL_CONSTEXPR14__ SizeType FindNext(ConstPointerType bits, SizeType position, bool value) __WSTL_NOEXCEPT__ {
                #ifdef __WSTL_BITSET_USE_CTZ___
                if(position < N) {
                    ElementType block = (value) ? *bits : ~(*bits);
                    block &= ElementType(~0 << position);

                    if(block != 0) return CountRightZero(block); 
                }
                #else
                if((value && *bits != AllClear) || (!value && *bits != AllSet)) {
                    ElementType mask = ElementType(1) << position;

                    while(position < N) {
                        if(((*bits & mask) != 0) == value) return position;
                        mask <<= 1;
                        ++position;
                    }
                }
                #endif

                return NoPosition;
            }

            static __WSTL_CONSTEXPR14__ void Swap(PointerType bits1, PointerType bits2) __WSTL_NOEXCEPT__ {
                wstl::Swap(*bits1, *bits2);
            }
        };

        template<size_t N, typename T, size_t Elements>
        class __Bitset : public __BitsetCommon<N, T> {
        private:
            typedef __BitsetCommon<N, T> Base;

        public:
            typedef typename Base::SizeType SizeType;
            typedef typename Base::ElementType ElementType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;
            typedef typename Base::SpanType SpanType;
            typedef typename Base::ConstSpanType ConstSpanType;
            
            using Base::NoPosition;

        protected:
            using Base::BitsPerElement;
            using Base::AllSet;
            using Base::AllClear;
            using Base::NumberOfElements;
            using Base::TopBits;
            using Base::TopMask;
            using Base::ActualSize;

            static __WSTL_CONSTEXPR14__ bool Test(ConstPointerType bits, SizeType position) __WSTL_NOEXCEPT__ {
                const SizeType index = position >> Logarithm2<BitsPerElement>::Value; // same as (position / BitsPerElement) but more efficient
                const ElementType mask = ElementType(1) << (position & (BitsPerElement - 1));

                return (bits[index] & mask) != 0;
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ bool Test(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                const SizeType index = Position >> Logarithm2<BitsPerElement>::Value; // same as (Position / BitsPerElement) but more efficient
                const ElementType mask = ElementType(1) << (Position & (BitsPerElement - 1));

                return (bits[index] & mask) != 0;
            }

            static __WSTL_CONSTEXPR14__ bool All(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                for(SizeType i = 1; i < NumberOfElements; ++i)
                    if(*bits++ != AllSet) return false;

                return (*bits & TopMask) == AllSet;
            }

            static __WSTL_CONSTEXPR14__ bool Any(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements; ++i)
                    if(*bits++ != AllClear) return true;

                return false;
            }

            static __WSTL_CONSTEXPR14__ bool None(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements; ++i)
                    if(*bits++ != AllClear) return false;

                return true;
            }

            static __WSTL_CONSTEXPR14__ SizeType Count(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                SizeType count = 0;

                for(SizeType i = 0; i < NumberOfElements; ++i) count += PopulationCount(*bits++);

                return count;
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) __WSTL_NOEXCEPT__ {
                for(SizeType i = 1; i < NumberOfElements; ++i) *bits++ = AllSet;
                *bits = (AllSet & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, SizeType position) __WSTL_NOEXCEPT__ {
                const SizeType index = position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (position & (BitsPerElement - 1));

                bits[index] |= bit;
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) __WSTL_NOEXCEPT__ {
                const SizeType index = Position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (Position & (BitsPerElement - 1));

                bits[index] |= bit;
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, SizeType position, bool value) {
                const SizeType index = position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (position & (BitsPerElement - 1));

                if(value) bits[index] |= bit;
                else bits[index] &= ~bit;
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, bool value) {
                const SizeType index = Position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (Position & (BitsPerElement - 1));

                if(value) bits[index] |= bit;
                else bits[index] &= ~bit;
            }

            template<size_t Position, bool Value>
            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) {
                const SizeType index = Position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (Position & (BitsPerElement - 1));

                if(Value) bits[index] |= bit;
                else bits[index] &= ~bit;
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements; ++i) *bits++ = AllClear;
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits, SizeType position) __WSTL_NOEXCEPT__ {
                const SizeType index = position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (position & (BitsPerElement - 1));

                bits[index] &= ~bit;
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits) __WSTL_NOEXCEPT__ {
                const SizeType index = Position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (Position & (BitsPerElement - 1));

                bits[index] &= ~bit;
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements - 1; ++i) *bits++ = ~*bits;
                *bits = (~*bits & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits, SizeType position) __WSTL_NOEXCEPT__ {
                const SizeType index = position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (position & (BitsPerElement - 1));

                bits[index] ^= bit;
            }

            template<size_t Position>
            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits) __WSTL_NOEXCEPT__ {
                const SizeType index = Position >> Logarithm2<BitsPerElement>::Value;
                const ElementType bit = ElementType(1) << (Position & (BitsPerElement - 1));

                bits[index] ^= bit;
            }

            static __WSTL_CONSTEXPR14__ bool OperatorEquality(ConstPointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                return Equal(bits1, bits1 + NumberOfElements, bits2);
            }

            static __WSTL_CONSTEXPR14__ void OperatorAND(PointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements; ++i) *bits1++ &= *bits2++;
            }

            static __WSTL_CONSTEXPR14__ void OperatorOR(PointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements; ++i) *bits1++ |= *bits2++;
            }

            static __WSTL_CONSTEXPR14__ void OperatorXOR(PointerType bits1, ConstPointerType bits2) __WSTL_NOEXCEPT__ {
                for(SizeType i = 0; i < NumberOfElements; ++i) *bits1++ ^= *bits2++;
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftLeft(PointerType bits, SizeType shift) __WSTL_NOEXCEPT__ {
                if(shift >= N) Reset(bits);
                else if(shift != 0) {
                    const SizeType bitShift = shift & (BitsPerElement - 1); // same as shift % BitsPerElement
                    const SizeType blockShift = shift >> Logarithm2<BitsPerElement>::Value;

                    if(bitShift == 0) {
                        // Perform full block shift
                        for(SizeType i = NumberOfElements - 1; i >= blockShift; --i)
                            bits[i] = bits[i - blockShift];
                    }
                    else {
                        // Perform full + partial shift
                        for(SizeType i = NumberOfElements - 1; i > blockShift; --i) {
                            const ElementType low = bits[i - blockShift] << bitShift;
                            const ElementType high = bits[i - blockShift - 1] >> (BitsPerElement - bitShift);

                            bits[i] = low | high;
                        }

                        bits[blockShift] = bits[0] << bitShift;
                    }

                    // Zero out last blocks
                    FillInRange(bits, blockShift, AllClear);

                    // Apply mask to the last block
                    bits[NumberOfElements - 1] &= TopMask;
                }
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftRight(PointerType bits, SizeType shift) __WSTL_NOEXCEPT__ {
                if(shift >= N) Reset(bits);
                else if(shift != 0) {
                    const SizeType bitShift = shift & (BitsPerElement - 1);
                    const SizeType blockShift = shift >> Logarithm2<BitsPerElement>::Value;

                    if(bitShift == 0) {
                        // Perform full block shift
                        for(SizeType i = 0; i < NumberOfElements - blockShift; ++i)
                            bits[i] = bits[i + blockShift];
                    }
                    else {
                        // Perform full + partial shift
                        for(SizeType i = 0; i < NumberOfElements - blockShift - 1; ++i) {
                            const ElementType low = bits[i + blockShift] >> bitShift;
                            const ElementType high = bits[i + blockShift + 1] << (BitsPerElement - bitShift);

                            bits[i] = low | high;
                        }

                        bits[NumberOfElements - blockShift - 1] = bits[NumberOfElements - 1] >> bitShift;
                    }

                    // Zero out last blocks
                    Fill(bits + (NumberOfElements - blockShift), bits + NumberOfElements, AllClear);
                }
            }

            template<typename U>
            static __WSTL_CONSTEXPR14__
            typename EnableIf<BitsPerElement == (sizeof(U) * 8), void>::Type Initialize(PointerType bits, U value) __WSTL_NOEXCEPT__ {
                *bits++ = value;
                FillInRange(bits, NumberOfElements - 1, 0);
            }

            template<typename U>
            static __WSTL_CONSTEXPR14__
            typename EnableIf<BitsPerElement != (sizeof(U) * 8), void>::Type Initialize(PointerType bits, U value) __WSTL_NOEXCEPT__ {
                SizeType i = 0;

                while((value != 0) && (i != NumberOfElements)) {
                    bits[i++] = value & AllSet;
                    value >>= BitsPerElement;
                }

                while(i != NumberOfElements) bits[i++] = 0;

                bits[NumberOfElements - 1] &= TopMask;
            }

            template<typename TChar>
            static __WSTL_CONSTEXPR14__ void FromString(PointerType bits, const TChar* string, size_t count, TChar one) __WSTL_NOEXCEPT__ {
                Reset(bits);

                if(string != NullPointer) {
                    const int useful = Min(N, count);
                    const int start = count - useful;

                    const SizeType iterCount = (useful + BitsPerElement - 1) >> Logarithm2<BitsPerElement>::Value;

                    int pos = count - 1;

                    for(SizeType i = 0; i < iterCount; ++i) {
                        ElementType value = 0;
                        ElementType mask = 1;
                    
                        for(SizeType j = 0; j < BitsPerElement && pos >= start; ++j) {
                            if(string[pos] == one) value |= mask;
                            --pos;
                            mask <<= 1;
                        }

                        bits[i] = value;
                    }
                }
            }

            static __WSTL_CONSTEXPR14__ void OperatorAssignment(PointerType bits, ConstPointerType bitsOther) __WSTL_NOEXCEPT__ {
                CopyInRange(bitsOther, NumberOfElements, bits);
            }

            template<typename TString>
            static __WSTL_CONSTEXPR14__ TString ToString(ConstPointerType bits, typename TString::ValueType zero, typename TString::ValueType one) {
                TString result;

                result.Resize(N);

                __WSTL_ASSERT_RETURNVALUE__(result.Size() == N, WSTL_MAKE_EXCEPTION(LengthError, "Bitset string size is too small"), result);

                for(SizeType i = N; i > 0; --i) result[N - i] = Test(bits, i - 1) ? one : zero;

                return result;
            }

            template<typename U>
            static __WSTL_CONSTEXPR14__ U Value(ConstPointerType bits) __WSTL_NOEXCEPT__ {
                U result = U(0);
                uint_least8_t shift = 0;

                for(SizeType i = 0; i < NumberOfElements; ++i) {
                    result |= U(typename MakeUnsigned<U>::Type(bits[i]) << shift);
                    shift += uint_least8_t(BitsPerElement);
                }

                return result;
            }

            static __WSTL_CONSTEXPR14__ SizeType FindNext(ConstPointerType bits, SizeType position, bool value) __WSTL_NOEXCEPT__ {
                #ifdef __WSTL_BITSET_USE_CTZ___
                if(position < N) {
                    const SizeType bitPosition = position & (BitsPerElement - 1);
                    SizeType blockPosition = position >> Logarithm2<BitsPerElement>::Value;

                    const ElementType invertMask = (value) ? ElementType(0) : ElementType(~0);

                    ElementType block = bits[blockPosition] ^ invertMask;
                    block &= ElementType(~0 << bitPosition);

                    while(blockPosition < NumberOfElements) {
                        if(block != 0) {
                            uint_least8_t ctz = CountRightZero(block);
                            return (blockPosition << Logarithm2<BitsPerElement>::Value) + ctz;
                        }

                        block = bits[++blockPosition] ^ invertMask;
                    }
                }
                #else
                if(position < N) {
                    SizeType bitPosition = position & (BitsPerElement - 1);
                    SizeType blockPosition = position >> Logarithm2<BitsPerElement>::Value;

                    ElementType mask = ElementType(1) << bitPosition;

                    while(blockPosition < NumberOfElements) {
                        ElementType block = bits[blockPosition++];

                        if((value && block != AllClear) || (!value && block != AllSet)) {
                            while(bitPosition++ < BitsPerElement && position < N) {
                                if(((block & mask) != 0) == value) return position;
                                mask <<= 1;
                                ++position;
                            }
                        } 
                        else position += (BitsPerElement - bitPosition);

                        bitPosition = 0;
                        mask = 1;
                    }
                }
                #endif

                return NoPosition;
            }

            template<bool Value>
            static __WSTL_CONSTEXPR14__ SizeType FindNext(ConstPointerType bits, SizeType position) __WSTL_NOEXCEPT__ {
                #ifdef __WSTL_BITSET_USE_CTZ___
                if(position < N) {
                    const SizeType bitPosition = position & (BitsPerElement - 1);
                    SizeType blockPosition = position >> Logarithm2<BitsPerElement>::Value;

                    const ElementType invertMask = (Value) ? ElementType(0) : ElementType(~0);

                    ElementType block = bits[blockPosition] ^ invertMask;
                    block &= ElementType(~0 << bitPosition);

                    while(blockPosition < NumberOfElements) {
                        if(block != 0) {
                            uint_least8_t ctz = CountRightZero(block);
                            return (blockPosition << Logarithm2<BitsPerElement>::Value) + ctz;
                        }

                        block = bits[++blockPosition] ^ invertMask;
                    }
                }
                #else
                if(position < N) {
                    SizeType bitPosition = position & (BitsPerElement - 1);
                    SizeType blockPosition = position >> Logarithm2<BitsPerElement>::Value;

                    ElementType mask = ElementType(1) << bitPosition;

                    while(blockPosition < NumberOfElements) {
                        ElementType block = bits[blockPosition++];

                        if((Value && block != AllClear) || (!Value && block != AllSet)) {
                            while(bitPosition++ < BitsPerElement && position < N) {
                                if(((block & mask) != 0) == Value) return position;
                                mask <<= 1;
                                ++position;
                            }
                        } 
                        else position += (BitsPerElement - bitPosition);

                        bitPosition = 0;
                        mask = 1;
                    }
                }
                #endif

                return NoPosition;
            }

            static __WSTL_CONSTEXPR14__ void Swap(PointerType bits1, PointerType bits2) {
                wstl::SwapRanges(bits1, bits1 + NumberOfElements, bits2);
            }
        };
    }

    // Bitset
    // Forward declaration

    template<size_t N = 0, typename T = unsigned char>
    class Bitset;

    template<>
    class Bitset<0, unsigned char> : public __private::__BitsetCommon<0, unsigned char> {};

    /// @brief Fixed-size sequence of bits
    /// @tparam N Number of bits
    /// @tparam T Underlying type to store bits
    /// @ingroup bitset
    template<size_t N, typename T>
    class Bitset : public __private::__Bitset<N, T, __private::__BitsetCommon<N, T>::NumberOfElements> {
    private:
        typedef __private::__Bitset<N, T, __private::__BitsetCommon<N, T>::NumberOfElements> Base;
    
        using Base::BitsPerElement;
        using Base::AllSet;
        using Base::AllClear;
        using Base::NumberOfElements;
        using Base::TopBits;
        using Base::TopMask;
        using Base::ActualSize;

    public:
        WSTL_STATIC_ASSERT(IsUnsigned<T>::Value, "Type must be unsigned");

        typedef typename Base::SizeType SizeType;
        typedef typename Base::ElementType ElementType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;
        typedef typename Base::SpanType SpanType;
        typedef typename Base::ConstSpanType ConstSpanType;

        using Base::NoPosition;

        /// @brief Proxy reference to a bit in the bitset
        class Reference {
        public:
            friend class Bitset;
            
            /// @brief Copy constructor
            /// @param other Other reference to copy from
            __WSTL_CONSTEXPR14__ Reference(const Reference& other) __WSTL_NOEXCEPT__ : m_Bitset(other.m_Bitset), m_Position(other.m_Position) {}

            /// @brief Conversion to bool
            /// @return Value of the referenced bit
            __WSTL_CONSTEXPR14__ operator bool() const __WSTL_NOEXCEPT__ {
                return m_Bitset->Test(m_Position);
            }

            /// @brief Copy assignment operator
            /// @param other Other reference to copy from
            __WSTL_CONSTEXPR14__ Reference& operator=(const Reference& other) __WSTL_NOEXCEPT__ {
                m_Bitset->Set(m_Position, bool(other));
                return *this;
            }

            /// @brief Assignment operator
            /// @param value Value to assign to the referenced bit
            __WSTL_CONSTEXPR14__ Reference& operator=(bool value) __WSTL_NOEXCEPT__ {
                m_Bitset->Set(m_Position, value);
                return *this;
            }

            /// @brief Bitwise NOT operator
            /// @return Inverted value of the referenced bit
            __WSTL_CONSTEXPR14__ bool operator~() const __WSTL_NOEXCEPT__ {
                return !m_Bitset->Test(m_Position);
            }

            /// @brief Flips the referenced bit
            __WSTL_CONSTEXPR14__ Reference& Flip() __WSTL_NOEXCEPT__ {
                m_Bitset->Flip(m_Position);
                return *this;
            }
            
        private:
            Bitset<N, ElementType>* m_Bitset;
            SizeType m_Position;

            /// @brief Private default constructor
            __WSTL_CONSTEXPR14__ Reference() __WSTL_NOEXCEPT__ : m_Bitset(NullPointer), m_Position(0) {}
            
            /// @brief Private constructor
            /// @param bitset Bitset containing the referenced bit
            /// @param position Position of the referenced bit
            __WSTL_CONSTEXPR14__ Reference(Bitset<N, ElementType>* bitset, SizeType position) __WSTL_NOEXCEPT__ : m_Bitset(bitset), m_Position(position) {}
        };

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ Bitset() __WSTL_NOEXCEPT__ : m_Bits()  {
            Base::Reset(m_Bits);
        }

        /// @brief Copy constructor
        /// @param other Other bitset to copy from
        __WSTL_CONSTEXPR14__ Bitset(const Bitset& other) __WSTL_NOEXCEPT__ : m_Bits() {
            Base::OperatorAssignment(m_Bits, other.m_Bits);
        }

        /// @brief Constructor from value
        /// @param value Value to initialize the bitset from
        __WSTL_CONSTEXPR14__ Bitset(unsigned long long value) __WSTL_NOEXCEPT__ : m_Bits() {
            Base::Initialize(m_Bits, value);
        }

        /// @brief Constructor from string
        /// @param string String to initialize the bitset from
        /// @param position Position in the string to start from
        /// @param count Number of characters to read from the string
        /// @param one Character representing a set bit (default is '1')
        /// @throws `OutOfRange` if position is out of range
        template<typename Derived, typename TChar, typename Traits>
        explicit Bitset(const BasicString<Derived, TChar, Traits>& string, typename BasicString<Derived, TChar, Traits>::SizeType position = 0, 
            typename BasicString<Derived, TChar, Traits>::SizeType count = BasicString<Derived, TChar, Traits>::NoPosition, TChar one = TChar('1')) : m_Bits() {
                __WSTL_ASSERT_RETURN__(position < string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset string position out of range"));
                Base::FromString(m_Bits, string.CString() + position, Min(count, string.Size() - position), one);
            }
        
        /// @brief Constructor from string view
        /// @param view String view to initialize the bitset from
        /// @param position Position in the string view to start from
        /// @param count Number of characters to read from the string view
        /// @param one Character representing a set bit (default is '1')
        /// @throws `OutOfRange` if position is out of range
        template<typename TChar, typename Traits>
        __WSTL_CONSTEXPR14__ explicit Bitset(const BasicStringView<TChar, Traits>& view, SizeType position = 0, 
            SizeType count = BasicStringView<TChar, Traits>::NoPosition, TChar one = TChar('1')) : m_Bits() {
                __WSTL_ASSERT_RETURN__(position < view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset view position out of range"));
                Base::FromString(m_Bits, view.Data() + position, Min(count, view.Size() - position), one);
            }
        
        /// @brief Constructor from C-style string
        /// @param string C-style string to initialize the bitset from
        /// @param one Character representing a set bit (default is '1')
        template<typename TChar>
        __WSTL_CONSTEXPR14__ explicit Bitset(const TChar* string, TChar one = TChar('1')) __WSTL_NOEXCEPT__ : m_Bits() {
            Base::FromString(m_Bits, string, CharacterTraits<TChar>::Length(string), one);
        }
        
        /// @brief Constructor from C-style string
        /// @param string C-style string to initialize the bitset from
        /// @param count Number of characters to read from the string
        /// @param one Character representing a set bit (default is '1')
        template<typename TChar>
        __WSTL_CONSTEXPR14__ explicit Bitset(const TChar* string, SizeType count, TChar one = TChar('1')) __WSTL_NOEXCEPT__ : m_Bits() {
            Base::FromString(m_Bits, string, count, one);
        }

        /// @brief Copy assignment operator
        /// @param other Other bitset to copy from
        __WSTL_CONSTEXPR14__ Bitset& operator=(const Bitset& other) __WSTL_NOEXCEPT__ {
            Base::OperatorAssignment(m_Bits, other.m_Bits);
            return *this;
        }

        /// @brief Subscript operator
        /// @param position Position of the bit to access
        /// @return Value of the bit at the specified position
        __WSTL_CONSTEXPR14__ bool operator[](SizeType position) const __WSTL_NOEXCEPT__ {
            return Base::Test(m_Bits, position);
        }

        /// @brief Subscript operator
        /// @param position Position of the bit to access
        /// @return Reference to the bit at the specified position
        __WSTL_CONSTEXPR14__ Reference operator[](SizeType position) __WSTL_NOEXCEPT__ {
            return Reference(this, position);
        }

        /// @brief Gets the value of the bit at the specified position
        /// @param position Position of the bit to access
        __WSTL_CONSTEXPR14__ bool Test(SizeType position) const {
            __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset test position out of range"), false);
            return Base::Test(m_Bits, position);
        }

        /// @brief Gets the value of the bit at the specified position - compile-time
        /// @tparam Position Position of the bit to access
        template<size_t Position>
        __WSTL_CONSTEXPR14__ bool Test() const {
            WSTL_STATIC_ASSERT(Position < N, "Test position out of bounds");
            return Base::template Test<Position>(m_Bits);
        }

        /// @brief Checks if all bits are set
        __WSTL_CONSTEXPR14__ bool All() const __WSTL_NOEXCEPT__ {
            return Base::All(m_Bits);
        }

        /// @brief Checks if any bit is set
        __WSTL_CONSTEXPR14__ bool Any() const __WSTL_NOEXCEPT__ {
            return Base::Any(m_Bits);
        }

        /// @brief Checks if no bits are set
        __WSTL_CONSTEXPR14__ bool None() const __WSTL_NOEXCEPT__ {
            return Base::None(m_Bits);
        }

        /// @brief Counts the number of set bits
        __WSTL_CONSTEXPR14__ SizeType Count() const __WSTL_NOEXCEPT__ {
            return Base::Count(m_Bits);
        }

        /// @brief Gets the size of the bitset
        static __WSTL_CONSTEXPR14__ SizeType Size() __WSTL_NOEXCEPT__ {
            return N;
        }

        /// @brief Sets all bits
        __WSTL_CONSTEXPR14__ Bitset& Set() __WSTL_NOEXCEPT__ {
            Base::Set(m_Bits);
            return *this;
        }

        /// @brief Sets the bit at the specified position
        /// @param position Position of the bit to set
        __WSTL_CONSTEXPR14__ Bitset& Set(SizeType position) {
            __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset set position out of range"), *this);
            Base::Set(m_Bits, position);
            return *this;
        }

        /// @brief Sets the bit at the specified position - compile-time
        /// @tparam Position Position of the bit to set
        template<size_t Position>
        __WSTL_CONSTEXPR14__ Bitset& Set() {
            WSTL_STATIC_ASSERT(Position < N, "Set position out of bounds");
            Base::template Set<Position>(m_Bits);
            return *this;
        }

        /// @brief Sets the bit at the specified position to the specified value
        /// @param position Position of the bit to set
        /// @param value Value to set the bit to
        __WSTL_CONSTEXPR14__ Bitset& Set(SizeType position, bool value) {
            __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset set position out of range"), *this);
            Base::Set(m_Bits, position, value);
            return *this;
        }

        /// @brief Sets the bit at the specified position to the specified value - compile-time
        /// @tparam Position Position of the bit to set
        /// @param value Value to set the bit to
        template<size_t Position>
        __WSTL_CONSTEXPR14__ Bitset& Set(bool value) {
            WSTL_STATIC_ASSERT(Position < N, "Set position out of bounds");
            Base::template Set<Position>(m_Bits, value);
            return *this;
        }

        /// @brief Sets the bit at the specified position to the specified value - compile-time
        /// @tparam Position Position of the bit to set
        /// @tparam Value Value to set the bit to
        template<size_t Position, bool Value>
        __WSTL_CONSTEXPR14__ Bitset& Set() {
            WSTL_STATIC_ASSERT(Position < N, "Set position out of bounds");
            Base::template Set<Position, Value>(m_Bits);
            return *this;
        }

        /// @brief Sets bits from a string
        /// @param string String to set bits from
        /// @param position Position in the string to start from
        /// @param count Number of characters to read from the string
        /// @param one Character representing a set bit (default is '1')
        /// @throws `OutOfRange` if position is out of range
        template<typename Derived, typename TChar, typename Traits>
        Bitset& Set(const BasicString<Derived, TChar, Traits>& string, SizeType position = 0, SizeType count = BasicString<Derived, TChar, Traits>::NoPosition, TChar one = TChar('1')) {
            __WSTL_ASSERT_RETURNVALUE__(position < string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset set string position out of range"), *this);
            Base::FromString(m_Bits, string.CString() + position, Min(count, string.Size() - position), one);
            return *this;
        }

        /// @brief Sets bits from a c-style string
        /// @param string C-style string to set bits from
        /// @param count Number of characters to read from the string
        /// @param one Character representing a set bit (default is '1')
        template<typename TChar>
        Bitset& Set(const TChar* string, SizeType count, TChar one = TChar('1')) __WSTL_NOEXCEPT__ {
            Base::FromString(m_Bits, string, count, one);
            return *this;
        }

        /// @brief Sets bits from a c-style string
        /// @param string C-style string to set bits from
        /// @param one Character representing a set bit (default is '1')
        template<typename TChar>
        Bitset& Set(const TChar* string, TChar one = TChar('1')) __WSTL_NOEXCEPT__ {
            Base::FromString(m_Bits, string, CharacterTraits<TChar>::Length(string), one);
            return *this;
        }

        /// @brief Resets all bits
        __WSTL_CONSTEXPR14__ Bitset& Reset() __WSTL_NOEXCEPT__ {
            Base::Reset(m_Bits);
            return *this;
        }

        /// @brief Resets the bit at the specified position
        /// @param position Position of the bit to reset
        __WSTL_CONSTEXPR14__ Bitset& Reset(SizeType position) {
            __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset reset position out of range"), *this);
            Base::Reset(m_Bits, position);
            return *this;
        }

        /// @brief Resets the bit at the specified position - compile-time
        /// @tparam Position Position of the bit to reset
        template<size_t Position>
        __WSTL_CONSTEXPR14__ Bitset& Reset() {
            WSTL_STATIC_ASSERT(Position < N, "Reset position out of bounds");
            Base::template Reset<Position>(m_Bits);
            return *this;
        }

        /// @brief Flips all bits
        __WSTL_CONSTEXPR14__ Bitset& Flip() __WSTL_NOEXCEPT__ {
            Base::Flip(m_Bits);
            return *this;
        }

        /// @brief Flips the bit at the specified position
        /// @param position Position of the bit to flip
        __WSTL_CONSTEXPR14__ Bitset& Flip(SizeType position) {
            __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset flip position out of range"), *this);
            Base::Flip(m_Bits, position);
            return *this;
        }

        /// @brief Flips the bit at the specified position - compile-time
        /// @tparam Position Position of the bit to flip
        template<size_t Position>
        __WSTL_CONSTEXPR14__ Bitset& Flip() {
            WSTL_STATIC_ASSERT(Position < N, "Flip position out of bounds");
            Base::template Flip<Position>(m_Bits);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Converts the bitset to a string
        /// @tparam TString String type to convert to (default is String<N>)
        /// @param zero Character representing a clear bit (default is '0')
        /// @param one Character representing a set bit (default is '1')
        /// @return String representation of the bitset
        template<typename TString = String<N>>
        #else
        /// @brief Converts the bitset to a string
        /// @tparam TString String type to convert to
        /// @param zero Character representing a clear bit (default is '0')
        /// @param one Character representing a set bit (default is '1')
        /// @return String representation of the bitset
        template<typename TString>
        #endif
        __WSTL_CONSTEXPR14__ TString ToString(typename TString::ValueType zero = typename TString::ValueType('0'), 
            typename TString::ValueType one = typename TString::ValueType('1')) const {
                return Base::template ToString<TString>(m_Bits, zero, one);
            }
        
        /// @brief Converts the bitset to an unsigned long if it fits
        __WSTL_CONSTEXPR14__ unsigned long ToUnsignedLong() const __WSTL_NOEXCEPT__ {
            WSTL_STATIC_ASSERT(sizeof(unsigned long) * 8 >= ActualSize, "Bitset can't be converted to unsigned long");
            return Base::template Value<unsigned long>(m_Bits);
        }

        /// @brief Converts the bitset to an unsigned long long if it fits
        __WSTL_CONSTEXPR14__ unsigned long long ToUnsignedLongLong() const __WSTL_NOEXCEPT__ {
            WSTL_STATIC_ASSERT(sizeof(unsigned long long) * 8 >= ActualSize, "Bitset can't be converted to unsigned long long");
            return Base::template Value<unsigned long long>(m_Bits);
        }

        /// @brief Finds the first occurrence of the specified bit value
        /// @param value Bit value to find
        /// @return Position of the first occurrence of the specified bit value, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirst(bool value) const __WSTL_NOEXCEPT__ {
            return Base::FindNext(m_Bits, 0, value);
        }

        /// @brief Finds the first occurrence of the specified bit value - compile-time
        /// @tparam Value Bit value to find
        /// @return Position of the first occurrence of the specified bit value, or `NoPosition` if not found
        template<bool Value>
        __WSTL_CONSTEXPR14__ SizeType FindFirst() const __WSTL_NOEXCEPT__ {
            return Base::template FindNext<Value>(m_Bits, 0);
        }
        
        /// @brief Finds the next occurrence of the specified bit value after the given position
        /// @param position Position to start searching from
        /// @param value Bit value to find
        /// @return Position of the next occurrence of the specified bit value, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindNext(SizeType position, bool value) const __WSTL_NOEXCEPT__ {
            return Base::FindNext(m_Bits, ++position, value);
        }

        /// @brief Finds the next occurrence of the specified bit value after the given position - compile-time
        /// @tparam Value Bit value to find
        /// @param position Position to start searching from
        /// @return Position of the next occurrence of the specified bit value, or `NoPosition` if not found
        template<bool Value>
        __WSTL_CONSTEXPR14__ SizeType FindNext(SizeType position) const __WSTL_NOEXCEPT__ {
            return Base::template FindNext<Value>(m_Bits, ++position);
        }

        /// @brief Swaps the contents of this bitset with another
        /// @param other Other bitset to swap with
        __WSTL_CONSTEXPR14__ void Swap(Bitset& other) __WSTL_NOEXCEPT__ {
            Base::Swap(m_Bits, other.m_Bits);
        }

        /// @brief Converts the bitset to an integral value
        /// @tparam U Integral type to convert to, must be large enough to hold all bits
        /// @return Integral representation of the bitset
        template<typename U>
        __WSTL_CONSTEXPR14__ U Value() const __WSTL_NOEXCEPT__ {
            WSTL_STATIC_ASSERT(IsIntegral<U>::Value, "Only integral types are supported");
            WSTL_STATIC_ASSERT((sizeof(U) * 8) >= ActualSize, "Type is too small");

            return Base::template Value<U>(m_Bits);
        }

        /// @brief Gets a span representing the bits in the bitset
        __WSTL_CONSTEXPR14__ SpanType Span() __WSTL_NOEXCEPT__ {
            return SpanType(m_Bits, NumberOfElements);
        }

        /// @brief Gets a const span representing the bits in the bitset
        __WSTL_CONSTEXPR14__ ConstSpanType Span() const __WSTL_NOEXCEPT__ {
            return ConstSpanType(m_Bits, NumberOfElements);
        }

        // Comparison operators

        friend __WSTL_CONSTEXPR14__ bool operator==(const Bitset& a, const Bitset& b) __WSTL_NOEXCEPT__ {
            return Base::OperatorEquality(a.m_Bits, b.m_Bits);
        }

        friend __WSTL_CONSTEXPR14__ bool operator!=(const Bitset& a, const Bitset& b) __WSTL_NOEXCEPT__ {
            return !(a == b);
        }

        // Bitwise operators

        __WSTL_CONSTEXPR14__ Bitset operator&(const Bitset& other) const __WSTL_NOEXCEPT__ {
            Bitset temp(*this);
            Base::OperatorAND(temp.m_Bits, other.m_Bits);
            return temp;
        }

        __WSTL_CONSTEXPR14__ Bitset operator|(const Bitset& other) const __WSTL_NOEXCEPT__ {
            Bitset temp(*this);
            Base::OperatorOR(temp.m_Bits, other.m_Bits);
            return temp;
        }

        __WSTL_CONSTEXPR14__ Bitset operator^(const Bitset& other) const __WSTL_NOEXCEPT__ {
            Bitset temp(*this);
            Base::OperatorXOR(temp.m_Bits, other.m_Bits);
            return temp;
        }

        __WSTL_CONSTEXPR14__ Bitset& operator&=(const Bitset& other) __WSTL_NOEXCEPT__ {
            Base::OperatorAND(m_Bits, other.m_Bits);
            return *this;
        }

        __WSTL_CONSTEXPR14__ Bitset& operator|=(const Bitset& other) __WSTL_NOEXCEPT__ {
            Base::OperatorOR(m_Bits, other.m_Bits);
            return *this;
        }

        __WSTL_CONSTEXPR14__ Bitset& operator^=(const Bitset& other) __WSTL_NOEXCEPT__ {
            Base::OperatorXOR(m_Bits, other.m_Bits);
            return *this;
        }

        __WSTL_CONSTEXPR14__ Bitset operator~() const __WSTL_NOEXCEPT__ {
            Bitset temp(*this);
            Base::Flip(temp.m_Bits);
            return temp;
        }

        __WSTL_CONSTEXPR14__ Bitset operator<<(SizeType shift) const __WSTL_NOEXCEPT__ {
            Bitset temp(*this);
            Base::OperatorShiftLeft(temp.m_Bits, shift);
            return temp;
        }

        __WSTL_CONSTEXPR14__ Bitset operator>>(SizeType shift) const __WSTL_NOEXCEPT__ {
            Bitset temp(*this);
            Base::OperatorShiftRight(temp.m_Bits, shift);
            return temp;
        }

        __WSTL_CONSTEXPR14__ Bitset& operator<<=(SizeType shift) __WSTL_NOEXCEPT__ {
            Base::OperatorShiftLeft(m_Bits, shift);
            return *this;
        }

        __WSTL_CONSTEXPR14__ Bitset& operator>>=(SizeType shift) __WSTL_NOEXCEPT__ {
            Base::OperatorShiftRight(m_Bits, shift);
            return *this;
        }
    
    protected:
        ElementType m_Bits[NumberOfElements];

        friend class Hash<Bitset>;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N, typename T>
    Bitset(const Bitset<N, T>&) -> Bitset<N, T>;
    #endif

    // Hash specialization

    template<size_t N, typename T>
    struct Hash<Bitset<N, T> > {
        size_t operator()(const Bitset<N, T>& bitset) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(bitset.m_Bits), reinterpret_cast<const uint8_t*>(bitset.m_Bits + bitset.NumberOfElements));
        }
    };

    // Bitset external

    namespace external {
        // Bitset
        // Forward declaration

        template<size_t N = 0, typename T = unsigned char>
        class Bitset;

        template<>
        class Bitset<0, unsigned char> : public __private::__BitsetCommon<0, unsigned char> {};

        /// @brief A version of `Bitset` that uses external storage
        /// @tparam N Number of bits
        /// @tparam T Underlying type to store bits
        /// @ingroup bitset
        template<size_t N, typename T>
        class Bitset : public __private::__Bitset<N, T, __private::__BitsetCommon<N, T>::NumberOfElements> {
        private:
            typedef __private::__Bitset<N, T, __private::__BitsetCommon<N, T>::NumberOfElements> Base;
        
            using Base::BitsPerElement;
            using Base::AllSet;
            using Base::AllClear;
            using Base::NumberOfElements;
            using Base::TopBits;
            using Base::TopMask;
            using Base::ActualSize;

        public:
            WSTL_STATIC_ASSERT(IsUnsigned<T>::Value, "Type must be unsigned");

            typedef typename Base::SizeType SizeType;
            typedef typename Base::ElementType ElementType;
            typedef typename Base::PointerType PointerType;
            typedef typename Base::ConstPointerType ConstPointerType;
            typedef typename Base::SpanType SpanType;
            typedef typename Base::ConstSpanType ConstSpanType;

            typedef Array<T, NumberOfElements> BufferType;

            using Base::NoPosition;

            /// @brief Proxy reference to a bit in the bitset
            class Reference {
            public:
                friend class Bitset;
                
                /// @brief Copy constructor
                /// @param other Other reference to copy from
                __WSTL_CONSTEXPR14__ Reference(const Reference& other) __WSTL_NOEXCEPT__ : m_Bitset(other.m_Bitset), m_Position(other.m_Position) {}

                /// @brief Conversion to bool
                /// @return Value of the referenced bit
                __WSTL_CONSTEXPR14__ operator bool() const __WSTL_NOEXCEPT__ {
                    return m_Bitset->Test(m_Position);
                }

                /// @brief Copy assignment operator
                /// @param other Other reference to copy from
                __WSTL_CONSTEXPR14__ Reference& operator=(const Reference& other) __WSTL_NOEXCEPT__ {
                    m_Bitset->Set(m_Position, bool(other));
                    return *this;
                }

                /// @brief Assignment operator
                /// @param value Value to assign to the referenced bit
                __WSTL_CONSTEXPR14__ Reference& operator=(bool value) __WSTL_NOEXCEPT__ {
                    m_Bitset->Set(m_Position, value);
                    return *this;
                }

                /// @brief Bitwise NOT operator
                /// @return Inverted value of the referenced bit
                __WSTL_CONSTEXPR14__ bool operator~() const __WSTL_NOEXCEPT__ {
                    return !m_Bitset->Test(m_Position);
                }

                /// @brief Flips the referenced bit
                __WSTL_CONSTEXPR14__ Reference& Flip() __WSTL_NOEXCEPT__ {
                    m_Bitset->Flip(m_Position);
                    return *this;
                }
                
            private:
                Bitset<N, ElementType>* m_Bitset;
                SizeType m_Position;

                /// @brief Private default constructor
                __WSTL_CONSTEXPR14__ Reference() __WSTL_NOEXCEPT__ : m_Bitset(NullPointer), m_Position(0) {}
                
                /// @brief Private constructor
                /// @param bitset Bitset containing the referenced bit
                /// @param position Position of the referenced bit
                __WSTL_CONSTEXPR14__ Reference(Bitset<N, ElementType>* bitset, SizeType position) __WSTL_NOEXCEPT__ : m_Bitset(bitset), m_Position(position) {}
            };

            /// @brief Default constructor
            /// @param buffer External buffer to store bits
            /// @throws `LogicError` if buffer is `nullptr`
            __WSTL_CONSTEXPR14__ explicit Bitset(PointerType buffer) : m_Bits(buffer) {
                __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                Base::Reset(m_Bits);
            }

            /// @brief Default constructor
            /// @param buffer External buffer to store bits
            __WSTL_CONSTEXPR14__ explicit Bitset(BufferType& buffer) __WSTL_NOEXCEPT__ : m_Bits(buffer.Data()) {
                Base::Reset(m_Bits);
            }

            /// @brief Copy constructor, deleted
            __WSTL_CONSTEXPR14__ Bitset(const Bitset& other) __WSTL_NOEXCEPT__ __WSTL_DELETE__;

            /// @brief Copy constructor with external buffer
            /// @param other Other bitset to copy from
            /// @param buffer External buffer to store bits
            /// @throws `LogicError` if buffer is `nullptr`
            __WSTL_CONSTEXPR14__ Bitset(const Bitset& other, PointerType buffer) : m_Bits(buffer) {
                __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                Base::OperatorAssignment(m_Bits, other.m_Bits);
            }

            /// @brief Copy constructor with external buffer
            /// @param other Other bitset to copy from
            /// @param buffer External buffer to store bits
            __WSTL_CONSTEXPR14__ Bitset(const Bitset& other, BufferType& buffer) __WSTL_NOEXCEPT__ : m_Bits(buffer.Data()) {
                Base::OperatorAssignment(m_Bits, other.m_Bits);
            }

            /// @brief Constructor from value
            /// @param value Value to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @throws `LogicError` if buffer is `nullptr`
            __WSTL_CONSTEXPR14__ Bitset(unsigned long long value, PointerType buffer) : m_Bits(buffer) {
                __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                Base::Initialize(m_Bits, value);
            }

            /// @brief Constructor from value
            /// @param value Value to initialize the bitset from
            /// @param buffer External buffer to store bits
            __WSTL_CONSTEXPR14__ Bitset(unsigned long long value, BufferType& buffer) __WSTL_NOEXCEPT__ : m_Bits(buffer.Data()) {
                Base::Initialize(m_Bits, value);
            }

            /// @brief Constructor from string
            /// @param string String to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param position Position in the string to start from
            /// @param count Number of characters to read from the string
            /// @param one Character representing a set bit (default is '1')
            /// @throws `OutOfRange` if position is out of range, or `LogicError` if buffer is `nullptr`
            template<typename Derived, typename TChar, typename Traits>
            explicit Bitset(const BasicString<Derived, TChar, Traits>& string, PointerType buffer, typename BasicString<Derived, TChar, Traits>::SizeType position = 0, 
                typename BasicString<Derived, TChar, Traits>::SizeType count = BasicString<Derived, TChar, Traits>::NoPosition, TChar one = TChar('1')) : m_Bits(buffer) {
                    __WSTL_ASSERT_RETURN__(position < string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset string position out of range"));
                    __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                    Base::FromString(m_Bits, string.CString() + position, Min(count, string.Size() - position), one);
                }

            /// @brief Constructor from string
            /// @param string String to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param position Position in the string to start from
            /// @param count Number of characters to read from the string
            /// @param one Character representing a set bit (default is '1')
            /// @throws `OutOfRange` if position is out of range
            template<typename Derived, typename TChar, typename Traits>
            explicit Bitset(const BasicString<Derived, TChar, Traits>& string, BufferType& buffer, typename BasicString<Derived, TChar, Traits>::SizeType position = 0, 
                typename BasicString<Derived, TChar, Traits>::SizeType count = BasicString<Derived, TChar, Traits>::NoPosition, TChar one = TChar('1')) : m_Bits(buffer.Data()) {
                    __WSTL_ASSERT_RETURN__(position < string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset string position out of range"));
                    Base::FromString(m_Bits, string.CString() + position, Min(count, string.Size() - position), one);
                }
            
            /// @brief Constructor from string view
            /// @param view String view to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param position Position in the string view to start from
            /// @param count Number of characters to read from the string view
            /// @param one Character representing a set bit (default is '1')
            /// @throws `OutOfRange` if position is out of range, or `LogicError` if buffer is `nullptr`
            template<typename TChar, typename Traits>
            __WSTL_CONSTEXPR14__ explicit Bitset(const BasicStringView<TChar, Traits>& view, PointerType buffer, SizeType position = 0, 
                SizeType count = BasicStringView<TChar, Traits>::NoPosition, TChar one = TChar('1')) : m_Bits(buffer) {
                    __WSTL_ASSERT_RETURN__(position < view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset view position out of range"));
                    __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                    Base::FromString(m_Bits, view.Data() + position, Min(count, view.Size() - position), one);
                }

            /// @brief Constructor from string view
            /// @param view String view to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param position Position in the string view to start from
            /// @param count Number of characters to read from the string view
            /// @param one Character representing a set bit (default is '1')
            /// @throws `OutOfRange` if position is out of range
            template<typename TChar, typename Traits>
            __WSTL_CONSTEXPR14__ explicit Bitset(const BasicStringView<TChar, Traits>& view, BufferType& buffer, SizeType position = 0, 
                SizeType count = BasicStringView<TChar, Traits>::NoPosition, TChar one = TChar('1')) : m_Bits(buffer.Data()) {
                    __WSTL_ASSERT_RETURN__(position < view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset view position out of range"));
                    Base::FromString(m_Bits, view.Data() + position, Min(count, view.Size() - position), one);
                }
                
            /// @brief Constructor from C-style string
            /// @param string C-style string to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param one Character representing a set bit (default is '1')
            /// @throws `LogicError` if buffer is `nullptr`
            template<typename TChar>
            __WSTL_CONSTEXPR14__ explicit Bitset(const TChar* string, PointerType buffer, TChar one = TChar('1')) : m_Bits(buffer) {
                __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                Base::FromString(m_Bits, string, CharacterTraits<TChar>::Length(string), one);
            }

            /// @brief Constructor from C-style string
            /// @param string C-style string to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param one Character representing a set bit (default is '1')
            template<typename TChar>
            __WSTL_CONSTEXPR14__ explicit Bitset(const TChar* string, BufferType& buffer, TChar one = TChar('1')) __WSTL_NOEXCEPT__ : m_Bits(buffer.Data()) {
                Base::FromString(m_Bits, string, CharacterTraits<TChar>::Length(string), one);
            }
            
            /// @brief Constructor from C-style string
            /// @param string C-style string to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param count Number of characters to read from the string
            /// @param one Character representing a set bit (default is '1')
            /// @throws `LogicError` if buffer is `nullptr`
            template<typename TChar>
            __WSTL_CONSTEXPR14__ explicit Bitset(const TChar* string, PointerType buffer, SizeType count, TChar one = TChar('1')) : m_Bits(buffer) {
                __WSTL_ASSERT__(m_Bits != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "Bitset buffer is nullptr"));
                Base::FromString(m_Bits, string, count, one);
            }

            /// @brief Constructor from C-style string
            /// @param string C-style string to initialize the bitset from
            /// @param buffer External buffer to store bits
            /// @param count Number of characters to read from the string
            /// @param one Character representing a set bit (default is '1')
            template<typename TChar>
            __WSTL_CONSTEXPR14__ explicit Bitset(const TChar* string, BufferType& buffer, SizeType count, TChar one = TChar('1')) __WSTL_NOEXCEPT__ : m_Bits(buffer.Data()) {
                Base::FromString(m_Bits, string, count, one);
            }

            /// @brief Copy assignment operator
            /// @param other Other bitset to copy from
            __WSTL_CONSTEXPR14__ Bitset& operator=(const Bitset& other) __WSTL_NOEXCEPT__ {
                Base::OperatorAssignment(m_Bits, other.m_Bits);
                return *this;
            }

            /// @brief Subscript operator
            /// @param position Position of the bit to access
            /// @return Value of the bit at the specified position
            __WSTL_CONSTEXPR14__ bool operator[](SizeType position) const __WSTL_NOEXCEPT__ {
                return Base::Test(m_Bits, position);
            }

            /// @brief Subscript operator
            /// @param position Position of the bit to access
            /// @return Reference to the bit at the specified position
            __WSTL_CONSTEXPR14__ Reference operator[](SizeType position) __WSTL_NOEXCEPT__ {
                return Reference(this, position);
            }

            /// @brief Gets the value of the bit at the specified position
            /// @param position Position of the bit to access
            __WSTL_CONSTEXPR14__ bool Test(SizeType position) const {
                __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset test position out of range"), false);
                return Base::Test(m_Bits, position);
            }

            /// @brief Gets the value of the bit at the specified position - compile-time
            /// @tparam Position Position of the bit to access
            template<size_t Position>
            __WSTL_CONSTEXPR14__ bool Test() const {
                WSTL_STATIC_ASSERT(Position < N, "Test position out of bounds");
                return Base::template Test<Position>(m_Bits);
            }

            /// @brief Checks if all bits are set
            __WSTL_CONSTEXPR14__ bool All() const __WSTL_NOEXCEPT__ {
                return Base::All(m_Bits);
            }

            /// @brief Checks if any bit is set
            __WSTL_CONSTEXPR14__ bool Any() const __WSTL_NOEXCEPT__ {
                return Base::Any(m_Bits);
            }

            /// @brief Checks if no bits are set
            __WSTL_CONSTEXPR14__ bool None() const __WSTL_NOEXCEPT__ {
                return Base::None(m_Bits);
            }

            /// @brief Counts the number of set bits
            __WSTL_CONSTEXPR14__ SizeType Count() const __WSTL_NOEXCEPT__ {
                return Base::Count(m_Bits);
            }

            /// @brief Gets the size of the bitset
            static __WSTL_CONSTEXPR14__ SizeType Size() __WSTL_NOEXCEPT__ {
                return N;
            }

            /// @brief Sets all bits
            __WSTL_CONSTEXPR14__ Bitset& Set() __WSTL_NOEXCEPT__ {
                Base::Set(m_Bits);
                return *this;
            }

            /// @brief Sets the bit at the specified position
            /// @param position Position of the bit to set
            __WSTL_CONSTEXPR14__ Bitset& Set(SizeType position) {
                __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset set position out of range"), *this);
                Base::Set(m_Bits, position);
                return *this;
            }

            /// @brief Sets the bit at the specified position - compile-time
            /// @tparam Position Position of the bit to set
            template<size_t Position>
            __WSTL_CONSTEXPR14__ Bitset& Set() {
                WSTL_STATIC_ASSERT(Position < N, "Set position out of bounds");
                Base::template Set<Position>(m_Bits);
                return *this;
            }

            /// @brief Sets the bit at the specified position to the specified value
            /// @param position Position of the bit to set
            /// @param value Value to set the bit to
            __WSTL_CONSTEXPR14__ Bitset& Set(SizeType position, bool value) {
                __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset set position out of range"), *this);
                Base::Set(m_Bits, position, value);
                return *this;
            }

            /// @brief Sets the bit at the specified position to the specified value - compile-time
            /// @tparam Position Position of the bit to set
            /// @param value Value to set the bit to
            template<size_t Position>
            __WSTL_CONSTEXPR14__ Bitset& Set(bool value) {
                WSTL_STATIC_ASSERT(Position < N, "Set position out of bounds");
                Base::template Set<Position>(m_Bits, value);
                return *this;
            }

            /// @brief Sets the bit at the specified position to the specified value - compile-time
            /// @tparam Position Position of the bit to set
            /// @tparam Value Value to set the bit to
            template<size_t Position, bool Value>
            __WSTL_CONSTEXPR14__ Bitset& Set() {
                WSTL_STATIC_ASSERT(Position < N, "Set position out of bounds");
                Base::template Set<Position, Value>(m_Bits);
                return *this;
            }

            /// @brief Sets bits from a string
            /// @param string String to set bits from
            /// @param position Position in the string to start from
            /// @param count Number of characters to read from the string
            /// @param one Character representing a set bit (default is '1')
            /// @throws `OutOfRange` if position is out of range
            template<typename Derived, typename TChar, typename Traits>
            Bitset& Set(const BasicString<Derived, TChar, Traits>& string, SizeType position = 0, SizeType count = BasicString<Derived, TChar, Traits>::NoPosition, TChar one = TChar('1')) {
                __WSTL_ASSERT_RETURNVALUE__(position < string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset set string position out of range"), *this);
                Base::FromString(m_Bits, string.CString() + position, Min(count, string.Size() - position), one);
                return *this;
            }

            /// @brief Sets bits from a c-style string
            /// @param string C-style string to set bits from
            /// @param count Number of characters to read from the string
            /// @param one Character representing a set bit (default is '1')
            template<typename TChar>
            Bitset& Set(const TChar* string, SizeType count, TChar one = TChar('1')) __WSTL_NOEXCEPT__ {
                Base::FromString(m_Bits, string, count, one);
                return *this;
            }

            /// @brief Sets bits from a c-style string
            /// @param string C-style string to set bits from
            /// @param one Character representing a set bit (default is '1')
            template<typename TChar>
            Bitset& Set(const TChar* string, TChar one = TChar('1')) __WSTL_NOEXCEPT__ {
                Base::FromString(m_Bits, string, CharacterTraits<TChar>::Length(string), one);
                return *this;
            }

            /// @brief Resets all bits
            __WSTL_CONSTEXPR14__ Bitset& Reset() __WSTL_NOEXCEPT__ {
                Base::Reset(m_Bits);
                return *this;
            }

            /// @brief Resets the bit at the specified position
            /// @param position Position of the bit to reset
            __WSTL_CONSTEXPR14__ Bitset& Reset(SizeType position) {
                __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset reset position out of range"), *this);
                Base::Reset(m_Bits, position);
                return *this;
            }

            /// @brief Resets the bit at the specified position - compile-time
            /// @tparam Position Position of the bit to reset
            template<size_t Position>
            __WSTL_CONSTEXPR14__ Bitset& Reset() {
                WSTL_STATIC_ASSERT(Position < N, "Reset position out of bounds");
                Base::template Reset<Position>(m_Bits);
                return *this;
            }

            /// @brief Flips all bits
            __WSTL_CONSTEXPR14__ Bitset& Flip() __WSTL_NOEXCEPT__ {
                Base::Flip(m_Bits);
                return *this;
            }

            /// @brief Flips the bit at the specified position
            /// @param position Position of the bit to flip
            __WSTL_CONSTEXPR14__ Bitset& Flip(SizeType position) {
                __WSTL_ASSERT_RETURNVALUE__(position < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Bitset flip position out of range"), *this);
                Base::Flip(m_Bits, position);
                return *this;
            }

            /// @brief Flips the bit at the specified position - compile-time
            /// @tparam Position Position of the bit to flip
            template<size_t Position>
            __WSTL_CONSTEXPR14__ Bitset& Flip() {
                WSTL_STATIC_ASSERT(Position < N, "Flip position out of bounds");
                Base::template Flip<Position>(m_Bits);
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Converts the bitset to a string
            /// @tparam TString String type to convert to (default is String<N>)
            /// @param zero Character representing a clear bit (default is '0')
            /// @param one Character representing a set bit (default is '1')
            /// @return String representation of the bitset
            template<typename TString = String<N>>
            #else
            /// @brief Converts the bitset to a string
            /// @tparam TString String type to convert to
            /// @param zero Character representing a clear bit (default is '0')
            /// @param one Character representing a set bit (default is '1')
            /// @return String representation of the bitset
            template<typename TString>
            #endif
            __WSTL_CONSTEXPR14__ TString ToString(typename TString::ValueType zero = typename TString::ValueType('0'), 
                typename TString::ValueType one = typename TString::ValueType('1')) const {
                    return Base::template ToString<TString>(m_Bits, zero, one);
                }
            
            /// @brief Converts the bitset to an unsigned long if it fits
            __WSTL_CONSTEXPR14__ unsigned long ToUnsignedLong() const __WSTL_NOEXCEPT__ {
                WSTL_STATIC_ASSERT(sizeof(unsigned long) * 8 >= ActualSize, "Bitset can't be converted to unsigned long");
                return Base::template Value<unsigned long>(m_Bits);
            }

            /// @brief Converts the bitset to an unsigned long long if it fits
            __WSTL_CONSTEXPR14__ unsigned long long ToUnsignedLongLong() const __WSTL_NOEXCEPT__ {
                WSTL_STATIC_ASSERT(sizeof(unsigned long long) * 8 >= ActualSize, "Bitset can't be converted to unsigned long long");
                return Base::template Value<unsigned long long>(m_Bits);
            }

            /// @brief Finds the first occurrence of the specified bit value
            /// @param value Bit value to find
            /// @return Position of the first occurrence of the specified bit value, or `NoPosition` if not found
            __WSTL_CONSTEXPR14__ SizeType FindFirst(bool value) const __WSTL_NOEXCEPT__ {
                return Base::FindNext(m_Bits, 0, value);
            }

            /// @brief Finds the first occurrence of the specified bit value - compile-time
            /// @tparam Value Bit value to find
            /// @return Position of the first occurrence of the specified bit value, or `NoPosition` if not found
            template<bool Value>
            __WSTL_CONSTEXPR14__ SizeType FindFirst() const __WSTL_NOEXCEPT__ {
                return Base::template FindNext<Value>(m_Bits, 0);
            }
            
            /// @brief Finds the next occurrence of the specified bit value after the given position
            /// @param position Position to start searching from
            /// @param value Bit value to find
            /// @return Position of the next occurrence of the specified bit value, or `NoPosition` if not found
            __WSTL_CONSTEXPR14__ SizeType FindNext(SizeType position, bool value) const __WSTL_NOEXCEPT__ {
                return Base::FindNext(m_Bits, ++position, value);
            }

            /// @brief Finds the next occurrence of the specified bit value after the given position - compile-time
            /// @tparam Value Bit value to find
            /// @param position Position to start searching from
            /// @return Position of the next occurrence of the specified bit value, or `NoPosition` if not found
            template<bool Value>
            __WSTL_CONSTEXPR14__ SizeType FindNext(SizeType position) const __WSTL_NOEXCEPT__ {
                return Base::template FindNext<Value>(m_Bits, ++position);
            }

            /// @brief Swaps the contents of this bitset with another
            /// @param other Other bitset to swap with
            __WSTL_CONSTEXPR14__ void Swap(Bitset& other) __WSTL_NOEXCEPT__ {
                Base::Swap(m_Bits, other.m_Bits);
            }

            /// @brief Converts the bitset to an integral value
            /// @tparam U Integral type to convert to, must be large enough to hold all bits
            /// @return Integral representation of the bitset
            template<typename U>
            __WSTL_CONSTEXPR14__ U Value() const __WSTL_NOEXCEPT__ {
                WSTL_STATIC_ASSERT(IsIntegral<U>::Value, "Only integral types are supported");
                WSTL_STATIC_ASSERT((sizeof(U) * 8) >= ActualSize, "Type is too small");

                return Base::template Value<U>(m_Bits);
            }

            /// @brief Gets a span representing the bits in the bitset
            __WSTL_CONSTEXPR14__ SpanType Span() __WSTL_NOEXCEPT__ {
                return SpanType(m_Bits, NumberOfElements);
            }

            /// @brief Gets a const span representing the bits in the bitset
            __WSTL_CONSTEXPR14__ ConstSpanType Span() const __WSTL_NOEXCEPT__ {
                return ConstSpanType(m_Bits, NumberOfElements);
            }

            // Comparison operators

            friend __WSTL_CONSTEXPR14__ bool operator==(const Bitset& a, const Bitset& b) __WSTL_NOEXCEPT__ {
                return Base::OperatorEquality(a.m_Bits, b.m_Bits);
            }

            friend __WSTL_CONSTEXPR14__ bool operator!=(const Bitset& a, const Bitset& b) __WSTL_NOEXCEPT__ {
                return !(a == b);
            }

            // Bitwise operators

            __WSTL_CONSTEXPR14__ Bitset operator&(const Bitset& other) const __WSTL_NOEXCEPT__ {
                Bitset temp(*this);
                Base::OperatorAND(temp.m_Bits, other.m_Bits);
                return temp;
            }

            __WSTL_CONSTEXPR14__ Bitset operator|(const Bitset& other) const __WSTL_NOEXCEPT__ {
                Bitset temp(*this);
                Base::OperatorOR(temp.m_Bits, other.m_Bits);
                return temp;
            }

            __WSTL_CONSTEXPR14__ Bitset operator^(const Bitset& other) const __WSTL_NOEXCEPT__ {
                Bitset temp(*this);
                Base::OperatorXOR(temp.m_Bits, other.m_Bits);
                return temp;
            }

            __WSTL_CONSTEXPR14__ Bitset& operator&=(const Bitset& other) __WSTL_NOEXCEPT__ {
                Base::OperatorAND(m_Bits, other.m_Bits);
                return *this;
            }

            __WSTL_CONSTEXPR14__ Bitset& operator|=(const Bitset& other) __WSTL_NOEXCEPT__ {
                Base::OperatorOR(m_Bits, other.m_Bits);
                return *this;
            }

            __WSTL_CONSTEXPR14__ Bitset& operator^=(const Bitset& other) __WSTL_NOEXCEPT__ {
                Base::OperatorXOR(m_Bits, other.m_Bits);
                return *this;
            }

            __WSTL_CONSTEXPR14__ Bitset operator~() const __WSTL_NOEXCEPT__ {
                Bitset temp(*this);
                Base::Flip(temp.m_Bits);
                return temp;
            }

            __WSTL_CONSTEXPR14__ Bitset operator<<(SizeType shift) const __WSTL_NOEXCEPT__ {
                Bitset temp(*this);
                Base::OperatorShiftLeft(temp.m_Bits, shift);
                return temp;
            }

            __WSTL_CONSTEXPR14__ Bitset operator>>(SizeType shift) const __WSTL_NOEXCEPT__ {
                Bitset temp(*this);
                Base::OperatorShiftRight(temp.m_Bits, shift);
                return temp;
            }

            __WSTL_CONSTEXPR14__ Bitset& operator<<=(SizeType shift) __WSTL_NOEXCEPT__ {
                Base::OperatorShiftLeft(m_Bits, shift);
                return *this;
            }

            __WSTL_CONSTEXPR14__ Bitset& operator>>=(SizeType shift) __WSTL_NOEXCEPT__ {
                Base::OperatorShiftRight(m_Bits, shift);
                return *this;
            }
        
        protected:
            ElementType* m_Bits;

            friend class Hash<Bitset>;
        };
    }

    // Hash specialization

    template<size_t N, typename T>
    struct Hash<external::Bitset<N, T> > {
        size_t operator()(const external::Bitset<N, T>& bitset) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(bitset.m_Bits), reinterpret_cast<const uint8_t*>(bitset.m_Bits + bitset.NumberOfElements));
        }
    };
}

#endif