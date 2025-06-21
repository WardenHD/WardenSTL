#ifndef __WSTL_POOL_HPP__
#define __WSTL_POOL_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "PlacementNew.hpp"
#include "StandardExceptions.hpp"
#include "NullPointer.hpp"
#include "BoundedIterator.hpp"
#include "private/Error.hpp"
#include "private/Swap.hpp"
#include <stddef.h>



namespace wstl {
    template<typename T, const size_t SIZE>
    class Pool : public TypedContainerBase<T> {
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;

        typedef ForwardBoundedIterator<T*> Iterator;
        typedef ForwardBoundedIterator<const T*> ConstIterator;

        Pool() : TypedContainerBase<T>(SIZE), m_Next(m_Buffer) {
            for (size_t i = 0; i < this->m_Capacity - 1; ++i) *reinterpret_cast<T**>(m_Buffer + i) = m_Buffer + i + 1;
            *reinterpret_cast<T**>(m_Buffer + this->m_Capacity - 1) = NullPointer; // last points to null
        }

        Iterator Begin() {
            return Iterator(m_Buffer, m_Buffer + this->m_Capacity);
        }

        ConstIterator Begin() const {
            return ConstIterator(m_Buffer, m_Buffer + this->m_Capacity);
        }

        ConstIterator ConstBegin() const {
            return ConstIterator(m_Buffer, m_Buffer + this->m_Capacity);
        }

        Iterator End() {
            return Iterator(m_Buffer + this->m_Capacity, m_Buffer + this->m_Capacity);
        }

        ConstIterator End() const {
            return ConstIterator(m_Buffer + this->m_Capacity, m_Buffer + this->m_Capacity);
        }

        ConstIterator ConstEnd() const {
            return ConstIterator(m_Buffer + this->m_Capacity, m_Buffer + this->m_Capacity);
        }

        T* Allocate() {
            __WSTL_ASSERT_RETURNVALUE__(m_Next != NullPointer, LengthError("Pool is full"), NullPointer);

            T* result = m_Next;
            m_Next = *reinterpret_cast<T**>(m_Next);
            ++m_CurrentSize;

            return result;
        }

        #ifdef __WSTL_CXX11__
        template<typename... Args>
        T* Create(Args&&... args) {
            T* item = Allocate();
            if(item) ::new(item) T(Forward<Args>(args)...);
            return item;
        }
        #else
        T* Create() {
            T* item = Allocate();
            if(item) ::new(item) T();
            return item;
        }

        template<typename Arg>
        T* Create(const Arg& arg) {
            T* item = Allocate();
            if(item) ::new(item) T(arg);
            return item;
        }

        template<typename Arg1, typename Arg2>
        T* Create(const Arg1& arg1, const Arg2& arg2) {
            T* item = Allocate();
            if(item) ::new(item) T(arg1, arg2);
            return item;
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        T* Create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            T* item = Allocate();
            if(item) ::new(item) T(arg1, arg2, arg3);
            return item;
        }
        #endif

        void Release(const T* const value) {
            __WSTL_ASSERT_RETURN__(Contains(value), OutOfRange("Pointer not in the pool range"));
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Pool is empty"));
            
            --this->m_CurrentSize;
            *reinterpret_cast<T**>(const_cast<T*>(value)) = m_Next;
            m_Next = const_cast<T*>(value);
        }

        void Destroy(const T* const value) {
            value->~T();
            Release(value);
        }

        void Clear() {
            for(T* i = m_Buffer; i < m_Buffer + this->Capacity - 1; ++i) Destroy(i);
        }

        bool Contains(const T* const value) const {
            return (value - m_Buffer) >= 0 && (value - m_Buffer) <= this->m_Capacity;
        }
    
    private:
        T m_Buffer[SIZE];
        T* m_Next;
    };
}

#endif