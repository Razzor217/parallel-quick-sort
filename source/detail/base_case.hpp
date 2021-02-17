/**
 * @file base_case.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definitions for base case sorting
 * @date 2021-02-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>

namespace detail
{
    /**
     * @brief Performs insertion sort on an input sequence specified by the 
     * range [`begin`, `end`).
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @tparam comparator Comparison function object
     * @param begin Iterator to the begin of the input sequence
     * @param end Iterator to the end of the input sequence
     * @param cmp Comparison function object to use
     */
    template <class random_access_iterator, class comparator>
    void insertion_sort(const random_access_iterator begin,
                        const random_access_iterator end,
                        comparator cmp)
    {
        for (random_access_iterator i {begin + 1}; i < end; ++i)
        {
            /*
             * - If e is smaller than all previously inserted elements, 
             *   insert e at the front.
             * - Otherwise, scan the sorted section from right to left while 
             *   e is smaller than the current element (terminates because first 
             *   element is smaller than or equal to e). 
             */
            const typename std::iterator_traits<random_access_iterator>::value_type e {std::move(*i)};
            if (cmp(e, *begin))
            {
                for (random_access_iterator j {i}; j > begin; --j)
                {
                    *j = std::move(*(j - 1));
                }
                *begin = std::move(e);
            }
            else
            {
                random_access_iterator j {i};
                for (; cmp(e, *(j - 1)); --j)
                {
                    *j = std::move(*(j - 1));
                }
                *j = std::move(e);
            }
        }
    }

    /**
     * @brief Performs insertion sort on an input sequence specified by the 
     * range [`begin`, `end`).
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @param begin Iterator to the begin of the input sequence
     * @param end Iterator to the end of the input sequence
     */
    template <class random_access_iterator>
    inline void insertion_sort(random_access_iterator begin,
                               random_access_iterator end)
    {
        using value_type = typename std::iterator_traits<random_access_iterator>::value_type;
        insertion_sort(std::move(begin), std::move(end), std::less<value_type>());
    }

    /**
     * @brief Invokes a sorter suitable for small sequences on the input 
     * specified by the range [`begin`, `end`).
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @tparam comparator Comparison function object
     * @param begin Iterator to the begin of the input sequence
     * @param end Iterator to the end of the input sequence
     * @param cmp Comparison function object
     */
    template <class random_access_iterator, class comparator>
    inline void small_sort(random_access_iterator begin,
                           random_access_iterator end,
                           comparator&& cmp)
    {
        if (begin == end)
        {
            return;
        }
        detail::insertion_sort(std::move(begin), std::move(end), std::forward<comparator>(cmp));
    }

    /**
     * @brief Invokes a sorter suitable for small sequences on the input 
     * specified by the range [`begin`, `end`).
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @param begin Iterator to the begin of the input sequence
     * @param end Iterator to the end of the input sequence
     */
    template <class random_access_iterator>
    inline void small_sort(random_access_iterator begin, 
                           random_access_iterator end)
    {
        using value_type = typename std::iterator_traits<random_access_iterator>::value_type;
        detail::small_sort(std::move(begin), std::move(end), std::less<value_type>());
    }

    /**
     * @brief Checks whether an input sequence specified by the range 
     * [`begin`, `end`) is (reverse) sorted already.
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @tparam comparator Comparison function object
     * @param begin Iterator to the begin of the input sequence
     * @param end Iterator to the end of the input sequence
     * @param cmp Comparison function object
     * @return true if the input is (reverse) sorted
     * @return false otherwise
     */
    template <class random_access_iterator, class comparator>
    bool check_sorted(random_access_iterator begin,
                      random_access_iterator end, 
                      comparator&& cmp)
    {
        if (begin == end)
        {
            return true;
        }

        /*
         * - If last element is greater than or equal to the first element, 
         *   the input cannot be reverse sorted. In this case, check whether 
         *   the input sequence is sorted.
         * - Otherwise, check if the input sequence is reverse sorted. In this 
         *   case, reverse the input sequence.
         */
        if (!cmp(*(end - 1), *begin))
        {
            return std::is_sorted(begin, end, cmp);
        }
        else
        {
            for (random_access_iterator it {begin}; it != end; ++it)
            {
                if (cmp(*it, *(it + 1)))
                {
                    return false;
                }
            }

            std::reverse(begin, end);
            return true;
        }
    }

    /**
     * @brief Checks whether an input sequence specified by the range 
     * [`begin`, `end`) is (reverse) sorted already.
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @param begin Iterator to the begin of the input sequence
     * @param end Iterator to the end of the input sequence
     * @return true if the input is (reverse) sorted
     * @return false otherwise
     */
    template <class random_access_iterator>
    inline bool check_sorted(random_access_iterator begin, 
                             random_access_iterator end)
    {
        using value_type = typename std::iterator_traits<random_access_iterator>::value_type;
        return detail::check_sorted(std::move(begin), std::move(end), std::less<value_type>());
    }
} // namespace detail