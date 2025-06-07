#ifndef __WSTL_CONTAINER_HPP__
#define __WSTL_CONTAINER_HPP__

#include "private/Platform.hpp"
#include <stddef.h>


namespace wstl {
    /// @brief Base class for all fixed-size containers
    /// @ingroup containers
    class FixedContainerBase {
    public:
        typedef size_t SizeType;

        /// @brief Gets the current size of the container
        SizeType Size() const {
            return m_CurrentSize;
        }

        /// @brief Checks if the container is empty
        bool Empty() const {
            return m_CurrentSize == 0;
        }

        /// @brief Gets the maximum size of the container
        SizeType MaxSize() const {
            return m_Capacity;
        }

        /// @brief Checks if the container is full
        bool Full() const {
            return m_CurrentSize == m_Capacity;
        }

        /// @brief Gets the capacity of the container
        SizeType Capacity() const {
            return m_Capacity;
        }

        /// @brief Gets the available space in the container
        SizeType Available() const {
            return MaxSize() - Size();
        }

    protected:
        /// @brief Protected constructor
        /// @param capacity The maximum number of elements container can hold
        FixedContainerBase(SizeType capacity) : m_Capacity(capacity), m_CurrentSize(0) {}

        /// @brief Protected destructor
        ~FixedContainerBase() {}

        SizeType m_CurrentSize;
        const SizeType m_Capacity;
    };

    /// @brief Base class for all typed containers
    /// @tparam T Type of the element to store in the container
    /// @ingroup containers
    template<typename T>
    class TypedContainerBase : public FixedContainerBase {
    public:
        typedef T ValueType;
        typedef typename FixedContainerBase::SizeType SizeType;
        typedef ptrdiff_t DifferenceType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;

    protected:
        /// @brief Protected constructor
        /// @param capacity The maximum number of elements container can hold
        TypedContainerBase(SizeType capacity) : FixedContainerBase(capacity) {}
    };
}

#endif