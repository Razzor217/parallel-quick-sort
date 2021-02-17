/**
 * @file parallel_sort.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of parallel quick sort interface methods
 * @date 2021-02-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "detail/base_case.hpp"
#include "detail/sorter.hpp"
#include "detail/sequential.hpp"
#include "configuration.hpp"

/**
 * @brief Helper function for creating reusable sorters
 * 
 * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
 * @tparam comparator Comparison function object
 * @tparam config Sorter configuration
 * @param cmp Comparison function object
 * @return Sequential sorter with the user configuration 
 */
template <class random_access_iterator,
          class comparator = std::less<>,
          class config = configuration<>>
sequential_sorter<extended_configuration<random_access_iterator, comparator, config>>
make_sorter(comparator cmp = {})
{
    return sequential_sorter<extended_configuration<random_access_iterator, comparator, config>> {true, std::move(cmp)};
}

/**
 * @brief Configurable interface for sequential quick sort
 * 
 * @tparam config Sorter configuration
 * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
 * @tparam comparator Comparison function object
 * @param begin Iterator to the begin of the input data
 * @param end Iterator to the end of the input data
 * @param cmp Comparison function object
 */
template <class config,
          class random_access_iterator,
          class comparator = std::less<>>
void sort(const random_access_iterator begin,
          const random_access_iterator end,
          comparator cmp = {})
{
    if (detail::check_sorted(begin, end, cmp))
    {
        return;
    }

    if (end - begin <= config::base_case_multiplier * config::base_case_size)
    {
        detail::small_sort(std::move(begin), std::move(end), std::move(cmp));
    }
    else
    {
        sequential_sorter<extended_configuration<random_access_iterator, comparator, config>>(false, std::move(cmp))
            (std::move(begin), std::move(end));
    }
}

/**
 * @brief Standard interface for sequential quick sort
 * 
 * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
 * @tparam comparator Comparison function object
 * @param begin Iterator to the begin of the input data
 * @param end Iterator to the end of the input data
 * @param cmp Comparison function object
 */
template <class random_access_iterator, class comparator>
void sort(const random_access_iterator begin,
          const random_access_iterator end,
          comparator cmp)
{
    sort<configuration<>>(std::move(begin), std::move(end), std::move(cmp));
}

/**
 * @brief Standard interface for sequential quick sort
 * 
 * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
 * @param begin Iterator to the begin of the input data
 * @param end Iterator to the end of the input data
 */
template <class random_access_iterator>
void sort(const random_access_iterator begin,
          const random_access_iterator end)
{
    sort<configuration<>>(std::move(begin), std::move(end), std::less<>());
}

namespace parallel
{

} // namespace parallel