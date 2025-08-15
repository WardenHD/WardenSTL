// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_CHECKSUM_HPP__
#define __WSTL_CHECKSUM_HPP__

#include "private/Platform.hpp"
#include "HasherBase.hpp"
#include "Bit.hpp"
#include <stdint.h>


/// @defgroup checksum Checksum
/// A set of checksum calculation templates
/// @ingroup maths

namespace wstl {
    // Standard checksum

    /// @brief A simple checksum class that calculates the sum of all elements
    /// @tparam T The type of the checksum value
    /// @ingroup checksum
    template<typename T>
    class Checksum : public HasherBase<Checksum<T>, T, uint8_t> {
    private:
        typedef HasherBase<Checksum<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ Checksum() {
            Reset();
        }

        /// @brief Constructor that initializes the checksum with a range of elements
        /// @param first The beginning of the range
        /// @param last The end of the range
        template<typename Iterator>
        __WSTL_CONSTEXPR14__ Checksum(Iterator first, Iterator last) {
            StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
            Reset();
            this->Append(first, last);
        }

        /// @brief Resets the checksum to its initial state
        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = 0;
        }

        /// @brief Pushes a value into the checksum
        /// @param value The value to be added to the checksum
        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash += value;
        }

    private:
        friend Base;

        /// @brief Finalizes the checksum calculation, does nothing
        __WSTL_CONSTEXPR14__ void Finalize() {}
    };

    // BSD checksum

    /// @brief BSD checksum implementation
    /// @tparam T The type of the checksum value
    /// @ingroup checksum
    template<typename T>
    class BSDChecksum : public HasherBase<BSDChecksum<T>, T, uint8_t> {
    private:
        typedef HasherBase<BSDChecksum<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ BSDChecksum() {
            Reset();
        }

        /// @brief Constructor that initializes the checksum with a range of elements
        /// @param first The beginning of the range
        /// @param last The end of the range
        template<typename Iterator>
        __WSTL_CONSTEXPR14__ BSDChecksum(Iterator first, Iterator last) {
            StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
            Reset();
            this->Append(first, last);
        }

        /// @brief Resets the checksum to its initial state
        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = 0;
        }

        /// @brief Pushes a value into the checksum
        /// @param value The value to be added to the checksum
        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash = RotateRight(this->m_Hash, 1) + value;
        }

    private:
        friend Base;

        /// @brief Finalizes the checksum calculation, does nothing
        __WSTL_CONSTEXPR14__ void Finalize() {}
    };

    // XOR checksum

    /// @brief XOR checksum implementation
    /// @tparam T The type of the checksum value
    /// @ingroup checksum
    template<typename T>
    class XORChecksum : public HasherBase<XORChecksum<T>, T, uint8_t> {
    private:
        typedef HasherBase<XORChecksum<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ XORChecksum() {
            Reset();
        }

        /// @brief Constructor that initializes the checksum with a range of elements
        /// @param first The beginning of the range
        /// @param last The end of the range
        template<typename Iterator>
        __WSTL_CONSTEXPR14__ XORChecksum(Iterator first, Iterator last) {
            StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
            Reset();
            this->Append(first, last);
        }

        /// @brief Resets the checksum to its initial state
        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = 0;
        }

        /// @brief Pushes a value into the checksum
        /// @param value The value to be added to the checksum
        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash ^= value;
        }

    private:
        friend Base;

        /// @brief Finalizes the checksum calculation, does nothing
        __WSTL_CONSTEXPR14__ void Finalize() {}
    };

    // XOR rotate checksum

    /// @brief XOR rotate checksum implementation
    /// @tparam T The type of the checksum value
    /// @ingroup checksum
    template<typename T>
    class XORRotateChecksum : public HasherBase<XORRotateChecksum<T>, T, uint8_t> {
    private:
        typedef HasherBase<XORRotateChecksum<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ XORRotateChecksum() {
            Reset();
        }

        /// @brief Constructor that initializes the checksum with a range of elements
        /// @param first The beginning of the range
        /// @param last The end of the range
        template<typename Iterator>
        __WSTL_CONSTEXPR14__ XORRotateChecksum(Iterator first, Iterator last) {
            StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
            Reset();
            this->Append(first, last);
        }

        /// @brief Resets the checksum to its initial state
        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = 0;
        }

        /// @brief Pushes a value into the checksum
        /// @param value The value to be added to the checksum
        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash = RotateLeft(this->m_Hash, 1) ^ value;
        }

    private:
        friend Base;

        /// @brief Finalizes the checksum calculation, does nothing
        __WSTL_CONSTEXPR14__ void Finalize() {}
    };

    // Parity checksum

    /// @brief Parity checksum implementation
    /// @tparam T The type of the checksum value
    /// @ingroup checksum
    template<typename T>
    class ParityChecksum : public HasherBase<ParityChecksum<T>, T, uint8_t> {
    private:
        typedef HasherBase<ParityChecksum<T>, T, uint8_t> Base;

    public:
        typedef typename Base::HashType HashType;
        typedef typename Base::ValueType ValueType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ ParityChecksum() {
            Reset();
        }

        /// @brief Constructor that initializes the checksum with a range of elements
        /// @param first The beginning of the range
        /// @param last The end of the range
        template<typename Iterator>
        __WSTL_CONSTEXPR14__ ParityChecksum(Iterator first, Iterator last) {
            StaticAssert(sizeof(typename IteratorTraits<Iterator>::ValueType) == sizeof(ValueType), "Type not supported");
            Reset();
            this->Append(first, last);
        }

        /// @brief Resets the checksum to its initial state
        __WSTL_CONSTEXPR14__ void Reset() {
            this->m_Hash = 0;
        }

        /// @brief Pushes a value into the checksum
        /// @param value The value to be added to the checksum
        __WSTL_CONSTEXPR14__ void PushBack(ValueType value) {
            this->m_Hash = this->m_Hash ^ Parity(value);
        }

    private:
        friend Base;

        /// @brief Finalizes the checksum calculation, does nothing
        __WSTL_CONSTEXPR14__ void Finalize() {}
    };
}

#endif