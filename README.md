# Parallel Quick Sort

The [Parallel Quick Sort](https://github.com/Razzor217/parallel-quick-sort) 
library implements a parallel variant of quick sort which not only performs 
recursive tasks in parallel but also partitions the input in a parallel 
manner.

The algorithm works as follows (recursive entry point):

1. The first thread in the group selects a pivot element
2. Each thread in the group partitions its section of the group-local 
input sequence 
3. A block permutation is performed by the thread group in order to place 
homogeneous blocks in the correct partition
4. A cleanup phase handles remaining elements
5. The first thread in the group calculates an appropriate distribution of 
threads amongst two new groups
6. Each remaining partition is sorted recursively by one group

## References

The parallel quick sort algorithm and its load balancing are inspired by 
the distributed memory variant presented in 
[Sequential and Parallel Data Structures and Algorithms](https://people.mpi-inf.mpg.de/~mehlhorn/ToolboxNew.html)
(p. 174). 

The implementation is based on the [implementation](https://github.com/SaschaWitt/ips4o) 
of In-Place Parallel Super Scalar Samplesort. Specifically, 
the qsmb::detail::barrier, qsmb::std_thread_pool and qsmb::detail::aligned_ptr classes are 
adopted almost directly from the sample sort implementation.
adopted almost directly from the sample sort implementation.
