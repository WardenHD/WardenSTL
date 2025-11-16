// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s error handling utilities,
// and some concepts and functions have been adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_ERROR_HPP__
#define __WSTL_ERROR_HPP__

#include "Platform.hpp"
#include "../Exception.hpp"

#ifdef __WSTL_ASSERT_SUPPORT__
#include <assert.h>
#endif

// Defines introduced

/// @def __WSTL_ASSERT_SUPPORT__
/// @brief If defined, enables assert from assert.h in error handling macros
/// @ingroup error_handler

namespace wstl {
    namespace __private {
        static void __ErrorHandlerError(const Exception&);
    }
}


#ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_HANDLE_ERRORS__
        #define __WSTL_ASSERT__(condition, exception) {             \
            if(!(condition)) {                                      \
                wstl::__private::__ErrorHandlerError((exception));  \
                throw((exception));                                 \
            }                                                       \
        }

        #define __WSTL_ASSERT_RETURN__(condition, exception) {      \
            if(!(condition)) {                                      \
                wstl::__private::__ErrorHandlerError((exception));  \
                throw((exception));                                 \
                return;                                             \
            }                                                       \
        }

        #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) {  \
            if(!(condition)) {                                              \
                wstl::__private::__ErrorHandlerError((exception));          \
                throw((exception));                                         \
                return(value);                                              \
            }                                                               \
        }

        #define __WSTL_THROW__(exception) {                     \
            wstl::__private::__ErrorHandlerError((exception));  \
            throw((exception));                                 \
        }

        #define __WSTL_THROW_RETURN__(exception) {              \
            wstl::__private::__ErrorHandlerError((exception));  \
            throw((exception));                                 \
            return;                                             \
        }

        #define __WSTL_THROW_RETURNVALUE__(exception, value) {  \
            wstl::__private::__ErrorHandlerError((exception));  \
            throw((exception));                                 \
            return (value);                                     \
        }
    #else
        #define __WSTL_ASSERT__(condition, exception) { \
            if(!(condition)) throw((exception));        \
        }

        #define __WSTL_ASSERT_RETURN__(condition, exception) {  \
            if(!(condition)) {                                  \
                throw((exception));                             \
                return;                                         \
            }                                                   \
        }

        #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) {  \
            if(!(condition)) {                                              \
                throw((exception));                                         \
                return(value);                                              \
            }                                                               \
        }

        #define __WSTL_THROW__(exception) { \
            throw((exception));             \
        }

        #define __WSTL_THROW_RETURN__(exception) {  \
            throw((exception));                     \
            return;                                 \
        }

        #define __WSTL_THROW_RETURNVALUE__(exception, value) {  \
            throw((exception));                                 \
            return (value);                                     \
        }
    #endif
#else
    #ifdef __WSTL_HANDLE_ERRORS__
        #define __WSTL_ASSERT__(condition, exception) {                         \
            if(!(condition)) wstl::__private::__ErrorHandlerError((exception)); \
        }                                                                       

        #define __WSTL_ASSERT_RETURN__(condition, exception) {      \
            if(!(condition)) {                                      \
                wstl::__private::__ErrorHandlerError((exception));  \
                return;                                             \
            }                                                       \
        }

        #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) {  \
            if(!(condition)) {                                              \
                wstl::__private::__ErrorHandlerError((exception));          \
                return (value);                                             \
            }                                                               \
        }

        #define __WSTL_THROW__(exception) {                     \
            wstl::__private::__ErrorHandlerError((exception));  \
        }

        #define __WSTL_THROW_RETURN__(exception) {              \
            wstl::__private::__ErrorHandlerError((exception));  \
            return;                                             \
        }

        #define __WSTL_THROW_RETURNVALUE__(exception, value) {  \
            wstl::__private::__ErrorHandlerError((exception));  \
            return (value);                                     \
        }
    #else
        #ifdef __WSTL_ASSERT_SUPPORT__
            #define __WSTL_ASSERT__(condition, exception) assert((condition))

            #define __WSTL_ASSERT_RETURN__(condition, exception) {  \
                if(!(condition)) {                                  \
                    assert(false);                                  \
                    return;                                         \
                }                                                   \
            }

            #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) {  \
                if(!(condition)) {                                              \
                    assert(false);                                              \
                    return (value);                                             \
                }                                                               \
            }

            #define __WSTL_THROW__(exception) { \
                assert(false);                  \
            }

            #define __WSTL_THROW_RETURN__(exception) {  \
                assert(false);                          \
                return;                                 \
            }

            #define __WSTL_THROW_RETURNVALUE__(exception, value) {  \
                assert(false);                                      \
                return (value);                                     \
            }
        #else
            #define __WSTL_ASSERT__(condition, exception)

            #define __WSTL_ASSERT_RETURN__(condition, exception) {  \
                if(!(condition)) return;                            \
            }

            #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) {  \
                if(!(condition)) return (value);                                \
            }

            #define __WSTL_THROW__(exception)

            #define __WSTL_THROW_RETURN__(exception) {  \
                return;                                 \
            }

            #define __WSTL_THROW_RETURNVALUE__(exception, value) {  \
                return (value);                                     \
            }
        #endif
    #endif
#endif

#endif