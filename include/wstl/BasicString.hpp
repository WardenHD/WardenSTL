#ifndef __WSTL_BASICSTRING_HPP__
#define __WSTL_BASICSTRING_HPP__

#include "private/Platform.hpp"
#include "CharacterTraits.hpp"
#include "Limits.hpp"
#include "Container.hpp"
#include "StandardExceptions.hpp"
#include "StringView.hpp"
#include "Memory.hpp"

// #define __WSTL_STRING_CLEAR_UNUSED__
#define __WSTL_STRING_TRUNCATION_CHECK__
#define __WSTL_STRING_TRUNCATION_ERROR__


namespace wstl {
    // No position (npos) constant

    namespace string {
        #ifdef __WSTL_CXX11__
        static constexpr size_t NoPosition = NumericLimits<size_t>::Max();

        #else
        enum {
            NoPosition = NumericLimits<size_t>::Max()
        };
        #endif
    }

    template<typename Derived, typename T, typename Traits = CharacterTraits<T> >
    class BasicString : public TypedContainerBase<T> {
    public:
        typedef Traits TraitsType;
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;

        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        enum {
            NoPosition = NumericLimits<SizeType>::Max()
        };

        ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position < this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String index out of range"));
            return m_Buffer[position];
        }

        ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position < this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String index out of range"));
            return m_Buffer[position];
        }

        ReferenceType operator[](SizeType position) {
            return m_Buffer[position];
        }

        ConstReferenceType operator[](SizeType position) const {
            return m_Buffer[position];
        }

        ReferenceType Front() {
            return m_Buffer[0];
        }

        ConstReferenceType Front() const {
            return m_Buffer[0];
        }

        ReferenceType Back() {
            return m_Buffer[this->m_CurrentSize - 1];
        }

        ConstReferenceType Back() const {
            return m_Buffer[this->m_CurrentSize - 1];
        }

        PointerType Data() __WSTL_NOEXCEPT__ {
            return m_Buffer;
        }

        __WSTL_CONSTEXPR__ ConstPointerType Data() const __WSTL_NOEXCEPT__ {
            return m_Buffer;
        }

        ConstPointerType CString() const {
            return m_Buffer;
        }

        operator BasicStringView<T, Traits>() const __WSTL_NOEXCEPT__ {
            return BasicStringView<T, Traits>(Data(), this->Size());
        }

        Iterator Begin() {
            return m_Buffer;
        }

        ConstIterator Begin() const {
            return m_Buffer;
        }

        ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return m_Buffer;
        }

        Iterator End() {
            return m_Buffer + this->m_CurrentSize;
        }

        ConstIterator End() const {
            return m_Buffer + this->m_CurrentSize;
        }

        ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return m_Buffer + this->m_CurrentSize;
        }

        ReverseIterator ReverseBegin() {
            return ReverseIterator(m_Buffer + this->m_CurrentSize);
        }

        ConstReverseIterator ReverseBegin() const {
            return ConstReverseIterator(m_Buffer + this->m_CurrentSize);
        }

        ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Buffer + this->m_CurrentSize);
        }

        ReverseIterator ReverseEnd() {
            return ReverseIterator(m_Buffer);
        }

        ConstReverseIterator ReverseEnd() const {
            return ConstReverseIterator(m_Buffer);
        }

        ConstReverseIterator ConstReverseEnd() const {
            return ConstReverseIterator(m_Buffer);
        }

        SizeType Length() const {
            return this->m_CurrentSize;
        }

        void Clear() {
            this->m_CurrentSize = 0;
            m_Buffer[0] = 0;

            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(m_Buffer, this->m_Capacity, 0);
            #endif
        }

        Derived Substring(SizeType position = 0, SizeType count = NoPosition) const {
            Derived result;

            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Substring position out of range"), result);
            result.Assign(m_Buffer + position, Min(count, this->Size() - position));

            return result;
        }

        BasicString& Insert(SizeType index, SizeType count, T ch) {
            if(count == 0) return *this;

            __WSTL_ASSERT_RETURNVALUE__(index <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String insert position out of range"), *this);

            // Check if the string truncates
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(count > this->Available()) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            Iterator position = Begin() + index;

            if(index + count >= this->m_Capacity) {
                this->m_CurrentSize = this->m_Capacity;
                Fill(position, End(), ch);
            }
            else {
                if(count >= this->Available()) this->m_CurrentSize = this->m_Capacity;
                else this->m_CurrentSize += count;

                // Shifting
                memory::Move(position, Min(this->m_CurrentSize - index, this->m_Capacity - index - count), position + count);
                FillInRange(position, count, ch);
            }

            m_Buffer[this->m_CurrentSize] = 0;
            return *this;
        }

        template<typename InputIterator>
        Iterator Insert(ConstIterator position, InputIterator first, InputIterator last) {
            Iterator result = const_cast<Iterator>(position);

            if(first == last) return result;

            DifferenceType index = Distance(ConstBegin(), position);
            DifferenceType count = Distance(first, last);

            __WSTL_ASSERT_RETURNVALUE__(index <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String insert position out of range"), result);

            // Check if the string truncates
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(SizeType(count) > this->Available()) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            if(index + count >= this->m_Capacity) {
                this->m_CurrentSize = this->m_Capacity;
                result = CopyCharacters(first, Distance(result, End()), result);
            }
            else {
                if(count >= this->Available()) this->m_CurrentSize = this->m_Capacity;
                else this->m_CurrentSize += count;

                // Shifting
                memory::Move(result, Min(this->m_CurrentSize - index, this->m_Capacity - index - count), result + count);
                
                result = CopyCharacters(first, count, result);
            }

            m_Buffer[this->m_CurrentSize] = 0;
            return result;
        }

        BasicString& Insert(SizeType index, const T* string) {
            Insert(ConstBegin() + index, string, string + Traits::Length(string));
            return *this;
        }

        BasicString& Insert(SizeType index, const T* string, SizeType count) {
            Insert(ConstBegin() + index, string, string + count);
            return *this;
        }

        template<typename UDerived, typename UTraits>
        BasicString& Insert(SizeType index, const BasicString<UDerived, T, UTraits>& string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            Insert(ConstBegin() + index, string.Begin(), string.End());
            return *this;
        }

        template<typename UDerived, typename UTraits>
        BasicString& Insert(SizeType index, const BasicString<UDerived, T, UTraits>& string, SizeType stringIndex, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(stringIndex <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Inserted string index out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            Insert(index, string.Data() + stringIndex, Min(count, string.Size() - stringIndex));
            return *this;
        }

        Iterator Insert(ConstIterator position, T ch) {
            Iterator result = const_cast<Iterator>(position);

            __WSTL_ASSERT_RETURNVALUE__(Distance(Begin(), result) <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String insert position out of range"), result);

            // Check if the string truncates
            if(this->m_CurrentSize >= this->m_Capacity) {
                #ifdef __WSTL_STRING_TRUNCATION_CHECK__
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
                #endif
            }
            else ++this->m_CurrentSize;

            if(position != End()) memory::Move(result, End() - 1, result + 1);
            *result = ch;

            m_Buffer[this->m_CurrentSize] = 0;
            return result;
        }

        Iterator Insert(ConstIterator position, SizeType count, T ch) {
            Insert(Distance(ConstBegin(), position), count, ch);
            return const_cast<Iterator>(position);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        Iterator Insert(ConstIterator position, InitializerList<T> list) {
            return Insert(position, list.Begin(), list.End());
        }
        #endif

        template<typename UTraits>
        BasicString& Insert(SizeType index, const BasicStringView<T, UTraits>& view) {
            Insert(Begin() + index, view.Begin(), view.End());
            return *this;
        }

        template<typename UTraits>
        BasicString& Insert(SizeType index, const BasicStringView<T, UTraits>& view, SizeType viewIndex, SizeType count = NoPosition) {
            Insert(index, view.Substring(viewIndex, count));
            return *this;
        }

        Iterator Erase(ConstIterator first, ConstIterator last) {
            Iterator start = const_cast<Iterator>(first);
            Iterator end = const_cast<Iterator>(last);

            if(start == end) return start;

            memory::Move(end, End(), start);
            this->m_CurrentSize -= Distance(first, last);

            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif

            return start;
        }

        Iterator Erase(ConstIterator position) {
            Iterator iterator = const_cast<Iterator>(position);

            memory::Move(iterator + 1, End(), iterator);
            m_Buffer[--this->m_CurrentSize] = 0;

            return iterator;
        }

        BasicString& Erase(SizeType index = 0, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(index <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String erase index out of range"), *this);
            Erase(Begin() + index, Begin() + index + Min(count, this->Size() - index));
            return *this;
        }

        void PushBack(T ch) {
            if(!this->Full()) {
                m_Buffer[this->m_CurrentSize] = ch;
                m_Buffer[++this->m_CurrentSize] = 0;
            }
            else {
                #ifdef __WSTL_STRING_TRUNCATION_CHECK__
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
                #endif
            }
        }

        void PopBack() {
            if(!this->Empty()) m_Buffer[--this->m_CurrentSize] = 0;
        }

        template<typename InputIterator>
        BasicString& Append(InputIterator first, InputIterator last) {
            if(first == last) return *this;

            DifferenceType count = Distance(first, last);
            
            // Check if the string truncates
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(count > this->Available()) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            count = Min(SizeType(count), this->Available());

            CopyCharacters(first, count, End());

            this->m_CurrentSize += SizeType(count);
            
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif

            return *this;
        }

        BasicString& Append(SizeType count, T ch) {
            // Check if the string truncates
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(count > this->Available()) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            count = Min(count, this->Available());

            FillInRange(End(), count, ch);
            this->m_CurrentSize += count;
            m_Buffer[this->m_CurrentSize] = 0;

            return *this;
        }

        BasicString& Append(const T* string, SizeType count) {
            return Append(string, string + count);
        }

        BasicString& Append(const T* string) {
            return Append(string, Traits::Length(string));
        }

        template<typename UTraits>
        BasicString& Append(const BasicStringView<T, UTraits>& view) {
            return Append(view.Begin(), view.End());
        }

        template<typename UTraits>
        BasicString& Append(const BasicStringView<T, UTraits>& view, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Appended string view position out of range"), *this);
            return Append(view.Substring(position, count));
        }

        template<typename UDerived, typename UTraits>
        BasicString& Append(const BasicString<UDerived, T, UTraits>& string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            return Append(string.Begin(), string.End());
        }

        template<typename UDerived, typename UTraits>
        BasicString& Append(const BasicString<UDerived, T, UTraits>& string, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Appended string position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            return Append(string.Data() + position, Min(count, string.Size() - position));
        }
        
        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        BasicString& Append(InitializerList<T> list) {
            return Append(list.Begin(), list.End());
        }
        #endif
        
        template<typename UDerived, typename UTraits>
        BasicString& operator+=(const BasicString<UDerived, T, UTraits>& string) {
            return Append(string);
        }

        BasicString& operator+=(T ch) {
            PushBack(ch);
            return *this;
        }

        BasicString& operator+=(const T* string) {
            return Append(string);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        BasicString& operator+=(InitializerList<T> list) {
            return Append(list);
        }
        #endif
        
        template<typename UTraits>
        BasicString& operator+=(const BasicStringView<T, UTraits>& view) {
            return Append(view);
        }

        template<typename InputIterator>
        BasicString& Replace(ConstIterator first, ConstIterator last, InputIterator first2, InputIterator last2) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, first2, Distance(first2, last2), false);
            #else
            return ReplaceImpl(first, last, first2, Distance(first2, last2));
            #endif
        }

        template<typename UDerived, typename UTraits>
        BasicString& Replace(SizeType position, SizeType count, const BasicString<UDerived, T, UTraits>& string) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            count = Min(count, this->Size() - position);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + count, string.CString(), string.Size(), string.IsTruncated());
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + count, string.CString(), string.Size());
            #endif
        }
        
        template<typename UDerived, typename UTraits>
        BasicString& Replace(ConstIterator first, ConstIterator last, const BasicString<UDerived, T, UTraits>& string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, string.CString(), string.Size(), string.IsTruncated());
            #else
            return ReplaceImpl(first, last, string.CString(), string.Size());
            #endif
        }

        template<typename UDerived, typename UTraits>
        BasicString& Replace(SizeType position, SizeType count, const BasicString<UDerived, T, UTraits>& string, SizeType position2, SizeType count2 = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            __WSTL_ASSERT_RETURNVALUE__(position2 <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Replacing string position out of range"), *this);
            
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string.CString() + position2, 
                Min(count2, string.Size() - position2), string.IsTruncated());
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string.CString() + position2, 
                Min(count2, string.Size() - position2));
            #endif
        }

        BasicString& Replace(SizeType position, SizeType count, const T* string, SizeType count2) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, count2, false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, count2);
            #endif
        }

        BasicString& Replace(ConstIterator first, ConstIterator last, const T* string, SizeType count2) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, string, count2, false);
            #else
            return ReplaceImpl(first, last, string, count2);
            #endif
        }

        BasicString& Replace(SizeType position, SizeType count, const T* string) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, Traits::Length(string), false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, Traits::Length(string));
            #endif
        }

        BasicString& Replace(ConstIterator first, ConstIterator last, const T* string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, string, Traits::Length(string), false);
            #else
            return ReplaceImpl(first, last, string, Traits::Length(string));
            #endif
        }

        BasicString& Replace(SizeType position, SizeType count, SizeType count2, T ch) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            Erase(position, Min(count, this->Size() - position));
            Insert(position, count2, ch);
            return *this;
        }

        BasicString& Replace(ConstIterator first, ConstIterator last, SizeType count2, T ch) {
            Erase(first, last);
            Insert(first, count2, ch);
            return *this;
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        BasicString& Replace(ConstIterator first, ConstIterator last, InitializerList<T> list) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, list.Begin(), list.Size(), false);
            #else
            return ReplaceImpl(first, last, list.Begin(), list.Size());
            #endif
        }
        #endif

        template<typename UTraits>
        BasicString& Replace(SizeType position, SizeType count, const BasicStringView<T, UTraits>& view) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), view.Data(), view.Size(), false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), view.Data(), view.Size());
            #endif
        }

        template<typename UTraits>
        BasicString& Replace(ConstIterator first, ConstIterator last, const BasicStringView<T, UTraits>& view) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, view.Data(), view.Size(), false);
            #else
            return ReplaceImpl(first, last, view.Data(), view.Size());
            #endif
        }

        template<typename UTraits>
        BasicString& Replace(SizeType position, SizeType count, const BasicStringView<T, UTraits>& view, SizeType position2, SizeType count2 = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            __WSTL_ASSERT_RETURNVALUE__(position2 <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Replacing string view position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), view.Data() + position2, Min(count2, view.Size() - position2), false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), view.Data() + position2, Min(count2, view.Size() - position2));
            #endif
        }

        template<size_t N>
        BasicString& Replace(SizeType position, SizeType count, const ValueType (&literal)[N]) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), literal, N, false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), literal, N);
            #endif
        }

        template<size_t N>
        BasicString& Replace(ConstIterator first, ConstIterator last, const ValueType (&literal)[N]) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, literal, N, false);
            #else
            return ReplaceImpl(first, last, literal, N);
            #endif
        }

        SizeType Copy(T* destination, SizeType count, SizeType position = 0) const {
            if(position > this->Size()) return 0;

            count = Min(count, this->Size() - position);
            memory::Move(m_Buffer + position, count, destination);

            return count;
        }

        void Resize(SizeType count, T ch) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(count > this->m_Capacity) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            count = Min(count, this->m_Capacity);

            // If new size is bigger
            if(count > this->m_CurrentSize) Fill(m_Buffer + this->m_CurrentSize, m_Buffer + count, ch);

            this->m_CurrentSize = count;
            
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif
        }

        void Resize(SizeType count) {
            Resize(count, 0);
        }

        template<typename Operation>
        void ResizeAndOverwrite(SizeType count, Operation operation) {
            __WSTL_ASSERT_RETURN__(count <= this->m_Capacity, WSTL_MAKE_EXCEPTION(LengthError, "New string size is bigger than capacity"));

            this->m_CurrentSize = operation(m_Buffer, count);
            
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif
        }

        void UninitializedResize(SizeType count) {
            this->m_CurrentSize = count;
            m_Buffer[this->m_CurrentSize] = 0;
        }

        void Swap(BasicString& other) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            wstl::Swap(m_Truncated, other.m_Truncated);
            #endif

            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
            SwapRanges(m_Buffer, m_Buffer + this->m_Capacity, other.m_Buffer);
        }

        template<typename InputIterator>
        BasicString& Assign(InputIterator first, InputIterator last) {
            if(first == last) return *this;

            DifferenceType count = Distance(first, last);
            
            // Check if the string truncates
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(count > this->m_Capacity) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            count = Min(SizeType(count), this->m_Capacity);

            CopyCharacters(first, count, Begin());

            this->m_CurrentSize = SizeType(count);
            
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif

            return *this;
        }

        #ifdef __WSTL_CXX11__
        template<typename UDerived, typename UTraits>
        BasicString& Assign(BasicString<UDerived, T, UTraits>&& string) {
            if(static_cast<void*>(&string) == static_cast<void*>(this)) return *this;

            SizeType count = string.Size();
            
            // Check if the string truncates
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(count > this->m_Capacity) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "String truncation"));
                #endif
            }
            #endif

            count = Min(count, this->m_Capacity);

            CopyCharacters(string.Begin(), count, Begin());

            this->m_CurrentSize = count;
            
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif

            return *this;
        } 
        #endif

        template<typename UDerived, typename UTraits>
        BasicString& Assign(const BasicString<UDerived, T, UTraits>& string) {
            return Assign(string.Begin(), string.End());
        }

        BasicString& Assign(SizeType count, T value) {
            Clear();
            Resize(count, value);
            return *this;
        }

        BasicString& Assign(const T* string, SizeType count) {
            return Assign(string, string + count);
        }

        BasicString& Assign(const T* string) {
            return Assign(string, string + Traits::Length(string));
        }

        template<typename UTraits>
        BasicString& Assign(const BasicStringView<T, UTraits>& view) {
            return Assign(view.Begin(), view.End());
        }

        template<typename UTraits>
        BasicString& Assign(const BasicStringView<T, UTraits>& view, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String assign position out of range"), *this);
            return Assign(view.Begin() + position, Min(count, view.Size() - position));
        }

        template<typename UDerived, typename UTraits>
        BasicString& Assign(const BasicString<UDerived, T, UTraits>& string, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String assign position out of range"), *this);
            return Assign(string.Begin() + position, Min(count, string.Size() - position));
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        BasicString& Assign(InitializerList<T> list) {
            return Assign(list.Begin(), list.End());
        }
        #endif

        template<typename UDerived, typename UTraits>
        BasicString& operator=(const BasicString<UDerived, T, UTraits>& string) {
            return Assign(string);
        }

        BasicString& operator=(const T* string) {
            return Assign(string);
        }

        BasicString& operator=(T value) {
            return Assign(1, value);
        }

        #ifdef __WSTL_CXX11__
        template<typename UDerived, typename UTraits>
        BasicString& operator=(BasicString<UDerived, T, UTraits>&& string) {
            return Assign(Move(string));
        }
        #endif

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        BasicString& operator=(InitializerList<T> list) {
            return Assign(list);
        }
        #endif

        template<typename UTraits>
        BasicString& operator=(const BasicStringView<T, UTraits>& view) {
            return Assign(view);
        }

        BasicString& operator=(NullPointerType) __WSTL_DELETE__;

        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        void ClearTruncated() {
            m_Truncated = false;
        }

        bool IsTruncated() const {
            return m_Truncated;
        }
        #endif

        /// @brief Finds the first occurrence of a C-style string
        /// @param string C-style string to find
        /// @param position Position to start the search from (default is 0)
        /// @param count Number of characters in the C-style string
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType Find(const T* string, SizeType position, SizeType count) const {
            if(position + count > this->Size()) return NoPosition;

            ConstIterator iterator = Search(Begin() + position, End(), string, string + count);
            if(iterator == End()) return NoPosition;

            return Distance(Begin(), iterator);
        }

        /// @brief Finds the first occurrence of a character
        /// @param ch Character to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType Find(T ch, SizeType position = 0) const {
            if(position + 1 > this->Size()) return NoPosition;

            ConstIterator iterator = wstl::Find(Begin() + position, End(), ch);
            if(iterator == End()) return NoPosition;

            return Distance(Begin(), iterator);
        }

        /// @brief Finds the first occurrence of a substring
        /// @param string Substring to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        template<typename UDerived, typename UTraits>
        SizeType Find(const BasicString<UDerived, T, UTraits>& string, SizeType position = 0) const {
            return Find(string.CString(), position, string.Size());
        }

        /// @brief Finds the first occurrence of a C-style string
        /// @param string C-style string to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType Find(const T* string, SizeType position = 0) const {
            return Find(string, position, Traits::Length(string));
        }

        template<typename UTraits>
        SizeType Find(const BasicStringView<T, UTraits>& view, SizeType position = 0) const {
            return Find(view.Data(), position, view.Size());
        }

        /// @brief Finds the last occurrence of a C-style string, searching backwards
        /// @param string C-style string to find
        /// @param position Position to start the backward search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType ReverseFind(const T* string, SizeType position, SizeType count) const {
            if(count > this->Size()) return NoPosition;

            ConstReverseIterator reverseBegin(string + count);
            ConstReverseIterator reverseEnd(string);

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator = Search(ReverseBegin() + position, ReverseEnd(), reverseBegin, reverseEnd);
            if(iterator == ReverseEnd()) return NoPosition;

            return Distance(Begin(), iterator.Base());
        }

        /// @brief Finds the last occurrence of a character, searching backwards
        /// @param ch Character to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType ReverseFind(T ch, SizeType position = NoPosition) const {
            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator = wstl::Find(ReverseBegin() + position, ReverseEnd(), ch);
            if(iterator == ReverseEnd()) return NoPosition;

            return Distance(Begin(), iterator.Base());
        }

        template<typename UDerived, typename UTraits>
        SizeType ReverseFind(const BasicString<UDerived, T, UTraits>& string, SizeType position = NoPosition) const {
            return ReverseFind(string.CString(), position, string.Size());
        }

        /// @brief Finds the last occurrence of a C-style string, searching backwards
        /// @param string C-style string to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType ReverseFind(const T* string, SizeType position = NoPosition) const {
            return ReverseFind(string, position, Traits::Length(string));
        }

        /// @brief Finds the last occurrence of a substring, searching backwards
        /// @param view Substring to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        template<typename UTraits>
        SizeType ReverseFind(const BasicStringView<T, UTraits>& view, SizeType position = NoPosition) const {
            return ReverseFind(view.Data(), position, view.Size());
        }

        SizeType FindFirstOf(const T* string, SizeType position, SizeType count) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i) {
                    for(size_t j = 0; j < count; ++j)
                        if(m_Buffer[i] == string[j]) return i;
                }
            }

            return NoPosition;
        }

        SizeType FindFirstOf(T ch, SizeType position = 0) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i) 
                    if(m_Buffer[i] == ch) return i;
            }

            return NoPosition;
        }

        template<typename UDerived, typename UTraits>
        SizeType FindFirstOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = 0) const {
            return FindFirstOf(string.CString(), position, string.Size());
        }

        SizeType FindFirstOf(const T* string, SizeType position = 0) const {
            return FindFirstOf(string, position, Traits::Length(string));
        }

        template<typename UTraits>
        SizeType FindFirstOf(const BasicStringView<T, UTraits>& view, SizeType position = 0) const {
            return FindFirstOf(view.Data(), position, view.Size());
        }

        SizeType FindLastOf(const T* string, SizeType position, SizeType count) const {
            if(this->Empty()) return NoPosition;

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position) {
                for(size_t j = 0; j < count; ++j)
                    if(m_Buffer[position] == string[j]) return position;
            }

            return NoPosition;
        }

        SizeType FindLastOf(T ch, SizeType position = NoPosition) const {
            if(this->Empty()) return NoPosition;

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position)
                if(m_Buffer[position] == ch) return position;

            return NoPosition;
        }

        template<typename UDerived, typename UTraits>
        SizeType FindLastOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = NoPosition) const {
            return FindLastOf(string.CString(), position, string.Size());
        }

        SizeType FindLastOf(const T* string, SizeType position = NoPosition) const {
            return FindLastOf(string, position, Traits::Length(string));
        }

        template<typename UTraits>
        SizeType FindLastOf(const BasicStringView<T, UTraits>& view, SizeType position = NoPosition) const {
            return FindLastOf(view.Data(), position, view.Size());
        }

        SizeType FindFirstNotOf(const T* string, SizeType position, SizeType count) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i) {
                    bool found = false;

                    for(size_t j = 0; j < count; ++j) {
                        if(m_Buffer[i] == string[j]) {
                            found = true;
                            break;
                        }
                    }

                    if(!found) return i;
                }
            }

            return NoPosition;
        }

        SizeType FindFirstNotOf(T ch, SizeType position = 0) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i)
                    if(m_Buffer[i] != ch) return i;
            }

            return NoPosition;
        }

        template<typename UDerived, typename UTraits>
        SizeType FindFirstNotOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = 0) const {
            return FindFirstNotOf(string.CString(), position);
        }

        SizeType FindFirstNotOf(const T* string, SizeType position = 0) const {
            return FindFirstNotOf(string, position, Traits::Length(string));
        }

        template<typename UTraits>
        SizeType FindFirstNotOf(const BasicStringView<T, UTraits>& view, SizeType position = 0) const {
            return FindFirstNotOf(view.Data(), position, view.Size());
        }

        SizeType FindLastNotOf(const T* string, SizeType position, SizeType count) const {
            if(this->Empty()) return NoPosition;

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position) {
                bool found = false;

                for(size_t j = 0; j < count; ++j) {
                    if(m_Buffer[position] == string[j]) {
                        found = true;
                        break;
                    }
                }

                if(!found) return position;
            }

            return NoPosition;
        }

        SizeType FindLastNotOf(T ch, SizeType position = NoPosition) const {
            if(this->Empty()) return NoPosition;

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator = ConstReverseIterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position)
                if(m_Buffer[position] != ch) return position;

            return NoPosition;
        }

        template<typename UDerived, typename UTraits>
        SizeType FindLastNotOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = NoPosition) const {
            return FindLastNotOf(string.CString(), position, string.Size());
        }

        SizeType FindLastNotOf(const T* string, SizeType position = NoPosition) const {
            return FindLastNotOf(string, position, Traits::Length(string));
        }

        template<typename UTraits>
        SizeType FindLastNotOf(const BasicStringView<T, UTraits>& view, SizeType position = NoPosition) const {
            return FindLastNotOf(view.Data(), position, view.Size());
        }

        template<typename UDerived, typename UTraits>
        int Compare(const BasicString<UDerived, T, UTraits>& other) const {
            return Compare(m_Buffer, this->Size(), other.Data(), other.Size());
        }

        template<typename UDerived, typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicString<UDerived, T, UTraits>& other) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), other.Data(), other.Size());
        }

        template<typename UDerived, typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicString<UDerived, T, UTraits>& other, SizeType position2, SizeType count2 = NoPosition) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            __WSTL_ASSERT__(position2 <= other.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position2 out of range"));

            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), other.Data() + position2, Min(count2, other.Size() - position2));
        }

        int Compare(const T* string) const {
            return Compare(m_Buffer, this->Size(), string, Traits::Length(string));
        }

        int Compare(SizeType position1, SizeType count1, const T* string) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), string, Traits::Length(string));
        }

        int Compare(SizeType position1, SizeType count1, const T* string, SizeType count2) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), string, count2);
        }

        template<typename UTraits>
        int Compare(const BasicStringView<T, UTraits>& view) const {
            return Compare(m_Buffer, this->Size(), view.Data(), view.Size());
        }

        template<typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicStringView<T, UTraits>& view) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), view.Data(), view.Size());
        }

        template<typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicStringView<T, UTraits>& view, SizeType position2, SizeType count2 = NoPosition) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            __WSTL_ASSERT__(position2 <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String view compare position2 out of range"));

            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), view.Data() + position2, Min(count2, view.Size() - position2));
        }

        template<typename UDerived, typename UTraits>
        bool StartsWith(const BasicString<UDerived, T, UTraits>& string) const {
            return Compare(0, string.Size(), string) == 0;
        }

        template<typename UTraits>
        bool StartsWith(const BasicStringView<T, UTraits>& view) const {
            return Compare(0, view.Size(), view) == 0;
        }

        bool StartsWith(const T* string) const {
            SizeType length = Traits::Length(string);
            return (this->Size() >= length) && (Compare(0, length, string) == 0);
        }

        bool StartsWith(T ch) const {
            return !this->Empty() && Traits::Equal(Front(), ch);
        }

        template<typename UDerived, typename UTraits>
        bool EndsWith(const BasicString<UDerived, T, UTraits>& string) const {
            return (this->Size() >= string.Size()) && (Compare(this->Size() - string.Size(), NoPosition, string) == 0);
        }

        template<typename UTraits>
        bool EndsWith(const BasicStringView<T, UTraits>& view) const {
            return (this->Size() >= view.Size()) && (Compare(this->Size() - view.Size(), NoPosition, view) == 0);
        }

        bool EndsWith(const T* string) const {
            SizeType length = Traits::Length(string);
            return (this->Size() >= length) && (Compare(this->Size() - length, NoPosition, string) == 0);
        }

        bool EndsWith(T ch) const {
            return !this->Empty() && Traits::Equal(Back(), ch);
        }

        template<typename UDerived, typename UTraits>
        bool Contains(const BasicString<UDerived, T, UTraits>& string) const {
            return Find(string) != NoPosition;
        }

        template<typename UTraits>
        bool Contains(const BasicStringView<T, UTraits>& string) const {
            return Find(string) != NoPosition;
        }

        bool Contains(const T* string) const {
            return Find(string) != NoPosition;
        }
        
        bool Contains(T ch) const {
            return Find(ch) != NoPosition;
        }

    protected:
        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        BasicString(T* buffer, SizeType capacity) : TypedContainerBase<T>(capacity), m_Buffer(buffer), m_Truncated(false) {
            m_Buffer[0] = 0;
        }

        #else
        BasicString(T* buffer, SizeType capacity) : TypedContainerBase<T>(capacity), m_Buffer(buffer) {
            m_Buffer[0] = 0;
        }

        #endif

        ~BasicString() {
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            Clear();
            #endif
        }

        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        bool m_Truncated;
        #endif
       
    private:
        T* m_Buffer;

        // Disable copy constructor
        BasicString(const BasicString&) {}

        template<typename Iterator1, typename Iterator2>
        static typename EnableIf<IsPointer<Iterator1>::Value && IsPointer<Iterator2>::Value, Iterator2>::Type
        CopyCharacters(Iterator1 source, size_t count, Iterator2 destination) {
            memory::Move(source, count, destination);
            return destination + count;
        }

        template<typename Iterator1, typename Iterator2>
        inline static typename EnableIf<!IsPointer<Iterator1>::Value || !IsPointer<Iterator2>::Value, Iterator2>::Type
        CopyCharacters(Iterator1 source, size_t count, Iterator2 destination) {
            return CopyInRange(source, count, destination);
        }

        static int Compare(ConstPointerType first1, SizeType count1, ConstPointerType first2, SizeType count2) {
            if(count1 > count2) return 1;
            if(count1 < count2) return -1;

            return string::CompareInRange(first1, first2, count1); // we don't care whether count1 or count2, here they are considered equal
        }

        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        BasicString& ReplaceImpl(ConstIterator first, ConstIterator last, const T* string, SizeType count, bool truncated) {
        #else
        BasicString& ReplaceImpl(ConstIterator first, ConstIterator last, const T* string, SizeType count) {
        #endif
            // Trivial case - replacing nothing with nothing
            if((first == last) && (string == NullPointer || count == 0)) return *this;

            // Invalid range
            if(first > last) return *this;

            Iterator start = const_cast<Iterator>(first);
            Iterator end = const_cast<Iterator>(last);

            const SizeType freeSpace = this->m_Capacity - Distance(m_Buffer, start); // theoretical possible insertion length after start
            const SizeType removeCount = Distance(start, end);
            const SizeType insertCount = (string == NullPointer) ? 0 : Min(count, freeSpace);

            const DifferenceType sizeChange = DifferenceType(insertCount - removeCount);

            // Handle the case when growing and source overlaps, in replace range, and is after it
            // Tail must be bigger to not lose replacing data
            Iterator tail = (string > first && string < last && insertCount > removeCount) ? const_cast<Iterator>(string) : end;
            
            SizeType tailCount = Distance(tail, End());
            SizeType tailSpace = tailCount + (this->Available() - sizeChange);

            // Check if tail or insert length are truncated
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            if(truncated || (insertCount != count && string != NullPointer) || (tailSpace < tailCount)) {
                m_Truncated = true;

                #ifdef __WSTL_STRING_TRUNCATION_ERROR__
                __WSTL_THROW__(WSTL_MAKE_EXCEPTION(LengthError, "Replace truncation"));
                #endif
            }
            #endif

            tailCount = Min(tailCount, tailSpace);

            // 3 cases: same size, grow, shrink
            if(insertCount == removeCount) {
                // Same size: just overwrite (move is used when source overlaps)
                memory::Move(string, insertCount, start);
            }
            else if (insertCount > removeCount) {
                // Grow: shift tail then copy
                memory::Move(tail, tailCount, tail + sizeChange);

                // Handle case when source overlaps and is after the replacing range start
                if(string > first && string < End()) string += sizeChange;
                memory::Move(string, insertCount, start);
            }
            else {
                // Shrink: move (is used when source overlaps) then shift tail
                memory::Move(string, insertCount, start);
                memory::Copy(tail, tailCount, tail + sizeChange);
            }

            this->m_CurrentSize = Min(this->m_CurrentSize + sizeChange, this->Capacity());
            
            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(Begin() + this->m_CurrentSize, Begin() + this->m_Capacity, 0);
            #else
            m_Buffer[this->m_CurrentSize] = 0;
            #endif

            return *this;
        }
    };

    // Comparison operators
    
    template<typename Derived, typename T, typename Traits> 
    inline bool operator==(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        return (a.Size() == b.Size()) && Equal(a.Begin(), a.End(), b.Begin());
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator==(const BasicString<Derived, T, Traits>& a, const T* b) {
        return (a.Size() == Traits::Length(b)) && Equal(a.Begin(), a.End(), b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator==(const T* a, const BasicString<Derived, T, Traits>& b) {
        return (b.Size() == Traits::Length(a)) && Equal(b.Begin(), b.End(), a);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator!=(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        return !(a == b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator!=(const BasicString<Derived, T, Traits>& a, const T* b) {
        return !(a == b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator!=(const T* a, const BasicString<Derived, T, Traits>& b) {
        return !(a == b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator<(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        return LexicographicalCompare(a.Begin(), a.End(), b.Begin(), b.End());
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator<(const BasicString<Derived, T, Traits>& a, const T* b) {
        return LexicographicalCompare(a.Begin(), a.End(), b, b + Traits::Length(b));
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator<(const T* a, const BasicString<Derived, T, Traits>& b) {
        return LexicographicalCompare(a, a + Traits::Length(a), b.Begin(), b.End());
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator>(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        return b < a;
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator>(const BasicString<Derived, T, Traits>& a, const T* b) {
        return b < a;
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator>(const T* a, const BasicString<Derived, T, Traits>& b) {
        return b < a;
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator<=(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        return !(a > b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator<=(const BasicString<Derived, T, Traits>& a, const T* b) {
        return !(a > b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator<=(const T* a, const BasicString<Derived, T, Traits>& b) {
        return !(a > b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator>=(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        return !(a < b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator>=(const BasicString<Derived, T, Traits>& a, const T* b) {
        return !(a < b);
    }

    template<typename Derived, typename T, typename Traits> 
    inline bool operator>=(const T* a, const BasicString<Derived, T, Traits>& b) {
        return !(a < b);
    }

    // Addition operator

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicString<Derived, T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        BasicString<Derived, T, Traits> result = a;
        result.Append(b);
        return result;
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicString<Derived, T, Traits>& a, const T* b) {
        BasicString<Derived, T, Traits> result = a;
        result.Append(b);
        return result;
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicString<Derived, T, Traits>& a, T b) {
        BasicString<Derived, T, Traits> result = a;
        result.PushBack(b);
        return result;
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicString<Derived, T, Traits>& a, const BasicStringView<T, Traits>& b) {
        BasicString<Derived, T, Traits> result = a;
        result.Append(b);
        return result;
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const T* a, const BasicString<Derived, T, Traits>& b) {
        BasicString<Derived, T, Traits> result = b;
        result.Insert(0, a);
        return result;
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(T a, const BasicString<Derived, T, Traits>& b) {
        BasicString<Derived, T, Traits> result = b;
        result.Insert(result.Begin(), a);
        return result;
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicStringView<T, Traits>& a, const BasicString<Derived, T, Traits>& b) {
        BasicString<Derived, T, Traits> result = b;
        result.Insert(0, a);
        return result;
    }

    #ifdef __WSTL_CXX11__
    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(BasicString<Derived, T, Traits>&& a, BasicString<Derived, T, Traits>&& b) {
        a.Append(b);
        return Move(a);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(BasicString<Derived, T, Traits>&& a, const BasicString<Derived, T, Traits>& b) {
        a.Append(b);
        return Move(a);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(BasicString<Derived, T, Traits>&& a, const T* b) {
        a.Append(b);
        return Move(a);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(BasicString<Derived, T, Traits>&& a, T b) {
        a.PushBack(b);
        return Move(a);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(BasicString<Derived, T, Traits>&& a, const BasicStringView<T, Traits>& b) {
        a.Append(b);
        return Move(a);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicString<Derived, T, Traits>& a, BasicString<Derived, T, Traits>&& b) {
        b.Insert(0, a);
        return Move(b);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const T* a, BasicString<Derived, T, Traits>&& b) {
        b.Insert(0, a);
        return Move(b);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(T a, BasicString<Derived, T, Traits>&& b) {
        b.Insert(b.Begin(), a);
        return Move(b);
    }

    template<typename Derived, typename T, typename Traits>
    BasicString<Derived, T, Traits> operator+(const BasicStringView<T, Traits>& a, BasicString<Derived, T, Traits>&& b) {
        b.Insert(0, a);
        return Move(b);
    }
    #endif

    // Erase

    template<typename Derived, typename T, typename Traits, typename U>
    typename BasicString<Derived, T, Traits>::SizeType Erase(BasicString<Derived, T, Traits>& container, const U& value) {
        typedef typename BasicString<Derived, T, Traits>::Iterator IteratorType;
        IteratorType iterator = Remove(container.Begin(), container.End(), value);

        IteratorType result = container.End() - iterator;
        container.Erase(result);

        return result;
    }

    // Erase if

    template<typename Derived, typename T, typename Traits, typename Predicate>
    typename BasicString<Derived, T, Traits>::SizeType EraseIf(BasicString<Derived, T, Traits>& container, Predicate predicate) {
        typedef typename BasicString<Derived, T, Traits>::Iterator IteratorType;
        IteratorType iterator = RemoveIf(container.Begin(), container.End(), predicate);

        IteratorType result = container.End() - iterator;
        container.Erase(result);
        
        return result;
    }
}

#endif