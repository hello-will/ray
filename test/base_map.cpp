/**
 * @file    base_map.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/30 11:28:08
**/

#if defined(ALL)||defined(BASE_MAP)
#include <base/normal_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/map.hpp>
#include <gtest/gtest.h>

TEST( test_base_map, normal )
{
    {
        uint8_t buf[1024*1024];
        ray::offset_alloc alloc;

        EXPECT_EQ( RAY_OK, alloc.init( buf, sizeof(buf) ) );
        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::map<int,int,ray::less<int>,ray::offset_alloc> mp;

            mp[1] = 1;
            mp[2] = 2;
            mp[3] = 3;
            mp.insert( ray::make_pair(1,1) );
            mp.insert( ray::make_pair(2,2) );
            mp.insert( ray::make_pair(3,3) );
            EXPECT_EQ( 3, mp.size() );

            for ( auto iter = mp.begin(); iter != mp.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", iter->second );
            }
            printf( "\n" );

            const ray::map<int,int,ray::less<int>,ray::offset_alloc> &c_mp = mp;
            for ( auto iter = c_mp.begin(); iter != c_mp.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", iter->second );
            }
            printf( "\n" );

            mp.erase( 1 );
            EXPECT_EQ( 2, mp.size() );
            mp.erase( 2 );
            EXPECT_EQ( 1, mp.size() );
            mp.erase( 3 );
            EXPECT_EQ( 0, mp.size() );

            mp[1] = 1;
            mp[2] = 2;
            mp[3] = 3;
            mp.insert( ray::make_pair(1,1) );
            mp.insert( ray::make_pair(2,2) );
            mp.insert( ray::make_pair(3,3) );
            EXPECT_EQ( 3, mp.size() );

            ray::map<int,int,ray::less<int>,ray::offset_alloc> mp_1( mp );
            EXPECT_EQ( 3, mp_1.size( ) );
            EXPECT_EQ( 3, mp.size() );

            ray::map<int,int,ray::less<int>,ray::offset_alloc> mp_2( ray::move(mp_1) );
            EXPECT_EQ( 3, mp_2.size() );
            EXPECT_EQ( 0, mp_1.size() );

            for ( auto iter = mp_2.begin(); iter != mp_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter).second );
            }
            printf( "\n" );

            mp_1 = ray::move( mp_2 );
            EXPECT_EQ( 3, mp_1.size() );
            EXPECT_EQ( 0, mp_2.size() );

            mp_2 = mp_1;
            EXPECT_EQ( 3, mp_2.size() );

            mp.clear();
            EXPECT_EQ( 0, mp.size() );

            mp_1.clear();
            EXPECT_EQ( 0, mp_1.size() );

            mp_2.clear();
            EXPECT_EQ( 0, mp_2.size() );
        }

        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
    }

    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::map<int, int, ray::less<int>, ray::normal_alloc> mp;

            mp[1] = 1;
            mp[2] = 2;
            mp[3] = 3;
            mp.insert( ray::make_pair(1,1) );
            mp.insert( ray::make_pair(2,2) );
            mp.insert( ray::make_pair(3,3) );
            EXPECT_EQ( 3, mp.size() );

            for ( auto iter = mp.begin(); iter != mp.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", iter->second );
            }
            printf( "\n" );

            const ray::map<int, int, ray::less<int>, ray::normal_alloc> &c_mp = mp;
            for ( auto iter = c_mp.begin(); iter != c_mp.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", iter->second );
            }
            printf( "\n" );

            mp.erase( 1 );
            EXPECT_EQ( 2, mp.size() );
            mp.erase( 2 );
            EXPECT_EQ( 1, mp.size() );
            mp.erase( 3 );
            EXPECT_EQ( 0, mp.size() );

            mp[1] = 1;
            mp[2] = 2;
            mp[3] = 3;
            mp.insert( ray::make_pair(1,1) );
            mp.insert( ray::make_pair(2,2) );
            mp.insert( ray::make_pair(3,3) );
            EXPECT_EQ( 3, mp.size() );

            ray::map<int, int, ray::less<int>, ray::normal_alloc> mp_1( mp );
            EXPECT_EQ( 3, mp_1.size() );
            EXPECT_EQ( 3, mp.size() );

            ray::map<int, int, ray::less<int>, ray::normal_alloc> mp_2( ray::move( mp_1 ) );
            EXPECT_EQ( 3, mp_2.size() );
            EXPECT_EQ( 0, mp_1.size() );

            for ( auto iter = mp_2.begin(); iter != mp_2.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter).second );
            }
            printf( "\n" );

            mp_1 = ray::move( mp_2 );
            EXPECT_EQ( 3, mp_1.size() );
            EXPECT_EQ( 0, mp_2.size() );

            mp_2 = mp_1;
            EXPECT_EQ( 3, mp_2.size() );

            mp.clear();
            EXPECT_EQ( 0, mp.size() );

            mp_1.clear();
            EXPECT_EQ( 0, mp_1.size() );

            mp_2.clear();
            EXPECT_EQ( 0, mp_2.size() );
        }

        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::map<int,int,ray::less<int>,ray::pool_alloc> mp;

            mp[1] = 1;
            mp[2] = 2;
            mp[3] = 3;
            mp.insert( ray::make_pair(1,1) );
            mp.insert( ray::make_pair(2,2) );
            mp.insert( ray::make_pair(3,3) );
            EXPECT_EQ( 3, mp.size() );

            for ( auto iter = mp.begin(); iter != mp.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", iter->second );
            }
            printf( "\n" );

            const ray::map<int,int,ray::less<int>,ray::pool_alloc> &c_mp = mp;
            for ( auto iter = c_mp.begin(); iter != c_mp.end(); ++iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                (*iter);
                printf( " %d", iter->second );
            }
            printf( "\n" );

            mp.erase( 1 );
            EXPECT_EQ( 2, mp.size() );
            mp.erase( 2 );
            EXPECT_EQ( 1, mp.size() );
            mp.erase( 3 );
            EXPECT_EQ( 0, mp.size() );

            mp[1] = 1;
            mp[2] = 2;
            mp[3] = 3;
            mp.insert( ray::make_pair(1,1) );
            mp.insert( ray::make_pair(2,2) );
            mp.insert( ray::make_pair(3,3) );
            EXPECT_EQ( 3, mp.size() );

            ray::map<int,int,ray::less<int>,ray::pool_alloc> mp_1( mp );
            EXPECT_EQ( 3, mp_1.size( ) );
            EXPECT_EQ( 3, mp.size() );

            ray::map<int,int,ray::less<int>,ray::pool_alloc> mp_2( ray::move(mp_1) );
            EXPECT_EQ( 3, mp_2.size() );
            EXPECT_EQ( 0, mp_1.size() );

            for ( auto iter = mp_2.begin(); iter != mp_2.end(); ++ iter )
            {
                ++iter;
                --iter;
                iter++;
                iter--;
                printf( " %d", (*iter).second );
            }
            printf( "\n" );

            mp_1 = ray::move( mp_2 );
            EXPECT_EQ( 3, mp_1.size() );
            EXPECT_EQ( 0, mp_2.size() );

            mp_2 = mp_1;
            EXPECT_EQ( 3, mp_2.size() );

            mp.clear();
            EXPECT_EQ( 0, mp.size() );

            mp_1.clear();
            EXPECT_EQ( 0, mp_1.size() );

            mp_2.clear();
            EXPECT_EQ( 0, mp_2.size() );
        }

        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }
}
#endif /* #if defined(ALL)||defined(BASE_MAP) */