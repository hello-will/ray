/**
 * @file    main.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 09:12:39
**/

#include <gtest/gtest.h>
#include <base/backtrace.hpp>

int main( int argc, char *argv[] )
{
    ray::backtrace<16> bt;
    BACKTRACE(bt);
    BACKTRACE_WRITE(bt,"\t",std::cout);

    srand( time(NULL) );
    testing::InitGoogleTest( &argc, argv );
    int ret = RUN_ALL_TESTS();

    getchar();
    return ret;
}
