/**
 * @file    hash_fun.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * data     2016/10/27 16:17:52
**/

#ifndef __BASE_HASH_FUN_HPP_20161027161752_96DC8FFB_CB72_4D69_9B32_382CCB1C3A23__
#define __BASE_HASH_FUN_HPP_20161027161752_96DC8FFB_CB72_4D69_9B32_382CCB1C3A23__

#include <base/error.hpp>
#include <base/config.hpp>

namespace ray
{
    template <typename __key>
    struct hash { };

    template<>
    struct hash<char>
    {
        size_t operator () ( char x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<unsigned char>
    {
        size_t operator () ( unsigned char x ) const
        {
            return x;
        }        
    };

    template<>
    struct hash<short>
    {
        size_t operator () ( short x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<unsigned short>
    {
        size_t operator () ( unsigned short x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<int>
    {
        size_t operator () ( int x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<unsigned int>
    {
        size_t operator () ( unsigned int x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<long>
    {
        size_t operator () ( long x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<unsigned long>
    {
        size_t operator () ( unsigned long x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<long long>
    {
        size_t operator () ( long long x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<unsigned long long>
    {
        size_t operator () ( unsigned long long x ) const
        {
            return x;
        }
    };

    template<>
    struct hash<void*>
    {
        size_t operator () ( const void *x ) const
        {
            union
            {
                const void *p;
                size_t      s;
            } t = {x};

            return t.s;
        }
    };

    template<>
    struct hash<const void*>
    {
        size_t operator () ( const void *x ) const
        {
            union
            {
                const void *p;
                size_t      s;
            } t = {x};

            return t.s;
        }
    };

    template<>
    struct hash<char*>
    {
        size_t operator () ( const char *s ) const
        {
            size_t h = 0;

            for ( ; *s; ++s )
            {
                h = 5*h + *s;
            }

            return h;
        }
    };

    template<>
    struct hash<const char*>
    {
        size_t operator () ( const char *s ) const
        {
            size_t h = 0;

            for ( ; *s; ++s )
            {
                h = 5*h + *s;
            }

            return h;
        }
    };
} /* namespace ray */

#endif /* __BASE_HASH_FUN_HPP_20161027161752_96DC8FFB_CB72_4D69_9B32_382CCB1C3A23__ */
