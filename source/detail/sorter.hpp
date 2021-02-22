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

namespace qsmb
{
    namespace detail
    {
        template <class config>
        class sorter
        {
        public:
            /**
             * @brief Iterator type for the input data
             * 
             */
            using iterator = typename config::iterator;

            /**
             * @brief Difference type of the iterator
             * 
             */
            using difference_type = typename config::difference_type;

            /**
             * @brief Value type of the input data
             * 
             */
            using value_type = typename config::value_type;

            /**
             * @brief Comparison function object
             * 
             */
            using less = typename config::less;

            /**
             * @brief Effiecient sequential implementation of quick sort
             * 
             * @param begin Iterator to the begin of the thread-local input
             * @param end Iterator to the end of the thread-local input
             */
            void sequential(iterator begin, iterator end);

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
} // namespace qsmb