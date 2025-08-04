#ifndef __WSTL_JENKINS_HPP__
#define __WSTL_JENKINS_HPP__

#include "../Platform.hpp"
#include "../../HasherBase.hpp"
#include <stdint.h>


namespace wstl {
    class Jenkins : public HasherBase<Jenkins, uint32_t, uint8_t> {
    private:
        typedef HasherBase<Jenkins, uint32_t, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        __WSTL_CONSTEXPR14__ Jenkins() : Base(), m_IsFinalized(false) {}

        template<typename Iterator>
        __WSTL_CONSTEXPR14__ Jenkins(Iterator first, Iterator last) : Base(first, last), m_IsFinalized(false) {}

        __WSTL_CONSTEXPR14__ void Reset() {
            m_Hash = 0;
            m_IsFinalized = false;
        }

        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            __WSTL_ASSERT_RETURN__(!m_IsFinalized, WSTL_MAKE_EXCEPTION(LogicError, "Cannot add value to finalized Jenkins hash"));

            m_Hash += value;
            m_Hash += (m_Hash << 10);
            m_Hash ^= (m_Hash >> 6);
        }

    private:
        bool m_IsFinalized;

        friend Base;

        __WSTL_CONSTEXPR14__ void Finalize() {
            if(!m_IsFinalized) {
                m_Hash += (m_Hash << 3);
                m_Hash ^= (m_Hash >> 11);
                m_Hash += (m_Hash << 15);
            }

            m_IsFinalized = true;
        }
    };
}

#endif