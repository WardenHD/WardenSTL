// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s error_handler.h
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been modified by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_ERRORHANDLER_HPP__
#define __WSTL_ERRORHANDLER_HPP__

#include "Exception.hpp"
#include "Functional.hpp"
#include "Utility.hpp"
#include "private/Error.hpp"


/// @defgroup error_handler Error Handler
/// @brief Error handler for when throwing exceptions is not required
/// @ingroup wstl

#ifdef __WSTL_HANDLE_ERRORS__
namespace wstl {
    /// @brief Error handler that is used when throwing exceptions is not required
    /// @ingroup error_handler
    /// @note Requires `__WSTL_HANDLE_ERRORS__` to be defined
    class ErrorHandler {
    public:
        /// @brief Sets function that will be called when an exception occurs
        /// @param function Function wrapper class that takes `const Exception&` as a parameter
        static void SetCallback(const Function<void(const Exception&)>& function) {
            Create(function);
        }

        /// @brief Sets function that will be called when an exception occurs
        /// @param function Member function wrapper class that takes `const Exception&` as a parameter
        template<typename Object>
        static void SetCallback(const Function<void(const Exception&), Object>& function) {
            Create<Object>(function);
        }

        /// @brief Sets function that will be called when an exception occurs
        /// @param function Const member function wrapper class that takes `const Exception&` as a parameter
        template<typename Object>
        static void SetCallback(const Function<void(const Exception&), const Object>& function) {
            Create<const Object>(function);
        }

        /// @brief Sets function that will be called when an exception occurs
        /// @param function Function pointer that takes `const Exception&` as a parameter
        template<void(*Function)(const Exception&)>
        static void SetCallback() {
            Create(Function);
        }

        // Run-time

        /// @brief Sets function that will be called when an exception occurs - runtime
        /// @tparam Object Type of the object
        /// @tparam Function Member function pointer that takes `const Exception&` as a parameter
        /// @param object Pointer to the object
        template<typename Object, void(Object::*Function)(const Exception&)>
        static void SetCallback(Object& object) {
            Create<Object>(Function, (void*) (&object));
        }

        /// @brief Sets function that will be called when an exception occurs - runtime
        /// @tparam Object Type of the object
        /// @tparam Function Const member function pointer that takes `const Exception&` as a parameter
        /// @param object Const pointer to the object
        template<typename Object, void(Object::*Function)(const Exception&) const>
        static void SetCallback(const Object& object) {
            Create<const Object>(Function, (void*) (&object));
        }

        // Compile-time

        /// @brief Sets function that will be called when an exception occurs - compile-time
        /// @tparam Object Type of the object
        /// @tparam Instance Reference to the object
        /// @tparam Function Member function pointer that takes `const Exception&` as a parameter
        template<typename Object, Object& Instance, void(Object::*Function)(const Exception&)>
        static void SetCallback() {
            Create<Object, Instance>(Function);
        }

        /// @brief Sets function that will be called when an exception occurs - compile-time
        /// @tparam Object Type of the object
        /// @tparam Instance Const reference to the object
        /// @tparam Function Const member function pointer that takes `const Exception&` as a parameter
        template<typename Object, const Object& Instance, void(Object::*Function)(const Exception&) const>
        static void SetCallback() {
            Create<const Object, Instance>(Function);
        }

        // Error

        /// @brief Calls the error handler function
        /// @param exception Exception that occurred
        static void Error(const Exception& exception) {
            Function<void(const Exception&)>& function = GetCallback();
            if(function) function(exception);
        }
    
    private:
        // Member

        /// @brief Gets the callback function
        /// @tparam Object Type of the object (default is void)
        /// @return Reference to the function wrapper class
        template<typename Object = void>
        static Function<void(const Exception&), Object>& GetCallback() {
            static Function<void(const Exception&), Object> function;
            return function;
        }

        // Const member

        /// @brief Gets the callback function
        /// @tparam Object Type of the object (default is void)
        /// @return Reference to the const function wrapper class
        template<typename Object>
        static Function<void(const Exception&), const Object>& GetCallback() {
            static Function<void(const Exception&), const Object> function;
            return function;
        }

        // Create 

        /// @brief Creates the callback function
        /// @param function Function wrapper class that takes `const Exception&` as a parameter
        static void Create(const Function<void(const Exception&)>& function) {
            Function<void(const Exception&)>& staticFunction = GetCallback<>();
            staticFunction = function;
        }

        /// @brief Creates the callback function
        /// @tparam Object Type of the object
        /// @param function Member function wrapper class that takes `const Exception&` as a parameter
        template<typename Object>
        static void Create(const Function<void(const Exception&), Object>& function) {
            Function<void(const Exception&), Object>& staticFunction = GetCallback<Object>();
            staticFunction = function;
        }

        /// @brief Creates the callback function
        /// @tparam Object Type of the object
        /// @param function Const member function wrapper class that takes `const Exception&` as a parameter
        template<typename Object>
        static void Create(const Function<void(const Exception&), const Object>& function) {
            Function<void(const Exception&), const Object>& staticFunction = GetCallback<const Object>();
            staticFunction = function;
        }

        /// @brief Creates the callback function
        /// @param function Function pointer that takes `const Exception&` as a parameter
        static void Create(void(*function)(const Exception&)) {
            Function<void(const Exception&)>& staticFunction = GetCallback<>();
            staticFunction = function;
        }

        // Runtime

        /// @brief Creates the callback function - runtime
        /// @tparam Object Type of the object
        /// @param function Member function pointer that takes `const Exception&` as a parameter
        /// @param object Pointer to the object
        template<typename Object>
        static void Create(void(Object::*function)(const Exception&), void* object) {
            Function<void(const Exception&), Object>& staticFunction = GetCallback<Object>();
            staticFunction = MakePair(static_cast<Object*>(object), function);
        }

        // Const, runtime
        
        /// @brief Creates the callback function - runtime
        /// @tparam Object Type of the object
        /// @param function Const member function pointer that takes `const Exception&` as a parameter
        /// @param object Const pointer to the object
        template<typename Object>
        static void Create(void(Object::*function)(const Exception&) const, void* object) {
            Function<void(const Exception&), const Object>& staticFunction = GetCallback<const Object>();
            staticFunction = MakePair(static_cast<const Object*>(object), function);
        }

        // Compile-time

        /// @brief Creates the callback function - compile-time
        /// @tparam Object Type of the object
        /// @tparam Instance Reference to the object
        /// @param function Member function pointer that takes `const Exception&` as a parameter
        template<typename Object, Object& Instance>
        static void Create(void(Object::*function)(const Exception&), void*) {
            Function<void(const Exception&), Object>& staticFunction = GetCallback<Object>();
            staticFunction = MakePair(Instance, function);
        }

        // Const, compile-time

        /// @brief Creates the callback function - compile-time
        /// @tparam Object Type of the object
        /// @tparam Instance Const reference to the object
        /// @param function Const member function pointer that takes `const Exception&` as a parameter
        template<typename Object, const Object& Instance>
        static void Create(void(Object::*function)(const Exception&) const, void*) {
            Function<void(const Exception&), const Object>& staticFunction = GetCallback<const Object>();
            staticFunction = MakePair(Instance, function);
        }
    };

    namespace __private {
        static void __ErrorHandlerError(const Exception& exception) {
            ErrorHandler::Error(exception);
        }
    }
}
#endif
#endif