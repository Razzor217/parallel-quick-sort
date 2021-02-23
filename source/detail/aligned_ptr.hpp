/**
 * @file aligned_ptr.hpp
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Provides aligned storage
 * @date 2021-02-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

namespace qsmb
{
    namespace detail
    {
        /**
         * @brief Aligns a pointer to the next multiple of `alignment`
         * 
         * @tparam value_type Type of the object pointed to by `ptr`
         * @param ptr Pointer to an object
         * @param alignment Desired alignment of the pointer
         * @return value_type* Aligned pointer 
         */
        template <class value_type>
        static value_type* align_pointer(value_type* ptr, std::size_t alignment)
        {
            std::uintptr_t value_ = reinterpret_cast<std::uintptr_t>(ptr);
            value_ = (value_ - 1 + alignment) & ~(alignment - 1);
            return reinterpret_cast<value_type*>(value_);
        }

        /**
         * @brief Provides aligned storage and constructs an object at the 
         * desired alignment
         * 
         * @tparam value_type Type of the object to align 
         */
        template <class value_type>
        class aligned_ptr
        {
        public:
            /**
             * @brief Reference type of the aligned_ptr
             * 
             */
            using reference = value_type&;

            /**
             * @brief Pointer type of the aligned_ptr
             * 
             */
            using pointer = value_type*;

            /**
             * @brief Returns a pointer to the aligned object or `nullptr` 
             * if no object is owned
             * 
             * @return pointer Pointer to the aligned object
             */
            inline pointer get() const { return value_; }

            /**
             * @brief Returns the aligned object owned by `*this`
             * 
             * @return reference Aligned object
             */
            inline reference operator *() const { return *get(); }

            /**
             * @brief Returns a pointer to the aligned object owned by `*this`
             * 
             * @return pointer Pointer to the aligned object
             * 
             * @see get()
             */
            inline pointer operator ->() const { return get(); }

            /**
             * @brief Default constructor
             * 
             */
            aligned_ptr() {}

            /**
             * @brief Constructs a new object at the specified alignment
             * 
             * @tparam args Arguments to forward to the constructor of the object
             * @param alignment Desired alignment for objects
             * @param arg Arguments to forward to the constructor of the object
             */
            template <class... args>
            explicit aligned_ptr(std::size_t alignment, args&&... arg) :
                alloc_ {new char[sizeof(value_type) + alignment]},
                value_ {new (align_pointer(alloc_, alignment)) value_type {std::forward<args>(arg)...}}
            {}

            aligned_ptr(const aligned_ptr&) = delete;

            /**
             * @brief Move constructor for aligned_ptr
             * 
             * @param other Other aligned_ptr used as data source
             */
            aligned_ptr(aligned_ptr&& other) :
                alloc_ {other.alloc_},
                value_ {other.value_}
            {
                other.alloc_ = nullptr;
            }

            aligned_ptr& operator =(const aligned_ptr&) = delete;

            /**
             * @brief Move assignment for aligned_ptr
             * 
             * @param other Other aligned_ptr used as data source
             * @return aligned_ptr& `*this`
             */
            aligned_ptr& operator =(aligned_ptr&& other)
            {
                std::swap(alloc_, other.alloc_);
                std::swap(value_, other.value_);
                return *this;
            }

            /**
             * @brief Destroy the aligned_ptr object
             * 
             */
            ~aligned_ptr()
            {
                if (alloc_)
                {
                    value_->~value_type();
                    delete[] alloc_;
                }
            }

        private:
            char* alloc_ = nullptr;
            pointer value_ = nullptr;
        };

        /**
         * @brief Provides aligned storage without constructing an object
         *
         */
        template <>
        class aligned_ptr<void>
        {
        public:
            /**
             * @brief Reference type of the aligned_ptr
             * 
             */
            using reference = char&;

            /**
             * @brief Pointer type of the aligned_ptr
             * 
             */
            using pointer = char*;

            /**
             * @brief Returns a pointer to the aligned storage owned by `*this`
             * 
             * @return pointer Pointer to the aligned storage
             */
            inline pointer get() const { return value_; }

            /**
             * @brief Returns the aligned storage owned by `*this`
             * 
             * @return reference Aligned storage
             */
            inline reference operator *() const { return *get(); }

            /**
             * @brief Returns a pointer to the aligned storage owned by `*this`
             * 
             * @return pointer Pointer to the aligned storage
             */
            inline pointer operator ->() const { return get(); }

            /**
             * @brief Default constructor
             * 
             */
            aligned_ptr() {}

            /**
             * @brief Create aligned storage without constructing an object
             * 
             * @tparam args Arguments to forward to the constructor of the object
             * @param alignment Desired alignment for objects
             * @param size Object size
             */
            template <class... args>
            explicit aligned_ptr(std::size_t alignment, std::size_t size) :
                alloc_ {new char[size + alignment]},
                value_ {(align_pointer(alloc_, alignment))}
            {}

            aligned_ptr(const aligned_ptr&) = delete;

            /**
             * @brief Move constructor for aligned_ptr
             * 
             * @param other Other aligned_ptr used as data source
             */
            aligned_ptr(aligned_ptr&& other) :
                alloc_ {other.alloc_},
                value_ {other.value_}
            {
                other.alloc_ = nullptr;
            }

            aligned_ptr& operator =(const aligned_ptr&) = delete;

            /**
             * @brief Move assignment for aligned_ptr
             * 
             * @param other Other aligned_ptr used as data source
             * @return aligned_ptr& `*this`
             */
            aligned_ptr& operator =(aligned_ptr&& other)
            {
                std::swap(alloc_, other.alloc_);
                std::swap(value_, other.value_);
                return *this;
            }

            /**
             * @brief Destroy the aligned_ptr object
             * 
             */
            ~aligned_ptr()
            {
                if (alloc_)
                {
                    delete[] alloc_;
                }
            }

        private:
            pointer alloc_ = nullptr;
            pointer value_ = nullptr;
        };
    } // namespace detail
} // namespace qsmb