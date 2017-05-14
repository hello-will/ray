#!/bin/bash

dir=$1
case $# in
    1)
        cp -f $dir/base/*.hpp ./base/
        cp -f $dir/base/*.cpp ./base/
        cp -f $dir/base/CMakeLists.txt ./base/

        cp -f $dir/graph/*.hpp ./graph/
        cp -f $dir/graph/*.cpp ./graph/
        cp -f $dir/graph/CMakeLists.txt ./graph/

        cp -f $dir/network/*.hpp ./network/
        cp -f $dir/network/*.cpp ./network/
        cp -f $dir/network/CMakeLists.txt ./network/

        cp -f $dir/test/*.hpp ./test/
        cp -f $dir/test/*.cpp ./test/
        cp -f $dir/test/CMakeLists.txt ./test/

        cp -f $dir/CMakeLists.txt ./
        cp -f $dir/clear.sh ./
    ;;
    *)
        echo "$0 source_directory"
    ;;
esac
