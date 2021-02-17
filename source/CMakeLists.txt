set(HEADERS
    ./detail/base_case.hpp
    ./detail/selection.hpp
    ./detail/sequential.hpp
    ./detail/sorter.hpp

    ./configuration.hpp
    ./parallel_sort.hpp
)


set(LIBS
    Threads::Threads
)

add_executable(base_case_test test/base_case.cpp ${HEADERS})
target_link_libraries(base_case_test ${LIBS})

add_executable(selection_test test/selection.cpp ${HEADERS})
target_link_libraries(selection_test ${LIBS})

add_executable(sequential_test test/sequential.cpp ${HEADERS})
target_link_libraries(sequential_test ${LIBS})