#define BOOST_TEST_MODULE detail::base_case

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "../detail/base_case.hpp"

BOOST_AUTO_TEST_CASE(test_small_sort)
{
    std::vector<int> data(128);

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(data.begin(), data.end(), generator);

    detail::small_sort(data.begin(), data.end());
    BOOST_CHECK(std::is_sorted(data.begin(), data.end()));
}

BOOST_AUTO_TEST_CASE(test_check_sorted)
{
    std::vector<int> data(128);
    std::iota(data.begin(), data.end(), 0);

    BOOST_CHECK(std::is_sorted(data.begin(), data.end()));
    BOOST_CHECK(std::is_sorted(data.rbegin(), data.rend()));

    data[0] = 128;
    BOOST_CHECK(std::is_sorted(data.begin(), data.end()));
}