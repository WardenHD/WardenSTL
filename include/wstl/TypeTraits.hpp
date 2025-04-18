// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_TYPETRAITS_HPP__
#define __WSTL_TYPETRAITS_HPP__

#include <stddef.h>
#include "NullPointer.hpp"
#include "private/Platform.hpp"


/// @defgroup type_traits Type Traits
/// @brief A set of type trait definitions
/// @ingroup utilities

namespace wstl {
    #ifdef __WSTL_CXX11__
    template<typename...>
    using void_t = void;

    /// @brief Type alias of `void_t` for consistent naming
    template<typename...>
    using VoidType = void;
    #endif

    // Integral constant
    
    /// @brief Wrapper for static constant
    /// @tparam T Type of value
    /// @tparam V Value to define
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/integral_constant
    template <typename T, T V>
    class IntegralConstant {
    public:
        static const T Value = V;

        typedef T ValueType;
        typedef IntegralConstant<T, V> Type; 

        __WSTL_CONSTEXPR__ operator ValueType() const __WSTL_NOEXCEPT__ {
            return Value;
        }

        __WSTL_CONSTEXPR__ ValueType operator()() const __WSTL_NOEXCEPT__ {
            return Value;
        }
    };

    template<typename T, T V>
    const T IntegralConstant<T, V>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc IntegralConstant
    /// @since C++17
    template<typename T, T V>
    inline constexpr T IntegralConstantVariable = IntegralConstant<T, V>::Value;
    #endif

    // Bool constant
    
    #ifdef __WSTL_CXX11__
    /// @brief Helper alias template for integral constant with boolean type
    /// @tparam V Boolean value
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/integral_constant
    template<bool V>
    using BoolConstant = IntegralConstant<bool, V>;
    #else
    /// @brief Helper alias template for integral constant with boolean type
    /// @tparam V Boolean value
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/integral_constant
    template<bool V>
    class BoolConstant : public IntegralConstant<bool, V> {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc BoolConstant
    /// @since C++17
    template<bool V>
    inline constexpr bool BoolConstantVariable = BoolConstant<V>::Value;
    #endif

    // Integral constant definitions

    /// @brief Static constant for false
    typedef BoolConstant<false> FalseType;
    /// @brief Static constant for true
    typedef BoolConstant<true> TrueType;

    // Remove reference
    
    /// @brief Removes reference from specified type
    /// @tparam T Type to remove reference from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_reference
    template<typename T>
    class RemoveReference;

    template<typename T>
    class RemoveReference { public: typedef T Type; };

    template<typename T>
    class RemoveReference<T&> { public: typedef T Type; };

    #ifdef __WSTL_CXX11__
    template<typename T>
    class RemoveReference<T&&> { public: typedef T Type; };
    #endif

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveReference
    /// @since C++11
    template<typename T>
    using RemoveReferenceType = typename RemoveReference<T>::Type;
    #endif

    // Remove pointer

    /// @brief Removes pointer from specified type
    /// @tparam T Type to remove pointer from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_pointer
    template<typename T>
    class RemovePointer;

    template<typename T>
    class RemovePointer { public: typedef T Type; };

    template<typename T>
    class RemovePointer<T*> { public: typedef T Type; };

    template<typename T>
    class RemovePointer<T* const> { public: typedef T Type; };

    template<typename T>
    class RemovePointer<T* volatile> { public: typedef T Type; };

    template<typename T>
    class RemovePointer<T* const volatile> { public: typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemovePointer
    /// @since C++11
    template<typename T>
    using RemovePointerType = typename RemovePointer<T>::Type;
    #endif

    // Remove const

    /// @brief Removes constant from specified type
    /// @tparam T Type to remove constant from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cv
    template<typename T>
    class RemoveConst;

    template<typename T>
    class RemoveConst { public: typedef T Type; };

    template<typename T>
    class RemoveConst<const T> { public: typedef T Type; };

    template<typename T>
    class RemoveConst<volatile T> { public: typedef volatile T Type; };

    template<typename T>
    class RemoveConst<const volatile T> { public: typedef volatile T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveConst
    /// @since C++11
    template<typename T>
    using RemoveConstType = typename RemoveConst<T>::Type;
    #endif

    // Remove volatile

    /// @brief Removes volatile specifier from specified type
    /// @tparam T Type to remove volatile from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cv
    template<typename T>
    class RemoveVolatile;

    template<typename T>
    class RemoveVolatile { public: typedef T Type; };

    template<typename T>
    class RemoveVolatile<volatile T> { public: typedef T Type; };

    template<typename T>
    class RemoveVolatile<const T> { public: typedef const T Type; };

    template<typename T>
    class RemoveVolatile<const volatile T> { public: typedef const T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveVolatile
    /// @since C++11
    template<typename T>
    using RemoveVolatileType = typename RemoveVolatile<T>::Type;
    #endif

    // Remove const and volatile

    /// @brief Removes const and volatile specifiers from specified type
    /// @tparam T Type to remove const and volatile from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cv
    template<typename T>
    class RemoveCV;

    template<typename T>
    class RemoveCV { public: typedef T Type; };

    template<typename T>
    class RemoveCV<const T> { public: typedef T Type; };

    template<typename T>
    class RemoveCV<volatile T> { public: typedef T Type; };

    template<typename T>
    class RemoveCV<const volatile T> { public: typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveCV
    /// @since C++11
    template<typename T>
    using RemoveCVType = typename RemoveCV<T>::Type;
    #endif

    // Remove extent

    /// @brief Removes one dimension of array
    /// @tparam T Array type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_extent
    template<typename T>
    class RemoveExtent;

    template<typename T>
    class RemoveExtent { public: typedef T Type; };

    template<typename T>
    class RemoveExtent<T[]> { public: typedef T Type; };

    template<typename T, size_t S>
    class RemoveExtent<T[S]> { public: typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveExtent
    /// @since C++11
    template<typename T>
    using RemoveExtentType = typename RemoveExtent<T>::Type;
    #endif

    // Remove all extents

    /// @brief Removes all dimensions in array
    /// @tparam T Array type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_all_extents
    template<typename T>
    class RemoveAllExtents;

    template<typename T>
    class RemoveAllExtents { public: typedef T Type; };

    template<typename T>
    class RemoveAllExtents<T[]> { public: typedef typename RemoveAllExtents<T>::Type Type; };

    template<typename T, size_t S>
    class RemoveAllExtents<T[S]> { public: typedef typename RemoveAllExtents<T>::Type Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveAllExtents
    /// @since C++11
    template<typename T>
    using RemoveAllExtentsType = typename RemoveAllExtents<T>::Type;
    #endif

    // Add pointer

    /// @brief Converts type into pointer
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_pointer
    template<typename T>
    class AddPointer;

    template<typename T>
    class AddPointer { public: typedef typename RemoveReference<T>::Type* Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddPointer
    /// @since C++11
    template<typename T>
    using AddPointerType = typename AddPointer<T>::Type;
    #endif

    // Add lvalue reference

    /// @brief Creates lvalue reference of specified type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_reference
    template<typename T>
    class AddLValueReference;

    template<typename T>
    class AddLValueReference { public: typedef T& Type; };

    template<typename T>
    class AddLValueReference<T&> { public: typedef T& Type; };

    template<>
    class AddLValueReference<void> { public: typedef void Type; };

    template<>
    class AddLValueReference<const void> { public: typedef const void Type; };

    template<>
    class AddLValueReference<volatile void> { public: typedef volatile void Type; };

    template<>
    class AddLValueReference<const volatile void> { 
    public: 
        typedef const volatile void Type; 
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddLValueReference
    /// @since C++11
    template<typename T>
    using AddLValueReferenceType = typename AddLValueReference<T>::Type;
    #endif

    // Add rvalue reference

    #ifdef __WSTL_CXX11__
    /// @brief Creates rvalue reference of specified type
    /// @tparam T Type to convert
    /// @since C++11
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_reference
    template<typename T>
    class AddRValueReference;

    template<typename T>
    class AddRValueReference { public: using Type = T&&; };

    template<typename T>
    class AddRValueReference<T&> { public: using Type = T&; };

    template<>
    class AddRValueReference<void> { public: using Type = void; };

    template<>
    class AddRValueReference<const void> { public: using Type = const void; };

    template<>
    class AddRValueReference<volatile void> { public: using Type = volatile void; };

    template<>
    class AddRValueReference<const volatile void> {
    public:
        using Type = const volatile void;
    };
    
    /// @copydoc AddRValueReference
    /// @since C++11
    template<typename T>
    using AddRValueReferenceType = typename AddRValueReference<T>::Type;
    #endif

    // Add const

    /// @brief Adds const specifier to type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_cv
    template<typename T>
    class AddConst;

    template<typename T>
    class AddConst { public: typedef const T Type; };  

    template<typename T>
    class AddConst<const T> { public: typedef const T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddConst
    /// @since C++11
    template<typename T>
    using AddConstType = typename AddConst<T>::Type;
    #endif

    // Add volatile

    /// @brief Adds volatile specifier to type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_cv
    template<typename T>
    class AddVolatile;

    template<typename T>
    class AddVolatile { public: typedef volatile T Type; };

    template<typename T>
    class AddVolatile<volatile T> { public: typedef volatile T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddVolatile
    /// @since C++11
    template<typename T>
    using AddVolatileType = typename AddVolatile<T>::Type;
    #endif

    // Add const volatile

    /// @brief Adds const and volatile specifiers to type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_cv
    template<typename T>
    class AddCV;

    template<typename T>
    class AddCV {
    public:
        typedef typename AddVolatile<typename AddConst<T>::Type>::Type Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddCV
    /// @since C++11
    template<typename T>
    using AddCVType = typename AddCV<T>::Type;
    #endif

    // Remove const volatile reference

    /// @brief Removes const volatile specifiers and reference from type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cvref
    template<typename T>
    class RemoveCVReference {
    public:
        typedef typename RemoveCV<typename RemoveReference<T>::Type>::Type Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveCVReference
    /// @since C++11
    template<typename T>
    using RemoveCVReferenceType = typename RemoveCVReference<T>::Type;
    #endif

    // Alignment of

    /// @brief Gets alignment requirements of type
    /// @tparam T Type to get alignment of
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/alignment_of
    template<typename T>
    class AlignmentOf;

    #ifdef __WSTL_CXX11__
    template<typename T>
    class AlignmentOf : public IntegralConstant<size_t, alignof(T)> {};
    #elif defined(__WSTL_MSVC__)
    template<typename T>
    class AlignmentOf : public IntegralConstant<size_t, size_t(__alingof(T))> {};
    #else
    template<typename T>
    class AlignmentOf : public IntegralConstant<size_t, size_t(__alingof__(T))> {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc AlignmentOf
    /// @since C++17
    template<typename T>
    inline constexpr size_t AlignmentOfVariable = AlignmentOf<T>::Value;
    #endif
    
    // Rank

    /// @brief Provides rank (depth) of an array type
    /// @tparam T Type to get rank of
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/rank
    template<typename T>
    class Rank;

    template<typename T>
    class Rank : public IntegralConstant<size_t, 0> {};

    template<typename T>
    class Rank<T[]> : public IntegralConstant<size_t, Rank<T>::Value + 1> {};

    template<typename T, size_t S>
    class Rank<T[S]> : public IntegralConstant<size_t, Rank<T>::Value + 1> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Rank
    /// @since C++17
    template<typename T>
    inline constexpr size_t RankVariable = Rank<T>::Value;
    #endif

    // Extent

    /// @brief Gets size of dimension of array
    /// @tparam T Array type
    /// @tparam N Array dimemsion
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/extent
    template<typename T, unsigned N = 0>
    class Extent;

    template<typename T>
    class Extent<T[], 0> : public IntegralConstant<size_t, 0> {};

    template<typename T, unsigned N>
    class Extent<T[], N> : public IntegralConstant<size_t, Extent<T, N - 1>::Value> {};

    template<typename T, unsigned N>
    class Extent<T[N], 0> : public IntegralConstant<size_t, N> {};

    template<typename T, unsigned I, unsigned N>
    class Extent<T[I], N> : public IntegralConstant<size_t, Extent<T, N - 1>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Extent
    /// @since C++17
    template<typename T, unsigned N = 0>
    inline constexpr size_t ExtentVariable = Extent<T, N>::Value;
    #endif

    // Declare value

    #ifdef __WSTL_CXX11__
    template<typename T>
    /// @brief Declares a value of specified type
    /// @tparam T Type of value
    /// @ingroup type_traits
    /// @since C++11
    typename AddRValueReference<T>::Type DeclareValue() __WSTL_NOEXCEPT__;
    #endif

    // Enable if
    
    /// @brief Defines a type if condition is satisfied
    /// @tparam T Type to define
    /// @tparam B Condition to satisfy
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/enable_if
    template<bool B, typename T>
    class EnableIf;

    template<bool B, typename T = void>
    class EnableIf {};

    template<typename T>
    class EnableIf<true, T> { 
    public:
        typedef T Type; 
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc EnableIf
    /// @since C++11
    template<bool B, typename T = void>
    using EnableIfType = typename EnableIf<B, T>::Type;
    #endif

    // Conditional

    /// @brief Defines type based on condition
    /// @tparam T Defined type if condition is true
    /// @tparam F Defined type if condition is false
    /// @tparam B Boolean condition
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/conditional
    template<bool B, typename T, typename F>
    class Conditional;

    template<bool B, typename T, typename F>
    class Conditional {
    public:
        typedef T Type;
    };

    template<typename T, typename F>
    class Conditional<false, T, F> {
    public:
        typedef F Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc Conditional
    /// @since C++11
    template<bool B, typename T, typename F>
    using ConditionalType = typename Conditional<B, T, F>::Type;
    #endif

    // Result of

    /// @brief Deduces the result/return type of callable object
    /// @tparam Callable Type of callable object
    /// @ingroup type_traits
    /// @note In C++98 it doesn't support functors without `ResultType`, 
    /// and maximum argument count is 2
    /// @see https://en.cppreference.com/w/cpp/types/result_of
    template<typename Callable>
    class ResultOf;

    #ifdef __WSTL_CXX11__
    template<typename Callable, typename... Args>
    class ResultOf<Callable(Args...)> {
    public:
        using Type = decltype(DeclareValue<Callable>()(DeclareValue<Args>()...));
    };

    #else
    template<typename Return, typename Arg1, typename Arg2>
    class ResultOf<Return(*)(Arg1, Arg2)> {
    public:
        typedef Return Type;
    };

    template<typename Return, typename Arg1, typename Arg2>
    class ResultOf<Return(&)(Arg1, Arg2)> {
    public:
        typedef Return Type;
    };

    template<typename Object, typename Return, typename Arg1, typename Arg2>
    class ResultOf<Return(Object::*)(Arg1, Arg2)> {
    public:
        typedef Return Type;
    };

    template<typename Object, typename Return, typename Arg1, typename Arg2>
    class ResultOf<Return(Object::*)(Arg1, Arg2) const> {
    public:
        typedef Return Type;
    };

    template<typename Return, typename Arg>
    class ResultOf<Return(*)(Arg)> {
    public:
        typedef Return Type;
    };

    template<typename Return, typename Arg>
    class ResultOf<Return(&)(Arg)> {
    public:
        typedef Return Type;
    };

    template<typename Object, typename Return, typename Arg>
    class ResultOf<Return(Object::*)(Arg)> {
    public:
        typedef Return Type;
    };

    template<typename Object, typename Return, typename Arg>
    class ResultOf<Return(Object::*)(Arg) const> {
    public:
        typedef Return Type;
    };

    template<typename Return>
    class ResultOf<Return(*)()> {
    public:
        typedef Return Type;
    };

    template<typename Return>
    class ResultOf<Return(&)()> {
    public:
        typedef Return Type;
    };

    template<typename Object, typename Return>
    class ResultOf<Return(Object::*)()> {
    public:
        typedef Return Type;
    };

    template<typename Object, typename Return>
    class ResultOf<Return(Object::*)() const> {
    public:
        typedef Return Type;
    };

    #endif

    // Specialization for functors

    template<typename Functor>
    class ResultOf {
    public:
        typedef typename Functor::ResultType Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc ResultOf
    /// @since C++11
    template<typename Callable>
    using ResultOfType = typename ResultOf<Callable>::Type;
    #endif

    // Conjunction

    /// @brief Performs logical AND on the sequence of traits
    /// @tparam T Sequence of traits
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/conjunction
    template<typename... T>
    class Conjunction;

    template<typename...>
    class Conjunction : public FalseType {};

    template<typename T>
    class Conjunction<T> : public T {};

    template<typename T1, typename... Tn>
    class Conjunction<T1, Tn...> : public Conditional<bool(T1::Value), Conjunction<Tn...>, T1>::Type {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Conjunction
    /// @since C++17
    template<typename... T>
    inline constexpr bool ConjunctionVariable = Conjunction<T...>::Value;
    #endif

    // Disjunction

    /// @brief Performs logical OR on the sequence of traits
    /// @tparam T Sequence of traits
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/disjunction
    template<typename... T>
    class Disjunction;

    template<typename...>
    class Disjunction : public FalseType {};

    template<typename T>
    class Disjunction<T> : public T {};

    template<typename T1, typename... Tn>
    class Disjunction<T1, Tn...> : public Conditional<bool(T1::Value), T1, Disjunction<Tn...>>::Type {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Disjunction
    /// @since C++17
    template<typename... T>
    inline constexpr bool DisjunctionVariable = Disjunction<T...>::Value;
    #endif

    // Negation

    /// @brief Performs logical NOT on the specified type
    /// @tparam T Type to negate
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/negation
    template<typename T>
    class Negation;

    template<typename T>
    class Negation : public BoolConstant<!bool(T::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Negation
    /// @since C++17
    template<typename T>
    inline constexpr bool NegationVariable = Negation<T>::Value;
    #endif

    // Type identity

    /// @brief Provides the member typedef type
    /// @tparam T Type to provide
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/type_identity
    template<typename T>
    class TypeIdentity;

    template<typename T>
    class TypeIdentity { public: typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc TypeIdentity
    /// @since C++11
    template<typename T>
    using TypeIdentityType = TypeIdentity<T>;
    #endif

    // Is const

    /// @brief Checks whether type is constant
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_const
    template<typename T>
    class IsConst;

    template<typename T>
    class IsConst : public FalseType {};

    template<typename T>
    class IsConst<const T> : public TrueType {};

    template<typename T>
    class IsConst<const volatile T> : public TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsConst
    /// @since C++17
    template<typename T>
    inline constexpr bool IsConstVariable = IsConst<T>::Value;
    #endif

    // Is volatile

    /// @brief Checks whether type is volatile
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_volatile
    template<typename T>
    class IsVolatile;

    template<typename T>
    class IsVolatile : public FalseType {};

    template<typename T>
    class IsVolatile<volatile T> : public TrueType {};

    template<typename T>
    class IsVolatile<const volatile T> : public TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsVolatile
    /// @since C++17
    template<typename T>
    inline constexpr bool IsVolatileVariable = IsVolatile<T>::Value;
    #endif

    // Is same

    /// @brief Checks whether two types are same
    /// @tparam T1 First type to check
    /// @tparam T2 Second type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_same
    template<typename T1, typename T2>
    class IsSame;

    template<typename T1, typename T2>
    class IsSame : public FalseType {};

    template<typename T>
    class IsSame<T, T> : public TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsSame
    /// @since C++17
    template<typename T1, typename T2>
    inline constexpr bool IsSameVariable = IsSame<T1, T2>::Value;
    #endif

    // Is null pointer

    /// @brief Checks whether pointer is null
    /// @tparam T Type of pointer to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_null_pointer
    template<typename T>
    class IsNullPointer : public IsSame<NullPointerType, typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNullPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNullPointerVariable = IsNullPointer<T>::Value;
    #endif

    // Is void

    /// @brief Checks whether type is void
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_void
    template<typename T>
    class IsVoid;

    template<typename T>
    class IsVoid : public FalseType {};

    template<> 
    class IsVoid<void> : public TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsVoid
    /// @since C++17
    template<typename T>
    inline constexpr bool IsVoidVariable = IsVoid<T>::Value;
    #endif

    // Is integral

    /// @brief Checks whether type is integral
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_integral
    template<typename T>
    class IsIntegral;
    
    template<typename T>
    class IsIntegral : public FalseType {};

    template<> class IsIntegral<char> : public TrueType {};
    template<> class IsIntegral<signed char> : public TrueType {};
    template<> class IsIntegral<bool> : public TrueType {};
    template<> class IsIntegral<short> : public TrueType {};
    template<> class IsIntegral<int> : public TrueType {};
    template<> class IsIntegral<long> : public TrueType {};
    template<> class IsIntegral<long long> : public TrueType {};
    template<> class IsIntegral<wchar_t> : public TrueType {};

    template<> class IsIntegral<unsigned char> : public TrueType {};
    template<> class IsIntegral<unsigned short> : public TrueType {};
    template<> class IsIntegral<unsigned int> : public TrueType {};
    template<> class IsIntegral<unsigned long> : public TrueType {};
    template<> class IsIntegral<unsigned long long> : public TrueType {};

    template<typename T> class IsIntegral<const T> : public IsIntegral<T> {};
    template<typename T> class IsIntegral<volatile T> : public IsIntegral<T> {};
    template<typename T> class IsIntegral<const volatile T> : public IsIntegral<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsIntegral
    /// @since C++17
    template<typename T>
    inline constexpr bool IsIntegralVariable = IsIntegral<T>::Value;
    #endif

    // Is floating point

    /// @brief Checks whether type is floating point (float, double)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_floating_point
    template<typename T>
    class IsFloatingPoint;

    template<typename T>
    class IsFloatingPoint : public FalseType {};

    template<> class IsFloatingPoint<float> : public TrueType {};
    template<> class IsFloatingPoint<double> : public TrueType {};
    template<> class IsFloatingPoint<long double> : public TrueType {};

    template<typename T> class IsFloatingPoint<const T> : public IsFloatingPoint<T> {};
    template<typename T> class IsFloatingPoint<volatile T> : public IsFloatingPoint<T> {};
    template<typename T> class IsFloatingPoint<const volatile T> : public IsFloatingPoint<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsFloatingPoint
    /// @since C++17
    template<typename T>
    inline constexpr bool IsFloatingPointVariable = IsFloatingPoint<T>::Value;
    #endif

    // Is arithmetic

    /// @brief Checks whether type if arithmetic (int, float, etc.)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_arithmetic
    template<typename T>
    class IsArithmetic : public BoolConstant<(IsIntegral<T>::Value || IsFloatingPoint<T>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsArithmetic
    /// @since C++17
    template<typename T>
    inline constexpr bool IsArithmeticVariable = IsArithmetic<T>::Value;
    #endif

    // Is fundamental

    /// @brief Checks whether type is fundamental (arithmetic, void, nullptr_t)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_fundamental
    template<typename T>
    class IsFundamental;

    template<typename T>
    class IsFundamental : public BoolConstant<(IsArithmetic<T>::Value || IsVoid<T>::Value ||
        IsSame<NullPointerType, typename RemoveCV<T>::Type>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsFundamental
    /// @since C++17
    template<typename T>
    inline constexpr bool IsFundamentalVariable = IsFundamental<T>::Value;
    #endif
    
    // Is compound

    /// @brief Checks whether type is compound
    /// (array, function, object pointer, function pointer, member object pointer, 
    /// member function pointer, reference, class, union, or enumeration, 
    /// including any cv-qualified variants)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_compound
    template<typename T>
    class IsCompound;

    template<typename T>
    class IsCompound : public BoolConstant<(!IsFundamental<T>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsCompound
    /// @since C++17
    template<typename T>
    inline constexpr bool IsCompoundVariable = IsCompound<T>::Value;
    #endif

    // Is function

    /// @brief Checks whether type is function
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_function 
    template<typename T>
    class IsFunction;

    template<typename T>
    class IsFunction : public FalseType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...)> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...)> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) volatile> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const volatile> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) volatile> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const volatile> : public TrueType {};

    #ifdef __WSTL_CXX11__
    template<typename Return, typename... Args> 
    class IsFunction<Return(Args...)&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) volatile&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const volatile&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...)&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) volatile&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const volatile&> : public TrueType {};

    template<typename Return, typename... Args> 
    class IsFunction<Return(Args...)&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) volatile&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const volatile&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...)&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) volatile&&> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const volatile&&> : public TrueType {};
    
    #ifdef __WSTL_EXCEPTIONS__
    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) volatile noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const volatile noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) volatile noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const volatile noexcept> : public TrueType {};

    template<typename Return, typename... Args> 
    class IsFunction<Return(Args...)& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) volatile& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const volatile& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...)& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) volatile& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const volatile& noexcept> : public TrueType {};

    template<typename Return, typename... Args> 
    class IsFunction<Return(Args...)&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) volatile&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args...) const volatile&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...)&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) volatile&& noexcept> : public TrueType {};

    template<typename Return, typename... Args>
    class IsFunction<Return(Args..., ...) const volatile&& noexcept> : public TrueType {};
    #endif
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsFunction
    /// @since C++17 
    template<typename T>
    inline constexpr bool IsFunctionVariable = IsFunction<T>::Value;
    #endif

    // Is array

    /// @brief Checks whether type is an array
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_array
    template<typename T>
    class IsArray;

    template<typename T>
    class IsArray : public FalseType {};

    template<typename T>
    class IsArray<T[]> : public TrueType {};
    
    template<typename T, size_t S>
    class IsArray<T[S]> : public TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsArray
    /// @since C++17
    template<typename T>
    inline constexpr bool IsArrayVariable = IsArray<T>::Value;
    #endif

    // Is union

    /// @brief Checks whether type is union
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_union
    template<typename T>
    class IsUnion;

    template<typename T>
    class IsUnion : public BoolConstant<
    #if defined(__WSTL_GCC__) || defined(__WSTL_MSVC__) || defined(__WSTL_ICC__)
    __is_union(T)
    #else
    false
    #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsUnion
    /// @since C++17
    template<typename T>
    inline constexpr bool IsUnionVariable = IsUnion<T>::Value;
    #endif

    // Is member pointer

    namespace __private {
        template<typename T>
        class __IsMemberPointer : public FalseType {};

        template<typename T, typename U>
        class __IsMemberPointer<T U::*> : public TrueType {};
    }

    /// @brief Checks whether type is pointer to non-static member object or function
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_member_pointer
    template<typename T>
    class IsMemberPointer : public __private::__IsMemberPointer<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMemberPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMemberPointerVariable = IsMemberPointer<T>::Value;
    #endif

    // Is member function pointer

    namespace __private {
        template<typename T>
        class __IsMemberFunctionPointer : public FalseType {};

        template<typename T, typename U>
        class __IsMemberFunctionPointer<T U::*> : public IsFunction<T> {};
    }
    
    /// @brief Checks whether type is pointer to non-static member function
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_member_function_pointer
    template<typename T>
    class IsMemberFunctionPointer : 
        public __private::__IsMemberFunctionPointer<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMemberFunctionPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMemberFunctionPointerVariable = IsMemberFunctionPointer<T>::Value;
    #endif

    // Is member object pointer

    /// @brief Checks whether type is pointer to non-static member object
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_member_object_pointer
    template<typename T>
    class IsMemberObjectPointer : public BoolConstant<IsMemberPointer<T>::Value && 
        !IsMemberFunctionPointer<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMemberObjectPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMemberObjectPointerVariable = IsMemberObjectPointer<T>::Value;
    #endif

    // Is class

    namespace __private {
        template<typename T>
        static long __TestClass(int T::*) {
            return !IsUnion<T>::Value;
        }

        template<typename T> 
        static char __TestClass(...) {
            return false;
        }
    }

    /// @brief Checks whether type is non-union class
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_class
    template<typename T>
    class IsClass : public BoolConstant<sizeof(__private::__TestClass<T>(0)) == sizeof(long)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsClass
    /// @since C++17
    template<typename T>
    inline constexpr bool IsClassVariable = IsClass<T>::Value;
    #endif

    // Is base of

    namespace __private {
        template<typename Base, typename Derived>
        static long __TestBaseOf(Base*);

        template<typename, typename>
        static char __TestBaseOf(...);
    }

    /// @brief Checks whether type is base of another type
    /// @tparam Base Base type
    /// @tparam Derived Derived type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_base_of
    template<typename Base, typename Derived>
    class IsBaseOf : public BoolConstant<IsClass<Base>::Value && IsClass<Derived>::Value 
        && (sizeof(__private::__TestBaseOf<Base, Derived>(static_cast<Derived*>(0))) == sizeof(long))> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsBaseOf
    /// @since C++17
    template<typename Base, typename Derived>
    inline constexpr bool IsBaseOfVariable = IsBaseOf<Base, Derived>::Value;
    #endif

    // Is convertible

    namespace __private {
        template<typename T>
        static long __TestReturnable(T(*)());

        template<typename>
        static char __TestReturnable(...);

        template<typename From, typename To>
        static long __TestImplicitlyConvertible(To(*)(From));

        template<typename, typename>
        static char __TestImplicitlyConvertible(...);

        template<typename T>
        static T __TestConvert(T);
    }

    /// @brief Checks whether implicit conversion can be done between types
    /// @tparam From Initial type
    /// @tparam To Type to convert to
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_convertible
    template<typename From, typename To>
    class IsConvertible : public BoolConstant<
        sizeof(__private::__TestReturnable<To>(static_cast<To(*)()>(0))) == sizeof(long) && 
        sizeof(__private::__TestImplicitlyConvertible<From, To>(&__private::__TestConvert)) == sizeof(long) || 
        (IsVoid<From>::Value && IsVoid<To>::Value)
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsConvertible
    /// @since C++17
    template<typename From, typename To>
    inline constexpr bool IsConvertibleVariable = IsConvertible<From, To>::Value;
    #endif

    // Is enum

    namespace __private {
        template<typename T>
        class __TestConvertibleToInt {
        public:
            static T Type();
            static long Check(int);
            static char Check(...);
            
            static __WSTL_CONSTEXPR__ const bool Value = sizeof(Check(Type())) == sizeof(long);
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __TestConvertibleToInt<T>::Value;
    }

    /// @brief Checks whether type is enumeration
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_enum
    template<typename T>
    class IsEnum : public BoolConstant<!IsFundamental<T>::Value && !IsClass<T>::Value && 
        !IsUnion<T>::Value && __private::__TestConvertibleToInt<T>::Value> {};
    
    #ifdef __WSTL_CXX17__
    /// @copydoc IsEnum
    /// @since C++17
    template<typename T>
    inline constexpr bool IsEnumVariable = IsEnum<T>::Value;
    #endif

    // Is pointer

    namespace __private {
        template<typename T>
        class __IsPointer : public FalseType {};

        template<typename T>
        class __IsPointer<T*> : public TrueType {};
    }

    /// @brief Checks whether type is pointer
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_pointer
    template<typename T>
    class IsPointer : public __private::__IsPointer<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsPointerVariable = IsPointer<T>::Value;
    #endif

    // Is scalar

    /// @brief Checks whether type is scalar (arithmetic, enum, pointer, pointers to member)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_scalar
    template<typename T>
    class IsScalar : public BoolConstant<IsArithmetic<T>::Value || IsEnum<T>::Value 
        || IsPointer<T>::Value || IsMemberPointer<T>::Value || IsNullPointer<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsScalar
    /// @since C++17
    template<typename T>
    inline constexpr bool IsScalarVariable = IsScalar<T>::Value;
    #endif

    // Is trivial

    /// @brief Checks whether type is trivial (not user-defined)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_trivial
    template<typename T>
    class IsTrivial : public BoolConstant<IsFundamental<T>::Value || IsPointer<T>::Value 
        || IsArray<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTrivial
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTrivialVariable = IsTrivial<T>::Value;
    #endif

    // Is POD

    /// @brief Checks whether type is plain-old data 
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_pod
    template<typename T>
    class IsPOD : public BoolConstant<IsFundamental<T>::Value && IsPointer<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsPOD
    /// @since C++17
    template<typename T>
    inline constexpr bool IsPODVariable = IsPOD<T>::Value;
    #endif

    // Is object

    /// @brief Checks whether type is object (not function, reference, void)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_object
    template<typename T>
    class IsObject : public BoolConstant<IsScalar<T>::Value || IsArray<T>::Value || 
        IsArray<T>::Value || IsUnion<T>::Value || IsClass<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsObject
    /// @since C++17
    template<typename T>
    inline constexpr bool IsObjectVariable = IsObject<T>::Value;
    #endif
    
    // Is constructible

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is constructor
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename... Args>
    class IsConstructible : public BoolConstant<__is_constructible(T, Args...)> {};
    #else
    /// @brief Checks whether type is constructor
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename Args = void>
    class IsConstructible : public BoolConstant<__is_constructible(T, Args)> {};
    
    /// @brief Checks whether type is constructor
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T>
    class IsConstructible<T, void> : public BoolConstant<__is_constructible(T)> {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsConstructible
    /// @since C++17
    template<typename T, typename... Args>
    inline constexpr bool IsConstructibleVariable = IsConstructible<T, Args...>::Value;
    #endif

    // Is trivially constuctible

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is trivially constructible (does not call not trivial operations)
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename... Args>
    class IsTriviallyConstructible : public BoolConstant<
    #ifdef __WSTL_GCC__
    __has_trivial_constructor(T)
    #else
    __is_trivially_constructible(T, Args...)
    #endif
    > {};
    #else
    /// @brief Checks whether type is trivially constructible (does not call not trivial operations)
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename Args = void>
    class IsTriviallyConstructible : public BoolConstant<
    #ifdef __WSTL_GCC__
    __has_trivial_constructor(T)
    #else
    __is_trivially_constructible(T, Args)
    #endif
    > {};

    /// @brief Checks whether type is trivially constructible (does not call not trivial operations)
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T>
    class IsTriviallyConstructible<T, void> : public BoolConstant<
    #ifdef __WSTL_GCC__
    __has_trivial_constructor(T)
    #else
    __is_trivially_constructible(T)
    #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyConstructible
    /// @since C++17
    template<typename T, typename... Args>
    inline constexpr bool IsTriviallyConstructibleVariable = IsTriviallyConstructible<T, Args...>::Value;
    #endif

    // Is nothrow constructible

    #ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T, typename... Args>
        static constexpr bool __TestNothrowConstructible(int) noexcept(noexcept(T(DeclareValue<Args>()...))) {
            return noexcept(T(DeclareValue<Args>()...));
        }

        template<typename, typename...>
        static constexpr bool __TestNothrowConstructible(...) {
            return false;
        }
    }

    /// @brief Checks whether type is nothrow constructible (noexcept)
    /// @tparam T Constructor type
    /// @tparam ...Args Arguments type
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename... Args>
    class IsNothrowConstructible : 
        public BoolConstant<__private::__TestNothrowConstructible<T, Args...>(0)> {};

    #else
    /// @brief Checks whether type is nothrow constructible (noexcept)
    /// @tparam T Constructor type
    /// @tparam Args Arguments type (does nothing, not supported in C++98)
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename Args = void>
    class IsNothrowConstructible : public BoolConstant<
    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__)
    __has_nothrow_constructor(T)
    #elif defined(__WSTL_MSVC__)
    _has_nothrow_constructor(T)
    #else
    false
    #endif
    > {};
    
    /// @brief Checks whether type is nothrow constructible (noexcept)
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T>
    class IsNothrowConstructible<T, void> : public BoolConstant<
    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__)
    __has_nothrow_constructor(T)
    #elif defined(__WSTL_MSVC__)
    _has_nothrow_constructor(T)
    #else
    false
    #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowConstructible
    /// @since C++17
    template<typename T, typename... Args>
    inline constexpr bool IsNothrowConstructibleVariable = IsNothrowConstructible<T, Args...>::Value;
    #endif
    #endif

    // Is default constructible

    /// @brief Checks whether type can be constructed without arguments (T, T())
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_default_constructible
    template<typename T>
    class IsDefaultConstructible : public IsConstructible<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsDefaultConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsDefaultConstructibleVariable = IsDefaultConstructible<T>::Value;
    #endif

    // Is trivially default constructible

    /// @brief Checks whether type can be trivially (not calling any not trivial operations) constructed without arguments (T, T())
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_default_constructible
    template<typename T>
    class IsTriviallyDefaultConstructible : public IsTriviallyConstructible<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyDefaultConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyDefaultConstructibleVariable = IsTriviallyDefaultConstructible<T>::Value;
    #endif

    // Is nothrow default constructible

    #if defined(__WSTL_CXX11__) && defined(__WSTL_EXCEPTIONS__)
    /// @brief Checks whether type can be nothrow (noexcept) constructed without arguments (T, T())
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_default_constructible
    template<typename T>
    class IsNothrowDefaultConstructible : public IsNothrowConstructible<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowDefaultConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowDefaultConstructibleVariable = IsNothrowDefaultConstructible<T>::Value;
    #endif
    #endif

    // Is copy constructible

    /// @brief Checks whether type can be copy-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_constructible
    template<typename T>
    class IsCopyConstructible : public IsConstructible<T, 
        typename AddLValueReference<typename AddConst<T>::Type>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsCopyConstructibleVariable = IsCopyConstructible<T>::Value;
    #endif

    // Is trivially copy constructible

    /// @brief Checks whether type can be trivially copy-constructed (not calling any not trivial operations)
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_constructible
    template<typename T>
    class IsTriviallyCopyConstructible : public IsTriviallyConstructible<T, 
        typename AddLValueReference<typename AddConst<T>::Type>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyCopyConstructibleVariable = IsTriviallyCopyConstructible<T>::Value;
    #endif

    // Is nothrow copy constructible

    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type can be nothrow (noexcept) copy-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_constructible
    template<typename T>
    class IsNothrowCopyConstructible : public IsNothrowConstructible<T, 
        typename AddLValueReference<typename AddConst<T>::Type>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowCopyConstructibleVariable = IsNothrowCopyConstructible<T>::Value;
    #endif
    #endif

    // Is move constructible

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type can be move-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_constructible
    template<typename T>
    class IsMoveConstructible : public IsConstructible<T, typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMoveConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMoveConstructibleVariable = IsMoveConstructible<T>::Value;
    #endif

    // Is trivially move constructible

    /// @brief Checks whether type can be trivially move-constructed (not calling any non-trivial operations)   
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_constructible
    template<typename T>
    class IsTriviallyMoveConstructible : public IsTriviallyConstructible<T, 
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyMoveConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyMoveConstructibleVariable = IsTriviallyMoveConstructible<T>::Value;
    #endif

    // Is nothrow move constructible

    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type can be nothrow (noexcept) move-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @since C++11 
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_move_constructible
    template<typename T>
    class IsNothrowMoveConstructible : public IsNothrowConstructible<T, 
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowMoveConstructibleVariable = IsNothrowMoveConstructible<T>::Value;
    #endif
    #endif
    #endif

    // Is assignable

    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T, typename U>
        static constexpr bool __TestAssignable(int) {
            return sizeof(decltype(DeclareValue<T&>() = DeclareValue<U&>(), void())) > 0;
        }   

        template<typename, typename>
        static constexpr bool __TestAssignable(...) {
            return false;
        }    
        #else
        template<typename T, typename U>
        static long __TestAssignable(int, T* t = static_cast<T*>(0), U* u = static_cast<U*>(0), 
        char(*)[sizeof(*t = *u)] = 0);

        template<typename, typename>
        static char __TestAssignable(...);
        #endif
    }    

    /// @brief Checks whether type is assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    class IsAssignable : public BoolConstant<
    #ifdef __WSTL_CXX11__
    __private::__TestAssignable<T, U>(0)
    #else
    sizeof(__private::__TestAssignable<T, U>(0)) == sizeof(long)
    #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsAssignable
    /// @since C++17
    template<typename T, typename U>
    inline constexpr bool IsAssignableVariable = IsAssignable<T, U>::Value;
    #endif

    // Is trivially assignable

    /// @brief Checks whether type is trivially assignable to another (not calling any non-trivial operations)
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    class IsTriviallyAssignable : public BoolConstant<IsAssignable<T, U>::Value && IsTrivial<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyAssignable
    /// @since C++17
    template<typename T, typename U>
    inline constexpr bool IsTriviallyAssignableVariable = IsTriviallyAssignable<T, U>::Value;
    #endif

    // Is nothrow assignable

    #ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is nothrow (noexcept) assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    class IsNothrowAssignable : public BoolConstant<IsAssignable<T, U>::Value && 
        noexcept(DeclareValue<T>() = DeclareValue<U>())> {};
    #else
    /// @brief Checks whether type is nothrow (noexcept) assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    class IsNothrowAssignable : public BoolConstant<
    #if defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
    __is_nothrow_assignable(T, U)
    #else
    false
    #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowAssignable
    /// @since C++17
    template<typename T, typename U>
    inline constexpr bool IsNothrowAssignableVariable = IsNothrowAssignable<T, U>::Value;
    #endif
    #endif

    // Is copy assignable

    /// @brief Checks whether type is copy-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_assignable
    template<typename T>
    class IsCopyAssignable : public IsAssignable<typename AddLValueReference<T>::Type, 
        typename AddLValueReference<const T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsCopyAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsCopyAssignableVariable = IsCopyAssignable<T>::Value;
    #endif
    
    // Is trivially copy assignable

    /// @brief Checks whether type is trivially (not calling any non-trivial operations) copy-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_assignable
    template<typename T>
    class IsTriviallyCopyAssignable : public IsTriviallyAssignable<typename AddLValueReference<T>::Type,
        typename AddLValueReference<const T>::Type> {};

    #ifdef __WSTL_CXX17__
    template<typename T>
    inline constexpr bool IsTriviallyCopyAssignableVariable = IsTriviallyCopyAssignable<T>::Value;
    #endif

    // Is nothrow copy assignable
    
    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type is nothrow (noexcept) copy-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_assignable
    template<typename T>
    class IsNothrowCopyAssignable : public IsNothrowAssignable<typename AddLValueReference<T>::Type,
        typename AddLValueReference<const T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowCopyAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowCopyAssignableVariable = IsNothrowCopyAssignable<T>::Value;
    #endif
    #endif

    // Is move assignable

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is move-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_assignable
    template<typename T>
    class IsMoveAssignable : public IsAssignable<typename AddLValueReference<T>::Type, 
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMoveAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMoveAssignableVariable = IsMoveAssignable<T>::Value;
    #endif
    
    // Is trivially move assignable

    /// @brief Checks whether type is trivially (not calling any non-trivial operations) move-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_assignable
    template<typename T>
    class IsTriviallyMoveAssignable : public IsTriviallyAssignable<typename AddLValueReference<T>::Type,
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyMoveAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyMoveAssignableVariable = IsTriviallyMoveAssignable<T>::Value;
    #endif

    // Is nothrow move assignable
    
    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type is nothrow (noexcept) move-assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @ingroup type_traits
    /// @since C++11 
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_move_assignable
    template<typename T>
    class IsNothrowMoveAssignable : public IsNothrowAssignable<typename AddLValueReference<T>::Type,
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowMoveAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowMoveAssignableVariable = IsNothrowMoveAssignable<T>::Value;
    #endif
    #endif
    #endif

    // Is destructible

    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T>
        static constexpr bool __TestDestructible(int) {
            return sizeof(decltype(DeclareValue<T>().~T(), void())) > 0;
        }

        template<typename T>
        static constexpr bool __TestDestructible(...) {
            return false;
        }
        #endif
    }

    /// @brief Checks whether type is destructible
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_destructible
    template<typename T>
    class IsDestructible : public BoolConstant<
    #ifdef __WSTL_CXX11__
    __private::__TestDestructible<T>(0)
    #elif defined(__WSTL_GCC__) || defined(__WSTL_CLANG__)
    __is_destructible(T)
    #elif defined(__WSTL_MSVC__)
    __has_user_destructor(T)
    #else
    false
    #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsDestructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsDestructibleVariable = IsDestructible<T>::Value;
    #endif

    // Is trivially destructible

    /// @brief Checks whether type is trivially destructible
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_destructible
    template<typename T>
    class IsTriviallyDestructible : public BoolConstant<!__private::__TestDestructible<T>(0)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyDestructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyDestructibleVariable = IsTriviallyDestructible<T>::Value;
    #endif

    // Is nothrow destructible

    #ifdef __WSTL_EXCEPTIONS__
    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T>
        static constexpr bool __TestNothrowDestructible(int) {
            return noexcept(DeclareValue<T&>().~T());
        }
        
        template<typename>
        static constexpr bool __TestNothrowDestructible(...) {
            return false;
        }
        #endif
    }

    /// @brief Checks whether type is nothrow (noexcept) destructible
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_destructible
    template<typename T>
    class IsNothrowDestructible : public BoolConstant<
    #ifdef __WSTL_CXX11__
    __private::__TestNothrowDestructible<T>(0)
    #elif defined(__WSTL_GCC__) || defined(__WSTL_MSVC__) || defined(__WSTL_CLANG__)
    __is_nothrow_destructible(T)
    #else
    false
    #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowDestructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowDestructibleVariable = IsNothrowDestructible<T>::Value;
    #endif
    #endif

    // Is trivially copyable

    /// @brief Checks whether type is trivially copyable
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_trivially_copyable
    template<typename T>
    class IsTriviallyCopyable : public BoolConstant<IsTriviallyConstructible<T>::Value &&
        IsTriviallyDestructible<T>::Value && IsTriviallyAssignable<T, T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyCopyable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyCopyableVariable = IsTriviallyCopyable<T>::Value;
    #endif

    // Is lvalue reference

    namespace __private {
        template<typename T>
        class __IsLValueReference : public FalseType {};

        template<typename T>
        class __IsLValueReference<T&> : public TrueType {};
    }   

    /// @brief Checks whether type is lvalue reference
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_lvalue_reference
    template<typename T>
    class IsLValueReference : public __private::__IsLValueReference<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsLValueReference
    /// @since C++17
    template<typename T>
    inline constexpr bool IsLValueReferenceVariable = IsLValueReference<T>::Value;
    #endif

    // Is rvalue reference

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T>
        class __IsRValueReference : public FalseType {};

        template<typename T>
        class __IsRValueReference<T&&> : public TrueType {};
    }

    /// @brief Checks whether type is rvalue reference
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_rvalue_reference
    template<typename T>
    class IsRValueReference : public __private::__IsRValueReference<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsRValueReference
    /// @since C++17
    template<typename T>
    inline constexpr bool IsRValueReferenceVariable = IsRValueReference<T>::Value;
    #endif
    #endif

    // Is reference (lvalue or rvalue)

    /// @brief Checks whether type is reference
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_reference
    template<typename T>
    class IsReference : public BoolConstant<(
        IsLValueReference<T>::Value
        #ifdef __WSTL_CXX11__
        || IsRValueReference<T>::Value
        #endif
    )> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsReference
    /// @since C++17
    template<typename T>
    inline constexpr bool IsReferenceVariable = IsReference<T>::Value;
    #endif

    // Is signed

    /// @brief Checks whether type is signed
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_signed
    template<typename T>
    class IsSigned;

    template<typename T>
    class IsSigned : public FalseType {};

    template<> class IsSigned<signed char> : public TrueType {};
    template<> class IsSigned<char> : public BoolConstant<(char(255) < 0)> {};
    template<> class IsSigned<wchar_t> : public BoolConstant<(wchar_t(-1) < wchar_t(0))> {};
    template<> class IsSigned<short> : public TrueType {};
    template<> class IsSigned<int> : public TrueType {};
    template<> class IsSigned<long> : public TrueType {};
    template<> class IsSigned<long long> : public TrueType {};
    template<> class IsSigned<float> : public TrueType {};
    template<> class IsSigned<double> : public TrueType {};
    template<> class IsSigned<long double> : public TrueType {};

    #ifdef __WSTL_CXX20__
    template<> class IsSigned<char8_t> : public BoolConstant<(char8_t(-1) < char8_t(0))> {};
    #endif

    #ifdef __WSTL_CXX11__
    template<> class IsSigned<char16_t> : public BoolConstant<(char16_t(-1) < char16_t(0))> {};
    template<> class IsSigned<char32_t> : public BoolConstant<(char32_t(-1) < char32_t(0))> {};
    #endif

    template<typename T> class IsSigned<const T> : public IsSigned<T> {};
    template<typename T> class IsSigned<volatile T> : public IsSigned<T> {};
    template<typename T> class IsSigned<const volatile T> : public IsSigned<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsSigned
    /// @since C++17
    template<typename T>
    inline constexpr bool IsSignedVariable = IsSigned<T>::Value;
    #endif

    // IsUnsigned

    /// @brief Checks whether type is unsigned
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_unsigned
    template<typename T>
    class IsUnsigned;

    template<typename T>
    class IsUnsigned : public FalseType {};

    template<> class IsUnsigned<bool> : public TrueType {};
    template<> class IsUnsigned<char> : public BoolConstant<(char(255) > 0)> {};
    template<> class IsUnsigned<wchar_t> : public BoolConstant<(wchar_t(-1) > wchar_t(0))> {};
    template<> class IsUnsigned<unsigned char> : public TrueType {};
    template<> class IsUnsigned<unsigned short> : public TrueType {};
    template<> class IsUnsigned<unsigned int> : public TrueType {};
    template<> class IsUnsigned<unsigned long> : public TrueType {};
    template<> class IsUnsigned<unsigned long long> : public TrueType {};

    #ifdef __WSTL_CXX20__
    template<> class IsUnsigned<char8_t> : public BoolConstant<(char8_t(-1) > char8_t(0))> {};
    #endif

    #ifdef __WSTL_CXX11__
    template<> class IsUnsigned<char16_t> : public BoolConstant<(char16_t(-1) > char16_t(0))> {};
    template<> class IsUnsigned<char32_t> : public BoolConstant<(char32_t(-1) > char32_t(0))> {};
    #endif

    template<typename T> class IsUnsigned<const T> : public IsUnsigned<T> {};
    template<typename T> class IsUnsigned<volatile T> : public IsUnsigned<T> {};
    template<typename T> class IsUnsigned<const volatile T> : public IsUnsigned<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsUnsigned
    /// @since C++17
    template<typename T>
    inline constexpr bool IsUnsignedVariable = IsUnsigned<T>::Value;
    #endif

    // Decay

    /// @brief Performs conversions same as when passing function arguments by value
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/decay
    template<typename T>
    class Decay {
    private:
        typedef typename RemoveReference<T>::Type U;

    public:
        typedef typename Conditional<IsArray<U>::Value, 
            typename AddPointer<typename RemoveExtent<T>::Type>::Type,
            typename Conditional<IsFunction<U>::Value, typename AddPointer<U>::Type, 
            typename RemoveCV<U>::Type>::Type>::Type Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc Decay
    /// @since C++11
    template<typename T>
    using DecayType = typename Decay<T>::Type;
    #endif

    // Unwrap reference

    /// @brief Extracts underlying type from reference wrappers
    /// @tparam T Reference wrapper type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
    template<typename T>
    class UnwrapReference;
    
    template<typename T>
    class UnwrapReference {
    public:
        typedef T Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc UnwrapReference
    /// @since C++11
    template<typename T>
    using UnwrapReferenceType = typename UnwrapReference<T>::Type;
    #endif

    // Unwrap reference decay

    /// @brief Combines the behaviour of Decay and UnwrapReference
    /// @tparam T Type to process
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
    template<typename T>
    class UnwrapReferenceDecay : public UnwrapReference<typename Decay<T>::Type> {};

    #ifdef __WSTL_CXX11__
    /// @copydoc UnwrapReferenceDecay
    /// @since C++11
    template<typename T>
    using UnwrapReferenceDecayType = typename UnwrapReferenceDecay<T>::Type;
    #endif

    // Common type

    #ifdef __WSTL_CXX11__
    /// @brief Determines the common type among all types
    /// @tparam Ts Types to find common type among
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/common_type
    template<typename... Ts>
    class CommonType;

    // One type

    template<typename T>
    class CommonType<T> : public CommonType<T, T> {};

    namespace __private {
        template<typename T1, typename T2>
        using __ConditionalResultType = decltype(false ? DeclareValue<T1>() : DeclareValue<T2>());

        template<typename, typename, typename = void>
        class __DecayConditionalResult {};

        template<typename T1, typename T2>
        class __DecayConditionalResult<T1, T2, VoidType<__ConditionalResultType<T1, T2>>> :
            public Decay<__ConditionalResultType<T1, T2>> {};

        template<typename T1, typename T2, typename = void>
        class __CommonType2 : public __DecayConditionalResult<const T1&, const T2&> {};

        template<typename T1, typename T2>
        class __CommonType2<T1, T2, VoidType<__ConditionalResultType<T1, T2>>> : 
            public __DecayConditionalResult<T1, T2> {};
    }

    // Two types

    template<typename T1, typename T2>
    class CommonType<T1, T2> : public Conditional<IsSame<T1, typename Decay<T1>::Type>::Value &&
        IsSame<T2, typename Decay<T2>::Type>::Value, __private::__CommonType2<T1, T2>, 
        CommonType<typename Decay<T1>::Type, typename Decay<T2>::Type>>::Type {};

    // 3+ types

    namespace __private {
        template<typename AlwaysVoid, typename T1, typename T2, typename... R>
        class __CommonTypeMany {};

        template<typename T1, typename T2, typename... R>
        class __CommonTypeMany<VoidType<typename CommonType<T1, T2>::Type>, T1, T2, R...> :
            public CommonType<typename CommonType<T1, T2>::Type, R...> {};
    }

    template<typename T1, typename T2, typename... R>
    class CommonType<T1, T2, R...> : public __private::__CommonTypeMany<void, T1, T2, R...> {};

    #ifdef __WSTL_CXX11__
    /// @copydoc CommonType
    /// @since C++11
    template<typename... T>
    using CommonTypeType = typename CommonType<T...>::Type;
    #endif
    #endif
    
    // MakeSigned

    /// @brief Makes unsigned type signed
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/make_signed
    template<typename T>
    class MakeSigned;

    template<typename T>
    class MakeSigned { public: typedef T Type; };

    template<>
    class MakeSigned<unsigned char> { public: typedef signed char Type; };

    template<>
    class MakeSigned<unsigned short> { public: typedef short Type; };

    template<>
    class MakeSigned<unsigned int> { public: typedef int Type; };

    template<>
    class MakeSigned<unsigned long> { public: typedef long Type; };

    template<>
    class MakeSigned<unsigned long long> { public: typedef long long Type; };

    #ifdef __WSTL_CXX20__
    template<>
    class MakeSigned<char8_t> { public: typedef signed char Type; };
    #endif

    #ifdef __WSTL_CXX11__
    template<>
    class MakeSigned<char16_t> { public: typedef signed short Type; };

    template<>
    class MakeSigned<char32_t> { public: typedef signed int Type; };

    /// @copydoc MakeSigned
    /// @since C++11
    template<typename T>
    using MakeSignedType = typename MakeSigned<T>::Type;
    #endif
    
    // MakeUnsigned

    /// @brief Makes signed type unsigned
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/make_unsigned
    template<typename T>
    class MakeUnsigned;

    template<typename T>
    class MakeUnsigned { public: typedef T Type; };

    template<>
    class MakeUnsigned<char> { public: typedef unsigned char Type; };

    template<>
    class MakeUnsigned<short> { public: typedef unsigned short Type; };

    template<>
    class MakeUnsigned<int> { public: typedef unsigned int Type; };

    template<>
    class MakeUnsigned<long> { public: typedef unsigned long Type; };

    template<>
    class MakeUnsigned<long long> { public: typedef unsigned long long Type; };

    #ifdef __WSTL_CXX20__
    template<>
    class MakeUnsigned<char8_t> { public: typedef unsigned char Type; };
    #endif

    #ifdef __WSTL_CXX11__
    template<>
    class MakeUnsigned<char16_t> { public: typedef unsigned short Type; };

    template<>
    class MakeUnsigned<char32_t> { public: typedef unsigned int Type; };

    /// @copydoc MakeUnsigned
    /// @since C++11
    template<typename T>
    using MakeUnsignedType = typename MakeUnsigned<T>::Type;
    #endif
}

#endif
