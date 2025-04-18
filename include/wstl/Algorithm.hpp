// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_ALGORITHM_HPP__
#define __WSTL_ALGORITHM_HPP__

#include "Iterator.hpp"
#include "Utility.hpp"
#include "Functional.hpp"
#include "InitializerList.hpp"


/// @defgroup algorithm Algorithm
/// @brief Algorithms that operate on ranges of elements
/// @ingroup wstl

namespace wstl {
    // Find

    /// @brief Finds the first element in a range that is equal to a specific value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to find
    /// @return Iterator that points to the found element, or last if not found
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find
    template<typename InputIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    InputIterator Find(InputIterator first, InputIterator last, const T& value) {
        for(; first != last; first++) if(*first == value) return first;
        return last;
    }

    // Find if

    /// @brief Finds the first element in a range that satisfies a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator that points to the found element, or last if not found
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find
    template<typename InputIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    InputIterator FindIf(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        for(; first != last; first++) 
            if(predicate(*first)) return first;
        return last;
    }

    // Find if not

    /// @brief Finds the first element in a range that does not satisfy a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator that points to the found element, or last if not found
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find
    template<typename InputIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    InputIterator FindIfNot(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        for(; first != last; first++) if(!predicate(*first)) return first;
        return last;
    }

    // All of

    /// @brief Checks if all elements in a range satisfy a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return True if all elements satisfy the predicate, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
    template<typename InputIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    bool AllOf(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        return FindIfNot(first, last, predicate) == last;
    }

    // Any of
    
    /// @brief Checks if at least one element in a range satisfies a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return True if at least one element satisfies the predicate, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
    template<typename InputIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__  
    bool AnyOf(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        return FindIf(first, last, predicate) != last;
    }

    // None of

    /// @brief Checks if no elements in a range satisfy a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return True if no elements satisfy the predicate, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
    template<typename InputIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    bool NoneOf(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        return FindIf(first, last, predicate) == last;
    }

    // For each

    /// @brief Applies a function to each element in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param function Unary function that takes an element as an argument
    /// @return The function object
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/for_each
    template<typename InputIterator, typename Function>
    __WSTL_CONSTEXPR14__ Function ForEach(InputIterator first, InputIterator last, Function function) {
        for(; first != last; first++) function(*first);
        return function;
    }

    // For each in range

    /// @brief Applies a function to first N element in a range
    /// @param first Iterator to the beginning of the range
    /// @param count Number of elements to apply the function to
    /// @param function Unary function that takes an element as an argument
    /// @return The function object
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/for_each_n
    template<typename InputIterator, typename Size, typename Function>
    inline __WSTL_CONSTEXPR14__ InputIterator ForEachInRange(InputIterator first, Size count, Function function) {
        for(Size i = 0; i < count; i++, first++) function(*first);
        return first;
    }
    
    // Count

    /// @brief Returns the number of elements in a range that are equal to a specific value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to count
    /// @return The number of elements equal to the value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/count
    template<typename InputIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    typename IteratorTraits<InputIterator>::DifferenceType Count(InputIterator first, InputIterator last, const T& value) {
        typename IteratorTraits<InputIterator>::DifferenceType count = 0;
        for(; first != last; first++) if(*first == value) count++;
        return count;
    }

    // Count if

    /// @brief Returns the number of elements in a range that satisfy a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Predicate to check
    /// @return The number of elements that satisfy the predicate
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/count
    template<typename InputIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    typename IteratorTraits<InputIterator>::DifferenceType CountIf(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        typename IteratorTraits<InputIterator>::DifferenceType count = 0;
        for(; first != last; first++) if(predicate(*first)) count++;
        return count;
    }

    // Mismatch
    
    /// @brief Returns a pair of iterators to the first element that is not equal between two ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @return Pair of iterators to the first mismatching elements 
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/mismatch
    template<typename InputIterator1, typename InputIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    Pair<InputIterator1, InputIterator2> Mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        while(first1 != last1 && *first1 == *first2) first1++, first2++;
        return MakePair(first1, first2);
    }

    /// @brief Returns a pair of iterators to the first element that does not satisfy the predicate between two ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range 
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Pair of iterators to the first mismatching elements
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/mismatch
    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    Pair<InputIterator1, InputIterator2> Mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate predicate) {
        while(first1 != last1 && predicate(*first1, *first2)) first1++, first2++;
        return MakePair(first1, first2);
    }

    /// @brief Returns a pair of iterators to the first element that is not equal between two ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range 
    /// @return Pair of iterators to the first mismatching elements
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/mismatch
    template<typename InputIterator1, typename InputIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    Pair<InputIterator1, InputIterator2> Mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        while(first1 != last1 && first2 != last2 && *first1 == *first2) first1++, first2++;
        return MakePair(first1, first2);
    }

    /// @brief Returns a pair of iterators to the first element that does not satisfy the predicate between two ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Pair of iterators to the first mismatching elements
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/mismatch
    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    Pair<InputIterator1, InputIterator2> Mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate predicate) {
        while(first1 != last1 && first2 != last2 && predicate(*first1, *first2)) first1++, first2++;
        return MakePair(first1, first2);
    }

    // Search

    /// @brief Searches for the first occurence of the sequence in a range
    /// @param first Iterator to the beginning of the first range
    /// @param last Iterator to the end of the first range
    /// @param sequenceFirst Iterator to the beginning of the sequence
    /// @param sequenceLast Iterator to the end of the sequence
    /// @return Iterator to the first occurence of the sequence
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/search
    template<typename ForwardIterator1, typename ForwardIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator1 Search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sequenceFirst, ForwardIterator2 sequenceLast) {
        for(; first != last; first++) {
            ForwardIterator1 it = first;
            for(ForwardIterator2 seq = sequenceFirst; ; it++, seq++) {
                if(seq == sequenceLast) return first;
                if(it == last) return last;
                if(!(*it == *seq)) break;
            }
        }
        
        return last;
    }

    /// @brief Searches for the first occurence of the sequence in a range using predicate
    /// @param first Iterator to the beginning of the first range
    /// @param last Iterator to the end of the first range
    /// @param sequenceFirst Iterator to the beginning of the sequence
    /// @param sequenceLast Iterator to the end of the sequence
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Iterator to the first occurence of the sequence
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/search
    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator1 Search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sequenceFirst, ForwardIterator2 sequenceLast, BinaryPredicate predicate) {
        for(; first != last; first++) {
            ForwardIterator1 it = first;
            for(ForwardIterator2 seq = sequenceFirst; ; it++, seq++) {
                if(seq == sequenceLast) return first;
                if(it == last) return last;
                if(!predicate(*it, *seq)) break;
            }
        }

        return last;
    }

    // Search in range

    /// @brief Searches for the first occurence of N consecutive copies of an element in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param count Number of consecutive copies to search for
    /// @param value Element to search for
    /// @return Iterator to the first occurence of N consecutive copies of the element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/search_n
    template<typename ForwardIterator, typename Size, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator SearchInRange(ForwardIterator first, ForwardIterator last, Size count, const T& value) {
        if(count <= 0) return first;

        ForwardIterator end = first;
        Advance(end, Distance(first, last) - count + 1);

        for(; first != end; first++) {
            if(!(*first == value)) continue;
            
            ForwardIterator candidate = first;
            ForwardIterator it = first;
            Size i = 1;
            for(++it; i < count; i++, it++) if(it == last || !(*it == value)) break;

            if(i == count) return candidate;
        }

        return last;
    }
    
    /// @brief Searches for the first occurence of N consecutive copies of an element in a range using predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param count Number of consecutive copies to search for
    /// @param value Element to search for
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Iterator to the first occurence of N consecutive copies of the element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/search_n
    template<typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator SearchInRange(ForwardIterator first, ForwardIterator last, Size count, const T& value, BinaryPredicate predicate) {
        if(count <= 0) return first;

        ForwardIterator end = first;
        Advance(end, Distance(first, last) - count + 1);

        for(; first != end; first++) {
            if(!predicate(*first, value)) continue;
            
            ForwardIterator candidate = first;
            ForwardIterator it = first;
            Size i = 1;
            for(++it; i < count; i++, it++) if(it == last || !predicate(*it, value)) break;

            if(i == count) return candidate;
        }

        return last;
    }

    // Find end

    /// @brief Searches for the last occurence of the sequence in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param sequenceFirst Iterator to the beginning of the sequence
    /// @param sequenceLast Iterator to the end of the sequence
    /// @return Iterator to the last occurence of the sequence
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find_end
    template<typename ForwardIterator1, typename ForwardIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator1 FindEnd(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sequenceFirst, ForwardIterator2 sequenceLast) {
        if(sequenceFirst == sequenceLast) return last;

        ForwardIterator1 result = last;
        while(true) {
            ForwardIterator1 it = Search(first, last, sequenceFirst, sequenceLast);
            if(it == last) break;
            result = it;
            first = result;
            first++;
        }   
            
        return result;
    }

    /// @brief Searches for the last occurence of the sequence in a range using predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param sequenceFirst Iterator to the beginning of the sequence
    /// @param sequenceLast Iterator to the end of the sequence
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Iterator to the last occurence of the sequence
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find_end
    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator1 FindEnd(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sequenceFirst, ForwardIterator2 sequenceLast, BinaryPredicate predicate) {
        if(sequenceFirst == sequenceLast) return last;

        ForwardIterator1 result = last;
        while(true) {
            ForwardIterator1 it = Search(first, last, sequenceFirst, sequenceLast, predicate);
            if(it == last) break;
            result = it;
            first = result;
            first++;
        }   
            
        return result;
    }

    // Find first of

    /// @brief Finds the first occurence of any element from a sequence in a another range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param sequenceFirst Iterator to the beginning of the sequence
    /// @param sequenceLast Iterator to the end of the sequence
    /// @return Iterator to the first occurence of any element from the sequence
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find_first_of
    template<typename InputIterator, typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    InputIterator FindFirstOf(InputIterator first, InputIterator last, ForwardIterator sequenceFirst, ForwardIterator sequenceLast) {
        for(; first != last; first++) {
            ForwardIterator it = sequenceFirst;
            for(; it != sequenceLast; it++) if(*first == *it) return first;
        }

        return last;
    }

    /// @brief Finds the first occurence of any element from a sequence in a another range using predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param sequenceFirst Iterator to the beginning of the sequence
    /// @param sequenceLast Iterator to the end of the sequence
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Iterator to the first occurence of any element from the sequence
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/find_first_of
    template<typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    InputIterator FindFirstOf(InputIterator first, InputIterator last, ForwardIterator sequenceFirst, ForwardIterator sequenceLast, BinaryPredicate predicate) {
        for(; first != last; first++) {
            ForwardIterator it = sequenceFirst;
            for(; it != sequenceLast; it++) if(predicate(*first, *it)) return first;
        }
        
        return last;
    }
    
    // Adjacent find

    /// @brief Searches for the first occurence of two consecutive equal elements in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the first occurence of two consecutive equal elements
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/adjacent_find
    template<typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator AdjacentFind(ForwardIterator first, ForwardIterator last) {
        if(first == last) return last;

        ForwardIterator next = Next(first);
        for(; next != last; next++, first++) if(*first == *next) return first;
        return last;
    }

    /// @brief Searches for the first occurence of two consecutive elements that satisfy a predicate in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Iterator to the first occurence of two consecutive elements that satisfy the predicate
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/adjacent_find
    template<typename ForwardIterator, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator AdjacentFind(ForwardIterator first, ForwardIterator last, BinaryPredicate predicate) {
        if(first == last) return last;

        ForwardIterator next = Next(first);
        for(; next != last; next++, first++) if(predicate(*first, *next)) return first;
        return last;
    }

    // Copy

    /// @brief Copies elements from one range to another
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/copy
    template<typename InputIterator, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    OutputIterator Copy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        for(; first != last; first++, resultFirst++) *resultFirst = *first;
        return resultFirst;
    }

    // Copy if

    /// @brief Copies elements that satisfy a predicate from one range to another
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/copy
    template<typename InputIterator, typename OutputIterator, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__
    OutputIterator CopyIf(InputIterator first, InputIterator last, OutputIterator resultFirst, UnaryPredicate predicate) {
        for(; first != last; first++) if(predicate(*first)) *resultFirst++ = *first;
        return resultFirst;
    }

    // Copy in range

    /// @brief Copies N elements from one range to another
    /// @param first Iterator to the beginning of the source range
    /// @param count Number of elements to copy
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/copy_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    OutputIterator CopyInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        if(count > 0) for(Size i = 0; i != count; i++, resultFirst++, first++) *resultFirst = *first; 
        return resultFirst;
    }

    // Copy backward

    /// @brief Copies elements from one range to another in reversed order
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultLast Iterator to the beginning of the destination range
    /// @return Output iterator to the last element copied
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    __WSTL_CONSTEXPR14__
    BidirectionalIterator2 CopyBackward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 resultLast) {
        while(first != last) *--resultLast = *--last;
        return resultLast;
    }

    // Move

    /// @brief Moves elements from one range to another
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element moved
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/move
    template<typename InputIterator, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    OutputIterator Move(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        for(; first != last; first++, resultFirst++) *resultFirst = __WSTL_MOVE__(*first);
        return resultFirst;
    }

    // Move backward

    /// @brief Moves elements from one range to another in reversed order
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultLast Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element moved
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/move_backward
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    __WSTL_CONSTEXPR14__
    BidirectionalIterator2 MoveBackward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 resultLast) {
        while(first != last) *--resultLast = __WSTL_MOVE__(*--last);
        return resultLast;
    }

    // Fill

    /// @brief Fills a range with a specific value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to fill the range with
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/fill
    template<typename ForwardIterator, typename T>
    __WSTL_CONSTEXPR14__ 
    void Fill(ForwardIterator first, ForwardIterator last, const T& value) {
        for(; first != last; first++) *first = value;
    }

    // Fill in range

    /// @brief Fills N elements in a range with a specific value
    /// @param first Iterator to the beginning of the range
    /// @param count Number of elements to fill
    /// @param value Value to fill the range with
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/fill_n
    template<typename OutputIterator, typename Size, typename T>
    __WSTL_CONSTEXPR14__ 
    OutputIterator FillInRange(OutputIterator first, Size count, const T& value) {
        if(count > 0) for(Size i = 0; i < count; i++) *first = value;
        return first;
    }

    // Transform

    /// @brief Applies a function for elements in a range and stores the results in a destination range
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param operation Unary function to apply to each element
    /// @return Output iterator to the past the last element transformed
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/transform
    template<typename InputIterator, typename OutputIterator, typename UnaryOperation>
    __WSTL_CONSTEXPR14__
    OutputIterator Transform(InputIterator first, InputIterator last, OutputIterator resultFirst, UnaryOperation operation) {
        for(; first != last; resultFirst++, first++) *resultFirst = operation(*first);
        return resultFirst;
    }
    
    /// @brief Applies a function for two elements in two ranges and stores the results in a destination range
    /// @param first1 Iterator to the beginning of the first source range
    /// @param last1 Iterator to the end of the first source range
    /// @param first2 Iterator to the beginning of the second source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param operation Binary function to apply to each two elements
    /// @return Output iterator to the past the last element transformed
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/transform
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperation>
    __WSTL_CONSTEXPR14__
    OutputIterator Transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator resultFirst, BinaryOperation operation) {
        for(; first1 != last1; resultFirst++, first1++, first2++) *resultFirst = operation(*first1, *first2);
        return resultFirst;
    }

    // Generate

    /// @brief Assigns the resulf of function calls to every element in range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param generator Generator function that takes no argument and returns some value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/generate
    template<typename ForwardIterator, typename Generator>
    __WSTL_CONSTEXPR14__ 
    void Generate(ForwardIterator first, ForwardIterator last, Generator generator) {
        for(; first != last; first++) *first = generator();
    }

    // Generate in range

    /// @brief Assigns the resulf of function calls to first N element in range
    /// @param first Iterator to the beginning of the range
    /// @param count Number of elements to apply the function to
    /// @param generator Generator function that takes no argument and returns some value
    /// @return Output iterator to the past the last element generated
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/generate_n
    template<typename OutputIterator, typename Size, typename Generator>
    __WSTL_CONSTEXPR14__ 
    OutputIterator GenerateInRange(OutputIterator first, Size count, Generator generator) {
        if(count > 0) for(Size i = 0; i < count; i++, first++) *first = generator();
        return first;
    }

    // Remove

    /// @brief Removes all elements equal to a specific value from a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to remove
    /// @return Iterator to the past the last element removed
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/remove
    template<typename ForwardIterator, typename T>
    __WSTL_CONSTEXPR14__ 
    ForwardIterator Remove(ForwardIterator first, ForwardIterator last, const T& value) {
        first = Find(first, last, value);
        if(first != last) 
            for(ForwardIterator i = first; ++i != last;) 
                if(!(*i == value)) *first++ = __WSTL_MOVE__(*i);

        return first;
    }

    // Remove if

    /// @brief Removes all elements that satisfy a predicate from a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator to the past the last element removed
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/remove
    template<typename ForwardIterator, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__ 
    ForwardIterator RemoveIf(ForwardIterator first, ForwardIterator last, UnaryPredicate predicate) {
        first = FindIf(first, last, predicate);
        if(first != last)
            for(ForwardIterator i = first; ++i != last;) 
                if(!predicate(*i)) *first++ = __WSTL_MOVE__(*i);

        return first;
    }

    // Remove copy

    /// @brief Copies all elements except those equal to a specific value from a range to another
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param value Value to omit
    /// @return Iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/remove_copy
    template<typename InputIterator, typename OutputIterator, typename T>
    __WSTL_CONSTEXPR14__
    OutputIterator RemoveCopy(InputIterator first, InputIterator last, OutputIterator resultFirst, const T& value) {
        for(; first != last; first++) if(!(*first == value)) *resultFirst++ = *first;
        return resultFirst;
    }

    /// @brief Copies all elements except those that satisfy a predicate from a range to another
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/remove_copy
    template<typename InputIterator, typename OutputIterator, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__
    OutputIterator RemoveCopyIf(InputIterator first, InputIterator last, OutputIterator resultFirst, UnaryPredicate predicate) {
        for(; first != last; first++) if(!predicate(*first)) *resultFirst++ = *first;
        return resultFirst;
    }

    // Replace

    /// @brief Replaces all elements equal to a specific value in a range with another value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param oldValue Value to replace
    /// @param newValue Value to replace with
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/replace
    template<typename ForwardIterator, typename T>
    __WSTL_CONSTEXPR14__
    void Replace(ForwardIterator first, ForwardIterator last, const T& oldValue, const T& newValue) {
        for(; first != last; first++) if(*first == oldValue) *first = newValue;
    }

    // Replace if

    /// @brief Replaces all elements that satisfy a predicate in a range with another value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @param newValue Value to replace with
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/replace
    template<typename ForwardIterator, typename UnaryPredicate, typename T>
    __WSTL_CONSTEXPR14__
    void ReplaceIf(ForwardIterator first, ForwardIterator last, UnaryPredicate predicate, const T& newValue) {
        for(; first != last; first++) if(predicate(*first)) *first = newValue;
    }

    // Replace copy

    /// @brief Replaces all elements equal to a specific value in a range with another value and copies them to another range
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param oldValue Value to replace
    /// @param newValue Value to replace with
    /// @return Iterator to the past the last element replaced
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/replace_copy
    template<typename InputIterator, typename OutputIterator, typename T>
    __WSTL_CONSTEXPR14__
    OutputIterator ReplaceCopy(InputIterator first, InputIterator last, OutputIterator resultFirst, const T& oldValue, const T& newValue) {
        for(; first != last; first++) *resultFirst++ = (*first == oldValue) ? newValue : *first;
        return resultFirst;
    }

    // Replace copy if

    /// @brief Replaces all elements that satisfy a predicate in a range with another value and copies them to another range
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @param newValue Value to replace with
    /// @return Iterator to the past the last element replaced
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/replace_copy
    template<typename InputIterator, typename OutputIterator, typename UnaryPredicate, typename T>
    __WSTL_CONSTEXPR14__
    OutputIterator ReplaceCopyIf(InputIterator first, InputIterator last, OutputIterator resultFirst, UnaryPredicate predicate, const T& newValue) {
        for(; first != last; first++) *resultFirst++ = predicate(*first) ? newValue : *first;
        return resultFirst;
    }

    // Iterator swap

    /// @brief Swaps the values of two iterators
    /// @param a Iterator to the first element
    /// @param b Iterator to the second element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/iter_swap
    template<typename ForwardIterator1, typename ForwardIterator2>
    __WSTL_CONSTEXPR14__ 
    void IteratorSwap(ForwardIterator1 a, ForwardIterator2 b) {
        wstl::Swap(*a, *b);
    }

    // Swap ranges

    /// @brief Swaps the elements of two ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @return Iterator to the past the last element swapped in second range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/swap_ranges
    template<typename ForwardIterator1, typename ForwardIterator2>
    __WSTL_CONSTEXPR14__ 
    ForwardIterator2 SwapRanges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
        for(; first1 != last1; first1++, first2++) IteratorSwap(first1, first2);
        return first2;
    }

    // Reverse
    
    /// @brief Reverses the elements of a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/reverse
    template<typename BidirectionalIterator>
    __WSTL_CONSTEXPR14__ 
    void Reverse(BidirectionalIterator first, BidirectionalIterator last) {
        while(first != last && first != --last) IteratorSwap(first++, last);
    }

    // Reverse copy

    /// @brief Reverses the elements of a range and copies them to another range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/reverse_copy
    template<typename BidirectionalIterator, typename OutputIterator>
    __WSTL_CONSTEXPR14__ 
    OutputIterator ReverseCopy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator resultFirst) {
        for(; first != last; resultFirst++) *resultFirst = *--last;
        return resultFirst;
    }

    // Rotate

    /// @brief Performs a left rotation on a range
    /// @param first Iterator to the beginning of the range
    /// @param middle Iterator to the element that should appear at the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the element that appears at the beginning of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/rotate
    template<typename ForwardIterator>
    __WSTL_CONSTEXPR14__ 
    ForwardIterator Rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
        if(first == middle) return last;
        if(middle == last) return first;
        
        Reverse(first, middle);
        Reverse(middle, last);
        Reverse(first, last);

        ForwardIterator result = first;
        Advance(result, Distance(middle, last));
        return result;
    }

    // Rotate copy

    /// @brief Performs a left rotation on a range and copies the result to another range
    /// @param first Iterator to the beginning of the range
    /// @param middle Iterator to the element that should appear at the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/rotate_copy
    template<typename OutputIterator, typename ForwardIterator>
    __WSTL_CONSTEXPR14__ 
    OutputIterator RotateCopy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator resultFirst) {
        resultFirst = Copy(middle, last, resultFirst);
        return Copy(first, middle, resultFirst);
    }

    // Unique

    /// @brief Removes all except first consecutive duplicate elements from a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the new end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/unique
    template<typename ForwardIterator>
    __WSTL_CONSTEXPR14__ 
    ForwardIterator Unique(ForwardIterator first, ForwardIterator last) {
        if(first == last) return last;

        ForwardIterator result = first;
        while(++first != last) 
            if(!(*result == *first) && ++result != first) *result = __WSTL_MOVE__(*first);

        return ++result;
    }

    /// @brief Removes consecutive duplicate elements from a range using a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Iterator to the new end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/unique
    template<typename ForwardIterator, typename BinaryPredicate>
    __WSTL_CONSTEXPR14__ 
    ForwardIterator Unique(ForwardIterator first, ForwardIterator last, BinaryPredicate predicate) {
        if(first == last) return last;
        
        ForwardIterator result = first;
        while(++first != last)
            if(!predicate(*result, *first) && ++result != first) *result = __WSTL_MOVE__(*first);

        return ++result;
    }

    // Unique copy

    /// @brief Copies all except first consecutive duplicate elements from a range to another
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/unique_copy
    template<typename InputIterator, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    inline OutputIterator UniqueCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        if (first == last) return resultFirst;
        *resultFirst = *first;

        while (++first != last) 
            if(!(*first == *resultFirst)) *++resultFirst = *first;

        return ++resultFirst;
    }

    /// @brief Copies all except first consecutive duplicate elements from a range to another using a predicate
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return Output iterator to the past the last element copied
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/unique_copy
    template<typename InputIterator, typename OutputIterator, typename BinaryPredicate>
    __WSTL_CONSTEXPR14__
    inline OutputIterator UniqueCopy(InputIterator first, InputIterator last, OutputIterator resultFirst, BinaryPredicate predicate) {
        if (first == last) return resultFirst;
        *resultFirst = *first;

        while (++first != last) 
            if(!predicate(*first, *resultFirst)) *++resultFirst = *first;

        return ++resultFirst;
    }

    // Is partitioned

    /// @brief Checks if a range is partitioned by a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return True if the range is partitioned, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_partitioned
    template<typename InputIterator, class UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool IsPartitioned(InputIterator first, InputIterator last, UnaryPredicate predicate) {
        for(; first != last; first++) if(!predicate(*first)) break;
        return FindIf(first, last, predicate) == last;
    }

    // Partition

    /// @brief Partitions a range by a predicate in way that elements that satisfy the predicate are 
    /// before those that don't
    /// @param first Bidirectional iterator to the beginning of the range
    /// @param last Bidirectional iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator to the new end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partition
    template<typename BidirectionalIterator, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__
    typename EnableIf<IsBidirectionalIterator<BidirectionalIterator>::Value, BidirectionalIterator>::Type 
    Partition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate predicate) {
        while(first != last) {
            first = FindIfNot(first, last, predicate);
            if(first == last) break;

            last = FindIf(ReverseIterator<BidirectionalIterator>(last),
                ReverseIterator<BidirectionalIterator>(first), predicate).Base();
            if(first == last) break;

            --last;
            IteratorSwap(first, last);
            first++;
        }

        return first;
    }

    /// @brief Partitions a range by a predicate in way that elements that satisfy the predicate are
    /// before those that don't
    /// @param first Forward iterator to the beginning of the range
    /// @param last Forward iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator to the new end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partition
    template<typename ForwardIterator, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__
    typename EnableIf<!IsBidirectionalIterator<ForwardIterator>::Value && IsForwardIterator<ForwardIterator>::Value, ForwardIterator>::Type 
    Partition(ForwardIterator first, ForwardIterator last, UnaryPredicate predicate) {
        first = FindIfNot(first, last, predicate);
        if(first == last) return;

        for(ForwardIterator i = Next(first); i != last; i++)
            if(predicate(*i)) {
                IteratorSwap(i, first);
                first++;
            }

        return first;
    }

    // Partition copy
    
    /// @brief Copies elements to two different ranges depending on the result of a predicate
    /// @param first Iterator to the beginning of the source range
    /// @param last Iterator to the end of the source range
    /// @param resultFirstTrue Iterator to the beginning of the destination range for elements 
    /// that satisfy the predicate
    /// @param resultFirstFalse Iterator to the beginning of the destination range for elements 
    /// that don't satisfy the predicate
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Pair of iterator to the end of the `resultFirstTrue`, and iterator to the end of the `resultFirstFalse`
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partition_copy
    template<typename InputIterator, typename OutputIterator1, typename OutputIterator2, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__
    Pair<OutputIterator1, OutputIterator2> PartitionCopy(InputIterator first, InputIterator last, OutputIterator1 resultFirstTrue, OutputIterator2 resultFirstFalse, UnaryPredicate predicate) {
        for(; first != last; first++) {
            if(predicate(*first)) *resultFirstTrue++ = *first;
            else *resultFirstFalse++ = *first;
        }

        return Pair<OutputIterator1, OutputIterator2>(resultFirstTrue, resultFirstFalse);
    }

    // Stable partition

    /// @brief Partitions a range by a predicate in way that elements that satisfy the predicate are
    /// before those that don't while preserving the relative order
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator to the end of the range of elements that satisfy the predicate
    template<typename BidirectionalIterator, typename UnaryPredicate>
    __WSTL_CONSTEXPR14__
    BidirectionalIterator StablePartition(BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate predicate) {
        typedef typename IteratorTraits<BidirectionalIterator>::DifferenceType Difference;
        Difference distance = Distance(first, last);

        if (distance < 2) return predicate(*first) ? last : first;

        BidirectionalIterator mid = Next(first, distance / 2);
        BidirectionalIterator left = StablePartition(first, mid, predicate);
        BidirectionalIterator right = StablePartition(mid, last, predicate);

        return Rotate(left, mid, right);
    }

    // Partition point

    /// @brief Finds the partition point of a range, a first element that does not satisfy a predicate
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param predicate Unary predicate function that returns a boolean value
    /// @return Iterator to the partition point
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partition_point
    template<typename ForwardIterator, typename UnaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator PartitionPoint(ForwardIterator first, ForwardIterator last, UnaryPredicate predicate) {
        while(first != last) {
            ForwardIterator middle = Next(first, Distance(first, last) / 2);
            if(predicate(*middle)) first = Next(middle);
            else last = middle;
        }
        
        return first;
    }

    // Min element

    /// @brief Finds the smallest element in a range using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Iterator to the smallest element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/min_element
    template<typename ForwardIterator, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator MinElement(ForwardIterator first, ForwardIterator last, Compare compare) {
        if(first == last) return last;

        ForwardIterator result = first;

        for(; first != last; first++) if(compare(*first, *result)) result = first;
        return result;
    }

    /// @brief Finds the smallest element in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the smallest element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/min_element
    template<typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline ForwardIterator MinElement(ForwardIterator first, ForwardIterator last) {
        return MinElement(first, last, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Min

    /// @brief Returns the smallest of two values using a comparator
    /// @param a First value
    /// @param b Second value
    /// @param compare Binary comparator function that returns a boolean value
    /// @return The smallest value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/min
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    const T& Min(const T& a, const T& b, Compare compare) {
        return compare(a, b) ? a : b;
    }

    /// @brief Returns the smallest of two values
    /// @param a First value
    /// @param b Second value
    /// @return The smallest value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/min
    template<typename T> 
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    const T& Min(const T& a, const T& b) {
        return (a < b) ? a : b;
    }

    #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
    /// @brief Returns the smallest value in an initializer list using a comparator
    /// @param list Initializer list to examine
    /// @param compare Binary comparator function that returns a boolean value
    /// @return The smallest value in the initializer list
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/min
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline T Min(InitializerList<T> list, Compare compare) {
        return *MinElement(list.Begin(), list.End(), compare);
    }

    /// @brief Returns the smallest value in an initializer list
    /// @param list Initializer list to examine
    /// @return The smallest value in the initializer list
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/min
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline T Min(InitializerList<T> list) {
        return *MinElement(list.Begin(), list.End());
    }
    #endif

    // Max element

    /// @brief Finds the biggest element in a range using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Iterator to the biggest element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/max_element
    template<typename ForwardIterator, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator MaxElement(ForwardIterator first, ForwardIterator last, Compare compare) {
        if(first == last) return last;

        ForwardIterator result = first;

        for(; first != last; first++) if(compare(*result, *first)) result = first;
        return result;
    }

    /// @brief Finds the biggest element in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the biggest element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/max_element
    template<typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline ForwardIterator MaxElement(ForwardIterator first, ForwardIterator last) {
        return MaxElement(first, last, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Max

    /// @brief Returns the biggest of two values using a comparator
    /// @param a First value
    /// @param b Second value
    /// @param compare Binary comparator function that returns a boolean value
    /// @return The biggest value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/max
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    const T& Max(const T& a, const T& b, Compare compare) {
        return compare(a, b) ? b : a;
    }

    /// @brief Returns the biggest of two values
    /// @param a First value
    /// @param b Second value
    /// @return The biggest value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/max
    template<typename T> 
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    const T& Max(const T& a, const T& b) {
        return (a < b) ? b : a;
    }

    #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
    /// @brief Returns the biggest value in an initializer list using a comparator
    /// @param list Initializer list to examine
    /// @param compare Binary comparator function that returns a boolean value
    /// @return The biggest value in the initializer list
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/max
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline T Max(InitializerList<T> list, Compare compare) {
        return *MaxElement(list.Begin(), list.End(), compare);
    }

    /// @brief Returns the biggest value in an initializer list
    /// @param list Initializer list to examine
    /// @return The biggest value in the initializer list
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/max
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline T Max(InitializerList<T> list) {
        return *MaxElement(list.Begin(), list.End());
    }
    #endif

    // Min max element

    /// @brief Finds the smallest and the largest element in a range using a  comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return A pair with the smallest and the largest element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/minmax_element
    template<typename ForwardIterator, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    Pair<ForwardIterator, ForwardIterator> MinMaxElement(ForwardIterator first, ForwardIterator last, Compare compare) {
        ForwardIterator min = first;
        ForwardIterator max = first;

        for(; first != last; first++) {
            if(compare(*first, *min)) min = first;
            if(compare(*max, *first)) max = first;
        }

        return MakePair(min, max);
    }

    /// @brief Finds the smallest and the largest element in a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return A pair with the smallest and the largest element
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/minmax_element
    template<typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline Pair<ForwardIterator, ForwardIterator> MinMaxElement(ForwardIterator first, ForwardIterator last) {
        return MinMaxElement(first, last, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Min max

    /// @brief Returns the smallest and the largest of two values using a comparator
    /// @param a First value
    /// @param b Second value
    /// @param compare Binary comparator function that returns a boolean value
    /// @return A pair with the smallest and the largest value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/minmax
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    Pair<const T&, const T&> MinMax(const T& a, const T& b, Compare compare) {
        return compare(a, b) ? Pair<const T&, const T&>(a, b) : Pair<const T&, const T&>(b, a);
    }

    /// @brief Returns the smallest and the largest of two values
    /// @param a First value
    /// @param b Second value
    /// @return A pair with the smallest and the largest value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/minmax
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    Pair<const T&, const T&> MinMax(const T& a, const T& b) {
        return (a < b) ? Pair<const T&, const T&>(a, b) : Pair<const T&, const T&>(b, a);
    }

    #if defined(__WSTL_CXX11__) && !defined(__WSTL_NO_INITIALIZERLIST__)
    /// @brief Returns the smallest and the largest value in an initializer list using a comparator
    /// @param list Initializer list to examine
    /// @param compare Binary comparator function that returns a boolean value
    /// @return A pair with the smallest and the largest value in the initializer list
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/minmax
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline Pair<T, T> MinMax(InitializerList<T> list, Compare compare) {
        Pair<const T*, const T*> result = MinMaxElement(list.Begin(), list.End(), compare);
        return MakePair(*result.First, *result.Second);
    }

    /// @brief Returns the smallest and the largest value in an initializer list
    /// @param list Initializer list to examine
    /// @return A pair with the smallest and the largest value in the initializer list
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/minmax
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline Pair<T, T> MinMax(InitializerList<T> list) {
        Pair<const T*, const T*> result = MinMaxElement(list.Begin(), list.End());
        return MakePair(*result.First, *result.Second);
    }
    #endif

    // Clamp

    /// @brief Returns specified value if in range, otherwise the nearest boundary
    /// @param value Value to clamp
    /// @param low Lower boundary
    /// @param high Upper boundary
    /// @return Clamped value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/clamp
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline const T& Clamp(const T& value, const T& low, const T& high) {
        return (value < low) ? low : (high > value) ? high : value;
    }

    /// @brief Returns specified value if in range, otherwise the nearest boundary using a comparator
    /// @param value Value to clamp
    /// @param low Lower boundary
    /// @param high Upper boundary
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Clamped value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/clamp
    template<typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline const T& Clamp(const T& value, const T& low, const T& high, Compare compare) {
        return compare(value, low) ? low : compare(high, value) ? high : value;
    }

    // Equal

    /// @brief Checks whether two ranges are equal using a predicate
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return True if elements in the ranges are equal, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/equal
    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool Equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate predicate) {
        for(; first1 != last1; first1++, first2++)
            if(!predicate(*first1, *first2)) return false;
        
        return true;
    }

    /// @brief Checks whether two ranges are equal
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @return True if elements in the ranges are equal, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/equal
    template<typename InputIterator1, typename InputIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool Equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        for(; first1 != last1; first1++, first2++)
            if(!(*first1 == *first2)) return false;
        
        return true;
    }

    /// @brief Checks whether two ranges are equal using a predicate
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param predicate Binary predicate function that returns a boolean value
    /// @return True if elements in the ranges are equal, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/equal
    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool Equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate predicate) {
        for(; first1 != last1 && first2 != last2; first1++, first2++)
            if(!predicate(*first1, *first2)) return false;

        return first1 == last1 && first2 == last2;
    }

    /// @brief Checks whether two ranges are equal
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @return True if elements in the ranges are equal, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/equal
    template<typename InputIterator1, typename InputIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool Equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        for(; first1 != last1 && first2 != last2; first1++, first2++)
            if(!(*first1 == *first2)) return false;

        return first1 == last1 && first2 == last2;
    }

    // Lexicographical compare

    /// @brief Compares two ranges lexicographically using a comparator
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return True if the first range is lexicographically less than the second range, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
    template<typename InputIterator1, typename InputIterator2, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool LexicographicalCompare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare compare) {
        for(; first1 != last1 && first2 != last2; first1++, first2++) {
            if(compare(*first1, *first2)) return true;
            if(compare(*first2, *first1)) return false;
        }

        return first1 == last1 && first2 != last2;
    }

    /// @brief Compares two ranges lexicographically
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @return True if the first range is lexicographically less than the second range, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
    template<typename InputIterator1, typename InputIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline bool LexicographicalCompare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        return LexicographicalCompare(first1, last1, first2, last2, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }
        
    // Make heap

    namespace __private {
        template<typename RandomAccessIterator, typename Compare>
        __WSTL_CONSTEXPR14__
        void __SiftDown(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator root, Compare compare) {
            typedef typename IteratorTraits<RandomAccessIterator>::DifferenceType DifferenceType;
            DifferenceType n = Distance(first, last);
            DifferenceType i = Distance(first, root);

            while(true) {
                DifferenceType left = 2 * i + 1;
                DifferenceType right = 2 * i + 2;
                DifferenceType largest = i;

                if(left < n && compare(*(first + largest), *(first + left))) largest = left;
                if(right < n && compare(*(first + largest), *(first + right))) largest = right;
                
                if(largest == i) break;
                IteratorSwap(first + i, first + largest);
                i = largest;
            }
        }
    }

    /// @brief Builds a heap from a range using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/make_heap
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void MakeHeap(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        typedef typename IteratorTraits<RandomAccessIterator>::DifferenceType DifferenceType;
        DifferenceType n = Distance(first, last);
        if(n <= 1) return;

        for(DifferenceType i = (n / 2) - 1; i >= 0; i--) {
            __private::__SiftDown(first, last, first + i, compare);
        }
    }

    /// @brief Builds a max heap from a range
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/make_heap
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void MakeHeap(RandomAccessIterator first, RandomAccessIterator last) {
        MakeHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Push heap

    namespace __private {
        template<typename RandomAccessIterator, typename Compare>
        __WSTL_CONSTEXPR14__
        void __SiftUp(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
            typedef typename IteratorTraits<RandomAccessIterator>::DifferenceType DifferenceType;
            DifferenceType i = Distance(first, last) - 1;
            while(i > 0) {
                DifferenceType parent = (i - 1) / 2;

                if(!compare(*(first + parent), *(first + i))) break;
                IteratorSwap(first + i, first + parent);
                i = parent;
            }
        }
    }

    /// @brief Pushes an element onto a heap using a comparator
    /// @param first Iterator to the beginning of the heap
    /// @param last Iterator to the end of the heap
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/push_heap
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void PushHeap(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        __private::__SiftUp(first, last, compare);
    }

    /// @brief Pushes an element onto a max heap
    /// @param first Iterator to the beginning of the heap
    /// @param last Iterator to the end of the heap
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/push_heap
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void PushHeap(RandomAccessIterator first, RandomAccessIterator last) {
        PushHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Pop heap

    /// @brief Pops the top element from a heap using a comparator
    /// @param first Iterator to the beginning of the heap
    /// @param last Iterator to the end of the heap
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/pop_heap
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void PopHeap(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        if(Distance(first, last) <= 1) return;
        IteratorSwap(first, last - 1);
        __private::__SiftDown(first, last - 1, first, compare);
    }

    /// @brief Pops the top element from a max heap
    /// @param first Iterator to the beginning of the heap
    /// @param last Iterator to the end of the heap
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/pop_heap
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void PopHeap(RandomAccessIterator first, RandomAccessIterator last) {
        PopHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Sort heap

    /// @brief Turns a heap into a sorted range using a comparator
    /// @param first Iterator to the beginning of the heap
    /// @param last Iterator to the end of the heap
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/sort_heap
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void SortHeap(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        while(Distance(first, last) > 1) {
            PopHeap(first, last, compare);
            --last;
        }
    }

    /// @brief Turns a max heap into a sorted range
    /// @param first Iterator to the beginning of the heap
    /// @param last Iterator to the end of the heap
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/sort_heap
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void SortHeap(RandomAccessIterator first, RandomAccessIterator last) {
        SortHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Is heap until

    /// @brief Checks if a range is a heap until a certain position using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Iterator to the first element that violates the heap property
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_heap_until
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    RandomAccessIterator IsHeapUntil(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        typedef typename IteratorTraits<RandomAccessIterator>::DifferenceType DifferenceType;
        DifferenceType n = Distance(first, last);

        for(DifferenceType parent = 0; parent < n; parent++) {
            DifferenceType left = 2 * parent + 1;
            DifferenceType right = 2 * parent + 2;

            if(left < n && compare(*(first + parent), *(first + left))) return first + left;

            if(right < n && compare(*(first + parent), *(first + right))) return first + right;
        }

        return last;
    }

    /// @brief Checks if a range is a max heap until a certain position
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the first element that violates the heap property
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_heap_until
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline RandomAccessIterator IsHeapUntil(RandomAccessIterator first, RandomAccessIterator last) {
        return IsHeapUntil(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Is heap

    /// @brief Checks if a range is a heap using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return True if the range is a heap, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_heap
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    inline bool IsHeap(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        return IsHeapUntil(first, last, compare) == last;
    }

    /// @brief Checks if a range is a max heap
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return True if the range is a max heap, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_heap
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline bool IsHeap(RandomAccessIterator first, RandomAccessIterator last) {
        return IsHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Is sorted until

    /// @brief Checks if a range is sorted using a comparator until a certain position
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Iterator to the first element that violates the sorted property
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_sorted_until
    template<typename ForwardIterator, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator IsSortedUntil(ForwardIterator first, ForwardIterator last, Compare compare) {
        if(first != last) {
            ForwardIterator next = first;
            while(++next != last) {
                if(compare(*next, *first)) return next;
                first = next;
            }
        }

        return first;
    }

    /// @brief Checks if a range is sorted in ascending order until a certain position
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return Iterator to the first element that violates the sorted property
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_sorted_until
    template<typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline ForwardIterator IsSortedUntil(ForwardIterator first, ForwardIterator last) {
        return IsSortedUntil(first, last, Less<>());
    }

    // Is sorted

    /// @brief Checks if a range is sorted
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return True if the range is sorted, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_sorted
    template<typename ForwardIterator>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline bool IsSorted(ForwardIterator first, ForwardIterator last) {
        return IsSortedUntil(first, last) == last;
    }

    /// @brief Checks if a range is sorted using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return True if the range is sorted, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_sorted
    template<typename ForwardIterator, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline bool IsSorted(ForwardIterator first, ForwardIterator last, Compare compare) {
        return IsSortedUntil(first, last, compare) == last;
    }

    // Quick sort

    /// @brief Sorts a range using quick sort algorithm and a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void QuickSort(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        // base case
        if(Distance(first, last) <= 1) return;

        RandomAccessIterator pivot = Previous(last);
        RandomAccessIterator i = first;

        for(RandomAccessIterator j = first; j != pivot; j++) {
            if (compare(*j, *pivot)) {
                IteratorSwap(i, j);
                i++;
            }
        }

        IteratorSwap(i, pivot);

        QuickSort(first, i, compare);
        QuickSort(Next(i), last, compare);
    }

    /// @brief Sorts a range into ascending order using quick sort algorithm
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void QuickSort(RandomAccessIterator first, RandomAccessIterator last) {
        QuickSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Partial sort

    /// @brief Sorts first N elements of a range using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param middle Iterator to the middle of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partial_sort
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void PartialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare compare) {
        if(Distance(first, last) <= 1) return;
        MakeHeap(first, middle, compare);
        
        for(RandomAccessIterator i = middle; i != last; i++) {
            if(compare(*i, *first)) {
                IteratorSwap(first, i);
                PopHeap(first, middle, compare);
            }
        }

        SortHeap(first, middle, compare);
    }

    /// @brief Sorts first N elements of a range into ascending order
    /// @param first Iterator to the beginning of the range
    /// @param middle Iterator to the middle of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partial_sort
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void PartialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
        PartialSort(first, middle, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Partial sort copy

    /// @brief Copies first N elements of a range and sorts them using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param resultLast Iterator to the end of the destination range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Iterator to the end of the sorted range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partial_sort_copy
    template<typename InputIterator, typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    RandomAccessIterator PartialSortCopy(InputIterator first, InputIterator last, RandomAccessIterator resultFirst, RandomAccessIterator resultLast, Compare compare) {
        if(Distance(resultFirst, resultLast) <= 0) return resultFirst;

        RandomAccessIterator resultEnd = resultFirst;
        for(; first != last && resultEnd != resultLast; first++, resultEnd++) *resultEnd = *first;

        MakeHeap(resultFirst, resultEnd, compare);

        for(; first != last; first++) {
            if(compare(*first, *resultFirst)) {
                *resultFirst = *first;
                PopHeap(resultFirst, resultEnd, compare);
            }
        }

        SortHeap(resultFirst, resultEnd, compare);
        return resultEnd;
    }

    /// @brief Copies first N elements of a range and sorts them into ascending order
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param resultLast Iterator to the end of the destination range
    /// @return Iterator to the end of the sorted range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/partial_sort_copy
    template<typename InputIterator, typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline RandomAccessIterator PartialSortCopy(InputIterator first, InputIterator last, RandomAccessIterator resultFirst, RandomAccessIterator resultLast) {
        return PartialSortCopy(first, last, resultFirst, resultLast, Less<typename IteratorTraits<InputIterator>::ValueType>());
    }

    // Merge

    /// @brief Merges two sorted ranges into a single sorted range using a comparator
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param compare Binary comparator function that returns a boolean value
    /// @return Iterator to the end of the merged range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/merge
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    OutputIterator Merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst, Compare compare) {
        while(first1 != last1 && first2 != last2) {
            if(compare(*first2, *first1)) *resultFirst++ = *first2++;
            else *resultFirst++ = *first1++;
        }

        Copy(first1, last1, resultFirst);
        Copy(first2, last2, resultFirst);
        return resultFirst;
    }

    /// @brief Merges two sorted ranges into a single sorted range
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Iterator to the end of the merged range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/merge
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    inline OutputIterator Merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst) {
        return Merge(first1, last1, first2, last2, resultFirst, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }

    // Inplace merge

    /// @brief Merges two sorted ranges into a single sorted range in-place using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param middle Iterator to the end of the first range and the beginning of the second range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator function that returns a boolean value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/inplace_merge
    template<typename BidirectionalIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void InplaceMerge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Compare compare) {
        if(first == middle || middle == last) return;

        BidirectionalIterator first1 = first;
        BidirectionalIterator first2 = middle;

        while(first1 != first2 && first2 != last) {
            if(!compare(*first2, *first1)) first1++;
            else {
                typename IteratorTraits<BidirectionalIterator>::ValueType value = *first2;
                Rotate(first1, first2, Next(first2));
                *first1 = value;
                first1++;
                first2++;
            }
        }
    }

    /// @brief Merges two sorted ranges into a single sorted range in-place
    /// @param first Iterator to the beginning of the range
    /// @param middle Iterator to the end of the first range and the beginning of the second range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/inplace_merge
    template<typename BidirectionalIterator>
    __WSTL_CONSTEXPR14__
    inline void InplaceMerge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
        InplaceMerge(first, middle, last, Less<typename IteratorTraits<BidirectionalIterator>::ValueType>());
    }

    // Merge sort

    namespace __private {
        template<typename RandomAccessIterator, typename Compare>
        __WSTL_CONSTEXPR14__
        void __MergeSort(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator temp, Compare compare) {
            if(Distance(first, last) <= 1) return;
            RandomAccessIterator middle = first + (last - first) / 2;

            __MergeSort(first, middle, temp, compare);
            __MergeSort(middle, last, temp, compare);

            Merge(first, middle, middle, last, temp);
            for(RandomAccessIterator l = first; l != last; l++, temp++) *l = *temp;
        }
    }

    /// @brief Sorts a range using merge sort algorithm and a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for sorting
    /// @ingroup algorithm
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void MergeSort(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        typename IteratorTraits<RandomAccessIterator>::ValueType temp[Distance(first, last)];
        __private::__MergeSort(first, last, temp, compare);
    }

    /// @brief Sorts a range into ascending order using merge sort algorithm
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void MergeSort(RandomAccessIterator first, RandomAccessIterator last) {
        MergeSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Heap sort

    /// @brief Sorts a range using heap sort algorithm and a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for sorting
    /// @ingroup algorithm
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    inline void HeapSort(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        MakeHeap(first, last, compare);
        SortHeap(first, last, compare);
    }

    /// @brief Sorts a range into ascending order using heap sort algorithm
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void HeapSort(RandomAccessIterator first, RandomAccessIterator last) {
        HeapSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }

    // Nth element

    namespace __private {
        template<typename RandomAccessIterator, typename Compare>
        class __PivotComparator {
        public:
            __WSTL_CONSTEXPR__ __PivotComparator(RandomAccessIterator pivot, Compare compare) 
                : m_Pivot(pivot), m_Compare(compare) {};

            bool operator()(const typename IteratorTraits<RandomAccessIterator>::ValueType& x) const {
                return m_Compare(x, *m_Pivot);
            }

        private:
            RandomAccessIterator m_Pivot;
            Compare m_Compare;
        };
    }

    /// @brief Partially sorts a range such that the element at `nth` is in its correct position using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param nth Iterator to the element to be placed in its correct position
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for sorting
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/nth_element
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    void NthElement(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare compare) {
        while(first < last) {
            RandomAccessIterator middle = Partition(first, last, 
                __private::__PivotComparator<RandomAccessIterator, Compare>(first, compare));

            IteratorSwap(first, middle);

            if(middle == nth) return;
            else if(middle < nth) first = middle + 1;
            else last = middle;
        }
    }

    /// @brief Partially sorts a range such that the element at `nth` is in its correct position
    /// @param first Iterator to the beginning of the range
    /// @param nth Iterator to the element to be placed in its correct position
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/nth_element
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void NthElement(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last) {
        NthElement(first, nth, last, Less<typename IteratorTraits<RandomAccessIterator>::ValueType>());
    }
    
    // Sort

    /// @brief Sorts a range using a comparator. Uses quick sort algorithm internally
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for sorting
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/sort
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    inline void Sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        QuickSort(first, last, compare);
    }

    /// @brief Sorts a range into ascending order. Uses quick sort algorithm internally
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/sort
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void Sort(RandomAccessIterator first, RandomAccessIterator last) {
        QuickSort(first, last);
    }

    // Stable sort

    /// @brief Sorts a range using a comparator, preserving order between equal elements. Uses merge sort algorithm internally
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for sorting
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/stable_sort
    template<typename RandomAccessIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    inline void StableSort(RandomAccessIterator first, RandomAccessIterator last, Compare compare) {
        MergeSort(first, last, compare);
    }

    /// @brief Sorts a range into ascending order, preserving order between equal elements. Uses merge sort algorithm internally
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/stable_sort
    template<typename RandomAccessIterator>
    __WSTL_CONSTEXPR14__
    inline void StableSort(RandomAccessIterator first, RandomAccessIterator last) {
        MergeSort(first, last);
    }

    // Lower bound

    /// @brief Finds the first element in a sorted range that is not less than the given value using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @param compare Binary comparator to use for examining
    /// @return Iterator to the first element that is not less than the given value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/lower_bound
    template<typename ForwardIterator, typename Compare, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator LowerBound(ForwardIterator first, ForwardIterator last, const T& value, Compare compare) {
        while(first != last) {
            ForwardIterator middle = first;
            Advance(middle, Distance(first, last) / 2);

            if(compare(*middle, value)) first = Next(middle);
            else last = middle;
        }

        return first;
    }

    /// @brief Finds the first element in a sorted range that is not less than the given value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @return Iterator to the first element that is not less than the given value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/lower_bound
    template<typename ForwardIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline ForwardIterator LowerBound(ForwardIterator first, ForwardIterator last, const T& value) {
        return LowerBound(first, last, value, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Upper bound

    /// @brief Finds the first element in a sorted range that is greater than the given value using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @param compare Binary comparator to use for examining
    /// @return Iterator to the first element that is greater than the given value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/upper_bound
    template<typename ForwardIterator, typename T, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator UpperBound(ForwardIterator first, ForwardIterator last, const T& value, Compare compare) {
        while(first != last) {
            ForwardIterator middle = first;
            Advance(middle, Distance(first, last) / 2);

            if(!compare(value, *middle)) first = Next(middle);
            else last = middle;
        }

        return first;
    }

    /// @brief Finds the first element in a sorted range that is greater than the given value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @return Iterator to the first element that is greater than the given value
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/upper_bound
    template<typename ForwardIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline ForwardIterator UpperBound(ForwardIterator first, ForwardIterator last, const T& value) {
        return UpperBound(first, last, value, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Binary search

    /// @brief Checks if there is an element in a sorted range that is equal to the given value using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @param compare Binary comparator to use for searching
    /// @return True if the value is found, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/binary_search
    template<typename ForwardIterator, typename Compare, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline bool BinarySearch(ForwardIterator first, ForwardIterator last, const T& value, Compare compare) {
        first = LowerBound(first, last, value, compare);
        return first != last && !compare(value, *first);
    }

    /// @brief Checks if there is an element in a sorted range that is equal to the given value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @return True if the value is found, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/binary_search
    template<typename ForwardIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline bool BinarySearch(ForwardIterator first, ForwardIterator last, const T& value) {
        return BinarySearch(first, last, value, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Equal range

    /// @brief Finds the range of elements in a sorted range that are equal to the given value using a comparator
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @param compare Binary comparator to use for searching
    /// @return A pair of iterators to the first occurence and the element that is greater than the given value in the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/equal_range
    template<typename ForwardIterator, typename Compare, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline Pair<ForwardIterator, ForwardIterator> EqualRange(ForwardIterator first, ForwardIterator last, const T& value, Compare compare) {
        return MakePair(LowerBound(first, last, value, compare), UpperBound(first, last, value, compare));
    }

    /// @brief Finds the range of elements in a sorted range that are equal to the given value
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @return A pair of iterators to the first occurence and the element that is greater than the given value in the range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/equal_range
    template<typename ForwardIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline Pair<ForwardIterator, ForwardIterator> EqualRange(ForwardIterator first, ForwardIterator last, const T& value) {
        return EqualRange(first, last, value, Less<typename IteratorTraits<ForwardIterator>::ValueType>());
    }

    // Binary find - custom

    /// @brief Find the given value in a sorted range using binary search algorithm, a comparator and an equality function
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @param compare Binary comparator to use for searching
    /// @param equality Equality function to use for comparison
    /// @return Iterator to the element that is equal to the given value in the range, or `last` if not found
    /// @ingroup algorithm
    template<typename ForwardIterator, typename T, typename Compare, typename Equality>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    ForwardIterator BinaryFind(ForwardIterator first, ForwardIterator last, const T& value, Compare compare, Equality equality) {
        ForwardIterator i = LowerBound(first, last, value, compare);
        return i != last || equality(value, *i) ? i : last;
    }

    /// @brief Find the given value in a sorted range using binary search algorithm
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param value Value to compare against
    /// @return Iterator to the element that is equal to the given value in the range, or `last` if not found
    /// @ingroup algorithm
    template<typename ForwardIterator, typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline ForwardIterator BinaryFind(ForwardIterator first, ForwardIterator last, const T& value) {
        typedef typename IteratorTraits<ForwardIterator>::ValueType ValueType;
        return BinaryFind(first, last, value, Less<ValueType>(), EqualTo<ValueType>());
    }

    // Includes

    /// @brief Checks if a sorted range is a subsequence of another sorted range using a comparator (can be non-contiguous)
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param compare Binary comparator to use for comparison
    /// @return True if the first range is a subsequence of the second range, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/includes
    template<typename InputIterator1, typename InputIterator2, typename Compare>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool Includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare compare) {
        while(first2 != last2) {
            if(first1 == last1 || compare(*first2, *first1)) return false;
            if(!compare(*first1, *first2)) first2++;
            first1++;
        }

        return true;
    }

    /// @brief Checks if a sorted range is a subsequence of another sorted range (can be non-contiguous)
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @return True if the first range is a subsequence of the second range, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/includes
    template<typename InputIterator1, typename InputIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    inline bool Includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        return Includes(first1, last1, first2, last2, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }

    // Set difference

    /// @brief Computes the difference between two sorted ranges using a comparator
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param compare Binary comparator to use for comparison
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_difference
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    OutputIterator SetDifference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst, Compare compare) {
        while(first1 != last1) {
            if(first2 == last2 || compare(*first1, *first2)) *resultFirst++ = *first1++;
            else if(compare(*first2, *first1)) first2++;
            else {
                first1++;
                first2++;
            }
        }

        return resultFirst;
    }

    /// @brief Computes the difference between two sorted ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_difference
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    inline OutputIterator SetDifference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst) {
        return SetDifference(first1, last1, first2, last2, resultFirst, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }

    // Set intersection

    /// @brief Computes the intersection between two sorted ranges using a comparator
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param compare Binary comparator to use for comparison
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_intersection
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    OutputIterator SetIntersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst, Compare compare) {
        while(first1 != last1 && first2 != last2) {
            if(compare(*first1, *first2)) first1++;
            else if(compare(*first2, *first1)) first2++;
            else {
                *resultFirst++ = *first1++;
                first2++;
            }
        }

        return resultFirst;
    }

    /// @brief Computes the intersection between two sorted ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_intersection
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    inline OutputIterator SetIntersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst) {
        return SetIntersection(first1, last1, first2, last2, resultFirst, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }

    // Set symmetric difference

    /// @brief Computes the symmetric difference between two sorted ranges using a comparator
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param compare Binary comparator to use for comparison
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_symmetric_difference
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    OutputIterator SetSymmetricDifference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst, Compare compare) {
        while(first1 != last1 && first2 != last2) {
            if(compare(*first1, *first2)) *resultFirst++ = *first1++;
            else if(compare(*first2, *first1)) *resultFirst++ = *first2++;
            else {
                first1++;
                first2++;
            }
        }

        resultFirst = Copy(first1, last1, resultFirst);
        resultFirst = Copy(first2, last2, resultFirst);
        
        return resultFirst;
    }

    /// @brief Computes the symmetric difference between two sorted ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_symmetric_difference
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    inline OutputIterator SetSymmetricDifference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst) {
        return SetSymmetricDifference(first1, last1, first2, last2, resultFirst, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }

    // Set union

    /// @brief Computes the union between two sorted ranges using a comparator
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @param compare Binary comparator to use for comparison
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_union
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    OutputIterator SetUnion(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst, Compare compare) {
        while(first1 != last1 && first2 != last2) {
            if(compare(*first1, *first2)) *resultFirst++ = *first1++;
            else if(compare(*first2, *first1)) *resultFirst++ = *first2++;
            else {
                *resultFirst++ = *first1++;
                first2++;
            }
        }

        resultFirst = Copy(first1, last1, resultFirst);
        resultFirst = Copy(first2, last2, resultFirst);

        return resultFirst;
    }

    /// @brief Computes the union between two sorted ranges
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param last2 Iterator to the end of the second range
    /// @param resultFirst Iterator to the beginning of the destination range
    /// @return Iterator to the past the last element of the destination range
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/set_union
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    __WSTL_CONSTEXPR14__
    inline OutputIterator SetUnion(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator resultFirst) {
        return SetUnion(first1, last1, first2, last2, resultFirst, Less<typename IteratorTraits<InputIterator1>::ValueType>());
    }

    // Is permutation

    /// @brief Checks if two sorted ranges are permutations of each other using a predicate
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @param predicate Binary predicate to use for comparison
    /// @return True if the ranges are permutations of each other, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_permutation
    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool IsPermutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, BinaryPredicate predicate) {
        typedef typename IteratorTraits<ForwardIterator1>::DifferenceType DifferenceType;
        DifferenceType len1 = Distance(first1, last1);
        ForwardIterator2 last2 = Next(first2, len1);

        if(len1 != Distance(first2, last2)) return false;

        for(ForwardIterator1 it = first1; it != last1; it++) {
            if(FindIf(first1, it, *it, BindFirst(predicate, *it)) != it) continue;
            if(CountIf(first1, last1, *it, BindFirst(predicate, *it)) != Count(first2, last2, *it, BindFirst(predicate, *it))) return false;
        }

        return true;
    }

    /// @brief Checks if two sorted ranges are permutations of each other
    /// @param first1 Iterator to the beginning of the first range
    /// @param last1 Iterator to the end of the first range
    /// @param first2 Iterator to the beginning of the second range
    /// @return True if the ranges are permutations of each other, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/is_permutation
    template<typename ForwardIterator1, typename ForwardIterator2>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    bool IsPermutation(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
        typedef typename IteratorTraits<ForwardIterator1>::DifferenceType DifferenceType;
        DifferenceType len1 = Distance(first1, last1);
        ForwardIterator2 last2 = Next(first2, len1);

        if(len1 != Distance(first2, last2)) return false;

        for(ForwardIterator1 it = first1; it != last1; it++) {
            if(Find(first1, it, *it) != it) continue;
            if(Count(first1, last1, *it) != Count(first2, last2, *it)) return false;
        }

        return true;
    }

    // Next permutation
    /// @brief Determines the next legicographical permutation in a range using a comparator.
    /// If no permutation is found, the range is transformed into the first permutation and false is returned
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for comparison
    /// @return True if a next permutation was found, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/next_permutation
    template<typename BidirectionalIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    bool NextPermutation(BidirectionalIterator first, BidirectionalIterator last, Compare compare) {
        if (first == last || Next(first) == last) return false;
        BidirectionalIterator i = Previous(last);

        while(true) {
            i--;
            if (compare(*i, *Next(i))) {
                BidirectionalIterator j = UpperBound(Next(i), last, *i, compare);
                IteratorSwap(i, j);

                Reverse(Next(i), last);
                return true;
            }
            
            if (i == first) {
                Reverse(first, last);
                return false;
            }
        }
    }

    /// @brief Determines the next legicographical permutation in a range.
    /// If no permutation is found, the range is transformed into the first permutation and false is returned
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return True if a next permutation was found, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/next_permutation
    template<typename BidirectionalIterator>
    __WSTL_CONSTEXPR14__
    inline bool NextPermutation(BidirectionalIterator first, BidirectionalIterator last) {
        return NextPermutation(first, last, Less<typename IteratorTraits<BidirectionalIterator>::ValueType>());
    }

    // Previous permutation

    /// @brief Determines the previous legicographical permutation in a range using a comparator.
    /// If no permutation is found, the range is transformed into the last permutation and false is returned
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @param compare Binary comparator to use for comparison
    /// @return True if a previous permutation was found, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/prev_permutation
    template<typename BidirectionalIterator, typename Compare>
    __WSTL_CONSTEXPR14__
    bool PreviousPermutation(BidirectionalIterator first, BidirectionalIterator last, Compare compare) {
        if (first == last || Next(first) == last) return false;
        BidirectionalIterator i = Previous(last);

        while(true) {
            i--;
            if (compare(*Next(i), *i)) {
                BidirectionalIterator j = Previous(LowerBound(Next(i), last, *i, compare));
                IteratorSwap(i, j);

                Reverse(Next(i), last);
                return true;
            }
            
            if (i == first) {
                Reverse(first, last);
                return false;
            }
        }
    }

    /// @brief Determines the previous legicographical permutation in a range.
    /// If no permutation is found, the range is transformed into the last permutation and false is returned
    /// @param first Iterator to the beginning of the range
    /// @param last Iterator to the end of the range
    /// @return True if a previous permutation was found, false otherwise
    /// @ingroup algorithm
    /// @see https://en.cppreference.com/w/cpp/algorithm/prev_permutation
    template<typename BidirectionalIterator>
    __WSTL_CONSTEXPR14__
    inline bool PreviousPermutation(BidirectionalIterator first, BidirectionalIterator last) {
        return PreviousPermutation(first, last, Less<typename IteratorTraits<BidirectionalIterator>::ValueType>());
    }
}

#endif