// #ifndef __WSTL_CRCTRAITS_HPP__
// #define __WSTL_CRCTRAITS_HPP__

// #include "private/Platform.hpp"
// #include <stdint.h>


// namespace wstl {
//     template<typename T, T Polynomial, T Initial, T FinalXOR, bool Reflect>
//     struct CRCParameters {
//         typedef T Type;
//         static const __WSTL_CONSTEXPR__ T PolynomialValue = Polynomial;
//         static const __WSTL_CONSTEXPR__ T InitialValue = Initial;
//         static const __WSTL_CONSTEXPR__ T FinalXORValue = FinalXOR;
//         static const __WSTL_CONSTEXPR__ bool ReflectValue = Reflect;
//     };

//     template<typename T, T Polynomial, T Initial, T FinalXOR, bool Reflect>
//     const __WSTL_CONSTEXPR__ T CRCParameters<T, Polynomial, Initial, FinalXOR, Reflect>::PolynomialValue;

//     template<typename T, T Polynomial, T Initial, T FinalXOR, bool Reflect>
//     const __WSTL_CONSTEXPR__ T CRCParameters<T, Polynomial, Initial, FinalXOR, Reflect>::InitialValue;

//     template<typename T, T Polynomial, T Initial, T FinalXOR, bool Reflect>
//     const __WSTL_CONSTEXPR__ T CRCParameters<T, Polynomial, Initial, FinalXOR, Reflect>::FinalXORValue;

//     template<typename T, T Polynomial, T Initial, T FinalXOR, bool Reflect>
//     const __WSTL_CONSTEXPR__ bool CRCParameters<T, Polynomial, Initial, FinalXOR, Reflect>::ReflectValue;

//     // 8-bit

//     typedef CRCParameters<uint8_t, 0xD5, 0, 0, false> CRC8ParametersDVBS2;


// }

// #endif