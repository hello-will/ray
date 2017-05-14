/**
 * @file    base_normal_alloc.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 11:07:24
**/

#if defined(ALL)||defined(BASE_STACK)
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/stack.hpp>
#include <gtest/gtest.h>

void test_normal( ray::stack<int,ray::normal_alloc> &st )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        if ( rand()%2 == 0 )
        {
            st.push( rand() );
        }
        else
        {
            int x;
            st.pop( x );
        }
    }
}

void test_offset( ray::stack<int,ray::offset_alloc> &st )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        if ( rand()%2 == 0 )
        {
            st.push( rand() );
        }
        else
        {
            int x;
            st.pop( x );
        }
    }
}

void test_pool( ray::stack<int,ray::pool_alloc> &st )
{
    for ( auto i = 0; i < 10000000; ++i )
    {
        if ( rand()%2 == 0 )
        {
            st.push( rand() );
        }
        else
        {
            int x;
            st.pop( x );
        }
    }
}


TEST( test_base_stack, normal )
{
    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::stack<int, ray::normal_alloc> st;
            st.push( 1 );
            st.push( 2 );
            st.push( 3 );

            int x;
            while ( st.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );

            /* multi thread */
            std::thread th[8];
            for ( auto i = 0; i < 8; ++i )
            {
                th[i] = std::thread( test_normal, std::ref(st) );
            }
            for ( auto i = 0; i < 8; ++i )
            {
                th[i].join();
            }
            st.clear();
        }
        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        uint8_t buf[1024*1024];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init(buf,sizeof(buf)) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::stack<int, ray::offset_alloc> st;
            st.push( 1 );
            st.push( 2 );
            st.push( 3 );

            int x;
            while ( st.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );

            /* multi thread */
            std::thread th[8];
            for ( auto i = 0; i < 8; ++i )
            {
                th[i] = std::thread( test_offset, std::ref(st) );
            }
            for ( auto i = 0; i < 8; ++i )
            {
                th[i].join();
            }
            st.clear();
        }
        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::stack<int, ray::pool_alloc> st;
            st.push( 1 );
            st.push( 2 );
            st.push( 3 );

            int x;
            while ( st.pop( x ) )
            {
                printf( "%d ", x );
            }
            printf( "\n" );

            /* multi thread */
            std::thread th[8];
            for ( auto i = 0; i < 8; ++i )
            {
                th[i] = std::thread( test_pool, std::ref(st) );
            }
            for ( auto i = 0; i < 8; ++i )
            {
                th[i].join();
            }
            st.clear();
        }
        ray::singleton<ray::offset_alloc>::seti( nullptr );
    }
}
#endif /* #if defined(ALL)||defined(BASE_STACK) */