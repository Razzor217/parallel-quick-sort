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
 * Some documentation goes here ...
 * 
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
     * Coarse description of parallel algorithm
     * 
     * 1. pivot selection by first thread in group
     * 2. group classification
     * 3. block permutation
     * 4. cleanup
     * 5. load balancing
     * 6. splitting group into two groups, 
     *    1st group sorts left partition (recursion), 
     *    second group sorts right partition (recursion)
     * 
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * 
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