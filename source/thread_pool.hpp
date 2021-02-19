/**
 * @file thread_pool.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of a thread pool using `std::thread`
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <thread>
#include <limits>
#include <memory>

#include "detail/barrier.hpp"

namespace qsmb
{
    class std_thread_pool
    {
    public:
        using sync = detail::barrier;

        int num_threads() {return 0; }

        template <class F>
        void operator ()(F&& func, int num_threads = std::numeric_limits<int>::max())
        {
            num_threads = std::min(num_threads, num_threads());
            if (num_threads > 1)
            {

            }
            else
            {
                func(0, 1);
            }
        }

    private:
        class impl
        {
        public:
            sync sync_;
            detail::barrier pool_barrier;
        };

        std::unique_ptr<impl> impl_;
    };
} // namespace qsmb