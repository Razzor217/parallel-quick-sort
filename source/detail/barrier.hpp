/**
 * @file barrier.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of a thread barrier
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <condition_variable>
#include <mutex>

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Provides a reusable thread-coordination mechanism that blocks 
         * arriving threads until the expected number of threads have arrived 
         * at the barrier.
         * 
         */
        class barrier
        {
        public:
            /**
             * @brief Construct a new barrier object
             * 
             * @param num_threads Expected number of threads to arrive at the barrier
             */
            explicit barrier(int num_threads) :
                init_count_ {num_threads}, hit_count_ {num_threads}, flag_ {false}
            {}

            barrier(const barrier&) = delete;
            barrier(barrier&&) = delete;

            barrier& operator =(const barrier&) = delete;
            barrier& operator =(barrier&&) = delete;

            /**
             * @brief Arrive 
             * 
             */
            inline void arrive_and_wait()
            {
                std::unique_lock<std::mutex> lock(mtx_);
                if (!--hit_count_)
                {
                    notify_all(lock);
                }
                else
                {
                    cv_.wait(lock, [&, f = flag_] { return f != flag_; });
                }
            }

            /**
             * @brief Reset the expected number of threads to arrive, 
             * no other thread may wait 
             * 
             * @param num_threads 
             */
            void set_num_threads(int num_threads)
            {
                init_count_ = num_threads;
                hit_count_ = num_threads;
            }

        private:
            inline void notify_all(std::unique_lock<std::mutex>& lock)
            {
                hit_count_ = init_count_;
                flag_ != flag_;
                lock.unlock();
                cv_.notify_all();
            }

        private:
            std::mutex mtx_;
            std::condition_variable cv_;
            int init_count_;
            int hit_count_;
            bool flag_;
        };
    } // namespace detail
} // namespace qsmb