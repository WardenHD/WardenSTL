#ifndef __WSTL_STRING_HPP__
#define __WSTL_STRING_HPP__

#include "private/Platform.hpp"
#include "BasicString.hpp"


namespace wstl {
    // String

    template<size_t N>
    class String : public BasicString<String<N>, char> {
    private:
        typedef BasicString<String<N>, char> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        template<typename InputIterator>
        String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        explicit String(const StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        explicit String(const StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        String(const String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        template<typename Derived>
        String(const BasicString<Derived, ValueType>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        template<typename Derived>
        String(const BasicString<Derived, ValueType>& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other, position, count);
        }

        #ifdef __WSTL_CXX11__
        String(String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        template<typename Derived>
        String(BasicString<Derived, ValueType>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        String(const String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        String(String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        template<typename Derived>
        String(BasicString<Derived, ValueType>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        String& operator=(const String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        template<typename Derived>
        String& operator=(const BasicString<Derived, ValueType>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        String& operator=(String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        template<typename Derived>
        String& operator=(BasicString<Derived, ValueType>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        String& operator=(const StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];

        String(NullPointerType) __WSTL_DELETE__;
        String& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    String(const char (&)[N]) -> String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived>
    struct Hash<BasicString<Derived, char> > {
        size_t operator()(const BasicString<Derived, char>& string) const {
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

    template<size_t N>
    String<N - 1> MakeString(const char (&string)[N]) {
        return String<N - 1>(string, string::Length(string, N - 1));
    }

    template<size_t Capacity, size_t N>
    String<Capacity> MakeStringWithCapacity(const char (&string)[N]) {
        return String<Capacity>(string, string::Length(string, N));
    }

    // Wide string

    template<size_t N>
    class WideString : public BasicString<WideString<N>, wchar_t> {
    private:
        typedef BasicString<WideString<N>, wchar_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        WideString() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        WideString(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        template<typename InputIterator>
        WideString(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        WideString(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        WideString(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        explicit WideString(const WideStringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        explicit WideString(const WideStringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        WideString(const WideString& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        template<typename Derived>
        WideString(const BasicString<Derived, ValueType>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        #ifdef __WSTL_CXX11__
        WideString(WideString&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        template<typename Derived>
        WideString(BasicString<Derived, ValueType>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }
        #endif

        WideString(const WideString& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        #ifdef __WSTL_CXX11__
        WideString(WideString&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        template<typename Derived>
        WideString(BasicString<Derived, ValueType>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        WideString(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif
        #endif

        WideString& operator=(const WideString& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        template<typename Derived>
        WideString& operator=(const BasicString<Derived, ValueType>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        #ifdef __WSTL_CXX11__
        WideString& operator=(WideString&& other) {
            this->Assign(Move(other));
            return *this;
        }

        template<typename Derived>
        WideString& operator=(BasicString<Derived, ValueType>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        WideString& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif
        #endif

        WideString& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        WideString& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        WideString& operator=(const WideStringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];

        WideString(NullPointerType) __WSTL_DELETE__;
        WideString& operator=(NullPointerType) __WSTL_DELETE__;
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    WideString(const char (&)[N]) -> WideString<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived>
    struct Hash<BasicString<Derived, wchar_t> > {
        size_t operator()(const BasicString<Derived, wchar_t>& string) const {
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

    template<size_t N>
    WideString<N - 1> MakeString(const wchar_t (&string)[N]) {
        return WideString<N - 1>(string, string::Length(string, N - 1));
    }

    template<size_t Capacity, size_t N>
    WideString<Capacity> MakeStringWithCapacity(const wchar_t (&string)[N]) {
        return WideString<Capacity>(string, string::Length(string, N));
    }

    // UTF-8 String

    #ifdef __WSTL_CXX20__
    template<size_t N>
    class U8String : public BasicString<U8String<N>, char8_t> {
    private:
        typedef BasicString<U8String<N>, char8_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        U8String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        U8String(NullPointerType) = delete;

        U8String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        template<typename InputIterator>
        U8String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        U8String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        U8String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        explicit U8String(const U8StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        explicit U8String(const U8StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        U8String(const U8String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        template<typename Derived>
        U8String(const BasicString<Derived, ValueType>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        U8String(U8String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        template<typename Derived>
        U8String(BasicString<Derived, ValueType>&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        U8String(const U8String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        U8String(U8String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        template<typename Derived>
        U8String(BasicString<Derived, ValueType>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        U8String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif

        U8String& operator=(const U8String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        U8String& operator=(NullPointerType) = delete;

        template<typename Derived>
        U8String& operator=(const BasicString<Derived, ValueType>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        U8String& operator=(U8String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        template<typename Derived>
        U8String& operator=(BasicString<Derived, ValueType>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        U8String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif

        U8String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        U8String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        U8String& operator=(const U8StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
    };

    // Template deduction guide

    template<size_t N>
    U8String(const char (&)[N]) -> U8String<N - 1>;

    // Hash specialization

    template<typename Derived>
    struct Hash<BasicString<Derived, char8_t> > {
        size_t operator()(const BasicString<Derived, char8_t>& string) const {
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

    template<size_t N>
    U8String<N - 1> MakeString(const char8_t (&string)[N]) {
        return U8String<N - 1>(string, string::Length(string, N - 1));
    }

    template<size_t Capacity, size_t N>
    U8String<Capacity> MakeStringWithCapacity(const char8_t (&string)[N]) {
        return U8String<Capacity>(string, string::Length(string, N));
    }
    #endif

    #ifdef __WSTL_CXX11__
    // UTF-16 string

    template<size_t N>
    class U16String : public BasicString<U16String<N>, char16_t> {
    private:
        typedef BasicString<U16String<N>, char16_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        U16String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        U16String(NullPointerType) = delete;

        U16String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        template<typename InputIterator>
        U16String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        U16String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        U16String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        explicit U16String(const U16StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        explicit U16String(const U16StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        U16String(const U16String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        template<typename Derived>
        U16String(const BasicString<Derived, ValueType>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        U16String(U16String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        U16String(const U16String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        U16String(U16String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        template<typename Derived>
        U16String(BasicString<Derived, ValueType>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        U16String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif

        U16String& operator=(NullPointerType) = delete;

        U16String& operator=(const U16String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        template<typename Derived>
        U16String& operator=(const BasicString<Derived, ValueType>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        U16String& operator=(U16String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        template<typename Derived>
        U16String& operator=(BasicString<Derived, ValueType>&& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        U16String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif

        U16String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        U16String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        U16String& operator=(const U16StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    U16String(const char (&)[N]) -> U16String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived>
    struct Hash<BasicString<Derived, char16_t> > {
        size_t operator()(const BasicString<Derived, char16_t>& string) const {
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

    template<size_t N>
    U16String<N - 1> MakeString(const char16_t (&string)[N]) {
        return U16String<N - 1>(string, string::Length(string, N - 1));
    }

    template<size_t Capacity, size_t N>
    U16String<Capacity> MakeStringWithCapacity(const char16_t (&string)[N]) {
        return U16String<Capacity>(string, string::Length(string, N));
    }

    // UTF-32 String

    template<size_t N>
    class U32String : public BasicString<U32String<N>, char32_t> {
    private:
        typedef BasicString<U32String<N>, char32_t> Base;
        
    public:
        typedef typename Base::ValueType ValueType;
        typedef typename Base::SizeType SizeType;

        U32String() : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {}

        U32String(NullPointerType) = delete;

        U32String(SizeType count, ValueType value) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(count, value);
        }

        template<typename InputIterator>
        U32String(InputIterator first, InputIterator last, typename EnableIf<!IsIntegral<InputIterator>::Value, int>::Type = 0) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(first, last);
        }

        U32String(const ValueType* string, SizeType count) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string, count);
        }

        U32String(const ValueType* string) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(string);
        }

        explicit U32String(const U32StringView& view) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view);
        }

        explicit U32String(const U32StringView& view, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(view.Substring(position, count));
        }

        U32String(const U32String& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        template<typename Derived>
        U32String(const BasicString<Derived, ValueType>& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other);
        }

        U32String(U32String&& other) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other));
        }

        U32String(const U32String& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(other.Substring(position, count));
        }

        U32String(U32String&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        template<typename Derived>
        U32String(BasicString<Derived, ValueType>&& other, SizeType position, SizeType count = Base::NoPosition) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(Move(other.Substring(position, count)));
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        U32String(InitializerList<ValueType> list) : Base(reinterpret_cast<ValueType*>(&m_Buffer), N) {
            this->Assign(list);
        }
        #endif

        U32String& operator=(const U32String& other) {
            if(&other != this) this->Assign(other);
            return *this;
        }

        U32String& operator=(NullPointerType) = delete;

        template<typename Derived>
        U32String& operator=(const BasicString<Derived, ValueType>& other) {
            if(static_cast<const void*>(&other) != static_cast<void*>(this)) this->Assign(other);
            return *this;
        }

        U32String& operator=(U32String&& other) {
            this->Assign(Move(other));
            return *this;
        }

        template<typename Derived>
        U32String& operator=(BasicString<Derived, ValueType>&& other) {
            this->Assign(Move(other));
            return *this;
        }

        #ifndef __WSTL_NO_INITIALIZERLIST__
        U32String& operator=(InitializerList<ValueType> list) {
            this->Assign(list);
            return *this;
        }
        #endif

        U32String& operator=(const ValueType* string) {
            this->Assign(string);
            return *this;
        }

        U32String& operator=(ValueType value) {
            this->Assign(1, value);
            return *this;
        }

        U32String& operator=(const U32StringView& view) {
            this->Assign(view);
            return *this;
        }

    private:
        char m_Buffer[N + 1];
    };

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<size_t N>
    U32String(const char (&)[N]) -> U32String<N - 1>;
    #endif

    // Hash specialization

    template<typename Derived>
    struct Hash<BasicString<Derived, char32_t> > {
        size_t operator()(const BasicString<Derived, char32_t>& string) const {
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

    template<size_t N>
    U32String<N - 1> MakeString(const char32_t (&string)[N]) {
        return U32String<N - 1>(string, string::Length(string, N - 1));
    }

    template<size_t Capacity, size_t N>
    U32String<Capacity> MakeStringWithCapacity(const char32_t (&string)[N]) {
        return U32String<Capacity>(string, string::Length(string, N));
    }
    #endif

    /// TODO: To string conversions (and in the future IO operations)
}

#endif