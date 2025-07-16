// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s array container implementation,
// and some concepts and functions have been adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_ARRAY_HPP__
#define __WSTL_ARRAY_HPP__

#include "private/Platform.hpp"
#include "private/TupleProperties.hpp"
#include "Iterator.hpp"
#include "Algorithm.hpp"
#include "private/Error.hpp"
#include "StandardExceptions.hpp"
#include <stddef.h>


/// @defgroup array Array
/// @brief A container that encapsulates a fixed size array
/// @ingroup containers

namespace wstl {
    // Array class

    /// @brief A container that encapsulates a fixed size array
    /// @tparam T Type of the elements in the array
    /// @tparam N Size of the array
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array
    template<typename T, size_t N>
    struct Array {
        typedef T ValueType;
        typedef size_t SizeType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        typedef typename Conditional<IsFundamental<T>::Value || IsPointer<T>::Value, T, 
        const T&>::Type ParameterType;

        T __m_Data[N];

        /// @brief Returns a reference to the element at the specified index with bounds checking
        /// @param index Index of the element to return
        /// @return Reference to the element at the specified index
        /// @throws IndexOutOfRange if the index is out of range
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType At(SizeType index) {
            __WSTL_ASSERT__(index < N,  WSTL_MAKE_EXCEPTION(OutOfRange));
            return __m_Data[index];
        }

        /// @brief Returns a const reference to the element at the specified index with bounds checking
        /// @param index Index of the element to return
        /// @return Const reference to the element at the specified index
        /// @throws IndexOutOfRange if the index is out of range
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType At(SizeType index) const {
            __WSTL_ASSERT__(index < N, WSTL_MAKE_EXCEPTION(OutOfRange));
            return __m_Data[index];
        }

        /// @brief Access operator
        /// @param index Index of the element to return
        /// @return Reference to the element at the specified index
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator[](SizeType index) {
            return __m_Data[index];
        }

        /// @brief Access operator
        /// @param index Index of the element to return
        /// @return Const reference to the element at the specified index
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType operator[](SizeType index) const {
            return __m_Data[index];
        }

        /// @brief Gets reference to the first element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType Front() {
            return __m_Data[0];
        }

        /// @brief Gets const reference to the first element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType Front() const {
            return __m_Data[0];
        }

        /// @brief Gets reference to the last element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType Back() {
            return __m_Data[N - 1];
        }

        /// @brief Gets const reference to the last element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType Back() const {
            return __m_Data[N - 1];
        }

        /// @brief Gets the data pointer of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ PointerType Data() __WSTL_NOEXCEPT__ {
            return __m_Data;
        }

        /// @brief Gets the const data pointer of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReferenceType Data() const __WSTL_NOEXCEPT__ {
            return __m_Data;
        }

        /// @brief Gets the iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Iterator Begin() __WSTL_NOEXCEPT__ {
            return __m_Data;
        }

        /// @brief Gets the iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator Begin() const __WSTL_NOEXCEPT__ {
            return __m_Data;
        }

        /// @brief Gets the const iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return Begin();
        }

        /// @brief Gets the iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Iterator End() __WSTL_NOEXCEPT__ {
            return __m_Data + N;
        }

        /// @brief Gets the iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator End() const __WSTL_NOEXCEPT__ {
            return __m_Data + N;
        }

        /// @brief Gets the const iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return End();
        }

        /// @brief Gets the reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
        ReverseIterator ReverseBegin() __WSTL_NOEXCEPT__ {
            return ReverseIterator(End());
        }

        /// @brief Gets the reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(End());
        }

        /// @brief Gets the const reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(End());
        }

        /// @brief Gets the reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
        ReverseIterator ReverseEnd() __WSTL_NOEXCEPT__ {
            return ReverseIterator(Begin());
        }

        /// @brief Gets the reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstIterator ReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(Begin());
        }

        /// @brief Gets the const reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ConstReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(Begin());
        }

        /// @brief Checks whether the array is empty
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__{
            return N == 0;
        }

        /// @brief Gets the size of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__{
            return N;
        }

        /// @brief Gets the maximum size of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__{
            return N;
        }

        /// @brief Fills the array with the specified value
        /// @param value The value to fill the array with
        __WSTL_CONSTEXPR14__ void Fill(ParameterType value) {
            wstl::Fill(Begin(), End(), value);
        }

        /// @brief Swaps the contents of the array with another array
        /// @param other The other array to swap with
        __WSTL_CONSTEXPR14__ void Swap(Array& other) __WSTL_NOEXCEPT__ {
            SwapRanges(Begin(), End(), other.Begin());
        }

        // Additional functions

        /// @brief Inserts a value at the specified position
        /// @param position Iterator to the position where the value should be inserted
        /// @param value The value to insert
        /// @return Iterator to the inserted value
        __WSTL_CONSTEXPR14__ Iterator Insert(Iterator position, ValueType value) {
            MoveBackward(position, End() - 1, End());
            *position = value;
            return position;
        }

        /// @brief Inserts a value at the specified index
        /// @param value The value to insert
        /// @param index The index where the value should be inserted
        /// @return Iterator to the inserted value
        __WSTL_CONSTEXPR14__ inline Iterator Insert(ValueType value, SizeType index) {
            return Insert(Begin() + index, value);
        }

        /// @brief Inserts a range of values at the specified position
        /// @param position Iterator to the position where the range should be inserted
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        /// @return Iterator to the first element of the inserted range
        __WSTL_CONSTEXPR14__ Iterator InsertRange(Iterator position, Iterator first, Iterator last) {
            MoveBackward(position, End() - Distance(first, last), End());
            Copy(first, last, position);
            return position;
        }

        /// @brief Inserts a range of values at the specified index
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        /// @param index The index where the range should be inserted
        /// @return Iterator to the first element of the inserted range
        __WSTL_CONSTEXPR14__ inline Iterator InsertRange(Iterator first, Iterator last, SizeType index) {
            return InsertRange(Begin() + index, first, last);
        }

        /// @brief Deletes a value at the specified position
        /// @param position Iterator to the position of the value to delete
        /// @param value The value to fill the deleted value with, default is 0 (array will be shifted anyway)
        /// @return Iterator to the position where the deleted value was
        __WSTL_CONSTEXPR14__ Iterator Delete(Iterator position, const T& value = 0) {
            *position = value;
            Move(position + 1, End(), position);
            return position;
        }

        /// @brief Deletes a value at the specified index
        /// @param index The index of the value to delete
        /// @param value The value to fill the deleted value with, default is 0 (array will be shifted anyway)
        /// @return Iterator to the position where the deleted value was
        __WSTL_CONSTEXPR14__ inline Iterator Delete(SizeType index, const T& value = 0) {
            return Delete(Begin() + index, value);
        }

        /// @brief Deletes a range of values
        /// @param first Iterator to the first element of the range to delete
        /// @param last Iterator to the last element of the range to delete
        /// @param value The value to fill the deleted range with, default is 0 (array will be shifted anyway)
        /// @return Iterator to the position where the beginning of the deleted range was
        __WSTL_CONSTEXPR14__ Iterator DeleteRange(Iterator first, Iterator last, const T& value = 0) {
            wstl::Fill(first, last, value);
            Move(last, End(), first);
            return first;
        }

        /// @brief Deletes a range of values
        /// @param first The index of the first element of the range to delete
        /// @param last The index of the last element of the range to delete
        /// @param value The value to fill the deleted range with, default is 0 (array will be shifted anyway)
        /// @return Iterator to the position where the beginning of the deleted range was
        __WSTL_CONSTEXPR14__ inline Iterator DeleteRange(SizeType first, SizeType last, const T& value = 0) {
            return DeleteRange(Begin() + first, Begin() + last, value);
        }
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T, typename... U>
    Array(T, U...) -> Array<T, sizeof...(U) + 1>;
    #endif

    // Swap specialization

    /// @brief Swaps the contents of two arrays
    /// @param a The first array
    /// @param b The second array
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array/swap2
    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__ inline void Swap(Array<T, N>& a, Array<T, N>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Tuple element specialization

    template<size_t I, typename T, size_t N>
    struct TupleElement<I, Array<T, N>> { typedef T Type; };

    // Tuple size specialization

    template<typename T, size_t N>
    struct TupleSize<Array<T, N>> : IntegralConstant<size_t, N> {};

    // Get specialization

    /// @brief Gets an element from array
    /// @tparam Index Index of element
    /// @param array Array from which to get element
    /// @return The element at the specified index
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array/get
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline T& Get(Array<T, N>& array) __WSTL_NOEXCEPT__ {
        StaticAssert(Index > N, "Index out of bounds");
        return array[Index];
    }

    /// @copydoc Get(Array<T, N>&)
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline const T& Get(const Array<T, N>& array) __WSTL_NOEXCEPT__ {
        StaticAssert(Index > N, "Index out of bounds");
        return array[Index];
    }

    #ifdef __WSTL_CXX11__
    /// @copydoc Get(Array<T, N>&)
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline T&& Get(Array<T, N>&& array) __WSTL_NOEXCEPT__ {
        StaticAssert(Index < N, "Index out of bounds");
        return Move(array[Index]);
    }

    /// @copydoc Get(Array<T, N>&)
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline const T&& Get(const Array<T, N>&& array) __WSTL_NOEXCEPT__ {
        StaticAssert(Index < N, "Index out of bounds");
        return Move(array[Index]);
    }
    #endif

    // Comparison operators

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator==(const Array<T, N>& a, const Array<T, N>& b) {
        return Equal(a.ConstBegin(), a.ConstEnd(), b.ConstBegin());
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator!=(const Array<T, N>& a, const Array<T, N>& b) {
        return !(a == b);
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator<(const Array<T, N>& a, const Array<T, N>& b) {
        return LexicographicalCompare(a.ConstBegin(), a.ConstEnd(), b.ConstBegin(), b.ConstEnd());
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator<=(const Array<T, N>& a, const Array<T, N>& b) {
        return !(b < a);
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator>(const Array<T, N>& a, const Array<T, N>& b) {
        return (b < a);
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator>=(const Array<T, N>& a, const Array<T, N>& b) {
        return !(a < b);
    }

    // To array

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T, size_t N, size_t... Indices>
        __WSTL_CONSTEXPR14__
        inline Array<RemoveCVType<T>, N> __ToArray(T (&array)[N], IndexSequence<Indices...>) {
            return {{array[Indices]...}};
        }

        template<typename T, size_t N, size_t... Indices>
        __WSTL_CONSTEXPR14__
        inline Array<RemoveCVType<T>, N> __ToArray(T (&&array)[N], IndexSequence<Indices...>) {
            return {{Move(array[Indices])...}};
        }
    }

    /// @brief Converts a built-in array to an `Array`
    /// @param array Built-in array to convert
    /// @return `Array` containing the elements of the built-in array
    /// @ingroup array
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/container/array/to_array
    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline Array<RemoveCVType<T>, N> ToArray(T (&array)[N]) {
        return __private::__ToArray(array, MakeIndexSequence<N>{});
    }

    /// @brief Converts a built-in array to an `Array` using move semantics
    /// @param array Built-in array to convert
    /// @return `Array` containing the elements of the built-in array
    /// @ingroup array
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/container/array/to_array
    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline Array<RemoveCVType<T>, N> ToArray(T (&&array)[N]) {
        return __private::__ToArray(Move(array), MakeIndexSequence<N>{});
    }
    #else
    /// @brief Converts a built-in array to an `Array`
    /// @param array Reference to the built-in array
    /// @return `wstl::Array` containing the elements of the built-in array
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array/to_array
    template<typename T, size_t N>
    inline Array<typename RemoveCV<T>::Type, N> ToArray(T (&array)[N]) {
        Array<typename RemoveCV<T>::Type, N> result;
        Copy(Begin(array), End(array), Begin(result));
        return result;
    }
    #endif
}

#endif