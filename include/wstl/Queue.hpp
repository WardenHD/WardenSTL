// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_QUEUE_HPP__
#define __WSTL_QUEUE_HPP__

#include "private/Platform.hpp"
#include "private/Swap.hpp"
#include "private/Error.hpp"
#include "StandardExceptions.hpp"
#include "Container.hpp"
#include <stddef.h>


/// @defgroup queue Queue
/// @ingroup containers
/// @brief A FIFO (First In First Out) queue with size defined at compile-time

namespace wstl {
    /// @brief A fixed-size queue that supports pushing and popping elements in FIFO order
    /// @tparam T Type of the element to store in the queue
    /// @tparam N The maximum number of elements the queue can hold
    /// @ingroup queue
    /// @see https://en.cppreference.com/w/cpp/container/queue
    template<typename T, size_t N>
    class Queue : public TypedContainerBase<T> {
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
        Queue() : TypedContainerBase<T>(N), m_StartIndex(0) {}

        /// @brief Constructor that initializes the queue with a range of elements
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        template<typename InputIterator>
        Queue(InputIterator first, InputIterator last) : TypedContainerBase<T>(N), m_StartIndex(0) {
            for(; first != last; ++first) ::new(&m_Buffer[this->m_CurrentSize++]) T(*first);
        }

        /// @brief Copy constructor
        /// @param other The queue to copy from
        Queue(const Queue& other) : TypedContainerBase<T>(N), m_StartIndex(0) {
            Clone(other);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The queue to move from
        /// @since C++11
        Queue(Queue&& other) : TypedContainerBase<T>(N), m_StartIndex(0) {
            Clone(Move(other));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor that initializes the queue with an initializer list
        /// @param list The initializer list to initialize the queue with
        /// @throws `LengthError` if list size exceeds the queue's capacity
        /// @since C++11
        Queue(InitializerList<T> list) : TypedContainerBase<T>(N), m_StartIndex(0) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(list.Size() <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack capacity exceeded"));

            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); ++it) 
                ::new(&m_Buffer[this->m_CurrentSize++]) T(*it);
        }
        #endif
        #endif

        /// @brief Destructor
        ~Queue() {
            Initialize<T>();
        }

        /// @brief Copy assignment operator
        /// @param other The queue to copy from
        Queue& operator=(const Queue& other) {
            if(this != &other) Clone(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The queue to move from
        /// @since C++11
        Queue& operator=(Queue&& other) {
            if(this != &other) Clone(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator 
        /// @param list The initializer list to assign to the queue
        /// @throws `LengthError` if list size exceeds the queue's capacity
        /// @since C++11
        Queue& operator=(InitializerList<T> list) {
            __WSTL_ASSERT_RETURNVALUE__(list.Size() <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Stack capacity exceeded"), *this);
            
            Initialize<T>();
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); ++it) 
                ::new(&m_Buffer[this->m_CurrentSize++]) T(*it);
            
            return *this;
        }
        #endif
        #endif

        /// @brief Gets reference to the first element in the queue that is to be removed
        ReferenceType Front() {
            return m_Buffer[m_StartIndex];
        }

        /// @brief Gets const reference to the first element in the queue that is to be removed
        ConstReferenceType Front() const {
            return m_Buffer[m_StartIndex];
        }

        /// @brief Gets reference to the last element in the queue that was added
        ReferenceType Back() {
            return m_Buffer[PhysicalIndex(this->m_CurrentSize - 1)];
        }

        /// @brief Gets const reference to the last element in the queue that was added
        ConstReferenceType Back() const {
            return m_Buffer[PhysicalIndex(this->m_CurrentSize - 1)];
        }

        /// @brief Pushes a new element to the back of the queue
        /// @param value The value to push onto the queue
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void Push(ConstReferenceType value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(value);
            ++this->m_CurrentSize;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes a new element to the back of the queue
        /// @param value The value to push onto the queue (rvalue reference)
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        void Push(ValueType&& value) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(Move(value));
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Pushes a range of elements to the queue
        /// @param first Iterator to the first element in the range
        /// @param last Iterator to the element following the last element in the range
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Push(InputIterator first, InputIterator last) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(Distance(first, last) <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            for(; first != last; ++first) {
                ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(*first);
                ++this->m_CurrentSize;
            }
        }

        /// @brief Pushes elements from a range to the queue
        /// @param range The range to push elements from
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Range>
        void PushRange(const Range& range) {
            Push(Begin(range), End(range));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Pushes elements to the queue by moving them from range
        /// @param range The range to move elements from (rvalue reference)
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11
        template<typename Range>
        void PushRange(Range&& range) {
            Push(MakeMoveIterator(Begin(range)), MakeMoveIterator(End(range)));
        }
        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Constructs a new element in-place at the back of the queue
        /// @param ...args Arguments to forward to the constructor of the element
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        /// @since C++11 
        template<typename... Args>
        void Emplace(Args&&... args) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(Forward<Args>(args)...);
            ++this->m_CurrentSize;
        }
        #else
        /// @brief Constructs a new element in-place at the back of the queue
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        void Emplace() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T();
            ++this->m_CurrentSize;
        }

        /// @brief Constructs a new element in-place at the back of the queue
        /// @param arg Argument to forward to the constructor of the element
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg>
        void Emplace(const Arg& arg) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(arg);
            ++this->m_CurrentSize;
        }

        /// @brief Constructs a new element in-place at the back of the queue
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2>
        void Emplace(const Arg1& arg1, const Arg2& arg2) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(arg1, arg2);
            ++this->m_CurrentSize;
        }

        /// @brief Constructs a new element in-place at the back of the queue
        /// @param arg1 First argument to pass to the constructor of the element
        /// @param arg2 Second argument to pass to the constructor of the element
        /// @param arg3 Third argument to pass to the constructor of the element
        /// @throws `LengthError` if the queue is full and `__WSTL_ASSERT_PUSHPOP__` is defined
        template<typename Arg1, typename Arg2, typename Arg3>
        void Emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Full(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is full"));

            ::new(&m_Buffer[PhysicalIndex(this->m_CurrentSize)]) T(arg1, arg2, arg3);
            ++this->m_CurrentSize;
        }
        #endif

        /// @brief Pops the front element from the queue
        /// @throws LengthError if the queue is empty and `__WSTL_ASSERT_PUSHPOP__` is defined
        void Pop() {
            __WSTL_ASSERT_PUSHPOP_RETURN__(!this->Empty(), WSTL_MAKE_EXCEPTION(LengthError, "Queue is empty"));

            m_Buffer[m_StartIndex].~T();
            m_StartIndex = (m_StartIndex + 1) % this->m_Capacity;
            --this->m_CurrentSize;
        }

        /// @brief Clears the queue, calling destructors for all elements
        void Clear() {
            Initialize<T>();
        }

        /// @brief Swaps content of two queues
        /// @param other The queue to swap with
        void Swap(Queue& other) {
            __Swap<T>(other);
        }
    
    private:
        T m_Buffer[N];
        SizeType m_StartIndex;

        /// @brief Clones another queue into this one
        /// @param other The queue to clone from
        void Clone(const Queue& other) {
            Initialize<T>();
            for(SizeType i = 0; i < other.Size(); ++i) Push(other.m_Buffer[other.PhysicalIndex(i)]);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Clones another queue into this one by moving its elements
        /// @param other The queue to clone from (rvalue reference)
        /// @since C++11
        void Clone(Queue&& other) {
            Initialize<T>();
            for(SizeType i = 0; i < other.Size(); ++i) Push(Move(other.m_Buffer[other.PhysicalIndex(i)]));
        }
        #endif

        /// @brief Converts a logical index to a physical index in the buffer
        /// @param index The logical index
        /// @return The physical index in the buffer
        SizeType PhysicalIndex(SizeType index) const {
            return (m_StartIndex + index) % this->m_Capacity;
        }

        /// @brief Initializes the queue, trivial version: does not call destructors
        template<typename U>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
            m_StartIndex = 0;
        }

        /// @brief Initializes the queue, non-trivial version: calls destructors
        template<typename U>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->Size() - 1 > 0) {
                m_Buffer[PhysicalIndex(this->m_CurrentSize)].~T();
                --this->m_CurrentSize;
            }

            m_StartIndex = 0;
        }

        /// @brief Swaps content of two queues, trivial version
        /// @param other The queue to swap with
        template<typename U>
        typename EnableIf<IsTriviallyCopyable<U>::Value, void>::Type __Swap(Queue& other) {
            wstl::Swap(m_Buffer, other.m_Buffer);
            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
            wstl::Swap(this->m_StartIndex, other.m_StartIndex);
        }

        /// @brief Swaps content of two queues, non-trivial version: constructs/destroys elements one by one
        /// @param other The queue to swap with
        template<typename U>
        typename EnableIf<!IsTriviallyCopyable<U>::Value, void>::Type __Swap(Queue& other) {
            SizeType minCount = Min(this->m_CurrentSize, other.m_CurrentSize);

            for(SizeType i = 0; i < minCount; ++i) {
                SizeType a = PhysicalIndex(i);
                SizeType b = other.PhysicalIndex(i);

                T temp = __WSTL_MOVE__(other.m_Buffer[b]);
                other.m_Buffer[b].~T();

                ::new(&other.m_Buffer[b]) T(__WSTL_MOVE__(m_Buffer[a]));
                m_Buffer[a].~T();

                ::new(&m_Buffer[a]) T(__WSTL_MOVE__(temp));
            }

            if(this->m_CurrentSize < other.m_CurrentSize) {
                for(SizeType i = this->m_CurrentSize; i < other.m_CurrentSize; ++i) {
                    SizeType a = PhysicalIndex(i);
                    SizeType b = other.PhysicalIndex(i);
                    
                    ::new(&m_Buffer[a]) T(__WSTL_MOVE__(other.m_Buffer[b]));
                    other.m_Buffer[b].~T();
                }
            }
            else if(this->m_CurrentSize > other.m_CurrentSize) {
                for(SizeType i = other.m_CurrentSize; i < this->m_CurrentSize; ++i) {
                    SizeType a = PhysicalIndex(i);
                    SizeType b = other.PhysicalIndex(i);

                    ::new(&other.m_Buffer[b]) T(__WSTL_MOVE__(m_Buffer[a]));
                    m_Buffer[a].~T();
                }
            }

            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
        }
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename Queue<T, N>::SizeType Queue<T, N>::StaticSize;

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T, typename... U>
    Queue(T, U...) -> Queue<T, sizeof...(U) + 1>;
    #endif

    // Make queue

    #ifdef __WSTL_CXX11__
    /// @brief Makes a queue out of the given values
    /// @tparam T Type of the elements in the queue
    /// @param values Values to initialize the queue with
    /// @return A queue containing the given values
    /// @ingroup queue
    /// @since C++11
    template<typename T, typename First, typename... Rest>
    constexpr auto MakeQueue(First&& first, Rest&&... rest) {
        return Queue<T, sizeof...(rest) + 1>({ Forward<First>(first), Forward<Rest>(rest)... });
    }

    /// @brief Makes a queue out of the given values
    /// @param values Values to initialize the queue with
    /// @return A queue containing the given values
    /// @ingroup queue
    /// @since C++11
    template<typename First, typename... Rest>
    constexpr auto MakeQueue(First&& first, Rest&&... rest) {
        using T = CommonTypeType<First, Rest...>;
        return Queue<T, sizeof...(rest) + 1>({ Forward<T>(first), Forward<T>(rest)... });
    }
    #endif

    // Queue adaptor

    namespace adaptor {
        /// @brief Queue adaptor that uses a specified container as the underlying storage
        /// @details Container is expected to support the following methods with usual semantics:
        /// - `Front()`
        /// - `Back()`
        /// - `PushBack()`
        /// - `PopFront()`
        ///
        /// @tparam T Type of the element to store in the queue
        /// @tparam N The maximum number of elements the queue can hold
        /// @tparam Container The container type to use as the underlying storage (default is `Deque`)
        /// @ingroup queue
        /// @see https://en.cppreference.com/w/cpp/container/queue
        template<typename T, size_t N, typename Container = Deque<T, N> >
        class Queue {
        public:
            typedef Container ContainerType;
            typedef typename Container::ValueType ValueType;
            typedef typename Container::SizeType SizeType;
            typedef typename Container::ReferenceType ReferenceType;
            typedef typename Container::ConstReferenceType ConstReferenceType;

            /// @brief The static size of the stack, needed for metaprogramming
            static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;

            /// @brief Default constructor
            Queue() : m_Container() {}

            /// @brief Constructor that initializes the queue with a given container
            /// @param container The container to initialize the queue with
            explicit Queue(const Container& container) : m_Container(container) {}

            /// @brief Copy constructor
            /// @param other The queue to copy from
            Queue(const Queue& other) : m_Container(other.m_Container) {}

            #ifdef __WSTL_CXX11__
            /// @brief Constructor that initializes the queue with a given container
            /// @param container The container to initialize the queue with (rvalue reference)
            /// @since C++11
            explicit Queue(Container&& container) : m_Container(Move(container)) {}

            /// @brief Move constructor
            /// @param other The queue to move from
            /// @since C++11
            Queue(Queue&& other) : m_Container(Move(other)) {}

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Constructor that initializes the queue with an initializer list
            /// @param list The initializer list to initialize the queue with
            /// @since C++11
            Queue(InitializerList<T> list) : m_Container(list) {}
            #endif
            #endif

            /// @brief Constructor that initializes the queue with a range of elements
            /// @param first Iterator to the first element in the range
            /// @param last Iterator to the element following the last element in the range
            template<typename InputIterator>
            Queue(InputIterator first, InputIterator last) : m_Container(first, last) {}

            /// @brief Copy assignment operator
            /// @param other The queue to copy from
            Queue& operator=(const Queue& other) {
                m_Container = other.m_Container;
                return *this;
            }

            #ifdef __WSTL_CXX11__
            /// @brief Move assignment operator
            /// @param other The queue to move from
            /// @since C++11
            Queue& operator=(Queue&& other) {
                m_Container = Move(other.m_Container);
                return *this;
            }

            #ifndef __WSTL_NO_INITIALIZERLIST__
            /// @brief Assignment operator
            /// @param list The initializer list to assign to the queue
            /// @since C++11
            Queue& operator=(InitializerList<T> list) {
                m_Container.Assign(list);
                return *this;
            }
            #endif
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
            void Swap(Queue& other) __WSTL_NOEXCEPT_EXPR__(noexcept(m_Container.Swap(other.m_Container))) {
                m_Container.Swap(other.m_Container);
            }
        
        private:
            Container m_Container;
        };

        template<typename T, size_t N, typename Container>
        const __WSTL_CONSTEXPR__ typename Queue<T, N, Container>::SizeType Queue<T, N, Container>::StaticSize;

        // Template deduction guide

        #ifdef __WSTL_CXX17__
        template<typename T, typename... U>
        Queue(T, U...) -> Queue<T, sizeof...(U) + 1>;

        template<typename Container>
        Queue(Container&&) -> Queue<typename RemoveReferenceType<Container>::ValueType, RemoveReferenceType<Container>::StaticSize, RemoveReferenceType<Container>>;
        #endif
    }
}

#endif