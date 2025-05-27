#ifndef __WSTL_DEQUE_HPP__
#define __WSTL_DEQUE_HPP__

#include "private/Platform.hpp"
#include "Iterator.hpp"
#include "CircularIterator.hpp"
#include "StandardExceptions.hpp"
#include "PlacementNew.hpp"
#include <stddef.h>


namespace wstl {
    // Deque interface

    template<typename T>
    class IDeque {
    public:
        typedef T ValueType;
        typedef size_t SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;

        SizeType Size() const {
            return m_CurrentSize;
        }

        bool Empty() const {
            return m_CurrentSize == 0;
        }

        SizeType MaxSize() const {
            return m_Capacity;
        }

        bool Full() const {
            return m_CurrentSize == m_Capacity;
        }

        SizeType Capacity() const {
            return m_Capacity;
        }

        SizeType Available() const {
            return MaxSize() - Size();
        }

    protected:
        IDeque(SizeType capacity) : m_Capacity(capacity), m_CurrentSize(0) {}

        ~IDeque();

        SizeType m_CurrentSize;
        const SizeType m_Capacity;
    };


    template<typename T, const size_t SIZE>
    class Deque : public IDeque<T> {
    public:
        typedef typename IDeque<T>::ValueType ValueType;
        typedef typename IDeque<T>::SizeType SizeType;
        typedef typename IDeque<T>::DifferenceType DifferenceType;
        typedef typename IDeque<T>::ReferenceType ReferenceType;
        typedef typename IDeque<T>::ConstReferenceType ConstReferenceType;
        typedef typename IDeque<T>::PointerType PointerType;
        typedef typename IDeque<T>::ConstPointerType ConstPointerType;

        typedef CircularIterator<T*> Iterator;
        typedef CircularIterator<const T*> ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        Deque() : IDeque<T>(SIZE), m_StartIndex(0) {
            Initialize();
        }

        // Deque(const Deque& other) : IDeque<T>(other.SIZE), m_StartIndex(0) {
        //     Initialize();
        //     // Copy elements
        // }

        // #ifdef __WSTL_CXX11__
        // Deque(Deque&& other) : IDeque<T>(other.SIZE), m_StartIndex(0) {
        //     Initialize();
        //     // Move elements
        // }
        // #endif

        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, void>::Type Assign(InputIterator first, InputIterator last) {
            Initialize();
            for(; first != last; ++first) PushBack(*first);
        }

        void Assign(SizeType count, ConstReferenceType value) {
            __WSTL_ASSERT_RETURN__(count <= this->m_Capacity, LengthError("Deque is full", __FILE__, __LINE__));
            Initialize();
            for(; count > 0; count--) PushBack(value);
        }
        
        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        void Assign(InitializerList<T> list) {
            Initialize();
            for(T* iterator = const_cast<T*>(list.Begin()); iterator != list.End(); iterator++) PushBack(*iterator);
        }
        #endif

        ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position < this->m_CurrentSize, OutOfRange("Deque index out of range", __FILE__, __LINE__));
            return m_Buffer[(this->m_StartIndex + position) % this->m_Capacity];
        }

        ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position < this->m_CurrentSize, OutOfRange("Deque index out of range", __FILE__, __LINE__));
            return m_Buffer[(this->m_StartIndex + position) % this->m_Capacity];
        }

        ReferenceType operator[](SizeType index) {
            return m_Buffer[(this->m_StartIndex + index) % this->m_Capacity];
        }

        ConstReferenceType operator[](SizeType index) const {
            return m_Buffer[(this->m_StartIndex + index) % this->m_Capacity];
        }

        ReferenceType Front() {
            return m_Buffer[this->m_StartIndex];
        }

        ConstReferenceType Front() const {
            return m_Buffer[this->m_StartIndex];
        }

        ReferenceType Back() {
            return m_Buffer[(this->m_StartIndex + this->m_CurrentSize - 1) % this->m_Capacity];
        }

        ConstReferenceType Back() const {
            return m_Buffer[(this->m_StartIndex + this->m_CurrentSize - 1) % this->m_Capacity];
        }

        Iterator Begin() {
            return Iterator(wstl::Begin(m_Buffer), wstl::End(m_Buffer), wstl::Begin(m_Buffer) + this->m_StartIndex);
        }

        ConstIterator Begin() const {
            return ConstIterator(wstl::Begin(m_Buffer), wstl::End(m_Buffer), wstl::Begin(m_Buffer) + this->m_StartIndex);
        }

        ConstIterator ConstBegin() const {
            return ConstIterator(wstl::ConstBegin(m_Buffer), wstl::ConstEnd(m_Buffer), wstl::ConstBegin(m_Buffer) + this->m_StartIndex);
        }

        Iterator End() {
            return Iterator(wstl::Begin(m_Buffer), wstl::End(m_Buffer), wstl::Begin(m_Buffer) + ((this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity));
        }

        ConstIterator End() const {
            return ConstIterator(wstl::Begin(m_Buffer), wstl::End(m_Buffer), wstl::Begin(m_Buffer) + ((this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity));
        }

        ConstIterator ConstEnd() const {
            return ConstIterator(wstl::ConstBegin(m_Buffer), wstl::ConstEnd(m_Buffer), wstl::ConstBegin(m_Buffer) + ((this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity));
        }

        ReverseIterator ReverseBegin() {
            return ReverseIterator(End());
        }

        ConstReverseIterator ReverseBegin() const {
            return ConstReverseIterator(End());
        }

        ConstReverseIterator ConstReverseBegin() const {
            return ConstReverseIterator(End());
        }

        ReverseIterator ReverseEnd() {
            return ReverseIterator(Begin());
        }

        ConstReverseIterator ReverseEnd() const {
            return ConstReverseIterator(Begin());
        }

        ConstReverseIterator ConstReverseEnd() const {
            return ConstReverseIterator(Begin());
        }

        void Clear() {
            Initialize();
        }

        Iterator Insert(ConstIterator position, ConstReferenceType value) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);
            
            if(pos == Begin()) CreateFront(value);
            else if(pos == End()) CreateBack(value);
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());
                    *--pos = value;
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    *pos = value;
                }
            }

            return pos;
        }

        #ifdef __WSTL_CXX11__
        Iterator Insert(ConstIterator position, T&& value) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            if(pos == Begin()) CreateFront(Move(value));
            else if(pos == End()) CreateBack(Move(value));
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());
                    pos--;
                    *pos = Move(value);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    *pos = Move(value);
                }
            }

            return pos;
        }
        #endif

        Iterator Insert(ConstIterator position, SizeType count, ConstReferenceType value) {
            Iterator pos;
            Iterator it = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(count <= this->m_Capacity - this->m_CurrentSize, LengthError("Deque is full", __FILE__, __LINE__), pos);

            if(position == Begin()) {
                for(SizeType i = 0; i < count; i++) CreateFront(value);
                pos = Begin();
            }
            else if(position == End()) {
                for(SizeType i = 0; i < count; i++) CreateBack(value);
                pos = End() - count;
            }
            else {
                pos = Iterator(wstl::Begin(m_Buffer), wstl::End(m_Buffer), it);

                const SizeType distanceFront = Distance(Begin(), it);
                const SizeType distanceBack = Distance(it, End());

                if(distanceFront < distanceBack) {
                    // Shift towards front
                    SizeType moveCount = distanceFront;
                    SizeType copyCount = Min(count, moveCount);
                    SizeType newCount = count - copyCount;

                    Iterator moveFrom = Begin() + copyCount - 1;

                    for(SizeType i = 0; i < newCount; i++) CreateFront(value);

                    for(SizeType i = 0; i < copyCount; i++) CreateFront(*moveFrom--);

                    Iterator moveSource = pos - (moveCount - copyCount);
                    Iterator moveDestination = Begin() + copyCount;

                    Move(moveSource, moveSource + (moveCount - copyCount), moveDestination);
                    FillInRange(pos - copyCount, count - newCount, value);

                    pos = Begin() + distanceFront;
                }
                else {
                    // Shift towards back
                    SizeType moveCount = distanceBack;
                    SizeType copyCount = Min(count, moveCount);
                    SizeType newCount = count - copyCount;

                    Iterator moveFrom = End() - copyCount;

                    for(SizeType i = 0; i < newCount; i++) CreateBack(value);

                    for(SizeType i = 0; i < copyCount; i++) CreateBack(*moveFrom++);

                    MoveBackward(pos, pos + (moveCount - copyCount), pos + count + (moveCount - copyCount));
                    FillInRange(pos, count - newCount, value);
                }
            }

            return pos;
        }

        template<typename Iterator>
        typename EnableIf<!IsIntegral<Iterator>::Value, typename Deque::Iterator>::Type
        Insert(ConstIterator position, Iterator first, Iterator last) {
            typename Deque::Iterator pos;
            typename Deque::Iterator it = Begin() + Distance(ConstBegin(), position);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(count <= this->m_Capacity - this->m_CurrentSize, LengthError("Deque is full", __FILE__, __LINE__), pos);

            if(position == Begin()) {
                CreateFront(first, count);
                pos = Begin();
            }
            else if(position == End()) {
                for(; first != last; first++) CreateBack(*first);
                pos = End() - count;
            }
            else {
                pos = typename Deque::Iterator(wstl::Begin(m_Buffer), wstl::End(m_Buffer), it);

                const SizeType distanceFront = Distance(Begin(), it);
                const SizeType distanceBack = Distance(it, End());

                if(distanceFront < distanceBack) {
                    // Shift towards front
                    SizeType moveCount = distanceFront;
                    SizeType copyCount = Min(count, moveCount);
                    SizeType newCount = count - copyCount;

                    Iterator moveFrom = Begin() + copyCount - 1;

                    CreateFront(first, newCount);

                    for(SizeType i = 0; i < copyCount; i++) CreateFront(*moveFrom--);

                    typename Deque::Iterator moveSource = pos - (moveCount - copyCount);
                    typename Deque::Iterator moveDestination = Begin() + copyCount;

                    Move(moveSource, moveSource + (moveCount - copyCount), moveDestination);
                    Copy(first, last, pos - copyCount);

                    pos = Begin() + distanceFront;
                }
                else {
                    // Shift towards back
                    SizeType moveCount = distanceBack;
                    SizeType copyCount = Min(count, moveCount);
                    SizeType newCount = count - copyCount;

                    Iterator moveFrom = End() - copyCount;

                    for(SizeType i = 0; i < newCount; i++, first++) CreateBack(*first);

                    for(SizeType i = 0; i < copyCount; i++) CreateBack(*moveFrom++);

                    MoveBackward(pos, pos + (moveCount - copyCount), pos + count + (moveCount - copyCount));
                    Copy(first, last, pos);
                }
            }

            return pos;
        }

        #ifndef __WSTL_CXX11__
        template<typename... Args>
        Iterator Emplace(ConstIterator position, Args&&... args) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(p) T(Forward<Args>(args)...);

            return pos;
        }

        #else
        template<typename Arg>
        Iterator Emplace(ConstIterator position, const Arg& arg) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(arg);

            return pos;
        }

        template<typename Arg1, typename Arg2>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(arg1, arg2);

            return pos;
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        Iterator Emplace(ConstIterator position, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            Iterator pos = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__), pos);

            void* pointer;
            
            if(pos == Begin()) {
                this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[this->m_StartIndex]);
            }
            else if(pos == End()) {
                this->m_CurrentSize++;
                pointer = static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity]);
            }
            else {
                if(Distance(Begin(), pos) < Distance(pos, End() - 1)) {
                    CreateFront(*Begin());
                    Move(Begin() + 1, pos, Begin());

                    (*--pos).~T();
                    pointer = AddressOf(*pos);
                }   
                else {
                    CreateBack(*(End() - 1));
                    MoveBackward(pos, End() - 2, End() - 1);
                    
                    (*pos).~T();
                    pointer = AddressOf(*pos);
                }
            }

            ::new(pointer) T(arg1, arg2, arg3);

            return pos;
        }
        #endif

        /// TODO: Implement from At to Erase

        void PushBack(ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateBack(value);
        }

        #ifdef __WSTL_CXX11__
        void PushBack(T&& value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateBack(Move(value));
        }
        #endif

        void PopBack() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Empty(), OutOfRange("Deque is empty", __FILE__, __LINE__));
            #endif
            DestroyBack();
        }

        void PushFront(ConstReferenceType value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateFront(value);
        }

        #ifdef __WSTL_CXX11__
        void PushFront(T&& value) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif
            CreateFront(Move(value));
        }
        #endif

        void PopFront() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Empty(), OutOfRange("Deque is empty", __FILE__, __LINE__));
            #endif
            DestroyFront();
        }
    
    // FOR DEBUG
    public:
        static const __WSTL_CONSTEXPR__ SizeType m_BufferCapacity = SIZE + 1;

        T m_Buffer[m_BufferCapacity];
        SizeType m_StartIndex;

        template<typename U = T>
        typename EnableIf<IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            this->m_CurrentSize = 0;
            this->m_StartIndex = 0;
        }

        template<typename U = T>
        typename EnableIf<!IsTriviallyDestructible<U>::Value, void>::Type Initialize() {
            while(this->m_CurrentSize > 0) DestroyBack();
            this->m_StartIndex = 0;
        }

        void CreateFront(ConstReferenceType value) {
            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(value);
            this->m_CurrentSize++;
        }

        template<typename Iterator>
        void CreateFront(Iterator first, SizeType count) {
            if(count == 0) return;

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - count) % this->m_BufferCapacity;
            this->m_CurrentSize += count;

            for(SizeType i = 0; i < count; i++, first++)
                ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex + i])) T(*first);
        }

        void CreateBack(ConstReferenceType value) {
            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(value);
            this->m_CurrentSize++;
        }

        #ifdef __WSTL_CXX11__
        void CreateFront(T&& value) {
            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(Move(value));
            this->m_CurrentSize++;
        }

        void CreateBack(T&& value) {
            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(Move(value));
            this->m_CurrentSize++;
        }
        #endif

        void DestroyBack() {
            this->m_CurrentSize--;
            m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity].~T();
        }

        void DestroyFront() {
            m_Buffer[this->m_StartIndex].~T();
            this->m_StartIndex = (this->m_StartIndex + 1) % this->m_BufferCapacity;
            this->m_CurrentSize--;
        }
    };
}

#endif