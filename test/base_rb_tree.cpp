/**
 * @file    base_offset_alloc.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 11:07:24
**/

#if defined(ALL)||defined(BASE_RB_TREE)
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/rb_tree.hpp>
#include <gtest/gtest.h>

template <typename __type>
struct comp
{
    bool operator()( const __type &x, const __type &y ) const
    {
        return x < y;
    }
};

template <typename __type>
struct keyof
{
    const __type& operator()( const __type &x ) const
    {
        return x;
    }
};

TEST( test_base_rb_tree, normal )
{
    {
        uint8_t buf[10240];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init( buf, sizeof(buf) ) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::offset_alloc> rt;

            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            EXPECT_EQ( 4, rt.size() );

            for ( auto iter = rt.begin(); iter != rt.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::offset_alloc> &c_rt = rt;
            for ( auto iter = c_rt.begin(); iter != c_rt.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt.erase( 1 );
            EXPECT_EQ( 2, rt.size() );
            rt.erase( 2 );
            EXPECT_EQ( 0, rt.size() );

            rt.insert_unique( 1 );
            rt.insert_unique( 2 );
            rt.insert_unique( 1 );
            rt.insert_unique( 2 );
            EXPECT_EQ( 2, rt.size() );

            for ( auto iter = rt.begin(); iter != rt.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            EXPECT_EQ( 4, rt.size() );

            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::offset_alloc> rt_1( rt );
            EXPECT_EQ( 4, rt_1.size() );
            EXPECT_EQ( 4, rt.size() );

            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::offset_alloc> rt_2( ray::move(rt_1) );
            EXPECT_EQ( 4, rt_2.size() );
            EXPECT_EQ( 0, rt_1.size() );

            for ( auto iter = rt_2.begin(); iter != rt_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt_1 = ray::move( rt_2 );
            rt_2 = rt_1;
            EXPECT_EQ( 4, rt_1.size() );
            EXPECT_EQ( 4, rt_2.size() );

            rt.erase_unique( 1 );
            EXPECT_EQ( 3, rt.size() );

            auto r = rt.equal_range_unique( 2 );
            rt.erase( r.first, r.second );
            EXPECT_EQ( 2, rt.size() );

            rt.clear();
            EXPECT_EQ( 0, rt.size() );

            rt_1.clear();
            EXPECT_EQ( 0, rt_1.size() );

            rt_2.clear();
            EXPECT_EQ( 0, rt_2.size() );
        }

        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
    }

    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::normal_alloc> rt;

            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            EXPECT_EQ( 4, rt.size() );

            for ( auto iter = rt.begin(); iter != rt.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::normal_alloc> &c_rt = rt;
            for ( auto iter = c_rt.begin(); iter != c_rt.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt.erase( 1 );
            EXPECT_EQ( 2, rt.size() );
            rt.erase( 2 );
            EXPECT_EQ( 0, rt.size() );

            rt.insert_unique( 1 );
            rt.insert_unique( 2 );
            rt.insert_unique( 1 );
            rt.insert_unique( 2 );
            EXPECT_EQ( 2, rt.size() );

            for ( auto iter = rt.begin(); iter != rt.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            EXPECT_EQ( 4, rt.size() );

            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::normal_alloc> rt_1( rt );
            EXPECT_EQ( 4, rt_1.size() );

            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::normal_alloc> rt_2( ray::move(rt_1) );
            EXPECT_EQ( 4, rt_2.size() );

            for ( auto iter = rt_2.begin(); iter != rt_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt_1 = ray::move( rt_2 );
            rt_2 = rt_1;
            EXPECT_EQ( 4, rt_1.size() );
            EXPECT_EQ( 4, rt_2.size() );

            rt.erase_unique( 1 );
            EXPECT_EQ( 3, rt.size() );

            auto r = rt.equal_range_unique( 2 );
            rt.erase( r.first, r.second );
            EXPECT_EQ( 2, rt.size() );

            rt.clear();
            EXPECT_EQ( 0, rt.size() );

            rt_1.clear();
            EXPECT_EQ( 0, rt_1.size() );

            rt_2.clear();
            EXPECT_EQ( 0, rt_2.size() );
        }

        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::pool_alloc> rt;

            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            EXPECT_EQ( 4, rt.size() );

            for ( auto iter = rt.begin(); iter != rt.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            const ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::pool_alloc> &c_rt = rt;
            for ( auto iter = c_rt.begin(); iter != c_rt.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt.erase( 1 );
            EXPECT_EQ( 2, rt.size() );
            rt.erase( 2 );
            EXPECT_EQ( 0, rt.size() );

            rt.insert_unique( 1 );
            rt.insert_unique( 2 );
            rt.insert_unique( 1 );
            rt.insert_unique( 2 );
            EXPECT_EQ( 2, rt.size() );

            for ( auto iter = rt.begin(); iter != rt.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt.insert_equal( 1 );
            rt.insert_equal( 2 );
            EXPECT_EQ( 4, rt.size() );

            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::pool_alloc> rt_1( rt );
            EXPECT_EQ( 4, rt_1.size() );
            EXPECT_EQ( 4, rt.size() );

            ray::rb_tree<int,ray::less<int>,int,keyof<int>,ray::pool_alloc> rt_2( ray::move(rt_1) );
            EXPECT_EQ( 4, rt_2.size() );
            EXPECT_EQ( 0, rt_1.size() );

            for ( auto iter = rt_2.begin(); iter != rt_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter) );
            }
            printf( "\n" );

            rt_1 = ray::move( rt_2 );
            rt_2 = rt_1;
            EXPECT_EQ( 4, rt_1.size() );
            EXPECT_EQ( 4, rt_2.size() );

            rt.erase_unique( 1 );
            EXPECT_EQ( 3, rt.size() );

            auto r = rt.equal_range_unique( 2 );
            rt.erase( r.first, r.second );
            EXPECT_EQ( 2, rt.size() );

            rt.clear();
            EXPECT_EQ( 0, rt.size() );

            rt_1.clear();
            EXPECT_EQ( 0, rt_1.size() );

            rt_2.clear();
            EXPECT_EQ( 0, rt_2.size() );
        }

        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }
}
#endif /* #if defined(ALL)||defined(BASE_RB_TREE) */