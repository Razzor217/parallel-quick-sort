/**
 * @file memory.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of thread-local and shared data 
 * @date 2021-02-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "aligned_ptr.hpp"
#include "block.hpp"
#include "buffer.hpp"

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Provides buffer storage for a specified number of threads
         * 
         * @tparam config Sorter configuration
         */
        template <class config>
        class buffer_storage : public aligned_ptr<void>
        {
        public:
            /**
             * @brief Difference type of the configuration
             * 
             */
            using difference_type = typename config::difference_type;

            /**
             * @brief Number of buffer elements per thread for which storage is allocated 
             * 
             */
            static constexpr const auto size_per_thread {config::block_size * 2 * (1 + config::enable_equality_bucket)};

            /**
             * @brief Returns a pointer to the buffer storage of the specified thread
             * 
             * @param id Thread ID
             * @return char* Pointer to the buffer storage
             */
            inline char* get(const difference_type id) { return this->get() + id * size_per_thread; }

            /**
             * @brief Default constructor
             * 
             */
            buffer_storage() {}

            /**
             * @brief Allocates storage for the specified number of threads
             * 
             * @param num_threads Number of threads
             */
            explicit buffer_storage(const difference_type num_threads)
                : aligned_ptr<void>(config::data_alignment, num_threads * size_per_thread)
            {}
        };

        /**
         * @brief Thread-local data
         * 
         * @tparam config Sorter configuration
         */
        template <class config>
        struct local_data
        {
            /**
             * @brief Difference type of the configuration
             * 
             */
            using difference_type = config::difference_type;

            /**
             * @brief Array of current bucket sizes
             * 
             */
            difference_type bucket_size_[config::num_buckets];
            
            /**
             * @brief Thread-local buffers for each bucket
             * 
             */
            buffers buffers_;

            /**
             * @brief Memory blocks used to swap blocks from/to the input data
             * 
             */
            block swap_[2];

            /**
             * @brief Overflow block
             * 
             */
            block overflow_;

            /**
             * @brief Resets the thread-local data
             * 
             */
            void reset()
            {
                std::fill_n(bucket_size, config::num_buckets, 0);
            }

            /**
             * @brief Construct thread-local data at the specified storage
             * 
             * @param buffer_storage Storage for the thread-local buffers
             */
            local_data(char* buffer_storage)
                : buffers {buffer_storage};
            {}
        };

        /**
         * @brief Task for sorter threads running in parallel
         * 
         * @tparam config Sorter configuration
         */
        template <class config>
        struct parallel_task
        {
            /**
             * @brief Difference type of the configuration
             * 
             */
            using difference_type = typename config::difference_type;

            /**
             * @brief Beginning of the section of the input data associated with this task
             * 
             */
            difference_type begin;

            /**
             * @brief End of the section of the input data associated with this task
             * 
             */
            difference_type end;

            friend bool operator ==(const parallel_task& l, const parallel_task& r) { return l.begin == r.begin && l.end == r.end; }
            friend bool operator !=(const parallel_task& l, const parallel_task& r) { return !(l == r); }
            friend bool operator <(const parallel_task& l, const parallel_task& r) { return l.end - l.begin < r.end - r.begin; }
            friend bool operator >(const parallel_task& l, const parallel_task& r) { return r < l; }
            friend bool operator <=(const parallel_task& l, const parallel_task& r) { return !(r < l); }
            friend bool operator >=(const parallel_task& l, const parallel_task& r) { return !(l < r); }
        };

        /**
         * @brief Data shared between sorter threads
         * 
         * @tparam config Sorter configuration
         */
        template <class config>
        class shared_data
        {
            /**
             * @brief Size type used
             * 
             */
            using size_type = std::size_t;

            /**
             * @brief Difference type used by the configuration
             * 
             */
            using difference_type = typename config::difference_type;
            
            /**
             * @brief Synchronization mechanism for the sorter threads
             * 
             */
            using sync = typename config::sync;

            /**
             * @brief Start of global buckets 
             * 
             */
            difference_type bucket_start_[config::num_buckets + 1];
            
            /**
             * @brief Overflow bucket
             * 
             */
            block* overflow_;
            
            /**
             * @brief Synchronization mechanism for the sorter threads
             * 
             */
            sync sync_;

            /**
             * @brief Pointers to thread-local data
             * 
             */
            std::vector<local_data*> local_data_;

            /**
             * @brief Resets the shared data
             * 
             */
            void reset()
            {
                std::fill_n(bucket_start_, config::num_buckets + 1, 0);
                overflow_ = nullptr;
            }

            /**
             * @brief Construct shared data for a specified number of threads
             * 
             * @param sync_ Specified synchronization object
             * @param num_threads Number of threads to share this dataset
             */
            shared_data(sync sync_, size_type num_threads)
                : sync(std::forward<sync>(sync_)), local(num_threads)
            {
                reset();
            }
        };
    } // namespace detail
} // namespace qsmb