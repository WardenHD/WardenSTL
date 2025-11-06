#ifndef __WSTL_BITSET_HPP__
#define __WSTL_BITSET_HPP__

#include "private/Platform.hpp"
#include "Limits.hpp"
#include "Span.hpp"

#include <stddef.h>


namespace wstl {
    namespace __private {
        template<size_t N, typename T>
        class __BitsetCommon {
        public:
            typedef size_t SizeType;
            typedef T ElementType;
            typedef T* PointerType;
            typedef const T* ConstPointerType;

            static const __WSTL_CONSTEXPR__ SizeType NoPosition = NumericLimits<SizeType>::Max();

        private:
            static const __WSTL_CONSTEXPR__ SizeType BitsPerElement = sizeof(ElementType) * 8;
            static const __WSTL_CONSTEXPR__ ElementType AllSet = NumericLimits<ElementType>::Max();
            static const __WSTL_CONSTEXPR__ ElementType AllClear = ElementType(0);
            static const __WSTL_CONSTEXPR__ SizeType NumberOfElements = (N % BitsPerElement == 0) ? N / BitsPerElement : N / BitsPerElement + 1; 
            static const __WSTL_CONSTEXPR__ SizeType TopBits = N % BitsPerElement;
            static const __WSTL_CONSTEXPR__ ElementType TopMask = (TopBits == 0) ? AllSet : ~(AllSet << TopBits);
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
            using Base::BitsPerElement;
            using Base::AllSet;
            using Base::AllClear;
            using Base::NumberOfElements;
            using Base::TopBits;
            using Base::TopMask;
            using Base::ActualSize;

            static __WSTL_CONSTEXPR14__ bool Test(ConstPointerType bits, SizeType position) {
                return (*bits & ElementType(1 << position)) != 0;
            }

            static __WSTL_CONSTEXPR14__ bool All(ConstPointerType bits) {
                return (*bits & TopMask) == AllSet;
            }

            static __WSTL_CONSTEXPR14__ bool Any(ConstPointerType bits) {
                return *bits != AllClear;
            }

            static __WSTL_CONSTEXPR14__ bool None(ConstPointerType bits) {
                return *bits == AllClear;
            }

            static __WSTL_CONSTEXPR14__ SizeType Count(ConstPointerType bits) {
                return PopulationCount(*bits);
            }

            static __WSTL_CONSTEXPR14__ SizeType Size() {
                return N;
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) {
                *bits = (AllSet & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, SizeType position) {
                *bits |= ElementType(1 << position);
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits) {
                *bits = AllClear;
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits, SizeType position) {
                *bits &= ~ElementType(1 << position);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits) {
                *bits = (~*bits & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits, SizeType position) {
                *bits ^= ElementType(1 << position);
            }

            static __WSTL_CONSTEXPR14__ bool OperatorEquality(ConstPointerType bits1, ConstPointerType bits2) {
                return *bits1 == *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorAND(ConstPointerType bits1, ConstPointerType bits2) {
                *bits1 &= *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorOR(ConstPointerType bits1, ConstPointerType bits2) {
                *bits1 |= *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorXOR(ConstPointerType bits1, ConstPointerType bits2) {
                *bits1 ^= *bits2;
            }

            static __WSTL_CONSTEXPR14__ void OperatorNOT(ConstPointerType bits) {
                *bits = (~*bits & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftLeft(ConstPointerType bits, SizeType position) {
                *bits >>= position;
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftRight(ConstPointerType bits, SizeType position) {
                *bits <<= position;
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
            using Base::BitsPerElement;
            using Base::AllSet;
            using Base::AllClear;
            using Base::NumberOfElements;
            using Base::TopBits;
            using Base::TopMask;
            using Base::ActualSize;

            static __WSTL_CONSTEXPR14__ bool Test(ConstPointerType bits, SizeType position) {
                SizeType index = position << Logarithm2<BitsPerElement>::Value; // same as (position / BitsPerElement) but more efficient
                return (bits[index] & ElementType(1 << position)) != 0;
            }

            static __WSTL_CONSTEXPR14__ bool All(ConstPointerType bits) {
                while(NumberOfElements-- != 1) 
                    if(*bits++ != AllSet) return false;

                return (*bits & TopMask) == AllSet;
            }

            static __WSTL_CONSTEXPR14__ bool Any(ConstPointerType bits) {
                while(NumberOfElements-- != 0)
                    if(*bits++ != AllClear) return true;

                return false;
            }

            static __WSTL_CONSTEXPR14__ bool None(ConstPointerType bits) {
                while(NumberOfElements-- != 0)
                    if(*bits++ != AllClear) return false;

                return true;
            }

            static __WSTL_CONSTEXPR14__ SizeType Count(ConstPointerType bits) {
                SizeType count = 0;

                while(NumberOfElements-- != 0) count += PopulationCount(*bits++);

                return count;
            }

            static __WSTL_CONSTEXPR14__ SizeType Size() {
                return N;
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits) {
                while(NumberOfElements-- != 1) *bits++ = AllSet;
                *bits = (AllSet & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Set(PointerType bits, SizeType position) {
                SizeType index = position << Logarithm2<BitsPerElement>::Value;
                bits[index] |= ElementType(1 << position);
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits) {
                while(NumberOfElements-- != 0) *bits = AllClear;
            }

            static __WSTL_CONSTEXPR14__ void Reset(PointerType bits, SizeType position) {
                SizeType index = position << Logarithm2<BitsPerElement>::Value;
                bits[index] &= ~ElementType(1 << position);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits) {
                while(NumberOfElements-- != 1) *bits = ~*bits;
                *bits = (~*bits & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void Flip(PointerType bits, SizeType position) {
                *bits ^= ElementType(1 << position);
            }

            static __WSTL_CONSTEXPR14__ bool OperatorEquality(ConstPointerType bits1, ConstPointerType bits2) {
                return Equal(bits1, bits1 + NumberOfElements, bits2);
            }

            static __WSTL_CONSTEXPR14__ void OperatorAND(ConstPointerType bits1, ConstPointerType bits2) {
                while(NumberOfElements-- != 0) *bits1++ &= *bits2++;
            }

            static __WSTL_CONSTEXPR14__ void OperatorOR(ConstPointerType bits1, ConstPointerType bits2) {
                while(NumberOfElements-- != 0) *bits1++ |= *bits2++;
            }

            static __WSTL_CONSTEXPR14__ void OperatorXOR(ConstPointerType bits1, ConstPointerType bits2) {
                while(NumberOfElements-- != 0) *bits1++ ^= *bits2++;
            }

            static __WSTL_CONSTEXPR14__ void OperatorNOT(ConstPointerType bits) {
                while(NumberOfElements-- != 1) *bits = ~*bits;
                *bits = (~*bits & TopMask);
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftLeft(ConstPointerType bits, SizeType position) {
                /// TODO
            }

            static __WSTL_CONSTEXPR14__ void OperatorShiftRight(ConstPointerType bits, SizeType position) {
                /// TODO
            }
        };
    }

    template<size_t N, typename T>
    class Bitset : public __private::__Bitset<N, T, __private::__BitsetCommon<N, T>::NumberOfElements> {
    private:
        typedef __private::__Bitset<N, T, __private::__BitsetCommon<N, T>::NumberOfElements> Base;

    public:
        typedef typename Base::SizeType SizeType;
        typedef typename Base::ElementType ElementType;
        typedef typename Base::PointerType PointerType;
        typedef typename Base::ConstPointerType ConstPointerType;
        typedef typename Base::SpanType SpanType;
        typedef typename Base::ConstSpanType ConstSpanType;

        using Base::NoPosition;
        using Base::BitsPerElement;
        using Base::AllSet;
        using Base::AllClear;
        using Base::NumberOfElements;
        using Base::TopBits;
        using Base::TopMask;
        using Base::ActualSize;

        class Reference {
        public:
            friend class Bitset;
            
            __WSTL_CONSTEXPR14__ Reference(const Reference& other) __WSTL_NOEXCEPT__ : m_Bitset(other.m_Bitset), m_Position(other.m_Position) {}

            __WSTL_CONSTEXPR14__ operator bool() const __WSTL_NOEXCEPT__ {
                return m_Bitset->Test(m_Position);
            }

            __WSTL_CONSTEXPR14__ Reference& operator=(const Reference& other) __WSTL_NOEXCEPT__ {
                m_Bitset->Set(m_Position, bool(other));
            }

            __WSTL_CONSTEXPR14__ Reference& operator=(bool value) __WSTL_NOEXCEPT__ {
                m_Bitset->Set(m_Position, value);
            }

            __WSTL_CONSTEXPR14__ bool operator~() const __WSTL_NOEXCEPT__ {
                return !m_Bitset->Test(m_Position);
            }

            __WSTL_CONSTEXPR14__ Reference& Flip() __WSTL_NOEXCEPT__ {
                m_Bitset->Flip(m_Position);
                return *this;
            }
            
        private:
            Bitset<N, ElementType>* m_Bitset;
            SizeType m_Position;

            __WSTL_CONSTEXPR14__ Reference() : m_Bitset(NullPointer), m_Position(0) {}

            __WSTL_CONSTEXPR14__ Reference(Bitset<N, ElementType>& bitset, SizeType position) : m_Bitset(bitset), m_Position(position) {}
        };

    
    private:
        ElementType m_Bits[NumberOfElements];
    };
}

#endif