/**
 * @file doxygen.h
 * @author Max Beddies (max dot beddies at t minus online dot de)
 * @brief Main project documentation
 * @date 2021-02-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @namespace parallel Namespace for parallel algorithms
 * 
 */

/**
 * @namespace detail Namespace for in-depth algorithms which are not part of 
 * public interface
 * 
 */

/**
 * @mainpage Parallel Quick Sort
 * 
 * The [Parallel Quick Sort](https://github.com/Razzor217/parallel-quick-sort) 
 * library implements a parallel variant of quick sort which not only performs 
 * recursive tasks in parallel but also partitions the input in a parallel 
 * manner.
 * 
 * The algorithm works as follows (recursive entry point):
 * 1. The first thread in the group selects a pivot element
 * 2. Each thread in the group partitions its section of the group-local 
 * input sequence 
 * 3. A block permutation is performed by the thread group in order to place 
 * homogeneous blocks in the correct partition
 * 4. A cleanup phase handles remaining elements
 * 5. The first thread in the group calculates an appropriate distribution of 
 * threads amongst two new groups
 * 6. Each remaining partition is sorted recursively by one group
 * 
 * @section References
 * 
 * The parallel quick sort algorithm and its load balancing are inspired by 
 * the distributed memory variant presented in 
 * [Sequential and Parallel Data Structures and Algorithms](https://people.mpi-inf.mpg.de/~mehlhorn/ToolboxNew.html)
 * (p. 174). 
 * 
 * The implementation is based on the [implementation](https://github.com/SaschaWitt/ips4o) 
 * of In-Place Parallel Super Scalar Samplesort. Specifically, 
 * the qsmb::detail::barrier, qsmb::std_thread_pool and qsmb::detail::aligned_ptr classes are 
 * adopted almost directly from the sample sort implementation.
 */



/*
 * TODO:
 * - add algorithm description (below) to documentation
 * - add description of block permutation
 * - add description of cleanup
 * - complete sorter
 *     - sequential variant (1 thread in group)
 *     - thread pool
 *     - synchronization mechanism (barrier)
 *     - buffers w/ read/write/size
 *     - all phases described below
 */


/*
 * Thread-local classification:
 * (thread-local start and end iterators)
 * 
 * 1. while end of input is not reached:
 *     a) read one block (or whatever is left) into local buffer
 *     b) partition block into local blocks for left, middle and right 
 *        partitions
 *     c) if a local partition block is full, write it back
 * 2. update local size of left/middle/right partitions 
 *    (partially filled buffers)
 * 
 * -> homogeneous blocks belonging to exactly one of left/middle/right 
 *    partitions, followed by empty blocks (partially filled local buffers)
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Group classification:
 * (start and end iterators for group) 
 * 
 * 1. view input data as array of blocks (last block partially filled)
 * 2. divide input data into t stripes of equal size 
 *    (i.e. equal number of blocks), one for each thread
 * 3. local classification on each thread
 *
 * -> stripes of the form (homogeneous blocks || empty blocks), 
 *    possibly partially filled buffers on each thread
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */