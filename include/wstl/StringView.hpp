#ifndef __WSTL_STRINGVIEW_HPP__
#define __WSTL_STRINGVIEW_HPP__

#include "private/Platform.hpp"
#include "CharacterTraits.hpp"
#include "StandardExceptions.hpp"
#include "Limits.hpp"
#include "Memory.hpp"
#include "Hash.hpp"

#include <stdint.h>

/// @defgroup string_view String View
/// Non-owning view of a contiguous sequence of characters
/// @ingroup containers


namespace wstl {
    /// @brief Base class for non-owning view of a contiguous sequence of characters
    /// @tparam T Character type
    /// @tparam Traits Character traits class
    /// @ingroup string_view
    /// @see https://en.cppreference.com/w/cpp/string/basic_string_view.html
    template<typename T, typename Traits = CharacterTraits<T> >
    class BasicStringView {
    public:
        typedef Traits TraitsType;
        typedef T ValueType;
        typedef size_t SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef const T* ConstIterator;
        typedef const T* Iterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        enum {
            /// @brief Special value indicating that no matches were found or that the position is invalid
            NoPosition = NumericLimits<SizeType>::Max()
        };

        /// @brief Default constructor
        __WSTL_CONSTEXPR__ BasicStringView() : m_Data(NullPointer), m_Size(0) {}

        /// @brief Copy constructor
        /// @param other String view to copy from
        __WSTL_CONSTEXPR__ BasicStringView(const BasicStringView& other) : m_Data(other.m_Data), m_Size(other.m_Size) __WSTL_NOEXCEPT__ {}

        /// @brief Constuctor from C-style string and length
        /// @param string C-style string pointer
        /// @param count Number of characters in the string
        __WSTL_CONSTEXPR__ BasicStringView(const T* string, SizeType count) : m_Data(string), m_Size(count) {}

        /// @brief Constuctor from C-style string
        /// @param string C-style string pointer
        __WSTL_CONSTEXPR__ BasicStringView(const T* string) : m_Data(string), m_Size(Traits::Length(string)) {}

        /// @brief Constructor from iterators
        /// @param first Iterator to the first character
        /// @param last Iterator to one past the last character
        __WSTL_CONSTEXPR__ BasicStringView(Iterator first, Iterator last) : m_Data(first), m_Size(Distance(first, last)) {}

        /// @brief Copy assignment operator
        /// @param other String view to copy from
        __WSTL_CONSTEXPR14__ BasicStringView& operator=(const BasicStringView& other) {
            m_Data = other.m_Data;
            m_Size = other.m_Size;
            return *this;
        }

        /// @brief Returns an iterator to the first character
        __WSTL_CONSTEXPR__ ConstIterator Begin() const __WSTL_NOEXCEPT__ {
            return m_Data;
        }

        /// @brief Returns a const iterator to the first character
        __WSTL_CONSTEXPR__ ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return m_Data;
        }

        /// @brief Returns an iterator to one past the last character
        __WSTL_CONSTEXPR__ ConstIterator End() const __WSTL_NOEXCEPT__ {
            return m_Data + m_Size;
        }

        /// @brief Returns a const iterator to one past the last character
        __WSTL_CONSTEXPR__ ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return m_Data + m_Size;
        }

        /// @brief Returns a reverse iterator to the first character
        __WSTL_CONSTEXPR__ ConstReverseIterator ReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Data + m_Size);
        }

        /// @brief Returns a const reverse iterator to the first character
        __WSTL_CONSTEXPR__ ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Data + m_Size);
        }

        /// @brief Returns a reverse iterator to one before the last character
        __WSTL_CONSTEXPR__ ConstReverseIterator ReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Data);
        }

        /// @brief Returns a const reverse iterator to one before the last character
        __WSTL_CONSTEXPR__ ConstReverseIterator ConstReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Data);
        }

        /// @brief Access operator
        /// @param index Index of the character to access
        __WSTL_CONSTEXPR__ ConstReferenceType operator[](SizeType index) const {
            return m_Data[index];
        }

        /// @brief Returns a reference to the character at specified position with bounds checking
        /// @param position Position of the character to access
        /// @return Reference to the character at specified position
        /// @throws LogicError if the string view is null, OutOfRange if the position is out of range
        ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(m_Data != NullPointer, WSTL_MAKE_EXCEPTION(LogicError, "String view is null"));
            __WSTL_ASSERT__(position < Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "String view index out of range"));
            return m_Data[position];
        }

        /// @brief Returns a reference to the first character
        __WSTL_CONSTEXPR__ ConstReferenceType Front() const {
            return *m_Data;
        }

        /// @brief Returns a reference to the last character
        __WSTL_CONSTEXPR__ ConstReferenceType Back() const {
            return *(m_Data + m_Size - 1);
        }

        /// @brief Returns a pointer to the underlying character array
        __WSTL_CONSTEXPR__ ConstPointerType Data() const __WSTL_NOEXCEPT__ {
            return m_Data;
        }

        /// @brief Returns the size of the string view
        __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__ {
            return m_Size;
        }

        /// @brief Returns the length of the string view
        __WSTL_CONSTEXPR__ SizeType Length() const __WSTL_NOEXCEPT__ {
            return m_Size;
        }

        /// @brief Returns the maximum size of the string view
        __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__ {
            return m_Size;
        }

        /// @brief Checks if the string view is empty
        __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__ {
            return m_Size == 0;
        }

        /// @brief Removes the first `count` characters from the string view
        /// @param count Number of characters to remove from the start
        __WSTL_CONSTEXPR14__ void RemovePrefix(SizeType count) {
            m_Data += count;
            m_Size -= count;
        }

        /// @brief Removes the last `count` characters from the string view
        /// @param count Number of characters to remove from the end
        __WSTL_CONSTEXPR14__ void RemoveSuffix(SizeType count) {
            m_Size -= count;
        }

        /// @brief Swaps the contents of this string view with another
        /// @param other String view to swap with
        __WSTL_CONSTEXPR14__ void Swap(BasicStringView& other) {
            wstl::Swap(m_Data, other.m_Data);
            wstl::Swap(m_Size, other.m_Size);
        }

        /// @brief Copies characters from the string view to a destination buffer
        /// @param destination Pointer to the destination buffer
        /// @param count Number of characters to copy
        /// @param position Position in the string view to start copying from (default is 0)
        /// @return Number of characters actually copied
        __WSTL_CONSTEXPR__ SizeType Copy(PointerType destination, SizeType count, SizeType position = 0) const {
            size_t toCopy = 0;

            if(position < Size()) {
                toCopy = Min(count, Size() - position);
                memory::Move(position, toCopy, destination);
            }

            return toCopy;
        }

        /// @brief Returns a substring of the string view
        /// @param position Starting position of the substring (default is 0)
        /// @param count Number of characters in the substring (default is NoPosition, which means until the end)
        /// @return A new string view representing the substring
        __WSTL_CONSTEXPR__ BasicStringView Substring(SizeType position = 0, SizeType count = NoPosition) const {
            BasicStringView result;
            
            __WSTL_ASSERT_RETURNVALUE__(position <= Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Substring position out of range"), result);
            result = BasicStringView(m_Data + position, Min(count, Size() - position));

            return result;
        }
        
        /// @brief Compares this string view with another
        /// @param other String view to compare with
        /// @return 0 if equal, negative if this is less, positive if this is greater
        __WSTL_CONSTEXPR14__ int Compare(BasicStringView other) const __WSTL_NOEXCEPT__ {
            SizeType size1 = Size();
            SizeType size2 = other.Size();

            if(size1 > size2) return 1;
            if(size1 < size2) return -1;

            return string::CompareInRange(Data(), other.Data(), size1); // we don't care whether size1 or size2, here they are considered equal
        }

        /// @brief Compares a substring of this string view with another string view
        /// @param position1 Starting position of the substring in this string view
        /// @param count1 Number of characters in the substring of this string view
        /// @param view String view to compare with
        /// @return 0 if equal, negative if this is less, positive if this is greater
        __WSTL_CONSTEXPR14__ int Compare(SizeType position1, SizeType count1, BasicStringView view) const {
            return Substring(position1, count1).Compare(view);
        }

        /// @brief Compares a substring of this string view with a substring of another string view
        /// @param position1 Starting position of the substring in this string view
        /// @param count1 Number of characters in the substring of this string view
        /// @param view String view to compare with
        /// @param position2 Starting position of the substring in the other string view
        /// @param count2 Number of characters in the substring of the other string view
        /// @return 0 if equal, negative if this is less, positive if this is greater
        __WSTL_CONSTEXPR14__ int Compare(SizeType position1, SizeType count1, BasicStringView view, SizeType position2, SizeType count2) const {
            return Substring(position1, count1).Compare(view.Substring(position2, count2));
        }

        /// @brief Compares this string view with a C-style string
        /// @param string C-style string to compare with
        /// @return 0 if equal, negative if this is less, positive if this is greater
        __WSTL_CONSTEXPR14__ int Compare(const T* string) const {
            return Compare(BasicStringView(string));
        }

        /// @brief Compares a substring of this string view with a C-style string
        /// @param position1 Starting position of the substring in this string view
        /// @param count1 Number of characters in the substring of this string view
        /// @param string C-style string to compare with
        /// @return 0 if equal, negative if this is less, positive if this is greater
        __WSTL_CONSTEXPR14__ int Compare(SizeType position1, SizeType count1, const T* string) const {
            return Substring(position1, count1).Compare(BasicStringView(string));
        }

        /// @brief Compares a substring of this string view with a substring of a C-style string with length
        /// @param position1 Starting position of the substring in this string view
        /// @param count1 Number of characters in the substring of this string view
        /// @param string C-style string to compare with
        /// @param count2 Number of characters in the substring of the C-style string
        /// @return 0 if equal, negative if this is less, positive if this is greater
        __WSTL_CONSTEXPR14__ int Compare(SizeType position1, SizeType count1, const T* string, SizeType count2) const {
            return Substring(position1, count1).Compare(BasicStringView(string, count2));
        }

        /// @brief Checks if the string view starts with a specified string view
        /// @param view String view to check
        __WSTL_CONSTEXPR14__ bool StartsWith(BasicStringView view) const __WSTL_NOEXCEPT__ {
            return (Size() >= view.Size()) && (Compare(0, view.Size(), view) == 0);
        }

        /// @brief Checks if the string view starts with a specified character
        /// @param ch Character to check
        __WSTL_CONSTEXPR14__ bool StartsWith(T ch) const __WSTL_NOEXCEPT__ {
            return !Empty() && Traits::Equal(Front(), ch);
        }

        /// @brief Checks if the string view starts with a specified C-style string
        /// @param string C-style string to check
        __WSTL_CONSTEXPR14__ bool StartsWith(const T* string) const {
            SizeType length = Traits::Length(string);
            return (Size() >= length) && (Compare(0, length, string) == 0);
        }

        /// @brief Checks if the string view ends with a specified string view
        /// @param view String view to check
        __WSTL_CONSTEXPR14__ bool EndsWith(BasicStringView view) const __WSTL_NOEXCEPT__ {
            return (Size() >= view.Size()) && (Compare(Size() - view.Size(), NoPosition, view) == 0);
        }
        
        /// @brief Checks if the string view ends with a specified character
        /// @param ch Character to check
        __WSTL_CONSTEXPR14__ bool EndsWith(T ch) const __WSTL_NOEXCEPT__ {
            return !Empty() && Traits::Equal(Back(), ch);
        }

        /// @brief Checks if the string view ends with a specified C-style string
        /// @param string C-style string to check
        __WSTL_CONSTEXPR14__ bool EndsWith(const T* string) const {
            SizeType length = Traits::Length(string);
            return (Size() >= length) && (Compare(Size() - length, NoPosition, string) == 0);
        }

        /// @brief Finds the first occurrence of a C-style string
        /// @param string C-style string to find
        /// @param position Position to start the search from (default is 0)
        /// @param count Number of characters in the C-style string
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType Find(const T* string, SizeType position, SizeType count) const {
            if(position + count > Size()) return NoPosition;

            ConstIterator iterator = Search(Begin() + position, End(), string, string + count);
            if(iterator == End()) return NoPosition;

            return Distance(Begin(), iterator);
        }

        /// @brief Finds the first occurrence of a character
        /// @param ch Character to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType Find(T ch, SizeType position = 0) const __WSTL_NOEXCEPT__ {
            if(position + 1 > Size()) return NoPosition;

            ConstIterator iterator = wstl::Find(Begin() + position, End(), ch);
            if(iterator == End()) return NoPosition;

            return Distance(Begin(), iterator);
        }

        /// @brief Finds the first occurrence of a substring
        /// @param view Substring to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType Find(BasicStringView view, SizeType position = 0) const __WSTL_NOEXCEPT__ {
            return Find(view.Data(), position, view.Size());
        }

        /// @brief Finds the first occurrence of a C-style string
        /// @param string C-style string to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType Find(const T* string, SizeType position = 0) const {
            return Find(string, position, Traits::Length(string));
        }

        /// @brief Finds the last occurrence of a character, searching backwards
        /// @param ch Character to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType ReverseFind(T ch, SizeType position = NoPosition) const __WSTL_NOEXCEPT__ {
            position = Size() - Min(position, Size());

            ConstReverseIterator iterator = wstl::Find(ReverseBegin() + position, ReverseEnd(), ch);
            if(iterator == ReverseEnd()) return NoPosition;

            return Distance(Begin(), iterator.Base() - 1);
        }

        /// @brief Finds the last occurrence of a C-style string, searching backwards
        /// @param string C-style string to find
        /// @param position Position to start the backward search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType ReverseFind(const T* string, SizeType position, SizeType count) const {
            if(count > Size()) return NoPosition;

            ConstReverseIterator reverseBegin(string + count);
            ConstReverseIterator reverseEnd(string);

            position = Size() - Min(position, Size());
            
            ConstReverseIterator iterator = Search(ReverseBegin() + position, ReverseEnd(), reverseBegin, reverseEnd);
            if(iterator == ReverseEnd()) return NoPosition;

            return Distance(Begin(), iterator.Base() - 1);
        }

        /// @brief Finds the last occurrence of a substring, searching backwards
        /// @param view Substring to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType ReverseFind(BasicStringView view, SizeType position = NoPosition) const __WSTL_NOEXCEPT__ {
            return ReverseFind(view.Data(), position, view.Size());
        }

        /// @brief Finds the last occurrence of a C-style string, searching backwards
        /// @param string C-style string to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType ReverseFind(const T* string, SizeType position = NoPosition) const {
            return ReverseFind(string, position, Traits::Length(string));
        }

        /// @brief Finds the first occurrence of a character
        /// @param ch Character to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstOf(T ch, SizeType position = 0) const __WSTL_NOEXCEPT__ {
            if(position < Size()) {
                for(size_t i = position; i < this->Size(); ++i) 
                    if(m_Data[i] == ch) return i;
            }

            return NoPosition;
        }

        /// @brief Finds the first occurrence of any character from a C-style string
        /// @param string C-style string containing characters to find
        /// @param position Position to start the search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstOf(const T* string, SizeType position, SizeType count) const {
            if(position < Size()) {
                for(size_t i = position; i < Size(); ++i) {
                    for(size_t j = 0; j < count; ++j)
                        if(m_Data[i] == string[j]) return i;
                }
            }

            return NoPosition;
        }

        /// @brief Finds the first occurrence of any character from a substring
        /// @param view Substring containing characters to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstOf(BasicStringView view, SizeType position = 0) const __WSTL_NOEXCEPT__ {
            return FindFirstOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the first occurrence of any character from a C-style string
        /// @param string C-style string containing characters to find
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstOf(const T* string, SizeType position = 0) const {
            return FindFirstOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the last occurrence of any character from a C-style string, searching backwards
        /// @param string C-style string containing characters to find
        /// @param position Position to start the backward search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastOf(const T* string, SizeType position, SizeType count) const {
            if(Empty()) return NoPosition;

            position = Min(position, Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position) {
                for(size_t j = 0; j < count; ++j)
                    if(m_Data[position] == string[j]) return position;
            }

            return NoPosition;
        }

        /// @brief Finds the last occurrence of a character, searching backwards
        /// @param ch Character to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastOf(T ch, SizeType position = NoPosition) const __WSTL_NOEXCEPT__ {
            if(Empty()) return NoPosition;

            position = Min(position, Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position)
                if(m_Data[position] == ch) return position;

            return NoPosition;
        }

        /// @brief Finds the last occurrence of any character from a substring, searching backwards
        /// @param view Substring containing characters to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastOf(BasicStringView view, SizeType position = NoPosition) const __WSTL_NOEXCEPT__ {
            return FindLastOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the last occurrence of any character from a C-style string, searching backwards
        /// @param string C-style string containing characters to find
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last occurrence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastOf(const T* string, SizeType position = NoPosition) const {
            return FindLastOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the first absence of any character from a C-style string
        /// @param string C-style string containing characters to avoid
        /// @param position Position to start the search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the first absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstNotOf(const T* string, SizeType position, SizeType count) const {
            if(position < Size()) {
                for(size_t i = position; i < Size(); ++i) {
                    bool found = false;

                    for(size_t j = 0; j < count; ++j) {
                        if(m_Data[i] == string[j]) {
                            found = true;
                            break;
                        }
                    }

                    if(!found) return i;
                }
            }

            return NoPosition;
        }

        /// @brief Finds the first absence of a character
        /// @param ch Character to avoid
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstNotOf(T ch, SizeType position = 0) const __WSTL_NOEXCEPT__ {
            if(position < Size()) {
                for(size_t i = position; i < Size(); ++i)
                    if(m_Data[i] != ch) return i;
            }

            return NoPosition;
        }

        /// @brief Finds the first absence of any character from a substring
        /// @param view Substring containing characters to avoid
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstNotOf(BasicStringView view, SizeType position = 0) const __WSTL_NOEXCEPT__ {
            return FindFirstNotOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the first absence of any character from a C-style string
        /// @param string C-style string containing characters to avoid
        /// @param position Position to start the search from (default is 0)
        /// @return The position of the first absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindFirstNotOf(const T* string, SizeType position = 0) const {
            return FindFirstNotOf(string, position, Traits::Length(string));
        }

        /// @brief Finds the last absence of any character from a C-style string, searching backwards
        /// @param string C-style string containing characters to avoid
        /// @param position Position to start the backward search from
        /// @param count Number of characters in the C-style string
        /// @return The position of the last absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastNotOf(const T* string, SizeType position, SizeType count) const {
            if(Empty()) return NoPosition;

            position = Min(position, Size() - 1);

            ConstReverseIterator iterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position) {
                bool found = false;

                for(size_t j = 0; j < count; ++j) {
                    if(m_Data[position] == string[j]) {
                        found = true;
                        break;
                    }
                }

                if(!found) return position;
            }

            return NoPosition;
        }

        /// @brief Finds the last absence of a character, searching backwards
        /// @param ch Character to avoid
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastNotOf(T ch, SizeType position = NoPosition) const __WSTL_NOEXCEPT__ {
            if(Empty()) return NoPosition;

            position = Min(position, Size() - 1);

            ConstReverseIterator iterator = ConstReverseIterator(Begin() + position + 1);

            for(; iterator != ReverseEnd(); ++iterator, --position)
                if(m_Data[position] != ch) return position;

            return NoPosition;
        }

        /// @brief Finds the last absence of any character from a substring, searching backwards
        /// @param view Substring containing characters to avoid
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastNotOf(BasicStringView view, SizeType position = NoPosition) const __WSTL_NOEXCEPT__ {
            return FindLastNotOf(view.Data(), position, view.Size());
        }

        /// @brief Finds the last absence of any character from a C-style string, searching backwards
        /// @param string C-style string containing characters to avoid
        /// @param position Position to start the backward search from (default is `NoPosition`, which means the end)
        /// @return The position of the last absence, or `NoPosition` if not found
        __WSTL_CONSTEXPR14__ SizeType FindLastNotOf(const T* string, SizeType position = NoPosition) const {
            return FindLastNotOf(string, position, Traits::Length(string));
        }

        /// @brief Checks if the string view contains a specified string view
        /// @param view String view to check
        __WSTL_CONSTEXPR14__ bool Contains(BasicStringView view) const __WSTL_NOEXCEPT__ {
            return Find(view) != NoPosition;
        }

        /// @brief Checks if the string view contains a specified character
        /// @param ch Character to check
        __WSTL_CONSTEXPR14__ bool Contains(T ch) const __WSTL_NOEXCEPT__ {
            return Find(ch) != NoPosition;
        }

        /// @brief Checks if the string view contains a specified C-style string
        /// @param string C-style string to check
        __WSTL_CONSTEXPR14__ bool Contains(const T* string) const __WSTL_NOEXCEPT__ {
            return Find(string) != NoPosition;
        }
    
    private:
        ConstPointerType m_Data;
        SizeType m_Size;

        /// @brief Disabled constructor for NullPointerType
        BasicStringView(NullPointerType) {}

        /// @brief Static helper for Compare function
        /// @param first1 Pointer to the first character of the first string
        /// @param count1 Number of characters in the first string
        /// @param first2 Pointer to the first character of the second string
        /// @param count2 Number of characters in the second string
        /// @return 0 if equal, negative if first or its size is less, positive if first or its size is greater
        static int Compare(ConstPointerType first1, SizeType count1, ConstPointerType first2, SizeType count2) {
            if(count1 > count2) return 1;
            if(count1 < count2) return -1;

            return string::CompareInRange(first1, first2, count1); // we don't care whether count1 or count2, here they are equal
        }
    };

    // Type aliases

    /// @brief String view for `char` type
    /// @ingroup string_view
    typedef BasicStringView<char> StringView;

    /// @brief String view for `wchar_t` type
    /// @ingroup string_view
    typedef BasicStringView<wchar_t> WideStringView;

    #ifdef __WSTL_CXX11__
    /// @brief String view for `char16_t` type
    /// @ingroup string_view
    /// @since C++11
    typedef BasicStringView<char16_t> U16StringView;

    /// @brief String view for `char32_t` type
    /// @ingroup string_view
    /// @since C++11
    typedef BasicStringView<char32_t> U32StringView;
    #endif

    #ifdef __WSTL_CXX20__
    /// @brief String view for `char8_t` type
    /// @ingroup string_view
    /// @since C++20
    typedef BasicStringView<char8_t> U8StringView;
    #endif

    // Comparison operators

    template<typename T, typename Traits>
    inline __WSTL_CONSTEXPR14__ bool operator==(const BasicStringView<T, Traits>& a, const BasicStringView<T, Traits>& b) {
        return (a.Size() == b.Size()) && Equal(a.Begin(), a.End(), b.Begin());
    }

    template<typename T, typename Traits>
    inline __WSTL_CONSTEXPR14__ bool operator!=(const BasicStringView<T, Traits>& a, const BasicStringView<T, Traits>& b) {
        return !(a == b);
    }

    template<typename T, typename Traits>
    inline __WSTL_CONSTEXPR14__ bool operator<(const BasicStringView<T, Traits>& a, const BasicStringView<T, Traits>& b) {
        return LexicographicalCompare(a.Begin(), a.End(), b.Begin(), b.End());
    }

    template<typename T, typename Traits>
    inline __WSTL_CONSTEXPR14__ bool operator>(const BasicStringView<T, Traits>& a, const BasicStringView<T, Traits>& b) {
        return b < a;
    }

    template<typename T, typename Traits>
    inline __WSTL_CONSTEXPR14__ bool operator<=(const BasicStringView<T, Traits>& a, const BasicStringView<T, Traits>& b) {
        return !(a > b);
    }

    template<typename T, typename Traits>
    inline __WSTL_CONSTEXPR14__ bool operator>=(const BasicStringView<T, Traits>& a, const BasicStringView<T, Traits>& b) {
        return !(a < b);
    }
    
    // Literal operators

    #ifdef __WSTL_CXX11__

    namespace __private {
        using size_t = decltype(sizeof 0);
    }

    inline namespace literals {
        inline namespace string_view_literals {
            __WSTL_CONSTEXPR__ StringView operator""_sv(const char* string, __private::size_t length) __WSTL_NOEXCEPT__ {
                return StringView{string, length};
            }

            __WSTL_CONSTEXPR__ WideStringView operator""_sv(const wchar_t* string, __private::size_t length) __WSTL_NOEXCEPT__ {
                return WideStringView{string, length};
            }

            __WSTL_CONSTEXPR__ U16StringView operator""_sv(const char16_t* string, __private::size_t length) __WSTL_NOEXCEPT__ {
                return U16StringView{string, length};
            }

            __WSTL_CONSTEXPR__ U32StringView operator""_sv(const char32_t* string, __private::size_t length) __WSTL_NOEXCEPT__ {
                return U32StringView{string, length};
            }

            #ifdef __WSTL_CXX20__
            __WSTL_CONSTEXPR__ U8StringView operator""_sv(const char8_t* string, __private::size_t length) __WSTL_NOEXCEPT__ {
                return U8StringView{string, length};
            }
            #endif
        }
    }
    #endif

    // Make string view

    /// @brief Creates a string view from a C-style string (array)
    /// @param string C-style string (array)
    /// @return A string view representing the C-style string
    /// @ingroup string_view
    template<size_t N>
    __WSTL_CONSTEXPR14__ StringView MakeStringView(const char (&string)[N]) {
        return StringView(string, CharacterTraits<char>::Length(string, N - 1));
    }

    /// @brief Creates a wide string view from a wide C-style string (array)
    /// @param string Wide C-style string (array)
    /// @return A wide string view representing the wide C-style string
    /// @ingroup string_view
    template<size_t N>
    __WSTL_CONSTEXPR14__ WideStringView MakeStringView(const wchar_t (&string)[N]) {
        return WideStringView(string, CharacterTraits<wchar_t>::Length(string, N - 1));
    }

    #ifdef __WSTL_CXX11__
    /// @brief Creates a UTF-16 string view from a UTF-16 C-style string (array)
    /// @param string UTF-16 C-style string (array)
    /// @return A UTF-16 string view representing the UTF-16 C-style string
    /// @ingroup string_view
    /// @since C++11
    template<size_t N>
    __WSTL_CONSTEXPR14__ U16StringView MakeStringView(const char16_t (&string)[N]) {
        return U16StringView(string, CharacterTraits<char16_t>::Length(string, N - 1));
    }

    /// @brief Creates a UTF-32 string view from a UTF-32 C-style string (array)
    /// @param string UTF-32 C-style string (array)
    /// @return A UTF-32 string view representing the UTF-32 C-style string
    /// @ingroup string_view
    /// @since C++11
    template<size_t N>
    __WSTL_CONSTEXPR14__ U32StringView MakeStringView(const char32_t (&string)[N]) {
        return U32StringView(string, CharacterTraits<char32_t>::Length(string, N - 1));
    }
    #endif

    #ifdef __WSTL_CXX20__
    /// @brief Creates a UTF-8 string view from a UTF-8 C-style string (array)
    /// @param string UTF-8 C-style string (array)
    /// @return A UTF-8 string view representing the UTF-8 C-style string
    /// @ingroup string_view
    /// @since C++20
    template<size_t N>
    __WSTL_CONSTEXPR14__ U8StringView MakeStringView(const char8_t (&string)[N]) {
        return U8StringView(string, CharacterTraits<char8_t>::Length(string, N - 1));
    }
    #endif

    // Hash function

    template<>
    struct Hash<StringView> {
        size_t operator()(const StringView& view) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(view.Data()), reinterpret_cast<const uint8_t*>(view.Data() + view.Size()));
        }
    };

    template<>
    struct Hash<WideStringView> {
        size_t operator()(const WideStringView& view) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(view.Data()), reinterpret_cast<const uint8_t*>(view.Data() + view.Size()));
        }
    };

    #ifdef __WSTL_CXX11__
    template<>
    struct Hash<U16StringView> {
        size_t operator()(const U16StringView& view) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(view.Data()), reinterpret_cast<const uint8_t*>(view.Data() + view.Size()));
        }
    };

    template<>
    struct Hash<U32StringView> {
        size_t operator()(const U32StringView& view) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(view.Data()), reinterpret_cast<const uint8_t*>(view.Data() + view.Size()));
        }
    };
    #endif

    #ifdef __WSTL_CXX20__
    template<>
    struct Hash<U8StringView> {
        size_t operator()(const U8StringView& view) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(view.Data()), reinterpret_cast<const uint8_t*>(view.Data() + view.Size()));
        }
    };
    #endif
}

#endif