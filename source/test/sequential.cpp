#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#include "../parallel_sort.hpp"

namespace test
{
    bool test_quick_sort()
    {
        std::vector<int> data(128);
        std::iota(data.begin(), data.end(), 0);

        std::random_device rd;
        std::mt19937 generator(rd());

        std::shuffle(data.begin(), data.end(), generator);

        dummy::sort(data.begin(), data.end());
        return std::is_sorted(data.begin(), data.end());
    }
}

int main(int argc, char** argv)
{
    const bool test {test::test_quick_sort()};

    if (test)
    {
        std::cout << "sequential quick sort successful" << std::endl;
    }
    else
    {
        std::cout << "sequential quick sort failed" << std::endl;
    }

    return 0;
}