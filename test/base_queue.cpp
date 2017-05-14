/**
 * @file    base_normal_alloc.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 11:07:24
**/

#if defined(ALL)||defined(BASE_QUEUE)
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/queue.hpp>
#include <gtest/gtest.h>

void test_normal( ray::queue<int,ray::normal_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        if ( rand()%2 == 0 )
        {
            qu.push( rand() );
        }
        else
        {
            int x;
            qu.pop( x );
        }
    }
}

void test_offset( ray::queue<int,ray::offset_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        if ( rand()%2 == 0 )
        {
            qu.push( rand() );
        }
        else
        {
            int x;
            qu.pop( x );
        }
    }
}

void test_pool( ray::queue<int,ray::pool_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        if ( rand()%2 == 0 )
        {
            qu.push( rand() );
        }
        else
        {
            int x;
            qu.pop( x );
        }
    }
}

TEST( test_base_queue, normal )
{
    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::queue<int, ray::normal_alloc> qu;
            qu.push( 1 );
            qu.push( 2 );
            qu.push( 3 );

            int x;
            while ( qu.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );

            /* multi thread */
            std::thread th[8];
            for ( auto i = 0; i < 8; ++i )
            {
                th[i] = std::thread( test_normal, std::ref(qu) );
            }
            for ( auto i = 0; i < 8; ++i )
            {
                th[i].join();
            }
            qu.clear();
        }
        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        uint8_t buf[1024*1024];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init(buf,sizeof(buf)) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::queue<int, ray::offset_alloc> qu;
            qu.push( 1 );
            qu.push( 2 );
            qu.push( 3 );

            int x;
            while ( qu.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );

            /* multi thread */
            std::thread th[8];
            for ( auto i = 0; i < 8; ++i )
            {
                th[i] = std::thread( test_offset, std::ref(qu) );
            }
            for ( auto i = 0; i < 8; ++i )
            {
                th[i].join();
            }
            qu.clear();
        }
        ray::singleton<ray::offset_alloc>::seti( nullptr );
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::queue<int, ray::pool_alloc> qu;
            qu.push( 1 );
            qu.push( 2 );
            qu.push( 3 );

            int x;
            while ( qu.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );

            /* multi thread */
            std::thread th[8];
            for ( auto i = 0; i < 8; ++i )
            {
                th[i] = std::thread( test_pool, std::ref(qu) );
            }
            for ( auto i = 0; i < 8; ++i )
            {
                th[i].join();
            }
            qu.clear();
        }
        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }

}
#endif /* #if defined(ALL)||defined(BASE_QUEUE) */