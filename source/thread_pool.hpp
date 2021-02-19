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
#include <memory>
#include <functional>

#include <limits>
#include <vector>

#include "detail/barrier.hpp"

namespace qsmb
{
    class std_thread_pool
    {
    public:
        using sync = detail::barrier;

        template <class F>
        void operator ()(F&& func, int num_threads = std::numeric_limits<int>::max())
        {
            num_threads = std::min(num_threads, num_threads());
            if (num_threads > 1)
            {
                impl_->run(func, num_threads);
            }
            else
            {
                func(0, 1);
            }
        }

        inline sync& get_sync() { return impl_->get_sync(); }
        inline int num_threads() { return impl_->num_threads(); }

    private:
        class impl
        {
        public:
            void main_loop(const int thread_id)
            {
                for (;;)
                {
                    barrier_.arrive_and_wait();
                    if (done_)
                    {
                        break;
                    }
                    if (thread_id < num_threads_)
                    {
                        func_(thread_id, num_threads_);
                    }
                    barrier_.arrive_and_wait();
                }
            }

            template <class F>
            void run(F&& func, const int num_threads)
            {
                func_ = func;
                num_threads_ = num_threads;
                sync_.set_num_threads(num_threads_);

                /*
                 * Start parallel execution
                 */
                barrier_.arrive_and_wait();
                func_(0, num_threads_);
                barrier_.arrive_and_wait();
            }

            inline sync& get_sync() { return sync_; }
            inline int num_threads() { return threads_.size() + 1; }

            impl(const int num_threads) :
                sync_ {std::max(1, num_threads)},
                barrier_ {std::max(1, num_threads)},
                num_threads_ {num_threads}
            {
                threads_.reserve(num_threads_ - 1);
                for (int i {1}; i < num_threads_; ++i)
                {
                    threads_.emplace_back(&impl::main_loop, this, i);
                }
            }

            ~impl()
            {
                done_ = true;
                barrier_.arrive_and_wait();
                for (auto& thread : threads_)
                {
                    thread.join();
                }
            }

        private:
            sync sync_;
            detail::barrier barrier_;
            std::vector<std::thread> threads_;
            std::function<void(int, int)> func_;
            int num_threads_;
            bool done_;
        };

        std::unique_ptr<impl> impl_;
    };
} // namespace qsmb