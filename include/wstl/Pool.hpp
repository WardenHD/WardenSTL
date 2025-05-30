#ifndef __WSTL_POOL_HPP__
#define __WSTL_POOL_HPP__

#include "private/Platform.hpp"
#include <stddef.h>



namespace wstl {
    class IPool {
    public:
        typedef size_t SizeType;
        

    protected:
        IPool(char* buffer, SizeType itemSize, SizeType capacity) :
            m_Buffer(buffer), m_CurrentSize(0), m_ItemsInUse(0), 
            m_ItemSize(itemSize), m_Capacity(capacity) {}
    
    private:
        char* m_Buffer;
        SizeType m_CurrentSize;
        SizeType m_ItemsInUse;
        const SizeType m_ItemSize;
        const SizeType m_Capacity;
    };
}

#endif