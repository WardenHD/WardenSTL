#ifndef __WSTL_MEMORY_HPP__
#define __WSTL_MEMORY_HPP__

#include "private/Platform.hpp"
// #include "Utility.hpp"
#include "NullPointer.hpp"
#include "TypeTraits.hpp"
// #include "Algorithm.hpp"


namespace wstl {
    template<class T>
    __WSTL_CONSTEXPR17__ typename EnableIf<!IsSame<T, NullPointerType>::Value, T>::Type* AddressOf(T& object) __WSTL_NOEXCEPT__ {
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(object)));
    }

    template<class T>
    __WSTL_CONSTEXPR17__ const T* AddressOf(const T&&) __WSTL_DELETE__;

//     #ifdef __WSTL_CXX11__
//     template<class Pointer>
//     class PointerTraits {
//     public:
//         using PointerType = Pointer;
//         using ElementType = typename Pointer::ElementType;
//         using DifferenceType = Conditional<!IsSame<typename Pointer::ElementType, NullPointerType>::Value, 
//             typename Pointer::ElementType, ptrdiff_t>;

//         template<class U>
//         using Rebind = typename Pointer::Rebind<U>;

//         static __WSTL_CONSTEXPR20__ PointerType PointerTo(ElementType& reference) {
//             return Pointer::PointerTo(reference);
//         }
//     };

//     template<class T>
//     class PointerTraits<T*> {
//     public:
//         using PointerType = T*;
//         using ElementType = T;
//         using DifferenceType = ptrdiff_t;

//         template<class U>
//         using Rebind = U*;

//         static __WSTL_CONSTEXPR20__ PointerType PointerTo(ElementType& reference) __WSTL_NOEXCEPT__ {
//             return AddressOf(reference);
//         }
//     };
//     #endif

//     template<class T>
//     class DefaultDelete {
//     public:
//         __WSTL_CONSTEXPR__ DefaultDelete() __WSTL_NOEXCEPT__ __WSTL_DELETE__;

//         template<class U>
//         __WSTL_CONSTEXPR__ DefaultDelete(const DefaultDelete<U>&) __WSTL_NOEXCEPT__ {}

//         void operator()(T* pointer) const {
//             delete pointer;
//         }
//     };

//     template<class T>
//     class DefaultDelete<T[]> {
//     public:
//         __WSTL_CONSTEXPR__ DefaultDelete() __WSTL_NOEXCEPT__ __WSTL_DELETE__;

//         template<class U>
//         __WSTL_CONSTEXPR__ DefaultDelete(const DefaultDelete<U[]>&) __WSTL_NOEXCEPT__ {}

//         template<class U>
//         void operator()(U* pointer) const {
//             delete[] pointer;
//         }
//     };

//     template<class T, class Deleter> 
//     class UniquePointer;

//     template<class T, class Deleter>
//     class UniquePointer<T, DefaultDelete<T> > {
//     public:
//         typedef Deleter DeleterType;
//         typedef T ElementType;
//         typedef T* Pointer;
//         typedef T& Reference;

//         __WSTL_CONSTEXPR__ UniquePointer() : m_Pointer(nullptr) __WSTL_NOEXCEPT__ {}
//         __WSTL_CONSTEXPR__ explicit UniquePointer(Pointer pointer) : m_Pointer(pointer) __WSTL_NOEXCEPT__ {}

//         #ifdef __WSTL_CXX11__
//         UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 m_Pointer = other.Release();
//                 m_Deleter = Move(other.m_Deleter);
//             }
//         }
//         #else
//         UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 m_Pointer = other.Release();
//                 m_Deleter = other.m_Deleter;
//             }
//         }
//         #endif

//         UniquePointer(Pointer pointer, typename Conditional<IsReference<Deleter>::Value, 
//             Deleter, typename AddLValueReference<const Deleter>::Type>::Type deleter) __WSTL_NOEXCEPT__
//             : m_Pointer(pointer), m_Deleter(deleter) {}

//         #ifdef __WSTL_CXX11__
//         UniquePointer(Pointer pointer, typename RemoveReference<Deleter>::Type&& deleter) __WSTL_NOEXCEPT__ 
//             : m_Pointer(pointer), m_Deleter(Move(deleter)) {}

//         template<typename U, typename E>
//         UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ 
//             : m_Pointer(other.Release()), m_Deleter(Forward<E>(other.m_Deleter)) {}
//         #endif

//         ~UniquePointer() {
//             if(m_Pointer != nullptr) deleter(m_Pointer);
//         }

//         Pointer Release() __WSTL_NOEXCEPT__ {
//             Pointer value = m_Pointer;
//             m_Pointer = nullptr;    
//             return value;
//         }

//         void Reset(Pointer pointer = Pointer()) __WSTL_NOEXCEPT__ {
//             if(m_Pointer == nullptr || pointer != m_Pointer) {
//                 Pointer value = m_Pointer;
//                 m_Pointer = pointer;

//                 if(value != nullptr) m_Deleter(value);
//             }
//         }

//         void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
//             wstl::Swap(m_Pointer, other.m_Pointer);
//         }

//         __WSTL_CONSTEXPR__ Pointer Get() const __WSTL_NOEXCEPT__ {
//             return m_Pointer;
//         }

//         Deleter& GetDeleter() __WSTL_NOEXCEPT__ {
//             return m_Deleter;
//         }

//         const Deleter& GetDeleter() const __WSTL_NOEXCEPT__ {
//             return m_Deleter;
//         }

//         explicit operator bool() const __WSTL_NOEXCEPT__ {
//             return m_Pointer != nullptr;
//         }

//         #ifdef __WSTL_CXX11__
//         UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 Reset(other.Release());
//                 m_Deleter = Move(other.m_Deleter);
//             }

//             return *this;
//         }

//         template<class U, class E>
//         UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 Reset(nullptr);
//                 m_Pointer = other.Release();
//                 m_Deleter = Forward<E>(other.m_Deleter);
//             }
                
//             return *this;
//         }
//         #else
//         UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 Reset(other.Release());
//                 m_Deleter = other.m_Deleter;
//             }

//             return *this;
//         }
//         #endif

//         UniquePointer& operator=(nullptr_t) __WSTL_NOEXCEPT__ {
//             if(m_Pointer) Reset(__WSTL_NULLPTR__);
//             return *this;
//         }

//         __WSTL_CONSTEXPR__ Pointer operator*() const __WSTL_NOEXCEPT__ {
//             return *Get();
//         }

//         __WSTL_CONSTEXPR__ Pointer operator->() const __WSTL_NOEXCEPT__ {
//             return Get();
//         }

//         __WSTL_CONSTEXPR__ Reference operator[](size_t i) const {
//             return Get()[i];
//         }

//     private:
//         Pointer m_Pointer;
//         Deleter m_Deleter;

//         UniquePointer(const UniquePointer&) __WSTL_DELETE__;
//         UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
//     };

//     template<class T, class Deleter>
//     class UniquePointer<T[], Deleter> {
//     public:
//         typedef T ElementType;
//         typedef T* Pointer;
//         typedef T& Reference;

//         __WSTL_CONSTEXPR__ UniquePointer() : m_Pointer(__WSTL_NULLPTR__) __WSTL_NOEXCEPT__ {}
//         __WSTL_CONSTEXPR__ UniquePointer(Pointer pointer) : m_Pointer(pointer) __WSTL_NOEXCEPT__ {}

//         #ifdef __WSTL_CXX11__
//         UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 m_Pointer = other.Release();
//                 m_Deleter = Move(other.m_Deleter);
//             }
//         }
//         #else
//         UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 m_Pointer = other.Release();
//                 m_Deleter = other.m_Deleter;
//             }
//         }
//         #endif

//         UniquePointer(Pointer pointer, typename Conditional<IsReference<Deleter>::Value, 
//             Deleter, typename AddLValueReference<const Deleter>::Type>::Type deleter) __WSTL_NOEXCEPT__
//             : m_Pointer(pointer), m_Deleter(deleter) {}

//         #ifdef __WSTL_CXX11__
//         UniquePointer(Pointer pointer, typename RemoveReference<Deleter>::Type&& deleter) __WSTL_NOEXCEPT__ 
//             : m_Pointer(pointer), m_Deleter(deleter) {}

//         template<typename U, typename E>
//         UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ 
//             : m_Pointer(other.Release()), m_Deleter(Forward<E>(other.m_Deleter)) {}
//         #endif

//         ~UniquePointer() {
//             if(m_Pointer != __WSTL_NULLPTR__) deleter(m_Pointer);
//         }

//         Pointer Release() __WSTL_NOEXCEPT__ {
//             Pointer value = m_Pointer;
//             m_Pointer = nullptr;    
//             return value;
//         }

//         template<class U>
//         void Reset(U pointer) __WSTL_NOEXCEPT__ {
//             if(pointer != m_Pointer) {
//                 Pointer value = m_Pointer;
//                 m_Pointer = pointer;

//                 if(value != nullptr) m_Deleter(value);
//             }
//         }

//         void Reset(nullptr_t = __WSTL_NULLPTR__) __WSTL_NOEXCEPT__ {
//             Reset(Pointer());
//         }

//         void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
//             wstl::Swap(m_Pointer, other.m_Pointer);
//         }

//         __WSTL_CONSTEXPR__ Pointer Get() const __WSTL_NOEXCEPT__ {
//             return m_Pointer;
//         }

//         Deleter& GetDeleter() __WSTL_NOEXCEPT__ {
//             return m_Deleter;
//         }

//         const Deleter& GetDeleter() const __WSTL_NOEXCEPT__ {
//             return m_Deleter;
//         }

//         explicit operator bool() const __WSTL_NOEXCEPT__ {
//             return m_Pointer != nullptr;
//         }

//         #ifdef __WSTL_CXX11__
//         UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 Reset(other.Release());
//                 m_Deleter = Move(other.m_Deleter);
//             }

//             return *this;
//         }

//         template<class U, class E>
//         UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 Reset(nullptr);
//                 m_Pointer = other.Release();
//                 m_Deleter = Forward<E>(other.m_Deleter);
//             }
                
//             return *this;
//         }
//         #else
//         UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
//             if(&other != this) {
//                 Reset(other.Release());
//                 m_Deleter = other.m_Deleter;
//             }

//             return *this;
//         }
//         #endif

//         UniquePointer& operator=(nullptr_t) __WSTL_NOEXCEPT__ {
//             Reset(nullptr);
//             return *this;
//         }

//         __WSTL_CONSTEXPR__ Pointer operator*() const __WSTL_NOEXCEPT__ {
//             return *m_Pointer;
//         }

//         __WSTL_CONSTEXPR__ Pointer operator->() const __WSTL_NOEXCEPT__ {
//             return m_Pointer;
//         }

//         __WSTL_CONSTEXPR__ Reference operator[](size_t i) const {
//             return m_Pointer[i];
//         }

//     private:
//         Pointer m_Pointer;
//         Deleter m_Deleter;

//         UniquePointer(const UniquePointer&) __WSTL_DELETE__;
//         UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
//     };

//     template<class T>
//     class WeakPointer {
//     public:
//         typedef T ElementType;
//         typedef T* Pointer;
//         typedef T& Reference;

//         __WSTL_CONSTEXPR__ WeakPointer() __WSTL_NOEXCEPT__ : m_Pointer(nullptr) {}
//         __WSTL_CONSTEXPR__ WeakPointer(const WeakPointer& reference) : m_Pointer(reference.m_Pointer) {}



//     private:
//         Pointer m_Pointer;
//     };

//     /// Uninitialized copy

//     template<class InputIterator, class OutputIterator>
//     typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
//     UninitializedCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
//         typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
//         OutputIterator result = resultFirst;

//         for(; first != last; first++, result++)
//             ::new(static_cast<void*>(AddressOf(*result))) ValueType(*first);
        
//         return resultFirst;
//     }

//     template<class InputIterator, class OutputIterator>
//     typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
//     UninitializedCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
//         return Copy(first, last, resultFirst);
//     }

//     /// Uninitialized copy in range

//     template<class InputIterator, class Size, class OutputIterator>
//     OutputIterator UninitializedCopyInRange(InputIterator first, Size count, OutputIterator resultFirst) {
//         return UninitializedCopy(first, first + count, resultFirst);
//     }

//     /// Uninitialized fill

//     template<class InputIterator, class T>
//     typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<InputIterator>::ValueType>::Value, InputIterator>::Type 
//     inline UninitializedFill(InputIterator first, InputIterator last, const T& value) {
//         Fill(first, last, value);
//         return last;
//     }

//     template<class OutputIterator, class T>
//     typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
//     UninitializedFill(OutputIterator first, OutputIterator last, const T& value) {
//         typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
//         for(; first != last; first++) 
//             ::new(static_cast<void*>(AddressOf(*first))) ValueType(value);
        
//         return last;
//     }

//     /// Uninitialized fill in range

//     template<class OutputIterator, class Size, class T>
//     inline OutputIterator UninitializedFillInRange(OutputIterator first, Size count, const T& value) {
//         return UninitializedFill(first, first + count, value);
//     }

//     /// Unitialized move

//     #ifdef __WSTL_CXX11__
//     template<class InputIterator, class OutputIterator>
//     typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
//     UninitializedMove(InputIterator first, InputIterator last, OutputIterator resultFirst) {
//         return Move(first, last, resultFirst);
//     }

//     template<class InputIterator, class OutputIterator>
//     typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
//     UninitializedMove(InputIterator first, InputIterator last, OutputIterator resultFirst) {
//         typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
//         OutputIterator result = resultFirst;

//         for(; first != last; first++, result++) 
//             ::new(static_cast<void*>(AddressOf(*result))) ValueType(Move(*first));

//         return resultFirst;
//     }  

//     /// Uninitialized move in range

//     template<class InputIterator, class Size, class OutputIterator>
//     inline OutputIterator UninitializedMoveInRange(InputIterator first, Size count, OutputIterator resultFirst) {
//         return UninitializedMove(first, first + count, resultFirst);
//     }
//     #endif

//     /// Uninitialized default construct

//     template<class OutputIterator>
//     typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
//     UninitializedDefaultConstruct(OutputIterator, OutputIterator) {
//         /// Do nothing
//     }

//     template<class OutputIterator>
//     typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
//     UninitializedDefaultConstruct(OutputIterator first, OutputIterator last) {
//         typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;

//         for(; first != last; first++) 
//             ::new(static_cast<void*>(AddressOf(*first))) ValueType;
//     }

//     /// Uninitialized default construct in range 

//     template<class OutputIterator, class Size>
//     inline OutputIterator UninitializedDefaultConstructInRange(OutputIterator first, Size count) {
//         OutputIterator result = first + count;
//         UninitializedDefaultConstruct(first, result);
//         return result;
//     }

//     /// Uninitialized value construct

//     template<class OutputIterator>
//     typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
//     UninitializedValueConstruct(OutputIterator first, OutputIterator last) {
//         typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;

//         Fill(first, last, ValueType());
//     }

//     template<class OutputIterator>
//     typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
//     UninitializedValueConstruct(OutputIterator first, OutputIterator last) {
//         typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;

//         for(; first != last; first++) 
//             ::new(static_cast<void*>(AddressOf(*first))) ValueType();
//     }

//     /// Uninitialized value construct in range

//     template<class OutputIterator, class Size>
//     inline OutputIterator UninitializedValueConstructInRange(OutputIterator first, Size count) {
//         OutputIterator result = first + count;
//         UninitializedValueConstruct(first, result);
//         return result;
//     }

//     /// Construct at

//     #ifdef __WSTL_CXX11__
//     template<class T, class... Args>
//     T* ConstructAt(T* pointer, Args&&... args) {
//         return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(Forward<Args>(args)...);
//     }
//     #else
//     template<class T>
//     T* ConstructAt(T* pointer) {
//         return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T();
//     }

//     template<class T, class Arg>
//     T* ConstructAt(T* pointer, const Arg& arg) {
//         return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(arg);
//     }
//     #endif
    
//     /// Destroy at

//     template<class T>
//     typename EnableIf<IsTriviallyDestructible<T>::Value, void>::Type DestroyAt(T*) {}

//     template<class T>
//     typename EnableIf<!IsTriviallyDestructible<T>::Value, void>::Type DestroyAt(T* pointer) {
//         pointer->~T();
//     }

//     /// Destroy

//     template<class Iterator>
//     typename EnableIf<IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, void>::Type 
//     Destroy(Iterator, Iterator) {}

//     template<class Iterator>
//     typename EnableIf<IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, void>::Type 
//     Destroy(Iterator first, Iterator last) {
//         for(; first != last; first++) DestroyAt(AddressOf(*first));
//     }

//     /// Destroy in range

//     template<class Iterator, class Size>
//     inline Iterator DestroyInRange(Iterator first, Size count) {
//         Iterator result = first + count;
//         Destroy(first, result);
//         return result;
//     }

//     /// Memory namespace

//     namespace memory {
//         /// Wrappers for functions (if there are user-defined ones)

//         template<class Pointer>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Copy(const Pointer sourceFirst, const Pointer sourceLast, const Pointer destinationFirst) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memcpy(reinterpret_cast<void*>(destinationFirst),
//                 reinterpret_cast<void*>(sourceFirst), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * static_cast<size_t>(sourceLast - sourceFirst)));
//         }

//         template<class Pointer>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Copy(const Pointer sourceFirst, size_t count, const Pointer destinationFirst) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memcpy(reinterpret_cast<void*>(destinationFirst),
//                 reinterpret_cast<void*>(sourceFirst), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * count));
//         }

//         template<class Pointer, class T>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Set(Pointer destinationFirst, const Pointer destinationLast, T value) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memset(reinterpret_cast<void*>(destinationFirst),
//             static_cast<char>(value), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * static_cast<size_t>(destinationLast - destinationFirst)));
//         }

//         template<class Pointer, class T>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Set(Pointer destinationFirst, size_t count, T value) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memset(reinterpret_cast<void*>(destinationFirst),
//             static_cast<char>(value), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * count));
//         }

//         template<class Pointer>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Move(const Pointer sourceFirst, const Pointer sourceLast, Pointer destinationFirst) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memmove(reinterpret_cast<void*>(destinationFirst),
//             reinterpret_cast<void*>(sourceFirst), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * static_cast<size_t>(sourceLast - sourceFirst)));
//         }

//         template<class Pointer>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Move(const Pointer sourceFirst, size_t count, Pointer destinationFirst) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memmove(reinterpret_cast<void*>(destinationFirst),
//             reinterpret_cast<void*>(sourceFirst), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * count));
//         }

//         template<class Pointer>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Compare(const Pointer sourceFirst, const Pointer sourceLast, Pointer destinationFirst) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memcmp(reinterpret_cast<void*>(destinationFirst),
//             reinterpret_cast<void*>(sourceFirst), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * static_cast<size_t>(sourceLast - sourceFirst)));
//         }

//         template<class Pointer>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, Pointer>::Type 
//         Compare(const Pointer sourceFirst, size_t count, Pointer destinationFirst) __WSTL_NOEXCEPT__ {
//             return reinterpret_cast<Pointer>(memcmp(reinterpret_cast<void*>(destinationFirst),
//             reinterpret_cast<void*>(sourceFirst), sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * count));
//         }

//         template<class Pointer, class T>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, char*>::Type 
//         Locate(Pointer sourceFirst, const Pointer sourceLast, T value) __WSTL_NOEXCEPT__ {\
//             void* result = memchr(reinterpret_cast<void*>(sourceFirst), static_cast<char>(value), 
//                 sizeof(typename IteratorTraits<Pointer>::ValueType) 
//                 * static_cast<size_t>(sourceLast - sourceFirst));
            
//             return reinterpret_cast<char*>((result == 0) ? sourceLast : result);
//         }

//         template<class Pointer, class T>
//         typename EnableIf<IsTriviallyCopyable<typename IteratorTraits<Pointer>::ValueType>::Value, char*>::Type 
//         Locate(Pointer sourceFirst, size_t count, T value) __WSTL_NOEXCEPT__ {
//             void* result = memchr(reinterpret_cast<void*>(sourceFirst), static_cast<char>(value), 
//                 sizeof(typename IteratorTraits<Pointer>::ValueType) * count);
            
//             return reinterpret_cast<char*>((result == 0) ? sourceLast : result);
//         }
//     }
}

// /// Operators for UniquePointer

// template<class T1, class T2, class TD1, class TD2>
// bool operator==(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
//     return a.Get() == b.Get();
// }

// template<class T1, class T2, class TD1, class TD2>
// bool operator!=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
//     return !(a == b);
// }

// template<class T1, class T2, class TD1, class TD2>
// bool operator<(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
//     return reinterpret_cast<char*>(a.Get()) < reinterpret_cast<char*>(b.Get());
// }

// template<class T1, class T2, class TD1, class TD2>
// bool operator<=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
//     return !(b < a);
// }

// template<class T1, class T2, class TD1, class TD2>
// bool operator>(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
//     return b < a;
// }

// template<class T1, class T2, class TD1, class TD2>
// bool operator>(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
//     return !(a < b);
// }


#endif