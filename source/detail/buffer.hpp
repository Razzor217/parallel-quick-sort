/**
 * @file buffer.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of buffer storage
 * @date 2021-02-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

#include "block.hpp"

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Wrapper class for buffered storage for each thread
         * 
         * @tparam config Sorter configuration
         */
        template <class config>
        class buffers
        {
        public:
            /**
             * @brief Iterator of the input data
             * 
             */
            using iterator = typename config::iterator;

            /**
             * @brief Value type of the input data
             * 
             */
            using value_type = typename config::value_type;

            /**
             * @brief Difference type of the iterator
             * 
             */
            using difference_type = typename config::difference_type;

            using pointer = value_type*;
            using const_pointer = const pointer;

            /**
             * @brief Check whether a buffer is full
             * 
             * @param i Buffer index
             * @return true If the buffer is full
             * @return false Otherwise
             */
            inline bool is_full(const difference_type i) 
            { 
                return buffer_[i].ptr == buffer_[i].end; 
            }

            /**
             * @brief Returns a pointer to the buffer data
             * 
             * @param i Buffer index
             * @return pointer Pointer to the buffer data
             */
            inline pointer data(const difference_type i)
            {
                return static_cast<pointer>(static_cast<void*>(storage_)) + i * config::block_size;
            }

            /**
             * @brief Returns the size of the selected buffer
             * 
             * @param i Buffer index
             * @return difference_type Size of the buffer 
             */
            inline difference_type size(const difference_type i)
            {
                return config::block_size - (buffer_[i].end - buffer_[i].ptr);
            }

            /**
             * @brief Reset the selected buffer
             * 
             * @param i Buffer index
             */
            void reset(const difference_type i)
            {
                if (block::destruct_storage)
                {
                    for (auto ptr {data(i)}, end {ptr + size(i)}; ptr != end; ++ptr)
                    {
                        ptr->~value_type();
                    }
                }
                reset_buffer(i);
            }

            /**
             * @brief Push an element to the selected buffer
             * 
             * @param i Buffer index
             * @param value Value to insert
             */
            void push(const difference_type i, value_type&& value)
            {
                if (block::initialized_storage)
                {
                    *buffer_[i].ptr++ = std::move(value);
                }
                else
                {
                    new (buffer_[i].ptr++) value_type(std::move(value));
                }
            }

            /**
             * @brief Write the selected buffer to `dest`
             * 
             * @param i Buffer index
             * @param dest Iterator to the data destination
             */
            void write_to(const difference_type i, iterator dest)
            {
                std::move(data(i), data(i) + config::block_size, std::move(dest));
                reset(i);
            }

            buffers(char* storage) :
                storage_ {static_cast<block*>(static_cast<void*>(storage))}
            {
                for (difference_type i {}; i < config::num_buckets; ++i)
                {
                    reset_buffer(i);
                    buffer_[i].end = buffer_[i].ptr + config::block_size;
                }
            }

        private:            
            struct info
            {
                pointer ptr;
                const_pointer end;
            };

            inline void reset_buffer(const difference_type i)
            {
                buffer_[i].ptr = static_cast<pointer>(static_cast<void*>(storage_)) + i * config::block_size;
            }

        private:
            info buffer_[config::num_buckets];

            block* storage_;
        };
    } // namespace detail
} // namespace qsmb