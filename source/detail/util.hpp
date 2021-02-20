/**
 * @file util.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Implementation of minor utilities
 * @date 2021-02-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <limits>

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Logarithm base 2 for unsigned long, rounded down
         * 
         * @param n Value 
         * @return log2(n)
         */
        inline constexpr unsigned long log2(unsigned long n)
        {
            return (std::numeric_limits<unsigned long>::digits - 1 - __builtin_clzl(n));
        }

        /**
         * @brief Logarithm base 2, rounded down
         * 
         * @tparam value_type 
         * @param n 
         * @return constexpr value_type 
         */
        template <class value_type>
        inline constexpr value_type log2t(value_type n)
        {
            static_assert(sizeof(value_type) == 4);

            --n;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            return ++n;
        } 
    } // namespace detail
} // namespace qsmb