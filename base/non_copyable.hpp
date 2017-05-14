/**
 * @file    non_copyable.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/21 12:01:51
**/

#ifndef __BASE_NON_COPYABLE_HPP_20160921120241_A7B6DD29_9A34_4BEC_87C7_431656EA4922__
#define __BASE_NON_COPYABLE_HPP_20160921120241_A7B6DD29_9A34_4BEC_87C7_431656EA4922__

#include <base/error.hpp>
#include <base/config.hpp>

namespace ray
{
    class non_copyable
    {
    protected:
        non_copyable( void ) = default;
        ~non_copyable( void ) = default;

        non_copyable( const non_copyable& ) = delete;
        non_copyable& operator = ( const non_copyable& ) = delete;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_NON_COPYABLE_HPP_20160921120241_A7B6DD29_9A34_4BEC_87C7_431656EA4922__ */