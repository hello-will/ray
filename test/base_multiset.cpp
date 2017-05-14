/**
 * @file    base_set.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/30 11:28:58
**/

#if defined(ALL)||defined(BASE_MULTISET)
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/set.hpp>
#include <gtest/gtest.h>

TEST( test_base_multiset, normal )
{
    {
        uint8_t buf[1024*1024];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init( buf, sizeof(buf) ) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::multiset<int,ray::less<int>,ray::offset_alloc> st;

            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            EXPECT_EQ( 6, st.size() );

            for ( auto iter = st.begin(); iter != st.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::multiset<int,ray::less<int>,ray::offset_alloc> &c_st = st;
            for ( auto iter = c_st.begin(); iter != c_st.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            st.erase( 1 );
            EXPECT_EQ( 4, st.size() );
            st.erase( 2 );
            EXPECT_EQ( 2, st.size() );
            st.erase( 3 );
            EXPECT_EQ( 0, st.size() );

            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            EXPECT_EQ( 6, st.size() );

            ray::multiset<int,ray::less<int>,ray::offset_alloc> st_1( st );
            EXPECT_EQ( 6, st_1.size() );
            EXPECT_EQ( 6, st.size() );

            ray::multiset<int,ray::less<int>,ray::offset_alloc> st_2( ray::move(st_1) );
            EXPECT_EQ( 6, st_2.size() );
            EXPECT_EQ( 0, st_1.size() );

            for ( auto iter = st_2.begin(); iter != st_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            st_1 = ray::move( st_2 );
            EXPECT_EQ( 6, st_1.size() );
            EXPECT_EQ( 0, st_2.size() );

            st_2 = st_1;
            EXPECT_EQ( 6, st_2.size() );

            st.clear();
            EXPECT_EQ( 0, st.size() );

            st_1.clear();
            EXPECT_EQ( 0, st_1.size() );

            st_2.clear();
            EXPECT_EQ( 0, st_2.size() );
        }

        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
    }

    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::multiset<int,ray::less<int>,ray::normal_alloc> st;

            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            EXPECT_EQ( 6, st.size() );

            for ( auto iter = st.begin(); iter != st.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::multiset<int,ray::less<int>,ray::normal_alloc> &c_st = st;
            for ( auto iter = c_st.begin(); iter != c_st.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            st.erase( 1 );
            EXPECT_EQ( 4, st.size() );
            st.erase( 2 );
            EXPECT_EQ( 2, st.size() );
            st.erase( 3 );
            EXPECT_EQ( 0, st.size() );

            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            EXPECT_EQ( 6, st.size() );

            ray::multiset<int,ray::less<int>,ray::normal_alloc> st_1( st );
            EXPECT_EQ( 6, st_1.size() );
            EXPECT_EQ( 6, st.size() );

            ray::multiset<int,ray::less<int>,ray::normal_alloc> st_2( ray::move(st_1) );
            EXPECT_EQ( 6, st_2.size() );
            EXPECT_EQ( 0, st_1.size() );

            for ( auto iter = st_2.begin(); iter != st_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            st_1 = ray::move( st_2 );
            EXPECT_EQ( 6, st_1.size() );
            EXPECT_EQ( 0, st_2.size() );

            st_2 = st_1;
            EXPECT_EQ( 6, st_2.size() );

            st.clear();
            EXPECT_EQ( 0, st.size() );

            st_1.clear();
            EXPECT_EQ( 0, st_1.size() );

            st_2.clear();
            EXPECT_EQ( 0, st_2.size() );
        }

        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::multiset<int,ray::less<int>,ray::pool_alloc> st;

            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            EXPECT_EQ( 6, st.size() );

            for ( auto iter = st.begin(); iter != st.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::multiset<int,ray::less<int>,ray::pool_alloc> &c_st = st;
            for ( auto iter = c_st.begin(); iter != c_st.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            st.erase( 1 );
            EXPECT_EQ( 4, st.size() );
            st.erase( 2 );
            EXPECT_EQ( 2, st.size() );
            st.erase( 3 );
            EXPECT_EQ( 0, st.size() );

            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            st.insert( 1 );
            st.insert( 2 );
            st.insert( 3 );
            EXPECT_EQ( 6, st.size() );

            ray::multiset<int,ray::less<int>,ray::pool_alloc> st_1( st );
            EXPECT_EQ( 6, st_1.size() );
            EXPECT_EQ( 6, st.size() );

            ray::multiset<int,ray::less<int>,ray::pool_alloc> st_2( ray::move(st_1) );
            EXPECT_EQ( 6, st_2.size() );
            EXPECT_EQ( 0, st_1.size() );

            for ( auto iter = st_2.begin(); iter != st_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            st_1 = ray::move( st_2 );
            EXPECT_EQ( 6, st_1.size() );
            EXPECT_EQ( 0, st_2.size() );

            st_2 = st_1;
            EXPECT_EQ( 6, st_2.size() );

            st.clear();
            EXPECT_EQ( 0, st.size() );

            st_1.clear();
            EXPECT_EQ( 0, st_1.size() );

            st_2.clear();
            EXPECT_EQ( 0, st_2.size() );
        }

        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }
}
#endif /* #if defined(ALL)||defined(BASE_MULTISET) */