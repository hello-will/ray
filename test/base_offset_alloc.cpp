/**
 * @file    base_offset_alloc.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 11:07:24
**/

#if defined(ALL)||defined(BASE_OFFSET_ALLOC)
#include <base/singleton.hpp>
#include <base/offset_alloc.hpp>
#include <gtest/gtest.h>

static void test( void )
{
    for ( auto i = 0; i < 10000000; i++ )
    {
        /* 0-4095 */
        size_t sz = rand()%4096;
        auto mem = ray::singleton<ray::offset_alloc>::geti()->alloc( sz );
        mem = ray::singleton<ray::offset_alloc>::geti()->realloc( sz+1, mem, sz );
        ray::singleton<ray::offset_alloc>::geti()->dealloc( mem, sz+1 );
    }
}

TEST( test_base_offset_alloc, normal )
{
    uint8_t buf[1024*1024];
    ray::offset_alloc alloc;

    EXPECT_EQ( RAY_OK, alloc.init(buf,sizeof(buf)) );
    ray::singleton<ray::offset_alloc>::seti( &alloc );

    char str[] = "test";
    auto mem = ray::singleton<ray::offset_alloc>::geti()->alloc( sizeof(str), str );
    EXPECT_NE( mem, ray::singleton<ray::offset_alloc>::geti()->null() );

    auto ptr = ray::singleton<ray::offset_alloc>::geti()->touch( mem );
    EXPECT_NE( ptr, nullptr );
    EXPECT_STREQ( str, (char*)ptr );

    ray::singleton<ray::offset_alloc>::geti()->dealloc( mem, sizeof(str) );

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


    ray::singleton<ray::offset_alloc>::seti( nullptr );
    alloc.close();
}
#endif /* #if defined(ALL)||defined(BASE_OFFSET_ALLOC) */