/**
 * @file selection.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definitions of algorithms for pivot selection
 * @date 2021-02-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cstddef>
#include <iterator>
#include <functional>

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Computes the median of `left`, `middle` and `right`
         * 
         * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
         * @tparam comparator Comparison function object
         * @param left Iterator to the left value
         * @param middle Iterator to the middle value
         * @param right Iterator to the right value
         * @param cmp Comparison function object
         * @return random_access_iterator Median of `left`, `middle` and `right`
         */
        template <class random_access_iterator, class comparator>
        inline random_access_iterator
        median_of_three(random_access_iterator left,
                        random_access_iterator middle,
                        random_access_iterator right,
                        comparator cmp)
        {
            return (cmp(*middle, *left) ^ cmp(*middle, *right)) ? middle :
                ((cmp(*left, *middle) ^ cmp(*left, *right)) ? left : right);
        }

        /**
         * @brief Computes the median of `left`, `middle` and `right`
         * 
         * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
         * @param left Iterator to the left value
         * @param middle Iterator to the middle value
         * @param right Iterator to the right value
         * @return random_access_iterator Median of `left`, `middle` and `right`
         */
        template <class random_access_iterator>
        inline random_access_iterator
        median_of_three(random_access_iterator left,
                        random_access_iterator middle,
                        random_access_iterator right)
        {
            using value_type = typename std::iterator_traits<random_access_iterator>::value_type;
            return detail::median_of_three(std::move(left),
                                        std::move(middle),
                                        std::move(right),
                                        std::less<value_type>());
        }

        /**
         * @brief Computes the pseudo median of nine of the range [`begin`, `end`)
         * 
         * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
         * @tparam comparator Comparison function object
         * @param begin Iterator to the begin of the input sequence
         * @param end Iterator to the end of the input sequence
         * @param cmp Comparison function object
         * @return random_access_iterator Pseudo median of nine of [`begin`, `end`)
         */
        template <class random_access_iterator, class comparator>
        inline random_access_iterator
        pseudo_median_of_nine(random_access_iterator begin,
                              random_access_iterator end,
                              comparator cmp)
        {
            const std::ptrdiff_t d {(end - begin) >> 3};
            return detail::median_of_three(detail::median_of_three(begin, begin + d, begin + 2 * d, cmp),
                                        detail::median_of_three(begin + 3 * d, begin + 4 * d, begin + 5 * d, cmp),
                                        detail::median_of_three(begin + 6 * d, begin + 7 * d, end - 1),
                                        cmp);
        }

        /**
         * @brief Computes the pseudo median of nine of the range [`begin`, `end`)
         * 
         * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
         * @param begin Iterator to the begin of the input sequence
         * @param end Iterator to the end of the input sequence
         * @return random_access_iterator Pseudo median of nine of [`begin`, `end`)
         */
        template <class random_access_iterator>
        inline random_access_iterator
        pseudo_median_of_nine(random_access_iterator begin,
                              random_access_iterator end)
        {
            using value_type = typename std::iterator_traits<random_access_iterator>::value_type;
            return detail::pseudo_median_of_nine(std::move(begin),
                                                std::move(end),
                                                std::less<value_type>());
        }
    } // namespace detail
} // namespace qsmb