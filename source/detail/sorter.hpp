/**
 * @file sorter.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of a helper class for sorting
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <iterator>

namespace detail
{
    template <class config>
    class sorter
    {
    public:
        using iterator = config::iterator;
        using difference_type = config::difference_type;
        using value_type = config::value_type;
        using less = config::less;

        /**
         * @brief Effiecient sequential implementation of quick sort
         * 
         * @param begin Iterator to the begin of the thread-local input
         * @param end Iterator to the end of the thread-local input
         */
        void sequential(const iterator begin, const iterator end);

        /*
         * TODO:
         * complete constructor
         */ 

        /**
         * @brief Construct a new sorter object
         * 
         * @param cmp Comparison function object
         */
        sorter(less cmp) :
            cmp_ {std::move(cmp)}
        {}

    private:
        less cmp_;
    };
} // namespace detail