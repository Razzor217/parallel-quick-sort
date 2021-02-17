/**
 * @file sequential.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of sequential quick sort
 * @date 2021-02-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "sorter.hpp"
#include "base_case.hpp"
#include "selection.hpp"

namespace detail
{
    template <class config>
    void sorter<config>::sequential(const iterator begin, const iterator end)
    {
        auto swap = [](const iterator i, const iterator j)
        {
            value_type tmp {std::move(*i)};
            *i = std::move(*j);
            *j = std::move(tmp);
        };

        while (end - begin > 2 * config::base_case_size)
        {
            iterator it {detail::median_of_three(begin, begin + (end - begin >> 1), end, cmp_)};
            swap(begin, it);
            
            /*
             * - i scans from left to right, starting at begin
             * - j scans from right to left, starting at end
             * - after the loop terminates (i = j + 1 or i = j + 2)
             *     - all elements in [begin, j] are smaller than or equal to the pivot and
             *     - all elements in [i, end) are greater than or equal to the pivot and
             *     - if i = j + 2 then the (j + 1)th element is equal to the pivot
             */
            iterator i {begin};
            iterator j {end};
            do
            {
                for (; cmp_(*i, *begin); ++i);
                for (; cmp_(*begin, *j); --j);
                if (i <= j)
                {
                    swap(i, j);
                    ++i;
                    --j;
                }
            } while (i <= j);
            
            /*
             * Recurse on smaller partition
             */
            if (i < begin + (begin + end >> 1))
            {
                sequential(begin, j);
                begin = std::move(i);
            }
            else
            {
                sequential(i, end);
                end = std::move(j);
            }
        }
        detail::small_sort(begin, end, cmp_);
    }
} // namespace detail

/**
 * @brief Helper class for sequential quick sort
 * 
 * @tparam config Sorter configuration, @see configuration
 */
template <class config>
class sequential_sorter
{
public:
    /**
     * @brief Sorter base class with user configuration
     * 
     */
    using sorter = detail::sorter<config>;

    /**
     * @brief Iterator type for the input data
     * 
     */
    using iterator = config::iterator;

    /**
     * @brief Comparison function object
     * 
     */
    using less = config::less;

    /**
     * @brief Function call operator, invokes sequential quick sort
     * 
     * @param begin Iterator to the begin of the input data
     * @param end Iterator to the end of the input data
     */
    void operator()(const iterator begin, const iterator end)
    {
        if (check_sorted_ && detail::check_sorted(begin, end, cmp_))
        {
            return;
        }
        sorter(cmp_).sequential(begin, end);
    }

    /**
     * @brief Construct a new sequential sorter object
     * 
     * @param check_sorted Enables check whether input data is sorted
     * @param cmp Comparison function object
     */
    sequential_sorter(const bool check_sorted, less cmp) :
        check_sorted_ {check_sorted}, cmp_ {std::move(cmp)}
    {}

private:
    const bool check_sorted_;
    less cmp_;
};