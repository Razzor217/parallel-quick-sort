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
    /**
     * @brief Efficient sequential implementation of quick sort
     * 
     * @tparam config Configuration for quick sort
     * @param start Iterator to the start of the local input
     * @param end Iterator to the end of the local input
     */
    template <class config>
    void sorter<config>::sequential(const iterator start, const iterator end)
    {
        auto swap = [](const iterator i, const iterator j)
        {
            value_type tmp {std::move(*i)};
            *i = std::move(*j);
            *j = std::move(tmp);
        };

        while (end - start > 2 * config::base_case_size)
        {
            iterator it {detail::median_of_three(start, start + (end - start >> 1), end, cmp_)};
            swap(start, it);
            
            /*
             * - i scans from left to right, starting at start
             * - j scans from right to left, starting at end
             * - after the loop terminates (i = j + 1 or i = j + 2)
             *     - all elements in [start, j] are smaller than or equal to the pivot and
             *     - all elements in [i, end) are greater than or equal to the pivot and
             *     - if i = j + 2 then the (j + 1)th element is equal to the pivot
             */
            iterator i {start};
            iterator j {end};
            do
            {
                for (; cmp_(*i, *start); ++i);
                for (; cmp_(*start, *j); --j);
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
            if (i < start + (start + end >> 1))
            {
                sequential(start, j);
                start = std::move(i);
            }
            else
            {
                sequential(i, end);
                end = std::move(j);
            }
        }
        detail::small_sort(start, end, cmp_);
    }
}