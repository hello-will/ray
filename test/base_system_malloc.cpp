/**
 * @file    base_system_malloc.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 11:07:24
**/

#if defined(ALL)||defined(BASE_SYSTEM_MALLOC)
#include <base/singleton.hpp>
#include <gtest/gtest.h>

static void test( void )
{
    for ( auto i = 0; i < 10000000; i++ )
    {
        /* 0-4095 */
        size_t sz = rand()%4096;
        auto mem = ::malloc( sz );
        mem = ::realloc( mem, sz+1 );
        ::free( mem );
    }
}


TEST( test_base_system_malloc, normal )
{
    /* multi thread */
    std::thread th[8];
    for ( auto i = 0; i < 8; ++i )
    {
        th[i] = std::thread( test );
    }
    for ( auto i = 0; i < 8; ++i )
    {
        th[i].join();
    }
}
#endif /* #if defined(ALL)||defined(BASE_SYSTEM_MALLOC) */