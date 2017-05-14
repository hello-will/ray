/**
 * @file    base_list.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 15:20:15
**/

#if defined(ALL)||defined(BASE_LIST)
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/list.hpp>
#include <gtest/gtest.h>

TEST( test_base_list, normal )
{
    {
        uint8_t buf[1024*1024];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init(buf,sizeof(buf)) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::list<uint32_t,ray::offset_alloc> lst;
            lst.push_front( 1 );
            lst.push_back( 3 );
            lst.push_back( 2 );
            lst.push_back( 2 );
            EXPECT_EQ( 4, lst.size() );
            lst.unique();
            EXPECT_EQ( 3, lst.size() );
            lst.remove_if( [](uint32_t x){return x==3;} );
            EXPECT_EQ( 2, lst.size() );

            lst.push_back( 1 );
            lst.push_back( 0 );
            lst.sort();
            for ( auto iter = lst.begin(); iter != lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::list<uint32_t,ray::offset_alloc> &c_lst = lst;
            for ( auto iter = c_lst.begin(); iter != c_lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            lst.reverse();
            for ( auto iter = lst.begin(); iter != lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            lst.pop_front();
            lst.pop_back();
            lst.clear();
            EXPECT_EQ( 0, lst.size() );
            EXPECT_TRUE( lst.empty() );

            lst.push_back( 1 );
            lst.push_back( 2 );
            lst.push_back( 3 );
            lst.push_back( 4 );

            ray::list<uint32_t,ray::offset_alloc> lst_1( lst );
            ray::list<uint32_t,ray::offset_alloc> lst_2( lst.begin(), lst.end() );
            lst_1.splice( lst_1.begin(), lst_2 );
            lst_1.sort();
            for ( auto iter = lst_1.begin(); iter != lst_1.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 2*lst.size(), lst_1.size() );
            EXPECT_EQ( 0, lst_2.size() );

            lst_2 = ray::move( lst_1 );
            for ( auto iter = lst_2.begin(); iter != lst_2.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 0, lst_1.size() );
            EXPECT_EQ( 2*lst.size(), lst_2.size() );

            lst_1 = lst_2;
            for ( auto iter = lst_1.begin(); iter != lst_1.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 2*lst.size(), lst_1.size() );
            EXPECT_EQ( 2*lst.size(), lst_2.size() );

            lst.clear();
            EXPECT_EQ( 0, lst.size() );

            lst_1.clear();
            EXPECT_EQ( 0, lst_1.size() );

            lst_2.clear();
            EXPECT_EQ( 0, lst_2.size() );
        }

        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
    }

    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::list<uint32_t,ray::normal_alloc> lst;
            lst.push_front( 1 );
            lst.push_back( 2 );
            lst.push_back( 2 );
            lst.push_back( 3 );
            EXPECT_EQ( 4, lst.size() );
            lst.unique();
            EXPECT_EQ( 3, lst.size() );
            lst.remove_if( [](uint32_t x){return x==3;} );
            EXPECT_EQ( 2, lst.size() );

            lst.push_back( 1 );
            lst.push_back( 0 );
            lst.sort();
            for ( auto iter = lst.begin(); iter != lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::list<uint32_t,ray::normal_alloc> &c_lst = lst;
            for ( auto iter = c_lst.begin(); iter != c_lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            lst.reverse();
            for ( auto iter = lst.begin(); iter != lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            lst.pop_front();
            lst.pop_back();
            lst.clear();
            EXPECT_EQ( 0, lst.size() );
            EXPECT_TRUE( lst.empty() );

            lst.push_back( 1 );
            lst.push_back( 2 );
            lst.push_back( 3 );
            lst.push_back( 4 );

            ray::list<uint32_t,ray::normal_alloc> lst_1( lst );
            ray::list<uint32_t,ray::normal_alloc> lst_2( lst.begin( ), lst.end( ) );
            lst_1.splice( lst_1.begin(), lst_2 );
            lst_1.sort();
            for ( auto iter = lst_1.begin(); iter != lst_1.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 2*lst.size(), lst_1.size() );
            EXPECT_EQ( 0, lst_2.size() );

            lst_2 = std::move( lst_1 );
            for ( auto iter = lst_2.begin(); iter != lst_2.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 0, lst_1.size() );
            EXPECT_EQ( 2*lst.size(), lst_2.size() );

            lst_1 = lst_2;
            for ( auto iter = lst_1.begin(); iter != lst_1.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 2*lst.size(), lst_1.size() );
            EXPECT_EQ( 2*lst.size(), lst_2.size() );

            lst.clear();
            EXPECT_EQ( 0, lst.size() );

            lst_1.clear();
            EXPECT_EQ( 0, lst_1.size() );

            lst_2.clear();
            EXPECT_EQ( 0, lst_2.size() );
        }

        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::list<uint32_t,ray::pool_alloc> lst;
            lst.push_front( 1 );
            lst.push_back( 2 );
            lst.push_back( 2 );
            lst.push_back( 3 );
            EXPECT_EQ( 4, lst.size() );
            lst.unique();
            EXPECT_EQ( 3, lst.size() );
            lst.remove_if( [](uint32_t x){return x==3;} );
            EXPECT_EQ( 2, lst.size() );

            lst.push_back( 1 );
            lst.push_back( 0 );
            lst.sort();
            for ( auto iter = lst.begin(); iter != lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::list<uint32_t,ray::pool_alloc> &c_lst = lst;
            for ( auto iter = c_lst.begin(); iter != c_lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            lst.reverse();
            for ( auto iter = lst.begin(); iter != lst.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            lst.pop_front();
            lst.pop_back();
            lst.clear();
            EXPECT_EQ( 0, lst.size() );
            EXPECT_TRUE( lst.empty() );

            lst.push_back( 1 );
            lst.push_back( 2 );
            lst.push_back( 3 );
            lst.push_back( 4 );

            ray::list<uint32_t,ray::pool_alloc> lst_1( lst );
            ray::list<uint32_t,ray::pool_alloc> lst_2( lst.begin(), lst.end() );
            lst_1.splice( lst_1.begin(), lst_2 );
            lst_1.sort();
            for ( auto iter = lst_1.begin(); iter != lst_1.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 2*lst.size(), lst_1.size() );
            EXPECT_EQ( 0, lst_2.size() );

            lst_2 = ray::move( lst_1 );
            for ( auto iter = lst_2.begin(); iter != lst_2.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 0, lst_1.size() );
            EXPECT_EQ( 2*lst.size(), lst_2.size() );

            lst_1 = lst_2;
            for ( auto iter = lst_1.begin(); iter != lst_1.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );
            EXPECT_EQ( 2*lst.size(), lst_1.size() );
            EXPECT_EQ( 2*lst.size(), lst_2.size() );

            lst.clear();
            EXPECT_EQ( 0, lst.size() );

            lst_1.clear();
            EXPECT_EQ( 0, lst_1.size() );

            lst_2.clear();
            EXPECT_EQ( 0, lst_2.size() );
        }

        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }
}
#endif /* #if defined(ALL)||defined(BASE_LIST) */