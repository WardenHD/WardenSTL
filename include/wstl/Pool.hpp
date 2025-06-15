#ifndef __WSTL_POOL_HPP__
#define __WSTL_POOL_HPP__

#include "private/Platform.hpp"
#include "Container.hpp"
#include "PlacementNew.hpp"
#include "StandardExceptions.hpp"
#include "NullPointer.hpp"
#include "private/Error.hpp"
#include <stddef.h>



namespace wstl {
    template<typename T, const size_t SIZE>
    class Pool : public TypedContainerBase<T> {
    public:
        typedef typename TypedContainerBase<T>::ValueType ValueType;
        typedef typename TypedContainerBase<T>::SizeType SizeType;
        typedef typename TypedContainerBase<T>::DifferenceType DifferenceType;
        typedef typename TypedContainerBase<T>::ReferenceType ReferenceType;
        typedef typename TypedContainerBase<T>::ConstReferenceType ConstReferenceType;
        typedef typename TypedContainerBase<T>::PointerType PointerType;
        typedef typename TypedContainerBase<T>::ConstPointerType ConstPointerType;


        T* Allocate() {
            __WSTL_ASSERT_RETURNVALUE__(!this->Full(), LengthError("Pool is full"), NullPointer);
        }
    
    private:
        T m_Buffer[SIZE];
        SizeType m_ItemsInitialized;
    };
}

#endif