cmake_minimum_required(VERSION 2.8)

project(parallel-quick-sort)

include_directories(.)

set(CMAKE_CXX_FLAGS "-std=c++17 -msse4.2 -Wall -Wextra -O3 -g")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_subdirectory(source)