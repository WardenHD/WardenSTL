#ifndef __WSTL_BOUNDEDITERATOR_HPP__
#define __WSTL_BOUNDEDITERATOR_HPP__

#include "private/Platform.hpp"
#include "Iterator.hpp"

namespace wstl {
    // Forward bounded iterator

    template<typename T>
    class ForwardBoundedIterator : public Iterator<ForwardIteratorTag, T> {
    public:
        typedef typename IteratorTraits<T>::ValueType ValueType;
        typedef ForwardIteratorTag IteratorCategory;
        typedef typename IteratorTraits<T>::ReferenceType ReferenceType;
        typedef typename IteratorTraits<T>::PointerType PointerType;
        typedef typename MakeUnsigned<typename IteratorTraits<T>::DifferenceType>::Type DifferenceType;

        __WSTL_CONSTEXPR14__ ForwardBoundedIterator() : m_Current(T()), m_End(T()) {}

        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(T begin, T end) : m_Current(begin), m_End(end) {}

        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(const ForwardBoundedIterator& other) : m_Current(other.m_Current), m_End(other.m_End) {}

        template<typename U>
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(const ForwardBoundedIterator<U>& other) : m_Current(other.m_Current), m_End(other.m_End) {}

        #ifdef __WSTL_CXX11__
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(ForwardBoundedIterator&& other) : m_Current(Move(other.m_Current)), m_End(Move(other.m_End)) {}

        template<typename U>
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator(ForwardBoundedIterator<U>&& other) : m_Current(Move(other.m_Current)), m_End(Move(other.m_End)) {}
        #endif

        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator=(const ForwardBoundedIterator& other) {
            m_Current = other.m_Current;
            m_End = other.m_End;
            return *this;
        }

        template<typename U>
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator=(const ForwardBoundedIterator<U>& other) {
            m_Current = other.m_Current;
            m_End = other.m_End;
            return *this;
        }

        #ifdef __WSTL_CXX11__
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator=(ForwardBoundedIterator&& other) {
            m_Current = Move(other.m_Current);
            m_End = Move(other.m_End);
            return *this;
        }

        template<typename U>
        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator=(ForwardBoundedIterator<U>&& other) {
            m_Current = Move(other.m_Current);
            m_End = Move(other.m_End);
            return *this;
        }
        #endif

        __WSTL_CONSTEXPR14__ ReferenceType operator*() { 
            return *m_Current; 
        }

        __WSTL_CONSTEXPR14__ const ReferenceType operator*() const { 
            return *m_Current; 
        }

        __WSTL_CONSTEXPR14__ T operator->() {
            return m_Current;
        }

        __WSTL_CONSTEXPR14__ const T operator->() const {
            return m_Current;
        }

        __WSTL_CONSTEXPR14__ operator T() const {
            return m_Current;
        }

        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator++() {
            if(m_Current != m_End) ++m_Current;
            return *this;
        }

        __WSTL_CONSTEXPR14__ ForwardBoundedIterator& operator++(int) {
            ForwardBoundedIterator original(*this);
            ++(*this);
            return original;
        }

    private:
        T m_Current;
        T m_End;
    };

    // Comparison operators

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator==(const ForwardBoundedIterator<U>& a, const ForwardBoundedIterator<U>& b) {
        return U(a) == U(b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator==(const ForwardBoundedIterator<U>& a, U b) {
        return U(a) == b;
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator==(U a, const ForwardBoundedIterator<U>& b) {
        return a == U(b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator!=(const ForwardBoundedIterator<U>& a, const ForwardBoundedIterator<U>& b) {
        return !(a == b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator!=(const ForwardBoundedIterator<U>& a, U b) {
        return !(a == b);
    }

    template<typename U>
    __WSTL_CONSTEXPR14__ inline bool operator!=(U a, const ForwardBoundedIterator<U>& b) {
        return !(a == b);
    }
}

#endif