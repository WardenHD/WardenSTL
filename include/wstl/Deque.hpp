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
            for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); it++) PushBack(*it);
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
            Iterator result = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(
                count <= this->m_Capacity - this->m_CurrentSize,
                LengthError("Deque is full", __FILE__, __LINE__),
                result
            );

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                for(SizeType i = 0; i < count; ++i) CreateFront(value);
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(SizeType i = 0; i < count; ++i) CreateBack(value);
                result = End() - count;
            }
            else {
                // Insert in the middle
                if (distanceFront <= distanceBack) {
                    SizeType createCopy = Min(count, distanceFront);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceFront - createCopy;

                    // Create new values
                    for(SizeType i = 0; i < createNew; i++) CreateFront(value);

                    // Create copies of old values
                    Iterator from = Begin() + createCopy - 1 + createNew;
                    for(SizeType i = 0; i < createCopy; i++, from--) CreateFront(*from);

                    // Move old values
                    from = result - copyOld;
                    Iterator to = Begin() + createCopy;
                    Move(from, from + copyOld, to);

                    // Fill the rest of the new values
                    to = result - createCopy;
                    FillInRange(to, copyNew, value);

                    result = Begin() + distanceFront;
                } 
                else {
                    SizeType createCopy = Min(count, distanceBack);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceBack - createCopy;

                    // Create new values
                    for(SizeType i = 0; i < createNew; i++) CreateBack(value);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; i++) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + count + copyOld);

                    // Fill the rest of the new values
                    FillInRange(result, copyNew, value);
                }
            }

            return result;
        }

        template<typename InputIterator>
        typename EnableIf<!IsIntegral<InputIterator>::Value, Iterator>::Type
        Insert(ConstIterator position, InputIterator first, InputIterator last) {
            Iterator result = Begin() + Distance(ConstBegin(), position);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(
                count <= this->m_Capacity - this->m_CurrentSize,
                LengthError("Deque is full", __FILE__, __LINE__),
                result
            );

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                CreateFront(first, count);
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(; first != last; first++) CreateBack(*first);
                result = End() - count;
            }
            else {
                // Insert in the middle
                if (distanceFront <= distanceBack) {
                    SizeType createCopy = Min(count, distanceFront);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceFront - createCopy;

                    // Create new values
                    CreateFront(first, createNew);

                    // Create copies of old values
                    CreateFront(Begin() + createNew, createCopy);

                    // Move old values
                    Iterator from = result - copyOld;
                    Iterator to = Begin() + createCopy;
                    Move(from, from + copyOld, to);
                    
                    // Copy new values
                    to = result - createCopy;
                    first += createNew;
                    CopyInRange(first, copyNew, to);

                    result = Begin() + distanceFront;
                } 
                else {
                    SizeType createCopy = Min(count, distanceBack);
                    SizeType createNew = (count > createCopy) ? count - createCopy : 0;
                    SizeType copyNew = (count > createNew) ? count - createNew : 0;
                    SizeType copyOld = distanceBack - createCopy;

                    // Create new values
                    InputIterator it = first + (count - createNew);
                    for(SizeType i = 0; i < createNew; i++, it++) CreateBack(*it);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; i++) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + count + copyOld);

                    // Copy new values
                    CopyInRange(first, copyNew, result);
                }
            }

            return result;
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        Iterator Insert(ConstIterator position, InitializerList<T> list) {
            Iterator result = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(
                list.Size() <= this->m_Capacity - this->m_CurrentSize,
                LengthError("Deque is full", __FILE__, __LINE__),
                result
            );

            SizeType distanceFront = Distance(Begin(), result);
            SizeType distanceBack = Distance(result, End());

            if(position == Begin()) {
                // Insert at front
                CreateFront(list.Begin(), list.Size());
                result = Begin();
            }
            else if(position == End()) {
                // Insert at back
                for(typename InitializerList<T>::Iterator it = list.Begin(); it != list.End(); it++) CreateBack(*it);
                result = End() - list.Size();
            }
            else {
                // Insert in the middle
                if (distanceFront <= distanceBack) {
                    SizeType createCopy = Min(list.Size(), distanceFront);
                    SizeType createNew = (list.Size() > createCopy) ? list.Size() - createCopy : 0;
                    SizeType copyNew = (list.Size() > createNew) ? list.Size() - createNew : 0;
                    SizeType copyOld = distanceFront - createCopy;

                    // Create new values
                    CreateFront(list.Begin(), createNew);

                    // Create copies of old values
                    CreateFront(Begin() + createNew, createCopy);

                    // Move old values
                    Iterator from = result - copyOld;
                    Iterator to = Begin() + createCopy;
                    Move(from, from + copyOld, to);
                    
                    // Copy new values
                    to = result - createCopy;
                    CopyInRange(list.Begin() + createNew, copyNew, to);

                    result = Begin() + distanceFront;
                } 
                else {
                    SizeType createCopy = Min(list.Size(), distanceBack);
                    SizeType createNew = (list.Size() > createCopy) ? list.Size() - createCopy : 0;
                    SizeType copyNew = (list.Size() > createNew) ? list.Size() - createNew : 0;
                    SizeType copyOld = distanceBack - createCopy;

                    // Create new values
                    typename InitializerList<T>::Iterator it = list.Begin() + (list.Size() - createNew);
                    for(SizeType i = 0; i < createNew; i++, it++) CreateBack(*it);

                    // Create copies of old values
                    for(SizeType i = 0; i < createCopy; i++) CreateBack(*(result + copyOld + i));

                    // Move old values
                    MoveBackward(result, result + copyOld, result + list.Size() + copyOld);

                    // Copy new values
                    CopyInRange(list.Begin(), copyNew, result);
                }
            }

            return result;
        }
        #endif

        #ifdef __WSTL_CXX11__
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

            ::new(pointer) T(Forward<Args>(args)...);

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

        Iterator Erase(Iterator position) {
            __WSTL_ASSERT_RETURNVALUE__(
                Distance(Begin(), position) <= DifferenceType(this->m_CurrentSize), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                position
            );

            if(position == Begin()) {
                DestroyFront();
                position = Begin();
            }
            else if(position == End() - 1) {
                DestroyBack();
                position = End();
            }
            else {
                if(Distance(Begin(), position) <= Distance(position, End())) {
                    MoveBackward(Begin(), position, position + 1);
                    DestroyFront();
                    position++;
                }
                else {
                    Move(position + 1, End(), position);
                    DestroyBack();
                }
            }

            return position;
        }

        Iterator Erase(ConstIterator position) {
            Iterator result = Begin() + Distance(ConstBegin(), position);

            __WSTL_ASSERT_RETURNVALUE__(
                Distance(Begin(), result) <= DifferenceType(this->m_CurrentSize), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                result
            );

            if(result == Begin()) {
                DestroyFront();
                result = Begin();
            }
            else if(result == End() - 1) {
                DestroyBack();
                result = End();
            }
            else {
                if(Distance(Begin(), result) <= Distance(result, End())) {
                    MoveBackward(Begin(), result, result + 1);
                    DestroyFront();
                    result++;
                }
                else {
                    Move(result + 1, End(), result);
                    DestroyBack();
                }
            }

            return result;
        }

        Iterator Erase(Iterator first, Iterator last) {
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(
                (Distance(Begin(), first) <= DifferenceType(this->m_CurrentSize)) && (Distance(Begin(), last) <= DifferenceType(this->m_CurrentSize)), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                first
            );

            if(first == Begin()) {
                for(SizeType i = 0; i < count; i++) DestroyFront();
                first = Begin();
            }
            else if(first == End() - count) {
                for(SizeType i = 0; i < count; i++) DestroyBack();
                first = End();
            }
            else {
                if(Distance(Begin(), first) < DifferenceType(this->m_CurrentSize / 2)) {
                    MoveBackward(Begin(), first, first + count);

                    for(SizeType i = 0; i < count; i++) DestroyFront();
                    first += count;
                }
                else {
                    Move(first + count, End(), first);

                    for(SizeType i = 0; i < count; i++) DestroyBack();
                }
            }

            return first;
        }

        Iterator Erase(ConstIterator first, ConstIterator last) {
            Iterator result = Begin() + Distance(ConstBegin(), first);
            SizeType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(
                (Distance(ConstBegin(), first) <= DifferenceType(this->m_CurrentSize)) && (Distance(ConstBegin(), last) <= DifferenceType(this->m_CurrentSize)), 
                OutOfRange("Deque index out of range", __FILE__, __LINE__), 
                result
            );

            if(result == Begin()) {
                for(SizeType i = 0; i < count; i++) DestroyFront();
                result = Begin();
            }
            else if(result == End() - count) {
                for(SizeType i = 0; i < count; i++) DestroyBack();
                result = End();
            }
            else {
                if(Distance(Begin(), result) <= DifferenceType(this->m_CurrentSize / 2)) {
                    MoveBackward(Begin(), result, result + count);

                    for(SizeType i = 0; i < count; i++) DestroyFront();
                    result += count;
                }
                else {
                    Move(result + count, End(), result);
                    
                    for(SizeType i = 0; i < count; i++) DestroyBack();
                }
            }

            return result;
        }

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
            CreateBack(Forward<T>(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        template<typename... Args>
        void EmplaceBack(Args&&... args) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(Forward<Args>(args)...);
            this->m_CurrentSize++;
        }

        #else
        template<typename Arg>
        void EmplaceBack(const Arg& arg) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(arg);
            this->m_CurrentSize++;
        }

        template<typename Arg1, typename Arg2>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(arg1, arg2);
            this->m_CurrentSize++;
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceBack(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            ::new(static_cast<void*>(&m_Buffer[(this->m_StartIndex + this->m_CurrentSize) % this->m_BufferCapacity])) T(arg1, arg2, arg3);
            this->m_CurrentSize++;
        }
        #endif

        template<typename InputIterator>
        inline void AppendRange(InputIterator first, InputIterator last) {
            return Insert(End(), first, last);
        }

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
            CreateFront(Forward<T>(value));
        }
        #endif

        #ifdef __WSTL_CXX11__
        template<typename... Args>
        void EmplaceFront(Args&&... args) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(Forward<Args>(args)...);
            this->m_CurrentSize++;
        }

        #else
        template<typename Arg>
        void EmplaceFront(const Arg& arg) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(arg);
            this->m_CurrentSize++;
        }

        template<typename Arg1, typename Arg2>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(arg1, arg2);
            this->m_CurrentSize++;
        }

        template<typename Arg1, typename Arg2, typename Arg3>
        void EmplaceFront(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Full(), LengthError("Deque is full", __FILE__, __LINE__));
            #endif

            this->m_StartIndex = (this->m_StartIndex + this->m_BufferCapacity - 1) % this->m_BufferCapacity;
            ::new(static_cast<void*>(&m_Buffer[this->m_StartIndex])) T(arg1, arg2, arg3);
            this->m_CurrentSize++;
        }
        #endif

        void PopFront() {
            #ifdef __WSTL_CHECK_PUSHPOP__
            __WSTL_ASSERT_RETURN__(!this->Empty(), OutOfRange("Deque is empty", __FILE__, __LINE__));
            #endif
            DestroyFront();
        }

        template<typename InputIterator>
        inline void PrependRange(InputIterator first, InputIterator last) {
            return Insert(Begin(), first, last);
        }

        void Swap(Deque& other) {
            
        }
    
    private:
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