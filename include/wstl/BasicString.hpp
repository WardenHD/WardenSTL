// #ifndef __WSTL_BASICSTRING_HPP__
// #define __WSTL_BASICSTRING_HPP__

// #include <stddef.h>


// namespace wstl {
//     template<typename T>
//     class BasicString {
//     public:
//         typedef BasicString<T> InterfaceType;

//         typedef T ValueType;
//         typedef T& Reference;
//         typedef const T& ConstReference;
//         typedef T* Pointer;
//         typedef const T* ConstPointer;
//         typedef T* Iterator;
//         typedef const T* ConstIterator;
//         typedef size_t SizeType;

//         Iterator Begin() {
//             return &m_Buffer[0];
//         }

//         ConstIterator ConstBegin() const {
//             return &m_Buffer[m_CurrentSize];
//         }

//         Iterator End() {
//             return &m_Buffer[m_CurrentSize];
//         }

//         ConstIterator ConstEnd() const {
//             return &m_Buffer[m_CurrentSize];
//         }

//         SizeType Size() const {
//             return m_CurrentSize;
//         }

//         SizeType Length() const {
//             return m_CurrentSize;
//         }

//         bool Empty() const {
//             return m_CurrentSize == 0;
//         }

//         bool Full() const {
//             return m_CurrentSize == m_Capacity;
//         }

//         size_t Capacity() const {
//             return m_Capacity;
//         }

//         size_t MaxSize() const {
//             return m_Capacity;
//         }

//         size_t Available() const {
//             return MaxSize() - Size();
//         }

//     protected:
//         BasicString(SizeType maxSize) : m_Capacity(maxSize), m_CurrentSize(0) {}
//         ~BasicString() {}

//     private:
//         const SizeType m_Capacity;
//         SizeType m_CurrentSize;
//         T* m_Buffer;
//     };
// };


// #endif
