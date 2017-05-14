/**
 * @file    base_ring_queue.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * data     2016/10/27 10:40:02
**/

#if defined(ALL)||defined(BASE_RING_QUEUE)
#include <base/queue.hpp>
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <gtest/gtest.h>

void test_normal_push( ray::ring_queue<int,ray::normal_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        qu.push( rand() );
    }
}

void test_normal_pop( ray::ring_queue<int,ray::normal_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        int x;
        qu.pop( x );
    }
}

void test_offset_push( ray::ring_queue<int,ray::offset_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        qu.push( rand() );
    }
}

void test_offset_pop( ray::ring_queue<int,ray::offset_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        int x;
        qu.pop( x );
    }
}

void test_pool_push( ray::ring_queue<int,ray::pool_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        qu.push( rand() );
    }
}

void test_pool_pop( ray::ring_queue<int,ray::pool_alloc> &qu )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        int x;
        qu.pop( x );
    }
}

TEST( test_base_ring_queue, normal )
{
    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            /* multi thread */
            for (auto i = 0; i < 4; i++)
            {
                std::thread th[2];
                ray::ring_queue<int,ray::normal_alloc> qu;

                th[0] = std::thread( test_normal_push, std::ref(qu) );
                th[1] = std::thread( test_normal_pop, std::ref(qu) );
                for ( auto i = 0; i < 2; ++i )
                {
                    th[i].join();
                }
            }

            ray::ring_queue<int,ray::normal_alloc> qu;
            qu.push( 1 );
            qu.push( 2 );
            qu.push( 3 );

            int x;
            while ( qu.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );
        }
        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        uint8_t buf[1024*1024];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init(buf,sizeof(buf)) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            /* multi thread */
            for (auto i = 0; i < 4; i++)
            {
                std::thread th[2];
                ray::ring_queue<int,ray::offset_alloc> qu;

                th[0] = std::thread( test_offset_push, std::ref(qu) );
                th[1] = std::thread( test_offset_pop, std::ref(qu) );
                for ( auto i = 0; i < 2; ++i )
                {
                    th[i].join();
                }
            }

            ray::ring_queue<int,ray::offset_alloc> qu;
            qu.push( 1 );
            qu.push( 2 );
            qu.push( 3 );

            int x;
            while ( qu.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );
        }
        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            /* multi thread */
            for (auto i = 0; i < 4; i++)
            {
                std::thread th[2];
                ray::ring_queue<int,ray::pool_alloc> qu;

                th[0] = std::thread( test_pool_push, std::ref(qu) );
                th[1] = std::thread( test_pool_pop, std::ref(qu) );
                for ( auto i = 0; i < 2; ++i )
                {
                    th[i].join();
                }
            }

            ray::ring_queue<int,ray::pool_alloc> qu;
            qu.push( 1 );
            qu.push( 2 );
            qu.push( 3 );

            int x;
            while ( qu.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );
        }
        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }
}
#endif /* #if defined(ALL)||defined(BASE_RING_QUEUE) */