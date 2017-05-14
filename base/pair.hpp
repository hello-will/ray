/**
 * @file    pair.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/28 11:03:57
**/

#ifndef __BASE_PAIR_HPP_20160928110407_C42D9044_5A52_4D16_BECC_D47F62BC5565__
#define __BASE_PAIR_HPP_20160928110407_C42D9044_5A52_4D16_BECC_D47F62BC5565__

#include <base/utils.hpp>

namespace ray
{
    template <typename __first, typename __second>
    struct pair
    {
        typedef __first  first_type;
        typedef __second second_type;

        first_type  first;
        second_type second;

        pair( void )
            : first( first_type() )
            , second( second_type() )
        { }

        pair( const first_type &f, const second_type &s )
            : first( f )
            , second( s )
        { }

        pair( first_type &&f, const second_type &s )
            : first( ray::move(f) )
            , second( s )
        { }

        pair( const first_type &f, second_type &&s )
            : first( f )
            , second( ray::move(s) )
        { }

        pair( first_type &&f, second_type &&s )
            : first( ray::move(f) )
            , second( ray::move(s) )
        { }

        template <typename __ofirst, typename __osecond>
        pair( const pair<__ofirst,__osecond> &x )
            : first( x.first )
            , second( x.second )
        { }

        template <typename __ofirst, typename __osecond>
        pair( const pair<__ofirst,__osecond> &&x )
            : first( ray::move(x.first) )
            , second( ray::move(x.second) )
        { }
    };

    template <typename __first, typename __second>
    inline bool operator == ( const pair<__first,__second> &x, const pair<__first,__second> &y )
    {
        return x.first == y.first && x.second == y.second;
    }

    template <typename __first, typename __second>
    inline bool operator < ( const pair<__first,__second> &x, const pair<__first,__second> &y )
    {
        return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
    }

    template <typename __first, typename __second>
    inline bool operator != ( const pair<__first,__second> &x, const pair<__first,__second> &y )
    {
        return !(x == y);
    }

    template <typename __first, typename __second>
    inline bool operator > ( const pair<__first,__second> &x, const pair<__first,__second> &y )
    {
        return y < x;
    }

    template <typename __first, typename __second>
    inline bool operator <= ( const pair<__first,__second> &x, const pair<__first,__second> &y )
    {
        return !(y < x);
    }

    template <typename __first, typename __second>
    inline bool operator >= ( const pair<__first,__second> &x, const pair<__first,__second> &y )
    {
        return !(x < y);
    }

    template <typename __first, typename __second>
    inline pair<__first,__second> make_pair( const __first &f, const __second &s )
    {
        return ray::move( pair<__first,__second>(f,s) );
    }

    template <typename __first, typename __second>
    inline pair<__first,__second> make_pair( __first &&f, const __second &s )
    {
        return ray::move( pair<__first,__second>(ray::move(f),s) );
    }

    template <typename __first, typename __second>
    inline pair<__first,__second> make_pair( const __first &f, __second &&s )
    {
        return ray::move( pair<__first,__second>(f,ray::move(s)) );
    }

    template <typename __first, typename __second>
    inline pair<__first,__second> make_pair( __first &&f, __second &&s )
    {
        return ray::move( pair<__first,__second>(ray::move(f),ray::move(s)) );
    }

    template <typename __first, typename __second,int __size>
    inline pair<const __first*,__second> make_pair( const __first (&f)[__size], const __second &s )
    {
        return ray::move( pair<const __first*,__second>((const __first*)f,s) );
    }

    template <typename __first, typename __second,int __size>
    inline pair<const __first*,__second> make_pair( __first const (&f)[__size], __second &&s )
    {
        return ray::move( pair<const __first*,__second>((const __first*)f,ray::move(s)) );
    }

    template <typename __first, typename __second,int __size>
    inline pair<__first,const __second*> make_pair( const __first &f, const __second (&s)[__size] )
    {
        return ray::move( pair<__first const*,__second>(f,(const __second*)s) );
    }

    template <typename __first, typename __second,int __size>
    inline pair<__first,const __second*> make_pair( __first &&f, const __second (&s)[__size] )
    {
        return ray::move( pair<__first const*,__second>(ray::move(f),(const __second*)s) );
    }

    template <typename __first, typename __second,int __fsize, int __ssize>
    inline pair<const __first*,const __second*> make_pair( __first const (&f)[__fsize], const __second (&s)[__ssize] )
    {
        return ray::move( pair<__first const*,__second>((const __first*)f,(const __second*)s) );
    }
} /* namespace ray */

#endif /* #ifndef __BASE_PAIR_HPP_20160928110407_C42D9044_5A52_4D16_BECC_D47F62BC5565__ */