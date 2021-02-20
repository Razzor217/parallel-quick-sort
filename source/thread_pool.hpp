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
    /**
     * @brief Wrapper for std::thread providing thread pool functionality
     * 
     */
    class std_thread_pool
    {
    public:
        /**
         * @brief Synchronization mechanism used by the std::thread pool
         * 
         */
        using sync = detail::barrier;

        /**
         * @brief Function call operator, invokes `func` on every thread of the pool
         * 
         * @tparam F Target function type to invoke
         * @param func Target function to invoke
         * @param num_threads Number of threads of the std::thread pool
         */
        template <class F>
        void operator ()(F&& func, int num_threads = std::numeric_limits<int>::max())
        {
            num_threads = std::min(num_threads, num_threads());
            if (num_threads > 1)
            {
                impl_->run(std::forward<F>(func), num_threads);
            }
            else
            {
                func(0, 1);
            }
        }

        /**
         * @brief Get the synchronization object used by the std::thread pool
         * 
         * @return sync& Reference to the synchronization object
         */
        inline sync& get_sync() { return impl_->get_sync(); }

        /**
         * @brief Get the number of available threads in the std::thread pool 
         * 
         * @return int Number of available threads
         */
        inline int num_threads() { return impl_->num_threads(); }

        /**
         * @brief Get the maximum (possible) number of available threads
         * 
         * @return int Maximum number of available threads
         */
        inline static int max_num_threads() { return std::thread::hardware_concurrency(); }

        /**
         * @brief Construct a new std thread pool object
         * 
         * @param num_threads Number of threads maintained by the std::thread pool
         */
        explicit std_thread_pool(int num_threads = std_thread_pool::max_num_threads()) :
            impl_ {new impl {num_threads}}
        {}

    private:
        class impl
        {
        public:
            /**
             * @brief Main execution loop of all threads in the std::thread pool
             * 
             * @param thread_id ID of the executing thread
             */
            void main_loop(const int thread_id)
            {
                for (;;)
                {
                    /*
                     * wait at barrier until main thread executes @see impl::run() 
                     */
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

            /**
             * @brief Entry point for parallel execution
             * 
             * @tparam F Function type to invoke
             * @param func Function to invoke on every thread of the std::thread pool
             * @param num_threads Number of threads to invoke `func` on 
             */
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

            /**
             * @brief Get the syncronization object used by the std::thread pool
             * 
             * @return sync& Reference to the synchronization object
             */
            inline sync& get_sync() { return sync_; }

            /**
             * @brief Get the number of threads maintained by the std::thread pool
             * 
             * @return int Number of maintained threads
             */
            inline int num_threads() { return threads_.size() + 1; }

            /**
             * @brief Construct a new impl object
             * 
             * @param num_threads Number of threads maintained by the std::thread pool
             */
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

            /**
             * @brief Destroy the impl object 
             * 
             */
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