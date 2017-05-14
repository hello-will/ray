/**
 * @file    utils.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/21 16:19:06
**/

#ifndef __BASE_UTILS_HPP_20160921161918_D1268FCA_9F7E_4452_BED6_1253BD340F4A__
#define __BASE_UTILS_HPP_20160921161918_D1268FCA_9F7E_4452_BED6_1253BD340F4A__

#include <base/error.hpp>
#include <base/config.hpp>

namespace ray
{
    /**
     * aligned size
    **/
    inline size_t aligned( size_t sz, size_t align )
    {
        return (sz + align - 1) & ~(align - 1);
    }

    /**
     * minimun
    **/
    template <typename __type>
    inline const __type& min( const __type &x, const __type &y )
    {
        if ( x < y )
        {
            return x;
        }

        return y;
    }

    /**
     * maximun
    **/
    template <typename __type>
    inline const __type& max( const __type &x, const __type &y )
    {
        if ( x > y )
        {
            return x;
        }

        return y;
    }

    /**
     * swap
    **/
    template <typename __type>
    inline void swap( __type &x, __type &y )
    {
        auto t = x;

        x = y;
        y = t;
    }

    /**
     * remove const
    **/
    template <typename __type>
    struct remove_const
    {
        typedef __type type;
    };
    template <typename __type>
    struct remove_const<const __type>
    {
        typedef __type type;
    };
    template <typename __type>
    struct remove_const<const __type[]>
    {
        typedef __type type[];
    };
    template <typename __type, unsigned int __size>
    struct remove_const<const __type[__size]>
    {
        typedef __type type[__size];
    };

    /**
     * remove reference
    **/
    template <typename __type>
    struct remove_reference
    {
        typedef __type type;
    };
    template <typename __type>
    struct remove_reference<__type&>
    {
        typedef __type type;
    };
    template <typename __type>
    struct remove_reference<__type&&>
    {
        typedef __type type;
    };

    /**
     * move
    **/
    template <typename __type>
    inline typename remove_reference<__type>::type&& move( __type &&x )
    {
        return (typename remove_reference<__type>::type&&)(x);
    }


    /**
     * less
    **/
    template <typename __type>
    struct less
    {
        bool operator () ( const __type &x, const __type &y ) const
        {
            return x < y;
        }
    };

    /**
     * great
    **/
    template <typename __type>
    struct great
    {
        bool operator () ( const __type &x, const __type &y ) const
        {
            return y < x;
        }
    };

    /**
     * equal
    **/
    template <typename __type>
    struct equal
    {
        bool operator () ( const __type &x, const __type &y ) const
        {
            return x == y;
        }
    };

    /**
     * not less
    **/
    template <typename __type>
    struct not_less
    {
        bool operator () ( const __type &x, const __type &y ) const
        {
            return !(x<y);
        }
    };

    /**
     * not great
    **/
    template <typename __type>
    struct not_great
    {
        bool operator () ( const __type &x, const __type &y ) const
        {
            return !(y<x);
        }
    };

    /**
     * not equal
    **/
    template <typename __type>
    struct not_equal
    {
        bool operator () ( const __type &x, const __type &y ) const
        {
            return !(x==y);
        }
    };

} /* namespace ray */

#endif /* #ifndef __BASE_UTILS_HPP_20160921161918_D1268FCA_9F7E_4452_BED6_1253BD340F4A__ */
