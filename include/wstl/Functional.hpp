// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_FUNCTIONAL_HPP__
#define __WSTL_FUNCTIONAL_HPP__

#include "private/Platform.hpp"
#include "private/Swap.hpp"
#include "Tuple.hpp"
#include "private/Error.hpp"


/// @defgroup functional Functional
/// @brief Functional programming utilities
/// @ingroup utilities

namespace wstl {
    // Function exceptions

    /// @brief Exception class for invalid function calls
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bad_function_call
    class BadFunctionCall : public Exception {
    public:
        /// @brief Constructor
        /// @param file The name of the source file where the exception occurred
        /// @param line The line number in the source file where the exception occurred
        BadFunctionCall(StringType file, NumericType line) : Exception("Bad function call", file, line) {}
    };

    // Unary function
    
    /// @brief Template for unary function traits
    /// @tparam Arg Type of the argument
    /// @tparam Return Return type of the function
    /// @deprecated Use `wstl::Function` instead
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unary_function
    template<typename Arg, typename Return>
    struct UnaryFunction {
        typedef Return ResultType;
        typedef Arg ArgumentType;
    };

    // Binary function

    /// @brief Template for binary function traits
    /// @tparam Arg1 Type of the first argument
    /// @tparam Arg2 Type of the second argument
    /// @tparam Return Return type of the function
    /// @deprecated Use `wstl::Function` instead
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binary_function
    template<typename Arg1, typename Arg2, typename Return>
    struct BinaryFunction {
        typedef Return ResultType;
        typedef Arg1 FirstArgumentType;
        typedef Arg2 SecondArgumentType;
    };

    // Function base

    /// @brief Base template for encapsulating callable objects
    /// @tparam Signature The function signature (`Return(Args...)`)
    /// @note In C++98 it supports maximum two arguments
    /// @ingroup functional
    template<typename Signature>
    class IFunction;

    #ifdef __WSTL_CXX11__
    template<typename Signature>
    class IFunction;

    template<typename Return, typename... Args>
    class IFunction<Return(Args...)> {
    public:
        typedef Return ResultType;

        virtual ~IFunction() {}

        virtual Return operator()(Args...) const = 0;
    };
    #else
    template<typename Return, typename Arg1, typename Arg2>
    class IFunction<Return(Arg1, Arg2)> {
    public:
        typedef Return ResultType;

        virtual ~IFunction() {}

        virtual Return operator()(Arg1, Arg2) const = 0;
    };

    template<typename Return, typename Arg>
    class IFunction<Return(Arg)> {
    public:
        typedef Return ResultType;

        virtual ~IFunction() {}

        virtual Return operator()(Arg) const = 0;
    };

    template<typename Return>
    class IFunction<Return()> {
    public:
        typedef Return ResultType;

        virtual ~IFunction() {}

        virtual Return operator()() const = 0;
    };
    #endif

    // Function 

    /// @brief Generic function wrapper that can hold and invoke callable objects
    /// @tparam Signature The signature of the function or callable object (`int(int, double)`)
    /// @tparam Object (Optional) The type of object for member functions. Default is `void` for free functions.
    /// @note In C++98 it supports maximum two arguments
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/function
    template<typename Signature, typename Object = void>
    class Function;

    #ifdef __WSTL_CXX11__
    // Function (many parameters)
    
    template<typename Return, typename... Args>
    class Function<Return(Args...)> : public IFunction<Return(Args...)> {
    public:
        __WSTL_CONSTEXPR14__ Function() __WSTL_NOEXCEPT__ : m_Function(nullptr) {}

        __WSTL_CONSTEXPR14__ Function(Return(*function)(Args...)) : m_Function(function) {}

        __WSTL_CONSTEXPR14__ Function(const Function& other) : m_Function(other.m_Function) {}

        __WSTL_CONSTEXPR14__ Function(Function&& other) __WSTL_NOEXCEPT__ : m_Function(other.m_Function) {
            other.m_Function = nullptr;
        }

        __WSTL_CONSTEXPR14__ void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
        }

        __WSTL_CONSTEXPR14__ Function& operator=(Return(*function)(Args...)) {
            m_Function = function;
            return *this;
        }

        __WSTL_CONSTEXPR14__ Function& operator=(const Function& other) {
            if(this != &other) m_Function = other.m_Function;
            return *this;
        }

        __WSTL_CONSTEXPR14__ Function& operator=(Function&& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                other.m_Function = nullptr;
            }
            return *this;
        }

        virtual Return operator()(Args&&... args) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (*m_Function)(Forward<Args>(args)...);
        }

        __WSTL_CONSTEXPR14__ operator bool() const {
            return m_Function != nullptr;
        }

        __WSTL_CONSTEXPR14__ Return(*Target() const)(Args...) {
            return m_Function;
        }

    private:
        Return (*m_Function)(Args...);
    };

    // Member

    template<typename Object, typename Return, typename... Args>
    class Function<Return(Args...), Object> : public IFunction<Return(Args...)> {
    public:
        typedef Object ObjectType;
        
        __WSTL_CONSTEXPR14__ Function() __WSTL_NOEXCEPT__ 
            : m_Function(nullptr), m_Object(nullptr) {}

        __WSTL_CONSTEXPR14__ Function(Object& object, Return(Object::*function)(Args...)) 
            : m_Function(function), m_Object(&object) {}

        __WSTL_CONSTEXPR14__ Function(const Function& other) 
            : m_Function(other.m_Function), m_Object(other.m_Object) {}

        __WSTL_CONSTEXPR14__ Function(Function&& other) __WSTL_NOEXCEPT__ 
            : m_Function(other.m_Function), m_Object(other.m_Object) {
            other.m_Object = nullptr;
            other.m_Function = nullptr;
        }

        __WSTL_CONSTEXPR14__ void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        __WSTL_CONSTEXPR14__ 
        Function& operator=(const Pair<Object&, Return(Object::*)(Args...)>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        __WSTL_CONSTEXPR14__ Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }

            return *this;
        }

        __WSTL_CONSTEXPR14__ Function& operator=(Function&& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
                other.m_Object = nullptr;
                other.m_Function = nullptr;
            }

            return *this;
        }

        virtual Return operator()(Args&&... args) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)(Forward<Args>(args)...);
        }

        __WSTL_CONSTEXPR14__ operator bool() const {
            return (m_Function != nullptr) && (m_Object != nullptr);
        }

        __WSTL_CONSTEXPR14__ Return(Object::*Target() const)(Args...) {
            return m_Function;
        }

    private:
        Object* m_Object;
        Return (Object::*m_Function)(Args...);
    };

    // Const member

    template<typename Object, typename Return, typename... Args>
    class Function<Return(Args...), const Object> : public IFunction<Return(Args...)> {
    public:
        typedef const Object ObjectType;

        __WSTL_CONSTEXPR14__ Function() __WSTL_NOEXCEPT__ 
            : m_Function(nullptr), m_Object(nullptr) {}

        __WSTL_CONSTEXPR14__ Function(const Object& object, Return(Object::*function)(Args...) const) 
            : m_Function(function), m_Object(&object) {}

        __WSTL_CONSTEXPR14__ Function(const Function& other) 
            : m_Function(other.m_Function), m_Object(other.m_Object) {}

        __WSTL_CONSTEXPR14__ Function(Function&& other) __WSTL_NOEXCEPT__ 
            : m_Function(other.m_Function), m_Object(other.m_Object) {
            other.m_Object = nullptr;
            other.m_Function = nullptr;
        }

        __WSTL_CONSTEXPR14__ void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        __WSTL_CONSTEXPR14__ 
        Function& operator=(const Pair<const Object&, Return(Object::*)(Args...) const>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        __WSTL_CONSTEXPR14__ Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }

            return *this;
        }

        __WSTL_CONSTEXPR14__ Function& operator=(Function&& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
                other.m_Object = nullptr;
                other.m_Function = nullptr;
            }

            return *this;
        }

        virtual Return operator()(Args&&... args) const override {
            return (m_Object->*m_Function)(Forward<Args>(args)...);
        }

        __WSTL_CONSTEXPR14__ operator bool() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Function != nullptr) && (m_Object != nullptr);
        }

        __WSTL_CONSTEXPR14__ Return(Object::*Target() const)(Args...) const {
            return m_Function;
        }

    private:
        const Object* m_Object;
        Return (Object::*m_Function)(Args...) const;
    };

    // Swap specialization

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline void Swap(Function<Return(Args...)>& a, Function<Return(Args...)>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline void Swap(Function<Return(Args...), Object>& a, Function<Return(Args...), Object>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(const Function<Return(Args...)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(NullPointerType, const Function<Return(Args...)>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(const Function<Return(Args...), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(NullPointerType, const Function<Return(Args...), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(const Function<Return(Args...)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == nullptr);
    }

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(NullPointerType, const Function<Return(Args...)>& function) __WSTL_NOEXCEPT__ {
        return !(nullptr == function);
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(const Function<Return(Args...), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == nullptr);
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(NullPointerType, const Function<Return(Args...), Object>& function) __WSTL_NOEXCEPT__ {
        return !(nullptr == function);
    }

    #else
    template<typename Return, typename Arg1, typename Arg2>
    class Function<Return(Arg1, Arg2)> : public IFunction<Return(Arg1, Arg2)> {
    public:
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        Function(Return(*function)(Arg1, Arg2)) : m_Function(function) {}

        Function(const Function& other) : m_Function(other.m_Function) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
        }

        Function& operator=(Return(*function)(Arg1, Arg2)) {
            m_Function = function;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) m_Function = other.m_Function;
            return *this;
        }

        virtual Return operator()(const Arg1& arg1, const Arg2& arg2) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (*m_Function)(arg1, arg2);
        }

        operator bool() const {
            return m_Function != NullPointer;
        }

        Return(*Target() const)(Arg1, Arg2) {
            return m_Function;
        }

    private:
        Return (*m_Function)(Arg1, Arg2);
    };

    // Member 

    template<typename Object, typename Return, typename Arg1, typename Arg2>
    class Function<Return(Arg1, Arg2), Object> : public IFunction<Return(Arg1, Arg2)> {
    public:
        typedef Object ObjectType;

        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        Function(Object& object, Return(Object::*function)(Arg1, Arg2)) 
            : m_Function(function), m_Object(&object) {}

        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        Function& operator=(const Pair<Object&, Return(Object::*)(Arg1, Arg2)>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }
            return *this;
        }

        virtual Return operator()(const Arg1& arg1, const Arg2& arg2) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg1, arg2);
        }

        operator bool() const {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        Return(Object::*Target() const)(Arg1, Arg2) {
            return m_Function;
        }

    private:
        Object* m_Object;
        Return (Object::*m_Function)(Arg1, Arg2);
    };

    // Const member

    template<typename Object, typename Return, typename Arg1, typename Arg2>
    class Function<Return(Arg1, Arg2), const Object> : public IFunction<Return(Arg1, Arg2)> {
    public:
        typedef const Object ObjectType;

        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        Function(const Object& object, Return(Object::*function)(Arg1, Arg2) const) 
            : m_Function(function), m_Object(&object) {}

        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        Function& operator=(const Pair<const Object&, Return(Object::*)(Arg1, Arg2) const>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }
            return *this;
        }

        virtual Return operator()(const Arg1& arg1, const Arg2& arg2) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg1, arg2);
        }

        operator bool() const {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        Return(Object::*Target() const)(Arg1, Arg2) const {
            return m_Function;
        }

    private:
        const Object* m_Object;
        Return (Object::*m_Function)(Arg1, Arg2) const;
    };

    // Swap specialization

    template<typename Return, typename Arg1, typename Arg2> 
    inline void Swap(Function<Return(Arg1, Arg2)>& a, Function<Return(Arg1, Arg2)>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline void Swap(Function<Return(Arg1, Arg2), Object>& a, Function<Return(Arg1, Arg2), Object>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(const Function<Return(Arg1, Arg2)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(NullPointerType, const Function<Return(Arg1, Arg2)>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(const Function<Return(Arg1, Arg2), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(NullPointerType, const Function<Return(Arg1, Arg2), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(const Function<Return(Arg1, Arg2)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg1, Arg2)>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(const Function<Return(Arg1, Arg2), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg1, Arg2), Object>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }


    // Function (1 argument)

    template<typename Return, typename Arg>
    class Function<Return(Arg)> : public IFunction<Return(Arg)> {
    public:
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        Function(Return(*function)(Arg)) : m_Function(function) {}

        Function(const Function& other) : m_Function(other.m_Function) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
        }

        Function& operator=(Return(*function)(Arg)) {
            m_Function = function;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) m_Function = other.m_Function;
            return *this;
        }

        virtual Return operator()(const Arg& arg) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (*m_Function)(arg);
        }

        operator bool() const {
            return m_Function != NullPointer;
        }

        Return(*Target() const)(Arg) {
            return m_Function;
        }

    private:
        Return (*m_Function)(Arg);
    };

    // Member

    template<typename Object, typename Return, typename Arg>
    class Function<Return(Arg), Object> : public IFunction<Return(Arg)> {
    public:
        typedef Object ObjectType;

        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        Function(Object& object, Return(Object::*function)(Arg)) 
            : m_Function(function), m_Object(&object) {}

        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        Function& operator=(const Pair<Object&, Return(Object::*)(Arg)>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }

            return *this;
        }

        virtual Return operator()(const Arg& arg) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg);
        }

        operator bool() const {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        Return(Object::*Target() const)(Arg) {
            return m_Function;
        }

    private:
        Object* m_Object;
        Return (Object::*m_Function)(Arg);
    };

    // Const member

    template<typename Object, typename Return, typename Arg>
    class Function<Return(Arg), const Object> : public IFunction<Return(Arg)> {
    public:
        typedef const Object ObjectType;

        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        Function(const Object& object, Return(Object::*function)(Arg) const) 
            : m_Function(function), m_Object(&object) {}

        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        Function& operator=(const Pair<const Object&, Return(Object::*)(Arg) const>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }

            return *this;
        }

        virtual Return operator()(const Arg& arg) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg);
        }

        operator bool() const {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        Return(Object::*Target() const)(Arg) const {
            return m_Function;
        }

    private:
        const Object* m_Object;
        Return (Object::*m_Function)(Arg) const;
    };

    // Swap specialization

    template<typename Return, typename Arg> 
    inline void Swap(Function<Return(Arg)>& a, Function<Return(Arg)>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    template<typename Object, typename Return, typename Arg>
    inline void Swap(Function<Return(Arg), Object>& a, Function<Return(Arg), Object>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return, typename Arg> 
    inline bool operator==(const Function<Return(Arg)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg> 
    inline bool operator==(NullPointerType, const Function<Return(Arg)>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator==(const Function<Return(Arg), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator==(NullPointerType, const Function<Return(Arg), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg> 
    inline bool operator!=(const Function<Return(Arg)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Return, typename Arg> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg)>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator!=(const Function<Return(Arg), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg), Object>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }


    // Function (no arguments)

    template<typename Return>
    class Function<Return()> : public IFunction<Return()> {
    public:
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        Function(Return(*function)()) : m_Function(function) {}

        Function(const Function& other) : m_Function(other.m_Function) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
        }

        Function& operator=(Return(*function)()) {
            m_Function = function;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) m_Function = other.m_Function;
            return *this;
        }

        virtual Return operator()() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (*m_Function)();
        }

        operator bool() const {
            return m_Function != NullPointer;
        }

        Return(*Target() const)() {
            return m_Function;
        }

    private:
        Return (*m_Function)();
    };

    // Member

    template<typename Object, typename Return>
    class Function<Return(), Object> : public IFunction<Return()> {
    public:
        typedef Object ObjectType;

        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        Function(Object& object, Return(Object::*function)()) 
            : m_Function(function), m_Object(&object) {}

        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        Function& operator=(const Pair<Object&, Return(Object::*)()>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }

            return *this;
        }

        virtual Return operator()() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)();
        }

        operator bool() const {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        Return(Object::*Target() const)() {
            return m_Function;
        }

    private:
        Object* m_Object;
        Return (Object::*m_Function)();
    };

    // Const member

    template<typename Object, typename Return>
    class Function<Return(), const Object> : public IFunction<Return()> {
    public:
        typedef const Object ObjectType;

        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        Function(const Object& object, Return(Object::*function)() const) 
            : m_Function(function), m_Object(&object) {}

        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        Function& operator=(const Pair<const Object&, Return(Object::*)() const>& pair) {
            m_Function = pair.Second;
            m_Object = &pair.First;
            return *this;
        }

        Function& operator=(const Function& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
            }

            return *this;
        }

        virtual Return operator()() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), BadFunctionCall(__FILE__, __LINE__), 
                static_cast<Return>(-1));
            return (m_Object->*m_Function)();
        }

        operator bool() const {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        Return(Object::*Target() const)() const {
            return m_Function;
        }

    private:
        const Object* m_Object;
        Return (Object::*m_Function)() const;
    };

    // Swap specialization

    template<typename Return> 
    inline void Swap(Function<Return()>& a, Function<Return()>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    template<typename Object, typename Return> 
    inline void Swap(Function<Return(), Object>& a, Function<Return(), Object>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return> 
    inline bool operator==(const Function<Return()>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return> 
    inline bool operator==(NullPointerType, const Function<Return()>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return> 
    inline bool operator==(const Function<Return(), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return> 
    inline bool operator==(NullPointerType, const Function<Return(), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return> 
    inline bool operator!=(const Function<Return()>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Return> 
    inline bool operator!=(NullPointerType, const Function<Return()>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }  

    template<typename Object, typename Return> 
    inline bool operator!=(const Function<Return(), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Object, typename Return> 
    inline bool operator!=(NullPointerType, const Function<Return(), Object>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }
    #endif

    // Reference wrapper

    /// @brief Wrapper class to store references
    /// @tparam T Type of the object to wrap
    /// @ingroup functional
    /// @note In C++98 it supports maximum two arguments for callable objects
    /// @see https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper 
    template<typename T>
    class ReferenceWrapper {
    public:
        /// @brief Alias for the wrapped type
        typedef T Type;

        /// @brief Constructor
        /// @param value The reference to the object to wrap
        __WSTL_CONSTEXPR14__ explicit ReferenceWrapper(T& value) __WSTL_NOEXCEPT__ : m_Pointer(&value)  {}
        
        /// @brief Copy constructor
        /// @param other Reference wrapper to copy from
        __WSTL_CONSTEXPR14__ ReferenceWrapper(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ 
            : m_Pointer(other.m_Pointer) {}      
        
        /// @brief Assignment operator
        /// @param other Reference wrapper to assign from
        __WSTL_CONSTEXPR14__
        ReferenceWrapper& operator=(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ {
            if(this != &other) m_Pointer = other.m_Pointer;
            return *this;
        }

        /// @brief Gets the wrapped reference
        /// @return The wrapped reference to the object
        __WSTL_CONSTEXPR14__ T& Get() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Implicit conversion operator to the wrapped reference
        /// @return The wrapped reference to the object
        __WSTL_CONSTEXPR14__ operator T&() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }
        
        #ifdef __WSTL_CXX11__
        /// @brief Calls the referenced object with the provided arguments, forwarding them
        /// @param args The arguments to be forwarded
        /// @return The result of calling the referenced object
        template<typename... Args>
        __WSTL_CONSTEXPR14__
        ResultOfType<T&(Args&&...)> operator()(Args&&... args) const {
            return (*m_Pointer)(Forward<Args>(args)...);
        }
        #else
        /// @brief Calls the referenced object with two arguments
        /// @param arg1 Value of the first argument
        /// @param arg2 Value of the second argument
        /// @return The result of calling the referenced object
        template<typename Arg1, typename Arg2>
        typename ResultOf<T&(const Arg1&, const Arg2&)>::Type operator()(const Arg1& arg1, const Arg2& arg2) const {
            return (*m_Pointer)(arg1, arg2);
        }

        /// @brief Calls the referenced object with one argument
        /// @param arg Value of the argument
        /// @return The result of calling the referenced object
        template<typename Arg>
        typename ResultOf<T&(const Arg&)>::Type operator()(const Arg& arg) const {
            return (*m_Pointer)(arg);
        }

        /// @brief Calls the referenced object with no arguments
        /// @return The result of calling the referenced object
        typename ResultOf<T&()>::Type operator()() const {
            return (*m_Pointer)();
        }
        #endif

    private:
        T* m_Pointer;
    };

    /// @brief Wrapper class to store volatile references
    /// @tparam T Type of the object to wrap
    /// @ingroup functional
    /// @note In C++98 it supports maximum two arguments for callable objects
    template<typename T>
    class ReferenceWrapper<volatile T> {
    public:
        /// @brief Alias for the wrapped type
        typedef volatile T Type;

        /// @brief Constructor
        /// @param value The volatile reference to the object to wrap
        __WSTL_CONSTEXPR14__ explicit ReferenceWrapper(volatile T& value) __WSTL_NOEXCEPT__ : m_Pointer(&value)  {}
        
        /// @brief Copy constructor
        /// @param other Reference wrapper to copy from
        __WSTL_CONSTEXPR14__ ReferenceWrapper(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ 
            : m_Pointer(other.m_Pointer) {}      
        
        /// @brief Assignment operator
        /// @param other Reference wrapper to assign from
        __WSTL_CONSTEXPR14__
        ReferenceWrapper& operator=(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ {
            if(this != &other) m_Pointer = other.m_Pointer;
            return *this;
        }

        /// @brief Gets the wrapped reference
        /// @return The wrapped volatile reference to the object
        __WSTL_CONSTEXPR14__ volatile T& Get() const volatile __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Implicit conversion operator to the wrapped reference
        /// @return The wrapped volatile reference to the object
        __WSTL_CONSTEXPR14__ operator volatile T&() const volatile __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }
        
        #ifdef __WSTL_CXX11__
        /// @brief Calls the referenced object with the provided arguments, forwarding them
        /// @param args The arguments to be forwarded
        /// @return The result of calling the referenced object
        template<typename... Args>
        __WSTL_CONSTEXPR14__
        ResultOfType<volatile T&(Args&&...)> operator()(Args&&... args) const volatile {
            return (*m_Pointer)(Forward<Args>(args)...);
        }
        #else
        /// @brief Calls the referenced object with the two arguments
        /// @param arg1 Value of the first argument
        /// @param arg2 Value of the second argument
        /// @return The result of calling the referenced object
        template<typename Arg1, typename Arg2>
        typename ResultOf<volatile T&(const Arg1&, const Arg2&)>::Type operator()(const Arg1& arg1, const Arg2& arg2) const volatile {
            return (*m_Pointer)(arg1, arg2);
        }

        /// @brief Calls the referenced object with the one arguments
        /// @param arg Value of the argument
        /// @return The result of calling the referenced object
        template<typename Arg>
        typename ResultOf<volatile T&(const Arg&)>::Type operator()(const Arg& arg) const volatile {
            return (*m_Pointer)(arg);
        }

        /// @brief Calls the referenced object with the no arguments
        /// @return The result of calling the referenced object
        typename ResultOf<volatile T&()>::Type operator()() const volatile {
            return (*m_Pointer)();
        }
        #endif

    private:
        volatile T* m_Pointer;
    };

    /// @brief Wrapper class to store const volatile references
    /// @tparam T Type of the object to wrap
    /// @ingroup functional
    /// @note In C++98 it supports maximum two arguments for callable objects
    template<typename T>
    class ReferenceWrapper<const volatile T> {
    public:
        /// @brief Alias for the wrapped type
        typedef const volatile T Type;

        /// @brief Constructor
        /// @param value The const volatile reference to the object to wrap
        __WSTL_CONSTEXPR14__ explicit ReferenceWrapper(const volatile T& value) __WSTL_NOEXCEPT__ : m_Pointer(&value) {}
        
        /// @brief Copy constructor
        /// @param other Reference wrapper to copy from
        __WSTL_CONSTEXPR14__ ReferenceWrapper(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ 
            : m_Pointer(other.m_Pointer) {}      
        
        /// @brief Assignment operator
        /// @param other Reference wrapper to assign from
        __WSTL_CONSTEXPR14__
        ReferenceWrapper& operator=(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ {
            if(this != &other) m_Pointer = other.m_Pointer;
            return *this;
        }

        /// @brief Gets the wrapped reference
        /// @return The wrapped const volatile reference to the object
        __WSTL_CONSTEXPR14__ const volatile T& Get() const volatile __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Implicit conversion operator to the wrapped reference
        /// @return The wrapped const volatile reference to the object
        __WSTL_CONSTEXPR14__ operator const volatile T&() const volatile __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }
        
        #ifdef __WSTL_CXX11__
        /// @brief Calls the referenced object with the provided arguments, forwarding them
        /// @param args The arguments to be forwarded
        /// @return The result of calling the referenced object
        template<typename... Args>
        __WSTL_CONSTEXPR14__
        ResultOfType<const volatile T&(Args&&...)> operator()(Args&&... args) const volatile {
            return (*m_Pointer)(Forward<Args>(args)...);
        }
        #else
        /// @brief Calls the referenced object with the two arguments
        /// @param arg1 Value of the first argument
        /// @param arg2 Value of the second argument
        /// @return The result of calling the referenced object
        template<typename Arg1, typename Arg2>
        typename ResultOf<const volatile T&(const Arg1&, const Arg2&)>::Type operator()(const Arg1& arg1, const Arg2& arg2) const volatile {
            return (*m_Pointer)(arg1, arg2);
        }

        /// @brief Calls the referenced object with the one arguments
        /// @param arg Value of the argument
        /// @return The result of calling the referenced object
        template<typename Arg>
        typename ResultOf<const volatile T&(const Arg&)>::Type operator()(const Arg& arg) const volatile {
            return (*m_Pointer)(arg);
        }

        /// @brief Calls the referenced object with the no arguments
        /// @return The result of calling the referenced object
        typename ResultOf<const volatile T&()>::Type operator()() const volatile {
            return (*m_Pointer)();
        }
        #endif

    private:
        const volatile T* m_Pointer;
    };

    // Reference

    /// @brief Creates a `ReferenceWrapper` for non-const reference
    /// @param t Object to wrap
    /// @return A `ReferenceWrapper` that holds a reference to given object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    inline ReferenceWrapper<T> Reference(T& t) {
        return ReferenceWrapper<T>(t);
    }

    /// @brief Creates a `ReferenceWrapper` from another `ReferenceWrapper` for non-const reference
    /// @param t The `ReferenceWrapper` object to convert
    /// @return A `ReferenceWrapper` that holds a reference to object in given `ReferenceWrapper`
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    inline ReferenceWrapper<T> Reference(ReferenceWrapper<T> t) {
        return ReferenceWrapper<T>(t.Get());
    }

    // Const reference

    /// @brief Creates a `ReferenceWrapper` for const reference
    /// @param t Object to wrap
    /// @return A `ReferenceWrapper` that holds a reference to given object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    inline ReferenceWrapper<const T> ConstReference(const T& t) {
        return ReferenceWrapper<const T>(t);
    }

    /// @brief Creates a `ReferenceWrapper` from another `ReferenceWrapper` for const reference
    /// @param t The `ReferenceWrapper` object to convert
    /// @return A `ReferenceWrapper` that holds a reference to object in given `ReferenceWrapper`
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    ReferenceWrapper<const T> ConstReference(ReferenceWrapper<T> t) {
        return ReferenceWrapper<const T>(t.Get());
    }

    // Volatile reference

    /// @brief Creates a `ReferenceWrapper` for volatile reference
    /// @param t Object to wrap
    /// @return A `ReferenceWrapper` that holds a reference to given object
    /// @ingroup functional
    template<typename T>
    ReferenceWrapper<volatile T> VolatileReference(volatile T& t) {
        return ReferenceWrapper<volatile T>(t);
    }

    /// @brief Creates a `ReferenceWrapper` from another `ReferenceWrapper` for volatile reference
    /// @param t The `ReferenceWrapper` object to convert
    /// @return A `ReferenceWrapper` that holds a reference to object in given `ReferenceWrapper`
    /// @ingroup functional
    template<typename T>
    ReferenceWrapper<volatile T> VolatileReference(ReferenceWrapper<T> t) {
        return ReferenceWrapper<volatile T>(t.Get());
    }

    // Const volatile reference

    /// @brief Creates a `ReferenceWrapper` for const volatile reference
    /// @param t Object to wrap
    /// @return A `ReferenceWrapper` that holds a reference to given object
    /// @ingroup functional
    template<typename T>
    ReferenceWrapper<const volatile T> CVReference(const volatile T& t) {
        return ReferenceWrapper<const volatile T>(t);
    }

    /// @brief Creates a `ReferenceWrapper` from another `ReferenceWrapper` for const volatile reference
    /// @param t The `ReferenceWrapper` object to convert
    /// @return A `ReferenceWrapper` that holds a reference to object in given `ReferenceWrapper`
    /// @ingroup functional
    template<typename T>    
    ReferenceWrapper<const volatile T> CVReference(ReferenceWrapper<T> t) {
        return ReferenceWrapper<const volatile T>(t.Get());
    }

    // UnwrapReference specialization

    template<typename T>
    struct UnwrapReference<ReferenceWrapper<T> > { typedef T& Type; };

    // Plus

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Plus;
    #else
    template<typename T>
    struct Plus;
    #endif

    /// @brief Functor that represents addition for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/plus
    template<typename T>
    struct Plus : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The sum of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a + b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents addition for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/plus_void
    template<>
    struct Plus<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The sum of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) + Forward<U>(b)) {
            return Forward<T>(a) + Forward<U>(b);
        } 
    };
    #endif

    // Minus

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Minus;
    #else
    template<typename T>
    struct Minus;
    #endif

    /// @brief Functor that represents subtraction for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/minus
    template<typename T>
    struct Minus : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The difference of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a - b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents subtraction for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/minus_void
    template<>
    struct Minus<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The difference of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) - Forward<U>(b)) {
            return Forward<T>(a) - Forward<U>(b);
        } 
    };
    #endif

    // Multiplies

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Multiplies;
    #else
    template<typename T>
    struct Multiplies;
    #endif

    /// @brief Functor that represents multiplication for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/multiplies
    template<typename T>
    struct Multiplies : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The product of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a * b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents multiplication for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/multiplies_void
    template<>
    struct Multiplies<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The product of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) * Forward<U>(b)) {
            return Forward<T>(a) * Forward<U>(b);
        } 
    };
    #endif

    // Divides

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Divides;
    #else
    template<typename T>
    struct Divides;
    #endif

    /// @brief Functor that represents division for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/divides
    template<typename T>
    struct Divides : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The quotient of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a / b;
        }
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents division for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/divides_void
    template<>
    struct Divides<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The quotient of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) / Forward<U>(b)) {
            return Forward<T>(a) / Forward<U>(b);
        } 
    };
    #endif

    // Modulus

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Modulus;
    #else
    template<typename T>
    struct Modulus;
    #endif

    /// @brief Functor that represents the modulus operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/modulus
    template<typename T>
    struct Modulus : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The remainder of the division of the first argument by the second argument
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a % b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents the modulus operation for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/modulus_void
    template<>
    struct Modulus<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The remainder of the division of the first argument by the second argument
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) % Forward<U>(b)) {
            return Forward<T>(a) % Forward<U>(b);
        } 
    };
    #endif

    // Negate

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Negate;
    #else
    template<typename T>
    struct Negate;
    #endif

    /// @brief Functor that represents the negation operation for an object
    /// @tparam T Type of the object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/negate
    template<typename T>
    struct Negate : UnaryFunction<T, T> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The negation of the argument
        __WSTL_CONSTEXPR__ T operator()(const T& x) const {
            return -x;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents the negation operation for an object 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/negate_void
    template<>
    struct Negate<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to an argument
        /// @param a Value of the argument
        /// @return The negation of the argument
        template<typename T>
        constexpr auto operator()(T&& x) const -> decltype(Forward<T>(x)) {
            return -Forward<T>(x);
        } 
    };
    #endif

    // Equal to

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct EqualTo;
    #else
    template<typename T>
    struct EqualTo;
    #endif
    
    /// @brief Functor that represents equality comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/equal_to
    template<typename T>
    struct EqualTo : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are equal, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a == b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents equality comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/equal_to_void
    template<>
    struct EqualTo<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are equal, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) == Forward<U>(b)) {
            return Forward<T>(a) == Forward<U>(b);
        } 
    };
    #endif

    // Not equal to

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct NotEqualTo;
    #else
    template<typename T>
    struct NotEqualTo;
    #endif

    /// @brief Functor that represents inequality comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_equal_to
    template<typename T>
    struct NotEqualTo : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are not equal, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a != b;
        }
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents inequality comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_equal_to_void
    template<>
    struct NotEqualTo<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;
    
        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are not equal, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) != Forward<U>(b)) {
            return Forward<T>(a) != Forward<U>(b);
        } 
    };
    #endif

    // Greater

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Greater;
    #else
    template<typename T>
    struct Greater;
    #endif

    /// @brief Functor that represents greater-than comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater
    template<typename T>
    struct Greater : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a > b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents greater-than comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater_void
    template<>
    struct Greater<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) > Forward<U>(b)) {
            return Forward<T>(a) > Forward<U>(b);
        } 
    };
    #endif

    // Less

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Less;
    #else
    template<typename T>
    struct Less;
    #endif
    
    /// @brief Functor that represents less-than comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less
    template<typename T>
    struct Less : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a < b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents less-than comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less_void
    template<>
    struct Less<void> : public BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) < Forward<U>(b)) {
            return Forward<T>(a) < Forward<U>(b);
        } 
    };
    #endif

    // Greater equal

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct GreaterEqual;
    #else
    template<typename T>
    struct GreaterEqual;
    #endif

    /// @brief Functor that represents greater-than-or-equal-to comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater_equal
    template<typename T>
    struct GreaterEqual : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than or equal to the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a >= b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents greater-than-or-equal-to comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater_equal_void
    template<>
    struct GreaterEqual<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than or equal to the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) >= Forward<U>(b)) {
            return Forward<T>(a) >= Forward<U>(b);
        } 
    };
    #endif

    // Less equal

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LessEqual;
    #else
    template<typename T>
    struct LessEqual;
    #endif

    /// @brief Functor that represents less-than-or-equal-to comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less_equal
    template<typename T>
    struct LessEqual : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than or equal to the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a <= b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents less-than-or-equal-to comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less_equal_void
    template<>
    struct LessEqual<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than or equal to the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) <= Forward<U>(b)) {
            return Forward<T>(a) <= Forward<U>(b);
        } 
    };
    #endif

    // Logical and

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LogicalAnd;
    #else
    template<typename T>
    struct LogicalAnd;
    #endif

    /// @brief Functor that represents logical AND operation between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_and
    template<typename T>
    struct LogicalAnd : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if both arguments are true, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a && b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents logical AND operation between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_and_void
    template<>
    struct LogicalAnd<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if both arguments are true, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) && Forward<U>(b)) {
            return Forward<T>(a) && Forward<U>(b);
        } 
    };
    #endif

    // Logical or

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LogicalOr;
    #else
    template<typename T>
    struct LogicalOr;
    #endif

    /// @brief Functor that represents logical OR operation between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_or
    template<typename T>
    struct LogicalOr : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if at least one of the arguments is true, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a || b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents logical OR operation between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_or_void
    template<>
    struct LogicalOr<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if at least one of the arguments is true, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) || Forward<U>(b)) {
            return Forward<T>(a) || Forward<U>(b);
        } 
    };
    #endif

    // Logical not

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LogicalNot;
    #else
    template<typename T>
    struct LogicalNot;
    #endif

    /// @brief Functor that represents logical NOT operation for an object
    /// @tparam T Type of the object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_not
    template<typename T>
    struct LogicalNot : UnaryFunction<T, bool> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return True if the argument is false, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& x) const {
            return !x;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents logical NOT operation for an object 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_not_void
    template<>
    struct LogicalNot<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to an argument
        /// @param a Value of the argument
        /// @return True if the argument is false, false otherwise
        template<typename T>
        constexpr auto operator()(T&& x) const -> decltype(!Forward<T>(x)) {
            return !Forward<T>(x);
        } 
    };
    #endif

    // Bitwise and

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseAnd;
    #else
    template<typename T>
    struct BitwiseAnd;
    #endif

    /// @brief Functor that represents bitwise AND operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_and
    template<typename T>
    struct BitwiseAnd : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise AND of the arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a & b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise AND operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_and_void
    template<>
    struct BitwiseAnd<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise AND of the arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) & Forward<U>(b)) {
            return Forward<T>(a) & Forward<U>(b);
        } 
    };
    #endif

    // Bitwise or

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseOr;
    #else
    template<typename T>
    struct BitwiseOr;
    #endif

    /// @brief Functor that represents bitwise OR operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_or
    template<typename T>
    struct BitwiseOr : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise OR of the arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a | b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise OR operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_or_void
    template<>
    struct BitwiseOr<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise OR of the arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) | Forward<U>(b)) {
            return Forward<T>(a) | Forward<U>(b);
        } 
    };
    #endif

    // Bitwise xor

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseXor;
    #else
    template<typename T>
    struct BitwiseXor;
    #endif

    /// @brief Functor that represents bitwise XOR operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_xor
    template<typename T>
    struct BitwiseXor : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise XOR of the arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a ^ b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise XOR operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_xor_void
    template<>
    struct BitwiseXor<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise XOR of the arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) ^ Forward<U>(b)) {
            return Forward<T>(a) ^ Forward<U>(b);
        } 
    };
    #endif

    // Bitwise not

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseNot;
    #else
    template<typename T>
    struct BitwiseNot;
    #endif

    /// @brief Functor that represents bitwise NOT operation for an object
    /// @tparam T Type of the object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_not
    template<typename T>
    struct BitwiseNot : UnaryFunction<T, T> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return Bitwise NOT of the argument
        __WSTL_CONSTEXPR__ T operator()(const T& x) const {
            return ~x;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise NOT operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_not_void
    template<>
    struct BitwiseNot<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;
        
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return Bitwise NOT of the argument
        template<typename T, typename U>
        constexpr auto operator()(T&& x) const -> decltype(~Forward<T>(x)) {
            return ~Forward<T>(x);
        } 
    };
    #endif

    // Identity

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Identity;
    #else
    template<typename T>
    struct Identity;
    #endif

    /// @brief Functor that returns its input unchanged for non-void types
    /// @tparam T Type of the input and output
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/identity
    template<typename T>
    struct Identity : UnaryFunction<T, T> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The argument itself
        __WSTL_CONSTEXPR__ T& operator()(T& x) const __WSTL_NOEXCEPT__ {
            return x;
        }

        /// @brief Applies the functor to an argument
        /// @param x Const value of the argument
        /// @return The argument itself
        __WSTL_CONSTEXPR__ const T& operator()(const T& x) const __WSTL_NOEXCEPT__ {
            return x;
        }
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that returns its input unchanged for arbitrary types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/identity_void
    template<>
    struct Identity<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef int IsTransparent;
        
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The argument itself
        template<typename T>
        constexpr T&& operator()(T&& x) {
            return Forward<T>(x);
        } 
    };
    #endif

    // Not function

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename Function>
        class __NotFunction {
        public:
            explicit constexpr __NotFunction(Function&& function) : m_Function(Forward<Function>(function)) {}
            constexpr __NotFunction(const Function& function) : m_Function(function) {}
            
            constexpr __NotFunction(__NotFunction&& other) : m_Function(Move(other.m_Function)) {}
            constexpr __NotFunction(const __NotFunction& other) : m_Function(other.m_Function) {}

            template<typename... Args>
            constexpr ResultOfType<Function(Args...)> operator()(Args&&... args) const {
                return !m_Function(Forward<Args>(args)...);
            }

        private:
            Function m_Function;
        };
    }

    /// @brief Creates a functor that negates the result of a given callable
    /// @param function The callable object to wrap and negate
    /// @return A functor that negates the result of the given callable
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_fn
    template<typename Function>
    constexpr __private::__NotFunction<DecayType<Function>> NotFunction(Function&& function) {
        return __private::__NotFunction<DecayType<Function>>(Forward<Function>(function));
    }
    #else
    namespace __private {
        template<typename Function>
        class __NotFunction {
        public:
            explicit __NotFunction(Function function) : m_Function(function) {}
            explicit __NotFunction(const Function& function) : m_Function(function) {}
            explicit __NotFunction(const __NotFunction& other) : m_Function(other.m_Function) {}

            typename ResultOf<Function>::Type operator()() const {
                return !m_Function();
            }

            template<typename Arg>
            typename ResultOf<Function>::Type operator()(const Arg& arg) const {
                return !m_Function(arg);
            }

            template<typename Arg1, typename Arg2>
            typename ResultOf<Function>::Type operator()(const Arg1& arg1, const Arg2& arg2) const {
                return !m_Function(arg1, arg2);
            }

        private:
            Function m_Function;
        };
    }

    /// @brief Creates a functor that negates the result of a given callable
    /// @param function The callable object to wrap and negate
    /// @return A functor that negates the result of the given callable
    /// @note It supports maximum two arguments
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_fn
    template<typename Function>
    inline __private::__NotFunction<Function> NotFunction(Function function) {
        return __private::__NotFunction<Function>(function);
    }
    #endif

    #ifdef __WSTL_CXX11__
    // Is placeholder

    /// @brief Checks whether type is placeholder, stores int `IntegralConstant` 
    /// with the value of placeholder number
    /// @tparam T Type to check
    /// @ingroup functional
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/is_placeholder
    template<typename T>
    struct IsPlaceholder : IntegralConstant<int, 0> {};

    // Placeholders

    namespace __private {
        template<int N>
        struct __Placeholder : IntegralConstant<int, N> {};
    }

    /// @brief Namespace that contain placeholders (unbound arguments) for `wstl::Bind` function
    namespace placeholders {
        #ifdef __WSTL_CXX17__
            #define __WSTL_PLACEHOLDER__ inline constexpr
        #else
            #define __WSTL_PLACEHOLDER__ extern const
        #endif
        
        __WSTL_PLACEHOLDER__ __private::__Placeholder<1> _1;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<2> _2;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<3> _3;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<4> _4;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<5> _5;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<6> _6;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<7> _7;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<8> _8;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<9> _9;
    }

    template<int N>
    struct IsPlaceholder<__private::__Placeholder<N>> : IntegralConstant<int, N> {};

    template<int N>
    struct IsPlaceholder<const __private::__Placeholder<N>> : IntegralConstant<int, N> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsPlaceholder
    /// @since C++17
    template<typename T>
    inline constexpr int IsPlaceholderVariable = IsPlaceholder<T>::Value;
    #endif

    // Is bind expression

    /// @brief Checks whether type is bind expression
    /// @tparam T Type to check
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/is_bind_expression
    template<typename T>
    struct IsBindExpression : FalseType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsBindExpression
    /// @since C++17
    template<typename T>
    inline constexpr bool IsBindExpressionVariable = IsBindExpression<T>::Value;
    #endif

    // Bind 

    namespace __private {
        template<typename Arg, typename Tuple>
        constexpr auto __ReplacePlaceholder(Arg&& arg, Tuple&&) -> decltype(Forward<Arg>(arg)) {
            return Forward<Arg>(arg);
        }

        template<int N, typename Tuple>
        constexpr auto __ReplacePlaceholder(const __Placeholder<N>&, Tuple&& tuple) -> decltype(Get<N - 1>(Forward<Tuple>(tuple))) {
            return Get<N - 1>(Forward<Tuple>(tuple));
        }

        template<typename Function, typename... Args>
        class __Bind {
        private:
            Function m_Function;
            Tuple<Args...> m_Args;

            template<typename Arg, typename Tuple>
            using __ResultType = decltype(__ReplacePlaceholder(DeclareValue<Arg>(), DeclareValue<Tuple>()));

            template<typename... CallArgs>
            using ResultType = decltype(DeclareValue<Function>()(DeclareValue<__ResultType<Args, Tuple<CallArgs...>>>()...));

            template<typename... CallArgs, size_t... Indices>
            constexpr ResultType<CallArgs...> Invoke(IndexSequence<Indices...>, CallArgs&&... callArgs) {
                return m_Function(__ReplacePlaceholder(Get<Indices>(m_Args), ForwardAsTuple(callArgs...))...);
            }

        public:
            constexpr __Bind(Function&& function, Args&&... args) 
                : m_Function(Forward<Function>(function)), m_Args(Forward<Args>(args)...) {} 

            template<typename... CallArgs>
            constexpr ResultType<CallArgs...> operator()(CallArgs&&... callArgs) {
                return Invoke(IndexSequenceFor<Args...>{}, Forward<CallArgs>(callArgs)...);
            }
        };
    }

    // Is bind expression specialization

    template<typename Function, typename... Args>
    struct IsBindExpression<__private::__Bind<Function, Args...>> : TrueType {};

    /// @brief Binds a function with specified arguments, including placeholders if needed
    /// @param function The function to bind
    /// @param args The arguments to bind, which may include placeholders
    /// @return A bound function object with the provided arguments
    /// @ingroup functional
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind
    template<typename Function, typename... Args>
    constexpr __private::__Bind<Function, Args...> Bind(Function&& function, Args&&... args) {
        return __private::__Bind<Function, Args...>(Forward<Function>(function), Forward<Args>(args)...);
    }
    #endif

    // Binder first

    /// @brief Functor that binds the first argument of a callable object
    /// @tparam Function Type of the callable object
    /// @ingroup functional
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binder12
    template<typename Function>
    class BinderFirst : public UnaryFunction<typename Function::SecondArgumentType, 
    typename Function::ResultType> {
    public:
        /// @brief Constructor
        /// @param function Callable object to bind
        /// @param arg Value of the first argument to bind
        BinderFirst(const Function& function, const typename Function::FirstArgumentType& arg) :
            m_Function(function), m_Arg(arg) {}

        /// @brief Invokes the callable object
        /// @param arg Value of the second argument
        /// @return Result of the callable object
        typename Function::ResultType operator()(typename Function::SecondArgumentType& arg) const {
            return m_Function(m_Arg, arg);
        }
    
        /// @copydoc BinderFirst::operator()()
        typename Function::ResultType operator()(const typename Function::SecondArgumentType& arg) const {
            return m_Function(m_Arg, arg);
        }

    protected:
        Function m_Function;
        typename Function::FirstArgumentType m_Arg;
    };

    /// @brief Creates a `BinderFirst` object that binds the first argument of a callable object
    /// @param function The callable object to bind 
    /// @param arg Value of the first argument to bind
    /// @return A `BinderFirst` object that wraps the callable object with the first argument bound
    /// @ingroup functional
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind12
    template<typename Function, typename T>
    BinderFirst<Function> BindFirst(const Function& function, const T& arg) {
        return BinderFirst<Function>(function, typename Function::FirstArgumentType(arg));
    }

    // Binder second

    /// @brief Functor that binds the second argument of a callable object
    /// @tparam Function Type of the callable object
    /// @ingroup functional
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binder12
    template<typename Function>
    class BinderSecond : public UnaryFunction<typename Function::FirstArgumentType, 
    typename Function::ResultType> {
    public:
        /// @brief Constructor
        /// @param function Callable object to bind
        /// @param arg Value of the second argument to bind
        BinderSecond(const Function& function, const typename Function::SecondArgumentType& arg) :
            m_Function(function), m_Arg(arg) {}
        
        /// @brief Invokes the callable object
        /// @param arg Value of the first argument
        /// @return Result of the callable object
        typename Function::ResultType operator()(typename Function::FirstArgumentType& arg) const {
            return m_Function(m_Arg, arg);
        }

        /// @copydoc BinderSecond::operator()()
        typename Function::ResultType operator()(const typename Function::FirstArgumentType& arg) const {
            return m_Function(m_Arg, arg);
        }

    protected:
        Function m_Function;
        typename Function::SecondArgumentType m_Arg;
    };

    /// @brief Creates a `BinderSecond` object that binds the second argument of a callable object
    /// @param function The callable object to bind 
    /// @param arg Value of the second argument to bind
    /// @return A `BinderSecond` object that wraps the callable object with the second argument bound
    /// @ingroup functional
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind12
    template<typename Function, typename T>
    BinderSecond<Function> BindSecond(const Function& function, const T& arg) {
        return BinderSecond<Function>(function, typename Function::SecondArgumentType(arg));
    }

    // Pointer to unary function

    /// @brief Functor that wraps a pointer to unary function
    /// @tparam Arg Type of the argument
    /// @tparam Return Result type of the function
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/pointer_to_unary_function
    template<typename Arg, typename Return>
    class PointerToUnaryFunction : public UnaryFunction<Arg, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the function with one argument
        explicit PointerToUnaryFunction(Return(*function)(Arg)) : m_Function(function) {}

        /// @brief Calls wrapped function with one argument
        /// @param arg Value of the argument
        /// @return Result of the function
        Return operator()(Arg arg) const {
            return (*m_Function)(arg);
        }

    private:
        Return(*m_Function)(Arg);
    };

    // Pointer to binary function

    /// @brief Functor that wraps a pointer to binary function
    /// @tparam Arg1 Type of the first argument
    /// @tparam Arg2 Type of the second argument
    /// @tparam Return Result type of the function
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/pointer_to_binary_function
    template<typename Arg1, typename Arg2, typename Return>
    class PointerToBinaryFunction : public BinaryFunction<Arg1, Arg2, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the function with two arguments
        explicit PointerToBinaryFunction(Return(*function)(Arg1, Arg2)) : m_Function(function) {}

        /// @brief Calls wrapped function with two arguments
        /// @param arg1 Value of the first argument
        /// @param arg2 Value of the second argument
        /// @return Result of the function
        Return operator()(Arg1 arg1, Arg2 arg2) const {
            return (*m_Function)(arg1, arg2);
        }

    private:
        Return(*m_Function)(Arg1, Arg2);
    };

    // Pointer function

    /// @brief Converts a unary function pointer into a `PointerToUnaryFunction` object
    /// @param function Pointer to a unary function
    /// @return A `PointerToUnaryFunction` object wrapping the provided function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ptr_fun
    template<typename Arg, typename Return>
    PointerToUnaryFunction<Arg, Return> PointerFunction(Return(*function)(Arg)) {
        return PointerToUnaryFunction<Arg, Return>(function);
    }

    /// @brief Converts a binary function pointer into a `PointerToBinaryFunction` object
    /// @param function Pointer to a binary function
    /// @return A `PointerToBinaryFunction` object wrapping the provided function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ptr_fun
    template<typename Arg1, typename Arg2, typename Return>
    PointerToBinaryFunction<Arg1, Arg2, Return> PointerFunction(Return(*function)(Arg1, Arg2)) {
        return PointerToBinaryFunction<Arg1, Arg2, Return>(function);
    }

    // Member function (old)
    
    /// @brief Wraps a non-const member function pointer into a callable object
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_t
    template<typename Return, typename T>
    class MemberFunctionType : public UnaryFunction<T*, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the non-const member function to be wrapped
        explicit MemberFunctionType(Return(T::*function)()) : m_Function(function) {}

        /// @brief Calls wrapped function on the given object
        /// @param object Pointer to the object on which to call
        /// @return Result of the function
        Return operator()(T* object) const {
            return (object->*m_Function)();
        }

    private:
        Return(T::*m_Function)();
    };

    /// @brief Wraps a const member function pointer into a callable object
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_t
    template<typename Return, typename T>
    class ConstMemberFunctionType : public UnaryFunction<const T*, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the const member function to be wrapped
        explicit ConstMemberFunctionType(Return(T::*function)() const) : m_Function(function) {}

        /// @brief Calls wrapped function on the given object
        /// @param object Pointer to the object on which to call
        /// @return Result of the function
        Return operator()(const T* object) const {
            return (object->*m_Function)();
        }

    private:
        Return(T::*m_Function)() const;
    };

    /// @brief Creates a callable object from a non-const member function pointer
    /// @param function Pointer to the non-const member function
    /// @return A `MemberFunctionType` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun
    template<typename Return, typename T>
    MemberFunctionType<Return, T> MemberFunction(Return(T::*function)()) {
        return MemberFunctionType<Return, T>(function);
    }

    /// @brief Creates a callable object from a const member function pointer
    /// @param function Pointer to the const member function
    /// @return A `ConstMemberFunctionType` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun
    template<typename Return, typename T>
    ConstMemberFunctionType<Return, T> MemberFunction(Return(T::*function)() const) {
        return ConstMemberFunctionType<Return, T>(function);
    }

    // Member function (1 argument)

    /// @brief Wraps a non-const member function pointer with one argument into a callable object
    /// @tparam Arg Type of the argument
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_t
    template<typename Arg, typename Return, typename T>
    class MemberFunction1Type : public BinaryFunction<T*, Arg, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the non-const member function to be wrapped
        explicit MemberFunction1Type(Return(T::*function)(Arg)) : m_Function(function) {}

        /// @brief Calls wrapped function on the given object with one argument
        /// @param object Pointer to the object on which to call
        /// @param arg Argument to pass to the function
        /// @return Result of the function
        Return operator()(T* object, Arg arg) const {
            return (object->*m_Function)(arg);
        }

    protected:
        Return(T::*m_Function)(Arg);
    };

    /// @brief Wraps a const member function pointer with one argument into a callable object
    /// @tparam Arg Type of the argument
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_t
    template<typename Arg, typename Return, typename T>
    class ConstMemberFunction1Type : public BinaryFunction<const T*, Arg, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the const member function to be wrapped
        explicit ConstMemberFunction1Type(Return(T::*function)(Arg) const) : m_Function(function) {}

        /// @brief Calls wrapped function on the given object with one argument
        /// @param object Pointer to the object on which to call
        /// @param arg Argument to pass to the function
        /// @return Result of the function
        Return operator()(const T* object, Arg arg) const {
            return (object->*m_Function)(arg);
        }

    protected:
        Return(T::*m_Function)(Arg) const;
    };

    /// @brief Creates a callable object from a non-const member function pointer with one argument
    /// @param function Pointer to the non-const member function
    /// @return A `MemberFunction1Type` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun
    template<typename Arg, typename Return, typename T>
    MemberFunction1Type<Arg, Return, T> MemberFunction(Return(T::*function)(Arg)) {
        return MemberFunction1Type<Arg, Return, T>(function);
    }

    /// @brief Creates a callable object from a const member function pointer with one argument
    /// @param function Pointer to the const member function
    /// @return A `ConstMemberFunction1Type` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun
    template<typename Arg, typename Return, typename T>
    ConstMemberFunction1Type<Arg, Return, T> MemberFunction(Return(T::*function)(Arg) const) {
        return ConstMemberFunction1Type<Arg, Return, T>(function);
    }

    // Member function reference (old)

    /// @brief Wraps a non-const member function, 
    /// operating on an object reference, into a callable object
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function  
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref_t
    template<typename Return, typename T>
    class MemberFunctionReferenceType : public UnaryFunction<T, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the non-const member function to be wrapped
        explicit MemberFunctionReferenceType(Return(T::*function)()) : m_Function(function) {}

        /// @brief Calls wrapped function on the given object
        /// @param object Reference to the object on which to call
        /// @return Result of the function
        Return operator()(T& object) const {
            return (object.*m_Function)();
        }

    protected:
        Return(T::*m_Function)();
    };

    /// @brief Wraps a const member function, 
    /// operating on an object reference, into a callable object
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function  
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref_t
    template<typename Return, typename T>
    class ConstMemberFunctionReferenceType : public UnaryFunction<T, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the const member function to be wrapped
        explicit ConstMemberFunctionReferenceType(Return(T::*function)() const) : m_Function(function) {}

        /// @brief Calls wrapped function on the given object
        /// @param object Reference to the object on which to call
        /// @return Result of the function
        Return operator()(const T& object) const {
            return (object.*m_Function)();
        }

    protected:
        Return(T::*m_Function)() const;
    };

    /// @brief Creates a callable object from a non-const member function,
    /// operating on an object reference
    /// @param function Pointer to the non-const member function
    /// @return A `MemberFunctionReferenceType` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref
    template<typename Return, typename T>
    MemberFunctionReferenceType<Return, T> MemberFunctionReference(Return(T::*function)()) {
        return MemberFunctionReferenceType<Return, T>(function);
    }

    /// @brief Creates a callable object from a const member function,
    /// operating on an object reference
    /// @param function Pointer to the const member function
    /// @return A `ConstMemberFunctionReferenceType` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref
    template<typename Return, typename T>
    ConstMemberFunctionReferenceType<Return, T> MemberFunctionReference(Return(T::*function)() const) {
        return ConstMemberFunctionReferenceType<Return, T>(function);
    }

    // Member function reference (1 argument)

    /// @brief Wraps a non-const member function with one argument, 
    /// operating on an object reference, into a callable object
    /// @tparam Arg Type of the argument
    /// @tparam Return Return type of the member function
    /// @tparam T Type of the class containing the member function  
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref_t
    template<typename Arg, typename Return, typename T>
    class MemberFunctionReference1Type : public BinaryFunction<T, Arg, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the non-const member function to be wrapped
        explicit MemberFunctionReference1Type(Return(T::*function)(Arg)) : m_Function(function) {}

        /// @brief Calls wrapped function with one argument on the given object 
        /// @param object Reference to the object on which to call
        /// @param arg Argument to pass to the function
        /// @return Result of the function
        Return operator()(T& object, Arg arg) const {
            return (object.*m_Function)(arg);
        }

    protected:
        Return(T::*m_Function)(Arg);
    };

    template<typename Arg, typename Return, typename T>
    class ConstMemberFunctionReference1Type : public BinaryFunction<T, Arg, Return> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the const member function to be wrapped
        explicit ConstMemberFunctionReference1Type(Return(T::*function)(Arg) const) : m_Function(function) {}

        /// @brief Calls wrapped function with one argument on the given object 
        /// @param object Reference to the object on which to call
        /// @param arg Argument to pass to the function
        /// @return Result of the function
        Return operator()(const T& object, Arg arg) const {
            return (object.*m_Function)(arg);
        }

    protected:
        Return(T::*m_Function)(Arg) const;
    };

    /// @brief Creates a callable object from a non-const member function with one argument,
    /// operating on an object reference
    /// @param function Pointer to the non-const member function
    /// @return A `MemberFunctionReference1Type` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref
    template<typename Arg, typename Return, typename T>
    MemberFunctionReference1Type<Arg, Return, T> MemberFunctionReference(Return(T::*function)(Arg)) {
        return MemberFunctionReference1Type<Arg, Return, T>(function);
    }

    /// @brief Creates a callable object from a const member function with one argument,
    /// operating on an object reference
    /// @param function Pointer to the const member function
    /// @return A `ConstMemberFunctionReference1Type` object wrapping the member function pointer
    /// @ingroup functional
    /// @deprecated Use `wstl::Function` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fun_ref
    template<typename Arg, typename Return, typename T>
    ConstMemberFunctionReference1Type<Arg, Return, T> MemberFunctionReference(Return(T::*function)(Arg) const) {
        return ConstMemberFunctionReference1Type<Arg, Return, T>(function);
    }

    // Unary negate (old)

    /// @brief Functor that negates the result of a unary predicate
    /// @tparam Predicate The type of the predicate to be negated
    /// @ingroup functional
    /// @deprecated Use `wstl::NotFunction` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unary_negate
    template<typename Predicate>
    class UnaryNegate : public UnaryFunction<typename Predicate::ArgumentType, bool> {
    public: 
        /// @brief Constructor
        /// @param predicate The predicate to be negated
        explicit UnaryNegate(const Predicate& predicate) : m_Predicate(predicate) {}

        /// @brief Calls the predicate with the one argument and negates its result
        /// @param argument The argument to pass to the predicate
        /// @return The negated result of the predicate
        bool operator()(const typename Predicate::ArgumentType& argument) const {
            return !m_Predicate(argument);
        }

    protected:
        Predicate m_Predicate;
    };

    /// @brief Creates a `UnaryNegate` object for a given predicate
    /// @param predicate The predicate whose result will be negated
    /// @return A `UnaryNegate` object that negates the result of the predicate
    /// @ingroup functional
    /// @deprecated Use `wstl::NotFunction` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not1
    template<typename Predicate>
    UnaryNegate<Predicate> Not1(const Predicate& predicate) {
        return UnaryNegate<Predicate>(predicate);
    }

    // Binary negate (old)

    /// @brief Functor that negates the result of a binary predicate
    /// @tparam Predicate The type of the predicate to be negated
    /// @ingroup functional
    /// @deprecated Use `wstl::NotFunction` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binary_negate
    template<typename Predicate>
    class BinaryNegate : public BinaryFunction<typename Predicate::FirstArgumentType, typename Predicate::SecondArgumentType, bool> {
    public:
        /// @brief Constructor
        /// @param predicate The predicate to be negated
        explicit BinaryNegate(const Predicate& predicate) : m_Predicate(predicate) {}

        /// @brief Calls the predicate with the two arguments and negates its result
        /// @param arg1 The first argument to pass to the predicate
        /// @param arg2 The second argument to pass to the predicate
        /// @return The negated result of the predicate
        bool operator()(const typename Predicate::FirstArgumentType& arg1, 
        const typename Predicate::SecondArgumentType& arg2) const {
            return !m_Predicate(arg1, arg2);
        }

    protected:
        Predicate m_Predicate;
    };

    /// @brief Creates a `BinaryNegate` object for a given predicate
    /// @param predicate The predicate whose result will be negated
    /// @return A `BinaryNegate` object that negates the result of the predicate
    /// @ingroup functional
    /// @deprecated Use `wstl::NotFunction` instead
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not2
    template<typename Predicate>
    BinaryNegate<Predicate> Not2(const Predicate& predicate) {
        return BinaryNegate<Predicate>(predicate);
    }
}

#endif