#!/bin/bash

rm -r CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile
rm *~

cd base
rm -r CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile
rm *~
cd ..

cd graph
rm -r CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile
rm *~
cd ..

cd network
rm -r CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile
rm *~
cd ..

cd test
rm -r CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile
rm *~
cd ..

cd bin
rm *
cd ..

cd lib
rm *
cd ..
