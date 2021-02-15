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
    };
}