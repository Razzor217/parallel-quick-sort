/**
 * @file block.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of wrappers for blocks of continuous memory
 * @date 2021-02-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Wrapper class handling a continuous block of memory
         * 
         * @tparam config Sorter configuration
         */
        template <class config>
        class block
        {
        public:
            /**
             * @brief Iterator type of the input data
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

            using const_reference = const value_type&;
            using pointer = value_type*;

            /**
             * @brief Determine whether data objects have to be initialized manually
             * 
             */
            static constexpr const bool initialized_storage {std::is_trivially_constructible<value_type>::value};
            
            /**
             * @brief Determines whether data objects have to be destroyed manually
             * 
             */
            static constexpr const bool destruct_storage {!initialized_storage && !std::is_trivially_destructible<value_type>::value};

            /**
             * @brief Returns a pointer to the data
             * 
             * @return pointer Pointer to data
             */
            pointer data()
            {
                return static_cast<pointer>(static_cast<void*>(storage_));
            }

            /**
             * @brief Returns the first element in the block
             * 
             * @return const_reference First element
             */
            const_reference head()
            {
                return *data();
            }

            /**
             * @brief Reads a complete block from `src` and initializes the memory if necessary
             * 
             * @param src Iterator to the data source
             */
            void read_from(iterator src)
            {
                if (initialized_storage)
                {
                    std::move(src, src + config::block_size, data());
                }
                else
                {
                    for (auto ptr {data()}, end {ptr + config::block_size}; ptr != end; ++ptr)
                    {
                        new (ptr) value_type {std::move(*src++)};
                    }
                }
            }

            /**
             * @brief Reads a partial block from `src` and initializes the memory if necessary
             * 
             * @param src Iterator to the data source
             * @param n Number of data elements to read
             */
            void read_from(iterator src, difference_type n)
            {
                if (initialized_storage)
                {
                    std::move(src, src + n, data());
                }
                else
                {
                    for (auto ptr {data()}, end {ptr + n}; ptr != end; ++ptr)
                    {
                        new (ptr) value_type {std::move(*src++)};
                    }
                }
            }

            /**
             * @brief Resets a partial block by destroying objects if necessary
             * 
             * @param n Number of elements to reset
             */
            void reset(difference_type n)
            {
                if (destruct_storage)
                {
                    for (auto ptr {data()}, end {ptr + n}; ptr != end; ++ptr)
                    {
                        ptr->~value_type();
                    }
                }
            }

            /**
             * @brief Writes a complete block to `dst` and destroys objects if necessary
             * 
             * @param dest Iterator to the data destination
             */
            void write_to(iterator dest)
            {
                std::move(data(), data() + config::block_size, dest);
                if (destruct_storage)
                {
                    for (auto ptr {data()}, end {ptr + config::block_size}; ptr != end; ++ptr)
                    {
                        ptr->~value_type();
                    }
                }
            }

            /**
             * @brief Writes a complete block to another block and destroys objects if necessary
             * 
             * @param dest Destination block
             */
            void write_to(block& dest)
            {
                if (initialized_storage)
                {
                    std::move(data(), data() + config::block_size, dest.data());
                }
                else
                {
                    for (auto src {data()}, dst {dest.data()}, end {src + config::block_size}; src != end; ++src, ++dst)
                    {
                        new (dst) value_type {std::move(*src)};
                    }
                }
                if (destruct_storage)
                {
                    for (auto ptr {data()}, end {ptr + config::block_size}; ptr != end; ++ptr)
                    {
                        ptr->~value_type();
                    }
                }
            }

        private:
            using storage_type = std::conditional<initialized_storage,
                                                   value_type,
                                                   std::aligned_storage_t<sizeof(value_type), alignof(value_type)>>;

            storage_type storage_[config::block_size];
        };
    } // namespace detail
} // namespace qsmb