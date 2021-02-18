#define BOOST_TEST_MODULE detail::sequential

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "../parallel_sort.hpp"

BOOST_AUTO_TEST_CASE(test_sequential_quick_sort)
{
    std::vector<int> data(128);
    std::iota(data.begin(), data.end(), 0);

    qsmb::sort(data.begin(), data.end());
    BOOST_CHECK(std::is_sorted(data.begin(), data.end()));
}