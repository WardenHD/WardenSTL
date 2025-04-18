// #ifndef __WSTL_BASEIOS_HPP__
// #define __WSTL_BASEIOS_HPP__

// #include "private/Platform.hpp"
// #include "Exception.hpp"
// #include <stdint.h>

// namespace wstl {
//     inline const __WSTL_CONSTEXPR__ char EndLine = '\n';
//     inline const __WSTL_CONSTEXPR__ char EndString = '\0';

//     class IOSException : public Exception {
//         public:
//         IOSException(const char* message) : Exception(message) {}
//     };

//     class BaseIOS {
//     public:
//         BaseIOS() : m_State(IOStateBits::GOOD) {}

//         enum FormatFlags {
//             BOOL_ALPHA          = (1 << 0),
//             DECIMAL             = (1 << 1),
//             FIXED               = (1 << 2),
//             HEXADECIMAL         = (1 << 3),
//             INTERNAL            = (1 << 4),
//             LEFT                = (1 << 5),
//             OCTAL               = (1 << 6),
//             RIGHT               = (1 << 7),
//             SCIENTIFIC          = (1 << 8),
//             SHOW_BASE           = (1 << 9),
//             SHOW_POINT          = (1 << 10),
//             SHOW_POSITION       = (1 << 11),
//             SKIP_WHITESPACES    = (1 << 12),
//             UNIT_BUFFER         = (1 << 13),
//             UPPER_CASE          = (1 << 14),
//             ADJUST_FIELD        = (1 << 15),
//             BASE_FIELD          = (1 << 16),
//             FLOAT_FIELD         = (1 << 17)
//         };

//         enum OpenModeBits {
//             IN          = (1 << 0),
//             OUT         = (1 << 1),
//             APPEND      = (1 << 2),
//             AT_END      = (1 << 3),
//             BINARY      = (1 << 4),
//             TRUNCATE    = (1 << 5)
//         };

//         enum SeekDirection {
//             BEGIN,
//             CURRENT,
//             END,
//         };

//         enum IOStateBits {
//             GOOD = 0,
//             BAD = (1 << 0),
//             EOF = (1 << 1),
//             FAIL = (1 << 2)
//         };

//         typedef uint32_t OpenMode;
//         typedef uint32_t IOState;

//     private:
//         uint16_t m_State;
//     };
// }

// #endif