// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s span implementation
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been modified by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_SPAN_HPP__
#define __WSTL_SPAN_HPP__

#include "private/Platform.hpp"
#include "Limits.hpp"
#include "TypeTraits.hpp"
#include "Iterator.hpp"
#include "CircularIterator.hpp"
#include "Byte.hpp"
#include "StandardExceptions.hpp"
#include "Hash.hpp"
#include <stddef.h>


/// @defgroup span Span
/// @brief A non-owning view over a contiguous sequence of objects
/// @ingroup containers

namespace wstl {
    // Span tag

    /// @brief Tag class for span types
    /// @ingroup span
    class SpanTag {};

    // Dynamic extent

    /// @brief A constant that indicates that the span stores data dynamically
    /// @ingroup span
    /// @see https://en.cppreference.com/w/cpp/container/span/dynamic_extent.html
    static const __WSTL_CONSTEXPR__ size_t DynamicExtent = NumericLimits<size_t>::Max();

    // Span - fixed extent

    /// @brief A non-owning view over a contiguous sequence of objects with a fixed extent
    /// @tparam T Type of the elements in the span
    /// @tparam Extent Size of the span, or `DynamicExtent` for a dynamic size
    /// @ingroup span
    /// @see https://en.cppreference.com/w/cpp/container/span
    template<typename T, size_t Extent = DynamicExtent>
    class Span : public SpanTag {
    public:
        typedef T ElementType;
        typedef typename RemoveCV<T>::Type ValueType;
        typedef size_t SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;

        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        typedef wstl::CircularIterator<Iterator> CircularIterator;
        typedef wstl::CircularIterator<ReverseIterator> ReverseCircularIterator;

        static const __WSTL_CONSTEXPR__ size_t ExtentVariable = Extent;

        /// @brief Constructor with iterator and size
        /// @param first Iterator to the first element
        /// @param count Number of elements in the span
        template<typename Iterator>
        __WSTL_CONSTEXPR__ explicit Span(Iterator first, SizeType /* count */) : m_Begin(ToAddress(first)) {}

        /// @brief Constructor with two iterators
        /// @param first Iterator to the first element
        /// @param last Iterator to the last element
        template<typename Iterator>
        __WSTL_CONSTEXPR__ explicit Span(Iterator first, Iterator /* last */) : m_Begin(ToAddress(first)) {}

        #ifdef __WSTL_CXX11__
        /// @brief Constructor with a built-in array
        /// @param array Built-in array to create a span from
        template<size_t N, typename = EnableIfType<(N == Extent), void>>
        __WSTL_CONSTEXPR__ Span(ElementType(&array)[N]) __WSTL_NOEXCEPT__ : m_Begin(array) {}

        #else
        /// @brief Constructor with a built-in array
        /// @param array Built-in array to create a span from
        template<size_t N>
        Span(ElementType(&array)[N], typename EnableIf<(N == Extent), void>::Type* = 0) : m_Begin(array) {}

        #endif

        #ifdef __WSTL_CXX11__
        /// @brief Constructor with a container that has `.Data()` and `.Size()` methods
        /// @param container Container to create a span from
        template<typename Container, typename = EnableIfType<
            !IsBaseOf<SpanTag, RemoveReferenceType<Container>>::Value && 
            !IsArray<RemoveReferenceType<Container>>::Value &&
            !IsPointer<RemoveReferenceType<Container>>::Value &&
            IsSame<RemoveCVType<T>, RemoveCVType<typename RemoveReferenceType<Container>::ValueType>>::Value, 
            void
        >>
        __WSTL_CONSTEXPR__ Span(Container&& container) __WSTL_NOEXCEPT__ : m_Begin(container.Data()) {}   
        #else
        /// @brief Constructor with a container that has `.Data()` and `.Size()` methods
        /// @param container Container to create a span from
        template<typename Container>
        Span(Container& container, typename EnableIf<
            !IsBaseOf<SpanTag, RemoveReferenceType<Container>>::Value && 
            !IsArray<RemoveReferenceType<Container>>::Value &&
            !IsPointer<RemoveReferenceType<Container>>::Value &&
            IsSame<RemoveCVType<T>, RemoveCVType<typename RemoveReferenceType<Container>::ValueType>>::Value, 
            void>::Type* = 0) : m_Begin(container.Data()) {}
        
        /// @brief Constructor with a container that has `.Data()` and `.Size()` methods
        /// @param container Container to create a span from
        template<typename Container>
        Span(const Container& container, typename EnableIf<
            !IsBaseOf<SpanTag, typename RemoveReference<Container>::Type>::Value && 
            !IsArray<typename RemoveReference<Container>::Type>::Value &&
            !IsPointer<typename RemoveReference<Container>::Type>::Value &&
            IsSame<typename RemoveCV<T>::Type, typename RemoveCV<typename RemoveReference<Container>::Type::ValueType>::Type>::Value, 
            void>::Type* = 0) : m_Begin(container.Data()) {}
        #endif

        /// @brief Copy constructor
        /// @param other Another span to copy from
        __WSTL_CONSTEXPR__ Span(const Span& other) : m_Begin(other.m_Begin) {}
        
        /// @brief Templated copy constructor
        /// @param other Another span with fixed extent to copy from
        template<typename U>
        __WSTL_CONSTEXPR__ Span(const Span<U, Extent>& other) : m_Begin(other.Data()) {}
        
        /// @brief Templated copy constructor
        /// @param other Another span with dynamic extent to copy from
        /// @throws `LengthError` if the size of the other span does not match the extent
        template<typename U>
        __WSTL_CONSTEXPR14__ Span(const Span<U, DynamicExtent>& other) : m_Begin(other.Data()) {
            __WSTL_ASSERT__(other.Size() == Extent, WSTL_MAKE_EXCEPTION(LengthError, "Span size mismatch"));
        }

        /// @brief Copy assignment operator
        /// @param other Another span to assign from
        __WSTL_CONSTEXPR14__ Span& operator=(const Span& other) {
            m_Begin = other.m_Begin;
            return *this;
        }

        /// @brief Returns an iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Iterator Begin() const __WSTL_NOEXCEPT__ {
            return m_Begin;
        }

        /// @brief Returns a const iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return m_Begin;
        }

        /// @brief Returns an iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Iterator End() const __WSTL_NOEXCEPT__ {
            return m_Begin + Extent;
        }

        /// @brief Returns a const iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return m_Begin + Extent;
        }

        /// @brief Returns a reverse iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReverseIterator ReverseBegin() const __WSTL_NOEXCEPT__ {
            return ReverseIterator(m_Begin + Extent);
        }

        /// @brief Returns a const reverse iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Begin + Extent);
        }

        /// @brief Returns a reverse iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReverseIterator ReverseEnd() const __WSTL_NOEXCEPT__ {
            return ReverseIterator(m_Begin);
        }

        /// @brief Returns a const reverse iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReverseIterator ConstReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Begin);
        }

        /// @brief Returns a circular iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ CircularIterator BeginCircular() const __WSTL_NOEXCEPT__ {
            return CircularIterator(Begin(), End());
        }

        /// @brief Returns a reverse circular iterator to the begin of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReverseCircularIterator ReverseBeginCircular() const __WSTL_NOEXCEPT__ {
            return ReverseCircularIterator(ReverseBegin(), ReverseEnd());
        }

        /// @brief Returns a reference to the first element of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType Front() const __WSTL_NOEXCEPT__ {
            return *m_Begin;
        }

        /// @brief Returns a reference to the last element of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType Back() const __WSTL_NOEXCEPT__ {
            return *(m_Begin + Extent - 1);
        }

        /// @brief Returns a reference to the element at the specified position with bounds checking
        /// @param position The position of the element to access
        /// @return Reference to the element at the specified position
        /// @throws `OutOfRange` if the position is out of bounds
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position >= Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Span position out of range"));
            return m_Begin[position];
        }

        /// @brief Returns a const reference to the element at the specified position with bounds checking
        /// @param position The position of the element to access
        /// @return Const reference to the element at the specified position
        /// @throws `OutOfRange` if the position is out of bounds
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position >= Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Span position out of range"));
            return m_Begin[position];
        }

        /// @brief Access operator
        /// @param index Index of the element to access
        /// @return Reference to the element at the specified index
        __WSTL_CONSTEXPR__ ReferenceType operator[](SizeType index) const {
            return m_Begin[index];
        }

        /// @brief Returns a pointer to the underlying data of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType Data() const __WSTL_NOEXCEPT__ {
            return m_Begin;
        }

        /// @brief Returns the size of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__ {
            return Extent;
        }

        /// @brief Returns the size in bytes of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType SizeBytes() const __WSTL_NOEXCEPT__ {
            return Extent * sizeof(ElementType);
        }

        /// @brief Returns the maximum size of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__ {
            return Size();
        }

        /// @brief Checks if the span is empty
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__ {
            return Extent == 0;
        }

        /// @brief Gets a subspan of the first N elements
        /// @tparam Count Number of elements in the subspan
        /// @return A new span with fixed extent containing the first N elements
        template<size_t Count>
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, Count> First() const __WSTL_NOEXCEPT__ {
            StaticAssert(Count <= Extent, "Original span does not contain 'Count' elements");

            return Span<ElementType, Count>(m_Begin, m_Begin + Count);
        }

        /// @brief Gets a subspan of the first N elements
        /// @param count Number of elements in the subspan
        /// @return A new span with dynamic extent containing the first N elements
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, DynamicExtent> First(SizeType count) const __WSTL_NOEXCEPT__ {
            return Span<ElementType, DynamicExtent>(m_Begin, m_Begin + count);
        }

        /// @brief Gets a subspan of the last N elements
        /// @tparam Count Number of elements in the subspan
        /// @return A new span with fixed extent containing the last N elements
        template<size_t Count>
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, Count> Last() const __WSTL_NOEXCEPT__ {
            StaticAssert(Count <= Extent, "Original span does not contain Count elements");

            return Span<ElementType, Count>(m_Begin + Extent - Count, m_Begin + Extent);
        }

        /// @brief Gets a subspan of the last N elements
        /// @param count Number of elements in the subspan
        /// @return A new span containing the last N elements
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, DynamicExtent> Last(SizeType count) const __WSTL_NOEXCEPT__ {
            return Span<ElementType, DynamicExtent>(m_Begin + Extent - count, m_Begin + Extent);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Gets a subspan starting from the specified offset
        /// @tparam Offset Offset from the beginning of the span
        /// @tparam Count Number of elements in the subspan, or `DynamicExtent`
        /// @return A new span starting from the specified offset
        template<size_t Offset, size_t Count = DynamicExtent>
        __WSTL_NODISCARD__ constexpr Span<ElementType, (Count != DynamicExtent) ? Count : (Extent - Offset)> Subspan() const {
            StaticAssert(Offset <= Extent, "Offset is not within the original span");
            StaticAssert((Count != DynamicExtent) ? Count <= Extent - Offset : true, "Offset + Count is not within the original span");

            return (Count != DynamicExtent) ? Span<ElementType, Count != DynamicExtent ? Count : Extent - Offset>(m_Begin + Offset, m_Begin + Offset + Count) :
                Span<ElementType, Count != DynamicExtent ? Count : Extent - Offset>(m_Begin + Offset, m_Begin + Extent);
        }
        #else
        /// @brief Gets a subspan starting from the specified offset
        /// @tparam Offset Offset from the beginning of the span
        /// @tparam Count Number of elements in the subspan, or `DynamicExtent`
        /// @return A new span starting from the specified offset
        template<size_t Offset, size_t Count>
        Span<ElementType, (Count != DynamicExtent) ? Count : (Extent - Offset)> Subspan() const {
            StaticAssert(Offset <= Extent, "Offset is not within the original span");
            StaticAssert((Count != DynamicExtent) ? Count <= Extent - Offset : true, "Offset + Count is not within the original span");

            return (Count != DynamicExtent) ? Span<ElementType, (Count != DynamicExtent ? Count : Extent - Offset)>(m_Begin + Offset, m_Begin + Offset + Count) :
                Span<ElementType, (Count != DynamicExtent ? Count : Extent - Offset)>(m_Begin + Offset, m_Begin + Extent);
        }
        #endif

        /// @brief Gets a subspan starting from the specified offset
        /// @param offset Offset from the beginning of the span
        /// @param count Number of elements in the subspan, default is `DynamicExtent`
        /// @return A new span with dynamic extent starting from the specified offset
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, DynamicExtent> Subspan(SizeType offset, SizeType count = DynamicExtent) const {
            return (count != DynamicExtent) ? Span<ElementType, DynamicExtent>(m_Begin + offset, m_Begin + offset + count) :
                Span<ElementType, DynamicExtent>(m_Begin + offset, m_Begin + Extent);
        }

        /// @brief Advances the internal pointer of the span by a specified number of elements
        /// @param count Number of elements to advance
        void Advance(SizeType count) {
            m_Begin += Min(count, Size());
        }

        /// @brief Reinterprets the span as a span of a different type
        /// @tparam New Type to reinterpret the span as
        /// @return A new span with the specified type
        /// @throws `BadCast` if the alignment or size of the new type does not match the original type
        template<typename New>
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Span<New, DynamicExtent> ReinterpretAs() const {
            __WSTL_ASSERT__(IsAligned<AlignmentOf<New>::Value>(m_Begin), WSTL_MAKE_EXCEPTION(BadCast, "Span cast error: alignment mismatch"));
            __WSTL_ASSERT__(SizeBytes() % sizeof(New) == 0, WSTL_MAKE_EXCEPTION(BadCast, "Span cast error: size mismatch"));

            return Span<New, DynamicExtent>(reinterpret_cast<New*>(m_Begin), reinterpret_cast<New*>(m_Begin) + Extent * sizeof(ElementType) / sizeof(New));
        }
        
    private:
        PointerType m_Begin;
    };

    // Span - dynamic extent

    /// @brief A non-owning view over a contiguous sequence of objects with a dynamic extent
    /// @tparam T Type of the elements in the span
    /// @ingroup span
    /// @see https://en.cppreference.com/w/cpp/container/span
    template<typename T>
    class Span<T, DynamicExtent> : public SpanTag {
    public:
        typedef T ElementType;
        typedef typename RemoveCV<T>::Type ValueType;
        typedef size_t SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;

        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        typedef wstl::CircularIterator<Iterator> CircularIterator;
        typedef wstl::CircularIterator<ReverseIterator> ReverseCircularIterator;

        static const __WSTL_CONSTEXPR__ size_t ExtentVariable = DynamicExtent;

        /// @brief Default constructor
        __WSTL_CONSTEXPR__ Span() __WSTL_NOEXCEPT__ : m_Begin(__WSTL_NULLPTR__), m_End(__WSTL_NULLPTR__) {}

        /// @brief Constructor with iterator and size
        /// @param first Iterator to the first element in the range
        /// @param count Number of elements in the range
        template<typename Iterator>
        __WSTL_CONSTEXPR__ Span(Iterator first, SizeType count) __WSTL_NOEXCEPT__ : m_Begin(ToAddress(first)), 
            m_End(ToAddress(first) + count) {}

        /// @brief Constructor with two iterators
        /// @param first Iterator to the first element
        /// @param last Iterator to the element past the last element
        template<typename Iterator>
        __WSTL_CONSTEXPR__ Span(Iterator first, Iterator last) __WSTL_NOEXCEPT__ : m_Begin(ToAddress(first)),
            m_End(ToAddress(first) + Distance(first, last)) {}

        /// @brief Constructor with a built-in array
        /// @param array Built-in array to create a span from
        template<size_t N>
        __WSTL_CONSTEXPR__ Span(ElementType(&array)[N]) __WSTL_NOEXCEPT__ : m_Begin(array), m_End(array + N) {}

        #ifdef __WSTL_CXX11__
        /// @brief Constructor with a container that has `.Data()` and `.Size()` methods
        /// @param container Container to create a span from
        template<typename Container, typename = EnableIfType<
            !IsBaseOf<SpanTag, RemoveReferenceType<Container>>::Value && 
            !IsArray<RemoveReferenceType<Container>>::Value &&
            !IsPointer<RemoveReferenceType<Container>>::Value &&
            IsSame<RemoveCVType<T>, RemoveCVType<typename RemoveReferenceType<Container>::ValueType>>::Value, 
            void
        >>
        __WSTL_CONSTEXPR__ Span(Container&& container) __WSTL_NOEXCEPT__ : m_Begin(container.Data()), 
            m_End(container.Data() + container.Size()) {}   
        #else
        /// @brief Constructor with a container that has `.Data()` and `.Size()` methods
        /// @param container Container to create a span from
        template<typename Container>
        Span(Container& container, typename EnableIf<
            !IsBaseOf<SpanTag, RemoveReferenceType<Container>>::Value && 
            !IsArray<RemoveReferenceType<Container>>::Value &&
            !IsPointer<RemoveReferenceType<Container>>::Value &&
            IsSame<RemoveCVType<T>, RemoveCVType<typename RemoveReferenceType<Container>::ValueType>>::Value, 
            void>::Type* = 0) : m_Begin(container.Data()), m_End(container.Data() + container.Size()) {}
        
        /// @brief Constructor with a container that has `.Data()` and `.Size()` methods
        /// @param container Container to create a span from
        template<typename Container>
        Span(const Container& container, typename EnableIf<
            !IsBaseOf<SpanTag, typename RemoveReference<Container>::Type>::Value && 
            !IsArray<typename RemoveReference<Container>::Type>::Value &&
            !IsPointer<typename RemoveReference<Container>::Type>::Value &&
            IsSame<typename RemoveCV<T>::Type, typename RemoveCV<typename RemoveReference<Container>::Type::ValueType>::Type>::Value, 
            void>::Type* = 0) : m_Begin(container.Data()), m_End(container.Data() + container.Size()) {}
        #endif
        
        /// @brief Copy constructor
        /// @param other Another span to copy from
        __WSTL_CONSTEXPR__ Span(const Span& other) __WSTL_NOEXCEPT__ : m_Begin(other.m_Begin), m_End(other.m_End) {}
        
        /// @brief Templated copy constructor
        /// @param other Another span with fixed extent to copy from
        template<typename U, size_t N>
        __WSTL_CONSTEXPR__ Span(const Span<U, N>& other) __WSTL_NOEXCEPT__ : m_Begin(other.Data()),
            m_End(other.Data() + other.Size()) {}
        
        /// @brief Copy assignment operator
        /// @param other Another span to assign from
        __WSTL_CONSTEXPR14__ Span& operator=(const Span& other) {
            m_Begin = other.m_Begin;
            m_End = other.m_End;
            return *this;
        }

        /// @brief Returns an iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Iterator Begin() const __WSTL_NOEXCEPT__ {
            return m_Begin;
        }

        /// @brief Returns a const iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return m_Begin;
        }

        /// @brief Returns an iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Iterator End() const __WSTL_NOEXCEPT__ {
            return m_End;
        }

        /// @brief Returns a const iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return m_End;
        }

        /// @brief Returns a reverse iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReverseIterator ReverseBegin() const __WSTL_NOEXCEPT__ {
            return ReverseIterator(m_End);
        }

        /// @brief Returns a const reverse iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_End);
        }

        /// @brief Returns a reverse iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReverseIterator ReverseEnd() const __WSTL_NOEXCEPT__ {
            return ReverseIterator(m_Begin);
        }

        /// @brief Returns a const reverse iterator to the end of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReverseIterator ConstReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(m_Begin);
        }

        /// @brief Returns a circular iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ CircularIterator BeginCircular() const __WSTL_NOEXCEPT__ {
            return CircularIterator(Begin(), End());
        }

        /// @brief Returns a reverse circular iterator to the beginning of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReverseCircularIterator ReverseBeginCircular() const __WSTL_NOEXCEPT__ {
            return ReverseCircularIterator(ReverseBegin(), ReverseEnd());
        }

        /// @brief Returns a reference to the first element of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType Front() const __WSTL_NOEXCEPT__ {
            return *m_Begin;
        }

        /// @brief Returns a reference to the last element of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType Back() const __WSTL_NOEXCEPT__ {
            return *(m_End - 1);
        }

        /// @brief Returns a reference to the element at the specified position with bounds checking
        /// @param position The position of the element to access
        /// @return Reference to the element at the specified position
        /// @throws `OutOfRange` if the position is out of bounds
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType At(SizeType position) {
            __WSTL_ASSERT__(position >= Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Span position out of range"));
            return m_Begin[position];
        }

        /// @brief Returns a const reference to the element at the specified position with bounds checking
        /// @param position The position of the element to access
        /// @return Const reference to the element at the specified position
        /// @throws `OutOfRange` if the position is out of bounds
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType At(SizeType position) const {
            __WSTL_ASSERT__(position >= Size(), WSTL_MAKE_EXCEPTION(OutOfRange, "Span position out of range"));
            return m_Begin[position];
        }

        /// @brief Access operator
        /// @param index Index of the element to access
        /// @return Reference to the element at the specified index
        __WSTL_CONSTEXPR__ ReferenceType operator[](SizeType index) const {
            return m_Begin[index];
        }

        /// @brief Returns a pointer to the underlying data of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType Data() const __WSTL_NOEXCEPT__ {
            return m_Begin;
        }

        /// @brief Returns the size of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__ {
            return m_End - m_Begin;
        }

        /// @brief Returns the size in bytes of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType SizeBytes() const __WSTL_NOEXCEPT__ {
            return (m_End - m_Begin) * sizeof(ElementType);
        }

        /// @brief Returns the maximum size of the span
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__ {
            return Size();
        }

        /// @brief Checks if the span is empty
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__ {
            return m_Begin == m_End;
        }

        /// @brief Gets a subspan of the first N elements
        /// @tparam Count Number of elements in the subspan
        /// @return A new span containing the first N elements
        template<size_t Count>
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, Count> First() const __WSTL_NOEXCEPT__ {
            return Span<ElementType, Count>(m_Begin, m_Begin + Count);
        }

        /// @brief Gets a subspan of the first N elements
        /// @param count Number of elements in the subspan
        /// @return A new span with dynamic extent containing the first N elements
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, DynamicExtent> First(SizeType count) const __WSTL_NOEXCEPT__ {
            return Span<ElementType, DynamicExtent>(m_Begin, m_Begin + count);
        }

        /// @brief Gets a subspan of the last N elements
        /// @tparam Count Number of elements in the subspan
        /// @return A new span with fixed extent containing the last N elements
        template<size_t Count>
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, Count> Last() const __WSTL_NOEXCEPT__ {
            return Span<ElementType, Count>(m_End - Count, m_End);
        }

        /// @brief Gets a subspan of the last N elements
        /// @param count Number of elements in the subspan
        /// @return A new span with dynamic extent containing the last N elements
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, DynamicExtent> Last(SizeType count) const __WSTL_NOEXCEPT__ {
            return Span<ElementType, DynamicExtent>(m_End - count, m_End);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Gets a subspan starting from the specified offset
        /// @tparam Offset Offset from the beginning of the span
        /// @tparam Count Number of elements in the subspan, or `DynamicExtent`
        /// @return A new span starting from the specified offset
        template<size_t Offset, size_t Count = DynamicExtent>
        __WSTL_NODISCARD__ constexpr Span<ElementType, (Count != DynamicExtent) ? Count : DynamicExtent> Subspan() const {
            return (Count != DynamicExtent) ? Span<ElementType, Count != DynamicExtent ? Count : DynamicExtent>(m_Begin + Offset, m_Begin + Offset + Count) :
                Span<ElementType, (Count != DynamicExtent) ? Count : DynamicExtent>(m_Begin + Offset, m_End);
        }
        #else
        /// @brief Gets a subspan starting from the specified offset
        /// @tparam Offset Offset from the beginning of the span
        /// @tparam Count Number of elements in the subspan
        /// @return A new span starting from the specified offset
        template<size_t Offset, size_t Count>
        Span<ElementType, (Count != DynamicExtent) ? Count : DynamicExtent> Subspan() const {
            return (Count != DynamicExtent) ? Span<ElementType, (Count != DynamicExtent) ? Count : DynamicExtent>(m_Begin + Offset, m_Begin + Offset + Count) :
                Span<ElementType, (Count != DynamicExtent) ? Count : DynamicExtent>(m_Begin + Offset, m_End);
        }
        #endif

        /// @brief Gets a subspan starting from the specified offset
        /// @param offset Offset from the beginning of the span
        /// @param count Number of elements in the subspan, default is `DynamicExtent`
        /// @return A new span with dynamic extent starting from the specified offset
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ Span<ElementType, DynamicExtent> Subspan(SizeType offset, SizeType count = DynamicExtent) const {
            return (count != DynamicExtent) ? Span<ElementType, DynamicExtent>(m_Begin + offset, m_Begin + offset + count) :
                Span<ElementType, DynamicExtent>(m_Begin + offset, m_End);
        }

        /// @brief Advances the internal pointer of the span by a specified number of elements
        /// @param count Number of elements to advance
        void Advance(SizeType count) {
            m_Begin += Min(count, Size());
        }

        /// @brief Reinterprets the span as a span of a different type
        /// @tparam New Type to reinterpret the span as
        /// @return A new span with the specified type
        /// @throws `BadCast` if the alignment or size of the new type does not match the original type
        template<typename New>
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Span<New, DynamicExtent> ReinterpretAs() const {
            __WSTL_ASSERT__(IsAligned<AlignmentOf<New>::Value>(m_Begin), WSTL_MAKE_EXCEPTION(BadCast, "Span cast error: alignment mismatch"));
            __WSTL_ASSERT__(SizeBytes() % sizeof(New) == 0, WSTL_MAKE_EXCEPTION(BadCast, "Span cast error: size mismatch"));

            return Span<New, DynamicExtent>(reinterpret_cast<New*>(m_Begin), (m_End - m_Begin) * sizeof(ElementType) / sizeof(New));
        }

    private:
        PointerType m_Begin;
        PointerType m_End;
    };

    template<typename T, size_t Extent>
    const __WSTL_CONSTEXPR__ size_t Span<T, Extent>::ExtentVariable;

    template<typename T>
    const __WSTL_CONSTEXPR__ size_t Span<T, DynamicExtent>::ExtentVariable;

    // Template deduction guides

    #ifdef __WSTL_CXX17__
    template<typename T, size_t N>
    Span(T (&)[N]) -> Span<T, N>;

    template<typename Iterator>
    Span(Iterator, Iterator) -> Span<typename IteratorTraits<Iterator>::ValueType, DynamicExtent>;

    template<typename Iterator>
    Span(Iterator, size_t) -> Span<typename IteratorTraits<Iterator>::ValueType, DynamicExtent>;

    template<typename T, size_t N>
    Span(Array<T, N>&) -> Span<T, N>;

    template<typename T, size_t N>
    Span(const Array<T, N>&) -> Span<T, N>;

    template<typename T, size_t Extent>
    Span(const Span<T, Extent>&) -> Span<T, Extent>;
    #endif

    // Hash specialization

    template<typename T, size_t Extent>
    struct Hash<Span<T, Extent> > {
        __WSTL_CONSTEXPR14__ size_t operator()(const Span<T>& view) {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(view.Data()), reinterpret_cast<const uint8_t*>(view.Data()) + view.Size());
        }
    };

    // Comparison operators

    template<typename T1, size_t N1, typename T2, size_t N2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ inline
    typename EnableIf<IsSame<typename RemoveCV<T1>::Type, typename RemoveCV<T2>::Type>::Value, bool>::Type 
    operator==(const Span<T1, N1>& a, const Span<T2, N2>& b) {
        return (a.Begin() == b.Begin()) && (a.Size() == b.Size()) && Equal(a.Begin(), a.End(), b.Begin(), b.End());
    }

    template<typename T1, size_t N1, typename T2, size_t N2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ inline
    bool operator!=(const Span<T1, N1>& a, const Span<T2, N2>& b) {
        return !(a == b);
    }

    // As bytes

    /// @brief Converts a span of type T to a span of bytes
    /// @param span The span to convert
    /// @return A span of bytes representing the data in the original span
    /// @ingroup span
    /// @see https://en.cppreference.com/w/cpp/container/span/as_bytes
    template<typename T, size_t N>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    Span<const Byte, (N == DynamicExtent) ? DynamicExtent : (N * sizeof(T))> AsBytes(Span<T, N> span) __WSTL_NOEXCEPT__ {
        return Span<const Byte, (N == DynamicExtent) ? DynamicExtent : (N * sizeof(T))>(reinterpret_cast<const Byte*>(span.Data()), span.SizeBytes());
    }

    // As writable bytes

    /// @brief Converts a span of type T to a span of writable bytes
    /// @param span The span to convert
    /// @return A span of writable bytes representing the data in the original span
    /// @ingroup span
    /// @see https://en.cppreference.com/w/cpp/container/span/as_bytes
    template<typename T, size_t N>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    Span<Byte, (N == DynamicExtent) ? DynamicExtent : (N * sizeof(T))> AsWritableBytes(Span<T, N> span) __WSTL_NOEXCEPT__ {
        StaticAssert(!IsConst<T>::Value, "Span<T> must be of non-const type");
        return Span<Byte, (N == DynamicExtent) ? DynamicExtent : (N * sizeof(T))>(reinterpret_cast<Byte*>(span.Data()), span.SizeBytes());
    }
}

#endif