#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "../detail/base_case.hpp"

namespace test
{
    bool test_insertion_sort()
    {
        std::vector<int> data(128);
        std::iota(data.begin(), data.end(), 0);

        std::random_device rd;
        std::mt19937 generator(rd());

        std::shuffle(data.begin(), data.end(), generator); 

        detail::insertion_sort(data.begin(), data.end());
        return std::is_sorted(data.begin(), data.end());
    }

    bool test_check_sorted()
    {
        std::vector<int> data(128);

        std::iota(data.begin(), data.end(), 0);
        if (!detail::check_sorted(data.begin(), data.end()))
        {
            return false;
        }

        std::iota(data.rbegin(), data.rend(), 0);
        if (!detail::check_sorted(data.begin(), data.end()))
        {
            return false;
        }

        std::iota(data.begin(), data.end(), 0);
        data[0] = 127;
        if (detail::check_sorted(data.begin(), data.end()))
        {
            return false;
        }

        std::iota(data.begin(), data.end(), 0);
        data[0] = 128;
        if (detail::check_sorted(data.begin(), data.end()))
        {
            return false;
        }

        return true;
    }
}

int main(int argc, char** argv)
{

    const bool test1 {test::test_insertion_sort()};
    const bool test2 {test::test_check_sorted()};

    if (test1)
    {
        std::cout << "insertion sort successful" << std::endl;
    }
    else
    {
        std::cout << "insertion sort failed" << std::endl;
    }

    if (test2)
    {
        std::cout << "sorted check successful" << std::endl;
    }
    else
    {
        std::cout << "sorted check failed" << std::endl;
    }

    return 0;
}