// #ifndef __WSTL_CRC_HPP__
// #define __WSTL_CRC_HPP__

// #include "private/Platform.hpp"
// #include "Bit.hpp"


// namespace wstl {
//     // CRC Compute steps

//     namespace __private {
//         template<typename T, T Polynomial, bool Reflect, T Input>
//         struct __CRCStep {
//         private:
//             static const __WSTL_CONSTEXPR__ bool XORPolynomial = Reflect ? (Input & T(1U)) == 1U : (Input & (T(1U) << (sizeof(T) * 8 - 1))) == 1U;
        
//         public:
//             static const __WSTL_CONSTEXPR__ T Value = Reflect ? (XORPolynomial ? ((Input >> 1U) ^ compile::ReverseBits<T, Polynomial>::Value) : (Input >> 1U)) : 
//                 (XORPolynomial ? ((Input << 1U) ^ Polynomial) : (Input << 1U));
//         };

//         template<typename T, T Polynomial, bool Reflect, T Input>
//         const __WSTL_CONSTEXPR__ T __CRCStep<T, Polynomial, Reflect, Input>::Value;
//     }

//     template<typename T, T Polynomial, bool Reflect, T Input, size_t N>
//     struct CRCComputeSteps {
//         static const __WSTL_CONSTEXPR__ T Value = __private::__CRCStep<T, Polynomial, Reflect, CRCComputeSteps<T, Polynomial, Reflect, Input, N - 1>::Value>::Value;
//     };

//     template<typename T, T Polynomial, bool Reflect, T Input>
//     struct CRCComputeSteps<T, Polynomial, Reflect, Input, 0> {
//         static const __WSTL_CONSTEXPR__ T Value = Input;
//     };

//     // CRC Table entry

//     template<typename T, T Polynomial, bool Reflect, size_t Index, uint8_t ChunkBits>
//     class CRCTableEntry;

//     // Chunk bit size of 2

//     template<typename T, T Polynomial, bool Reflect, size_t Index>
//     class CRCTableEntry<T, Polynomial, Reflect, Index, 2> {
//     public:
//         static const __WSTL_CONSTEXPR__ T Entry = Reflect ? T(Index) : (T(Index) << (sizeof(T) * 8 - 2));

//         static const __WSTL_CONSTEXPR__ T Value = CRCComputeSteps<T, Polynomial, Reflect, Entry, 2>::Value;
//     };

//     // Chunk bit size of 4

//     template<typename T, T Polynomial, bool Reflect, size_t Index>
//     class CRCTableEntry<T, Polynomial, Reflect, Index, 4> {
//     public:
//         static const __WSTL_CONSTEXPR__ T Entry = Reflect ? T(Index) : (T(Index) << (sizeof(T) * 8 - 4));

//         static const __WSTL_CONSTEXPR__ T Value = CRCComputeSteps<T, Polynomial, Reflect, Entry, 4>::Value;
//     };

//     // Chunk bit size of 8

//     template<typename T, T Polynomial, bool Reflect, size_t Index>
//     class CRCTableEntry<T, Polynomial, Reflect, Index, 8> {
//     public:
//         static const __WSTL_CONSTEXPR__ T Entry = Reflect ? T(Index) : (T(Index) << (sizeof(T) * 8 - 8));

//         static const __WSTL_CONSTEXPR__ T Value = CRCComputeSteps<T, Polynomial, Reflect, Entry, 8>::Value;
//     };

//     // CRC Type

//     template<typename Parameters, size_t TableSize>
//     class CRCType;

//     template<typename Parameters>
//     class CRCType<Parameters, 0> : public HasherBase<CRCType<Parameters, 0>, typename Parameters::Type, uint8_t> {
//     private:
//         typedef HasherBase<CRCType<Parameters, 0>, uint8_t, uint8_t> Base;

//     public:
//         typedef typename Base::HashType HashType;
//         typedef typename Base::ValueType ValueType;

//         __WSTL_CONSTEXPR__ CRCType() {
//             Reset();
//         };

//         template<typename Iterator>
//         __WSTL_CONSTEXPR14__ CRCType(Iterator first, Iterator last) {
//             WSTL_STATIC_ASSERT(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
//             Reset();
//             this->Append(first, last);
//         }

//         __WSTL_CONSTEXPR14__ void Reset() {
//             this->m_Hash = Parameters::ReflectValue ? compile::ReverseBits<typename Parameters::Type, Parameters::InitialValue>::Value : Parameters::InitialValue;
//         }

//         __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
//             this->m_Hash ^= value;

//             if __WSTL_IF_CONSTEXPR__(Parameters::ReflectValue) {
//                 for(size_t i = 0; i < 8; ++i) {
//                     if(this->m_Hash & 1) this->m_Hash = (this->m_Hash >> 1) ^ compile::ReverseBits<typename Parameters::Type, Parameters::PolynomialValue>::Value;
//                     else this->m_Hash >>= 1; 
//                 }
//             }
//             else {
//                 for(size_t i = 0; i < 8; ++i) {
//                     if(this->m_Hash & 0x80) this->m_Hash = (this->m_Hash << 1) ^ Parameters::PolynomialValue;
//                     else this->m_Hash <<= 1; 
//                 }
//             }
//         }
    
//     private:
//         friend Base;

//         __WSTL_CONSTEXPR14__ void Finalize() {
//             this->m_Hash ^= Parameters::FinalXORValue;
//         }
//     };
// }   

// #endif