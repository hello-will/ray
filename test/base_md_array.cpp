/**
 * @file    base_md_array.cpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * data     2016/10/28 15:48:20
**/

#if defined(ALL)||defined(BASE_MD_ARRAY)
#include <base/md_array.hpp>
#include <base/normal_alloc.hpp>
#include <base/pool_alloc.hpp>
#include <base/offset_alloc.hpp>
#include <gtest/gtest.h>

TEST( test_base_md_array, normal )
{
    {
        ray::normal_alloc alloc;
        ray::singleton<ray::normal_alloc>::seti( &alloc );
        {
            ray::md_array<int,ray::normal_alloc,16> mr(1);

            mr[0] = 0;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 2 );
            mr[1] = 1;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 4 );
            mr[2] = 2;
            mr[3] = 3;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 8 );
            mr[4] = 4;
            mr[5] = 5;
            mr[6] = 6;
            mr[7] = 7;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 6 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 4 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 2 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.clear();

            ray::md_array<int,ray::normal_alloc,4096> mrx(1);
            for ( auto i = 0; i < 100; ++i )
            {
                auto s = rand() % 16777216;
                mrx.resize( s );
                for ( auto j = 0; j < s; ++j )
                {
                    mrx[j] = j;
                }
            }
        }

        ray::singleton<ray::normal_alloc>::seti( nullptr );
    }

    {
        ray::pool_alloc alloc;
        ray::singleton<ray::pool_alloc>::seti( &alloc );
        {
            ray::md_array<int,ray::pool_alloc,16> mr(1);

            mr[0] = 0;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 2 );
            mr[1] = 1;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 4 );
            mr[2] = 2;
            mr[3] = 3;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 8 );
            mr[4] = 4;
            mr[5] = 5;
            mr[6] = 6;
            mr[7] = 7;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 6 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 4 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 2 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.clear();

            ray::md_array<int,ray::pool_alloc,4096> mrx(1);
            for ( auto i = 0; i < 100; ++i )
            {
                auto s = rand() % 16777216;
                mrx.resize( s );
                for ( auto j = 0; j < s; ++j )
                {
                    mrx[j] = j;
                }
            }
        }

        ray::singleton<ray::pool_alloc>::seti( nullptr );
    }

    {
        ray::offset_alloc alloc;
        char *buf = new char[256*1024*1024];
        alloc.init(buf,256*1024*1024);

        ray::singleton<ray::offset_alloc>::seti( &alloc );
        {
            ray::md_array<int,ray::offset_alloc,16> mr(1);

            mr[0] = 0;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 2 );
            mr[1] = 1;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 4 );
            mr[2] = 2;
            mr[3] = 3;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 8 );
            mr[4] = 4;
            mr[5] = 5;
            mr[6] = 6;
            mr[7] = 7;
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 6 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 4 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.resize( 2 );
            for ( auto i = 0; i < mr.size(); ++i )
            {
                printf( "%d ", mr[i] );
            }
            printf( "\n" );

            mr.clear();

            ray::md_array<int,ray::offset_alloc,4096> mrx(1);
            for ( auto i = 0; i < 100; ++i )
            {
                auto s = rand() % 16777216;
                mrx.resize( s );
                for ( auto j = 0; j < s; ++j )
                {
                    mrx[j] = j;
                }
            }
        }

        ray::singleton<ray::offset_alloc>::seti( nullptr );
        alloc.close();
        delete []buf;
    }
}
#endif /* #if defined(ALL)||defined(BASE_MD_ARRAY) */