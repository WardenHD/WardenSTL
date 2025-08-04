#ifndef __WSTL_HASHERBASE_HPP__
#define __WSTL_HASHERBASE_HPP__

#include "private/Platform.hpp"
#include "Iterator.hpp"


namespace wstl {
    template<typename Derived, typename THash, typename TValue>
    class HasherBase {
    public:
        typedef BackInsertIterator<Derived> InsertIterator;
        typedef THash HashType;
        typedef TValue ValueType;

        StaticAssert(IsUnsigned<ValueType>::Value && IsUnsigned<HashType>::Value, "Unsigned types in derived class are not supported");

        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            static_cast<Derived*>(this)->PushBack(value);
        }

        template<typename Iterator>
        __WSTL_CONSTEXPR14__ void Append(Iterator first, Iterator last) {
            for(; first != last; ++first) PushBack(*first);
        }

        #ifdef __WSTL_CXX11__
        template<typename Container>
        __WSTL_CONSTEXPR14__ void Append(Container&& container) {
            Append(Begin(container), End(container));
        }
        #else

        template<typename Container>
        void Append(const Container& container) {
            Append(Begin(container), End(container));
        }
        #endif

        __WSTL_CONSTEXPR14__ void Reset() {
            static_cast<Derived*>(this)->Reset();
        }

        __WSTL_CONSTEXPR14__ HashType Value() {
            static_cast<Derived*>(this)->Finalize();
            return m_Hash;
        }

        __WSTL_CONSTEXPR14__ operator HashType() {
            static_cast<Derived*>(this)->Finalize();
            return m_Hash;
        }

        __WSTL_CONSTEXPR14__ InsertIterator Inserter() {
            return InsertIterator(*static_cast<Derived*>(this));
        }

    protected:
        HashType m_Hash;

        __WSTL_CONSTEXPR14__ HasherBase() {
            Reset();
        }

        template<typename Iterator>
        __WSTL_CONSTEXPR14__ HasherBase(Iterator first, Iterator last) {
            StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
            Reset();
            Append(first, last);
        }
    };
}

#endif