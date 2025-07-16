// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_TUPLE_HPP__
#define __WSTL_TUPLE_HPP__

#include "private/TupleProperties.hpp"
#include "private/Swap.hpp"
#include "Utility.hpp"


/// @defgroup tuple Tuple
/// @brief A recursive, lightweight implementation of a tuple data structure
/// @ingroup utilities
/// @since C++11

namespace wstl {
    // Tuple
    /// TODO: Implementation for C++98

    #ifdef __WSTL_CXX11__
    /// @brief Fixed size container that holds elements of different types
    /// @tparam ...Types 
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple
    template<typename... Types>
    class Tuple;

    template<>
    class Tuple<> {
    public:
        // NO-OP
        __WSTL_CONSTEXPR14__ void Swap(Tuple&) __WSTL_NOEXCEPT__ {}
    };

    template<typename Head, typename... Tail>
    class Tuple<Head, Tail...> {
    public:
        /// @brief Default constructor
        constexpr Tuple() = default;

        /// @brief Parameterized constructor - initializes head and recursively the rest
        /// with the same types
        /// @param head Value of the head element
        /// @param ...tail Remaining values to initialize recursively
        constexpr Tuple(const Head& head, const Tail&... tail) : m_Head(head), m_Tail(tail...) {}
        
        /// @brief Templated parameterized constructor - initializes head and recursively the rest 
        /// with potentially different types
        /// @param head Value of the head element
        /// @param ...tail Remaining values to initialize recursively
        template<typename UHead, typename... UTail, typename = EnableIfType<
        IsConstructible<Head, UHead>::Value && IsConstructible<Tuple<UTail...>, UTail...>::Value>>
        constexpr Tuple(UHead&& head, UTail&&... tail) : m_Head(Forward<UHead>(head)), 
            m_Tail(Forward<UTail>(tail)...) {}

        /// @brief Copy constructor - copies from tuple of the same types
        /// @param other Tuple to copy from
        constexpr Tuple(const Tuple& other) : m_Head(other.m_Head), m_Tail(other.m_Tail) {}

        /// @brief Templated copy constructor - copies from tuple of potentially different types
        /// @param other Tuple to copy from
        template<typename UHead, typename... UTail, typename = EnableIfType<
        IsConstructible<Head, const UHead&>::Value && IsConstructible<Tuple<UTail...>, const UTail&...>::Value>>
        constexpr Tuple(const Tuple<UHead, UTail...>& other) : m_Head(other.__GetHead()),
            m_Tail(other.__GetTail()) {}

        /// @brief Move constructor - moves from tuple with the same types
        /// @param other Tuple to move from
        constexpr Tuple(Tuple&& other) : m_Head(Move(other.m_Head)), m_Tail(Move(other.m_Tail)) {}

        /// @brief Templated move constructor - moves from tuple with potentially different types 
        /// @param other Tuple to move from
        template<typename UHead, typename... UTail, typename = EnableIfType<
        IsConstructible<Head, UHead>::Value && IsConstructible<Tuple<Tail...>, UTail...>::Value>>
        constexpr Tuple(Tuple<UHead, UTail...>&& other) : m_Head(Move(other.__GetHead())),
            m_Tail(Move(other.__GetTail())) {}

        /// @brief Copy constructor for pair
        /// @param pair Pair to copy from
        template<typename U1, typename U2>
        constexpr Tuple(const Pair<U1, U2>& pair) : m_Head(pair.First), m_Tail(pair.Second) {}

        /// @brief Move constructor for pair
        /// @param pair Pair to move from
        template<typename U1, typename U2>
        constexpr Tuple(Pair<U1, U2>&& pair) : m_Head(Move(pair.First)), 
            m_Tail(Move(pair.Second)) {}

        /// @brief Assignment operator - assigns with the same types
        /// @param other Tuple to assign from
        __WSTL_CONSTEXPR14__ Tuple& operator=(const Tuple& other) {
            if(this != &other) {
                m_Head = other.m_Head;
                m_Tail = other.m_Tail;
            }

            return *this;
        }
        
        /// @brief Templated assignment operator - assigns with potentially different types
        /// @param other Tuple to assign from
        template<typename UHead, typename... UTail, typename = EnableIfType<
        IsAssignable<Head, UHead>::Value && IsAssignable<Tuple<Tail...>, Tuple<UTail...>>::Value>>
        __WSTL_CONSTEXPR14__ Tuple& operator=(const Tuple<UHead, UTail...>& other) {
            m_Head = other.m_Head;
            m_Tail = other.m_Tail;
            
            return *this;
        }

        /// @brief Move assignment operator - assigns using move semantics with the same types
        /// @param other Tuple to move from
        __WSTL_CONSTEXPR14__ Tuple& operator=(Tuple&& other) __WSTL_NOEXCEPT__ {
            if(this != &other) {
                m_Head = Move(other.m_Head);
                m_Tail = Move(other.m_Tail);
            }

            return *this;
        }

        /// @brief Templated move assignment operator - assigns using move semantics 
        /// with potentially different types
        /// @param other Tuple to move from
        template<typename UHead, typename... UTail, typename = EnableIfType<
        IsAssignable<Head, UHead>::Value && IsAssignable<Tuple<Tail...>, Tuple<UTail...>>::Value>>
        __WSTL_CONSTEXPR14__ Tuple& operator=(Tuple<UHead, UTail...>&& other) __WSTL_NOEXCEPT__ {
            m_Head = Move(other.m_Head);
            m_Tail = Move(other.m_Tail);

            return *this;
        }
        
        /// @brief Assignment operator - assigns from pair 
        /// @param pair Pair to assign from
        template<typename E1, typename E2>
        __WSTL_CONSTEXPR14__ Tuple& operator=(const Pair<E1, E2>& pair) {
            m_Head = pair.First;
            m_Tail = pair.Second;
            return *this;
        }
        
        /// @brief Move assignment operator - assigns from pair using move semantics
        /// @param pair Pair to move from
        template<typename E1, typename E2>
        __WSTL_CONSTEXPR14__ Tuple& operator=(Pair<E1, E2>&& pair) {
            m_Head = Move(pair.First);
            m_Tail = Move(pair.Second);
            return *this;
        }

        /// @brief Swaps content of two tuples
        /// @param other Tuple to swap contents with
        __WSTL_CONSTEXPR14__ void Swap(Tuple& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Head, other.m_Head);
            m_Tail.Swap(other.m_Tail);
        }

        /// @brief Getter for the head of the tuple
        /// @note Private function. DO NOT use directly in code!
        constexpr Head& __GetHead() {
            return m_Head;
        }

        /// @copydoc __GetHead()
        constexpr const Head& __GetHead() const {
            return m_Head;
        }

        /// @brief Getter for the tail (rest elements) of the tuple
        /// @note Private function. DO NOT use directly in code!
        constexpr Tuple<Tail...>& __GetTail() {
            return m_Tail;
        }

        /// @copydoc __GetTail()
        constexpr const Tuple<Tail...>& __GetTail() const {
            return m_Tail;
        }
    
    private:
        Head m_Head;
        Tuple<Tail...> m_Tail;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename... T>
    Tuple(T...) -> Tuple<T...>;
    #endif

    // Swap specialization

    template<typename... Types>
    /// @brief Swaps content of two tuples
    /// @param a First tuple
    /// @param b Second tuple
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/swap2
    __WSTL_CONSTEXPR14__ 
    inline void Swap(Tuple<Types...>& a, Tuple<Types...>& b) __WSTL_NOEXCEPT__ {
        a.Swap(b);
    }

    // Tuple element specialization

    template<typename Head, typename... Tail>
    struct TupleElement<0, Tuple<Head, Tail...>> { typedef Head Type; };

    template<size_t Index, typename Head, typename... Tail>
    struct TupleElement<Index, Tuple<Head, Tail...>> {
        typedef typename TupleElement<Index - 1, Tuple<Tail...>>::Type Type;
    };

    // Tuple size specialization

    template<typename... Types>
    struct TupleSize<Tuple<Types...>> : IntegralConstant<size_t, sizeof...(Types)> {};

    // Get specialization

    /// @brief Gets an element from tuple
    /// @tparam Index Index of element
    /// @param t Tuple from which to get element
    /// @return The element at the specified index
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/get
    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index > 0), TupleElementType<Index, Tuple<Head, Tail...>>> Get(Tuple<Head, Tail...>& tuple) __WSTL_NOEXCEPT__ {
        StaticAssert(Index < sizeof...(Tail) + 1, "Index out of bounds");
        return Get<Index - 1>(tuple.__GetTail());
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 0), Head> Get(Tuple<Head, Tail...>& tuple) __WSTL_NOEXCEPT__ {
        return tuple.__GetHead();
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index > 0), const TupleElementType<Index, Tuple<Head, Tail...>>> Get(const Tuple<Head, Tail...>& tuple) __WSTL_NOEXCEPT__ {
        StaticAssert(Index < sizeof...(Tail) + 1, "Index out of bounds");
        return Get<Index - 1>(tuple.__GetTail());
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 0), const Head> Get(const Tuple<Head, Tail...>& tuple) __WSTL_NOEXCEPT__ {
        return tuple.__GetHead();
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index > 0), TupleElementType<Index, Tuple<Head, Tail...>>&&> Get(Tuple<Head, Tail...>&& tuple) __WSTL_NOEXCEPT__ {
        StaticAssert(Index < sizeof...(Tail) + 1, "Index out of bounds");
        return Get<Index - 1>(Move(tuple.__GetTail()));
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 0), TupleElementType<Index, Tuple<Head, Tail...>>&&> Get(Tuple<Head, Tail...>&& tuple) __WSTL_NOEXCEPT__ {
        return Move(tuple.__GetHead());
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index > 0), const TupleElementType<Index, Tuple<Head, Tail...>>&&> Get(const Tuple<Head, Tail...>&& tuple) __WSTL_NOEXCEPT__ {
        StaticAssert(Index < sizeof...(Tail) + 1, "Index out of bounds");
        return Get<Index - 1>(Move(tuple.__GetTail()));
    }

    template<size_t Index, typename Head, typename... Tail>
    __WSTL_CONSTEXPR14__
    inline EnableIfType<(Index == 0), const TupleElementType<Index, Tuple<Head, Tail...>>&&> Get(const Tuple<Head, Tail...>&& tuple) __WSTL_NOEXCEPT__ {
        return Move(tuple.__GetHead());
    }

    // Comparison operators

    template<typename... TTypes, typename... UTypes>
    __WSTL_CONSTEXPR14__
    inline bool operator==(const Tuple<TTypes...>& a, const Tuple<UTypes...>& b) {
        return (a.__GetHead() == b.__GetHead()) && (a.__GetTail() == b.__GetTail());
    }

    template<typename... TTypes, typename... UTypes>
    __WSTL_CONSTEXPR14__
    inline bool operator!=(const Tuple<TTypes...>& a, const Tuple<UTypes...>& b) {
        return !(a == b);
    }

    template<typename... TTypes, typename... UTypes>
    __WSTL_CONSTEXPR14__
    inline bool operator<(const Tuple<TTypes...>& a, const Tuple<UTypes...>& b) {
        return (a.__GetHead() < b.__GetHead()) || 
            (!(b.__GetHead() < a.__GetHead()) && a.__GetTail() < b.__GetTail());
    }

    template<typename... TTypes, typename... UTypes>
    __WSTL_CONSTEXPR14__
    inline bool operator<=(const Tuple<TTypes...>& a, const Tuple<UTypes...>& b) {
        return !(b < a);
    }

    template<typename... TTypes, typename... UTypes>
    __WSTL_CONSTEXPR14__
    inline bool operator>(const Tuple<TTypes...>& a, const Tuple<UTypes...>& b) {
        return b < a;
    }

    template<typename... TTypes, typename... UTypes>
    __WSTL_CONSTEXPR14__
    inline bool operator>=(const Tuple<TTypes...>& a, const Tuple<UTypes...>& b) {
        return !(a < b);
    }

    // Base cases for empty tuples

    __WSTL_CONSTEXPR14__
    inline bool operator==(const Tuple<>&, const Tuple<>&) {
        return true;
    }

    __WSTL_CONSTEXPR14__
    inline bool operator<(const Tuple<>&, const Tuple<>&) {
        return false;
    }

    // Make tuple

    /// @brief Makes a tuple out of elements
    /// @param ...args Values of elements
    /// @return Tuple of these elements
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/make_tuple
    template<typename... Types>
    __WSTL_CONSTEXPR14__
    inline auto MakeTuple(Types&&... args) {
        return Tuple<UnwrapReferenceDecayType<Types>...>(Forward<Types>(args)...);
    }

    // Tie

    /// @brief Makes a tuple of lvalue references to its arguments
    /// @param ...args Arguments to be tied
    /// @return Tuple of lvalue references to these arguments
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/tie
    template<typename... Types>
    __WSTL_CONSTEXPR14__
    inline Tuple<Types&...> Tie(Types&... args) __WSTL_NOEXCEPT__ {
        return Tuple<Types&...>(args...);
    }

    // Forward as tuple

    /// @brief Makes a tuple of forwarding references to its arguments
    /// @param ...args Arguments to be forwarded
    /// @return Tuple of forwarding references to these arguments
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/forward_as_tuple
    template<typename... Types>
    __WSTL_CONSTEXPR14__
    inline Tuple<Types&&...> ForwardAsTuple(Types&&... args) __WSTL_NOEXCEPT__ {
        return Tuple<Types&&...>(Forward<Types>(args)...);
    }

    // Ignore

    namespace __private {
        struct __IgnoreType {
            template<typename T>
            __WSTL_CONSTEXPR14__ const __IgnoreType& operator=(const T&) const __WSTL_NOEXCEPT__ {
                return *this;
            }
        };
    }

    /// @brief Placeholder to skip an element when unpack a tuple using `Tie()`
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/ignore
    inline const __WSTL_CONSTEXPR14__ __private::__IgnoreType Ignore;

    // Tuple concatenate

    template<typename Tuple>
    __WSTL_CONSTEXPR14__
    Tuple TupleConcatenate(Tuple&& tuple) {
        return Forward<Tuple>(tuple);
    }

    #ifdef __WSTL_CXX14__
    namespace __private {
        template<typename Tuple1, typename Tuple2, size_t... Indices1, size_t... Indices2>
        constexpr auto __TupleConcatenate(Tuple1&& t1, Tuple2&& t2, IndexSequence<Indices1...>, IndexSequence<Indices2...>) {
            return Tuple<TupleElementType<Indices1, DecayType<Tuple1>>..., 
                         TupleElementType<Indices2, DecayType<Tuple2>>...>(
                Get<Indices1>(Forward<Tuple1>(t1))..., 
                Get<Indices2>(Forward<Tuple2>(t2))...
            );
        }
    }

    /// @brief Constructs a tuple that is a concatenation of other tuples
    /// @param t1 First tuple
    /// @param t2 Second tuple
    /// @return Concatenated tuple
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/tuple_cat
    template<typename Tuple1, typename Tuple2>
    constexpr decltype(auto) 
    TupleConcatenate(Tuple1&& t1, Tuple2&& t2) {
        using Indices1 = MakeIndexSequence<TupleSize<DecayType<Tuple1>>::Value>;
        using Indices2 = MakeIndexSequence<TupleSize<DecayType<Tuple2>>::Value>;

        return __private::__TupleConcatenate(
            Forward<Tuple1>(t1), 
            Forward<Tuple2>(t2), 
            Indices1{}, 
            Indices2{}
        );
    }               

    /// @brief Constructs a tuple that is a concatenation of other tuples
    /// @param t1 First tuple
    /// @param t2 Second tuple
    /// @param ...ts Other tuples to concatenate
    /// @return Concatenated tuple
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/tuple_cat
    template<typename Tuple1, typename Tuple2, typename... Tuples>
    constexpr decltype(auto) TupleConcatenate(Tuple1&& t1, Tuple2&& t2, Tuples&&... ts) {
        return TupleConcatenate(
            TupleConcatenate(Forward<Tuple1>(t1), Forward<Tuple2>(t2)), 
            Forward<Tuples>(ts)...
        );
    }
    #else
    namespace __private {
        template<typename Tuple1, typename Tuple2, size_t... Indices1, size_t... Indices2>
        Tuple<TupleElementType<Indices1, DecayType<Tuple1>>..., TupleElementType<Indices2, DecayType<Tuple2>>...> 
        __TupleConcatenate(Tuple1&& t1, Tuple2&& t2, IndexSequence<Indices1...>, IndexSequence<Indices2...>) {
            return Tuple<TupleElementType<Indices1, DecayType<Tuple1>>..., 
                         TupleElementType<Indices2, DecayType<Tuple2>>...>(
                Get<Indices1>(Forward<Tuple1>(t1))..., 
                Get<Indices2>(Forward<Tuple2>(t2))...
            );
        }
    }

    /// @brief Constructs a tuple that is a concatenation of other tuples
    /// @param t1 First tuple
    /// @param t2 Second tuple
    /// @return Concatenated tuple
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/tuple_cat
    template<typename... Types1, typename... Types2> 
    Tuple<Types1..., Types2...> TupleConcatenate(Tuple<Types1...>& t1, Tuple<Types2...>& t2) {
        using Indices1 = MakeIndexSequence<TupleSize<DecayType<Tuple<Types1...>>>::Value>;
        using Indices2 = MakeIndexSequence<TupleSize<DecayType<Tuple<Types2...>>>::Value>;

        return __private::__TupleConcatenate(
            Forward<Tuple<Types1...>>(t1), 
            Forward<Tuple<Types2...>>(t2), 
            Indices1{}, 
            Indices2{}
        );
    }               

    template<typename Tuple1, typename Tuple2, typename... Tuples>
    /// @brief Constructs a tuple that is a concatenation of other tuples
    /// @param t1 First tuple
    /// @param t2 Second tuple
    /// @param ...ts Other tuples to concatenate
    /// @return Concatenated tuple
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/tuple_cat
    decltype(TupleConcatenate(TupleConcatenate(DeclareValue<Tuple1>(), DeclareValue<Tuple2>()), DeclareValue<Tuples>()...))
    TupleConcatenate(Tuple1&& t1, Tuple2&& t2, Tuples&&... ts) {
        return TupleConcatenate(
            TupleConcatenate(Forward<Tuple1>(t1), Forward<Tuple2>(t2)), 
            Forward<Tuples>(ts)...
        );
    }
    #endif

    // Make from tuple

    namespace __private {
        template<typename T, typename Tuple, size_t... Indices>
        __WSTL_CONSTEXPR14__
        inline T __MakeFromTuple(Tuple&& tuple, IndexSequence<Indices...>) {
            return T(Get<Indices>(Forward<Tuple>(tuple))...);
        }
    }

    /// @brief Constructs an object using the arguments from tuple
    /// @tparam T Type of the constructible object
    /// @tparam Tuple Type of the tuple
    /// @param tuple Tuple with the arguments
    /// @return Constructed object
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/make_from_tuple
    template<typename T, typename Tuple>
    __WSTL_CONSTEXPR14__ 
    inline T MakeFromTuple(Tuple&& tuple) {
        return __private::__MakeFromTuple(Forward<Tuple>(tuple), 
            MakeIndexSequence<TupleSize<RemoveReferenceType<Tuple>>::Value>{});
    }

    // Apply

    namespace __private {
        #ifdef __WSTL_CXX14__
        template<typename Callable, typename Tuple, size_t... Indices>
        constexpr decltype(auto) __Apply(Callable&& callable, Tuple&& tuple, IndexSequence<Indices...>) {
            return Forward<Callable>(callable)(Get<Indices>(Forward<Tuple>(tuple))...);
        }
        #else
        template<typename Callable, typename Tuple, size_t... Indices>
        inline ResultOfType<Callable(TupleElementType<Indices, Tuple>...)> __Apply(Callable&& callable, Tuple&& tuple, IndexSequence<Indices...>) {
            return Forward<Callable>(callable)(Get<Indices>(Forward<Tuple>(tuple))...);
        }
        #endif
    }

    #ifdef __WSTL_CXX14__
    /// @brief Calls an object with arguments from tuple
    /// @param callable Object to invoke
    /// @param tuple Tuple whose elements will be used as arguments
    /// @return The value return by callable object
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/apply
    template<typename Callable, typename Tuple>
    constexpr decltype(auto) Apply(Callable&& callable, Tuple&& tuple) {
        return __private::__Apply(Forward<Callable>(callable), Forward<Tuple>(tuple),
            MakeIndexSequence<TupleSize<RemoveReferenceType<Tuple>>::Value>{});
    }
    #else
    /// @brief Calls an object with arguments from tuple
    /// @param callable Object to invoke
    /// @param tuple Tuple whose elements will be used as arguments
    /// @return The value return by callable object
    /// @ingroup tuple
    /// @see https://en.cppreference.com/w/cpp/utility/tuple/apply
    template<typename Callable, typename... Types>
    inline ResultOfType<Callable(Types...)> Apply(Callable&& callable, Tuple<Types...>& tuple) {
        return __private::__Apply(Forward<Callable>(callable), Forward<Tuple<Types...>>(tuple), 
            MakeIndexSequence<TupleSize<DecayType<Tuple<Types...>>>::Value>{});
    }
    #endif

    #endif
}

#endif