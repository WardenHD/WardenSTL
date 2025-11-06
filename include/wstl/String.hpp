// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_STRING_HPP__
#define __WSTL_STRING_HPP__

#include "private/Platform.hpp"
#include "BasicString.hpp"


namespace wstl {
    // String

    /// @brief Specialization of `BasicString` for `char` type, a 1-byte character string
    /// @tparam N Capacity of the string
    /// @ingroup string
    /// @see https://en.cppreference.com/w/cpp/string/basic_string
    template<size_t N>
    class String : public BasicString<String<N>, char> {
    private:
        typedef BasicString<String<N>, char> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        /// @brief Default constructor
        String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        /// @brief Constructor with count and value
        /// @param count Number of characters
        /// @param value Character to fill with
        String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        /// @brief Constructor from iterators
        /// @param first Iterator to the first character
        /// @param last Iterator to one past the last character
        template<typename InputIterator>
        String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        /// @brief Constructor from C-style string and length
        /// @param string C-style string pointer
        /// @param count Number of characters in the string
        String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        /// @brief Constructor from C-style string
        /// @param string C-style string pointer
        String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        /// @brief Constructor from string view
        /// @param view String view to construct from
        explicit String(const StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        /// @brief Constructor from substring of string view
        /// @param view String view to construct from
        /// @param position Starting position in the string view
        /// @param count Number of characters to take from the string view (default is `NoPosition`, meaning until the end)
        explicit String(const StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        /// @brief Copy constructor
        /// @param other String to copy from
        String(const String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        String(const BasicString<Derived, ValueType, Traits>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor with substring from string interface
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        String(const BasicString<Derived, ValueType, Traits>& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other, position, count);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other String to move from
        /// @since C++11
        String(String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        /// @brief Move constructor from string interface
        /// @param other String to move from
        /// @since C++11
        template<typename Derived, typename Traits>
        String(BasicString<Derived, ValueType, Traits>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        /// @brief Copy constructor with substring
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        String(const String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor with substring
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        String(String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        /// @brief Move constructor with substring from string interface
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        String(BasicString<Derived, ValueType, Traits>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor from initializer list
        /// @param list Initializer list of characters
        String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        /// @brief Copy assignment operator from another string
        /// @param other String to copy from
        String& operator=(const String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        /// @brief Copy assignment operator from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        String& operator=(const BasicString<Derived, ValueType, Traits>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator from another string
        /// @param other String to move from
        String& operator=(String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        /// @brief Move assignment operator from string interface
        /// @param other String to move from
        template<typename Derived, typename Traits>
        String& operator=(BasicString<Derived, ValueType, Traits>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator from initializer list
        /// @param list Initializer list of characters
        String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Copy assignment operator from a C-style string
        /// @param string C-style string to assign from
        String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        /// @brief Assignment operator for a character
        /// @param value Character to assign
        String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        /// @brief Assignment operator for string view
        /// @param view String view to assign from
        String& operator=(const StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
        
        /// @brief Deleted null pointer constructor
        String(NullPointerType) __WSTL_DELETE__;

        /// @brief Deleted null pointer assignment operator
        String& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    String(const char (&)[N]) -> String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived, typename Traits>
    struct Hash<BasicString<Derived, char, Traits> > {
        size_t operator()(const BasicString<Derived, char, Traits>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    template<size_t N>
    struct Hash<String<N>> {
        size_t operator()(const String<N>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    // Make string

    /// @brief Creates a string from a string literal
    /// @param string String literal
    /// @return Constructed string (its length excludes the null terminator)
    template<size_t N>
    String<N - 1> MakeString(const char (&string)[N]) {
        return String<N - 1>(string, string::Length(string, N - 1));
    }

    /// @brief Creates a string with specified capacity from a string literal
    /// @tparam Capacity Capacity of the string to create
    /// @param string String literal
    /// @return Constructed string with the specified capacity (excluding the null terminator)
    template<size_t Capacity, size_t N>
    String<Capacity> MakeStringWithCapacity(const char (&string)[N]) {
        return String<Capacity>(string, string::Length(string, N));
    }

    // Literal operator

    #ifdef __WSTL_CXX11__
    inline namespace literals {
        inline namespace string_literals {
            /// @brief User-defined literal for creating `String`
            /// @tparam T Character type (must be `char`)
            /// @tparam Ts Characters of the string literal
            /// @return Constructed string
            /// @warning This literal works in an unexpected way with IntelliSense, and may show errors that do not actually exist
            /// @since C++11
            template<typename T, T... Ts>
            EnableIfType<IsSame<T, char>::Value, String<sizeof...(Ts)>> operator""_s() {
                #ifdef __WSTL_NO_INITIALIZERLIST__
                T string[] = {Ts..., '\0'};
                return String<sizeof...(Ts)>(string, sizeof...(Ts));
                #else
                return String<sizeof...(Ts)>({Ts...});
                #endif
            }
        }
    }
    #endif

    // Wide String

    /// @brief Specialization of `BasicString` for `wchar_t` type, a 2-byte or 4-byte character string
    /// @tparam N Capacity of the string
    /// @ingroup string
    /// @see https://en.cppreference.com/w/cpp/string/basic_string
    template<size_t N>
    class WideString : public BasicString<WideString<N>, wchar_t> {
    private:
        typedef BasicString<WideString<N>, wchar_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        /// @brief Default constructor
        WideString() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        /// @brief Constructor with count and value
        /// @param count Number of characters
        /// @param value Character to fill with
        WideString(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        /// @brief Constructor from iterators
        /// @param first Iterator to the first character
        /// @param last Iterator to one past the last character
        template<typename InputIterator>
        WideString(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        /// @brief Constructor from C-style string and length
        /// @param string C-style string pointer
        /// @param count Number of characters in the string
        WideString(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        /// @brief Constructor from C-style string
        /// @param string C-style string pointer
        WideString(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        /// @brief Constructor from string view
        /// @param view String view to construct from
        explicit WideString(const WideStringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        /// @brief Constructor from substring of string view
        /// @param view String view to construct from
        /// @param position Starting position in the string view
        /// @param count Number of characters to take from the string view (default is `NoPosition`, meaning until the end)
        explicit WideString(const WideStringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        /// @brief Copy constructor
        /// @param other String to copy from
        WideString(const WideString& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        WideString(const BasicString<Derived, ValueType>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor with substring from string interface
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        WideString(const BasicString<Derived, ValueType, Traits>& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other, position, count);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other String to move from
        /// @since C++11
        WideString(WideString&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        /// @brief Move constructor from string interface
        /// @param other String to move from
        /// @since C++11
        template<typename Derived, typename Traits>
        WideString(BasicString<Derived, ValueType, Traits>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        /// @brief Copy constructor with substring
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        WideString(const WideString& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor with substring
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        WideString(WideString&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        /// @brief Move constructor with substring from string interface
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        WideString(BasicString<Derived, ValueType>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor from initializer list
        /// @param list Initializer list of characters
        WideString(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        /// @brief Copy assignment operator from another string
        /// @param other String to copy from
        WideString& operator=(const WideString& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        /// @brief Copy assignment operator from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        WideString& operator=(const BasicString<Derived, ValueType, Traits>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator from another string
        /// @param other String to move from
        WideString& operator=(WideString&& other) {
            this->Assign(Move(other));
            return *this;
        }

        /// @brief Move assignment operator from string interface
        /// @param other String to move from
        template<typename Derived, typename Traits>
        WideString& operator=(BasicString<Derived, ValueType, Traits>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator from initializer list
        /// @param list Initializer list of characters
        WideString& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Copy assignment operator from a C-style string
        /// @param string C-style string to assign from
        WideString& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        /// @brief Assignment operator for a character
        /// @param value Character to assign
        WideString& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        /// @brief Assignment operator for string view
        /// @param view String view to assign from
        WideString& operator=(const WideStringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
        
        /// @brief Deleted null pointer constructor
        WideString(NullPointerType) __WSTL_DELETE__;

        /// @brief Deleted null pointer assignment operator
        WideString& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    WideString(const wchar_t (&)[N]) -> WideString<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived, typename Traits>
    struct Hash<BasicString<Derived, wchar_t, Traits> > {
        size_t operator()(const BasicString<Derived, wchar_t, Traits>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    template<size_t N>
    struct Hash<WideString<N>> {
        size_t operator()(const WideString<N>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    // Make string

    /// @brief Creates a string from a string literal
    /// @param string String literal
    /// @return Constructed string (its length excludes the null terminator)
    template<size_t N>
    WideString<N - 1> MakeString(const wchar_t (&string)[N]) {
        return WideString<N - 1>(string, string::Length(string, N - 1));
    }

    /// @brief Creates a string with specified capacity from a string literal
    /// @tparam Capacity Capacity of the string to create
    /// @param string String literal
    /// @return Constructed string with the specified capacity (excluding the null terminator)
    template<size_t Capacity, size_t N>
    WideString<Capacity> MakeStringWithCapacity(const wchar_t (&string)[N]) {
        return WideString<Capacity>(string, string::Length(string, N));
    }

    // Literal operator

    #ifdef __WSTL_CXX11__
    inline namespace literals {
        inline namespace string_literals {
            /// @brief User-defined literal for creating `WideString`
            /// @tparam T Character type (must be `wchar_t`)
            /// @tparam Ts Characters of the string literal
            /// @return Constructed string
            /// @warning This literal works in an unexpected way with IntelliSense, and may show errors that do not actually exist
            /// @since C++11
            template<typename T, T... Ts>
            EnableIfType<IsSame<T, wchar_t>::Value, WideString<sizeof...(Ts)>> operator""_s() {
                #ifdef __WSTL_NO_INITIALIZERLIST__
                T string[] = {Ts..., '\0'};
                return WideString<sizeof...(Ts)>(string, sizeof...(Ts));
                #else
                return WideString<sizeof...(Ts)>({Ts...});
                #endif
            }
        }
    }
    #endif

    #ifdef __WSTL_CXX11__
    // U16 String

    /// @brief Specialization of `BasicString` for `char16_t` type, a 2-byte character string
    /// @tparam N Capacity of the string
    /// @ingroup string
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/string/basic_string
    template<size_t N>
    class U16String : public BasicString<U16String<N>, char> {
    private:
        typedef BasicString<U16String<N>, char> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        /// @brief Default constructor
        U16String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        /// @brief Constructor with count and value
        /// @param count Number of characters
        /// @param value Character to fill with
        U16String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        /// @brief Constructor from iterators
        /// @param first Iterator to the first character
        /// @param last Iterator to one past the last character
        template<typename InputIterator>
        U16String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        /// @brief Constructor from C-style string and length
        /// @param string C-style string pointer
        /// @param count Number of characters in the string
        U16String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        /// @brief Constructor from C-style string
        /// @param string C-style string pointer
        U16String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        /// @brief Constructor from string view
        /// @param view String view to construct from
        explicit U16String(const U16StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        /// @brief Constructor from substring of string view
        /// @param view String view to construct from
        /// @param position Starting position in the string view
        /// @param count Number of characters to take from the string view (default is `NoPosition`, meaning until the end)
        explicit U16String(const U16StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        /// @brief Copy constructor
        /// @param other String to copy from
        U16String(const U16String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        U16String(const BasicString<Derived, ValueType, Traits>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor with substring from string interface
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        U16String(const BasicString<Derived, ValueType, Traits>& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other, position, count);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other String to move from
        /// @since C++11
        U16String(U16String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        /// @brief Move constructor from string interface
        /// @param other String to move from
        /// @since C++11
        template<typename Derived, typename Traits>
        U16String(BasicString<Derived, ValueType, Traits>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        /// @brief Copy constructor with substring
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        U16String(const U16String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor with substring
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        U16String(U16String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        /// @brief Move constructor with substring from string interface
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        U16String(BasicString<Derived, ValueType, Traits>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor from initializer list
        /// @param list Initializer list of characters
        U16String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        /// @brief Copy assignment operator from another string
        /// @param other String to copy from
        U16String& operator=(const U16String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        /// @brief Copy assignment operator from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        U16String& operator=(const BasicString<Derived, ValueType, Traits>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator from another string
        /// @param other String to move from
        U16String& operator=(U16String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        /// @brief Move assignment operator from string interface
        /// @param other String to move from
        template<typename Derived, typename Traits>
        U16String& operator=(BasicString<Derived, ValueType, Traits>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator from initializer list
        /// @param list Initializer list of characters
        U16String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Copy assignment operator from a C-style string
        /// @param string C-style string to assign from
        U16String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        /// @brief Assignment operator for a character
        /// @param value Character to assign
        U16String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        /// @brief Assignment operator for string view
        /// @param view String view to assign from
        U16String& operator=(const U16StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
        
        /// @brief Deleted null pointer constructor
        U16String(NullPointerType) __WSTL_DELETE__;

        /// @brief Deleted null pointer assignment operator
        U16String& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    U16String(const char16_t (&)[N]) -> U16String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived, typename Traits>
    struct Hash<BasicString<Derived, char16_t, Traits> > {
        size_t operator()(const BasicString<Derived, char16_t, Traits>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    template<size_t N>
    struct Hash<U16String<N>> {
        size_t operator()(const U16String<N>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    // Make string

    /// @brief Creates a string from a string literal
    /// @param string String literal
    /// @return Constructed string (its length excludes the null terminator)
    template<size_t N>
    U16String<N - 1> MakeString(const char16_t (&string)[N]) {
        return U16String<N - 1>(string, string::Length(string, N - 1));
    }

    /// @brief Creates a string with specified capacity from a string literal
    /// @tparam Capacity Capacity of the string to create
    /// @param string String literal
    /// @return Constructed string with the specified capacity (excluding the null terminator)
    template<size_t Capacity, size_t N>
    U16String<Capacity> MakeStringWithCapacity(const char16_t (&string)[N]) {
        return U16String<Capacity>(string, string::Length(string, N));
    }

    // Literal operator

    #ifdef __WSTL_CXX11__
    inline namespace literals {
        inline namespace string_literals {
            /// @brief User-defined literal for creating `U16String`
            /// @tparam T Character type (must be `char16_t`)
            /// @tparam Ts Characters of the string literal
            /// @return Constructed string
            /// @warning This literal works in an unexpected way with IntelliSense, and may show errors that do not actually exist
            /// @since C++11
            template<typename T, T... Ts>
            EnableIfType<IsSame<T, char16_t>::Value, U16String<sizeof...(Ts)>> operator""_s() {
                #ifdef __WSTL_NO_INITIALIZERLIST__
                T string[] = {Ts..., '\0'};
                return U16String<sizeof...(Ts)>(string, sizeof...(Ts));
                #else
                return U16String<sizeof...(Ts)>({Ts...});
                #endif
            }
        }
    }
    #endif

    // U32 String

    /// @brief Specialization of `BasicString` for `char32_t` type, a 4-byte character string
    /// @tparam N Capacity of the string
    /// @ingroup string
    /// @see https://en.cppreference.com/w/cpp/string/basic_string
    template<size_t N>
    class U32String : public BasicString<U32String<N>, char32_t> {
    private:
        typedef BasicString<U32String<N>, char32_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        /// @brief Default constructor
        U32String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        /// @brief Constructor with count and value
        /// @param count Number of characters
        /// @param value Character to fill with
        U32String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        /// @brief Constructor from iterators
        /// @param first Iterator to the first character
        /// @param last Iterator to one past the last character
        template<typename InputIterator>
        U32String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        /// @brief Constructor from C-style string and length
        /// @param string C-style string pointer
        /// @param count Number of characters in the string
        U32String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        /// @brief Constructor from C-style string
        /// @param string C-style string pointer
        U32String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        /// @brief Constructor from string view
        /// @param view String view to construct from
        explicit U32String(const U32StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        /// @brief Constructor from substring of string view
        /// @param view String view to construct from
        /// @param position Starting position in the string view
        /// @param count Number of characters to take from the string view (default is `NoPosition`, meaning until the end)
        explicit U32String(const U32StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        /// @brief Copy constructor
        /// @param other String to copy from
        U32String(const U32String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        U32String(const BasicString<Derived, ValueType, Traits>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor with substring from string interface
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        U32String(const BasicString<Derived, ValueType, Traits>& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other, position, count);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other String to move from
        /// @since C++11
        U32String(U32String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        /// @brief Move constructor from string interface
        /// @param other String to move from
        /// @since C++11
        template<typename Derived, typename Traits>
        U32String(BasicString<Derived, ValueType, Traits>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        /// @brief Copy constructor with substring
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        U32String(const U32String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor with substring
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        U32String(U32String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        /// @brief Move constructor with substring from string interface
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        U32String(BasicString<Derived, ValueType, Traits>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor from initializer list
        /// @param list Initializer list of characters
        U32String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        /// @brief Copy assignment operator from another string
        /// @param other String to copy from
        U32String& operator=(const U32String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        /// @brief Copy assignment operator from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        U32String& operator=(const BasicString<Derived, ValueType, Traits>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator from another string
        /// @param other String to move from
        U32String& operator=(U32String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        /// @brief Move assignment operator from string interface
        /// @param other String to move from
        template<typename Derived, typename Traits>
        U32String& operator=(BasicString<Derived, ValueType, Traits>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator from initializer list
        /// @param list Initializer list of characters
        U32String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Copy assignment operator from a C-style string
        /// @param string C-style string to assign from
        U32String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        /// @brief Assignment operator for a character
        /// @param value Character to assign
        U32String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        /// @brief Assignment operator for string view
        /// @param view String view to assign from
        U32String& operator=(const U32StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
        
        /// @brief Deleted null pointer constructor
        U32String(NullPointerType) __WSTL_DELETE__;

        /// @brief Deleted null pointer assignment operator
        U32String& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    U32String(const char32_t (&)[N]) -> U32String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived, typename Traits>
    struct Hash<BasicString<Derived, char32_t, Traits> > {
        size_t operator()(const BasicString<Derived, char32_t, Traits>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    template<size_t N>
    struct Hash<U32String<N>> {
        size_t operator()(const U32String<N>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    // Make string

    /// @brief Creates a string from a string literal
    /// @param string String literal
    /// @return Constructed string (its length excludes the null terminator)
    template<size_t N>
    U32String<N - 1> MakeString(const char32_t (&string)[N]) {
        return U32String<N - 1>(string, string::Length(string, N - 1));
    }

    /// @brief Creates a string with specified capacity from a string literal
    /// @tparam Capacity Capacity of the string to create
    /// @param string String literal
    /// @return Constructed string with the specified capacity (excluding the null terminator)
    template<size_t Capacity, size_t N>
    U32String<Capacity> MakeStringWithCapacity(const char32_t (&string)[N]) {
        return U32String<Capacity>(string, string::Length(string, N));
    }

    // Literal operator

    #ifdef __WSTL_CXX11__
    inline namespace literals {
        inline namespace string_literals {
            /// @brief User-defined literal for creating `U32String`
            /// @tparam T Character type (must be `char32_t`)
            /// @tparam Ts Characters of the string literal
            /// @return Constructed string
            /// @warning This literal works in an unexpected way with IntelliSense, and may show errors that do not actually exist
            /// @since C++11
            template<typename T, T... Ts>
            EnableIfType<IsSame<T, char32_t>::Value, U32String<sizeof...(Ts)>> operator""_s() {
                #ifdef __WSTL_NO_INITIALIZERLIST__
                T string[] = {Ts..., '\0'};
                return U32String<sizeof...(Ts)>(string, sizeof...(Ts));
                #else
                return U32String<sizeof...(Ts)>({Ts...});
                #endif
            }
        }
    }
    #endif
    #endif

    #ifdef __WSTL_CXX20__
    // U8 String

    /// @brief Specialization of `BasicString` for `char8_t` type, a UTF-8 encoded string
    /// @tparam N Capacity of the string
    /// @ingroup string
    /// @see https://en.cppreference.com/w/cpp/string/basic_string
    template<size_t N>
    class U8String : public BasicString<U8String<N>, char8_t> {
    private:
        typedef BasicString<U8String<N>, char8_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        /// @brief Default constructor
        U8String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        /// @brief Constructor with count and value
        /// @param count Number of characters
        /// @param value Character to fill with
        U8String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        /// @brief Constructor from iterators
        /// @param first Iterator to the first character
        /// @param last Iterator to one past the last character
        template<typename InputIterator>
        U8String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        /// @brief Constructor from C-style string and length
        /// @param string C-style string pointer
        /// @param count Number of characters in the string
        U8String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        /// @brief Constructor from C-style string
        /// @param string C-style string pointer
        U8String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        /// @brief Constructor from string view
        /// @param view String view to construct from
        explicit U8String(const U8StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        /// @brief Constructor from substring of string view
        /// @param view String view to construct from
        /// @param position Starting position in the string view
        /// @param count Number of characters to take from the string view (default is `NoPosition`, meaning until the end)
        explicit U8String(const U8StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        /// @brief Copy constructor
        /// @param other String to copy from
        U8String(const U8String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        U8String(const BasicString<Derived, ValueType, Traits>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        /// @brief Copy constructor with substring from string interface
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        U8String(const BasicString<Derived, ValueType, Traits>& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other, position, count);
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other String to move from
        /// @since C++11
        U8String(U8String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        /// @brief Move constructor from string interface
        /// @param other String to move from
        /// @since C++11
        template<typename Derived, typename Traits>
        U8String(BasicString<Derived, ValueType, Traits>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        /// @brief Copy constructor with substring
        /// @param other String to copy from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        U8String(const U8String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor with substring
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        U8String(U8String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        /// @brief Move constructor with substring from string interface
        /// @param other String to move from
        /// @param position Starting position in the other string
        /// @param count Number of characters to take from the other string (default is `NoPosition`, meaning until the end)
        template<typename Derived, typename Traits>
        U8String(BasicString<Derived, ValueType, Traits>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Constructor from initializer list
        /// @param list Initializer list of characters
        U8String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        /// @brief Copy assignment operator from another string
        /// @param other String to copy from
        U8String& operator=(const U8String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        /// @brief Copy assignment operator from string interface
        /// @param other String to copy from
        template<typename Derived, typename Traits>
        U8String& operator=(const BasicString<Derived, ValueType, Traits>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator from another string
        /// @param other String to move from
        U8String& operator=(U8String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        /// @brief Move assignment operator from string interface
        /// @param other String to move from
        template<typename Derived, typename Traits>
        U8String& operator=(BasicString<Derived, ValueType, Traits>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        /// @brief Assignment operator from initializer list
        /// @param list Initializer list of characters
        U8String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        /// @brief Copy assignment operator from a C-style string
        /// @param string C-style string to assign from
        U8String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        /// @brief Assignment operator for a character
        /// @param value Character to assign
        U8String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        /// @brief Assignment operator for string view
        /// @param view String view to assign from
        U8String& operator=(const U8StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
        
        /// @brief Deleted null pointer constructor
        U8String(NullPointerType) __WSTL_DELETE__;

        /// @brief Deleted null pointer assignment operator
        U8String& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    U8String(const char8_t (&)[N]) -> U8String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived, typename Traits>
    struct Hash<BasicString<Derived, char8_t, Traits> > {
        size_t operator()(const BasicString<Derived, char8_t, Traits>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    template<size_t N>
    struct Hash<U8String<N>> {
        size_t operator()(const U8String<N>& string) const {
            return __private::__GenericHash<size_t>(reinterpret_cast<const uint8_t*>(string.Data()), reinterpret_cast<const uint8_t*>(string.Data() + string.Size()));
        }
    };

    // Make string

    /// @brief Creates a string from a string literal
    /// @param string String literal
    /// @return Constructed string (its length excludes the null terminator)
    template<size_t N>
    U8String<N - 1> MakeString(const char8_t (&string)[N]) {
        return U8String<N - 1>(string, string::Length(string, N - 1));
    }

    /// @brief Creates a string with specified capacity from a string literal
    /// @tparam Capacity Capacity of the string to create
    /// @param string String literal
    /// @return Constructed string with the specified capacity (excluding the null terminator)
    template<size_t Capacity, size_t N>
    U8String<Capacity> MakeStringWithCapacity(const char8_t (&string)[N]) {
        return U8String<Capacity>(string, string::Length(string, N));
    }

    // Literal operator

    #ifdef __WSTL_CXX11__
    inline namespace literals {
        inline namespace string_literals {
            /// @brief User-defined literal for creating `U8String`
            /// @tparam T Character type (must be `char8_t`)
            /// @tparam Ts Characters of the string literal
            /// @return Constructed string
            /// @warning This literal works in an unexpected way with IntelliSense, and may show errors that do not actually exist
            /// @since C++11
            template<typename T, T... Ts>
            EnableIfType<IsSame<T, char8_t>::Value, U8String<sizeof...(Ts)>> operator""_s() {
                #ifdef __WSTL_NO_INITIALIZERLIST__
                T string[] = {Ts..., '\0'};
                return U8String<sizeof...(Ts)>(string, sizeof...(Ts));
                #else
                return U8String<sizeof...(Ts)>({Ts...});
                #endif
            }
        }
    }
    #endif
    #endif

    /// TODO: To string conversions (and in the future IO operations)
}

#endif