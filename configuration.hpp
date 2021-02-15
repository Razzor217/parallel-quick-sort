#pragma once

#include <cstddef>
#include <iterator>

#ifndef BASE_CASE_SIZE
#define BASE_CASE_SIZE 16
#endif

#ifndef BASE_CASE_MULTIPLIER
#define BASE_CASE_MULTIPLIER 16
#endif

template <std::ptrdiff_t base_case_size_ = BASE_CASE_SIZE,
          std::ptrdiff_t base_case_multiplier_ = BASE_CASE_MULTIPLIER>
class configuration
{
public:
    static constexpr const std::ptrdiff_t base_case_size {base_case_size_};
    static constexpr const std::ptrdiff_t base_case_multiplier {base_case_multiplier_};
};

template <class random_access_iterator,
          class comparator,
          class config = configuration<>>
class extended_configuration : public configuration
{
public:
    using iterator = random_access_iterator;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using difference_type = typename std::iterator_traits<iterator>::difference_type;

    using less = comparator;

    using base_config = config;

    static constexpr const difference_type base_case_size {config::base_case_size};
    static constexpr const difference_type base_case_multiplier {config::base_case_multiplier};
};

#undef BASE_CASE_SIZE
#undef BASE_CASE_MULTIPLIER