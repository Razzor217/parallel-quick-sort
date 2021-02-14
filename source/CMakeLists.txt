set(HEADERS
    ./detail/base_case.hpp
    ./detail/selection.hpp

    ./parallel_sort.hpp
)


set(LIBS
    Threads::Threads
)

add_executable(base_case test/base_case.cpp ${HEADERS})
target_link_libraries(base_case ${LIBS})

add_executable(selection test/selection.cpp ${HEADERS})
target_link_libraries(selection ${LIBS})