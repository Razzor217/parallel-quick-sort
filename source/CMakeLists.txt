set(HEADERS
    ./detail/base_case.hpp

    ./parallel_sort.hpp
)


set(LIBS
    Threads::Threads
)

add_executable(test test/test.cpp ${HEADERS})
target_link_libraries(test ${LIBS})