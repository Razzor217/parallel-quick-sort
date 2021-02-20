/**
 * @file configuration.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Definition of sorter configurations
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cstddef>
#include <iterator>

#include "thread_pool.hpp"
#include "detail/util.hpp"

#ifndef BASE_CASE_SIZE
#define BASE_CASE_SIZE 16
#endif

#ifndef BASE_CASE_MULTIPLIER
#define BASE_CASE_MULTIPLIER 16
#endif

#ifndef BLOCK_SIZE
#define BLOCK_SIZE (2 << 10)
#endif

#ifndef DATA_ALIGNMENT
#define DATA_ALIGNMENT (4 << 10)
#endif

namespace qsmb
{
    /**
     * @brief Base class used to configure the sorters.
     * 
     * @tparam base_case_size_ Desired base case threshold
     * @tparam base_case_multiplier_ Multiplier for the base case threshold
     */
    template <std::ptrdiff_t base_case_size_ = BASE_CASE_SIZE,
              std::ptrdiff_t base_case_multiplier_ = BASE_CASE_MULTIPLIER,
              std::ptrdiff_t block_size_ = BLOCK_SIZE,
              std::ptrdiff_t data_alignment_ = DATA_ALIGNMENT>
    class configuration
    {
    public:
        /**
         * @brief Desired base case threshold
         * 
         */
        static constexpr const std::ptrdiff_t base_case_size {base_case_size_};
        
        /**
         * @brief Multiplier for the base case threshold
         * 
         */
        static constexpr const std::ptrdiff_t base_case_multiplier {base_case_multiplier_};

        /**
         * @brief Number of bytes per block
         * 
         */
        static constexpr const std::ptrdiff_t block_size {block_size_};

        /**
         * @brief Alignment for shared and thread-local data
         * 
         */
        static constexpr const std::ptrdiff_t data_alignment {data_alignment_};
    };

    /**
     * @brief Exentended configuration for the sorters.
     * 
     * @tparam random_access_iterator Iterator type meeting the requirements of random access iterators
     * @tparam comparator Comparison function object
     * @tparam config Base configuration for the sorters, @see configuration
     */
    template <class random_access_iterator,
              class comparator,
              class config = configuration<>,
              class thread_pool_ = std_thread_pool>
    class extended_configuration : public config
    {
    public:
        /**
         * @brief Iterator type for the input data
         * 
         */
        using iterator = random_access_iterator;

        /**
         * @brief Value type of the input data
         * 
         */
        using value_type = typename std::iterator_traits<iterator>::value_type;
        
        /**
         * @brief Difference type of the iterator
         * 
         */
        using difference_type = typename std::iterator_traits<iterator>::difference_type;

        /**
         * @brief Comparison function object
         * 
         */
        using less = comparator;

        /**
         * @brief Base configuration with user-specified parameters
         * 
         */
        using base_config = config;

        /**
         * @brief Thread pool for parallel execution
         * 
         */
        using thread_pool = thread_pool_;

        /**
         * @brief Synchronization mechanism for parallel execution
         * 
         */
        using sync = typename thread_pool::sync;

        /**
         * @brief Desired base case threshold
         * 
         */
        static constexpr const difference_type base_case_size {config::base_case_size};
        
        /**
         * @brief Multiplier for the base case threshold
         * 
         */
        static constexpr const difference_type base_case_multiplier {config::base_case_multiplier};

        /**
         * @brief Number of elements in one block
         * 
         */
        static constexpr const difference_type block_size 
        {
            1 << detail::log2t(config::block_size < sizeof(value_type) ? 1 : 
                               config::block_size / sizeof(value_type))
        };

        /**
         * @brief Aligns an offset to the next block boundary
         * 
         * @param offset Offset to align
         * @return Offset aligned to block boundary 
         */
        static constexpr difference_type align_to_next_block(difference_type offset)
        {
            return (offset + block_size - 1) & ~(block_size - 1);
        }
    };

    #undef BASE_CASE_SIZE
    #undef BASE_CASE_MULTIPLIER
} // namespace qsmb