// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_QUEUE_HPP__
#define __WSTL_QUEUE_HPP__

#include "private/Platform.hpp"
#include "Deque.hpp"
#include <stddef.h>


/// @defgroup queue Queue
/// @ingroup containers
/// @brief A FIFO (First In First Out) queue with size defined at compile-time

namespace wstl {
    // Queue adaptor

    /// @brief Queue adaptor that uses a specified container as the underlying storage
    /// @details Container is expected to support the following methods with usual semantics:
    /// - `Front()`
    /// - `Back()`
    /// - `PushBack()`
    /// - `PopFront()`
    ///
    /// @tparam Container The container type to use
    /// @ingroup queue
    /// @see https://en.cppreference.com/w/cpp/container/queue
    template<typename Container>
    class QueueAdaptor {
    public:
        typedef Container ContainerType;
        typedef typename Container::ValueType ValueType;
        typedef typename Container::SizeType SizeType;
        typedef typename Container::ReferenceType ReferenceType;
        typedef typename Container::ConstReferenceType ConstReferenceType;

        /// @brief Default constructor
        QueueAdaptor() : m_Container() {}

        /// @brief Constructor that initializes the queue with a given container
        /// @param container The container to initialize the queue with
        explicit QueueAdaptor(const Container& container) : m_Container(container) {}

        /// @brief Copy constructor
        /// @param other The queue to copy from
        QueueAdaptor(const QueueAdaptor& other) : m_Container(other.m_Container) {}

        #ifdef __WSTL_CXX11__
        /// @brief Constructor that initializes the queue with a given container
        /// @param container The container to initialize the queue with (rvalue reference)
        /// @since C++11
        explicit QueueAdaptor(Container&& container) : m_Container(Move(container)) {}

        /// @brief Move constructor
        /// @param other The queue to move from
        /// @since C++11
        QueueAdaptor(QueueAdaptor&& other) : m_Container(Move(other.m_Container)) {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param ...args Arguments to forward to the constructor of the container
        template<typename... Args>
        explicit QueueAdaptor(InPlaceType, Args&&... args) : m_Container(Forward<Args>(args)...) {}

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the queue with a given container with provided arguments
        // in-place and an initializer list
        /// @param list The initializer list to initialize the container with
        /// @param ...args Arguments to forward to the constructor of the container
        /// @since C++11
        template<typename U, typename... Args>
        explicit QueueAdaptor(InPlaceType, InitializerList<U> list, Args&&... args) : m_Container(list, Forward<Args>(args)...) {}
        #endif
        #else
        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        explicit QueueAdaptor(InPlaceType) : m_Container() {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param arg Argument to pass to the constructor of the container
        template<typename Arg>
        explicit QueueAdaptor(InPlaceType, const Arg& arg) : m_Container(arg) {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param arg1 First argument to pass to the constructor of the container
        /// @param arg2 Second argument to pass to the constructor of the container
        template<typename Arg1, typename Arg2>
        explicit QueueAdaptor(InPlaceType, const Arg1& arg1, const Arg2& arg2) : m_Container(arg1, arg2) {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param arg1 First argument to pass to the constructor of the container
        /// @param arg2 Second argument to pass to the constructor of the container
        /// @param arg3 Third argument to pass to the constructor of the container
        template<typename Arg1, typename Arg2, typename Arg3>
        explicit QueueAdaptor(InPlaceType, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : m_Container(arg1, arg2, arg3) {}
        #endif

        /// @brief Copy assignment operator
        /// @param other The queue to copy from
        QueueAdaptor& operator=(const QueueAdaptor& other) {
            m_Container = other.m_Container;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The queue to move from
        /// @since C++11
        QueueAdaptor& operator=(QueueAdaptor&& other) {
            m_Container = Move(other.m_Container);
            return *this;
        }
        #endif

        /// @brief Returns a reference to the front element that is to be removed
        ReferenceType Front() {
            return m_Container.Front();
        }

        /// @brief Returns a const reference to the front element that is to be removed
        ConstReferenceType Front() const {
            return m_Container.Front();
        }

        /// @brief Returns a reference to the back element
        ReferenceType Back() {
            return m_Container.Back();
        }

        /// @brief Returns a const reference to the back element
        ConstReferenceType Back() const {
            return m_Container.Back();
        }

        /// @brief Checks whether the queue is empty
        bool Empty() const {
            return m_Container.Empty();
        }

        /// @brief Returns the number of elements in the queue
        SizeType Size() const {
            return m_Container.Size();
        }

        /// @brief Pushes a new element to the back of the queue
        /// @param value The value to push onto the queue
        void Push(ConstReferenceType value) {
            m_Container.PushBack(value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes a new element to the back of the queue
        /// @param value The value to push onto the queue (rvalue reference)
        /// @since C++11
        void Push(ValueType&& value) {
            m_Container.PushBack(Move(value));
        }
        #endif

        /// @brief Pushes a range of elements to the queue
        /// @details Requires container to have `AppendRange` method
        /// @param range The range to push elements from
        template<typename Range>
        void PushRange(const Range& range) {
            m_Container.AppendRange(range);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes a range of elements to the queue
        /// @details Requires container to have `AppendRange` method
        /// @param range The range to push elements from (rvalue reference)
        /// @since C++11
        template<typename Range>
        void PushRange(Range&& range) {
            m_Container.AppendRange(Forward<Range>(range));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Constructs a new element in-place at the back of the queue
        /// @param ...args Arguments to forward to the constructor of the element
        /// @since C++11
        template<typename... Args>
        void Emplace(Args&&... args) {
            m_Container.EmplaceBack(Forward<Args>(args)...);
        }
        #else
        /// @brief Constructs a new element in-place at the back of the queue
        void Emplace() {
            m_Container.EmplaceBack();
        }

        /// @brief Constructs a new element in-place at the back of the queue
        /// @param arg Argument to forward to the constructor of the element
        template<typename Arg>
        void Emplace(const Arg& arg) {
            m_Container.EmplaceBack(arg);
        }

        /// @brief Constructs a new element in-place at the back of the queue
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        template<typename Arg1, typename Arg2>
        void Emplace(const Arg1& arg1, const Arg2& arg2) {
            m_Container.EmplaceBack(arg1, arg2);
        }

        /// @brief Constructs a new element in-place at the back of the queue
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        template<typename Arg1, typename Arg2, typename Arg3>
        void Emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            m_Container.EmplaceBack(arg1, arg2, arg3);
        }
        #endif

        /// @brief Pops the front element from the queue
        void Pop() {
            m_Container.PopFront();
        }

        /// @brief Clears the queue
        void Clear() {
            m_Container.Clear();
        }

        /// @brief Swaps content of two queues
        /// @param other The queue to swap with
        void Swap(QueueAdaptor& other) __WSTL_NOEXCEPT_EXPR__(noexcept(m_Container.Swap(other.m_Container))) {
            m_Container.Swap(other.m_Container);
        }
    
    protected:
        Container m_Container;

        template<typename UContainer>
        friend inline bool operator==(const QueueAdaptor<UContainer>&, const QueueAdaptor<UContainer>&);

        template<typename UContainer>
        friend inline bool operator<(const QueueAdaptor<UContainer>&, const QueueAdaptor<UContainer>&);
    };

    // Comparison operators

    template<typename Container>
    inline bool operator==(const QueueAdaptor<Container>& a, const QueueAdaptor<Container>& b) {
        return a.m_Container == b.m_Container;
    }

    template<typename Container>
    inline bool operator!=(const QueueAdaptor<Container>& a, const QueueAdaptor<Container>& b) {
        return !(a == b);
    }

    template<typename Container>
    inline bool operator<(const QueueAdaptor<Container>& a, const QueueAdaptor<Container>& b) {
        return a.m_Container < b.m_Container;
    }

    template<typename Container>
    inline bool operator<=(const QueueAdaptor<Container>& a, const QueueAdaptor<Container>& b) {
        return !(b < a);
    }

    template<typename Container>
    inline bool operator>(const QueueAdaptor<Container>& a, const QueueAdaptor<Container>& b) {
        return b < a;
    }

    template<typename Container>
    inline bool operator>=(const QueueAdaptor<Container>& a, const QueueAdaptor<Container>& b) {
        return !(a < b);
    }

    // Convenience wrappers

    /// @brief Small wrapper for `QueueAdaptor` that works with a fixed-size container
    /// @tparam T The type of elements stored in the queue
    /// @tparam N The maximum number of elements the queue can hold
    /// @tparam Container The container type to use (defaults to `Deque<T, N>`)
    /// @ingroup queue
    template<typename T, size_t N, typename Container = Deque<T, N>>
    class Queue : public QueueAdaptor<Container> {
    private:
        typedef QueueAdaptor<Container> Base;

    public:
        typedef typename Base::ContainerType ContainerType;
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;
        typedef typename Base::ReferenceType ReferenceType;
        typedef typename Base::ConstReferenceType ConstReferenceType;

        /// @brief The static size, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

        /// @brief Default constructor
        Queue() : Base() {}

        /// @brief Constructor that initializes the queue with a given container
        /// @param container The container to initialize the queue with
        explicit Queue(const Container& container) : Base(container) {}

        /// @brief Copy constructor
        /// @param other The queue to copy from
        Queue(const Queue& other) : Base(other.m_Container) {}

        #ifdef __WSTL_CXX11__
        /// @brief Constructor that initializes the queue with a given container
        /// @param container The container to initialize the queue with (rvalue reference)
        /// @since C++11
        explicit Queue(Container&& container) : Base(Move(container)) {}

        /// @brief Move constructor
        /// @param other The queue to move from
        /// @since C++11
        Queue(Queue&& other) : Base(Move(other.m_Container)) {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param ...args Arguments to forward to the constructor of the container
        template<typename... Args>
        explicit Queue(InPlaceType tag, Args&&... args) : Base(tag, Forward<Args>(args)...) {}

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the queue with a given container with provided arguments
        // in-place and an initializer list
        /// @param list The initializer list to initialize the container with
        /// @param ...args Arguments to forward to the constructor of the container
        /// @since C++11
        template<typename U, typename... Args>
        explicit Queue(InPlaceType tag, InitializerList<U> list, Args&&... args) : Base(tag, list, Forward<Args>(args)...) {}
        #endif
        #else
        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        explicit Queue(InPlaceType) : Base() {}
        
        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param arg Argument to forward to the constructor of the container
        template<typename Arg>
        explicit Queue(InPlaceType tag, const Arg& arg) : Base(tag) {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param arg1 First argument to forward to the constructor of the container
        /// @param arg2 Second argument to forward to the constructor of the container
        template<typename Arg1, typename Arg2>
        explicit Queue(InPlaceType tag, const Arg1& arg1, const Arg2& arg2) : Base(tag, arg1, arg2) {}

        /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
        /// @param arg1 First argument to forward to the constructor of the container
        /// @param arg2 Second argument to forward to the constructor of the container
        /// @param arg3 Third argument to forward to the constructor of the container
        template<typename Arg1, typename Arg2, typename Arg3>
        explicit Queue(InPlaceType tag, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : Base(tag, arg1, arg2, arg3) {}
        #endif

        /// @brief Copy assignment operator
        /// @param other The queue to copy from
        Queue& operator=(const Queue& other) {
            this->m_Container = other.m_Container;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The queue to move from
        /// @since C++11
        Queue& operator=(Queue&& other) {
            this->m_Container = Move(other.m_Container);
            return *this;
        }
        #endif
    };

    template<typename T, size_t N, typename Container>
    const __WSTL_CONSTEXPR__ typename Queue<T, N, Container>::SizeType Queue<T, N, Container>::StaticSize;

    namespace external {
        /// @brief Small wrapper for `QueueAdaptor` that works with a container with external storage
        /// @tparam T The type of elements stored in the queue
        /// @tparam Container The type of the underlying container to use (defaults to `Deque<T>`)
        /// @ingroup queue
        template<typename T, typename Container = Deque<T>>
        class Queue : public QueueAdaptor<Container> {
        private:
            typedef QueueAdaptor<Container> Base;

        public:
            typedef typename Base::ContainerType ContainerType;
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;

            /// @brief Constructor that initializes the queue with an external buffer
            /// @param buffer Pointer to the external buffer
            /// @param capacity Capacity of the external buffer
            Queue(T* buffer, SizeType capacity) : Base(InPlaceType(), buffer, capacity) {}

            /// @brief Constructor that initializes the queue with a given container
            /// @param container The container to initialize the queue with
            explicit Queue(const Container& container) : Base(container) {}

            /// @brief Copy constructor
            /// @param other The queue to copy from
            Queue(const Queue& other) : Base(other.m_Container) {}

            #ifdef __WSTL_CXX11__
            /// @brief Constructor that initializes the queue with a given container
            /// @param container The container to initialize the queue with (rvalue reference)
            /// @since C++11
            explicit Queue(Container&& container) : Base(Move(container)) {}

            /// @brief Move constructor
            /// @param other The queue to move from
            /// @since C++11
            Queue(Queue&& other) : Base(Move(other.m_Container)) {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param ...args Arguments to forward to the constructor of the container
            template<typename... Args>
            explicit Queue(InPlaceType tag, Args&&... args) : Base(tag, Forward<Args>(args)...) {}

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Constructor that initializes the queue with a given container with provided arguments
            // in-place and an initializer list
            /// @param list The initializer list to initialize the container with
            /// @param ...args Arguments to forward to the constructor of the container
            /// @since C++11
            template<typename U, typename... Args>
            explicit Queue(InPlaceType tag, InitializerList<U> list, Args&&... args) : Base(tag, list, Forward<Args>(args)...) {}
            #endif
            #else
            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            explicit Queue(InPlaceType) : Base() {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param arg Argument to forward to the constructor of the container
            template<typename Arg>
            explicit Queue(InPlaceType tag, const Arg& arg) : Base(tag) {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param arg1 First argument to forward to the constructor of the container
            /// @param arg2 Second argument to forward to the constructor of the container
            template<typename Arg1, typename Arg2>
            explicit Queue(InPlaceType tag, const Arg1& arg1, const Arg2& arg2) : Base(tag, arg1, arg2) {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param arg1 First argument to forward to the constructor of the container
            /// @param arg2 Second argument to forward to the constructor of the container
            /// @param arg3 Third argument to forward to the constructor of the container
            template<typename Arg1, typename Arg2, typename Arg3>
            explicit Queue(InPlaceType tag, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : Base(tag, arg1, arg2, arg3) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The queue to copy from
            Queue& operator=(const Queue& other) {
                this->m_Container = other.m_Container;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The queue to move from
            /// @since C++11
            Queue& operator=(Queue&& other) {
                this->m_Container = Move(other.m_Container);
                return *this;
            }
            #endif
        };

        /// @brief Small wrapper for `QueueAdaptor` that works with a fixed-size container with external storage
        /// @tparam T The type of elements stored in the queue
        /// @tparam N The maximum number of elements the queue can hold
        /// @tparam Container The type of the underlying container to use (defaults to `FixedDeque<T, N>`)
        /// @ingroup queue
        template<typename T, size_t N, typename Container = FixedDeque<T, N>>
        class FixedQueue : public QueueAdaptor<Container> {
        private:
            typedef QueueAdaptor<Container> Base;

        public:
            typedef typename Base::ContainerType ContainerType;
            typedef typename Base::ValueType ValueType;
            typedef typename Base::SizeType SizeType;
            typedef typename Base::ReferenceType ReferenceType;
            typedef typename Base::ConstReferenceType ConstReferenceType;

            /// @brief The static size, needed for metaprogramming
            static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

            /// @brief Constructor that initializes the queue with an external buffer
            /// @param buffer Pointer to the external buffer
            FixedQueue(T* buffer) : Base(InPlaceType(), buffer) {}

            /// @brief Constructor that initializes the queue with a given container
            /// @param container The container to initialize the queue with
            explicit FixedQueue(const Container& container) : Base(container) {}

            /// @brief Copy constructor
            /// @param other The queue to copy from
            FixedQueue(const FixedQueue& other) : Base(other.m_Container) {}

            #ifdef __WSTL_CXX11__
            /// @brief Constructor that initializes the queue with a given container
            /// @param container The container to initialize the queue with (rvalue reference)
            /// @since C++11
            explicit FixedQueue(Container&& container) : Base(Move(container)) {}

            /// @brief Move constructor
            /// @param other The queue to move from
            /// @since C++11
            FixedQueue(FixedQueue&& other) : Base(Move(other.m_Container)) {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param ...args Arguments to forward to the constructor of the container
            template<typename... Args>
            explicit FixedQueue(InPlaceType tag, Args&&... args) : Base(tag, Forward<Args>(args)...) {}

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Constructor that initializes the queue with a given container with provided arguments
            // in-place and an initializer list
            /// @param list The initializer list to initialize the container with
            /// @param ...args Arguments to forward to the constructor of the container
            /// @since C++11
            template<typename U, typename... Args>
            explicit FixedQueue(InPlaceType tag, InitializerList<U> list, Args&&... args) : Base(tag, list, Forward<Args>(args)...) {}
            #endif
            #else
            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            explicit FixedQueue(InPlaceType) : Base() {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param arg Argument to forward to the constructor of the container
            template<typename Arg>
            explicit FixedQueue(InPlaceType tag, const Arg& arg) : Base(tag) {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param arg1 First argument to forward to the constructor of the container
            /// @param arg2 Second argument to forward to the constructor of the container
            template<typename Arg1, typename Arg2>
            explicit FixedQueue(InPlaceType tag, const Arg1& arg1, const Arg2& arg2) : Base(tag, arg1, arg2) {}

            /// @brief Constructor that initializes the queue with a given container with provided arguments in-place
            /// @param arg1 First argument to forward to the constructor of the container
            /// @param arg2 Second argument to forward to the constructor of the container
            /// @param arg3 Third argument to forward to the constructor of the container
            template<typename Arg1, typename Arg2, typename Arg3>
            explicit FixedQueue(InPlaceType tag, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) : Base(tag, arg1, arg2, arg3) {}
            #endif

            /// @brief Copy assignment operator
            /// @param other The queue to copy from
            FixedQueue& operator=(const FixedQueue& other) {
                this->m_Container = other.m_Container;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The queue to move from
            /// @since C++11
            FixedQueue& operator=(FixedQueue&& other) {
                this->m_Container = Move(other.m_Container);
                return *this;
            }
            #endif
        };

        template<typename T, size_t N, typename Container>
        const __WSTL_CONSTEXPR__ typename FixedQueue<T, N, Container>::SizeType FixedQueue<T, N, Container>::StaticSize;
    }
}

#endif