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
        template <class config>
        class buffer_storage : public align_pointer<void>
        {
        public:
            using difference_type = typename config::difference_type;

            static constexpr const auto size_per_thread {config::block_size * 2 * (1 + config::enable_equality_bucket)};

            inline char* get(const difference_type id) { return this->get() + id * size_per_thread; }

            buffer_storage() {}

            explicit buffer_storage(const difference_type num_threads)
                : align_pointer<void>(config::data_alignment, num_threads * size_per_thread)
            {}
        };

        template <class config>
        struct local_data
        {
            using difference_type = config::difference_type;

            difference_type bucket_size_[config::num_buckets];
            buffers buffers_;
            block swap_[2];
            block overflow_;

            void reset()
            {
                std::fill_n(bucket_size, config::num_buckets, 0);
            }

            local_data(char* buffer_storage)
                : buffers {buffer_storage};
            {}
        };

        template <class config>
        struct parallel_task
        {
            using difference_type = typename config::difference_type;

            difference_type begin;
            difference_type end;

            friend bool operator ==(const parallel_task& l, const parallel_task& r) { return l.begin == r.begin && l.end == r.end; }
            friend bool operator !=(const parallel_task& l, const parallel_task& r) { return !(l == r); }
            friend bool operator <(const parallel_task& l, const parallel_task& r) { return l.end - l.begin < r.end - r.begin; }
            friend bool operator >(const parallel_task& l, const parallel_task& r) { return r < l; }
            friend bool operator <=(const parallel_task& l, const parallel_task& r) { return !(r < l); }
            friend bool operator >=(const parallel_task& l, const parallel_task& r) { return !(l < r); }
        };

        template <class config>
        class shared_data
        {
            using size_type = std::size_t;
            using difference_type = typename config::difference_type;
            using sync = typename config::sync;

            difference_type bucket_start_[config::num_buckets + 1];
            block* overflow_;
            
            std::vector<sync> sync_;
            std::vector<local_data*> local_data_;

            void reset()
            {
                std::fill_n(bucket_start_, config::num_buckets + 1, 0);
                overflow_ = nullptr;
            }

            shared_data(size_type num_threads)
                : sync(num_threads), local(num_threads)
            {
                reset();
            }
        };
    } // namespace detail
} // namespace qsmb