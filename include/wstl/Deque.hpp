// #ifndef __WSTL_DEQUE_HPP__
// #define __WSTL_DEQUE_HPP__

// #include "private/Platform.hpp"
// #include "Iterator.hpp"
// #include "CircularIterator.hpp"
// #include <stddef.h>


// namespace wstl {
//     template<typename T>
//     class IDeque {
//     public:
//         typedef T ValueType;
//         typedef size_t SizeType;
//         typedef ptrdiff_t DifferenceType;
//         typedef T& ReferenceType;
//         typedef const T& ConstReferenceType;
//         typedef T* PointerType;
//         typedef const T* ConstPointerType;
        
//         typedef CircularIterator<T*> Iterator;
//         typedef CircularIterator<const T*> ConstIterator;
//         typedef wstl::ReverseIterator<Iterator> ReverseIterator;
//         typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

//         SizeType Size() const {
//             return m_CurrentSize;
//         }

//         bool Empty() const {
//             return m_CurrentSize == 0;
//         }

//         SizeType MaxSize() const {
//             return m_Capacity;
//         }

//         bool Full() const {
//             return m_CurrentSize == m_Capacity;
//         }

//         SizeType Capacity() const {
//             return m_Capacity;
//         }

//         SizeType Available() const {
//             return MaxSize() - Size();
//         }



//     protected:
//         IDeque(SizeType capacity) : m_Capacity(capacity), m_CurrentSize(0), m_StartIndex(0) {}

//         ~IDeque();

//     private:
//         SizeType m_CurrentSize;
//         const SizeType m_Capacity;
//         SizeType m_StartIndex;
//     };

//     template<typename T, const size_t SIZE>
//     class Deque {
    
//     };
// }

// #endif