#ifndef __WSTL_BUMPALLOCATOR_HPP__
#define __WSTL_BUMPALLOCATOR_HPP__

#include "Allocator.hpp"
#include "NullPointer.hpp"
#include "private/Error.hpp" 
#include <stdint.h>


namespace wstl {
    class BumpAllocator : public Allocator {
    public:
        /// @brief Constructor
        /// @param base The base address of the memory block
        /// @param limit The limit of the memory block
        BumpAllocator(void* base, size_t limit) : m_Base(reinterpret_cast<uint8_t*>(base)),
            m_Allocated(0), m_Limit(limit) {}

        virtual void* Allocate(size_t size) __WSTL_OVERRIDE__ {
            __WSTL_ASSERT_RETURNVALUE__(m_Allocated + size <= m_Limit, BadAllocation("BumpAllocator: Allocation exceeds limit", __FILE__, __LINE__), NullPointer);

            void* result = m_Base + m_Allocated;
            m_Allocated += size;
            return result;
        }

        /// @brief Frees a block of memory at the specified address - does not do anything
        virtual void Free(void*) __WSTL_OVERRIDE__ {
            // Does not do anything, not operational
        }
    
    private:
        uint8_t* m_Base;
        size_t m_Allocated;
        size_t m_Limit;
    };
}

#endif