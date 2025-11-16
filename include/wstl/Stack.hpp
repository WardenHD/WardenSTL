// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_STACK_HPP__
#define __WSTL_STACK_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "private/Swap.hpp"
#include "private/Error.hpp"
#include "StandardExceptions.hpp"
#include "Iterator.hpp"


/// @defgroup stack Stack
/// @ingroup containers
/// @brief A LIFO (Last In First Out) stack with size defined at compile-time

namespace wstl {
    /// @brief A fixed-size stack that supports pushing and popping elements in LIFO order
    /// @tparam T Type of the element to store in the stack
    /// @tparam SIZE The maximum number of elements the stack can hold
    /// @ingroup stack
    /// @see https://en.cppreference.com/w/cpp/container/stack
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

        /// @brief Default constructor
        Stack() : TypedContainerBase<T>(SIZE) {}

        /// @brief Constructor that initializes the stack with a range of elements
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        template<typename InputIterator>
        Stack(InputIterator first, InputIterator last) : TypedContainerBase<T>(SIZE) {
            for(; first != last; ++first) ::new(&m_Buffer[this->m_CurrentSize++]) T(*first);
        }

        /// @brief Copy constructor
        /// @param other The stack to copy from
        Stack(const Stack& other) : TypedContainerBase<T>(SIZE) {
            Clone(other);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The stack to move from
        Stack(Stack&& other) : TypedContainerBase<T>(SIZE) {
            Clone(Move(other));
        }
        
        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the stack with an initializer list
        /// @param list The initializer list to initialize the stack with
        Stack(InitializerList<T> list) : TypedContainerBase<T>(SIZE) {
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); ++it) 
                ::new(&m_Buffer[this->m_CurrentSize++]) T(__WSTL_MOVE__(*it));
        }
        #endif
        #endif

        /// @brief Destructor
        ~Stack() {
            Initialize();
        }

        /// @brief Copy assignment operator
        /// @param other The stack to copy from
        Stack& operator=(const Stack& other) {
            if(this != &other) Clone(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The stack to move from
        Stack& operator=(Stack&& other) {
            if(this != &other) Clone(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator 
        /// @param list The initializer list to assign to the stack
        /// @throws LengthError if list size exceeds the stack's capacity
        Stack& operator=(InitializerList<T> list) {
            __WSTL_ASSERT_RETURNVALUE__(list.Size() <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"), *this);
            
            Initialize();
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); ++it) 
                ::new(&m_Buffer[this->m_CurrentSize++]) T(__WSTL_MOVE__(*it));
            
            return *this;
        }
        #endif
        #endif

        /// @brief Returns a reference to the top element
        ReferenceType Top() {
            return m_Buffer[(this->m_CurrentSize > 0) ? this->m_CurrentSize - 1 : 0];
        }

        /// @brief Returns a const reference to the top element
        ConstReferenceType Top() const {
            return m_Buffer[(this->m_CurrentSize > 0) ? this->m_CurrentSize - 1 : 0];
        }

        /// @brief Pushes a new element onto the top of the stack
        /// @param value The value to push onto the stack
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void Push(ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes a new element onto the top of the stack
        /// @param value The value to push onto the stack (rvalue reference)
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void Push(T&& value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(Move(value));
        }
        #endif

        /// @brief Pushes a range of elements onto the stack
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Push(InputIterator first, InputIterator last) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(Distance(first, last) <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            for(; first != last; ++first) ::new(&m_Buffer[this->m_CurrentSize++]) T(__WSTL_MOVE__(*first));
        }

        /// @brief Pushes a specific number of elements onto the stack
        /// @param count The number of elements to push
        /// @param value The value to push onto the stack
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void Push(SizeType count, ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(count <= this->m_Capacity - this->m_CurrentSize, WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            for(SizeType i = 0; i < count; ++i) ::new(&m_Buffer[this->m_CurrentSize++]) T(value);
        }

        /// @brief Pushes elements from a container onto the stack
        /// @param container The container to push elements from
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Container>
        void PushRange(const Container& container) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            for(SizeType i = 0; i < Size(container); ++i) ::new(&m_Buffer[this->m_CurrentSize++]) T(*(Begin(container) + i));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes elements from a container onto the stack
        /// @param container The container to push elements from (rvalue reference)
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Container>
        void PushRange(Container&& container) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            for(SizeType i = 0; i < Size(container); ++i) ::new(&m_Buffer[this->m_CurrentSize++]) T(Move(*(Begin(container) + i)));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Constructs a new element in-place at the top of the stack
        /// @param ...args Arguments to forward to the constructor of the element
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined 
        template<typename... Args>
        void Emplace(Args&&... args) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(Forward<Args>(args)...);
        }
        #else
        /// @brief Constructs a new element in-place at the top of the stack
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        void Emplace() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T();
        }

        /// @brief Constructs a new element in-place at the top of the stack
        /// @param arg Argument to forward to the constructor of the element
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg>
        void Emplace(const Arg& arg) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg);
        }

        /// @brief Constructs a new element in-place at the top of the stack
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void Emplace(const Arg1& arg1, const Arg2& arg2) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg1, arg2);
        }

        /// @brief Constructs a new element in-place at the top of the stack
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws LengthError if the stack is full and `__WSTL_CHECK_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void Emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));
            #endif

            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg1, arg2, arg3);
        }
        #endif

        /// @brief Pops the top element from the stack
        /// @param count The number of elements to pop from the stack (default is 1)
        /// @throws LengthError if the stack is empty and `__WSTL_CHECK_PUSHPOP__` is defined
        void Pop(SizeType count = 1) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(count <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack is empty"));
            #endif

            while(count-- > 0) m_Buffer[this->m_CurrentSize--].~T();
        }

        /// @brief Reverses the order of elements in the stack
        void Reverse() {
            wstl::Reverse(m_Buffer, m_Buffer + this->m_CurrentSize);
        }

        /// @brief Clears the stack, calling destructors for all elements
        void Clear() {
            Initialize();
        }

        /// @brief Swaps content of two stacks, for trivial types
        /// @param other The stack to swap with
        template<typename U = T>
        typename EnableIf<IsTriviallyCopyable<U>::Value, void>::Type Swap(Stack& other) {
            wstl::Swap(m_Buffer, other.m_Buffer);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
        }

        /// @brief Swaps content of two stacks, for non-trivial types
        /// @param other The stack to swap with
        template<typename U = T>
        typename EnableIf<!IsTriviallyCopyable<U>::Value, void>::Type Swap(Stack& other) {
            SizeType minCount = Min(this->m_CurrentSize, other.m_CurrentSize);

            for(SizeType i = 0; i < minCount; ++i) {
                T temp = __WSTL_MOVE__(other.m_Buffer[i]);
                other.m_Buffer[i].~T();
                ::new(&other.m_Buffer[i]) T(__WSTL_MOVE__(m_Buffer[i]));
                m_Buffer[i].~T();
                ::new(&m_Buffer[i]) T(__WSTL_MOVE__(temp));
            }

            if(this->m_CurrentSize < other.m_CurrentSize) {
                for(SizeType i = this->m_CurrentSize; i < other.m_CurrentSize; ++i) {
                    ::new(&m_Buffer[i]) T(__WSTL_MOVE__(other.m_Buffer[i]));
                    other.m_Buffer[i].~T();
                }
            }
            else if(this->m_CurrentSize > other.m_CurrentSize) {
                for(SizeType i = other.m_CurrentSize; i < this->m_CurrentSize; ++i) {
                    ::new(&other.m_Buffer[i]) T(__WSTL_MOVE__(m_Buffer[i]));
                    m_Buffer[i].~T();
                }
            }

            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
        }
        
    private:
        T m_Buffer[SIZE];

        void Clone(const Stack& other) {
            Initialize();
            for(SizeType i = 0; i < Size(other); ++i) Push(other.m_Buffer[i]);
        }

        #ifdef __WSTL_CXX11__
        void Clone(Stack&& other) {
            Initialize();
            for(SizeType i = 0; i < Size(other); ++i) Push(Move(other.m_Buffer[i]));
        }
        #endif

        template<typename U = T>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
        }

        template<typename U = T>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->Size() - 1 > 0) m_Buffer[this->m_CurrentSize--].~T();
        }
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T, typename... U>
    Stack(T, U...) -> Stack<T, sizeof...(U) + 1>;
    #endif

    // Make stack

    #ifdef __WSTL_CXX11__
    /// @brief Makes a stack out of the given values
    /// @tparam T Type of the elements in the stack
    /// @param values Values to initialize the stack with
    /// @return A stack containing the given values
    /// @ingroup stack
    template<typename T, typename First, typename... Rest>
    constexpr auto MakeStack(First&& first, Rest&&... rest) {
        return Stack<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }

    /// @brief Makes a stack out of the given values
    /// @param values Values to initialize the stack with
    /// @return A stack containing the given values
    /// @ingroup stack
    template<typename First, typename... Rest>
    constexpr auto MakeStack(First&& first, Rest&&... rest) {
        using T = CommonTypeType<First, Rest...>;
        return Stack<T, sizeof...(rest) + 1>({ Forward<T>(first), Forward<T>(rest)... });
    }
    #endif
}

#endif