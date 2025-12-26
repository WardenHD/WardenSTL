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
    /// @tparam N The maximum number of elements the stack can hold
    /// @ingroup stack
    /// @see https://en.cppreference.com/w/cpp/container/stack
    template<typename T, size_t N>
    class Stack : public TypedContainerBase<T> {
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;

        /// @brief The static size, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

        /// @brief Default constructor
        Stack() : TypedContainerBase<T>(N) {}

        /// @brief Constructor that initializes the stack with a range of elements
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        template<typename InputIterator>
        Stack(InputIterator first, InputIterator last) : TypedContainerBase<T>(N) {
            for(; first != last; ++first) ::new(&m_Buffer[this->m_CurrentSize++]) T(*first);
        }

        /// @brief Copy constructor
        /// @param other The stack to copy from
        Stack(const Stack& other) : TypedContainerBase<T>(N) {
            Clone(other);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The stack to move from
        /// @since C++11
        Stack(Stack&& other) : TypedContainerBase<T>(N) {
            Clone(Move(other));
        }
        
        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the stack with an initializer list
        /// @param list The initializer list to initialize the stack with
        /// @since C++11
        Stack(InitializerList<T> list) : TypedContainerBase<T>(N) {
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); ++it) 
                ::new(&m_Buffer[this->m_CurrentSize++]) T(*it);
        }
        #endif
        #endif

        /// @brief Destructor
        ~Stack() {
            Initialize<T>();
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
        /// @since C++11
        Stack& operator=(Stack&& other) {
            if(this != &other) Clone(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator 
        /// @param list The initializer list to assign to the stack
        /// @throws `LengthError` if list size exceeds the stack's capacity
        /// @since C++11
        Stack& operator=(InitializerList<T> list) {
            __WSTL_ASSERT_RETURNVALUE__(list.Size() <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"), *this);
            
            Initialize<T>();
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); ++it) 
                ::new(&m_Buffer[this->m_CurrentSize++]) T(*it);
            
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
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void Push(ConstReferenceType value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes a new element onto the top of the stack
        /// @param value The value to push onto the stack (rvalue reference)
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        void Push(ValueType&& value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T(Move(value));
        }
        #endif

        /// @brief Pushes a range of elements onto the stack
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Push(InputIterator first, InputIterator last) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(Distance(first, last) <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            for(; first != last; ++first) ::new(&m_Buffer[this->m_CurrentSize++]) T(*first);
        }

        /// @brief Pushes elements from a range onto the stack
        /// @param range The range to push elements from
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Range>
        void PushRange(const Range& range) {
            Push(Begin(range), End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes elements onto the stack by moving them from range
        /// @param range The range to move elements from (rvalue reference)
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename Range>
        void PushRange(Range&& range) {
            Push(MakeMoveIterator(Begin(range)), MakeMoveIterator(End(range)));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Constructs a new element in-place at the top of the stack
        /// @param ...args Arguments to forward to the constructor of the element
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename... Args>
        void Emplace(Args&&... args) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T(Forward<Args>(args)...);
        }
        #else
        /// @brief Constructs a new element in-place at the top of the stack
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void Emplace() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T();
        }

        /// @brief Constructs a new element in-place at the top of the stack
        /// @param arg Argument to forward to the constructor of the element
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg>
        void Emplace(const Arg& arg) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg);
        }

        /// @brief Constructs a new element in-place at the top of the stack
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void Emplace(const Arg1& arg1, const Arg2& arg2) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg1, arg2);
        }

        /// @brief Constructs a new element in-place at the top of the stack
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws `LengthError` if the stack is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void Emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is full"));

            ::new(&m_Buffer[this->m_CurrentSize++]) T(arg1, arg2, arg3);
        }
        #endif

        /// @brief Pops the top element from the stack
        /// @throws `LengthError` if the stack is empty and `__WSTL_ASSERT_PUSHPOP__` is defined
        void Pop() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "Stack is empty"));

            m_Buffer[this->m_CurrentSize--].~T();
        }

        /// @brief Reverses the order of elements in the stack
        void Reverse() {
            wstl::Reverse(m_Buffer, m_Buffer + this->m_CurrentSize);
        }

        /// @brief Clears the stack, calling destructors for all elements
        void Clear() {
            Initialize<T>();
        }

        /// @brief Swaps content of two stacks
        /// @param other The stack to swap with
        void Swap(Stack& other) {
            __Swap<T>(other);
        }
        
    private:
        T m_Buffer[N];

        /// @brief Clones another stack into this one
        /// @param other The stack to clone from
        void Clone(const Stack& other) {
            Initialize<T>();
            for(SizeType i = 0; i < other.Size(); ++i) Push(other.m_Buffer[i]);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Clones another stack into this one (rvalue reference)
        /// @param other The stack to clone from
        /// @since C++11
        void Clone(Stack&& other) {
            Initialize<T>();
            for(SizeType i = 0; i < other.Size(); ++i) Push(Move(other.m_Buffer[i]));
        }
        #endif

        /// @brief Initializes the stack, trivial version
        template<typename U>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
        }

        /// @brief Initializes the stack, non-trivial version: calls destructors
        template<typename U>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->Size() - 1 > 0) m_Buffer[this->m_CurrentSize--].~T();
        }

        /// @brief Swaps content of two stacks, trivial version
        /// @param other The stack to swap with
        template<typename U>
        typename EnableIf<IsTriviallyCopyable<U>::Value, void>::Type __Swap(Stack& other) {
            wstl::Swap(m_Buffer, other.m_Buffer);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);

        }

        /// @brief Swaps content of two stacks, non-trivial version: constructs/destroys elements one by one
        /// @param other The stack to swap with
        template<typename U>
        typename EnableIf<!IsTriviallyCopyable<U>::Value, void>::Type __Swap(Stack& other) {
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
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename Stack<T, N>::SizeType Stack<T, N>::StaticSize;

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
    /// @since C++11
    template<typename T, typename First, typename... Rest>
    constexpr auto MakeStack(First&& first, Rest&&... rest) {
        return Stack<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }

    /// @brief Makes a stack out of the given values
    /// @param values Values to initialize the stack with
    /// @return A stack containing the given values
    /// @ingroup stack
    /// @since C++11
    template<typename First, typename... Rest>
    constexpr auto MakeStack(First&& first, Rest&&... rest) {
        using T = CommonTypeType<First, Rest...>;
        return Stack<T, sizeof...(rest) + 1>({ Forward<T>(first), Forward<T>(rest)... });
    }
    #endif

    // Stack adaptor
    
    namespace adaptor {
        /// @brief Stack adaptor that uses a specified container as the underlying storage
        /// @details Container is expected to support the following methods with usual semantics:
        /// - `Back()`
        /// - `PushBack()`
        /// - `PopBack()`
        ///
        /// @tparam T Type of the element to store in the stack
        /// @tparam N The maximum number of elements the stack can hold
        /// @tparam Container The container type to use as the underlying storage (default is `Deque`)
        /// @ingroup stack
        /// @see https://en.cppreference.com/w/cpp/container/stack
        template<typename T, size_t N, typename Container = Deque<T, N> >
        class Stack {
        public:
            typedef Container ContainerType;
            typedef typename Container::ValueType ValueType;
            typedef typename Container::SizeType SizeType;
            typedef typename Container::ReferenceType ReferenceType;
            typedef typename Container::ConstReferenceType ConstReferenceType;

            /// @brief The static size of the stack, needed for metaprogramming
            static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

            /// @brief Default constructor
            Stack() : m_Container() {}
            
            /// @brief Constructor that initializes the stack with a given container
            /// @param container The container to use as the underlying storage
            explicit Stack(const Container& container) : m_Container(container) {}

            /// @brief Copy constructor
            /// @param other The stack to copy from
            Stack(const Stack& other) : m_Container(other.m_Container) {}

            #ifdef __WSTL_CXX11__
            /// @brief Constructor that initializes the stack with a given container (rvalue reference)
            /// @param container The container to use as the underlying storage
            /// @since C++11
            explicit Stack(Container&& container) : m_Container(Move(container)) {}

            /// @brief Move constructor
            /// @param other The stack to move from
            /// @since C++11
            Stack(Stack&& other) : m_Container(Move(other.m_Container)) {}

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Constructor that initializes the stack with an initializer list
            /// @param list The initializer list to initialize the stack with
            /// @since C++11
            Stack(InitializerList<T> list) : m_Container(list) {}
            #endif
            #endif
            
            /// @brief Constructor that initializes the stack with a range of elements
            /// @param first Iterator to the first element in the range
            /// @param last Iterator to the element following the last element in the range
            template<typename InputIterator>
            Stack(InputIterator first, InputIterator last) : m_Container(first, last) {}

            /// @brief Copy assignment operator
            /// @param other The stack to copy from
            Stack& operator=(const Stack& other) {
                m_Container = other.m_Container;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The stack to move from
            /// @since C++11
            Stack& operator=(Stack&& other) {
                m_Container = Move(other.m_Container);
                return *this;
            }

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Assignment operator
            /// @details Requires container to have `Assign` method for initializer list
            /// @param list The initializer list to assign to the stack
            /// @since C++11
            Stack& operator=(InitializerList<T> list) {
                m_Container.Assign(list);
                return *this;
            }
            #endif
            #endif

            /// @brief Returns a reference to the top element
            ReferenceType Top() {
                return m_Container.Back();
            }

            /// @brief Returns a const reference to the top element
            ConstReferenceType Top() const {
                return m_Container.Back();
            }

            /// @brief Checks if the stack is empty
            bool Empty() const {
                return m_Container.Empty();
            }

            /// @brief Returns the number of elements in the stack
            SizeType Size() const {
                return m_Container.Size();
            }

            /// @brief Pushes a new element onto the top of the stack
            /// @param value The value to push onto the stack
            void Push(ConstReferenceType value) {
                m_Container.PushBack(value);
            }

            #ifdef __WSTL_CXX11__
            /// @brief Pushes a new element onto the top of the stack
            /// @param value The value to push onto the stack (rvalue reference)
            /// @since C++11
            void Push(ValueType&& value) {
                m_Container.PushBack(Move(value));
            }
            #endif

            /// @brief Pushes a range of elements onto the stack
            /// @details Requires container to have `AppendRange` method
            /// @param range The range to push elements from
            template<typename Range>
            void PushRange(const Range& range) {
                m_Container.AppendRange(range);
            }

            #ifdef __WSTL_CXX11__
            /// @brief Pushes a range of elements onto the stack
            /// @details Requires container to have `AppendRange` method
            /// @param range The range to push elements from (rvalue reference)
            /// @since C++11
            template<typename Range>
            void PushRange(Range&& range) {
                m_Container.AppendRange(Forward<Range>(range));
            }
            #endif

            #ifdef __WSTL_CXX11__
            /// @brief Constructs a new element in-place at the top of the stack
            /// @param ...args Arguments to forward to the constructor of the element
            /// @since C++11
            template<typename... Args>
            void Emplace(Args&&... args) {
                m_Container.EmplaceBack(Forward<Args>(args)...);
            }
            #else
            /// @brief Constructs a new element in-place at the top of the stack
            void Emplace() {
                m_Container.EmplaceBack();
            }

            /// @brief Constructs a new element in-place at the top of the stack
            /// @param arg Argument to forward to the constructor of the element
            template<typename Arg>
            void Emplace(const Arg& arg) {
                m_Container.EmplaceBack(arg);
            }

            /// @brief Constructs a new element in-place at the top of the stack
            /// @param arg1 First argument to pass to the constructor of the element
            /// @param arg2 Second argument to pass to the constructor of the element
            template<typename Arg1, typename Arg2>
            void Emplace(const Arg1& arg1, const Arg2& arg2) {
                m_Container.EmplaceBack(arg1, arg2);
            }

            /// @brief Constructs a new element in-place at the top of the stack
            /// @param arg1 First argument to pass to the constructor of the element
            /// @param arg2 Second argument to pass to the constructor of the element
            /// @param arg3 Third argument to pass to the constructor of the element
            template<typename Arg1, typename Arg2, typename Arg3>
            void Emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
                m_Container.EmplaceBack(arg1, arg2, arg3);
            }
            #endif

            /// @brief Pops the top element from the stack
            void Pop() {
                m_Container.PopBack();
            }

            /// @brief Reverses the order of elements in the stack
            void Reverse() {
                wstl::Reverse(m_Container.Begin(), m_Container.End());
            }

            /// @brief Clears the stack
            void Clear() {
                m_Container.Clear();
            }

            /// @brief Swaps content of two stacks
            /// @param other The stack to swap with
            void Swap(Stack& other) __WSTL_NOEXCEPT_EXPR__(noexcept(m_Container.Swap(other.m_Container))) {
                m_Container.Swap(other.m_Container);
            }

            private:
                Container m_Container;
        };

        template<typename T, size_t N, typename Container>
        const __WSTL_CONSTEXPR__ typename Stack<T, N, Container>::SizeType Stack<T, N, Container>::StaticSize;

        // Template deduction guide

        #ifdef __WSTL_CXX17__
        template<typename T, typename... U>
        Stack(T, U...) -> Stack<T, sizeof...(U) + 1>;

        template<typename Container>
        Stack(Container&&) -> Stack<typename RemoveReferenceType<Container>::ValueType, RemoveReferenceType<Container>::StaticSize, RemoveReferenceType<Container>>;
        #endif
    }
}

#endif