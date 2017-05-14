cmake_minimum_required(VERSION 2.8)

project(base CXX)

file(GLOB TMP *.h)
list(APPEND INC ${TMP})
file(GLOB TMP *.hpp)
list(APPEND INC ${TMP})
aux_source_directory(. SRC)

add_library(base STATIC ${INC} ${SRC})