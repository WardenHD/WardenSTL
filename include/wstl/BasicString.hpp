// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s basic_string.h,
// but it has been re-implemented with custom logic for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_BASICSTRING_HPP__
#define __WSTL_BASICSTRING_HPP__

#include "private/Platform.hpp"
#include "CharacterTraits.hpp"
#include "Limits.hpp"
#include "Container.hpp"
#include "StandardExceptions.hpp"
#include "StringView.hpp"
#include "Memory.hpp"


/// @def __WSTL_STRING_CLEAR_UNUSED__
/// @brief If defined, unused portion of the string buffer is filled with zeros every time the content is modified

/// @def __WSTL_STRING_TRUNCATION_CHECK__
/// @brief If defined, string will check for trucation on operations that may cause it and store a truncation flag
/// @details Also enables `.IsTruncated()` and `.ClearTruncated()` methods for checking and clearing the flag

/// @def __WSTL_STRING_TRUNCATION_ERROR__
/// @brief If defined, the exception `LengthError` will be thrown when a truncation occurs

namespace wstl {
    // No position (npos) constant

    namespace string {
        #ifdef __WSTL_CXX11__
        /// @brief Special constant indicating no position with value `SizeType(-1)`
        /// @ingroup string
        /// @see https://en.cppreference.com/w/cpp/string/basic_string
        static constexpr size_t NoPosition = NumericLimits<size_t>::Max();

        #else
        /// @brief Special constant indicating no position with value `SizeType(-1)`
        /// @ingroup string
        /// @see https://en.cppreference.com/w/cpp/string/basic_string
        enum {
            NoPosition = NumericLimits<size_t>::Max()
        };
        #endif
    }

    // Basic string

    /// @brief A common base class for string types
    /// @tparam Derived The derived string type
    /// @tparam T Character type
    /// @tparam Traits Character traits type
    /// @ingroup string
    /// @see https://en.cppreference.com/w/cpp/string/basic_string
    template<typename Derived, typename T, typename Traits = CharacterTraits<T> >
    class BasicString;

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

        /// @brief Special constant indicating no position with value `SizeType(-1)`
        static const __WSTL_CONSTEXPR__ SizeType NoPosition = NumericLimits<SizeType>::Max();

        /// @brief Accesses the character at specified position with bounds checking
        /// @param position Position of the character to access
        /// @return Reference to that character
        ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position < this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String index out of range"));
            return m_Buffer[position];
        }

        /// @brief Accesses the character at specified position with bounds checking
        /// @param position Position of the character to access
        /// @return Const reference to that character
        ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position < this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String index out of range"));
            return m_Buffer[position];
        }

        /// @brief Subscript operator
        /// @param position Position of the character to access
        /// @return Reference to that character
        ReferenceType operator[](SizeType position) {
            return m_Buffer[position];
        }

        /// @brief Subscript operator
        /// @param position Position of the character to access
        /// @return Const reference to that character
        ConstReferenceType operator[](SizeType position) const {
            return m_Buffer[position];
        }

        /// @brief Returns reference to the first character
        ReferenceType Front() {
            return m_Buffer[0];
        }

        /// @brief Returns const reference to the first character
        ConstReferenceType Front() const {
            return m_Buffer[0];
        }

        /// @brief Returns reference to the last character
        ReferenceType Back() {
            return m_Buffer[this->m_CurrentSize - 1];
        }

        /// @brief Returns const reference to the last character
        ConstReferenceType Back() const {
            return m_Buffer[this->m_CurrentSize - 1];
        }

        /// @brief Returns pointer to the internal character buffer
        PointerType Data() __WSTL_NOEXCEPT__ {
            return m_Buffer;
        }

        /// @brief Returns const pointer to the internal character buffer
        __WSTL_CONSTEXPR__ ConstPointerType Data() const __WSTL_NOEXCEPT__ {
            return m_Buffer;
        }

        /// @brief Returns C-style string (null-terminated), same as `Data()`
        ConstPointerType CString() const {
            return m_Buffer;
        }

        /// @brief Conversion operator to `BasicStringView`
        operator BasicStringView<T, Traits>() const __WSTL_NOEXCEPT__ {
            return BasicStringView<T, Traits>(Data(), this->Size());
        }

        /// @brief Returns an iterator to the first character
        Iterator Begin() {
            return m_Buffer;
        }

        /// @brief Returns a const iterator to the first character
        ConstIterator Begin() const {
            return m_Buffer;
        }

        /// @brief Returns a const iterator to the first character
        ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return m_Buffer;
        }

        /// @brief Returns an iterator to one past the last character
        Iterator End() {
            return m_Buffer + this->m_CurrentSize;
        }

        /// @brief Returns a const iterator to one past the last character
        ConstIterator End() const {
            return m_Buffer + this->m_CurrentSize;
        }

        /// @brief Returns a const iterator to one past the last character
        ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return m_Buffer + this->m_CurrentSize;
        }

        /// @brief Returns a reverse iterator to the first character (one past the last character in normal order)
        ReverseIterator ReverseBegin() {
            return ReverseIterator(m_Buffer + this->m_CurrentSize);
        }

        /// @brief Returns a const reverse iterator to the first character (one past the last character in normal order)
        ConstReverseIterator ReverseBegin() const {
            return ConstReverseIterator(m_Buffer + this->m_CurrentSize);
        }

        /// @brief Returns a const reverse iterator to the first character (one past the last character in normal order)
        ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Buffer + this->m_CurrentSize);
        }

        /// @brief Returns a reverse iterator to one past the last character (first character in normal order)
        ReverseIterator ReverseEnd() {
            return ReverseIterator(m_Buffer);
        }

        /// @brief Returns a const reverse iterator to one past the last character (first character in normal order)
        ConstReverseIterator ReverseEnd() const {
            return ConstReverseIterator(m_Buffer);
        }

        /// @brief Returns a const reverse iterator to one past the last character (first character in normal order)
        ConstReverseIterator ConstReverseEnd() const {
            return ConstReverseIterator(m_Buffer);
        }

        /// @brief Returns the length of the string
        SizeType Length() const {
            return this->m_CurrentSize;
        }

        /// @brief Clears the string content
        void Clear() {
            this->m_CurrentSize = 0;
            m_Buffer[0] = 0;

            #ifdef __WSTL_STRING_CLEAR_UNUSED__
            memory::Set(m_Buffer, this->m_Capacity, 0);
            #endif
        }

        /// @brief Returns a substring of the string
        /// @param position Starting position of the substring, default is `0` (from the beginning)
        /// @param count Number of characters in the substring, default is `NoPosition` (to the end of the string)
        /// @return The resulting substring
        Derived Substring(SizeType position = 0, SizeType count = NoPosition) const {
            Derived result;

            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Substring position out of range"), result);
            result.Assign(m_Buffer + position, Min(count, this->Size() - position));

            return result;
        }

        /// @brief Inserts `count` characters at specified position
        /// @param index Position to insert at (number)
        /// @param count Number of characters to insert
        /// @param ch Character to insert
        /// @return Reference to this string
        /// @throws `OutOfRange` if `index` is greater than the string size
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

        /// @brief Inserts a range of characters at specified position
        /// @param position Position to insert at (iterator)
        /// @param first Iterator to the first character to insert
        /// @param last Iterator to one past the last character to insert
        /// @return Iterator to the first inserted character, or `position` if insertion failed
        /// @throws `OutOfRange` if `position` is greater than the string size
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

        /// @brief Inserts a C-style string at specified position
        /// @param index Position to insert at (number)
        /// @param string C-style string to insert
        /// @return Reference to this string
        BasicString& Insert(SizeType index, const T* string) {
            Insert(ConstBegin() + index, string, string + Traits::Length(string));
            return *this;
        }

        /// @brief Inserts a C-style string with specified length at specified position
        /// @param index Position to insert at (number)
        /// @param string C-style string to insert
        /// @param count Number of characters from the C-style string to insert
        /// @return Reference to this string
        BasicString& Insert(SizeType index, const T* string, SizeType count) {
            Insert(ConstBegin() + index, string, string + count);
            return *this;
        }

        /// @brief Inserts another string at specified position
        /// @param index Position to insert at (number)
        /// @param string String to insert
        /// @return Reference to this string
        template<typename UDerived, typename UTraits>
        BasicString& Insert(SizeType index, const BasicString<UDerived, T, UTraits>& string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            Insert(ConstBegin() + index, string.Begin(), string.End());
            return *this;
        }

        /// @brief Inserts a substring of another string at specified position
        /// @param index Position to insert at (number)
        /// @param string String to insert from
        /// @param stringIndex Starting position in the string to insert from
        /// @param count Number of characters to insert, default is `NoPosition` (to the end of the string)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `stringIndex` is greater than the size of `string`
        template<typename UDerived, typename UTraits>
        BasicString& Insert(SizeType index, const BasicString<UDerived, T, UTraits>& string, SizeType stringIndex, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(stringIndex <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Inserted string index out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            Insert(index, string.Data() + stringIndex, Min(count, string.Size() - stringIndex));
            return *this;
        }

        /// @brief Inserts a character at specified position
        /// @param position Position to insert at (iterator)
        /// @param ch Character to insert
        /// @return Iterator to the inserted character, or `position` if insertion failed
        /// @throws `OutOfRange` if `position` is greater than the string size
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

        /// @brief Inserts `count` characters at specified position
        /// @param position Position to insert at (iterator)
        /// @param count Number of characters to insert
        /// @param ch Character to insert
        /// @return Iterator to the first inserted character
        Iterator Insert(ConstIterator position, SizeType count, T ch) {
            Insert(Distance(ConstBegin(), position), count, ch);
            return const_cast<Iterator>(position);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Inserts an initializer list of characters at specified position
        /// @param position Position to insert at (iterator)
        /// @param list Initializer list of characters to insert
        /// @return Iterator to the first inserted character, or `position` if insertion failed
        /// @throws `OutOfRange` if `position` is greater than the string size
        /// @note Requires `__WSTL_NO_INITIALIZERLIST__` to be undefined
        /// @since C++11
        Iterator Insert(ConstIterator position, InitializerList<T> list) {
            return Insert(position, list.Begin(), list.End());
        }
        #endif

        /// @brief Inserts a string view at specified position
        /// @param index Position to insert at (number)
        /// @param view String view to insert
        /// @return Reference to this string
        /// @throws `OutOfRange` if `index` is greater than the string size
        template<typename UTraits>
        BasicString& Insert(SizeType index, const BasicStringView<T, UTraits>& view) {
            Insert(Begin() + index, view.Begin(), view.End());
            return *this;
        }

        /// @brief Inserts a substring of a string view at specified position
        /// @param index Position to insert at (number)
        /// @param view String view to insert from
        /// @param viewIndex Starting position in the string view to insert from
        /// @param count Number of characters to insert, default is `NoPosition` (to the end of the string view)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `viewIndex` is greater than the size of `view`, or if `index` is greater than the size of this string
        template<typename UTraits>
        BasicString& Insert(SizeType index, const BasicStringView<T, UTraits>& view, SizeType viewIndex, SizeType count = NoPosition) {
            Insert(index, view.Substring(viewIndex, count));
            return *this;
        }

        /// @brief Erases a range of characters from the string
        /// @param first Iterator to the first character to erase
        /// @param last Iterator to one past the last character to erase
        /// @return Iterator to the `last` position prior to erasure
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

        /// @brief Erases a character at specified position
        /// @param position Position to erase (iterator)
        /// @return Iterator that follows the removed character
        Iterator Erase(ConstIterator position) {
            Iterator iterator = const_cast<Iterator>(position);

            memory::Move(iterator + 1, End(), iterator);
            m_Buffer[--this->m_CurrentSize] = 0;

            return iterator;
        }

        /// @brief Erases a range of characters from the string
        /// @param index Starting position to erase from, default is `0` (from the beginning)
        /// @param count Number of characters to erase, default is `NoPosition` (to the end of the string)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `index` is greater than the string size
        BasicString& Erase(SizeType index = 0, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(index <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String erase index out of range"), *this);
            Erase(Begin() + index, Begin() + index + Min(count, this->Size() - index));
            return *this;
        }

        /// @brief Appends a character to the end of the string
        /// @param ch Character to append
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

        /// @brief Removes the last character from the string
        void PopBack() {
            if(!this->Empty()) m_Buffer[--this->m_CurrentSize] = 0;
        }

        /// @brief Appends a range of characters to the end of the string
        /// @param first Iterator to the first character to append
        /// @param last Iterator to one past the last character to append
        /// @return Reference to this string
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

        /// @brief Appends `count` characters to the end of the string
        /// @param count Number of characters to append
        /// @param ch Character to append
        /// @return Reference to this string
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

        /// @brief Appends a C-style string with specified length to the end of the string
        /// @param string C-style string to append
        /// @param count Number of characters from the C-style string to append
        /// @return Reference to this string
        BasicString& Append(const T* string, SizeType count) {
            return Append(string, string + count);
        }

        /// @brief Appends a C-style string to the end of the string
        /// @param string C-style string to append
        /// @return Reference to this string
        BasicString& Append(const T* string) {
            return Append(string, Traits::Length(string));
        }

        /// @brief Appends a string view to the end of the string
        /// @param view String view to append
        /// @return Reference to this string
        template<typename UTraits>
        BasicString& Append(const BasicStringView<T, UTraits>& view) {
            return Append(view.Begin(), view.End());
        }

        /// @brief Appends a substring of a string view to the end of the string
        /// @param view String view to append from
        /// @param position Starting position in the string view to append from, default is `0` (from the beginning)
        /// @param count Number of characters to append, default is `NoPosition` (to the end of the string view)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of `view`
        template<typename UTraits>
        BasicString& Append(const BasicStringView<T, UTraits>& view, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Appended string view position out of range"), *this);
            return Append(view.Substring(position, count));
        }

        /// @brief Appends another string to the end of the string
        /// @param string String to append
        /// @return Reference to this string
        template<typename UDerived, typename UTraits>
        BasicString& Append(const BasicString<UDerived, T, UTraits>& string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            return Append(string.Begin(), string.End());
        }

        /// @brief Appends a substring of another string to the end of the string
        /// @param string String to append from
        /// @param position Starting position in the string to append from
        /// @param count Number of characters to append, default is `NoPosition` (to the end of the string)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of `string`
        template<typename UDerived, typename UTraits>
        BasicString& Append(const BasicString<UDerived, T, UTraits>& string, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Appended string position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            m_Truncated |= string.IsTruncated();
            #endif

            return Append(string.Data() + position, Min(count, string.Size() - position));
        }
        
        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Appends an initializer list of characters to the end of the string
        /// @param list Initializer list of characters to append
        /// @return Reference to this string
        /// @note Requires `__WSTL_NO_INITIALIZERLIST__` to be undefined
        /// @since C++11
        BasicString& Append(InitializerList<T> list) {
            return Append(list.Begin(), list.End());
        }
        #endif
        
        /// @brief Addition combination operator for another string
        /// @param string String to append
        /// @return Reference to this string
        template<typename UDerived, typename UTraits>
        BasicString& operator+=(const BasicString<UDerived, T, UTraits>& string) {
            return Append(string);
        }

        /// @brief Addition combination operator for character
        /// @param ch Character to append
        /// @return Reference to this string
        BasicString& operator+=(T ch) {
            PushBack(ch);
            return *this;
        }

        /// @brief Addition combination operator for C-style string
        /// @param string C-style string to append
        /// @return Reference to this string
        BasicString& operator+=(const T* string) {
            return Append(string);
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Addition combination operator for initializer list of characters
        /// @param list Initializer list of characters to append
        /// @return Reference to this string
        /// @note Requires `__WSTL_NO_INITIALIZERLIST__` to be undefined
        /// @since C++11
        BasicString& operator+=(InitializerList<T> list) {
            return Append(list);
        }
        #endif
        
        /// @brief Addition combination operator for string view
        /// @param view String view to append
        /// @return Reference to this string
        template<typename UTraits>
        BasicString& operator+=(const BasicStringView<T, UTraits>& view) {
            return Append(view);
        }

        /// @brief Replaces a range of characters with a range of characters from iterators
        /// @details If `first == last`, the insertion is performed, if `first2 == last2`, the erasure is performed
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param first2 Iterator to the first character to replace with
        /// @param last2 Iterator to one past the last character to replace with
        /// @return Reference to this string
        template<typename InputIterator>
        BasicString& Replace(ConstIterator first, ConstIterator last, InputIterator first2, InputIterator last2) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, first2, Distance(first2, last2), false);
            #else
            return ReplaceImpl(first, last, first2, Distance(first2, last2));
            #endif
        }

        /// @brief Replaces a range of characters with another string
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param string String to replace with
        /// @return Reference to this string
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
        
        /// @brief Replaces a range of characters with another string
        /// @details If `first == last`, the insertion is performed instead
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param string String to replace with
        /// @return Reference to this string
        template<typename UDerived, typename UTraits>
        BasicString& Replace(ConstIterator first, ConstIterator last, const BasicString<UDerived, T, UTraits>& string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, string.CString(), string.Size(), string.IsTruncated());
            #else
            return ReplaceImpl(first, last, string.CString(), string.Size());
            #endif
        }

        /// @brief Replaces a range of characters with a substring of another string
        /// @details If `count` is `0`, the insertion is performed instead
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param string String to replace with
        /// @param position2 Starting position in the string to replace from
        /// @param count2 Number of characters to replace from, default is `NoPosition` (to the end of the string)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position2` is greater than the size of `string` or `position` is greater than the size of this string
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

        /// @brief Replaces a range of characters with a C-style string with specified length
        /// @details If `count` is `0`, the insertion is performed instead, if `string` is `nullptr` or `count2` is `0`, the erasure is performed
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param string C-style string to replace with
        /// @param count2 Number of characters from the C-style string to replace with
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of this string
        BasicString& Replace(SizeType position, SizeType count, const T* string, SizeType count2) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, count2, false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, count2);
            #endif
        }

        /// @brief Replaces a range of characters with a C-style string with specified length
        /// @details If `first == last`, the insertion is performed instead, if `string` is `nullptr` or `count2` is `0`, the erasure is performed
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param string C-style string to replace with
        /// @param count2 Number of characters from the C-style string to replace with
        /// @return Reference to this string
        BasicString& Replace(ConstIterator first, ConstIterator last, const T* string, SizeType count2) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, string, count2, false);
            #else
            return ReplaceImpl(first, last, string, count2);
            #endif
        }

        /// @brief Replaces a range of characters with a C-style string
        /// @details If `first == last`, the insertion is performed instead, if `string` is `nullptr`, the erasure is performed
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param string C-style string to replace with
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of this string
        BasicString& Replace(SizeType position, SizeType count, const T* string) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, Traits::Length(string), false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), string, Traits::Length(string));
            #endif
        }

        /// @brief Replaces a range of characters with a C-style string
        /// @details If `first == last`, the insertion is performed instead, if `string` is `nullptr`, the erasure is performed
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param string C-style string to replace with
        /// @return Reference to this string
        BasicString& Replace(ConstIterator first, ConstIterator last, const T* string) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, string, Traits::Length(string), false);
            #else
            return ReplaceImpl(first, last, string, Traits::Length(string));
            #endif
        }

        /// @brief Replaces a range of characters with `count2` characters
        /// @details If `count` is `0`, the insertion is performed instead, if `count2` is `0`, the erasure is performed
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param count2 Number of characters to replace with
        /// @param ch Character to replace with
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of this string
        BasicString& Replace(SizeType position, SizeType count, SizeType count2, T ch) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            Erase(position, Min(count, this->Size() - position));
            Insert(position, count2, ch);
            return *this;
        }

        /// @brief Replaces a range of characters with `count2` characters
        /// @details If `first == last`, the insertion is performed instead, if `count2` is `0`, the erasure is performed
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param count2 Number of characters to replace with
        /// @param ch Character to replace with
        /// @return Reference to this string
        BasicString& Replace(ConstIterator first, ConstIterator last, SizeType count2, T ch) {
            Erase(first, last);
            Insert(first, count2, ch);
            return *this;
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Replaces a range of characters with an initializer list of characters
        /// @details If `first == last`, the insertion is performed instead
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param list Initializer list of characters to replace with
        /// @return Reference to this string
        /// @note Requires `__WSTL_NO_INITIALIZERLIST__` to be undefined
        /// @since C++11
        BasicString& Replace(ConstIterator first, ConstIterator last, InitializerList<T> list) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, list.Begin(), list.Size(), false);
            #else
            return ReplaceImpl(first, last, list.Begin(), list.Size());
            #endif
        }
        #endif

        /// @brief Replaces a range of characters with a string view
        /// @details If `first == last`, the insertion is performed instead
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param view String view to replace with
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of this string
        template<typename UTraits>
        BasicString& Replace(SizeType position, SizeType count, const BasicStringView<T, UTraits>& view) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);
            
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), view.Data(), view.Size(), false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), view.Data(), view.Size());
            #endif
        }

        /// @brief Replaces a range of characters with a string view
        /// @details If `first == last`, the insertion is performed instead
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param view String view to replace with
        /// @return Reference to this string
        template<typename UTraits>
        BasicString& Replace(ConstIterator first, ConstIterator last, const BasicStringView<T, UTraits>& view) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, view.Data(), view.Size(), false);
            #else
            return ReplaceImpl(first, last, view.Data(), view.Size());
            #endif
        }

        /// @brief Replaces a range of characters with a substring of a string view
        /// @details If `count` is `0`, the insertion is performed instead
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param view String view to replace with
        /// @param position2 Starting position in the string view to replace from
        /// @param count2 Number of characters to replace from, default is `NoPosition` (to the end of the string view)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position2` is greater than the size of `view` or `position` is greater than the size of this string
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

        /// @brief Replaces a range of characters with a literal C-style string
        /// @details If `count` is `0`, the insertion is performed instead
        /// @param position Starting position to replace at
        /// @param count Number of characters to replace
        /// @param literal Literal C-style string to replace with
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of this string
        template<size_t N>
        BasicString& Replace(SizeType position, SizeType count, const ValueType (&literal)[N]) {
            __WSTL_ASSERT_RETURNVALUE__(position <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String replace position out of range"), *this);

            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), literal, N, false);
            #else
            return ReplaceImpl(Begin() + position, Begin() + position + Min(count, this->Size() - position), literal, N);
            #endif
        }

        /// @brief Replaces a range of characters with a literal C-style string
        /// @details If `first == last`, the insertion is performed instead
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param literal Literal C-style string to replace with
        /// @return Reference to this string
        template<size_t N>
        BasicString& Replace(ConstIterator first, ConstIterator last, const ValueType (&literal)[N]) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            return ReplaceImpl(first, last, literal, N, false);
            #else
            return ReplaceImpl(first, last, literal, N);
            #endif
        }

        /// @brief Copies a range of characters to a destination buffer
        /// @param destination Destination buffer to copy to
        /// @param count Number of characters to copy
        /// @param position Starting position to copy from, default is `0` (from the beginning)
        /// @return Number of characters actually copied
        SizeType Copy(T* destination, SizeType count, SizeType position = 0) const {
            if(position > this->Size()) return 0;

            count = Min(count, this->Size() - position);
            memory::Move(m_Buffer + position, count, destination);

            return count;
        }

        /// @brief Resizes the string to contain `count` characters
        /// @param count New size of the string
        /// @param ch Character to fill new characters with (if the new size is bigger)
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

        /// @brief Resizes the string to contain `count` characters, new characters are zero-initialized
        /// @param count New size of the string
        void Resize(SizeType count) {
            Resize(count, 0);
        }

        /// @brief Resizes the string to contain at most `count` characters and overwrites them using the provided operation
        /// @tparam Operation Type of the operation, must be callable with signature `SizeType(PointerType data, SizeType count)`
        /// @param count The maximum possible new size of the string
        /// @param operation Operation to overwrite the string data, must return the actual new size of the string
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
        
        /// @brief Resizes the string to contain `count` characters without initializing new characters, except null-terminator
        /// @param count New size of the string
        void UninitializedResize(SizeType count) {
            this->m_CurrentSize = count;
            m_Buffer[this->m_CurrentSize] = 0;
        }

        /// @brief Swaps the contents of this string with another string
        /// @param other Other string to swap with
        void Swap(BasicString& other) {
            #ifdef __WSTL_STRING_TRUNCATION_CHECK__
            wstl::Swap(m_Truncated, other.m_Truncated);
            #endif

            wstl::Swap(this->m_CurrentSize, other.m_CurrentSize);
            SwapRanges(m_Buffer, m_Buffer + this->m_Capacity, other.m_Buffer);
        }

        /// @brief Assigns a range of characters from iterators to the string
        /// @param first Iterator to the first character to assign
        /// @param last Iterator to one past the last character to assign
        /// @return Reference to this string
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
        /// @brief Assigns a range of characters from a movable string to the string
        /// @param string Movable string to assign from
        /// @return Reference to this string
        /// @since C++11
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

        /// @brief Assigns another string to the string
        /// @param string String to assign from
        /// @return Reference to this string
        template<typename UDerived, typename UTraits>
        BasicString& Assign(const BasicString<UDerived, T, UTraits>& string) {
            return Assign(string.Begin(), string.End());
        }

        /// @brief Assigns `count` characters to the string
        /// @param count Number of characters to assign
        /// @param value Character to assign
        /// @return Reference to this string
        BasicString& Assign(SizeType count, T value) {
            Clear();
            Resize(count, value);
            return *this;
        }

        /// @brief Assigns a C-style string with specified length to the string
        /// @param string C-style string to assign from
        /// @param count Number of characters from the C-style string to assign
        /// @return Reference to this string
        BasicString& Assign(const T* string, SizeType count) {
            return Assign(string, string + count);
        }

        /// @brief Assigns a C-style string to the string
        /// @param string C-style string to assign from
        /// @return Reference to this string
        BasicString& Assign(const T* string) {
            return Assign(string, string + Traits::Length(string));
        }

        /// @brief Assigns a string view to the string
        /// @param view String view to assign from
        /// @return Reference to this string
        template<typename UTraits>
        BasicString& Assign(const BasicStringView<T, UTraits>& view) {
            return Assign(view.Begin(), view.End());
        }

        /// @brief Assigns a substring of a string view to the string
        /// @param view String view to assign from
        /// @param position Starting position in the string view to assign from
        /// @param count Number of characters to assign, default is `NoPosition` (to the end of the string view)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of `view`
        template<typename UTraits>
        BasicString& Assign(const BasicStringView<T, UTraits>& view, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String assign position out of range"), *this);
            return Assign(view.Begin() + position, Min(count, view.Size() - position));
        }

        /// @brief Assigns a substring of another string to the string
        /// @param string String to assign from
        /// @param position Starting position in the string to assign from
        /// @param count Number of characters to assign, default is `NoPosition` (to the end of the string)
        /// @return Reference to this string
        /// @throws `OutOfRange` if `position` is greater than the size of `string`
        template<typename UDerived, typename UTraits>
        BasicString& Assign(const BasicString<UDerived, T, UTraits>& string, SizeType position, SizeType count = NoPosition) {
            __WSTL_ASSERT_RETURNVALUE__(position <= string.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String assign position out of range"), *this);
            return Assign(string.Begin() + position, Min(count, string.Size() - position));
        }

        #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
        /// @brief Assigns an initializer list of characters to the string
        /// @param list Initializer list of characters to assign
        /// @return Reference to this string
        /// @note Requires `__WSTL_NO_INITIALIZERLIST__` to be undefined
        /// @since C++11
        BasicString& Assign(InitializerList<T> list) {
            return Assign(list.Begin(), list.End());
        }
        #endif

        /// @brief Copy assignment operator from another string
        /// @param string String to assign from
        /// @return Reference to this string
        template<typename UDerived, typename UTraits>
        BasicString& operator=(const BasicString<UDerived, T, UTraits>& string) {
            return Assign(string);
        }

        /// @brief Copy assignment operator from a C-style string
        /// @param string C-style string to assign from
        /// @return Reference to this string
        BasicString& operator=(const ValueType* string) {
            return Assign(string);
        }

        /// @brief Copy assignment operator from a character
        /// @param value Character to assign
        /// @return Reference to this string
        BasicString& operator=(ValueType value) {
            return Assign(1, value);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator from another string
        /// @param string Movable string to assign from
        /// @return Reference to this string
        /// @since C++11
        template<typename UDerived, typename UTraits>
        BasicString& operator=(BasicString<UDerived, T, UTraits>&& string) {
            return Assign(Move(string));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Copy assignment operator from an initializer list of characters
        /// @param list Initializer list of characters to assign
        /// @return Reference to this string
        /// @note Requires `__WSTL_NO_INITIALIZERLIST__` to be undefined
        /// @since C++11
        BasicString& operator=(InitializerList<T> list) {
            return Assign(list);
        }
        #endif
        #endif

        /// @brief Copy assignment operator from a string view
        /// @param view String view to assign from
        /// @return Reference to this string
        template<typename UTraits>
        BasicString& operator=(const BasicStringView<T, UTraits>& view) {
            return Assign(view);
        }

        /// @brief Deleted assignment operator from `nullptr`
        BasicString& operator=(NullPointerType) __WSTL_DELETE__;

        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        /// @brief Clears the truncated flag
        /// @note Requires `__WSTL_STRING_TRUNCATION_CHECK__` to be defined
        void ClearTruncated() {
            m_Truncated = false;
        }

        /// @brief Gets whether the string has been truncated
        /// @note Requires `__WSTL_STRING_TRUNCATION_CHECK__` to be defined
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

        /// @brief Finds the first occurrence of a string view
        /// @param view Substring to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
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

        /// @brief Finds the last occurrence of a substring, searching backwards
        /// @param string Substring to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
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

        /// @brief Finds the first occurrence of any character from a C-style string
        /// @param string C-style string containing characters to find
        /// @param position Position to start the search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType FindFirstOf(const T* string, SizeType position, SizeType count) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i) {
                    for(size_t j = 0; j < count; ++j)
                        if(m_Buffer[i] == string[j]) return i;
                }
            }

            return NoPosition;
        }

        /// @brief Finds the first occurrence of a character
        /// @param ch Character to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType FindFirstOf(T ch, SizeType position = 0) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i) 
                    if(m_Buffer[i] == ch) return i;
            }

            return NoPosition;
        }

        /// @brief Finds the first occurrence of any character from another string
        /// @param string String containing characters to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        template<typename UDerived, typename UTraits>
        SizeType FindFirstOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = 0) const {
            return FindFirstOf(string.CString(), position, string.Size());
        }

        /// @brief Finds the first occurrence of any character from a C-style string
        /// @param string C-style string containing characters to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType FindFirstOf(const T* string, SizeType position = 0) const {
            return FindFirstOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the first occurrence of any character from a string view
        /// @param view String view containing characters to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        template<typename UTraits>
        SizeType FindFirstOf(const BasicStringView<T, UTraits>& view, SizeType position = 0) const {
            return FindFirstOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the last occurrence of any character from a C-style string, searching backwards
        /// @param string C-style string containing characters to find
        /// @param position Position to start the backward search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the last occurrence, or `NoPosition` if not found
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

        /// @brief Finds the last occurrence of a character, searching backwards
        /// @param ch Character to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType FindLastOf(T ch, SizeType position = NoPosition) const {
            if(this->Empty()) return NoPosition;

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position)
                if(m_Buffer[position] == ch) return position;

            return NoPosition;
        }

        /// @brief Finds the last occurrence of any character from another string, searching backwards
        /// @param string String containing characters to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        template<typename UDerived, typename UTraits>
        SizeType FindLastOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = NoPosition) const {
            return FindLastOf(string.CString(), position, string.Size());
        }

        /// @brief Finds the last occurrence of any character from a C-style string, searching backwards
        /// @param string C-style string containing characters to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType FindLastOf(const T* string, SizeType position = NoPosition) const {
            return FindLastOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the last occurrence of any character from a string view, searching backwards
        /// @param view String view containing characters to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        template<typename UTraits>
        SizeType FindLastOf(const BasicStringView<T, UTraits>& view, SizeType position = NoPosition) const {
            return FindLastOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the first occurrence of any character not in a C-style string
        /// @param string C-style string containing characters to not find
        /// @param position Position to start the search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the first occurrence, or `NoPosition` if not found
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

        /// @brief Finds the first occurrence of a character not equal to the given character
        /// @param ch Character to not find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType FindFirstNotOf(T ch, SizeType position = 0) const {
            if(position < this->Size()) {
                for(size_t i = position; i < this->Size(); ++i)
                    if(m_Buffer[i] != ch) return i;
            }

            return NoPosition;
        }

        /// @brief Finds the first occurrence of any character not in another string
        /// @param string String containing characters to not find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        template<typename UDerived, typename UTraits>
        SizeType FindFirstNotOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = 0) const {
            return FindFirstNotOf(string.CString(), position);
        }

        /// @brief Finds the first occurrence of any character not in a C-style string
        /// @param string C-style string containing characters to not find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        SizeType FindFirstNotOf(const T* string, SizeType position = 0) const {
            return FindFirstNotOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the first occurrence of any character not in a string view
        /// @param view String view containing characters to not find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        template<typename UTraits>
        SizeType FindFirstNotOf(const BasicStringView<T, UTraits>& view, SizeType position = 0) const {
            return FindFirstNotOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the last occurrence of any character not in a C-style string, searching backwards
        /// @param string C-style string containing characters to not find
        /// @param position Position to start the backward search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the last occurrence, or `NoPosition` if not found
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

        /// @brief Finds the last occurrence of a character not equal to the given character, searching backwards
        /// @param ch Character to not find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType FindLastNotOf(T ch, SizeType position = NoPosition) const {
            if(this->Empty()) return NoPosition;

            position = Min(position, this->Size() - 1);

            ConstReverseIterator iterator = ConstReverseIterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position)
                if(m_Buffer[position] != ch) return position;

            return NoPosition;
        }

        /// @brief Finds the last occurrence of any character not in another string, searching backwards
        /// @param string String containing characters to not find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        template<typename UDerived, typename UTraits>
        SizeType FindLastNotOf(const BasicString<UDerived, T, UTraits>& string, SizeType position = NoPosition) const {
            return FindLastNotOf(string.CString(), position, string.Size());
        }

        /// @brief Finds the last occurrence of any character not in a C-style string, searching backwards
        /// @param string C-style string containing characters to not find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        SizeType FindLastNotOf(const T* string, SizeType position = NoPosition) const {
            return FindLastNotOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the last occurrence of any character not in a string view, searching backwards
        /// @param view String view containing characters to not find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        template<typename UTraits>
        SizeType FindLastNotOf(const BasicStringView<T, UTraits>& view, SizeType position = NoPosition) const {
            return FindLastNotOf(view.Data(), position, view.Size());
        }

        /// @brief Compares this string with another string
        /// @param other Other string to compare with
        /// @return Negative value if less than, zero if equal, positive value if greater than
        template<typename UDerived, typename UTraits>
        int Compare(const BasicString<UDerived, T, UTraits>& other) const {
            return Compare(m_Buffer, this->Size(), other.Data(), other.Size());
        }

        /// @brief Compares a substring of this string with another string
        /// @param position1 Starting position in this string
        /// @param count1 Number of characters from this string to compare
        /// @param other Other string to compare with
        /// @return Negative value if less than, zero if equal, positive value if greater than
        /// @throws `OutOfRange` if `position1` is greater than the size of this string
        template<typename UDerived, typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicString<UDerived, T, UTraits>& other) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), other.Data(), other.Size());
        }

        /// @brief Compares a substring of this string with a substring of another string
        /// @param position1 Starting position in this string
        /// @param count1 Number of characters from this string to compare
        /// @param other Other string to compare with
        /// @param position2 Starting position in the other string
        /// @param count2 Number of characters from the other string to compare (default is `NoPosition`, which means the end)
        /// @return Negative value if less than, zero if equal, positive value if greater than
        /// @throws `OutOfRange` if `position1` is greater than the size of this string, or `position2` is greater than the size of `other`
        template<typename UDerived, typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicString<UDerived, T, UTraits>& other, SizeType position2, SizeType count2 = NoPosition) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            __WSTL_ASSERT__(position2 <= other.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position2 out of range"));

            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), other.Data() + position2, Min(count2, other.Size() - position2));
        }

        /// @brief Compares this string with a C-style string
        /// @param string C-style string to compare with
        /// @return Negative value if less than, zero if equal, positive value if greater than
        int Compare(const T* string) const {
            return Compare(m_Buffer, this->Size(), string, Traits::Length(string));
        }

        /// @brief Compares a substring of this string with a C-style string
        /// @param position1 Starting position in this string
        /// @param count1 Number of characters from this string to compare
        /// @param string C-style string to compare with
        /// @return Negative value if less than, zero if equal, positive value if greater than
        /// @throws `OutOfRange` if `position1` is greater than the size of this string
        int Compare(SizeType position1, SizeType count1, const T* string) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), string, Traits::Length(string));
        }

        /// @brief Compares a substring of this string with a C-style string with specified length
        /// @param position1 Starting position in this string
        /// @param count1 Number of characters from this string to compare
        /// @param string C-style string to compare with
        /// @param count2 Number of characters from the C-style string to compare
        /// @return Negative value if less than, zero if equal, positive value if greater than
        /// @throws `OutOfRange` if `position1` is greater than the size of this string
        int Compare(SizeType position1, SizeType count1, const T* string, SizeType count2) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), string, count2);
        }

        /// @brief Compares this string with a string view
        /// @param view String view to compare with
        /// @return Negative value if less than, zero if equal, positive value if greater than
        template<typename UTraits>
        int Compare(const BasicStringView<T, UTraits>& view) const {
            return Compare(m_Buffer, this->Size(), view.Data(), view.Size());
        }

        /// @brief Compares a substring of this string with a string view
        /// @param position1 Starting position in this string
        /// @param count1 Number of characters from this string to compare
        /// @param view String view to compare with
        /// @return Negative value if less than, zero if equal, positive value if greater than
        /// @throws `OutOfRange` if `position1` is greater than the size of this string
        template<typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicStringView<T, UTraits>& view) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), view.Data(), view.Size());
        }

        /// @brief Compares a substring of this string with a substring of a string view
        /// @param position1 Starting position in this string
        /// @param count1 Number of characters from this string to compare
        /// @param view String view to compare with
        /// @param position2 Starting position in the string view
        /// @param count2 Number of characters from the string view to compare (default is `NoPosition`, which means the end)
        /// @return Negative value if less than, zero if equal, positive value if greater than
        /// @throws `OutOfRange` if `position1` is greater than the size of this string, or `position2` is greater than the size of `view`
        template<typename UTraits>
        int Compare(SizeType position1, SizeType count1, const BasicStringView<T, UTraits>& view, SizeType position2, SizeType count2 = NoPosition) const {
            __WSTL_ASSERT__(position1 <= this->Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String compare position1 out of range"));
            __WSTL_ASSERT__(position2 <= view.Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String view compare position2 out of range"));

            return Compare(m_Buffer + position1, Min(count1, this->Size() - position1), view.Data() + position2, Min(count2, view.Size() - position2));
        }

        /// @brief Checks if this string starts with a given string
        /// @param string String to check
        template<typename UDerived, typename UTraits>
        bool StartsWith(const BasicString<UDerived, T, UTraits>& string) const {
            return Compare(0, string.Size(), string) == 0;
        }

        /// @brief Checks if this string starts with a given string view
        /// @param view String view to check
        template<typename UTraits>
        bool StartsWith(const BasicStringView<T, UTraits>& view) const {
            return Compare(0, view.Size(), view) == 0;
        }

        /// @brief Checks if this string starts with a given C-style string
        /// @param string C-style string to check
        bool StartsWith(const T* string) const {
            SizeType length = Traits::Length(string);
            return (this->Size() >= length) && (Compare(0, length, string) == 0);
        }

        /// @brief Checks if this string starts with a given character
        /// @param ch Character to check
        bool StartsWith(T ch) const {
            return !this->Empty() && Traits::Equal(Front(), ch);
        }

        /// @brief Checks if this string ends with a given string
        /// @param string String to check
        template<typename UDerived, typename UTraits>
        bool EndsWith(const BasicString<UDerived, T, UTraits>& string) const {
            return (this->Size() >= string.Size()) && (Compare(this->Size() - string.Size(), NoPosition, string) == 0);
        }

        /// @brief Checks if this string ends with a given string view
        /// @param view String view to check
        template<typename UTraits>
        bool EndsWith(const BasicStringView<T, UTraits>& view) const {
            return (this->Size() >= view.Size()) && (Compare(this->Size() - view.Size(), NoPosition, view) == 0);
        }

        /// @brief Checks if this string ends with a given C-style string
        /// @param string C-style string to check
        bool EndsWith(const T* string) const {
            SizeType length = Traits::Length(string);
            return (this->Size() >= length) && (Compare(this->Size() - length, NoPosition, string) == 0);
        }
        
        /// @brief Checks if this string ends with a given character
        /// @param ch Character to check
        bool EndsWith(T ch) const {
            return !this->Empty() && Traits::Equal(Back(), ch);
        }

        /// @brief Checks if this string contains a given string
        /// @param string String to check
        template<typename UDerived, typename UTraits>
        bool Contains(const BasicString<UDerived, T, UTraits>& string) const {
            return Find(string) != NoPosition;
        }

        /// @brief Checks if this string contains a given string view
        /// @param string String view to check
        template<typename UTraits>
        bool Contains(const BasicStringView<T, UTraits>& view) const {
            return Find(view) != NoPosition;
        }

        /// @brief Checks if this string contains a given C-style string
        /// @param string C-style string to check
        bool Contains(const T* string) const {
            return Find(string) != NoPosition;
        }
        
        /// @brief Checks if this string contains a given character
        /// @param ch Character to check
        bool Contains(T ch) const {
            return Find(ch) != NoPosition;
        }

    protected:
        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        /// @brief Protected constructor
        /// @param buffer Pointer to the character buffer
        /// @param capacity Capacity of the buffer
        BasicString(T* buffer, SizeType capacity) : TypedContainerBase<T>(capacity), m_Buffer(buffer), m_Truncated(false) {
            m_Buffer[0] = 0;
        }

        #else
        /// @brief Protected constructor
        /// @param buffer Pointer to the character buffer
        /// @param capacity Capacity of the buffer
        BasicString(T* buffer, SizeType capacity) : TypedContainerBase<T>(capacity), m_Buffer(buffer) {
            m_Buffer[0] = 0;
        }

        #endif

        /// @brief Destructor, clears the buffer if configured
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

        /// @brief Copies characters from source to destination
        /// @param source Source iterator
        /// @param count Number of characters to copy
        /// @param destination Destination iterator
        /// @return Iterator to the position after the last copied character in the destination
        template<typename Iterator1, typename Iterator2>
        static typename EnableIf<IsPointer<Iterator1>::Value && IsPointer<Iterator2>::Value, Iterator2>::Type
        CopyCharacters(Iterator1 source, size_t count, Iterator2 destination) {
            memory::Move(source, count, destination);
            return destination + count;
        }

        /// @brief Copies characters from source to destination
        /// @param source Source iterator
        /// @param count Number of characters to copy
        /// @param destination Destination iterator
        /// @return Iterator to the position after the last copied character in the destination
        template<typename Iterator1, typename Iterator2>
        inline static typename EnableIf<!IsPointer<Iterator1>::Value || !IsPointer<Iterator2>::Value, Iterator2>::Type
        CopyCharacters(Iterator1 source, size_t count, Iterator2 destination) {
            return CopyInRange(source, count, destination);
        }

        /// @brief Compares two character sequences, static helper
        /// @param first1 Pointer to the first character sequence
        /// @param count1 Number of characters in the first sequence
        /// @param first2 Pointer to the second character sequence
        /// @param count2 Number of characters in the second sequence
        /// @return Negative value if less than, zero if equal, positive value if greater than
        static int Compare(ConstPointerType first1, SizeType count1, ConstPointerType first2, SizeType count2) {
            if(count1 > count2) return 1;
            if(count1 < count2) return -1;

            return string::CompareInRange(first1, first2, count1); // we don't care whether count1 or count2, here they are considered equal
        }

        #ifdef __WSTL_STRING_TRUNCATION_CHECK__
        /// @brief Internal replace implementation
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param string Pointer to the C-style string to insert
        /// @param count Number of characters from the C-style string to insert
        /// @param truncated Whether truncation has already occurred before
        /// @return Reference to this string
        /// @note Requires `__WSTL_STRING_TRUNCATION_CHECK__` to be defined
        BasicString& ReplaceImpl(ConstIterator first, ConstIterator last, const T* string, SizeType count, bool truncated) {
        #else
        /// @brief Internal replace implementation
        /// @param first Iterator to the first character to replace
        /// @param last Iterator to one past the last character to replace
        /// @param string Pointer to the C-style string to insert
        /// @param count Number of characters from the C-style string to insert
        /// @return Reference to this string
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

    template<typename Derived, typename T, typename Traits>
    const __WSTL_CONSTEXPR__ typename BasicString<Derived, T, Traits>::SizeType BasicString<Derived, T, Traits>::NoPosition;

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
}

#endif