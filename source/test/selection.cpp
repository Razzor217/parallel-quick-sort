#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "../detail/selection.hpp"

namespace test
{
    bool test_median_of_three()
    {
        std::vector<int> data {0, 1, 2};
        return detail::median_of_three(data.begin(), data.begin() + 1, data.begin() + 2) == (data.begin() + 1);
    }

    bool test_pseudo_median_of_nine()
    {
        std::vector<int> data {0, 1, 2, 3, 4, 5, 6, 7, 8};
        return detail::pseudo_median_of_nine(data.begin(), data.end()) == (data.begin() + 4);
    }
}

int main(int argc, char** argv)
{

    const bool test1 {test::test_median_of_three()};
    const bool test2 {test::test_pseudo_median_of_nine()};

    if (test1)
    {
        std::cout << "median of three successful" << std::endl;
    }
    else
    {
        std::cout << "median of three failed" << std::endl;
    }

    if (test2)
    {
        std::cout << "pseudo median of nine successful" << std::endl;
    }
    else
    {
        std::cout << "pseudo median of nine failed" << std::endl;
    }
    
    return 0;
}