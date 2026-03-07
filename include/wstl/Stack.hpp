// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_STACK_HPP__
#define __WSTL_STACK_HPP__

#include "private/Platform.hpp"
#include "Deque.hpp"


/// @defgroup stack Stack
/// @ingroup containers
/// @brief A LIFO (Last In First Out) stack container adaptor

namespace wstl {
    // Stack adaptor

    /// @brief Stack adaptor that uses a specified container as the underlying storage
    /// @details Container is expected to support the following methods with usual semantics:
    /// - `Front()`
    /// - `Back()`
    /// - `PushBack()`
    /// - `PopFront()`
    ///
    /// @tparam Container The type of the underlying container to use
    /// @ingroup stack
    /// @see https://en.cppreference.com/w/cpp/container/stack
    template<typename Container>
    class StackAdaptor {
    public:
        typedef Container ContainerType;
        typedef typename Container::ValueType ValueType;
        typedef typename Container::SizeType SizeType;
        typedef typename Container::ReferenceType ReferenceType;
        typedef typename Container::ConstReferenceType ConstReferenceType;

        /// @brief Default constructor
        StackAdaptor() : m_Container() {}

        /// @brief Constructor that initializes the stack with a given container
        /// @param container The container to use as the underlying storage
        explicit StackAdaptor(const Container& container) : m_Container(container) {}

        /// @brief Copy constructor
        /// @param other The stack to copy from
        StackAdaptor(const StackAdaptor& other) : m_Container(other.m_Container) {}

        #ifdef __WSTL_CXX11__
        /// @brief Constructor that initializes the stack with a given container (rvalue reference)
        /// @param container The container to use as the underlying storage
        /// @since C++11
        explicit StackAdaptor(Container&& container) : m_Container(Move(container)) {}

        /// @brief Move constructor
        /// @param other The stack to move from
        /// @since C++11
        StackAdaptor(StackAdaptor&& other) : m_Container(Move(other.m_Container)) {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param ...args Arguments to forward to the constructor of the container
        template<typename... Args>
        explicit StackAdaptor(InPlaceType, Args&&... args) : m_Container(Forward<Args>(args)...) {}
        
        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the stack with a given container with provided arguments 
        // in-place and an initializer list
        /// @param list The initializer list to initialize the container with
        /// @param ...args Arguments to forward to the constructor of the container
        template<typename U, typename... Args>
        explicit StackAdaptor(InPlaceType, InitializerList<U> list, Args&&... args) : m_Container(list, Forward<Args>(args)...) {}
        #endif
        #else
        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        explicit StackAdaptor(InPlaceType) : m_Container() {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param arg Argument to forward to the constructor of the container
        template<typename Arg>
        explicit StackAdaptor(InPlaceType, const Arg& arg) : m_Container(arg) {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param arg1 First argument to pass to the constructor of the container
        /// @param arg2 Second argument to pass to the constructor of the container
        template<typename Arg1, typename Arg2>
        explicit StackAdaptor(InPlaceType, const Arg1& arg1, const Arg2& arg2) : m_Container(arg1, arg2) {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param arg1 First argument to pass to the constructor of the container
        /// @param arg2 Second argument to pass to the constructor of the container
        /// @param arg3 Third argument to pass to the constructor of the container
        template<typename Arg1, typename Arg2, typename Arg3>
        explicit StackAdaptor(InPlaceType, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : m_Container(arg1, arg2, arg3) {}
        #endif

        /// @brief Copy assignment operator
        /// @param other The stack to copy from
        StackAdaptor& operator=(const StackAdaptor& other) {
            m_Container = other.m_Container;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The stack to move from
        /// @since C++11
        StackAdaptor& operator=(StackAdaptor&& other) {
            m_Container = Move(other.m_Container);
            return *this;
        }
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
        void Swap(StackAdaptor& other) __WSTL_NOEXCEPT_EXPR__(noexcept(m_Container.Swap(other.m_Container))) {
            m_Container.Swap(other.m_Container);
        }

    protected:
        Container m_Container;
    };

    // Convenience wrappers

    /// @brief Small wrapper for `StackAdaptor` that works with a fixed-size container
    /// @tparam T The type of elements stored in the stack
    /// @tparam N The maximum number of elements the stack can hold
    /// @tparam Container The type of the underlying container to use (defaults to `Deque<T, N>`)
    /// @ingroup stack
    template<typename T, size_t N, typename Container = Deque<T, N> >
    class Stack : public StackAdaptor<Container> {
    private:
        typedef StackAdaptor<Container> Base;

    public:
        typedef typename Base::ContainerType ContainerType;
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;

        /// @brief The static size, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

        /// @brief Default constructor
        Stack() : Base() {}
        
        /// @brief Constructor that initializes the stack with a given container
        /// @param container The container to use as the underlying storage
        explicit Stack(const Container& container) : Base(container) {}

        /// @brief Copy constructor
        /// @param other The stack to copy from
        Stack(const Stack& other) : Base(other) {}

        #ifdef __WSTL_CXX11__
        /// @brief Constructor that initializes the stack with a given container (rvalue reference)
        /// @param container The container to use as the underlying storage
        /// @since C++11
        explicit Stack(Container&& container) : Base(Move(container)) {}

        /// @brief Move constructor
        /// @param other The stack to move from
        /// @since C++11
        Stack(Stack&& other) : Base(Move(other)) {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param ...args Arguments to forward to the constructor of the container
        template<typename... Args>
        explicit Stack(InPlaceType tag, Args&&... args) : Base(tag, Forward<Args>(args)...) {}

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the stack with a given container with provided arguments 
        // in-place and an initializer list
        /// @param list The initializer list to initialize the container with
        /// @param ...args Arguments to forward to the constructor of the container
        /// @since C++11
        template<typename U, typename... Args>
        explicit Stack(InPlaceType tag, InitializerList<U> list, Args&&... args) : Base(tag, list, Forward<Args>(args)...) {}
        #endif
        #else
        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        explicit Stack(InPlaceType) : Base() {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param arg Argument to forward to the constructor of the container
        template<typename Arg>
        explicit Stack(InPlaceType tag, const Arg& arg) : Base(tag, arg) {}

        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param arg1 First argument to pass to the constructor of the container
        /// @param arg2 Second argument to pass to the constructor of the container
        template<typename Arg1, typename Arg2>
        explicit Stack(InPlaceType tag, const Arg1& arg1, const Arg2& arg2) : Base(tag, arg1, arg2) {}
        
        /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
        /// @param arg1 First argument to pass to the constructor of the container
        /// @param arg2 Second argument to pass to the constructor of the container
        /// @param arg3 Third argument to pass to the constructor of the container
        template<typename Arg1, typename Arg2, typename Arg3>
        explicit Stack(InPlaceType tag, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : Base(tag, arg1, arg2, arg3) {}
        #endif
        
        /// @brief Copy assignment operator
        /// @param other The stack to copy from
        Stack& operator=(const Stack& other) {
            this->m_Container = other.m_Container;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The stack to move from
        /// @since C++11
        Stack& operator=(Stack&& other) {
            this->m_Container = Move(other.m_Container);
            return *this;
        }
        #endif
    };

    namespace external {
        /// @brief Small wrapper for `StackAdaptor` that works with a container with external storage
        /// @tparam T The type of elements stored in the stack
        /// @tparam Container The type of the underlying container to use (defaults to `external::Deque<T>`)
        /// @ingroup stack
        template<typename T, typename Container = Deque<T> >
        class Stack : public StackAdaptor<Container> {
        private:
            typedef StackAdaptor<Container> Base;

        public:
            typedef typename Base::ContainerType ContainerType;
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;

            /// @brief Constructor that initializes the stack with an external buffer
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            Stack(T* buffer, SizeType capacity) : Base(InPlaceType(), buffer, capacity) {}
            
            /// @brief Constructor that initializes the stack with a given container
            /// @param container The container to use as the underlying storage
            explicit Stack(const Container& container) : Base(container) {}

            /// @brief Copy constructor
            /// @param other The stack to copy from
            Stack(const Stack& other) : Base(other) {}

            #ifdef __WSTL_CXX11__
            /// @brief Constructor that initializes the stack with a given container (rvalue reference)
            /// @param container The container to use as the underlying storage
            /// @since C++11
            explicit Stack(Container&& container) : Base(Move(container)) {}

            /// @brief Move constructor
            /// @param other The stack to move from
            /// @since C++11
            Stack(Stack&& other) : Base(Move(other)) {}

            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param ...args Arguments to forward to the constructor of the container
            template<typename... Args>
            explicit Stack(InPlaceType tag, Args&&... args) : Base(tag, Forward<Args>(args)...) {}

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Constructor that initializes the stack with a given container with provided arguments 
            // in-place and an initializer list
            /// @param list The initializer list to initialize the container with
            /// @param ...args Arguments to forward to the constructor of the container
            /// @since C++11
            template<typename U, typename... Args>
            explicit Stack(InPlaceType tag, InitializerList<U> list, Args&&... args) : Base(tag, list, Forward<Args>(args)...) {}
            #endif
            #else
            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            explicit Stack(InPlaceType) : Base() {}

            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param arg Argument to forward to the constructor of the container
            template<typename Arg>
            explicit Stack(InPlaceType tag, const Arg& arg) : Base(tag, arg) {}

            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param arg1 First argument to pass to the constructor of the container
            /// @param arg2 Second argument to pass to the constructor of the container
            template<typename Arg1, typename Arg2>
            explicit Stack(InPlaceType tag, const Arg1& arg1, const Arg2& arg2) : Base(tag, arg1, arg2) {}
            
            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param arg1 First argument to pass to the constructor of the container
            /// @param arg2 Second argument to pass to the constructor of the container
            /// @param arg3 Third argument to pass to the constructor of the container
            template<typename Arg1, typename Arg2, typename Arg3>
            explicit Stack(InPlaceType tag, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : Base(tag, arg1, arg2, arg3) {}
            #endif
            
            /// @brief Copy assignment operator
            /// @param other The stack to copy from
            Stack& operator=(const Stack& other) {
                this->m_Container = other.m_Container;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The stack to move from
            /// @since C++11
            Stack& operator=(Stack&& other) {
                this->m_Container = Move(other.m_Container);
                return *this;
            }
            #endif
        };

        /// @brief Small wrapper for `StackAdaptor` that works with a fixed-size container with external storage
        /// @tparam T The type of elements stored in the stack
        /// @tparam N The maximum number of elements the stack can hold
        /// @tparam Container The type of the underlying container to use (defaults to `external::FixedDeque<T, N>`)
        /// @ingroup stack
        template<typename T, size_t N, typename Container = FixedDeque<T, N> >
        class FixedStack : public StackAdaptor<Container> {
        private:
            typedef StackAdaptor<Container> Base;
            
        public:
            typedef typename Base::ContainerType ContainerType;
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;

            /// @brief The static size, needed for metaprogramming
            static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

            /// @brief Constructor that initializes the stack with an external buffer
            /// @param buffer Pointer to the external buffer
            FixedStack(T* buffer) : Base(InPlaceType(), buffer) {}
            
            /// @brief Constructor that initializes the stack with a given container
            /// @param container The container to use as the underlying storage
            explicit FixedStack(const Container& container) : Base(container) {}

            /// @brief Copy constructor
            /// @param other The stack to copy from
            FixedStack(const FixedStack& other) : Base(other.m_Container) {}

            #ifdef __WSTL_CXX11__
            /// @brief Constructor that initializes the stack with a given container (rvalue reference)
            /// @param container The container to use as the underlying storage
            /// @since C++11
            explicit FixedStack(Container&& container) : Base(Move(container)) {}

            /// @brief Move constructor
            /// @param other The stack to move from
            /// @since C++11
            FixedStack(FixedStack&& other) : Base(Move(other.m_Container)) {}

            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param ...args Arguments to forward to the constructor of the container
            template<typename... Args>
            explicit FixedStack(InPlaceType tag, Args&&... args) : Base(tag, Forward<Args>(args)...) {}

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Constructor that initializes the stack with a given container with provided arguments
            // in-place and an initializer list
            /// @param list The initializer list to initialize the container with
            /// @param ...args Arguments to forward to the constructor of the container
            template<typename U, typename... Args>
            explicit FixedStack(InPlaceType tag, InitializerList<U> list, Args&&... args) : Base(tag, list, Forward<Args>(args)...) {}
            #endif
            #else
            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            explicit FixedStack(InPlaceType) : Base() {}

            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param arg Argument to forward to the constructor of the container
            template<typename Arg>
            explicit FixedStack(InPlaceType tag, const Arg& arg) : Base(tag, arg) {}

            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param arg1 First argument to pass to the constructor of the container
            /// @param arg2 Second argument to pass to the constructor of the container
            template<typename Arg1, typename Arg2>
            explicit FixedStack(InPlaceType tag, const Arg1& arg1, const Arg2& arg2) : Base(tag, arg1, arg2) {}
            
            /// @brief Constructor that initializes the stack with a given container with provided arguments in-place
            /// @param arg1 First argument to pass to the constructor of the container
            /// @param arg2 Second argument to pass to the constructor of the container
            /// @param arg3 Third argument to pass to the constructor of the container
            template<typename Arg1, typename Arg2, typename Arg3>
            explicit FixedStack(InPlaceType tag, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : Base(tag, arg1, arg2, arg3) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The stack to copy from
            FixedStack& operator=(const FixedStack& other) {
                this->m_Container = other.m_Container;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The stack to move from
            /// @since C++11
            FixedStack& operator=(FixedStack&& other) {
                this->m_Container = Move(other.m_Container);
                return *this;
            }
            #endif
        };

        template<typename T, size_t N, typename Container>
        const __WSTL_CONSTEXPR__ typename FixedStack<T, N, Container>::SizeType FixedStack<T, N, Container>::StaticSize;
    }
}

#endif