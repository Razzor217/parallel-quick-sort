#define BOOST_TEST_MODULE detail::selection

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "../detail/selection.hpp"

BOOST_AUTO_TEST_CASE(test_median_of_three_middle1)
{
    std::vector<int> data = {0, 1, 2};

    auto it = qsmb::detail::median_of_three(data.begin(),
                                            data.begin() + 1,
                                            data.begin() + 2);
    BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_median_of_three_middle2)
{
    std::vector<int> data = {2, 1, 0};

    auto it = qsmb::detail::median_of_three(data.begin(),
                                            data.begin() + 1,
                                            data.begin() + 2);
    BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_median_of_three_left1)
{
    std::vector<int> data = {1, 0, 2};

    auto it = qsmb::detail::median_of_three(data.begin(),
                                            data.begin() + 1,
                                            data.begin() + 2);
    BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_median_of_three_left2)
{
    std::vector<int> data = {1, 2, 0};

    auto it = qsmb::detail::median_of_three(data.begin(),
                                            data.begin() + 1,
                                            data.begin() + 2);
    BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_median_of_three_right1)
{
    std::vector<int> data = {1, 0, 2};

    auto it = qsmb::detail::median_of_three(data.begin(),
                                            data.begin() + 1,
                                            data.begin() + 2);
    BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_median_of_three_right2)
{
    std::vector<int> data = {1, 2, 0};

    auto it = qsmb::detail::median_of_three(data.begin(),
                                            data.begin() + 1,
                                            data.begin() + 2);
    BOOST_CHECK_EQUAL(*it, 1);
}

/*
 * TODO:
 * add tests for median of nine
 */