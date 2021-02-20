set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif()

set(HEADERS
    ./detail/aligned_ptr.hpp
    ./detail/barrier.hpp

    ./detail/base_case.hpp
    ./detail/selection.hpp
    ./detail/sequential.hpp
    ./detail/sorter.hpp

    ./configuration.hpp
    ./thread_pool.hpp

    ./parallel_sort.hpp
)


set(LIBS
    Threads::Threads
)

add_executable(base_case_test test/base_case.cpp ${HEADERS})
add_executable(selection_test test/selection.cpp ${HEADERS})
add_executable(sequential_test test/sequential.cpp ${HEADERS})

target_link_libraries(base_case_test ${LIBS})
target_link_libraries(selection_test ${LIBS})
target_link_libraries(sequential_test ${LIBS})

add_test(NAME base_case COMMAND base_case_test)
add_test(NAME selection COMMAND selection_test)
add_test(NAME sequential COMMAND sequential_test)