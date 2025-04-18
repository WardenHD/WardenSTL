#ifndef __WSTL_ITERATOR_HPP__
#define __WSTL_ITERATOR_HPP__

#include <stddef.h>

#include "private/Swap.hpp"
#include "TypeTraits.hpp"
#include "Memory.hpp"


/// @defgroup iterator Iterator
/// @ingroup utilities
/// @brief Provides functionality to work with custom iterator

namespace wstl {
    // Iterator tags

    /// @brief A tag class used to identify input iterators.
    /// Input iterators are iterators that can only be used for single-pass 
    /// sequential input operations, such as reading values from a sequence.
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_tags
    class InputIteratorTag {};

    /// @brief A tag class used to identify output iterators.
    /// Output iterators are iterators that can only be used for single-pass 
    /// sequential output operations, such as writing values to a sequence.
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_tags
    class OutputIteratorTag {};

    /// @brief A tag class used to identify forward iterators.
    /// Forward iterators support single-pass sequential traversal and allow 
    /// reading or writing elements. Unlike input/output iterators, they can 
    /// be incremented multiple times without losing access to previously visited elements.
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_tags
    class ForwardIteratorTag : public InputIteratorTag {};

    /// @brief A tag class used to identify bidirectional iterators.
    /// Bidirectional iterators extend forward iterators by allowing both 
    /// forward and backward traversal of a sequence.
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_tags
    class BidirectionalIteratorTag : public ForwardIteratorTag {};

    /// @brief A tag class used to identify random-access iterators.
    /// Random-access iterators extend bidirectional iterators by supporting 
    /// constant-time access to any element in the sequence, using arithmetic operators.
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_tags
    class RandomAccessIteratorTag : public BidirectionalIteratorTag {};

    /// @brief A tag class used to identify contiguous iterators.
    /// Contiguous iterators extend random-access iterators by guaranteeing 
    /// that elements are stored in contiguous memory locations, like arrays.
    // class ContiguousIteratorTag : public RandomAccessIteratorTag {};


    // Iterator

    /// @brief Base class for any simple iterator
    /// @tparam Category Type of the iterator (tag)
    /// @tparam T Type of the element
    /// @tparam Distance Type of the distance
    /// @tparam Pointer Type of the pointer
    /// @tparam Reference Type of the reference
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator
    template<typename Category, typename T, typename Distance = ptrdiff_t, 
        typename Pointer = T*, typename Reference = T&>
    class Iterator {
    public:
        typedef Category IteratorCategory;
        typedef T ValueType;
        typedef Distance DifferenceType;
        typedef Pointer PointerType;
        typedef Reference ReferenceType;
    };

    // Iterator traits

    /// @brief Provides uniform interface for accessing properties of an iterator
    /// @tparam Iterator Type of the iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_traits
    template<typename Iterator>
    class IteratorTraits {
    public:
        /// @brief The category of the iterator (e.g. Input, Forward, Bidirectional)
        typedef typename Iterator::IteratorCategory IteratorCategory;
        /// @brief The type of the element pointed by the iterator
        typedef typename Iterator::ValueType ValueType;
        /// @brief The type of the distance between two iterators
        typedef typename Iterator::DifferenceType DifferenceType;
        /// @brief The type of the pointer to the elements
        typedef typename Iterator::PointerType PointerType;
        /// @brief The type of the reference to the elements
        typedef typename Iterator::ReferenceType ReferenceType;
    };
    
    /// @brief Specialization of `IteratorTraits` for pointer types
    /// @tparam T Type of the element pointed to by the pointer
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_traits
    template<typename T>
    class IteratorTraits<T*> {
    public:
        /// @brief The category of the iterator for raw pointers is `RandomAccessIteratorTag`
        typedef RandomAccessIteratorTag IteratorCategory;
        /// @brief The type of the element pointed by the pointer
        typedef T ValueType;
        /// @brief The type of the distance between two pointers
        typedef ptrdiff_t DifferenceType;
        /// @brief The type of the pointer to the elements (same as the raw pointer itself)
        typedef T* PointerType;
        /// @brief The type of the reference to the elements
        typedef T& ReferenceType;
    };

    /// @brief Specialization of `IteratorTraits` for const pointer types
    /// @tparam T Type of the element pointed to by the const pointer
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/iterator_traits
    template<typename T>
    class IteratorTraits<const T*> {
    public:
        /// @brief The category of the iterator for const pointers is `RandomAccessIteratorTag`
        typedef RandomAccessIteratorTag IteratorCategory;
        /// @brief The type of the element pointed by the const pointer
        typedef T ValueType;
        /// @brief The type of the distance between two const pointers
        typedef ptrdiff_t DifferenceType;
        /// @brief The type of the pointer to the elements (same as the raw pointer itself)
        typedef const T* PointerType;
        /// @brief The type of the reference to the elements
        typedef const T& ReferenceType;
    };

    // Advance

    namespace __private {
        template<typename Iterator, typename Distance>
        __WSTL_CONSTEXPR14__ 
        inline void __Advance(Iterator& iterator, Distance count, OutputIteratorTag) {
            while(--count) iterator++;
        }

        template<typename Iterator, typename Distance>
        __WSTL_CONSTEXPR14__ 
        inline void __Advance(Iterator& iterator, Distance count, ForwardIteratorTag) {
            while(--count) iterator++;
        }

        template<typename Iterator, typename Distance>
        __WSTL_CONSTEXPR14__ 
        inline void __Advance(Iterator& iterator, Distance count, BidirectionalIteratorTag) {
            if(count > 0) while(--count) iterator++;
            else while(++count) iterator--;
        }

        template<typename Iterator, typename Distance>
        __WSTL_CONSTEXPR14__ 
        inline void __Advance(Iterator& iterator, Distance count, RandomAccessIteratorTag) {
            iterator += count;
        }
    }

    
    template<typename Iterator, typename Distance> 
    /// @brief Advances the given iterator by the specified number of steps
    /// @param iterator Reference to the iterator to be advanced
    /// @param count The number of steps to advance the iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/advance
    __WSTL_CONSTEXPR14__ inline void Advance(Iterator& iterator, Distance count) {
        __private::__Advance(iterator, count, typename IteratorTraits<Iterator>::IteratorCategory());
    }

    // Distance

    namespace __private {
        template<typename Iterator>
        __WSTL_CONSTEXPR14__ typename IteratorTraits<Iterator>::DifferenceType 
        __Distance(Iterator first, Iterator last, InputIteratorTag) {
            typename IteratorTraits<Iterator>::DifferenceType result = 0;
            for(; first != last; result++, first++);
            return result;
        }   

        template<typename Iterator>
        __WSTL_CONSTEXPR14__ typename IteratorTraits<Iterator>::DifferenceType 
        __Distance(Iterator first, Iterator last, RandomAccessIteratorTag) {
            return last - first;
        }
    }

    /// @brief Calculates distance between two iterators
    /// @param first The starting iterator
    /// @param last The ending iterator
    /// @return The distance between two iterators
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/distance
    template<typename Iterator>
    __WSTL_CONSTEXPR14__ 
    inline typename IteratorTraits<Iterator>::DifferenceType Distance(Iterator first, Iterator last) {
        return __private::__Distance(first, last, typename IteratorTraits<Iterator>::IteratorCategory());
    }

    // Next

    /// @brief Returns an iterator `count` steps ahead of the given iterator
    /// @param iterator The starting iterator
    /// @param count The number of steps to advance the iterator (default is 1)
    /// @return Advanced copy of the iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/next
    template<typename Iterator>
    __WSTL_CONSTEXPR14__ 
    inline Iterator Next(Iterator iterator, typename IteratorTraits<Iterator>::DifferenceType count = 1) {
        Advance(iterator, count);
        return iterator;
    }

    // Previous

    /// @brief Returns an iterator `count` steps before the given iterator
    /// @param iterator The starting iterator
    /// @param count The number of steps to move the iterator backwards (default is 1)
    /// @return Moved back copy of the iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/prev
    template<typename Iterator>
    __WSTL_CONSTEXPR14__ 
    inline Iterator Previous(Iterator iterator, typename IteratorTraits<Iterator>::DifferenceType count = 1) {
        Advance(iterator, -count);
        return iterator;
    }

    // Is forward iterator

    /// @brief Checks if a given type is a forward iterator or its derival
    /// @tparam T Type to check
    template<typename T>
    class IsForwardIterator : public IsBaseOf<ForwardIteratorTag, typename IteratorTraits<T>::IteratorCategory> {};

    // Is input iterator

    /// @brief Checks if a given type is an input iterator or its derival
    /// @tparam T Type to check
    template<typename T>
    class IsInputIterator : public IsBaseOf<InputIteratorTag, typename IteratorTraits<T>::IteratorCategory> {};

    // Is output iterator

    /// @brief Checks if a given type is an output iterator or its derival
    /// @tparam T Type to check
    template<typename T>
    class IsOutputIterator : public IsBaseOf<OutputIteratorTag, typename IteratorTraits<T>::IteratorCategory> {};

    // Is bidirectional iterator

    /// @brief Checks if a given type is an bidirectional iterator or its derival
    /// @tparam T Type to check
    template<typename T>
    class IsBidirectionalIterator : public IsBaseOf<BidirectionalIteratorTag, typename IteratorTraits<T>::IteratorCategory> {};

    // Is random access iterator

    /// @brief Checks if a given type is a random access iterator or its derival
    /// @tparam T Type to check
    template<typename T>
    class IsRandomAccessIterator : public IsBaseOf<RandomAccessIteratorTag, typename IteratorTraits<T>::IteratorCategory> {};

    // Is contiguous iterator

    // /// @brief Checks if a given type is a contiguous iterator or its derival
    // /// @tparam T Type to check
    // template<typename T>
    // class IsContiguousIterator : public IsBaseOf<ContiguousIteratorTag, typename IteratorTraits<T>::IteratorCategory> {};

    // Reverse iterator

    /// @brief A reverse iterator adapter that allows reverse iteration over a range of elements.
    /// The `ReverseIterator` adapts a given iterator type to iterate in the reverse direction. 
    /// It works by decrementing the base iterator and providing access to the previous element.
    /// @tparam T The type of the underlying iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/reverse_iterator
    template<typename T>
    class ReverseIterator : public Iterator<typename IteratorTraits<T>::IteratorCategory, 
    typename IteratorTraits<T>::ValueType, typename IteratorTraits<T>::DifferenceType, 
    typename IteratorTraits<T>::PointerType, typename IteratorTraits<T>::ReferenceType> {
    public:
        typedef T PointerType;
        typedef typename IteratorTraits<T>::DifferenceType DifferenceType;
        typedef typename IteratorTraits<T>::ReferenceType ReferenceType;

        /// @brief Type of the underlying iterator
        typedef T IteratorType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ ReverseIterator() : m_Current() {}

        /// @brief Parameterized constructor
        /// @param iterator Base iterator to adapt
        __WSTL_CONSTEXPR14__ explicit ReverseIterator(IteratorType iterator) : m_Current(iterator) {}

        /// @brief Templated copy constructor - copies from reverse iterator of potentially different type
        /// @param other Reverse iterator to copy from
        template<typename U>
        __WSTL_CONSTEXPR14__ ReverseIterator(const ReverseIterator<U>& other) : m_Current(other.m_Current) {}

        /// @brief Templated copy assignemt operator - assigns a reverse iterator 
        /// with potentially different type
        /// @param other Reverse iterator to assign from
        template<typename U>
        __WSTL_CONSTEXPR14__ ReverseIterator& operator=(const ReverseIterator<U>& other) {
            m_Current = other.Base();
            return *this;
        }

        /// @brief Returns the underlying iterator
        __WSTL_CONSTEXPR14__ IteratorType Base() const {
            return m_Current;
        }
        
        /// @brief Dereference operator
        /// @return Reference to the element that precedes the base iterator
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator*() const {
            IteratorType temp = m_Current;
            return *--temp;
        }
        
        /// @brief Arrow operator - provides access to element's member functions or properties
        /// @return Pointer to the element
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ PointerType operator->() const {
            return AddressOf(this->operator*());
        }

        /// @brief Pre-increment operator - moves the iterator backwards by one element
        /// @return Reference to the updated iterator
        __WSTL_CONSTEXPR14__ ReverseIterator& operator++() {
            --m_Current;
            return *this;
        }

        /// @brief Post-increment operator - moves the iterator backwards by one element
        /// @return Reference to the iterator before incrementing
        __WSTL_CONSTEXPR14__ ReverseIterator operator++(int) {
            ReverseIterator temp = *this;
            --m_Current;
            return temp;
        }

        /// @brief Pre-decrement operator - moves the iterator forward by one element
        /// @return Reference to the updated iterator
        __WSTL_CONSTEXPR14__ ReverseIterator& operator--() {
            ++m_Current;
            return *this;
        }

        /// @brief Post-decrement operator - moves the iterator forward by one element
        /// @return Reference to the iterator before decrementing
        __WSTL_CONSTEXPR14__ ReverseIterator operator--(int) {
            ReverseIterator temp = *this;
            ++m_Current;
            return temp;
        }

        /// @brief Adds a given offset to the iterator and returns a new reverse iterator
        /// @param n The offset to add (negative for backward movement)
        /// @return A new reverse iterator advanced by `n` positions
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReverseIterator operator+(const DifferenceType n) const {
            return ReverseIterator(m_Current - n);
        }

        /// @brief Advances the iterator by a given offset
        /// @param n The offset to add (negative for backward movement)
        /// @return A reference to the updated iterator
        __WSTL_CONSTEXPR14__ ReverseIterator& operator+=(const DifferenceType n) {
            m_Current -= n;
            return *this;
        }

        /// @brief Subtracts a given offset from the iterator and returns a new reverse iterator
        /// @param n The offset to subtract (negative for forward movement)
        /// @return A new reverse iterator moved back by `n` positions
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReverseIterator operator-(const DifferenceType n) const {
            return ReverseIterator(m_Current + n);
        }

        /// @brief Moves the iterator back by a given offset
        /// @param n The offset to subtract (negative for forward movement)
        /// @return A reference to the updated iterator
        __WSTL_CONSTEXPR14__ ReverseIterator& operator-=(const DifferenceType n) {
            m_Current += n;
            return *this;
        }

        /// @brief Provides access to the element at a given offset relative to the iterator
        /// @param n The offset from the current position
        /// @return A reference to the element at the specified offset
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator[](const DifferenceType n) const {
            return *(*this + n);
        }

    protected:
        IteratorType m_Current;
    };

    // Comparison operators for ReverseIterator

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator==(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return x.Base() == y.Base();
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator!=(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return !(x == y);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator<(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return y.Base() < x.Base();
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator<=(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return !(y < x);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator>(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return y < x;
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator>=(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return !(x < y);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ typename ReverseIterator<T>::DifferenceType operator-(const ReverseIterator<T>& x, const ReverseIterator<T>& y) {
        return y.Base() - x.Base();
    }

    template<typename T, class Difference>
    __WSTL_CONSTEXPR14__ ReverseIterator<T> operator+(Difference n, const ReverseIterator<T>& iterator) {
        return iterator.operator+(n);
    }

    /// @brief Makes a reverse iterator from the given iterator
    /// @param iterator The base iterator to adapt
    /// @return A `ReverseIterator` object that starts at the position represented by the given iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/make_reverse_iterator
    template<typename T>
    __WSTL_CONSTEXPR14__ ReverseIterator<T> MakeReverseIterator(T iterator) {
        return ReverseIterator<T>(iterator);
    }

    #ifdef __WSTL_CXX11__
    // Move iterator

    /// @brief An iterator adaptor that converts a given iterator into a move iterator.
    /// The `MoveIterator` allows elements to be moved instead of copied when iterating.
    /// This is useful for optimizing performance when transferring ownership of resources.
    /// @tparam T The type of the underlying iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/move_iterator
    template<typename T>
    class MoveIterator : public Iterator<typename IteratorTraits<T>::IteratorCategory,
    typename IteratorTraits<T>::ValueType, typename IteratorTraits<T>::DifferenceType,
    T, typename IteratorTraits<T>::ValueType&&> {
    public:
        /// @brief The type of the underlying pointer
        typedef T PointerType;
        
        /// @brief The type used for computing iterator differences
        typedef typename IteratorTraits<T>::DifferenceType DifferenceType;
        
        /// @brief The reference type, which is an rvalue reference to allow moves
        typedef typename IteratorTraits<T>::ValueType&& ReferenceType;
        
        /// @brief The type of the underlying iterator
        typedef T IteratorType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ MoveIterator() : m_Current() {}

        /// @brief Parameterized constructor
        /// @param iterator Base iterator to adapt
        __WSTL_CONSTEXPR14__ explicit MoveIterator(IteratorType iterator) : m_Current(iterator) {}

        /// @brief Templated copy constructor - copies from move iterator of potentially different type
        /// @param other Move iterator to copy from
        template<typename U>
        __WSTL_CONSTEXPR14__ MoveIterator(const MoveIterator<U>& other) : m_Current(other.m_Current) {}

        /// @brief Templated copy assignemt operator - assigns a reverse iterator 
        /// with potentially different type
        /// @param other Reverse iterator to assign from
        template<typename U>
        __WSTL_CONSTEXPR14__ MoveIterator& operator=(const MoveIterator<U>& other) {
            m_Current = other.m_Current;
            return *this;
        }

        /// @brief Returns the underlying iterator
        __WSTL_CONSTEXPR14__ IteratorType Base() const {
            return m_Current;
        }
        
        /// @brief Arrow operator - provides access to element's member functions or properties
        /// @return Pointer to the element
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ PointerType operator->() const {
            return m_Current;
        }

        /// @brief Dereference operator
        /// @return Reference to the element that precedes the base iterator
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator*() const {
            return Move(*m_Current);
        }

        /// @brief Pre-increment operator - moves the iterator forward by one element
        /// @return Reference to the updated iterator
        __WSTL_CONSTEXPR14__ MoveIterator& operator++() {
            m_Current++;
            return *this;
        }

        /// @brief Post-increment operator - moves the iterator forward by one element
        /// @return Reference to the iterator before incrementing
        __WSTL_CONSTEXPR14__ MoveIterator operator++(int) {
            MoveIterator temp = *this;
            m_Current++;
            return temp;
        }

        /// @brief Pre-decrement operator - moves the iterator backwards by one element
        /// @return Reference to the updated iterator
        __WSTL_CONSTEXPR14__ MoveIterator& operator--() {
            m_Current--;
            return *this;
        }

        /// @brief Post-decrement operator - moves the iterator backwards by one element
        /// @return Reference to the iterator before decrementing
        __WSTL_CONSTEXPR14__ MoveIterator operator--(int) {
            MoveIterator temp = *this;
            m_Current--;
            return temp;
        }

        /// @brief Adds a given offset to the iterator and returns a new move iterator
        /// @param n The offset to add (negative for backward movement)
        /// @return A new move iterator advanced by `n` positions
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ MoveIterator operator+(DifferenceType n) const {
            return MoveIterator(m_Current + n);
        }

        /// @brief Advances the iterator by a given offset
        /// @param n The offset to add (negative for backward movement)
        /// @return A reference to the updated iterator
        __WSTL_CONSTEXPR14__ MoveIterator operator+=(DifferenceType n) {
            m_Current += n;
            return *this;
        }
        
        /// @brief Subtracts a given offset from the iterator and returns a new reverse iterator
        /// @param n The offset to subtract (negative for forward movement)
        /// @return A new reverse iterator moved back by `n` positions
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ MoveIterator operator-(DifferenceType n) const {
            return MoveIterator(m_Current - n);
        }

        /// @brief Moves the iterator back by a given offset
        /// @param n The offset to subtract (negative for forward movement)
        /// @return A reference to the updated iterator
        __WSTL_CONSTEXPR14__ MoveIterator operator-=(DifferenceType n) {
            m_Current -= n;
            return *this;
        }

        /// @brief Provides access to the element at a given offset relative to the iterator
        /// @param n The offset from the current position
        /// @return A reference to the element at the specified offset
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator[](DifferenceType n) const {
            return wstl::Move(m_Current[n]);
        }
    
    private:
        IteratorType m_Current;
    };

    // Comparison operators for MoveIterator

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator==(const MoveIterator<T>& x, const MoveIterator<T>& y) {
        return x.Base() == y.Base();
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator!=(const MoveIterator<T>& x, const MoveIterator<T>& y) {
        return !(x == y);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator<(const MoveIterator<T>& x, const MoveIterator<T>& y) {
        return x.Base() < y.Base();
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator<=(const MoveIterator<T>& x, const MoveIterator<T>& y) {
        return !(y < x);
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator>(const MoveIterator<T>& x, const MoveIterator<T>& y) {
        return y < x;
    }

    template<typename T>
    __WSTL_CONSTEXPR14__ bool operator>=(const MoveIterator<T>& x, const MoveIterator<T>& y) {
        return !(x < y);
    }

    template<typename T1, typename T2>
    __WSTL_CONSTEXPR14__ auto operator-(const MoveIterator<T1>& x, const MoveIterator<T2>& y) -> decltype(x.Base() - y.Base()) {
        return x.Base() - y.Base();
    }

    template<typename T, class Difference>
    __WSTL_CONSTEXPR14__ MoveIterator<T> operator+(Difference n, const MoveIterator<T>& iterator) {
        return iterator + n;
    }

    /// @brief Makes a move iterator from the given iterator
    /// @param iterator The base iterator to adapt
    /// @return A `MoveIterator` object that starts at the position represented by the given iterator
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/make_move_iterator
    template<typename T>
    __WSTL_CONSTEXPR14__ MoveIterator<T> MakeMoveIterator(T iterator) {
        return MoveIterator<T>(iterator);
    }
    #endif

    // Insert iterator

    /// @brief An output iterator that inserts elements into a container at a specified position.
    /// This iterator allows inserting new elements at a given position while iterating.
    /// It is primarily used with algorithms that output data into a container.
    /// @tparam Container The type of the container to insert elements into
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/insert_iterator
    template<typename Container>
    class InsertIterator : public Iterator<OutputIteratorTag, void, void, void, void> {
    public:
        /// @brief Type of the container being modified
        typedef Container ContainerType;

        /// @brief Constructor
        /// @param container The container to insert elements into
        /// @param iterator The position at which elements should be inserted
        __WSTL_CONSTEXPR14__ explicit InsertIterator(Container& container, typename Container::Iterator iterator) : 
            m_Container(AddressOf(container)), m_Iterator(iterator) {}

        /// @brief Inserts an element into the container at the current iterator position.
        /// The position is updated after insertion
        /// @param value The value to insert
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ InsertIterator& operator=(const typename Container::ValueType& value) {
            m_Iterator = m_Container->Insert(m_Iterator, value);
            m_Iterator++;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts an element into the container at the current iterator position using move semantics.
        /// The position is updated after insertion.
        /// @param value The value to insert (rvalue reference)
        /// @return A reference to the current iterator
        /// @since C++11
        __WSTL_CONSTEXPR14__ InsertIterator operator=(const typename Container::ValueType&& value) {
            m_Iterator = m_Container->Insert(m_Iterator, Move(value));
            m_Iterator++;
            return *this;
        }
        #endif
        
        /// @brief Dereference operator. Since this is an output iterator, it returns itself.
        /// @return A reference to the current iterator
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ InsertIterator& operator*() {
            return *this;
        }

        /// @brief Pre-increment operator. No effect, as insertion modifies the position automatically.
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ InsertIterator& operator++() {
            return *this;
        }

        /// @brief Post-increment operator. No effect, as insertion modifies the position automatically.
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ InsertIterator& operator++(int) {
            return *this;
        }

    protected:
        Container* m_Container;
        typename Container::Iterator m_Iterator;
    };

    /// @brief Creates an insert iterator for inserting elements into a container
    /// @param container The container to insert into
    /// @param iterator The position at which elements should be inserted
    /// @return An `InsertIterator` associated with the given container and position
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/inserter
    template<typename Container>
    InsertIterator<Container> Inserter(Container& container, typename Container::Iterator iterator) {
        return InsertIterator<Container>(container, iterator);
    }

    // Front insert iterator


    /// @brief An output iterator that inserts elements at the front of a container
    /// This iterator is primarily used with containers that support `PushFront`
    /// @tparam Container The type of the container
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/front_insert_iterator
    template<typename Container>
    class FrontInsertIterator : public Iterator<OutputIteratorTag, void, void, void, void> {
    public:
        /// @brief The type of the container being modified
        typedef Container ContainerType;

        /// @brief Constructor
        /// @param container The container where elements will be inserted at the front
        __WSTL_CONSTEXPR14__ explicit FrontInsertIterator(Container& container) 
            : m_Container(AddressOf(container)) {}

        /// @brief Inserts an element at the front of the container
        /// @param value The value to insert
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ FrontInsertIterator<Container>& operator=(const typename Container::ValueType& value) {
            m_Container->PushFront(value);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts an element at the front of the container using move semantics
        /// @param value The value to insert (rvalue reference)
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ FrontInsertIterator<Container>& operator=(typename Container::ValueType&& value) {
            m_Container->PushFront(Move(value));
            return *this; 
        }
        #endif

        /// @brief Dereference operator. Since this is an output iterator, it returns itself.
        /// @return A reference to the current iterator
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ FrontInsertIterator& operator*() {
            return *this;
        }

        /// @brief Pre-increment operator. No effect, as insertion modifies the container automatically.
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ FrontInsertIterator& operator++() {
            return *this;
        }

        /// @brief Post-increment operator. No effect, as insertion modifies the container automatically.
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ FrontInsertIterator& operator++(int) {
            return *this;
        }

    protected:
        Container* m_Container;
    };

    /// @brief Makes a front insert iterator for inserting elements at the front of a container
    /// @param container The container to insert into
    /// @return A `FrontInsertIterator` associated with the given container
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/front_inserter
    template<typename Container>
    __WSTL_CONSTEXPR14__ FrontInsertIterator<Container> FrontInserter(Container& container) {
        return FrontInsertIterator<Container>(container);
    }

    /// Back insert iterator

    /// @brief An output iterator that inserts elements at the back of a container
    /// This iterator is primarily used with containers that support `PushBack`
    /// @tparam Container The type of the container
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/back_insert_iterator
    template<typename Container>
    class BackInsertIterator : public Iterator<OutputIteratorTag, void, void, void, void> {
    public:
        /// @brief The type of the container being modified
        typedef Container ContainerType;

        /// @brief Constructor
        /// @param container The container where elements will be inserted at the back
        explicit BackInsertIterator(Container& container) : m_Container(AddressOf(container)) {}

        /// @brief Inserts an element at the back of the container
        /// @param value The value to insert
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ BackInsertIterator& operator=(const typename Container::ValueType& value) {
            m_Container->PushBack(value);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Inserts an element at the back of the container using move semantics
        /// @param value The value to insert (rvalue reference)
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ BackInsertIterator& operator=(typename Container::ValueType&& value) {
            m_Container->PushBack(Move(value));
            return *this; 
        }
        #endif

        /// @brief Dereference operator. Since this is an output iterator, it returns itself.
        /// @return A reference to the current iterator
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ BackInsertIterator& operator*() {
            return *this;
        }

        /// @brief Pre-increment operator. No effect, as insertion modifies the container automatically.
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ BackInsertIterator& operator++() {
            return *this;
        }

        /// @brief Post-increment operator. No effect, as insertion modifies the container automatically.
        /// @return A reference to the current iterator
        __WSTL_CONSTEXPR14__ BackInsertIterator& operator++(int) {
            return *this;
        }

    protected:
        Container* m_Container;
    };

    /// @brief Makes a back insert iterator for inserting elements at the back of a container
    /// @param container The container to insert into
    /// @return A `BackInsertIterator` associated with the given container
    /// @ingroup iterator
    /// @see https://en.cppreference.com/w/cpp/iterator/back_inserter
    template<class Container>
    __WSTL_CONSTEXPR14__ BackInsertIterator<Container> BackInserter(Container& container) {
        return BackInsertIterator<Container>(container);
    }

    // Range access functions

    /// @brief Returns an iterator to the beginning of the container
    /// @param container The container whose beginning iterator is returned
    /// @return An iterator pointing to the first element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/begin
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::Iterator Begin(Container& container) {
        return container.Begin();
    }
    
    /// @brief Returns a const iterator to the beginning of the container
    /// @param container The container whose beginning iterator is returned
    /// @return A const iterator pointing to the first element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/begin
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstIterator Begin(const Container& container) {
        return container.Begin();
    }

    /// @brief Returns a const iterator to the beginning of the container
    /// @param container The container whose beginning iterator is returned
    /// @return A const iterator pointing to the first element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/begin
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstIterator ConstBegin(const Container& container) {
        return container.ConstBegin();
    }

    /// @brief Returns an iterator to the end of the container
    /// @param container The container whose end iterator is returned
    /// @return An iterator pointing to the last element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/end
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::Iterator End(Container& container) {
        return container.End();
    }

    /// @brief Returns a const iterator to the end of the container
    /// @param container The container whose end iterator is returned
    /// @return A const iterator pointing to the last element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/end
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstIterator End(const Container& container) {
        return container.End();
    }

    /// @brief Returns a const iterator to the end of the container
    /// @param container The container whose end iterator is returned
    /// @return A const iterator pointing to the last element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/end
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstIterator ConstEnd(const Container& container) {
        return container.ConstEnd();
    }

    /// @brief Returns a pointer to the beginning of a raw array
    /// @param array The array whose beginning is returned
    /// @return A pointer to the first element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/begin
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline T* Begin(T (&array)[N]) {
        return &array[0];
    }

    /// @brief Returns a const pointer to the beginning of a raw array
    /// @param array The array whose beginning is returned
    /// @return A const pointer to the first element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/begin
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline const T* Begin(const T (&array)[N]) {
        return &array[0];
    }

    /// @brief Returns a const pointer to the beginning of a raw array
    /// @param array The array whose beginning is returned
    /// @return A const pointer to the first element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/begin
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline const T* ConstBegin(const T (&array)[N]) {
        return &array[0];
    }

    /// @brief Returns a pointer to the end of a raw array
    /// @param array The array whose beginning is returned
    /// @return A pointer to the last element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/end
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline T* End(T (&array)[N]) {
        return &array[N];
    }

    /// @brief Returns a const pointer to the end of a raw array
    /// @param array The array whose beginning is returned
    /// @return A const pointer to the last element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/end
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline const T* End(const T (&array)[N]) {
        return &array[N];
    }

    /// @brief Returns a const pointer to the end of a raw array
    /// @param array The array whose beginning is returned
    /// @return A const pointer to the last element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/end
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline const T* ConstEnd(const T (&array)[N]) {
        return &array[N];
    }

    // Reverse range access functions

    /// @brief Returns a reverse iterator to the beginning (last element) of the container
    /// @param container The container whose reverse beginning iterator is returned
    /// @return A reverse iterator pointing to the last element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rbegin
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ReverseIterator ReverseBegin(Container& container) {
        return container.ReverseBegin();
    }

    /// @brief Returns a const reverse iterator to the beginning (last element) of the container
    /// @param container The container whose reverse beginning iterator is returned
    /// @return A const reverse iterator pointing to the last element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rbegin
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstReverseIterator ReverseBegin(const Container& container) {
        return container.ReverseBegin();
    }

    /// @brief Returns a const reverse iterator to the beginning (last element) of the container
    /// @param container The container whose reverse beginning iterator is returned
    /// @return A const reverse iterator pointing to the last element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rbegin
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstReverseIterator ConstReverseBegin(const Container& container) {
        return container.ConstReverseBegin();
    }

    /// @brief Returns a reverse iterator to the end (before first element) of the container
    /// @param container The container whose reverse end iterator is returned
    /// @return A reverse iterator pointing past the first element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rend
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ReverseIterator ReverseEnd(Container& container) {
        return container.ReverseEnd();
    }

    /// @brief Returns a const reverse iterator to the end (before first element) of the container
    /// @param container The container whose reverse end iterator is returned
    /// @return A const reverse iterator pointing past the first element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rend
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstReverseIterator ReverseEnd(const Container& container) {
        return container.ReverseEnd();
    }

    /// @brief Returns a const reverse iterator to the end (before first element) of the container
    /// @param container The container whose reverse end iterator is returned
    /// @return A const reverse iterator pointing past the first element of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rend
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ConstReverseIterator ConstReverseEnd(const Container& container) {
        return container.ConstReverseEnd();
    }

    /// @brief Returns a reverse iterator to the beginning (last element) of a raw array
    /// @param array The array whose reverse beginning is returned
    /// @return A reverse iterator pointing to the last element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rbegin
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline ReverseIterator<T*> ReverseBegin(T (&array)[N]) {
        return ReverseIterator<T*>(&array[N]);
    }

    /// @brief Returns a const reverse iterator to the beginning (last element) of a raw array
    /// @param array The array whose reverse beginning is returned
    /// @return A const reverse iterator pointing to the last element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rbegin
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline ReverseIterator<const T*> ConstReverseBegin(const T (&array)[N]) {
        return ReverseIterator<const T*>(&array[N]);
    }

    /// @brief Returns a reverse iterator to the end (before first element) of a raw array
    /// @param array The array whose reverse end is returned
    /// @return A reverse iterator pointing before the first element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rend
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline ReverseIterator<T*> ReverseEnd(T (&array)[N]) {
        return ReverseIterator<T*>(&array[0]);
    }

    /// @brief Returns a const reverse iterator to the end (before first element) of a raw array
    /// @param array The array whose reverse end is returned
    /// @return A const reverse iterator pointing before the first element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/rend
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline ReverseIterator<const T*> ConstReverseEnd(const T (&array)[N]) {
        return ReverseIterator<const T*>(&array[0]);
    }

    // Size
    
    /// @brief A compile-time function that returns the size of an array
    /// @param array A reference to the array
    /// @return A reference to a character array of size `N`, allowing compile-time size deduction
    /// @ingroup container
    template<typename T, size_t N>
    char(&ArraySize(T(&array)[N]))[N];

    #define ARRAY_SIZE(a) sizeof(ArraySize(a))
    
    /// @brief Returns the number of elements in a raw array
    /// @param array A reference to the array
    /// @return The number of elements in the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/size
    template<typename T, size_t N>
    __WSTL_CONSTEXPR__ inline size_t Size(T (&array)[N]) {
        return N;
    }

    /// @brief Returns the number of elements in a container
    /// @param container The container whose size is to be retrieved
    /// @return The number of elements in the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/size
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::SizeType Size(const Container& container) {
        return container.Size();
    }

    // Empty

    template<typename Container>
    /// @brief Checks if a container is empty
    /// @param container The container to check
    /// @return True if the container is empty, false otherwise
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/empty
    __WSTL_CONSTEXPR__ inline bool Empty(const Container& container) {
        return container.Empty();
    }

    template<typename T, size_t N>
    /// @brief Checks if a fixed-size array is empty
    /// @param array The array to check
    /// @return Always false, as fixed-size arrays are never empty
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/empty
    __WSTL_CONSTEXPR__ inline bool Empty(T (&array)[N]) {
        return false;
    }

    // Data

    /// @brief Retrieves a pointer to the underlying data of the given container
    /// @param container Reference to the container
    /// @return Pointer to the underlying data storage of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/data
    template<typename Container>
    __WSTL_CONSTEXPR__ inline typename Container::ValueType* Data(Container& container) {
        return container.Data();
    }

    /// @brief Retrieves a const pointer to the underlying data of the given container
    /// @tparam Container Type of the container
    /// @param container Reference to the container
    /// @return Const pointer to the underlying data storage of the container
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/data
    template<typename Container>
    __WSTL_CONSTEXPR__ inline const typename Container::ValueType* Data(const Container& container) {
        return container.Data();
    }

    template<typename T, size_t N>
    /// @brief Retrieves a pointer to the first element of a raw array
    /// @param array Reference to the array
    /// @return Pointer to the first element of the array
    /// @ingroup container
    /// @see https://en.cppreference.com/w/cpp/iterator/data
    __WSTL_CONSTEXPR__ inline T* Data(T (&array)[N]) __WSTL_NOEXCEPT__ {
        return array;
    }
}        



#endif