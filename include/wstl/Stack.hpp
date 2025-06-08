#ifndef __WSTL_STACK_HPP__
#define __WSTL_STACK_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "private/Swap.hpp"
#include "private/Error.hpp"
#include "StandardExceptions.hpp"
#include "Iterator.hpp"


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

        Stack() : TypedContainerBase<T>(SIZE) {
            Initialize();
        }

        template<typename InputIterator>
        Stack(InputIterator first, InputIterator last) : TypedContainerBase<T>(SIZE) {
            for(; first != last; first++) ::new(&m_Buffer[this->m_CurrentSize++]) T(*first);
        }

        Stack(const Stack& other) : TypedContainerBase<T>(SIZE) {
            Clone(other);
        }

        #ifdef __WSTL_CXX11__
        Stack(Stack&& other) : TypedContainerBase<T>(SIZE) {
            Clone(Move(other));
        }
        #endif

        ~Stack() {
            Initialize();
        }

        Stack& operator=(const Stack& other) {
            Clone(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        Stack& operator=(Stack&& other) {
            Clone(Move(other));
            return *this;
        }
        #endif

        ReferenceType Top() {
            return m_Buffer[(this->m_CurrentSize > 0) ? this->m_CurrentSize - 1 : 0];
        }

        ConstReferenceType Top() const {
            return m_Buffer[(this->m_CurrentSize > 0) ? this->m_CurrentSize - 1 : 0];
        }

        void Push(ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Stack is full", __FILE__, __LINE__));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(value);
        }

        #ifdef __WSTL_CXX11__
        void Push(T&& value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Stack is full", __FILE__, __LINE__));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(value);
        }
        #endif

        template<typename Container>
        void PushRange(const Container& container) {
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Stack is full", __FILE__, __LINE__));

            for(SizeType i = 0; i < Size(container); i++) ::new(&m_Buffer[this->m_CurrentSize++]) T(*(Begin(container) + i));
        }

        #ifdef __WSTL_CXX11__
        template<typename Container>
        void PushRange(Container&& container) {
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Stack is full", __FILE__, __LINE__));

            for(SizeType i = 0; i < Size(container); i++) ::new(&m_Buffer[this->m_CurrentSize++]) T(Move(*(Begin(container) + i)));
        }
        #endif

        #ifdef __WSTL_CXX11__
        template<typename... Args>
        void Emplace(Args&&... args) {
            ::new(&m_Buffer[this->m_CurrentSize++]) T(Forward<Args>(args)...);
        }
        #else
        void Emplace() {
            ::new(&m_Buffer[this->m_CurrentSize++]) T();
        }

        template<typename Arg>
        void Emplace(const Arg& arg) {
            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg);
        }

        template<typename Arg1, typename Arg2>
        void Emplace(const Arg1& arg1, const Arg2& arg2) {
            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg1, arg2);
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        void Emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg1, arg2, arg3);
        }
        #endif

        void Pop() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Empty(), LengthError("Stack is empty", __FILE__, __LINE__));
            #endif

            m_Buffer[this->m_CurrentSize--].~T();
        }

        void Clear() {
            Initialize();
        }

        void Swap(Stack& other) {
            wstl::Swap(m_Buffer, other.m_Buffer);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
        }
        
    public: // for debug
        T m_Buffer[SIZE];

        void Clone(const Stack& other) {
            Initialize();
            for(SizeType i = 0; i < Size(other); i++) Push(other.m_Buffer[i]);
        }

        #ifdef __WSTL_CXX11__
        void Clone(Stack&& other) {
            Initialize();
            for(SizeType i = 0; i < Size(other); i++) Push(Move(other.m_Buffer[i]));
        }
        #endif

        template<typename U = T>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
        }

        template<typename U = T>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->Size() > 0) m_Buffer[this->m_CurrentSize--].~T();
        }
    };
}

#endif