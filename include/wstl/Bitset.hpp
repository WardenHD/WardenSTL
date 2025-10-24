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
            static const __WSTL_CONSTEXPR__ SizeType BitsPerElement = sizeof(ElementType) * 8;
            static const __WSTL_CONSTEXPR__ ElementType AllSet = NumericLimits<ElementType>::Max();
            static const __WSTL_CONSTEXPR__ ElementType AllClear = ElementType(0);

        private:
            static const __WSTL_CONSTEXPR__ SizeType NumberOfElements = (N % BitsPerElement == 0) ? N / BitsPerElement : N / BitsPerElement + 1; 
            static const __WSTL_CONSTEXPR__ SizeType TopBits = N % BitsPerElement;
            static const __WSTL_CONSTEXPR__ ElementType TopMask = (TopBits == 0) ? AllSet : ~(AllSet << TopBits);
            static const __WSTL_CONSTEXPR__ SizeType ActualSize = NumberOfElements * BitsPerElement;

        public:
            typedef Span<T, NumberOfElements> SpanType;
            typedef Span<const T, NumberOfElements> ConstSpanType;
        };

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::NoPosition;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::BitsPerElement;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::ElementType __BitsetCommon<N, T>::AllSet;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::ElementType __BitsetCommon<N, T>::AllClear;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::NumberOfElements;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::TopBits;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::ElementType __BitsetCommon<N, T>::TopMask;

        template<size_t N, typename T>
        const __WSTL_CONSTEXPR__ __BitsetCommon<N, T>::SizeType __BitsetCommon<N, T>::ActualSize;
    }

    template<size_t N, typename T>
    class Bitset {
    public:
        
    };
}

#endif