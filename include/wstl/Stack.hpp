#ifndef __WSTL_STACK_HPP__
#define __WSTL_STACK_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"


namespace wstl {
    template<typename T, const size_t SIZE>
    class Stack : public TypedContainerBase<T> {
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;

        Stack() : TypedContainerBase<T>(SIZE) {}

        template<typename InputIterator>
        Stack(InputIterator first, InputIterator last) : TypedContainerBase<T>(SIZE) {
            // push elements
        }

        Stack(const Stack& other) : TypedContainerBase<T>(SIZE) {
            // copy elements
        }

        #ifdef __WSTL_CXX11__
        Stack(Stack&& other) : TypedContainerBase<T>(SIZE) {
            // move elements
        }
        #endif

        ~Stack() {

        }

        Stack& operator=(const Stack& other) {
            // copy elements
        }

        #ifdef __WSTL_CXX11__
        Stack& operator=(Stack&& other) {
            // move elements
        }
        #endif

        ReferenceType Top() {
            return m_Buffer[(this->m_CurrentSize > 0) ? this->m_CurrentSize - 1 : 0];
        }

        ConstReferenceType Top() const {
            return m_Buffer[(this->m_CurrentSize > 0) ? this->m_CurrentSize - 1 : 0];
        }

        void Push(ConstReferenceType value) {
            
        }
        
    private:
        T m_Buffer[SIZE];
    };
}

#endif