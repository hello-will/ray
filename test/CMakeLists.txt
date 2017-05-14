cmake_minimum_required(VERSION 2.8)

project(test_ray CXX)

if(WIN32)
    include_directories($ENV{GTEST_INCLUDE_DIR})
    link_directories($ENV{GTEST_LIBRARY_DIR})
    set(
        LIBS
        gtest
        gtest_main
    )
else(WIN32)
    set(
        LIBS
        gtest
        gtest_main
        atomic
        pthread
        rt
    )
endif(WIN32)

file(GLOB TMP *.h)
list(APPEND INC ${TMP})
file(GLOB TMP *.hpp)
list(APPEND INC ${TMP})
aux_source_directory(. SRC)

add_executable(test_ray ${INC} ${SRC})
target_link_libraries(test_ray ${LIBS})
